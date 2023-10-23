.set noreorder
.set noat

.global ebase_handler
ebase_handler:
# save v0 and v1 context
ctc0 $v0, $4
ctc0 $v1, $5

# backup Cause and Status register
mfc0 $v0, $13
mfc0 $v1, $12
ctc0 $v0, $3
ctc0 $v1, $2

# backup EPC
mfc0 $v1, $14
ctc0 $v1, $0

addiu $v1, $sp, -0x90

# backup all register states
sw $at, 0($v1)
sw $a0, 4($v1)
sw $a1, 8($v1)
sw $a2, 0xC($v1)
sw $a3, 0x10($v1)
sw $t0, 0x14($v1)
sw $t1, 0x18($v1)
sw $t2, 0x1C($v1)
sw $t3, 0x20($v1)
sw $t4, 0x24($v1)
sw $t5, 0x28($v1)
sw $t6, 0x2C($v1)
sw $t7, 0x30($v1)
sw $s0, 0x34($v1)
sw $s1, 0x38($v1)
sw $s2, 0x3C($v1)
sw $s3, 0x40($v1)
sw $s4, 0x44($v1)
sw $s5, 0x48($v1)
sw $s6, 0x4C($v1)
sw $s7, 0x50($v1)
sw $t8, 0x54($v1)
sw $t9, 0x58($v1)
sw $k0, 0x5C($v1)
sw $k1, 0x60($v1)
sw $gp, 0x64($v1)
sw $sp, 0x68($v1)
sw $fp, 0x6C($v1)
sw $ra, 0x70($v1)

move $sp, $v1

# backup lo/hi states
mflo $v1
sw $v1, 0x74($sp)
mfhi $v1
sw $v1, 0x78($sp)

# backup v0/v1
cfc0 $v1, $4
sw $v1, 0x7C($sp)
cfc0 $v1, $5
sw $v1, 0x80($sp)

# invoke our exception handler
move $a0, $v0
cfc0 $a1, $0

mfc0 $v0, $8
move $a2, $v0
la $v0, exception_handler
jalr $v0
move $a3, $sp

# restore lo/hi states
lw $v1, 0x78($sp)
mthi $v1
lw $v1, 0x74($sp)
mflo $v1

move $v1, $sp

# backup all register states
lw $at, 0($v1)
lw $a0, 4($v1)
lw $a1, 8($v1)
lw $a2, 0xC($v1)
lw $a3, 0x10($v1)
lw $t0, 0x14($v1)
lw $t1, 0x18($v1)
lw $t2, 0x1C($v1)
lw $t3, 0x20($v1)
lw $t4, 0x24($v1)
lw $t5, 0x28($v1)
lw $t6, 0x2C($v1)
lw $t7, 0x30($v1)
lw $s0, 0x34($v1)
lw $s1, 0x38($v1)
lw $s2, 0x3C($v1)
lw $s3, 0x40($v1)
lw $s4, 0x44($v1)
lw $s5, 0x48($v1)
lw $s6, 0x4C($v1)
lw $s7, 0x50($v1)
lw $t8, 0x54($v1)
lw $t9, 0x58($v1)
lw $k0, 0x5C($v1)
lw $k1, 0x60($v1)
lw $gp, 0x64($v1)
lw $sp, 0x68($v1)
lw $fp, 0x6C($v1)
lw $ra, 0x70($v1)

# restore Cause and Status register
cfc0 $v0, $3
mtc0 $v0, $13
cfc0 $v1, $2
mtc0 $v1, $12

# restore EPC
cfc0 $v1, $0
mtc0 $v1, $14

# restore v0 and v1 context
cfc0 $v0, $4
cfc0 $v1, $5

# return from exception
eret
.global ebase_handler_size
ebase_handler_size:
.dword ebase_handler_size - ebase_handler