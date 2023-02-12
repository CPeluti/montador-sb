section .bss
endereco resb 2

section .text
global _start
_start:

    push endereco
    call input_c

    push endereco
    call output_c

    mov eax, 1
    mov ebx, 0
    int 80h

input_c:
    enter 0, 0
    
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp+8]
    mov edx, 2          ; 2 por causa do enter
    int 80h

    leave
    ret 2

output_c:
    enter 0, 0

    mov eax, 4
    mov ebx, 1
    mov ecx, [ebp+8]
    mov edx, 1
    int 80h

    leave
    ret 2