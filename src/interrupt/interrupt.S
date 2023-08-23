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
