lis $4
.word 4
add $5, $4, $1
mult $4, $2
mflo $6
add $6, $6, $1
lw $7, -4($5)
beginloop: beq $5, $6, endloop
add $5, $5, $4
lw $8, -4($5)
slt $9, $7, $8
beq $9, $0, 1
add $7, $8, $0
beq $0, $0, beginloop
endloop: add $3, $7, $0
jr $31
