outra_macro: macro
stop
endmacro
MUL_N: MACRO &a &b &c
MUl &a
STORE &b
copy &a,&c
outra_macro
ENDMACRO
SECTION TEXT
INPUT N
LOAD N
FAT: SUB ONE
JMPZ FIM
STORE AUX
MUL_N aux one n
LOAD AUX
JMP FAT
FIM: OUTPUT N
STOP
SECTION DATA
AUX: SPACE
N: SPACE
ONE: CONST 0x12