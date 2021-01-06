// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <string>
#include <algorithm>
#include <WinSock2.h>
using namespace std;

using std::ofstream;
using std::endl;
using std::wstring;


int times = 0;

// change this to the proper signature of the hook
typedef INT(WINAPI* HOOK_TYPE)(const char*, const char*);

HOOK_TYPE orig_strcmp;


INT RecvHook(const char* string1, const char* string2) {
	//int res = orig_strcmp(string1,string2);


	if (times == 0) {
		times++;
		return 1;
	}
	if (times == 1) {
		times++;
		return 0;
	}
	return strcmp(string1, string2);

}


void setHook() {



	HMODULE curr_prog = GetModuleHandle(NULL); // get the handle of the main process
	HMODULE target_dll = GetModuleHandle(L"msvcrt.dll"); // get the handle of the dll conatining the function we want to override
	DWORD lpProtect = 0;
	DWORD IAT_Func_Offset = 0x9294;
	LPVOID JumpTo;
	LPDWORD IAT_ADDRESS;

	if ((curr_prog == NULL) || (target_dll == NULL)) {
		return;
	}

	orig_strcmp = (HOOK_TYPE)GetProcAddress(target_dll, "strcmp");
	if (orig_strcmp == NULL) {
		return;
	};
	IAT_ADDRESS = (LPDWORD)(curr_prog + IAT_Func_Offset / 4);
	if ((*IAT_ADDRESS) != (DWORD)orig_strcmp) {
		return;
	}

	JumpTo = (LPVOID)((char*)&RecvHook);
	VirtualProtect(IAT_ADDRESS, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy(IAT_ADDRESS, &JumpTo, 0x4);
	VirtualProtect(IAT_ADDRESS, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);


}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		setHook();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
