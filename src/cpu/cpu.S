.set noreorder
.set noat

.global cpu_enable_interrupts
cpu_enable_interrupts:
li $v0, 1
mtic $v0, $0
jr $ra
nop

.global cpu_get_status
cpu_get_status:
mfc0 $v0, $12
jr $ra
nop

.global cpu_set_status
cpu_set_status:
mtc0 $a0, $12
jr $ra
nop
