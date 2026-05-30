#pragma once
#include <mutex>
#include <condition_variable>
#include <sstream>

using namespace std;

class Control_GUI
{
public:
    mutex mx;
    condition_variable cv;
    stringstream buffer;
    bool stop = false;
};

#ifdef COURSEWORK2DLL_EXPORTS
#define COURSEWORK2DLL_API extern "C" __declspec(dllexport)
#else
#define COURSEWORK2DLL_API extern "C" __declspec(dllimport)
#endif

COURSEWORK2DLL_API bool CW2_Initialize();

COURSEWORK2DLL_API bool CW2_Start(Control_GUI* gui);

COURSEWORK2DLL_API void CW2_Stop();

COURSEWORK2DLL_API void CW2_Shutdown();
