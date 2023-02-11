section .data
msg db "-300", 0dH, 0ah
section .bss
buffer resb 1
flag resb 1
section .text
global _start
_start:
	push dword 0 ; adiciona acumulador inicial na pilha
	mov byte [flag], 0
input:	
	mov eax, 3
	mov ebx, 0
	mov ecx, buffer
	mov edx, 1
	int 80h
	cmp byte [buffer], '-'
	jne nNegativo
	mov byte [flag], 1
	jmp input
nNegativo:
	cmp byte [buffer], 0ah
	je nPush ; se for final da linha
	cmp byte [buffer], 0dH
	je nPush
	pop eax ; pega valor total
	mov ecx, 10
	mul ecx ; ax=valor
	mov ecx, [buffer]
	sub ecx, 0x30
	add eax,ecx ; soma numero do input no resultado da multiplicação
	push dword eax
	jmp input ;se adicionou um numero na pilha, ainda não acabou
nPush:
	pop eax ; res fica em eax
	cmp byte [flag], 1
	jne fim
	neg eax
fim:
	push eax
	add eax, 0x30
	mov [buffer], eax
	mov eax ,4
	mov ebx, 1
	mov ecx, buffer
	mov edx, 1
	int 80h
	pop eax
outra:
	push eax ; adiciona numero na pilha
	call output
	mov eax,1
	mov ebx,0
	int 80h
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

