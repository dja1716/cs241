beq $2, $0, finish

lis $4
.word 4
lis $6
.word print
mult $4, $2
mflo $3 ;Total N
add $3, $3, $1
add $5, $0, $1


begin:
add $5, $5, $4
lw $1, -4($5)
sw $31, -4($30)
lis $31
.word 4
sub $30, $30, $31

jalr $6

lis $31
.word 4
add $30, $30, $31
lw $31, -4($30)


bne $5, $3, begin

finish:
jr $31
