// Hooking template @Michael Ezra. Good luck in the test :)
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <Shlwapi.h>

#define DLL_PATH "client.dll"
#define true 1
#define false 0


BOOL dllInjector(char* dllpath, DWORD pID);


int main(int argc, char** argv)
{

	// Create Process in SUSPENDED mode.
	PROCESS_INFORMATION pi;
	STARTUPINFOA Startup;
	ZeroMemory(&Startup, sizeof(Startup)); //fill block with zeros
	ZeroMemory(&pi, sizeof(pi)); //fill block with zeros

	//create a process from a PE path (suspended mode).
	LPSTR argument = (LPSTR)"client.exe DMSG";
	CreateProcessA(NULL, argument, NULL, NULL, NULL, CREATE_SUSPENDED, NULL, NULL, &Startup, &pi);

	//Inject the dll (pi = process id. DLL_PATH - Dll's name. 
	if (!(dllInjector((char*)DLL_PATH, pi.dwProcessId))) //pi - process object
		return 1;


	//run the process
	Sleep(1000);
	ResumeThread(pi.hThread);
	return 0;
}

/*
@param dllpath - dll to inject.
@pID - identification of the target procees to inject to.

@return False on failure, True otherwise.
*/
BOOL dllInjector(char* dllpath, DWORD pID)
{
	HANDLE pHandle;
	LPVOID remoteString;
	LPVOID remoteLoadLib;

	// Get a handler to the target process.
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

	if (!pHandle) //error checking
		return false;

	//find the address of dll's loading function (LoadLibaryA at Kernel32.dll).
	//We search the function on the current process, but the address should not be different in the target process.
	remoteLoadLib = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

	// Copy dll's path string to the target process's memory space.
	remoteString = (LPVOID)VirtualAllocEx(pHandle, NULL, strlen(DLL_PATH), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(pHandle, (LPVOID)remoteString, dllpath, strlen(dllpath), NULL);

	//Create sub remote thread at the target process, to execute LoadLibaryA function on the target process.
	if (NULL == CreateRemoteThread(pHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)remoteLoadLib, (LPVOID)remoteString, NULL, NULL)) {
		return false; //error checking
	}

	//Close the handle! (This doesn't terminate the process)
	CloseHandle(pHandle);

	return true;
}
