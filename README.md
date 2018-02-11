# KSDK2_KV58 IAR project

本工程是基于KSDK例程模板修改而来，优化了原工程滥用宏定义导致时钟频率计算不正常的不便利，模仿lpld提供了软硬两个延时函数。已完成常见智能车制作常见模块的初始化配置，可结合KSDK手册参考阅读，了解KSDK的使用。同时附上一些自己今年陆陆续续移植的驱动，常见或者不常见的，成品或半成品:

*L3G4200D/NRF24L01/ST7735/SSD1351/OV7725/TVP5150/VL53L0X/ADNS9800*

限于不便透露我们的算法源代码，就不提供单独的例程了，需要的可以自己移植。

---
## 板级属性:
XTAL：50Mhz

#### 默认打开FPU，全程高性能模式
---

时钟部分：

> /board/src/clock_config.c

如果想超频，修改这个文件里的
mcgConfig_BOARD_BootClockRUN.pll0Config.prdiv和vidv改变pll的预分频和倍频系数，以改变MCG的输出；
再注意配置simConfig_BOARD_BootClockRUN.clkdiv1，对Core、Bus、FlexBus、FastPeripheral/Flash配置合适的分频系数。（注意KV58的Flash频率不能超过32.5Mhz!)

管脚复用部分：

> /board/src/pin_mux.c

具体复用表参见datasheet pinout一章，推荐使用PORT_SetPinMux而不是工程中多用的PORT_SetPinConfig设置复用功能！

外设初始化部分：

> /board/src/board.c

常见外设功能的初始化代码集中在这个文件内。可以参考着代码和KSDK手册熟悉库的使用。
注意：KSDK默认外设库的FTM操作很坑，一是PWM精度只有100，二是CnV的触发和同步方式非常坑爹。
本人修改了几行底层代码。现在工程里PWM精度默认10000，同时每调用一次FTM_UpdatePwmDutycycle，都会强制触发Software Trigger，重装时间点设置为计数器溢出时。如果不想深入了解，直接调用函数就行。如果想自己修改，请阅读RM关于FTM部分的内容，CnV等寄存器的缓冲机制，以及Enhanced PWM Synchronization Mode的同步状态机。

最后一点，外设初始化的时候多要提供一个总线频率，如I2C_MasterInit(I2C1, &I2C1_config, SystemBusClock);
此时，需要确认待初始化的外设是挂载在哪条内部总线上的（读Reference Manual相关章节可以知晓），然后提供当前这条内部总线的频率。工程在clock_config.c中完成MCG、SIM初始化后调用了经过修改的SystemCoreClockUpdate()函数，会自动根据预设的分频系数和有源晶振频率计算各个总线的频率值，外设初始化时直接调用即可。如果提供了错误的初始化频率，可能会导致外设的工作频率不正常。

---

附可调用的频率变量列表：

SystemCoreClock - CPU内核频率，默认235Mhz

SystemFlexBusClock - FlexBus频率

SystemBusClock  - BUS频率

SystemFlashClock    - FLASH频率（事实上由于KV58的Flash和Bus共用一个分频系数，意味着FlashClock==BusClock）

SystemMCGFFClock - MCGFF时钟频率，一个低速时钟，用于给舵机FTM模块提供低频时钟源，实现低频率的PWM输出（50HZ）