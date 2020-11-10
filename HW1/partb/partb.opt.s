	.file	"partb.c"
	.intel_syntax noprefix
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC0:
	.ascii "Guess a number between 1 and 100\0"
LC1:
	.ascii "Enter your guess: \0"
LC2:
	.ascii "%d\0"
LC3:
	.ascii "Too big, try again.\0"
LC4:
	.ascii "Too small, try again.\0"
LC5:
	.ascii "Correct! you win!\0"
	.section	.text.startup,"x"
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	push	ebp
	mov	ebp, esp
	push	esi
	push	ebx
	lea	esi, [ebp-12]
	sub	esp, 12
	call	___main
	mov	DWORD PTR [esp], 0
	call	_time
	mov	DWORD PTR [esp], eax # _srand need 1 param - THE TIME AS A RANDOM SEED
	call	_srand
	call	_rand
	mov	ecx, 100
	cdq
	mov	DWORD PTR [esp], OFFSET FLAT:LC0
	idiv	ecx
	mov	DWORD PTR [ebp-12], -1
	lea	ebx, [edx+1]
	call	_puts
L2:
	mov	DWORD PTR [esp], OFFSET FLAT:LC1 # the wrong string was printed (LC1 IS THE CORRECT)
	call	_printf
	mov	DWORD PTR [esp+4], esi
	mov	DWORD PTR [esp], OFFSET FLAT:LC2
	call	_scanf
	cmp	ebx, DWORD PTR [ebp-12]
	jge	L3	#there was no response when you insert a number that is small but there is one when it's too big
	mov	DWORD PTR [esp], OFFSET FLAT:LC3
	jmp	L7
L3:
	jle	L5
	mov	DWORD PTR [esp], OFFSET FLAT:LC4
L7:
	call	_puts
	jmp	L2
L5:
	mov	DWORD PTR [esp], OFFSET FLAT:LC5
	call	_puts
	add	esp, 12
	xor	eax, eax
	pop	ebx
	pop	esi
	pop	ebp
	ret
	.ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
	.def	_time;	.scl	2;	.type	32;	.endef
	.def	_srand;	.scl	2;	.type	32;	.endef
	.def	_rand;	.scl	2;	.type	32;	.endef
	.def	_puts;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_scanf;	.scl	2;	.type	32;	.endef
