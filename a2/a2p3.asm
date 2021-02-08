bne $2, $0, 3
lis $3
.word -1
jr $31
lis $7
.word 4
mult $7, $2
mflo $4
add $4, $4, $1
lw $3, -4($4)
jr $31
