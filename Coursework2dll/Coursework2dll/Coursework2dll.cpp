#include "pch.h"
#include "Coursework2dll.h"

#include <windows.h>
#include <thread>
#include <stop_token>
#include <mutex>
#include <string>
#include <cstring>

#include "DataProducer.h"
#include "Control.h"
#include "Item.h"
#include "Entry.h"

using namespace std;

static HMODULE DataProducer = nullptr;

static InitializeFunc Initialize = nullptr;
static StartFunc Start = nullptr;

static jthread producerThread;
static jthread consumerThread;
static stop_source stopSource;

static Control_A producerControl;
static Control_GUI* guiControl = nullptr;

void ConsumerLoop(stop_token st);

bool CW2_Initialize()
{
    DataProducer = LoadLibraryA("DataProducer.dll");

    if (!DataProducer)
        return false;

    Initialize = reinterpret_cast<InitializeFunc>(
        GetProcAddress(DataProducer, "Initialize")
        );

    Start = reinterpret_cast<StartFunc>(
        GetProcAddress(DataProducer, "Start")
        );

    if (!Initialize || !Start)
        return false;

    try
    {
        Initialize();
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool CW2_Start(Control_GUI* gui)
{
    try
    {
        if (!Start || !gui)
            return false;

        guiControl = gui;

        {
            lock_guard<mutex> lock(guiControl->mx);
            guiControl->stop = false;
            guiControl->buffer.str("");
            guiControl->buffer.clear();
        }

        stopSource = stop_source();
        stop_token token = stopSource.get_token();

        producerControl.stop = token;

        producerThread = jthread([]()
            {
                Start('A', &producerControl);
            });

        consumerThread = jthread([token]()
            {
                ConsumerLoop(token);
            });

        return true;
    }
    catch (...)
    {
        return false;
    }
}

void CW2_Stop()
{
    stopSource.request_stop();

    producerControl.cva.notify_all();

    if (guiControl)
    {
        {
            lock_guard<mutex> lock(guiControl->mx);
            guiControl->stop = true;
        }

        guiControl->cv.notify_all();
    }

    if (producerThread.joinable())
        producerThread.join();

    if (consumerThread.joinable())
        consumerThread.join();
}

void CW2_Shutdown()
{
    CW2_Stop();

    if (DataProducer)
    {
        FreeLibrary(DataProducer);
        DataProducer = nullptr;
    }

    Initialize = nullptr;
    Start = nullptr;
}

void ConsumerLoop(stop_token st)
{
    while (!st.stop_requested())
    {
        unique_lock<mutex> lock(producerControl.mx);

        producerControl.cva.wait(lock, st, [&]()
            {
                return !producerControl.Items.empty() || st.stop_requested();
            });

        if (st.stop_requested())
            break;

        while (!producerControl.Items.empty())
        {
            Item* item = producerControl.Items.front();
            producerControl.Items.pop();

            lock.unlock();

            Entry entry = *item;

            if (guiControl)
            {
                {
                    lock_guard<mutex> guiLock(guiControl->mx);
                    guiControl->buffer << entry << "\n";
                }

                guiControl->cv.notify_one();
            }

            delete item;

            lock.lock();
        }

        producerControl.cva.notify_one();
    }
}