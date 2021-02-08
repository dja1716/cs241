lis $10
.word 0xffff000c
lis $12
.word 64
lis $4
.word 4
add $5, $0, $1
mult $4, $2
mflo $6
add $6, $6, $1
beginloop: beq$6, $5, endloop
add $5, $5, $4
lw $11, -4($5)
beq $11, $0, zero
add $11, $11, $12
sw $11, 0($10)
beq $0, $0, beginloop
zero: lis $11
.word 32
sw $11, 0($10)
beq $0, $0, beginloop
endloop: jr $31
