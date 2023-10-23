.set noreorder
.set noat

.global exception_register_ebase_handler
exception_register_ebase_handler:
mtc0 $a0, $25
jr $ra
nop

.global exception_get_ebase_handler
exception_get_ebase_handler:
mfc0 $v0, $25
jr $ra
nop
