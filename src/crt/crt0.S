
# Copyright 2018 davee. All rights reserved.
#
# This software is intellectual property of molecule.
# You may not copy, republish, display, distrivute, transmit, sell, rent, lease, loan
# or otherwise make available in any form or by any means all or any portion of this software.

.set noreorder

.section .init.start
.global _start
_start:
    la $sp, 0x041FFFF0
    la $a0, __bss_start
    la $a1, __bss_end

clear_bss:
    sltu $a2, $a0, $a1
    addiu $a0, $a0, 4
    bnel $a2, $0, clear_bss
    sw $0, -4($a0)
    
    jal _init
    nop

    jal main
    nop

    jal _fini
    nop

inf_loop:
    b inf_loop
    nop
