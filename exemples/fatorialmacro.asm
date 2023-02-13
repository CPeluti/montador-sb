SECTION TEXT
INPUT N
LOAD N
FAT: ADD ONE
store N
output n
STOP
SECTION DATA
AUX: SPACE
N: SPACE
ONE: CONST 1

;Chama input()
;mov eax, [n]
;add eax, [one]
;mov eax, 1\nmov ebx, 0\nint 80h