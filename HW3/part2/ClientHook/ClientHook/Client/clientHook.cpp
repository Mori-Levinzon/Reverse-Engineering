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
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlstr.h>

using std::ofstream;
using std::endl;
using std::wstring;
using std::string;

ofstream log_file("log.txt");


// change this to the proper signature of the hook
typedef BOOL(WINAPI* HOOK_TYPE)(SOCKET, char*, int, int);

HOOK_TYPE orig_recv;



string DEFcase(const string& input, int i) {
	int sum = (input[i] - '0') + (input[i + 2] - '0');
	switch (sum) {
	case 0x0d:   return "D";
	case 0x0e:   return "E";
	case 0x0f:   return "F";
	default:   return "F";
	}
}


string convert_ASCII(string hex) {
	string ascii = "";
	for (size_t i = 0; i < hex.length(); i += 2) {
		//taking two characters from hex string
		string part = hex.substr(i, 2);
		//changing it into base 16
		char ch = stoul(part, nullptr, 16);
		//putting it into the ASCII string
		ascii += ch;
	}
	return ascii;
}


void decypherMessage(char*& charInput,int length) {
	printf("enter to the debugger\n");
	//log_file << "BEFORE THE CHANGE " << LPCWSTRinput << endl;
	//log_file << "BEFORE THE CHANGE ADRESS " << *LPCWSTRinput << endl;

	std::string input(charInput, charInput + length);
	printf("ENTRANCE TO FUNCTION%s\n", input);

	string output = "";
	for (int i = 0; i < input.size(); i++) {
		//printf("option A\n");
		//wprintf(L'A');
		//printf("option B\n");
		//wprintf(L"A");
		switch (input[i]) {
		case 'A':   output += "1";//'1'
			break;
		case 'J':   output += "A";//'A'
			break;
		case 'Q':   output += "B";//'B'
			break;
		case 'K':   output += "C";//'C'
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
			if (i < input.size() - 1) {
				if (input[i + 1] == L'-') {
					output += "0";//'0'
					i += 2;
					break;
				}
				else if ((input[i + 1] == L'+')) {
					output += DEFcase(input, i);
					i += 2;
					break;
				}
			}
			output += std::to_string((((int)(input[i])) - ((int)(L'0'))));
		}

	}
	printf("exit FROM HOOK\n");
	printf("%s\n", output);

	string convertedString = convert_ASCII(output);

	printf("%s\n", convertedString);

	for (int i = 0; i < length;i++) {
		charInput[i]= convertedString[i];
	}
}


int WINAPI setRecvHook(SOCKET s, char* buffer, int    len, int    flags) {
	// reverse the string presented
	int res = orig_recv(s, buffer, len, flags);
	decypherMessage(buffer, len);
	//log_file << decypherMessage(buffer) << endl;
	return res;
}



void setHook() {

	log_file << "dll attached" << endl;


	HMODULE curr_prog = GetModuleHandle(L"client.exe"); // get the handle of the main process
	HMODULE target_dll = GetModuleHandle(L"Ws2_32.dll"); // get the handle of the dll conatining the function we want to override
	DWORD lpProtect = 0;
	DWORD IAT_Func_Offset = 0xA2AC;
	LPVOID JumpTo;
	LPDWORD IAT_ADDRESS;
	//log_file << "after offset" << endl << "IAT address: " << 0 << endl;

	if ((curr_prog == NULL) || (target_dll == NULL)) {
		log_file << "couldnt get handles";
		return;
	}
	//log_file << "after check 1" << endl << "IAT address: " << 0 << endl;

	orig_recv = (HOOK_TYPE)GetProcAddress(target_dll, "recv");
	if (orig_recv == NULL) {
		log_file << "couldnt find function" << endl;
		return;
	};
	IAT_ADDRESS = (LPDWORD)(curr_prog + IAT_Func_Offset / 4);
	//log_file << "curr_prog: " << curr_prog << endl;
	//log_file << "IAT_Func_Offset: " << IAT_Func_Offset << endl;
	//log_file << "IAT address: " << *IAT_ADDRESS << endl;
	//log_file << "IAT address without *: " << IAT_ADDRESS << endl;
	//log_file << "orig_recv: " << (DWORD)orig_recv << endl;
	//log_file << "orig_recv: " << (LPDWORD)orig_recv << endl;
	//log_file << "what shoulhave been: " << ((LPDWORD)orig_recv - (LPDWORD)curr_prog)*4 << endl;

	if ((*IAT_ADDRESS) != (DWORD)orig_recv) {
		log_file << "IAT contents does not match - maybe check the offset again" << endl;
		return;
	}

	log_file << "changing IAT entry" << endl << "IAT address: " << IAT_ADDRESS << endl;
	JumpTo = (LPVOID)((char*)&setRecvHook);
	VirtualProtect(IAT_ADDRESS, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy(IAT_ADDRESS, &JumpTo, 0x4);
	VirtualProtect(IAT_ADDRESS, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
	log_file << "success" << endl;


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
