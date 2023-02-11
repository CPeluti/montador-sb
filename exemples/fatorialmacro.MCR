section text 
input n 
load n 
fat: 
sub one 
jmpz fim 
store aux 
mul k 
store n 
load aux 
jmp fat 
fim: output n 
stop 
section data 
aux: space 
n: space 
one: const 0x12 
