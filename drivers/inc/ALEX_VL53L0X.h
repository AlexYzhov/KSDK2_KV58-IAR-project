#ifndef _ALEX_VL53L0X
#define _ALEX_VL53L0X

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70

#define VL53L0X_REG_SYSRANGE_START                  0x00
    /** mask existing bit in #VL53L0X_REG_SYSRANGE_START*/
    #define VL53L0X_REG_SYSRANGE_MODE_MASK          0x0F
    /** bit 0 in #VL53L0X_REG_SYSRANGE_START write 1 toggle state in
    * continuous mode and arm next shot in single shot mode */
    #define VL53L0X_REG_SYSRANGE_MODE_START_STOP    0x01
    /** bit 1 write 0 in #VL53L0X_REG_SYSRANGE_START set single shot mode */
    #define VL53L0X_REG_SYSRANGE_MODE_SINGLESHOT    0x00
    /** bit 1 write 1 in #VL53L0X_REG_SYSRANGE_START set back-to-back
    *  operation mode */
    #define VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK    0x02
    /** bit 2 write 1 in #VL53L0X_REG_SYSRANGE_START set timed operation
    *  mode */
    #define VL53L0X_REG_SYSRANGE_MODE_TIMED         0x04
    /** bit 3 write 1 in #VL53L0X_REG_SYSRANGE_START set histogram operation
    *  mode */
    #define VL53L0X_REG_SYSRANGE_MODE_HISTOGRAM     0x08

#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_SYSTEM_INTERRUPT_CLEAR          0x0B
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define VL53L0X_Add                                 0x29

extern uint8_t VL53L0X_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);//连续写
extern uint8_t VL53L0X_Read_Len(uint8_t reg, uint8_t len, uint8_t *buf);//连续读
extern uint8_t VL53L0X_Write_Byte(uint8_t reg, uint8_t data);//写一个字节
extern uint8_t VL53L0X_Read_Byte(uint8_t reg);//读一个字节
extern uint16_t bswap(uint8_t b[]); 				 
extern uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg);	
extern uint16_t makeuint16(int lsb, int msb);

extern void VL53L0X_Init(void);
extern uint16_t VL53L0X_ReadSingleShotInMillimeters(void);

#endif