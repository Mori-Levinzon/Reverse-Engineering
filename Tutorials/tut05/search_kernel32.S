push ebp
mov ebp, esp

; allocate local variables and initialize them to 0
sub esp, 0x1c
xor eax, eax
mov [ebp - 0x04], eax       ; will store number of exported functions
mov [ebp - 0x08], eax       ; will store address of exported functions addresses table
mov [ebp - 0x0c], eax       ; will store address of exported functions name table
mov [ebp - 0x10], eax       ; will store address of exported functions ordinal table
mov [ebp - 0x14], eax       ; will store a null terminated byte string LoadLibrary
mov [ebp - 0x18], eax       ; will store address to function
mov [ebp - 0x1c], eax       ; reserved

mov edx, [ebp + 0x8]        ; get pointer to function
mov [ebp - 0x14], edx       ; store pointer to function


mov eax, [fs:0x30]          ; Pointer to PEB (https://en.wikipedia.org/wiki/Win32_Thread_Information_Block)
mov eax, [eax + 0x0c]       ; Pointer to Ldr
mov eax, [eax + 0x14]       ; Pointer to InMemoryOrderModuleList
mov eax, [eax]              ; this program's module
mov eax, [eax]              ; ntdll module
mov eax, [eax -0x8 + 0x18]  ; kernel32.DllBase

; kernel32 base address
mov ebx, eax                ; store kernel32.dll base address in ebx

; get address of PE signature
mov eax, [ebx + 0x3c]       ; 0x3c into the image - RVA of PE signature
add eax, ebx                ; address of PE signature: eax = eax + kernel32 base -> eax = 0xf8 + kernel32 base

; get address of Export Table
mov eax, [eax + 0x78]       ; 0x78 bytes after the PE signature is an RVA of Export Table
add eax, ebx                ; address of Export Table = Export Table RVA + kernel32 base

; get number of exported functions
mov ecx, [eax + 0x14]
mov [ebp - 0x4], ecx        ; store number of exported functions

; get address of exported functions table
mov ecx, [eax + 0x1c]       ; get RVA of exported functions table
add ecx, ebx                ; get address of exported functions table
mov [ebp - 0x8], ecx        ; store address of exported functions table

; get address of name pointer table
mov ecx, [eax + 0x20]       ; get RVA of Name Pointer Table
add ecx, ebx                ; get address of Name Pointer Table
mov [ebp - 0x0c], ecx       ; store address of Name Pointer Table

; get address of functions ordinal table
mov ecx, [eax + 0x24]       ; get RVA of functions ordinal table
add ecx, ebx                ; get address of functions ordinal table
mov [ebp - 0x10], ecx       ; store address of functions ordinal table

; loop through exported function name pointer table and find position of LoadLibrary
xor eax, eax
xor ecx, ecx

findPosition:
mov esi, [ebp - 0x14]       ; esi = pointer to LoadLibrary
mov edi, [ebp - 0x0c]       ; edi = pointer to exported function names table
cld                         ; https://en.wikipedia.org/wiki/Direction_flag
mov edi, [edi + eax*4]      ; get RVA of the next function name in the exported function names table
add edi, ebx                ; get address of the next function name in the exported function names table

mov cx, 8                   ; tell the next-comparison instruction to compare first 8 bytes
repe cmpsb                  ; check if esi == edi

jz LoadFound
inc eax                     ; increase the counter
cmp eax, [ebp - 0x4]        ; check if we have looped over all the exported function names
jmp findPosition

LoadFound:
mov ecx, [ebp - 0x10]       ; ecx = ordinal table
mov edx, [ebp - 0x8]        ; edx = export address table


mov ax, [ecx + eax * 2]     ; get LoadLibrary ordinal
mov eax, [edx + eax * 4]    ; get RVA of LoadLibrary function
add eax, ebx                ; get VA of LoadLibrary

add esp, 0x1c               ; local variables
pop ebp
ret
