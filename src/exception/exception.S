.set noreorder
.set noat

.global register_ebase_handler
register_ebase_handler:
mtc0 $a0, $25
jr $ra
nop

.global get_ebase_handler
get_ebase_handler:
mfc0 $v0, $25
jr $ra
nop
