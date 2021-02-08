lis $3
.word 1
lis $4
.word 4
lis $5
.word 0xffff000c
lis $6
.word 32
lis $11
.word 64

bne $2, $0, 1
jr $31

add $9, $1, $0

loop:
	lw $8, 0($9)
	add $7, $7, $3
	add $9, $9, $4
	
	add $8, $8, $11
	bne $8, $11, 1
	sub $8, $8, $6
	sw $8, 0($5)
	bne $7, $2, loop
jr $31
