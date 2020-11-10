main:
        push    ebp
        mov     ebp, esp
		
		push 0x0
		push 0x41797261
		push 0x7262694c
		push 0x64616f4c
		push esp            

		call FindFunction
		add esp, 0x14

		push 0x00006c6c
		push 0x642e7472
		push 0x6376736d
		push esp

		call eax
		add esp, 0x0c

		push eax
		push 0x00007373
		push 0x65726464
		push 0x41636f72
		push 0x50746547
		push esp

		call FindFunction
		add esp, 0x14
		pop ebx

		push 0x00006674
		push 0x6e697270
		push esp
		push ebx
		
		mov edi, eax
		
		call eax
		add esp, 0x08
		
		mov esi, eax
		
		mov eax, edi
		push 0x00000066
		push 0x6e616373
		push esp
		push ebx
		
		call eax
		add esp, 0x08
		
		mov edi, eax

        push    ecx
        sub     esp, 68
        mov     DWORD PTR [ebp-12], 0
        mov     DWORD PTR [ebp-16], 0
        mov     DWORD PTR [ebp-20], 0
        mov     DWORD PTR [ebp-24], 0
        mov     DWORD PTR [ebp-28], 0
        mov     DWORD PTR [ebp-44], 2126629
L7:
        sub     esp, 8
        lea     eax, [ebp-40]
        push    eax
        lea     eax, [ebp-43]
        push    eax
        call    edi
        add     esp, 16
        cmp     eax, -1
        setne   al
        test    al, al
        je      L2
        movzx   eax, BYTE PTR [ebp-40]
        cmp     al, 114
        jne     L3
        add     DWORD PTR [ebp-12], 1
        add     DWORD PTR [ebp-16], 1
L3:
        movzx   eax, BYTE PTR [ebp-40]
        cmp     al, 115
        jne     L4
        add     DWORD PTR [ebp-12], 1
        add     DWORD PTR [ebp-16], 1
        add     DWORD PTR [ebp-20], 1
        add     DWORD PTR [ebp-24], 1
L4:
        movzx   eax, BYTE PTR [ebp-40]
        cmp     al, 99
        jne     L5
        add     DWORD PTR [ebp-24], 2
        add     DWORD PTR [ebp-28], 3
L5:
        movzx   eax, BYTE PTR [ebp-40]
        cmp     al, 100
        jne     L7
        add     DWORD PTR [ebp-20], 1
        add     DWORD PTR [ebp-24], 1
        add     DWORD PTR [ebp-28], 1
        jmp     L7
L2:
        mov     DWORD PTR [ebp-68], 1681010725
        mov     DWORD PTR [ebp-64], 842016032
        mov     DWORD PTR [ebp-60], 807739492
        mov     DWORD PTR [ebp-56], 622879794
        mov     DWORD PTR [ebp-52], 543437360
        mov     DWORD PTR [ebp-48], 1681010725
        mov     BYTE PTR [ebp-44], 0
        sub     esp, 8
        push    DWORD PTR [ebp-28]
        push    DWORD PTR [ebp-24]
        push    DWORD PTR [ebp-20]
        push    DWORD PTR [ebp-16]
        push    DWORD PTR [ebp-12]
        lea     eax, [ebp-68]
        push    eax
        call    esi
        add     esp, 32
        mov     eax, 0
        pop ebp
        ret
FindFunction:

