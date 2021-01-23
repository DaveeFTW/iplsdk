.set noreorder
.set noat

.global interrupt_suspend
interrupt_suspend:
mfic $v0, $0
mtic $0, $0
nop
nop
nop
nop
nop
nop
nop
jr $ra
nop

.global interrupt_resume
interrupt_resume:
mtic $a0, $0
nop
nop
nop
nop
nop
nop
nop
jr      $ra
nop

.global interrupt_resume_with_sync
interrupt_resume_with_sync:
sync
nop
sync
nop
mtic $a0, $0
nop
nop
nop
nop
nop
nop
nop
jr      $ra
nop

.set at
.global interrupt_init
interrupt_init:
li $v0, 0x2000000F
sw $v0, 0xBC300008
sw $0, 0xBC300018
sw $0, 0xBC300028
jr      $ra
sync

.global interrupt_enable
interrupt_enable:
li $v1, 1
sllv $a0, $v1, $a0

la $v0, __enabled_interrupts
lw $v1, 0($v0)
or $v1, $a0
sw $v1, 0($v0)
li $v0, 0x2000000F
or $v1, $v0
sw $v1, 0xBC300008
sw $0, 0xBC300018
sw $0, 0xBC300028
jr $ra
sync

__enabled_interrupts:
.word 0x0