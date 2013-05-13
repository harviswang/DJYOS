//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//版权所有 (C), 2005-2009,   lst.
//所属模块:  公共
//作者：     lst
//版本：      V1.0.0
//初始版本完成日期：2009-08-05
//文件描述:  cortex-m0寄存器的头文件
//其他说明:  无
//修订历史:
//2. ...
//1. 日期: 2009-08-05
//   作者: lst
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#ifndef __CORTEXM0_H__
#define __CORTEXM0_H__

#ifdef __cplusplus
extern "C" {
#endif

//定义cmsis版本号，cmsis是arm公司定义的一套用于访问cpu资源的标准方法集。
#define cn_cm0_cmsis_version_main  (0x01)
#define cn_cm0_cmsis_version_sub   (0x30)
#define cn_cm0_cmsis_version       ((cn_cm0_cmsis_version_main << 16) | cn_cm0_cmsis_version_sub) 

#define __cortex_m                (0x00)

#ifdef  __ICCARM__ 
  #include <intrinsics.h> 
#endif


#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    2
#endif

typedef enum IRQn
{
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  HardFault_IRQn                = -13,      /*!<  3 Hard Fault Interrupt                          */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */
} IRQn_Type;

//------------------------ SystemTick ----------------------------------------
//CTRL 中断控制及状态寄存器，0xE000_E010,bm_为位掩码，bo_为位偏移
#define bm_systick_ctrl_countflag   (1<<16) //rc,计数值变为0时自动置1，读清零
#define bm_systick_ctrl_clksource   (1<<2)  //rw,时钟选择，0=外部时钟stclk，1=内核fclk
#define bm_systick_ctrl_tickint     (1<<1)  //rw,1=SysTick 数到0 时产生SysTick 异常请求
#define bm_systick_ctrl_enable      (1<<0)  //rw,SysTick 定时器的使能位
#define bo_systick_ctrl_countflag   16   //rc,计数值变为0时自动置1，读清零
#define bo_systick_ctrl_clksource   2    //rw,时钟选择，0=外部时钟stclk，1=内核fclk
#define bo_systick_ctrl_tickint     1    //rw,1=SysTick 数到0时产生SysTick 异常请求
#define bo_systick_ctrl_enable      0    //rw,SysTick 定时器的使能位

//CTRL SysTick校准数值寄存器 0xE000_E01c,bm_为位掩码，bo_为位偏移
#define bm_systick_calib_NOREF  (1<<31)  //r,1=STCLK 不可用,0=可用
#define bm_systick_calib_SKEW   (1<<30)  //r,1=校准值不是准确的10ms，0=是
#define bo_systick_calib_NOREF  31  //r,1=STCLK 不可用,0=可用
#define bo_systick_calib_SKEW   30  //r,1=校准值不是准确的10ms，0=是
//rw,10ms 的时间内倒计数的格数。芯片设计者应该通过Cortex‐M3 的输入信号提供该数值。
//若该值读回零，则表示无法使用校准功能
#define bm_systick_calib_TENMS  (0xffffff<<0)
#define bo_systick_calib_TENMS  0

struct systick_reg      //0xe000e010
{
    vu32 ctrl;
    vu32 reload;    //当倒数至零时，将被重装载的值
    vu32 current;   //读取时返回当前倒计数的值，写则清零，并清CTRL中的COUNTFLAG 标志
    vuc32 verify;   //SysTick校准数值寄存器
};
//
//cpu版本号
#define cn_revision_r0p0   0x410fc230
#define cn_revision_r1p0   0x410fc231
#define cn_revision_r1p1   0x411fc231
#define cn_revision_r2p0   0x412fc320

//定义scb_reg.ICSR 中断控制及状态寄存器，0xE000_ED04 ,bm_为位掩码，bo_为位偏移  
#define bm_scb_icsr_nmipendset  (1<<31)     //rw 写1挂起nmi,读1=nmi挂起
#define bm_scb_icsr_pendsvset   (1<<28)     //rw 写1挂起pendsv，读返回挂起状态
#define bm_scb_icsr_pendsvclr   (1<<27)     //w 写1清除pendsv挂起状态
#define bm_scb_icsr_pendstset   (1<<26)     //rw 写1挂起systick，读返回挂起状态
#define bm_scb_icsr_pendstclr   (1<<25)     //w 写1清除systick挂起状态
#define bm_scb_icsr_isrpending  (1<<22)     //r 有中断挂起（不含nmi）
#define bm_scb_icsr_vectpending (0x3f<<12)  //r 挂起的中断中优先级最高的中断号
#define bm_scb_icsr_vectactive  (0x3f<<0)   //r 正在服务的异常号
#define bo_scb_icsr_nmipendset  31    
#define bo_scb_icsr_pendsvset   28    
#define bo_scb_icsr_pendsvclr   27    
#define bo_scb_icsr_pendstset   26    
#define bo_scb_icsr_pendstclr   25    
#define bo_scb_icsr_isrpending  22    
#define bo_scb_icsr_vectpending 12
#define bo_scb_icsr_vectactive  0  

//AIRCR 应用程序中断及复位控制寄存器，0xE000_ED0c ,bm_为位掩码，bo_为位偏移  
#define bm_scb_aircr_vectkey        (0xffff<<16)
#define bm_scb_aircr_endianess      (1<<15)     
#define bm_scb_aircr_sysresetreq    (1<<2)      
#define bm_scb_aircr_vectclractive  (1<<1)      
#define bo_scb_aircr_vectkey        16  //rw 写本寄存器，必须同时对此域写0x05fa
#define bo_scb_aircr_endianess      15  //r 1=大端，0=小端
#define bo_scb_aircr_sysresetreq    2   //w 写1逻辑复位芯片
#define bo_scb_aircr_vectclractive  1   //w 保留用于调试

//定义scb_reg.SCR 系统控制寄存器,0xE000_ED10 ,bm_为位掩码，bo_为位偏移  
#define bm_scb_scr_sevonpend    (1<<4)      
#define bm_scb_scr_sleepdeep    (1<<2)      
#define bm_scb_scr_sleeponexit  (1<<1)      
#define bo_scb_scr_sevonpend    4      //rw 1=任何中断和事件都可以唤醒处理器
                                       //   0=只有使能的中断和事件可以唤醒处理器
#define bo_scb_scr_sleepdeep    2      //rw 0=普通睡眠，1=深度睡眠
#define bo_scb_scr_sleeponexit  1      //rw 1=从中断返回时进入sleep

//定义scb_reg.CCR 配置与控制寄存器,0xE000_ED14 ,bm_为位掩码，bo_为位偏移  
#define bm_scb_ccr_stkalign         (1<<9) 
#define bm_scb_ccr_unaligntrp       (1<<3) 
#define bo_scb_ccr_stkalign         9   //rw 异常响应自动入栈时强制dp双字对齐
#define bo_scb_ccr_unaligntrp       3   //rw 1=未对齐访问将掉进用法fault

//特别注意:m0的scb寄存器与m3不同，m3可以字节访问，而m0不能字节访问
struct scb_reg
{
    vuc32 CPUID;
    vu32 ICSR;          //中断控制及状态寄存器，0xE000_ED04
    vu32 res0;
    vu32 AIRCR;         //应用程序中断及复位控制寄存器，0xE000_ED0c
    vu32 SCR;           //系统控制寄存器,0xE000_ED10
    vu32 CCR;           //配置与控制寄存器,0xE000_ED14
    vu32 res1;
    vu32 pri8_11;       //异常的优先级,0xE000_ED1c,pri11=svc
    vu32 pri12_15;      //异常的优先级,0xE000_ED20，pri14=pendsv,pri15=systick
    vu32 SHCSR;         //datasheet上无，但nxp的代码上有?
    vu32 res4[2];
    vu32 DFSR;          //调试 fault 状态寄存器(DFSR) 0xE000_ED30
                        //datasheet上无，但nxp的代码上有?
};

#ifdef __cplusplus
}
#endif

#endif /*__CORTEXM0_H__*/


