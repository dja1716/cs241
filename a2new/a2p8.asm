lis $11
.word -1
lis $4
.word 4
lis $10
.word 1
add $3, $0, $10
lis $8
.word 8



compare:
add $1, $1, $4
add $1, $1, $4
lw $12, -4($1)
beq $11, $12, leftEnd

add $3, $3, $10
mult $12, $4
mflo $18
add $18, $18, $1
lis $17
.word compare
sw $31, -4($30)
sub $30, $30, $4
sw $1, -4($30)
sub $30, $30, $4

add $1, $18, $0
jalr $17

add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $1, -4($30)

leftEnd:
sw $3, -4($30)
sub $30, $30, $4
add $1, $1, $4
lw $13, -4($1)
beq $11, $13, rightEnd
add $3, $3, $10
mult $13, $4
mflo $18
add $18, $18, $1
lis $17
.word compare

sw $31, -4($30)
sub $30, $30, $4
sw $1, -4($30)
sub $30, $30, $4
add $1, $18, $0

jalr $17

add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $1, -4($30)

rightEnd:
add $30, $30, $4
lw $5, -4($30)
sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
lis $17
.word maximum

add $1, $5, $0
add $2, $3, $0
jalr $17

add $30, $30, $4
lw $1, -4($30)
add $30, $30, $4
lw $2, -4($30)
jr $31

maximum:
slt $3, $1, $2
beq $3, $0, 2
add $3, $2, $0
jr $31
add $3, $1, $0
jr $31
