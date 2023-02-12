section .bss
aux resd 1
n resd 1
section .data
one dd 0x12
section .text
global _start
_start:
input [n]
mov eax, [n]
fat: 
mov eax, [aux]
fim: 
mov eax, 1
mov ebx, 0
int 80h
