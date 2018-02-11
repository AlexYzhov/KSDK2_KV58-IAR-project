#ifndef _PART_SCCB_
#define _PART_SCCB_

//SCCB ¹Ü½ÅÅäÖÃ
#define SCCB_SCL        GPIOE11
#define SCCB_SDA        GPIOE10

#define SCL_H()         GPIOE->PSOR = 1U << 11U;
#define SCL_L()         GPIOE->PCOR = 1U << 11U;
#define SCL_DDR_OUT()   do{GPIOE->PDDR&=(uint32_t)~(1U<<11U);GPIOE->PDDR|=(uint32_t)(1U<<11U);}while(0) 
#define SCL_DDR_IN()    do{GPIOE->PDDR&=(uint32_t)~(1U<<11U);}while(0)
                                    
#define SDA_H()         GPIOE->PSOR = 1U << 10U;
#define SDA_L()         GPIOE->PCOR = 1U << 10U;
#define SDA_IN()        ((GPIOE->PDIR&(1U<<10U))>>10U)
#define SDA_DDR_OUT()   GPIOE->PDDR&=(uint32_t)~(1U<<10U);\
                        GPIOE->PDDR|=(uint32_t)(1U<<10U);
#define SDA_DDR_IN()    GPIOE->PDDR&=(uint32_t)~(1U<<10U);

#define SCCB_DELAY()    ALEX_CPU_Delay(200)

// #define SCCB_WriteByte(base, SlaveAddress, REG_Address, REG_data) I2C_WriteByte(base, SlaveAddress, REG_Address, REG_data)
extern void I2C_WriteByte(I2C_Type *base, uint8_t SlaveAddress, uint8_t REG_Address,unsigned char REG_data);
extern uint8_t I2C_ReadByte(I2C_Type *base, uint8_t SlaveAddress, uint8_t REG_Address);
// extern uint8_t SCCB_ReadByte(I2C_Type *base, uint8_t SlaveAddress, uint8_t REG_Address);

extern int16_t SCCB_WriteByte_soft(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
extern int16_t SCCB_ReadByte_soft(uint8_t SlaveAddress, uint8_t REG_Address);

#endif