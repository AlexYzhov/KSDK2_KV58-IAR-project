# KSDK2_KV58 IAR project

�������ǻ���KSDK����ģ���޸Ķ������Ż���ԭ�������ú궨�嵼��ʱ��Ƶ�ʼ��㲻�����Ĳ�������ģ��lpld�ṩ����Ӳ������ʱ����������ɳ������ܳ���������ģ��ĳ�ʼ�����ã��ɽ��KSDK�ֲ�ο��Ķ����˽�KSDK��ʹ�á�ͬʱ����һЩ�Լ�����½½������ֲ���������������߲������ģ���Ʒ����Ʒ:

*L3G4200D/NRF24L01/ST7735/SSD1351/OV7725/TVP5150/VL53L0X/ADNS9800*

���ڲ���͸¶���ǵ��㷨Դ���룬�Ͳ��ṩ�����������ˣ���Ҫ�Ŀ����Լ���ֲ��

---
## �弶����:
XTAL��50Mhz

#### Ĭ�ϴ�FPU��ȫ�̸�����ģʽ
---

ʱ�Ӳ��֣�

> /board/src/clock_config.c

����볬Ƶ���޸�����ļ����
mcgConfig_BOARD_BootClockRUN.pll0Config.prdiv��vidv�ı�pll��Ԥ��Ƶ�ͱ�Ƶϵ�����Ըı�MCG�������
��ע������simConfig_BOARD_BootClockRUN.clkdiv1����Core��Bus��FlexBus��FastPeripheral/Flash���ú��ʵķ�Ƶϵ������ע��KV58��FlashƵ�ʲ��ܳ���32.5Mhz!)

�ܽŸ��ò��֣�

> /board/src/pin_mux.c

���帴�ñ�μ�datasheet pinoutһ�£��Ƽ�ʹ��PORT_SetPinMux�����ǹ����ж��õ�PORT_SetPinConfig���ø��ù��ܣ�

�����ʼ�����֣�

> /board/src/board.c

�������蹦�ܵĳ�ʼ�����뼯��������ļ��ڡ����Բο��Ŵ����KSDK�ֲ���Ϥ���ʹ�á�
ע�⣺KSDKĬ��������FTM�����ܿӣ�һ��PWM����ֻ��100������CnV�Ĵ�����ͬ����ʽ�ǳ��ӵ���
�����޸��˼��еײ���롣���ڹ�����PWM����Ĭ��10000��ͬʱÿ����һ��FTM_UpdatePwmDutycycle������ǿ�ƴ���Software Trigger����װʱ�������Ϊ���������ʱ��������������˽⣬ֱ�ӵ��ú������С�������Լ��޸ģ����Ķ�RM����FTM���ֵ����ݣ�CnV�ȼĴ����Ļ�����ƣ��Լ�Enhanced PWM Synchronization Mode��ͬ��״̬����

���һ�㣬�����ʼ����ʱ���Ҫ�ṩһ������Ƶ�ʣ���I2C_MasterInit(I2C1, &I2C1_config, SystemBusClock);
��ʱ����Ҫȷ�ϴ���ʼ���������ǹ����������ڲ������ϵģ���Reference Manual����½ڿ���֪������Ȼ���ṩ��ǰ�����ڲ����ߵ�Ƶ�ʡ�������clock_config.c�����MCG��SIM��ʼ��������˾����޸ĵ�SystemCoreClockUpdate()���������Զ�����Ԥ��ķ�Ƶϵ������Դ����Ƶ�ʼ���������ߵ�Ƶ��ֵ�������ʼ��ʱֱ�ӵ��ü��ɡ�����ṩ�˴���ĳ�ʼ��Ƶ�ʣ����ܻᵼ������Ĺ���Ƶ�ʲ�������

---

���ɵ��õ�Ƶ�ʱ����б�

SystemCoreClock - CPU�ں�Ƶ�ʣ�Ĭ��235Mhz

SystemFlexBusClock - FlexBusƵ��

SystemBusClock  - BUSƵ��

SystemFlashClock    - FLASHƵ�ʣ���ʵ������KV58��Flash��Bus����һ����Ƶϵ������ζ��FlashClock==BusClock��

SystemMCGFFClock - MCGFFʱ��Ƶ�ʣ�һ������ʱ�ӣ����ڸ����FTMģ���ṩ��Ƶʱ��Դ��ʵ�ֵ�Ƶ�ʵ�PWM�����50HZ��