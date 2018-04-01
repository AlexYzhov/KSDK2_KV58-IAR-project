 /* This is a template file for board configuration */

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* The board name */
#define BOARD_NAME "###-not-specified-###"


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* Initialize debug console. */
void BOARD_InitDebugConsole(void);

void BOARD_InitUART0(void);
void BOARD_InitUART4(void);
void BOARD_InitADC1(void);
void BOARD_InitI2C1(void);
void BOARD_InitI2C3(void);
void BOARD_InitSPI0(void);
void BOARD_InitSPI1(void);
void BOARD_InitSPI2(void);
void BOARD_InitSDHC(void);
void BOARD_InitFTM0(void);
void BOARD_InitFTM1(void);
void BOARD_InitFTM2(void);
void BOARD_InitFTM3(void);
void BOARD_InitPIT0(void);
void BOARD_InitPIT1(void);
void BOARD_InitPIT2(void);
void BOARD_InitPIT3(void);
void BOARD_InitGPIO(void);
void BOARD_InitLPTMR(void);
void BOARD_InitGPIO(void);
void BOARD_InitGPIO_Camera(void);
void BOARD_InitDMA(void);
void BOARD_InitXBARA(void);
void BOARD_InitXBARB(void);
void BORAD_InitAOI(void);

extern void BOARD_InitALL(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
