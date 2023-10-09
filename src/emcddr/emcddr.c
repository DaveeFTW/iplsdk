#include "emcddr.h"

#include <sysreg.h>

#include <stddef.h>

#define REG32(addr)                 ((volatile uint32_t *)(addr))
#define EMCDDR_MMIO_BASE            (0xBD000000)

#define EMCDDR_UNK20_REG            REG32(EMCDDR_MMIO_BASE + 0x20)
#define UNK20_BIT16                 (1 << 16)
#define UNK20_BIT15                 (1 << 15)

#define EMCDDR_UNK24_REG            REG32(EMCDDR_MMIO_BASE + 0x24)

#define EMCDDR_POWER_DOWN_CTR_REG   REG32(EMCDDR_MMIO_BASE + 0x2C)

#define EMCDDR_UNK30_REG            REG32(EMCDDR_MMIO_BASE + 0x30)
#define UNK30_BIT5                  (1 << 5)
#define UNK30_BIT4                  (1 << 4)
#define UNK30_BIT0                  (1 << 0)

#define EMCDDR_UNK34_REG            REG32(EMCDDR_MMIO_BASE + 0x34)
#define EMCDDR_UNK38_REG            REG32(EMCDDR_MMIO_BASE + 0x38)
#define EMCDDR_UNK40_REG            REG32(EMCDDR_MMIO_BASE + 0x40)
#define EMCDDR_UNK44_REG            REG32(EMCDDR_MMIO_BASE + 0x44)

typedef struct
{
    uint32_t unk0;
    uint32_t unk4;
} DdrParams;

static inline void sync(void)
{
    __asm("sync");
}

static const DdrParams *params_for_type(enum DdrType type) {
    static const DdrParams g_params_32mb =
    {
        .unk0 = UNK30_BIT4,
        .unk4 = 0
    };

    static const DdrParams g_params_64mb =
    {
        .unk0 = UNK30_BIT0 | UNK30_BIT4,
        .unk4 = 0
    };

    switch (type) {
        case DDR_TYPE_32MB:
            return &g_params_32mb;

        case DDR_TYPE_64MB:
            return &g_params_64mb;
    }

    // TODO: error handling?
    return NULL;
}

static void set_ddr_params(enum DdrType type)
{
    const DdrParams *params = params_for_type(type);
    *EMCDDR_UNK30_REG = (*EMCDDR_UNK30_REG & ~(UNK30_BIT5 | UNK30_BIT4 | UNK30_BIT0)) | params->unk0;
    *EMCDDR_UNK38_REG = params->unk4;
    sync();
}

typedef struct
{
    uint32_t unk0;
    uint32_t unk4;
} DdrParams2;

static const DdrParams2 g_ddr_params2[] = {
    // 0
    {
        .unk0 = 0x11110000,
        .unk4 = 0x6000F,
    },
    // 1
    {
        .unk0 = 0x11110000,
        .unk4 = 0x7000F,
    },
    // 2
    {
        .unk0 = 0x11110000,
        .unk4 = 0x7000F,
    },
    // 3
    {
        .unk0 = 0x11110400,
        .unk4 = 0x70000,
    },
    // there are more, but PSP only seems to use number 3 anyway
};

static void set_ddr_params2(int unk, int unk2)
{
    *EMCDDR_UNK30_REG = (*EMCDDR_UNK30_REG & (UNK30_BIT5 | UNK30_BIT4 | UNK30_BIT0)) | ((unk2 - 3) << 12) | g_ddr_params2[unk].unk0;
    *EMCDDR_UNK44_REG = g_ddr_params2[unk].unk4 & 0xFFFF;
    *EMCDDR_UNK34_REG = g_ddr_params2[unk].unk4 >> 16;
    sync();
}

static void set_ddr_params3(int unk, int unk2)
{
    *EMCDDR_UNK40_REG = unk2 | 0x10;
    (void)*EMCDDR_UNK40_REG;
    *EMCDDR_UNK40_REG = unk | 0x10;
    (void)*EMCDDR_UNK40_REG;
    *EMCDDR_UNK40_REG = 0x20;
    sync();
}

static void set_power_down_counter(int unk)
{
    *EMCDDR_POWER_DOWN_CTR_REG = (unk < 0) ? (0) : (0x80000000 | unk);
    sync();
}

static void exec_cmd(uint32_t cmd, uint32_t unk, uint32_t param)
{
    while (*EMCDDR_UNK20_REG & UNK20_BIT16);
    *EMCDDR_UNK24_REG = ((cmd & 1) << 10) | (unk << 16) | (param & ~(0x8400));
    sync();
    *EMCDDR_UNK20_REG = cmd | UNK20_BIT15;
    sync();
    while (*EMCDDR_UNK20_REG & UNK20_BIT16);
}

static void reset_device(int dev, int unk)
{
    exec_cmd(0x25, 0, 0);
    exec_cmd(0x22, 0, 0);
    exec_cmd(0x22, 0, 0);
    exec_cmd(0x20, 0, (dev << 4) | 1);
    exec_cmd(0x20, 2, unk << 5);
}

void init_ddr(enum DdrType type)
{
    sysreg_busclk_enable(BUSCLK_EMCDDR);
    set_ddr_params(type);
    set_ddr_params2(3, 3);
    set_ddr_params3(8, 10);
    set_power_down_counter(4);
    reset_device(3, 1);
}