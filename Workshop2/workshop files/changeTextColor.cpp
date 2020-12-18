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


using std::ofstream;
using std::endl;
using std::wstring;
using std::wofstream;
ofstream log_file("log.txt");

LPVOID real_func_address;
int len_override;


COLORREF get_color() {
    //while (!IsDebuggerPresent()) { Sleep(1); }
    return RGB(0, 255, 0);
}

void log_index(int index) {
    log_file << index << endl;
}

__declspec(naked) void SetGetSysColorHook()
{
    __asm {
        ; call get_color
        mov eax, [esp + 4]; get nIndex from the stack
  
        cmp eax, COLOR_WINDOWTEXT
        jne return_to_func
        ; get rid of the arguments cause its __stdcall but dont change the ret address
        pop eax
        pop edx
        push eax 

        call get_color
        ret


        return_to_func:
                    ; restore the commands
        mov edi, edi; not neccesery but whatever
        push ebp
        mov ebp, esp

        ; jump back
        mov eax, real_func_address
        mov ecx, len_override
        add eax, ecx
        jmp eax
    }
}

void setHook() {

    LPVOID f;
    HMODULE h = GetModuleHandle(L"user32.dll");
    CHAR JmpOpcode[7] = "\xE9\x90\x90\x90\x90\x90";
    DWORD lpProtect = 0;
    LPVOID JumpTo;

    if (h == NULL) {
        log_file << "GetSysColor: couldnt get handle" << endl;
        return;
    }
    f = GetProcAddress(h, "GetSysColor");

    if (f == NULL) {
        log_file << "GetSysColor: couldnt get function" << endl;
        return;
    }

    // save the return address of f for jumping back 
    real_func_address = f;

    log_file << "GetSysColor: setting hook" << endl;
    // calculate relative jump to DrawTextHook from f, add 5 cause its from eip after the execution
    JumpTo = (LPVOID)((char*)&SetGetSysColorHook - ((char*)f + 5));
    memcpy(JmpOpcode + 1, &JumpTo, 0x4); // write the jump 	
    VirtualProtect((char*)f, 0x7, PAGE_EXECUTE_READWRITE, &lpProtect);
    len_override = 5; // nubmer of bytes to override, changes from function to function
    memcpy((char*)f, &JmpOpcode, len_override);
    VirtualProtect((char*)f, 0x7, PAGE_EXECUTE_READ, &lpProtect);


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
