#include "include.h"

void Init_ALL(void)
{
    __disable_irq();

    BOARD_BootClockRUN();               //初始化系统时钟代码(初始化MCG)
    
    BOARD_InitALL();                    //初始化所有BOARD资源
    
    // BOARD_InitDebugConsole();           //初始化Debug控制台
    
    ALEX_lptmr_DelayMs(50);
    
    __enable_irq();
}

__ramfunc void ALEX_lptmr_DelayMs(uint32_t ms)
{
    //LPTMR_StartTimer(LPTMR0); 
    
    if(!ms)
        return;
    
    /* 开启LPTMR时钟 */
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK; 
    
    /* 设置毫秒延时计数 */
    LPTMR0->CMR = ms; 
    
    /* 设置LPTMR不分频采用1kHz LPO为时钟源 */
    LPTMR0->PSR = 0|LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK; 
    
    /* 开启定时器 */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; 
    
    /* 等待计数达到设定值 */
    while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK));
    
    /*清除定时比较标志 */
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
    
    //LPTMR_StopTimer (LPTMR0);
}

__ramfunc void ALEX_CPU_Delay(volatile uint32_t cnt)
{
    while(cnt--);
}