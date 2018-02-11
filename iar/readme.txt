该文件夹是基于KSDK 2.1，IAR环境下的K66FX1M0VLQ18工程。

Read & Write↓
根目录/app/：功能函数文件夹，用于存放 算法/功能性 程序代码。同时根目录存放了main.c及include.h，若为方便管理中断，可再添加独立的isr.c。
根目录/board/：用于存放MCU初始化相关的代码（如MCU时钟、管脚复用等）。
根目录/driver/：用于存放外置传感器的驱动代码（如陀螺仪、摄像头、液晶屏、距离传感器等）。

根目录/iar/：IAR工程文件夹（不要自己修改！）。

Read Only↓
根目录/Library/CMSIS/Include：此处存放ARM提供的CMSIS文件。
根目录/Library/MK66F18/：此处存放NXP提供的关于MK66F18芯片的描述文件。
根目录/Library/MK66F18/drivers/：此处存放NXP编写的KSDK外设库。
根目录/Library/MK66F18/startup/：此处存放NXP编写的MCU启动文件。
根目录/Library/MK66F18/utilities/：此处存放NXP编写的debug console及shell。