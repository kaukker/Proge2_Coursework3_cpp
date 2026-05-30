#pragma once

#ifdef COURSEWORK2DLL_EXPORTS
#define DLLSPEC extern "C" __declspec(dllexport)
#else
#define DLLSPEC extern "C" __declspec(dllimport)
#endif

class Control_GUI;

DLLSPEC bool CW2_Initialize();
DLLSPEC bool CW2_Start(Control_GUI* gui);
DLLSPEC void CW2_Stop();
DLLSPEC void CW2_Shutdown();

