section .data
msg db "-300", 0dH, 0ah

section .bss
buffer resb 1
flag resb 1
endereco resd 1

section .text
global _start
_start:

	push endereco	; ebp+8
	call input

	; mov eax, 4
	; mov ebx, 1
	; add byte [endereco], 0x30
	; mov ecx, endereco
	; mov edx, 1
	; int 80h

	push eax ; adiciona numero na pilha
	call output

	mov eax,1
	mov ebx,0
	int 80h

input:	
	enter 2, 0	; ebp-1 = buffer / ebp-2 = flag

	push dword 0 		; adiciona acumulador inicial na pilha
	mov byte [ebp-2], 0	; flag = 0

	loop_i:
		mov ecx, ebp
		dec ecx
		mov eax, 3
		mov ebx, 0
		mov edx, 1
		int 80h
		
		cmp byte [ebp-1], '-'
		jne nNegativo
		mov byte [ebp-2], 1
		jmp loop_i
	nNegativo:
		cmp byte [ebp-1], 0ah
		je nPush ; se for final da linha
		cmp byte [ebp-1], 0dH
		je nPush
		pop eax ; pega valor total
		mov ecx, 10
		mul ecx ; ax=valor
		mov ecx, [ebp-1]
		sub ecx, 0x30
		add eax,ecx ; soma numero do input no resultado da multiplicação
		push eax
		jmp loop_i ;se adicionou um numero na pilha, ainda não acabou
	nPush:
		pop eax ; res fica em eax
		cmp byte [ebp-2], 1
		jne fim
		neg eax
	fim:
		mov [ebp+8], eax
		
		leave
		ret 4
		


output:
	enter 0,0
	mov eax, [ebp+8]
	check_signal:
		cmp eax,0
		jge loop
		mov byte [buffer], '-'
		push eax
		mov eax, 4
		mov ebx, 1
		mov ecx, buffer
		mov edx, 1
		int 80h
		pop eax
		neg eax
	loop:
		xor edx, edx
		mov ecx, 10
		div ecx
		add edx, 0x30
		push edx; move edx para add de print
		cmp eax,0
		jne loop
	loop_print:
		pop eax
		mov [buffer], eax
		mov eax, 4
		mov ebx, 1
		mov ecx, buffer
		mov edx, 1
		int 80h
		cmp ebp, esp
		jne loop_print
		leave
		ret 4

