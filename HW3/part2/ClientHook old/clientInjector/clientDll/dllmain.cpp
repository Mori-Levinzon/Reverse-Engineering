// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
// Hooking template @Michael Ezra. Good luck in the test :)
// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>

void MichaelSetHookRegular();
void MichaelSetHookHotPatching();
void MichaelSetIATHook();


// Runs when the process load the dll 
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		///he DLL is being loadedas a result of the process starting up or as a result of a call to LoadLibrary function
	case DLL_PROCESS_ATTACH:

		//<INSTRUCTION>: place here the set hook function you want to use.
		MichaelSetIATHook();

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
