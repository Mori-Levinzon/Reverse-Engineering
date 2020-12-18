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


using std::ofstream;
using std::endl;
using std::wstring;

ofstream log_file("log.txt");


__declspec(naked) void textColorHook()
{
    __asm {
        mov eax, [esp + 0x20]; Some �random� value from the stack.
        and eax, 0x00FFFFFF
        mov[esp + 0x8], eax; COLORREF
        ; SetTextColor(_In_��HDC hdc, ; _In_��COLORREF crColor);
        nop
        nop; Jump to the real SetTextColor
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
    }
}

void setHook() {

    LPVOID f;
    HMODULE h = GetModuleHandle(L"gdi32.dll");
    CHAR JmpOpcode[6] = "\xE9\x90\x90\x90\x90";
    CHAR JmpOpcode2[8] = "\xB8\x90\x90\x90\x90\xFF\xE0";
    DWORD lpProtect = 0;
    LPVOID JumpTo;

    if (h == NULL) {
        log_file << "ColorDll: couldnt get handle" << endl;
        return;
    }
    f = GetProcAddress(h, "SetTextColor");

    if (f == NULL) { 
        log_file << "ColorDll: couldnt get function" << endl; 
        return; }
    log_file << "ColorDll: setting hook" << endl;
    // calculate relative jump to setTextHook from f	
    JumpTo = (LPVOID)((char*)&textColorHook - (char*)f);
    memcpy(JmpOpcode + 1, &JumpTo, 0x4); // write the jump 	
    VirtualProtect((char*)f - 5, 0x7, PAGE_EXECUTE_READWRITE, &lpProtect);
    memcpy((char*)f - 5, &JmpOpcode, 0x5); // override the first two bytes with jmp short 
    *(char*)f = 0xEB;
    *((char*)(f)+1) = 0xf9;

    VirtualProtect((char*)f - 5, 0x7, PAGE_EXECUTE_READ, &lpProtect);
    // jump to f from setTextHook 
    JumpTo = (LPVOID)((char*)f + 2);
    memcpy(JmpOpcode2 + 1, &JumpTo, 0x4);
    VirtualProtect((char*)textColorHook, 0x100, PAGE_EXECUTE_READWRITE, &lpProtect);
    memcpy((char*)&textColorHook + 0x1f, &JmpOpcode2, 0x7);
    VirtualProtect((char*)textColorHook, 0x100, PAGE_EXECUTE_READ, &lpProtect);
}

BOOL APIENTRY DllMain( HMODULE hModule,
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

