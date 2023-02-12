SECTION TEXT
INPUT N
LOAD N
FAT: ADD ONE
STOP
SECTION DATA
AUX: SPACE
N: SPACE
ONE: CONST 0x12

;Chama input()
;mov eax, [n]
;add eax, [one]
;mov eax, 1\nmov ebx, 0\nint 80h