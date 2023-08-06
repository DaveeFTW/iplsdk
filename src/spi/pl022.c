
#define NUM_BIT_MASK(num) ((1 << num) - 1)
#define BIT_CLEAR(val, offset, num) (val & ~(NUM_BIT_MASK(num) << offset))
#define BIT_EXT(val, offset, num) ((val >> offset) & NUM_BIT_MASK(num))
#define BIT_INS(val, bits, offset, num) (BIT_CLEAR(val, offset, num) | ((bits & NUM_BIT_MASK(num)) << offset))

#define CR0_OFFSET (0)
#define CR1_OFFSET (4)

enum FrameFormat {
    MOTOROLA_SPI = 0b00,
    TI_SYNCHRONOUS_SERIAL = 0b01,
    NATIONAL_MICROWIRE = 0b10,
};


// Control register 0, SSPCR0
static unsigned int cr0_default(void) { return 0; }
static unsigned int cr0_get_dss(unsigned int cr0) { return BIT_EXT(cr0, 0, 4); }
static unsigned int cr0_get_frf(unsigned int cr0) { return BIT_EXT(cr0, 4, 2); }
static unsigned int cr0_get_spo(unsigned int cr0) { return BIT_EXT(cr0, 6, 1); }
static unsigned int cr0_get_sph(unsigned int cr0) { return BIT_EXT(cr0, 7, 1); }
static unsigned int cr0_get_scr(unsigned int cr0) { return BIT_EXT(cr0, 8, 8); }
static unsigned int cr0_set_dss(unsigned int cr0, unsigned int dss) { return BIT_INS(cr0, dss, 0, 4); }
static unsigned int cr0_set_frf(unsigned int cr0, unsigned int frf) { return BIT_INS(cr0, frf, 4, 2); }
static unsigned int cr0_set_spo(unsigned int cr0, unsigned int spo) { return BIT_INS(cr0, spo, 6, 1); }
static unsigned int cr0_set_sph(unsigned int cr0, unsigned int sph) { return BIT_INS(cr0, sph, 7, 1); }
static unsigned int cr0_set_scr(unsigned int cr0, unsigned int scr) { return BIT_INS(cr0, scr, 8, 8); }


unsigned int init_device(void) {
    unsigned int cr0 = cr0_default();

    // perform 16 bit data transfers
    cr0 = cr0_set_dss(cr0, 0b1111);

    // use motorola SPI frame format
    cr0 = cr0_set_frf(cr0, 0b00);

    // set SSPCLKOUT for motorola SPI
    cr0 = cr0_set_spo(cr0, 0b1);

    cr0 = cr0_set_sph(cr0, 0b1);

    return cr0;
}