#ifndef __FPGA_H_
#define __FPGA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CN_LOCK_VALUE          0xAA55
//CPLD基址    0x60000000              
typedef struct 
{
    uc16 uc16AC_Freq;           //工频(PT信号交流频率)
    uc16 uc16AD_Busy;           //AD是否忙标志
    u16 u16LcdContrast;         //只写，液晶对比度控制
    u16 u16FeedDog;             //只写，喂狗
    u16 u16KI_RW;               //写入开入通道地址，读入8个开入
    u16 u16KO1;                 //只写，开出1
    uc16 uc16KO2;               //只写，开出2
    uc16 uc16AD_DataLen;        //AD数据长度
    u16 u16AD_Data;             //AD数据
    u16 u16IntInterval;         //只写，超级中断间隔，uS
    u16 u16Lock;                //锁定寄存器(写入0xaa55)
    u16 u16UnLock;              //解锁寄存器(写入0xaa55)
    u16 u16EnAD;                //使能AD相关模块(不使能，则AD和测频均不能工作)
    u16 u16Counter10mS;         //10ms计数器
}tagFpgaReg;

extern tagFpgaReg volatile * const g_ptFpgaReg;

#ifdef __cplusplus
}
#endif

#endif //__FPGA_H_

