#ifndef _PART_TVP5150_
#define _PART_TVP5150_

#define TVP5150_Addr 0xB8       //I2C_SEL=0Ê±£¬Addr=0xb8; I2C_SEL=1Ê±£¬Addr=0xBA

#define TVP_REG0 0x00           // Video input source selection.(AIP1A or AIP1B)
#define TVP_REG1 0x01           // Analog channel controls.
#define TVP_REG2 0x02           // Operation mode controls.
#define TVP_REG3 0x03           // Miscellaneous controls.
#define TVP_REG4 0x04           // Auto Switch.
#define TVP_REG13 0x0D          // Output and Data rate options.
#define TVP_REG16 0x0F          // Configuration Shared Pin.

#define REG0_Default  0x00      // AIP1A selected. CVBS signal without force black screen output. 
#define REG1_Default  0x15      // AGC Enabled.
#define REG2_Default  0x00      // Reference to Datasheet Page 31.
#define REG3_Default  0x0D      // Pin27 has been connected to GND so here we disable its output.
                                // [Y7:Y0] output activated. Vertical blanking off. Clock output Enable.
#define REG4_Default  0xDC      // Auto Switch detects CVBS signal in PALN standard.
#define REG13_Default 0x40      // Entended coding range(Y,U,V range from 1 to 254).
#define REG16_Default 0x12      // 
                                // Offset binary code(2s complement + 128). No YCbCr bypass.
                                // 8-bit YUV422 with discrete sync output.

extern void TVP5150_Init(void);
#endif