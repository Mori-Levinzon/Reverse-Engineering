// Hooking template @Michael Ezra. Good luck in the test :)
#include <windows.h>
#include <stdio.h>
#include <stack>  
#include "pch.h"

#include "winternl.h"
#include <stdlib.h>
#include <atlstr.h>
#include <string>
#include <stack>

using std::ofstream;
using std::wstring;


//----------------------------System variables: DONT CHANGE IT
DWORD addr; //return address - used as temp variable to store the return address (used by hook function).
std::stack<DWORD> retAddresses; //used the store return the returned address in case of a recursion.
DWORD tmp_register; // Used as a temp register (useb by hook function).
HMODULE h2;


//---------------------------<INSTRUCTION>: Put here your global variables :)

wstring decypherMessage(wstring& input);
// END global variables section


/*
 a hook function that ran before the original function.
 <INSTRUCTION> change it to any signature you want to :)
*/
#pragma optimize( "", off ) //disable compiler optimizations
__declspec(naked) int hook_before(int  n, int y)
{
	//intialize ebp (DONT CHANGE IT)
	__asm {
		push ebp
		mov ebp, esp
	}

	//<INSTRUCTION>: your code section - put here your code:

	//END your code section.

	//restore ebp and ret (DONT CHANGE IT)
	__asm {
		pop ebp
		ret
	}

}
#pragma optimize( "", on ) //enable compiler optimizations


//!!!!!!!DONT CHANGE IT. The hook dynamicly update that function!!!!!!
__declspec(naked) void restoreCmdAndJmpBack() {
	//WHAT ARE U DOING, YOU SHOULD NOT TOUCH THIS FUNCTION
	__asm {
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
		nop
		nop
	}

}
//!!!!!!!DONT CHANGE IT!!!!!!



/*
 a hook function that ran *after* the original function.
 <INSTRUCTION> change it to any signature you want to :)
 Some Notes:
	- Due to the GCC calling conventions - eax holds the output of the original function.
	- any C-code you write here may affect the registers (e.g change eax which lead to different output).
	- you can save the reigsters before and restore them by pusha and popa asm commands.
*/
#pragma optimize( "", off ) //disable compiler optimizations
__declspec(naked) int hook_after(int n, int y)
{


	//intialize ebp (DONT CHANGE IT)
	__asm {
		push ebp
		mov ebp, esp
	}

	//<INSTRUCTION>: your code section - put here your code:
	__asm {
		push eax
		mov eax, [esp + 8]
		push eax
		call decypherMessage
		mov[esp + 8], eax
		pop eax
	}
	//END your code section.


	//restore ebp and ret (DONT CHANGE IT)
	_asm {
		pop ebp
		ret
	}
}
#pragma optimize( "", on ) //disable compiler optimizations


// Main hook function.
__declspec(naked) void hook()
{
	//Part1: save return address - and call to hook_before
	__asm {
		mov tmp_register, eax
		pop eax //pop return address
		mov addr, eax
		mov eax, tmp_register
	}

	//save ret address (registers are stored before using the std stack function,and restore after).
	__asm {
		pusha
	}
	retAddresses.push(addr);
	__asm {
		popa
	}

	//call hook_before
	__asm {
		call hook_before
	}


	//Part2: callback to the original function (and execute the restored commands if needed)
	__asm {
		call restoreCmdAndJmpBack
	}



	//Part3: call hook after function: 
	__asm {
		call hook_after
	}

	//part4: restore ret address and return.
	__asm {
		pusha
	}
	addr = retAddresses.top();
	retAddresses.pop();
	__asm {
		popa
	}


	__asm {
		mov tmp_register, eax
		mov eax, addr
		push eax
		mov eax, tmp_register
		ret
	}
}

wstring DEFcase(const wstring& input, int i) {
	int sum = (input[i] - '0') + (input[i + 2] - '0');
	switch (sum) {
	case 0x0d:   return L"D";
	case 0x0e:   return L"E";
	case 0x0f:   return L"F";
	default:   return L"F";
	}
}


wstring convert_ASCII(wstring hex) {
	wstring ascii = L"";
	for (size_t i = 0; i < hex.length(); i += 2) {
		//taking two characters from hex string
		wstring part = hex.substr(i, 2);
		//changing it into base 16
		char ch = stoul(part, nullptr, 16);
		//putting it into the ASCII string
		ascii += ch;
	}
	return ascii;
}


wstring decypherMessage(wstring& input) {
	printf("enter to the debugger\n");
	while (IsDebuggerPresent() == false) {
		Sleep(1);
	}
	printf("exit the debugger\n");
	wstring output = L"";
	for (int i = 0; i < input.size(); i++) {
		switch (input[i]) {
		case L'A':   output += L"1";//'1'
			break;
		case L'J':   output += L"A";//'A'
			break;
		case L'Q':   output += L"B";//'B'
			break;
		case L'K':   output += L"C";//'C'
			break;
		case L'0':
		case L'1':
		case L'2':
		case L'3':
		case L'4':
		case L'5':
		case L'6':
		case L'7':
		case L'8':
		case L'9':
			if (i < input.size() - 1) {
				if (input[i + 1] == '-') {
					output += L"0";//'0'
					i += 2;
					break;
				}
				else if ((input[i + 1] == '+')) {
					output += DEFcase(input, i);
					i += 2;
					break;
				}
			}
			output += std::to_wstring((((int)(input[i])) - ((int)('0'))));
		}

	}

	return convert_ASCII(output);
}


///---------------------------SET HOOK ----------------------------------
//Assume no hot-patching
void MichaelSetHookRegular() {
	//~~~~~~~~~~~~~~~~~~~~~~~~~<INSTRUCTION>: FILL AND CHANGE the fields (moduleName, toHook function, number of bytes should restored)!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	LPCWSTR moduleName = L"test.exe";
	LPCSTR functionToHookName = "_Z4stamii";
	unsigned int bytesResotre = 0xA; // 0x7 < bytesResotre <= 20 (decimal). We fill these bytes with our jump[7 bytes], and nops[the rest].
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	DWORD recoverdCommandsLoc = 0;
	unsigned int space_for_jmp = 0x5;
	unsigned int space_for_back_jmp = 0x7;
	DWORD jmpBackLoc = 0x30 - space_for_back_jmp;
	CHAR  recoverdCommands[20];
	SIZE_T virtualProtect_callBack_size = 0x40;

	CHAR  JmpOpcode[6] = "\xE9\x90\x90\x90\x90";
	CHAR  JmpBack[8] = "\xB8\x90\x90\x90\x90\xFF\xE0";


	HMODULE h = GetModuleHandle(moduleName);
	if (h == NULL) {
		MessageBoxW(NULL, L"Can't find module", NULL, NULL); //can help on debugging.
		return;
	}
	LPVOID f = GetProcAddress(h, functionToHookName);
	if (f == NULL) {
		MessageBoxW(NULL, L"Can't find function...", NULL, NULL); //can help on debugging.
		return;
	}

	//--------------------------------------changes at functionToHook f ---------------------------

	// Calculate relative jump to hook from f
	LPVOID JumpTo = (LPVOID)((char*)&hook - ((char*)f + 5));  //no hot patching
	memcpy(JmpOpcode + 1, &JumpTo, 0x4);//copy the address

	// change premissions to write the jmp to our hook
	DWORD lpProtect = 0;
	VirtualProtect((char*)f, space_for_jmp, PAGE_EXECUTE_READWRITE, &lpProtect);

	//copy commands before we override them
	for (int i = 0; i < bytesResotre; ++i) {
		recoverdCommands[i] = *((char*)f + i);
		*((char*)f + i) = 0x90;
	}

	//ovverding original function with the jump to our hook
	memcpy((char*)f, &JmpOpcode, 0x5);

	//Restore permissions
	VirtualProtect((char*)f, space_for_jmp, PAGE_EXECUTE_READ, &lpProtect);



	//------------------------------changes at restoreCmdAndJmpBack function ----------------------

	// update jmpBack array
	JumpTo = (LPVOID)((char*)f + 5); // +5 because there is not hot patching 
	memcpy(JmpBack + 1, &JumpTo, 0x4);

	VirtualProtect((char*)restoreCmdAndJmpBack, virtualProtect_callBack_size, PAGE_EXECUTE_READWRITE, &lpProtect);//change permissions

	//resotre commands (write these commands in restoreCmdAndJmpBack function)
	for (int i = 0; i < bytesResotre; ++i)
		*((char*)restoreCmdAndJmpBack + i + recoverdCommandsLoc) = recoverdCommands[i];

	//set the jumpBack
	memcpy((char*)&restoreCmdAndJmpBack + jmpBackLoc, &JmpBack, 0x7);

	VirtualProtect((char*)restoreCmdAndJmpBack, virtualProtect_callBack_size, PAGE_EXECUTE_READ, &lpProtect);//resotre permissions
}



//Hot-patching
void MichaelSetHookHotPatching() {
	//~~~~~~~~~~~~~~~~~~~~~~~~~<INSTRUCTION>: FILL AND CHANGE!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	LPCWSTR moduleName = L"USER32.dll";
	LPCSTR functionToHookName = "SetWindowTextW";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	unsigned int space_for_jmp = 0x5;
	unsigned int space_for_back_jmp = 0x7;
	DWORD jmpBackLoc = 0x30 - space_for_back_jmp;
	SIZE_T virtualProtect_callBack_size = 0x40;

	CHAR  JmpOpcode[6] = "\xE9\x90\x90\x90\x90";
	CHAR  JmpBack[8] = "\xB8\x90\x90\x90\x90\xFF\xE0";


	HMODULE h = GetModuleHandle(moduleName);
	if (h == NULL) {
		MessageBoxW(NULL, L"Can't find module", NULL, NULL); //can help on debugging.
		return;
	}
	LPVOID f = GetProcAddress(h, functionToHookName);
	if (f == NULL) {

		MessageBoxW(NULL, L"Can't find function", NULL, NULL); //can help on debugging.
		return;
	}

	//--------------------------------------changes at functionToHook f ---------------------------

	// calculate relative jump to hook from f
	LPVOID JumpTo = (LPVOID)((char*)&hook - (char*)f);  // hot patching
	memcpy(JmpOpcode + 1, &JumpTo, 0x4);//copy the address

	// change premissions to write the jmp to our hook
	DWORD lpProtect = 0;
	VirtualProtect((char*)f - 5, space_for_jmp + 0x2, PAGE_EXECUTE_READWRITE, &lpProtect);
	//ovverding original function with the jump to our hook
	memcpy((char*)f - 5, &JmpOpcode, 0x5);

	//jmp -7
	*((char*)f) = 0xEB;
	*((char*)(f)+1) = 0xf9;

	//Restore permissions
	VirtualProtect((char*)f - 5, space_for_jmp + 0x2, PAGE_EXECUTE_READ, &lpProtect);



	//------------------------------changes at restoreCmdAndJmpBack function ----------------------

	// update jmpBack array
	JumpTo = (LPVOID)((char*)f + 2); // +2 because there is not hot patching 
	memcpy(JmpBack + 1, &JumpTo, 0x4);

	VirtualProtect((char*)restoreCmdAndJmpBack, virtualProtect_callBack_size, PAGE_EXECUTE_READWRITE, &lpProtect);//change permissions

	//set the jumpBack
	memcpy((char*)&restoreCmdAndJmpBack + jmpBackLoc, &JmpBack, 0x7);

	VirtualProtect((char*)restoreCmdAndJmpBack, virtualProtect_callBack_size, PAGE_EXECUTE_READ, &lpProtect);//resotre permissions
}


//IAT hook
void MichaelSetIATHook() {
	//~~~~~~~~~~~~~~~~~~~~~~~~~<INSTRUCTION>: FILL AND CHANGE!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	LPCWSTR mainModule = L"client.exe";
	LPCWSTR hookedModule = L"Ws2_32.dll";
	LPCSTR functionToHook = "recv";
	DWORD iatEntryOffset = 0X12B4;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	unsigned int space_for_back_jmp = 0x7;
	CHAR  jmpBackOpcode[8] = "\xB8\x90\x90\x90\x90\xFF\xE0";
	DWORD jmpBackLoc = 0x30 - space_for_back_jmp;
	SIZE_T virtualProtect_callBack_size = 0x40;// must be >= jmpBackLocInHook+7

	LPVOID f;
	HMODULE h = GetModuleHandle(mainModule);
	HMODULE h2 = GetModuleHandle(hookedModule);

	DWORD lpProtect = 0;
	LPVOID JumpTo;
	LPVOID iat;

	if (h == NULL) {
		MessageBoxW(NULL, L"Can't find module", NULL, NULL);
		return;
	}
	f = GetProcAddress(h2, functionToHook);
	if (f == NULL) {
		MessageBoxW(NULL, L"Can't find function", NULL, NULL);
		return;
	}

	iat = h + iatEntryOffset / 4;

	//Change IAT
	JumpTo = (LPVOID)((char*)&hook);
	VirtualProtect((char*)iat, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);//permissions
	memcpy(iat, &JumpTo, 0x4);

	//Change JumpBack
	JumpTo = (LPVOID)((char*)f);
	memcpy(jmpBackOpcode + 1, &f, 0x4);
	VirtualProtect((char*)restoreCmdAndJmpBack, virtualProtect_callBack_size, PAGE_EXECUTE_READWRITE, &lpProtect);//permissions
	memcpy((char*)&restoreCmdAndJmpBack + jmpBackLoc, &jmpBackOpcode, 0x7);
	VirtualProtect((char*)restoreCmdAndJmpBack, virtualProtect_callBack_size, PAGE_EXECUTE_READ, &lpProtect);//resotre persmissions
}




