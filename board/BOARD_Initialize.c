#include "include.h"

void Init_ALL(void)
{
    __disable_irq();

    BOARD_BootClockRUN();               //��ʼ��ϵͳʱ�Ӵ���(��ʼ��MCG)
    
    BOARD_InitALL();                    //��ʼ������BOARD��Դ
    
    // BOARD_InitDebugConsole();           //��ʼ��Debug����̨
    
    ALEX_lptmr_DelayMs(50);
    
    __enable_irq();
}

__ramfunc void ALEX_lptmr_DelayMs(uint32_t ms)
{
    //LPTMR_StartTimer(LPTMR0); 
    
    if(!ms)
        return;
    
    /* ����LPTMRʱ�� */
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK; 
    
    /* ���ú�����ʱ���� */
    LPTMR0->CMR = ms; 
    
    /* ����LPTMR����Ƶ����1kHz LPOΪʱ��Դ */
    LPTMR0->PSR = 0|LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK; 
    
    /* ������ʱ�� */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; 
    
    /* �ȴ������ﵽ�趨ֵ */
    while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK));
    
    /*�����ʱ�Ƚϱ�־ */
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
    
    //LPTMR_StopTimer (LPTMR0);
}

__ramfunc void ALEX_CPU_Delay(volatile uint32_t cnt)
{
    while(cnt--);
}