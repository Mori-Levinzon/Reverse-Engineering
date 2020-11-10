	.file	"challenge.c"
	.intel_syntax noprefix
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC0:
	.ascii "%s\0"
LC1:
	.ascii "%.2d %.2d %.2d %.2d %.2d\0"
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	push	ebp
	mov	ebp, esp
	and	esp, -16
	sub	esp, 64
	call	___main
	mov	DWORD PTR [esp+60], 0
	mov	DWORD PTR [esp+56], 0
	mov	DWORD PTR [esp+52], 0
	mov	DWORD PTR [esp+48], 0
	mov	DWORD PTR [esp+44], 0
	jmp	L2
L6:
	movzx	eax, BYTE PTR [esp+34]
	cmp	al, 99
	jne	L3
	add	DWORD PTR [esp+48], 2
	add	DWORD PTR [esp+60], 3
L3:
	movzx	eax, BYTE PTR [esp+34]
	cmp	al, 114
	jne	L4
	add	DWORD PTR [esp+44], 1
	add	DWORD PTR [esp+56], 1
L4:
	movzx	eax, BYTE PTR [esp+34]
	cmp	al, 115
	jne	L5
	add	DWORD PTR [esp+44], 1
	add	DWORD PTR [esp+56], 1
	add	DWORD PTR [esp+52], 1
	add	DWORD PTR [esp+48], 1
L5:
	movzx	eax, BYTE PTR [esp+34]
	cmp	al, 100
	jne	L2
	add	DWORD PTR [esp+52], 1
	add	DWORD PTR [esp+48], 1
	add	DWORD PTR [esp+60], 1
L2:
	lea	eax, [esp+34]
	mov	DWORD PTR [esp+4], eax
	mov	DWORD PTR [esp], OFFSET FLAT:LC0
	call	_scanf
	cmp	eax, -1
	jne	L6
	mov	eax, DWORD PTR [esp+60]
	mov	DWORD PTR [esp+20], eax
	mov	eax, DWORD PTR [esp+48]
	mov	DWORD PTR [esp+16], eax
	mov	eax, DWORD PTR [esp+52]
	mov	DWORD PTR [esp+12], eax
	mov	eax, DWORD PTR [esp+56]
	mov	DWORD PTR [esp+8], eax
	mov	eax, DWORD PTR [esp+44]
	mov	DWORD PTR [esp+4], eax
	mov	DWORD PTR [esp], OFFSET FLAT:LC1
	call	_printf
	mov	eax, 0
	leave
	ret
	.ident	"GCC: (tdm64-1) 5.1.0"
	.def	_scanf;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
