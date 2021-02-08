; 20547127 hdum
add $5, $0, $0
lis $6
.word 1
sub $4, $2, $6
lis $24
.word 241
add $15, $1, $0
lis $14
.word 4

lis $16
.word 0xffff000c



start:
beq $4, $5, end
add $5, $5, $6
lw $11, 0($15)
add $15, $15, $14
lw $12, 0($15)
sub $13, $11, $12
bne $13, $24, next
lis $11
.word 48
sw $11, 0($16)
lis $11
.word 10
sw $11, 0($16)
beq $0, $0, start

next:
slt $11, $13, $24
bne $11, $6, next2
lis $11
.word 45
sw $11, 0($16)
lis $11
.word 49
sw $11, 0($16)
lis $11
.word 10
sw $11, 0($16)
beq $0, $0, start

next2:
lis $11
.word 49
sw $11, 0($16)
lis $11
.word 10
sw $11, 0($16)
beq $0, $0, start


end:
jr $31
