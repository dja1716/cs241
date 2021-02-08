beq $2, $0, return
lis $3
.word -1
lis $4
.word 4
lis $5
.word 64
lis $6
.word 0xffff000c
lis $7
.word 32
output:
	lw $8, 0($1)
	add $1, $1, $4
	add $2, $2, $3
	bne $8, $0, 2
	add $8, $8, $7
	beq $0, $0, 1
	add $8, $8, $5
	sw $8, 0($6)
	bne $2, $0, output
return:
	jr $31
