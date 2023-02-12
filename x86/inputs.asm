%define TAMANHO 10

section .bss
tamanho_string  resd 1
endereco        resb TAMANHO
buffer          resb 1

section .text
global _start
_start:

    mov dword [tamanho_string], TAMANHO ; exemplo
    ;push tamanho_string         ; epb+8
    call input_s

    mov dword [tamanho_string], 8        ; epb+8 EXEMPLO
    call output_s

    mov eax, 1
    mov ebx, 0
    int 80h

input_s:
    enter 4, 0              ; epb-4 é o i do "for"
    mov dword [ebp-4], 0    ; inicia com o index 0

    loop_read:
        dec dword [tamanho_string]
        je termina
        
        mov eax, 3
        mov ebx, 0
        mov ecx, buffer
        mov edx, 1
        int 80h

        mov ebx, [ebp-4]        ; coloca o i em ebx
        mov ecx, [buffer]       ; coloca o conteudo do buffer em ecx
        mov [ebx+endereco], ecx ; coloca o conteudo de ecx no vetor da memoria
        inc dword [ebp-4]       ; i++

        jmp loop_read

    termina:
        leave
        ret 4


output_s:
    enter 4, 0              ; epb-4 é o i do "for"
    mov dword [ebp-4], 0    ; inicia com o index 0

    loop_print:

        mov esi, [ebp-4]
        add esi, endereco
        
        mov eax, 4
        mov ebx, 1
        mov ecx, esi
        mov edx, 1
        int 80h

        inc dword [ebp-4]       ; i++
        dec dword [tamanho_string]
        je fim

        jmp loop_print

    fim:
        leave
        ret 4