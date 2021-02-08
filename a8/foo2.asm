lis $11;
.word 1
.import print
lis $4
.word 4
sub $29, $30, $4
sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
lw $3, 0($29)
add $12, $3, $0
lis $3
.word 23456
sw $3, 0($12)
lw $3, 0($29)
lw $3, 0($3)
add $30, $30, $4
add $30, $30, $4
jr $31
