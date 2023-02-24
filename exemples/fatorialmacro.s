section .bss
n resd 10
section .data
section .text
global _start
input_c:
enter 0, 0
mov eax, 3
mov ebx, 0
mov ecx, [ebp+8]
mov edx, 2
int 80h
leave
ret 4
output_c:
enter 0, 0
mov eax, 4
mov ebx, 1
mov ecx, [ebp+8]
mov edx, 2
int 80h
leave
ret 4
input_s:
enter 0, 0
mov ecx, [ebp+12]
mov edx, [ebp+8]
mov eax, 3
mov ebx, 0
int 80h
leave
ret 8
output_s:
enter 0, 0
mov ecx, [ebp+12]
mov edx, [ebp+8]
mov eax, 4
mov ebx, 1
int 80h
leave
ret 8
input:
enter 2, 0
push dword 0
mov byte [ebp-2], 0
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
je nPush
cmp byte [ebp-1], 0dH
je nPush
pop eax
mov ecx, 10
mul ecx
mov ecx, [ebp-1]
sub ecx, 0x30
add eax,ecx
push eax
jmp loop_i
nPush:
pop eax
cmp byte [ebp-2], 1
jne fim
neg eax
fim:
mov ebx, [ebp+8]
mov [ebx], eax
leave
ret 4
output:
enter 1,0
mov eax, [ebp+8]
check_signal:
cmp eax, 0
jge loop_o
mov byte [ebp-1], '-'
push eax
mov ecx, ebp
dec ecx
mov eax, 4
mov ebx, 1
mov edx, 1
int 80h
pop eax
neg eax
loop_o:
xor edx, edx
mov ecx, 10
div ecx
add edx, 0x30
push edx
cmp eax, 0
jne loop_o
loop_print:
pop eax
mov [ebp-1], eax
mov ecx, ebp
dec ecx
mov eax, 4
mov ebx, 1
mov edx, 1
int 80h
cmp ecx, esp
jne loop_print
leave
ret 4
_start:
push eax
push n
push 10
call input_s
pop eax
push eax
push n
push 10
call output_s
pop eax
mov eax, 1
mov ebx, 0
int 80h
