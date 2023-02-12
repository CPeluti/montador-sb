%define TAMANHO 10

section .bss
tamanho_string  resd 1
endereco        resb TAMANHO
buffer          resb 1

section .text
global _start
_start:

    mov dword [tamanho_string], TAMANHO ; exemplo
    push endereco               ; ebp+12
    push 8                       ; ebp+8
    call input_s

    mov dword [tamanho_string], 8           ; epb+8 EXEMPLO
    push endereco                           ; ebp+12
    push 8                     ; ebp+8
    call output_s

    mov eax, 1
    mov ebx, 0
    int 80h

input_s:
    enter 0, 0                  ; epb-4 é o i do "for"
    ; mov dword [ebp-4], 0      ; inicia com o index 0
    mov ecx, [ebp+12]           ; coloca o endereco em ebx
    mov edx, [ebp+8]            ; coloca o tamanho em ecx

    mov eax, 3
    mov ebx, 0
    int 80h

    leave
    ret 8


output_s:
    enter 0, 0                  ; epb-4 é o i do "for"
    mov ecx, [ebp+12]           ; coloca o endereco em ebx
    mov edx, [ebp+8]            ; coloca o tamanho em ecx

    mov eax, 4
    mov ebx, 1
    int 80h

    leave
    ret 8