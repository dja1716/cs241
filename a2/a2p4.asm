lis $10
.word 4
lis $20
.word 1


bne $2, $0, 1
jr $31

lw $3, 0($1)
bne $2, $20, 1
jr $31

begin:
	add $11, $0, $0
	mult $5, $10
	mflo $11
	add $11, $11, $1
	lw $21, 0($11)
	slt $13, $3, $21
	beq $13, $0, 1
	add $3, $21, $0
	add $5, $5, $20
	bne $5, $2, begin
jr $31
