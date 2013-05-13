//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//版权所有 (C), 2005-2009,   lst.
//所属模块:  公共
//作者：     lst
//版本：      V1.0.0
//初始版本完成日期：2009-08-05
//文件描述:  cortex-m3寄存器的头文件
//其他说明:  无
//修订历史:
//2. ...
//1. 日期: 2009-08-05
//   作者: lst
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#ifndef __CM3_H__
#define __CM3_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum IRQn
{
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt                   */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt                           */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt                         */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
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
#define bo_systick_ctrl_tickint     1    //rw,1=SysTick 数到0 时产生SysTick 异常请求
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
#define bm_scb_icsr_nmipendset  (1<<31)     //rw 写1挂起nmi
#define bm_scb_icsr_pendsvset   (1<<28)     //rw 写1挂起pendsv，读返回挂起状态
#define bm_scb_icsr_pendsvclr   (1<<27)     //w 写1清除pendsv挂起状态
#define bm_scb_icsr_pendstset   (1<<26)     //rw 写1挂起systick，读返回挂起状态
#define bm_scb_icsr_pendstclr   (1<<25)     //w 写1清除systick挂起状态
#define bm_scb_icsr_isrpreempt  (1<<23)     //r 调试用
#define bm_scb_icsr_isrpending  (1<<22)     //r 有中断挂起（不含nmi）
#define bm_scb_icsr_vectpending (0x3ff<<12) //r 挂起的中断中优先级最高的中断号
#define bm_scb_icsr_rettobase   (1<<11)     //r 1=执行异常服务程序且没有嵌套。
                                            //  0=主程序中或嵌套异常服务中
#define bm_scb_icsr_vectactive  (0xff<<0)   //r 正在服务的异常号
#define bo_scb_icsr_nmipendset  31    
#define bo_scb_icsr_pendsvset   28    
#define bo_scb_icsr_pendsvclr   27    
#define bo_scb_icsr_pendstset   26    
#define bo_scb_icsr_pendstclr   25    
#define bo_scb_icsr_isrpreempt  23    
#define bo_scb_icsr_isrpending  22    
#define bo_scb_icsr_vectpending 12
#define bo_scb_icsr_rettobase   11    
#define bo_scb_icsr_vectactive  0  

//AIRCR 应用程序中断及复位控制寄存器，0xE000_ED0c ,bm_为位掩码，bo_为位偏移  
#define bm_scb_aircr_vectkey        (0xffff<<16)
#define bm_scb_aircr_endianess      (1<<15)     
#define bm_scb_aircr_prigroup       (7<<8)      
#define bm_scb_aircr_sysresetreq    (1<<2)      
#define bm_scb_aircr_vectclractive  (1<<1)      
#define bm_scb_aircr_vectreset      (1<<0)      
#define bo_scb_aircr_vectkey        16  //rw 写本寄存器，必须同时对此域写0x05fa
#define bo_scb_aircr_endianess      15  //r 1=大端，0=小端
#define bo_scb_aircr_prigroup       8   //rw 本域+1=子优先级占用位数
#define bo_scb_aircr_sysresetreq    2   //w 写1逻辑复位芯片
#define bo_scb_aircr_vectclractive  1   //w 写1清除所有异常活动状态信息。
#define bo_scb_aircr_vectreset      0   //w 写1复位cm3内核，不影响内核以外电路

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
#define bm_scb_ccr_bfhfnmign        (1<<8) 
#define bm_scb_ccr_div0trp          (1<<4) 
#define bm_scb_ccr_unaligntrp       (1<<3) 
#define bm_scb_ccr_usersetmpend     (1<<1) 
#define bm_scb_ccr_nonbasethrdena   (1<<0) 
#define bo_scb_ccr_stkalign         9   //rw 异常响应自动入栈时强制dp双字对齐
#define bo_scb_ccr_bfhfnmign        8   //rw 1=硬fault和nmi服务中忽略数据总线fault
#define bo_scb_ccr_div0trp          4   //rw 1=使能除o用法fault
#define bo_scb_ccr_unaligntrp       3   //rw 1=未对齐访问将掉进用法fault
#define bo_scb_ccr_usersetmpend     1   //rw 1=允许用户代码设置stir
#define bo_scb_ccr_nonbasethrdena   0   //rw,写1允许异服程序修改exc_return进入线程模式

//定义scb_reg.SHCSR位掩码，0xE000ED24,bm_为位掩码，bo_为位偏移  
#define bm_scb_shcsr_usgfaultena    (1<<18)  
#define bm_scb_shcsr_busfaultena    (1<<17)  
#define bm_scb_shcsr_memfaultena    (1<<16)  
#define bm_scb_shcsr_svcallpended   (1<<15)  
#define bm_scb_shcsr_busfaultpended (1<<14)  
#define bm_scb_shcsr_memfaultpended (1<<13)  
#define bm_scb_shcsr_usgfaultpended (1<<12)  
#define bm_scb_shcsr_systickact     (1<<11)  
#define bm_scb_shcsr_pendsvact      (1<<10)  
#define bm_scb_shcsr_monitoract     (1<<8)   
#define bm_scb_shcsr_svcallact      (1<<7)   
#define bm_scb_shcsr_usgfaultact    (1<<3)   
#define bm_scb_shcsr_busfaultact    (1<<1)   
#define bm_scb_shcsr_memfaultact    (1<<0)   
#define bo_scb_shcsr_usgfaultena    18  //rw,b,1 = 使能用法 fault。
#define bo_scb_shcsr_busfaultena    17  //rw,b,1 = 使能总线 fault。
#define bo_scb_shcsr_memfaultena    16  //rw,b,1 = 使能存储器管理 fault。
#define bo_scb_shcsr_svcallpended   15  //rw,b,1 = SVC 悬起中。
#define bo_scb_shcsr_busfaultpended 14  //rw,b,1 = 总线 fault 悬起中
#define bo_scb_shcsr_memfaultpended 13  //rw,b,1 = 存储器管理 fault 悬起中
#define bo_scb_shcsr_usgfaultpended 12  //rw,b,1 = 用法 fault 悬起中
#define bo_scb_shcsr_systickact     11  //rw,b,1 = SysTick 异常活动中
#define bo_scb_shcsr_pendsvact      10  //rw,b,1 = PendSV 异常活动中
#define bo_scb_shcsr_monitoract     8   //rw,b,1 = Devug Monitor 异常活动中
#define bo_scb_shcsr_svcallact      7   //rw,b,1= SVC 异常活动中
#define bo_scb_shcsr_usgfaultact    3   //rw,b,1= 用法 fault 异常活动中
#define bo_scb_shcsr_busfaultact    1   //rw,b,1 = 总线 fault 异常活动中
#define bo_scb_shcsr_memfaultact    0   //rw,b,1 = 存储器管理 fault 异常活动中

//MFSR 存储器管理 fault 状态寄存器,0xE000_ED28 ,bm_为位掩码，bo_为位偏移  
#define bm_scb_mfsr_MMARVALID   (1<<7)   
#define bm_scb_mfsr_STKERR      (1<<4)   
#define bm_scb_mfsr_UNSTKERR    (1<<3)   
#define bm_scb_mfsr_DACCVIOL    (1<<1)   
#define bm_scb_mfsr_IACCVIOL    (1<<0)   
#define bo_scb_mfsr_MMARVALID   7      //r,存储器管理fault地址寄存器（MMAR）有效
#define bo_scb_mfsr_STKERR      4      //r/Wc,入栈时发生错误
#define bo_scb_mfsr_UNSTKERR    3      //r/Wc,出栈时发生错误
#define bo_scb_mfsr_DACCVIOL    1      //r/Wc,数据访问违例
#define bo_scb_mfsr_IACCVIOL    0      //r/Wc,取址访问违例

//定义scb_reg.BFSR 总线 fault 状态寄存器,0xE000_ED29 ,bm_为位掩码，bo_为位偏移  
#define bm_scb_bfsr_BFARVALID   (1<<7)   
#define bm_scb_bfsr_STKERR      (1<<4)   
#define bm_scb_bfsr_UNSTKERR    (1<<3)   
#define bm_scb_bfsr_IMPREISERR  (1<<2)   
#define bm_scb_bfsr_PRECISERR   (1<<1)   
#define bm_scb_bfsr_IACCVIOL    (1<<0)   
#define bo_scb_bfsr_BFARVALID   7    //r,总线fault地址寄存器（BFAR）有效
#define bo_scb_bfsr_STKERR      4    //r/Wc,入栈时发生错误
#define bo_scb_bfsr_UNSTKERR    3    //r/Wc,出栈时发生错误
#define bo_scb_bfsr_IMPREISERR  2    //r/Wc,不精确的数据访问违例（violation）
#define bo_scb_bfsr_PRECISERR   1    //r/Wc,精确的数据访问违例（violation）
#define bo_scb_bfsr_IACCVIOL    0    //r/Wc,取址访问违例

//UFSR 用法 fault 状态寄存器，地址：0xE000_ED2A ,bm_为位掩码，bo_为位偏移  
#define bm_scb_ufsr_DIVBYZERO   (1<<9) 
#define bm_scb_ufsr_UNALIGNED   (1<<8) 
#define bm_scb_ufsr_NOCP        (1<<3) 
#define bm_scb_ufsr_INVPC       (1<<2) 
#define bm_scb_ufsr_INVSTATE    (1<<1) 
#define bm_scb_ufsr_UNDEFINSTR  (1<<0) 
#define bo_scb_ufsr_DIVBYZERO   9   //r/Wc,表示除法运算时除数为零
#define bo_scb_ufsr_UNALIGNED   8   //r/Wc,未对齐访问导致的 fault?
#define bo_scb_ufsr_NOCP        3   //r/Wc,试图执行协处理器相关指令?
#define bo_scb_ufsr_INVPC       2   //r/Wc,在异常返回时EXC_RETURN非法
#define bo_scb_ufsr_INVSTATE    1   //r/Wc,试图切入 ARM 状态
#define bo_scb_ufsr_UNDEFINSTR  0   //r/Wc,执行的指令其编码是未定义的

//定义scb_reg.HFSR 硬 fault 状态寄存器  0xE000_ED2C ,bm_为位掩码，bo_为位偏移  
#define bm_scb_hfsr_DEBUGEVT    (1<<31)
#define bm_scb_hfsr_FORCED      (1<<30)
#define bm_scb_hfsr_VECTBL      (1<<1) 
#define bo_scb_hfsr_DEBUGEVT    31  //r/Wc,因调试事件而产生
#define bo_scb_hfsr_FORCED      30  //r/Wc,是总线 fault，存储器管理
                                    //    fault 或是用法 fault 上访的结果
#define bo_scb_hfsr_VECTBL      1    //r/Wc,在取向量时发生的
struct scb_reg
{
    vuc32 CPUID;
    vu32 ICSR;          //中断控制及状态寄存器，0xE000_ED04
    vu32 VTOR;          //向量表偏移量寄存器，0xE000_ED08
    vu32 AIRCR;         //应用程序中断及复位控制寄存器，0xE000_ED0c
    vu32 SCR;           //系统控制寄存器,0xE000_ED10
    vu32 CCR;           //配置与控制寄存器,0xE000_ED14
    vu8 mmf_pri4;       //存储器管理 fault 的优先级,0xE000_ED18
    vu8 bus_pri5;       //总线 fault 的优先级,0xE000_ED19
    vu8 usr_pri6;       //用法 fault 的优先级,0xE000_ED1a
    vu8 res1[4];
    vu8 svc_pri11;      //svc 异常的优先级,0xE000_ED1f
    vu8 dbg_pri12;      //调试监视器的优先级,0xE000_ED20
    vu8 res2[1];
    vu8 pendsv_pri14;   //pendsv 异常的优先级,0xE000_ED22
    vu8 systick_pri15;  //systick的优先级,0xE000_ED23
    vu32 SHCSR;
    vu8 MFSR;           //存储器管理 fault 状态寄存器(MFSR),0xE000_ED28
    vu8 BFSR;           //总线 fault 状态寄存器(MFSR),0xE000_ED29
    vu16 UFSR;          //用法 fault 状态寄存器(UFSR)，地址：0xE000_ED2A
    vu32 HFSR;          //硬 fault 状态寄存器      0xE000_ED2C
    vu32 DFSR;          //调试 fault 状态寄存器(DFSR)    0xE000_ED30
    vu32 MMFAR;         //发生存储管理fault的地址寄存器(MMAR)    0xE000_ED34
    vu32 BFAR;          //发生总线fault的地址寄存器(MMAR)    0xE000_ED38
    vu32 AFAR;          //由芯片制造商决定（可选）    0xE000_ED3C
};

#define STIR_reg_addr   0xe000ef00

extern struct scb_reg volatile * const pg_scb_reg;

#ifdef __cplusplus
}
#endif

#endif /*__CM3_H__*/


