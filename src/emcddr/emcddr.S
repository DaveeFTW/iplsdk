.set noreorder
.set noat

#define EMCDDR_MMIO_BASE            (0xBD000000)
#define EMCDDR_FLUSH_REG            (EMCDDR_MMIO_BASE + 0x04)
#define EMCDDR_POWER_DOWN_CTR_REG   (EMCDDR_MMIO_BASE + 0x2C)
#define EMCDDR_UNK30_REG            (EMCDDR_MMIO_BASE + 0x30)
#define EMCDDR_UNK34_REG            (EMCDDR_MMIO_BASE + 0x34)
#define EMCDDR_UNK40_REG            (EMCDDR_MMIO_BASE + 0x40)
#define EMCDDR_UNK44_REG            (EMCDDR_MMIO_BASE + 0x44)

.global __emcddr_support_code_start
.global emcddr_init2
__emcddr_support_code_start:
emcddr_init2:
    // no need to backup $a1 as we use it immediately and then never again
    move    $t4, $a0
    move    $t5, $a2
    move    $t6, $a3
    move    $t7, $t0
    move    $t8, $t1
    move    $t9, $ra

    // step 1: set ddr params2. $a0, and $a1 are already aligned on the ABI
    bal     emcddr_set_params2
    nop

    // TODO: there is a check for a positive or zero value for $a2. we don't
    // do this, but maybe its needed in the future for something

    li      $t0, 0xBC100068
    ori     $t1, $t4, 0x80
    sw      $t1, 0($t0)
    sync

    // if there are no params for emcddr_set_param3, then just exit immediately
    bltz    $t6, emcddr_init2_exit
    nop

    // step2: set ddr params3. the original parameters are $a3, and $a4, now
    // known as $t6, and $t7
    move    $a0, $t6
    bal     emcddr_set_params3
    move    $a1, $t7

    li      $t0, EMCDDR_UNK40_REG
    li      $t2, 2
    li      $t3, 350

// wait until bit 0x100 is set
emcddr_init2_wait1:
    lw      $t1, 0($t0)
    addiu   $t3, $t3, -1
    bltz    $t3, some_self_test_maybe
    nop
    andi    $t1, $t1, 0x100
    beqz    $t1, emcddr_init2_wait1
    nop

// wait until bit 0x100 is cleared
emcddr_init2_wait2:
    lw      $t1, 0($t0)
    addiu   $t3, $t3, -1
    bltz    $t3, some_self_test_maybe
    nop
    andi    $t1, $t1, 0x100
    bnez    $t1, emcddr_init2_wait2
    nop
    addiu   $t2, $t2, -1
    bgtz    $t2, emcddr_init2_wait1
    nop

some_self_test_maybe:
    beqz    $t8, emcddr_init2_exit
    nop

some_self_test_loop:
    // hit invalidate the data cache
    cache   0x19, 0($t8)

    // load 16 bytes and perform some test?
    lw      $t0, 0($t8)
    lw      $t1, 4($t8)
    lw      $t2, 8($t8)
    lw      $t3, 12($t8)
    or      $v0, $t2, $t3
    and     $v0, $t1, $v0
    nor     $v0, $0, $v0
    rotr    $t2, $t2, 1
    xor     $t2, $t2, $t3
    or      $t0, $t0, $t2
    or      $v0, $v0, $t0
    bnez    $v0, some_self_test_loop
    nop

emcddr_init2_exit:
    jr      $t9
    nop

.global emcddr_flush
emcddr_flush:
    sync
    li      $v0, EMCDDR_FLUSH_REG
    andi    $a0, $a0, 0xF
    sw      $a0, 0($v0)

emcddr_flush_wait_loop:
    lw      $v1, 0($v0)
    and     $v1, $v1, $a0
    bnez     $v1, emcddr_flush_wait_loop
    nop
    jr      $ra
    nop

.global emcddr_power_down_counter
emcddr_power_down_counter:
    li      $v0, EMCDDR_POWER_DOWN_CTR_REG
    li      $v1, 0x80000000
    or      $v1, $v1, $a0
    slti    $t0, $a0, 1
    movn    $v1, $0, $t0
    sw      $v1, 0($v0)
    sync
    jr      $ra
    nop

.global emcddr_set_params2
emcddr_set_params2:
    // since this module is location agnostic, we must do some clever usage of
    // bal (branch and link) to perform pc relative accesses
    move    $a2, $ra
    bal     emcddr_get_pc_rel
    nop

emcddr_get_pc_rel:
    addiu   $t0, $ra, ddr_calibration_data-emcddr_get_pc_rel

    // index $a0 into the calibration data array. each entry is 8 bytes, so
    // a left shift of 3 is equivalent: (x << 3) == x*2^3
    sll     $t1, $a0, 3
    addu    $t0, $t0, $t1
    lw      $v0, 0($t0)
    lw      $v1, 4($t0)

    // process parameter two in $a1. (a1 - 3) * 0x1000
    addiu   $t0, $a1, -3
    sll     $t0, $t0, 12 // multiply by 0x1000
    or      $v0, $v0, $t0

    // update UNK30
    li      $t0, EMCDDR_UNK30_REG
    lw      $t1, 0($t0)
    and     $t1, $t1, 0x31
    or      $t1, $t1, $v0
    sw      $t1, 0($t0)

    // update UNK44
    li      $t0, EMCDDR_UNK44_REG
    andi    $t1, $v1, 0xFFFF
    sw      $t1, 0($t0)

    // update UNK34
    li      $t0, EMCDDR_UNK34_REG
    srl     $t1, $v1, 16
    sw      $t1, 0($t0)

    jr      $a2
    nop

.global emcddr_set_params3
emcddr_set_params3:
    // set REG40 as $a1 | 0x10
    li      $t0, EMCDDR_UNK40_REG
    ori     $t1, $a1, 0x10
    sw      $t1, 0($t0)

    // read out REG40 but do nothing with the result
    lw      $0, 0($t0)
    
    // set REG40 as $a0 | 0x10
    li      $t0, EMCDDR_UNK40_REG
    ori     $t1, $a0, 0x10
    sw      $t1, 0($t0)

    // read out REG40 but do nothing with the result
    lw      $0, 0($t0)

    // set REG40 to 0x20
    li      $t1, 0x20
    sw      $t1, 0($t0)
    sync
    jr      $ra
    nop


.align 4
ddr_calibration_data:
    // calibration data 0
    .word 0x11110000
    .word 0x6000F
    // calibration data 1
    .word 0x11110000
    .word 0x7000F
    // calibration data 2
    .word 0x11110000
    .word 0x7000F
    // calibration data 3
    .word 0x11110400
    .word 0x70000
    // calibration data 4
    .word 0x11110400
    .word 0x70008
    // calibration data 5
    .word 0x11110400
    .word 0x7000A

.global __emcddr_support_code_end
__emcddr_support_code_end: