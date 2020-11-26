
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
#include <stdlib.h>

using std::endl;
using std::ofstream;
using std::wofstream;
using std::wstring;
ofstream log_file("log.txt");

LPVOID real_func_address;
int len_override;


void change_str(LPWSTR str) {
    //while (IsDebuggerPresent() == false) { Sleep(1); }
    str[0] = L'1';
}

__declspec(naked) void DrawTextHook()
{
    __asm {
        ; do something here
        mov eax, [esp+8]
        push eax
        call change_str
        pop eax
 
        ; restore the commands overriden
        mov edi, edi
        push ebp
        mov ebp,esp


        ; jump back
        mov eax, real_func_address
        mov ecx, len_override
        add eax, ecx
        jmp eax
    }
}

void setHook()
{

    LPVOID f;
    HMODULE h = GetModuleHandle(L"user32.dll");
    CHAR JmpOpcode[7] = "\xE9\x90\x90\x90\x90\x90";
    DWORD lpProtect = 0;
    LPVOID JumpTo;

    if (h == NULL)
    {
        log_file << "couldnt get handle" << endl;
        return;
    }
    f = GetProcAddress(h, "DrawTextW");
    // alternatively, get the function address by offset:
    // f = (char*)h + offset_to_func
    if (f == NULL)
    {
        log_file << "couldnt get function" << endl;
        return;
    }

    // save the return address of f for jumping back
    real_func_address = f;
    len_override = 5; // nubmer of bytes to override, changes from function to function


    log_file << "setting hook" << endl;
    // calculate relative jump to HOOK_NAME from f, add 5 cause its from eip after the execution
    JumpTo = (LPVOID)((char*)&DrawTextHook - ((char*)f + 5));
    memcpy(JmpOpcode + 1, &JumpTo, 0x4); // write the jump
    VirtualProtect((char*)f, 0x7, PAGE_EXECUTE_READWRITE, &lpProtect);
    memcpy((char*)f, &JmpOpcode, len_override);
    VirtualProtect((char*)f, 0x7, PAGE_EXECUTE_READ, &lpProtect);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
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
