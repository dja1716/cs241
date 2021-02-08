.import init
.import print
.import new
.import delete
lis $11; for true
.word 1
lis $4
.word 4
sub $29, $30, $4
sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lis $7
.word init
jalr $7
add $30, $30, $4
lw $31, -4($30)
add $3, $11, $0
sw $3, -8($29)
sub $30, $30, $4
lis $3
.word 1
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $7
.word new
jalr $7
add $30, $30, $4
lw $31, -4($30)
bne $3, $0, 1
add $3, $11, $0
sw $3, -8($29)
add $15, $1, $0
lis $3
.word 1
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $10
.word print
jalr $10
add $30, $30, $4
lw $31, -4($30)
add $1, $15, $0
lw $3, -8($29)
add $12, $3, $0
lis $3
.word 5
sw $3, 0($12)
add $15, $1, $0
lis $3
.word 1
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $10
.word print
jalr $10
add $30, $30, $4
lw $31, -4($30)
add $1, $15, $0
lw $3, -8($29)
lw $3, 0($3)
add $30, $30, $4
add $30, $30, $4
jr $31
