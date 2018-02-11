/* This is a template file for pins configuration */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include <stdbool.h>

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void InitPins_UART0_LPC824(void);
void InitPins_UART4_UpperCOM(void);
void InitPins_ADC1_AD8210(void);
void InitPins_I2C1_VL53L0X(void);
void InitPins_I2C3_SCCB(void);
void InitPins_SPI0_L3G4200D_SCREEN(void);
void InitPins_SPI1_NRF2401(void);
void InitPins_SPI2_ADNS9818(void);
void InitPins_SDHC(void);
void InitPins_FTMx_QD_Encoder(void);
void InitPins_FTM0CHx_Motor(void);
void InitPins_FTM1_QuadDecoder(void);
void InitPins_FTM2_QuadDecoder(void);
void InitPins_FTM3CHx_Servo(void);
void InitPins_GPIO_Camera(void);
void InitPins_GPIO_KEY(void);
void InitPins_GPIO_LED(void);
void InitPins_GPIO_LCD(void);

extern void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _PIN_MUX_H_  */
