push ebp            ; store ebp
mov ebp, esp

push 0x0       		; pushing null
push 0x41797261		; pushing A,y,r,a
push 0x7262694c		; pushing r,b,i,L
push 0x64616f4c		; pushing d,a,o,L
push esp            ; push pointer for "LoadLibraryA"

call FindFunction   ; call FindFunction("LoadLibraryA")
add esp, 0x14       ; clear stack

push 0x00006c6c		; pushing null,l,l
push 0x642e7472		; pushing d,.,t,r
push 0x6376736d		; pushing c,v,s,m
push esp

call eax            ; call LoadLibrary("msvcrt.dll")
add esp, 0x0c       ; clear stack (note arguments are cleared already)

push eax            ; store module handle for msvcrt
push 0x00007373		; pushing null,s,s
push 0x65726464		; pushing e,r,d,d
push 0x41636f72		; pushing A,c,o,r
push 0x50746547		; pushing P,t,e,G
push esp            ; push pointer for "GetProcAddress"

call FindFunction   ; call FindFunction("GetProcAddress")
add esp, 0x14       ; clear stack
pop ebx             ; restore module handle for msvcrt

push 0x00006674		; pushing null,f,t
push 0x6e697270		; pushing n,i,r,p
push esp            ; push pointer for "printf"
push ebx            ; push module handle for msvcrt

call eax            ; call GetProcAddress(msvcrt, "printf")
add esp, 0x08       ; clear stack (note arguments are cleared already)

xor edx, edx
push edx            ; pushing null
push 0x21646c72		; pushing !,d,l,r
push 0x6f57206f		; pushing o,W, ,o
push 0x6c6c6548		; pushing l,l,e,H
push esp            ; push pointer for "Hello World!"

call eax            ; call printf("Hello World!")
add esp, 0x14       ; clear stack

mov eax, 44         ; set exit code to 44
pop ebp             ; restore ebp
ret                 ; return

FindFunction:       ; placeholder for search_kernel32
