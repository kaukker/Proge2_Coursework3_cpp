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

void ConsumerLoop();

// Producer DLL'i laadimine ja initsialiseerimine
bool CW2_Initialize()
{
    DataProducer = LoadLibraryA("DataProducer.dll"); // Laeb DataProducer.dll

    // Kontroll kas DLL'i laadimine oli edukas
    if (!DataProducer)
    {
        return false;
    }

    // Hangi DLL-ist funktsioonide aadressid
    Initialize = reinterpret_cast<InitializeFunc>(
        GetProcAddress(DataProducer, "Initialize"));

    Start = reinterpret_cast<StartFunc>(
        GetProcAddress(DataProducer, "Start"));

    // Kontroll kas funktsioonid leiti
    if (!Initialize || !Start)
    {
        FreeLibrary(DataProducer); // Vabastab DLL'i
        DataProducer = nullptr;
        return false;
    }

    try
    {
        Initialize(); // DLL'i initsialiseerimine
    }
    catch (...)
    {
        FreeLibrary(DataProducer);
        DataProducer = nullptr;
        Initialize = nullptr;
        Start = nullptr;
        return false;
    }

    return true;
}

// Käivitab consumer ja produser thread'id
bool CW2_Start(Control_GUI* gui)
{
    try
    {
        if (!Start || !gui)
        {
            return false;
        }

        guiControl = gui;

        {
            lock_guard<mutex> lock(guiControl->mx); // Loob luku ja lukustab mutexi
            guiControl->stop = false;

            // Tühjendab väljundpuhvri
            guiControl->buffer.str("");
            guiControl->buffer.clear();
        }

        stopSource = stop_source(); // Loob stop_source objekti thread'ide peatamiseks
        producerControl.stop = stopSource.get_token(); // Salvestab stop tokeni control objekti sisse

        // Producer thread
        producerThread = jthread([]()
            {
                Start('A', &producerControl); // Käivitab DLL-is item'ite generaatori
            });

        // Consumer thread
        consumerThread = jthread([]()
            {
                ConsumerLoop();
            });

        return true;
    }
    catch (...)
    {
        return false;
    }
}

// Peatab threadid
void CW2_Stop()
{
    stopSource.request_stop(); // Saadab stop-signaali

    // Saadab välja teavitused, et äratada threadid ooteseisundist
    producerControl.cva.notify_all(); 

    if (guiControl)
    {
        {
            lock_guard<mutex> lock(guiControl->mx); // Loob luku ja lukustab mutexi
            guiControl->stop = true;
        }

        // Teavitab GUI's threadi 
        guiControl->cv.notify_all();
    }

    // Ootab producer thread'i lõppu
    if (producerThread.joinable())
    {
        producerThread.join();
    }

    // Ootab consumer thread'i lõppu
    if (consumerThread.joinable())
    {
        consumerThread.join();
    }
}

// Sulgeb süsteemi täielikult
void CW2_Shutdown()
{
    // Peata threadid
    CW2_Stop();

    // Vabastab DLL'i
    if (DataProducer)
    {
        FreeLibrary(DataProducer);
        DataProducer = nullptr;
    }

    Initialize = nullptr;
    Start = nullptr;
}

// Consumer thread'i põhitsükkel
void ConsumerLoop()
{
    // Töötab seni kuni peatmaimis käsku pole saadetud
    while (!producerControl.stop.stop_requested())
    {
        // Loob luku ja lukustab mutexi
        unique_lock<mutex> lock(producerControl.mx);

        // Ootab kuni järjekirda lisatakse elemente
        // või programm peatatatakse
        producerControl.cva.wait(lock, producerControl.stop, [&]()
            {
                return !producerControl.Items.empty();
            });

        // Töötleb kõik järjekorras olevad elemendid
        while (!producerControl.Items.empty())
        {
            // Võtab ja eemaldab esimese elemendi järjekorrast
            Item* item = producerControl.Items.front();
            producerControl.Items.pop();

            lock.unlock(); // Vabastab mutexi

            Entry entry = *item;

            if (guiControl)
            {
                {
                    lock_guard<mutex> guiLock(guiControl->mx);
                    guiControl->buffer << entry << "\n"; // Lisa kirje GUI puhvrisse
                }

                guiControl->cv.notify_one(); // Saadab teate, et puhvirs on andmed
            }

            delete item;

            lock.lock(); // lukustab mutexi
        }

        // Saadab teate, et järjekord on tühi
        producerControl.cva.notify_one();
    }
}