lis $4
.word 4
lis $10
.word 10
lis $11
.word 0xffff000c
lis $12
.word -1
lis $13
.word 1
lis $15
.word 48

add $14, $0, $0
add $6, $1, $0
slt $5, $6, $0
beq $5, $0, next
mult $1, $12
mflo $6
lis $7
.word 45
sw $7, 0($11)

next:
div $6, $10
mfhi $5
mflo $6
sw $5, -4($30)
sub $30, $30, $4
add $14, $14, $13
beq $6, $0, print
beq $0, $0, next

print:
add $30, $30, $4
lw $7, -4($30)
add $7, $7, $15
sw $7, 0($11)
sub $14, $14, $13
bne $14, $0, print
sw $10, 0($11)
jr $31 
