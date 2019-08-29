/*
 * Copyright 2018 davee. All rights reserved.
 *
 * This software is intellectual property of molecule.
 * You may not copy, republish, display, distribute, transmit, sell, rent, lease, loan
 * or otherwise make available in any form or by any means all or any portion of this software.
 *
 */

OUTPUT_FORMAT("elf32-littlemips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips:allegrex)
ENTRY(_start)

MEMORY
{
    SCRATCH : ORIGIN = 0x00010000, LENGTH = 0x00010000
    VRAM : ORIGIN = 0x040F0000, LENGTH = 0x00400000
}

SECTIONS
{
    .init : { *(.init.start) *(.init) } >VRAM
    .text : { *(.text) } >VRAM
    .sdata : { *(.sdata) *(.sdata.*) *(.gnu.linkonce.s.*) } >VRAM
    .rodata : { *(.rodata) } > VRAM
    .data : { *(.data*) } > VRAM
    .bss :
    {
        __bss_start = .;
        *(.bss*)
        __bss_end = .;
    } > VRAM
}
