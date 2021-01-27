from sys import argv
import subprocess
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
