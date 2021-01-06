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

//ofstream log_file("log.txt");


// change this to the proper signature of the hook
typedef INT(WINAPI* HOOK_TYPE)(SOCKET, char*, int, int);

HOOK_TYPE orig_recv;

char DEFcase(const string& input, int i) {
	int sum = (input[i] - '0') + (input[i + 2] - '0');
	switch (sum) {
	case 0x0d:   return 'D';
	case 0x0e:   return 'E';
	case 0x0f:   return 'F';
	default:   return 'F';
	}
}

int convert_ASCII(char hex[], char* ascii) {
	//char ascii[240];
	int j = 0;
	for (size_t i = 0; i < strlen(hex); i += 2) {
		//taking two characters from hex string
		string part1(1, hex[i]);
		string part2(1, hex[i + 1]);
		//changing it into base 16
		char ch =(char) stoul(part1 + part2, nullptr, 16);
		//putting it into the ASCII string
		ascii[j++] = ch;
	}
	ascii[j++] = '\0';
	return j;
}

INT WINAPI RecvHook(SOCKET s, char* buff, int len, int flags) {
	//std::cout << "enter the hook" << endl;
	int res =orig_recv(s, buff, len, flags);
		char output[480] = { 0 };
		int j = 0;
		for (int i = 0; i < (int) strlen(buff); i++) {
			switch (buff[i]) {
			case 'A':   output[j] = '1';//'1'
				j++;
				break;
			case 'J':   output[j] = 'A';//'A'
				j++;
				break;
			case 'Q':   output[j] = 'B';//'B'
				j++;
				break;
			case 'K':   output[j] = 'C';//'C'
				j++;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (i < (int)strlen(buff) - 1) {
					if (buff[i + 1] == '-') {
						output[j] = '0';//'0'
						j++;
						i += 2;
						break;
					}
					else if ((buff[i + 1] == '+')) {
						output[j] = DEFcase(buff, i);
						j++;
						i += 2;
						break;
					}
				}
				output[j] = (char)(((int)(buff[i])) - ((int)('0')) + '0');
				j++;
			}
		}
	int textWritten = convert_ASCII(output, buff);
	//std::cout << "exit the hook" << endl;
	return textWritten;
}



void setHook() {

	//std::cout << "dll attached" << endl;


	HMODULE curr_prog = GetModuleHandle(NULL); // get the handle of the main process
	HMODULE target_dll = GetModuleHandle(L"Ws2_32.dll"); // get the handle of the dll conatining the function we want to override
	DWORD lpProtect = 0;
	DWORD IAT_Func_Offset = 0xA2AC;
	LPVOID JumpTo;
	LPDWORD IAT_ADDRESS;

	if ((curr_prog == NULL) || (target_dll == NULL)) {
		std::cout <<  "couldnt get handles";
		return;
	}

	orig_recv = (HOOK_TYPE)GetProcAddress(target_dll, "recv");
	if (orig_recv == NULL) {
		std::cout <<  "couldnt find function" << endl;
		return;
	};
	IAT_ADDRESS = (LPDWORD)(curr_prog + IAT_Func_Offset / 4);
	if ((*IAT_ADDRESS) != (DWORD)orig_recv) {
		std::cout <<  "IAT contents does not match - maybe check the offset again" << endl;
		return;
	}

	//log_file << "changing IAT entry" << endl << "IAT address: " << IAT_ADDRESS << endl;
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
