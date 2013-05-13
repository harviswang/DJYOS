//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 硬件定时器
//作者：lst
//版本：V1.0.0
//文件描述: 硬件定时器操作
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-10-15
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __timer_hard_h__
#define __timer_hard_h__

#ifdef __cplusplus
extern "C" {
#endif

//timer位域，bo_为位偏移，1位的用位带地址，bb_前缀，多位用掩码，bm_前缀
//uart1 sr寄存器位定义
#define bb_tim1_cr1_base   (0x42000000 + 0x13800*32)           //位带基址
#define bb_tim1_cr1_cen    (*(vu32*)(bb_tim1_cr1_base+4*0))
#define bb_tim1_cr1_udis   (*(vu32*)(bb_tim1_cr1_base+4*1))
#define bb_tim1_cr1_urs    (*(vu32*)(bb_tim1_cr1_base+4*2))
#define bb_tim1_cr1_opm    (*(vu32*)(bb_tim1_cr1_base+4*3))
#define bb_tim1_cr1_dir    (*(vu32*)(bb_tim1_cr1_base+4*4))
#define bb_tim1_cr1_cms    0x00000060
#define bb_tim1_cr1_ckd    0x00000300
#define bb_tim1_cr1_cen    0    //0=停止，1=启动计数
#define bb_tim1_cr1_udis   1    //0=允许uev，1=禁止uev  0
#define bb_tim1_cr1_urs    2
#define bb_tim1_cr1_opm    3
#define bb_tim1_cr1_dir    4
#define bb_tim1_cr1_cms    5
#define bb_tim1_cr1_ckd    8

//tim1 at 0x40012c00
//tim2 at 0x40000000
//tim3 at 0x40000400
//tim4 at 0x40000800
//tim5 at 0x40000c00
//tim6 at 0x40001000
//tim7 at 0x40001400
//tim8 at 0x40013400
struct timer_reg
{//             offset
    u32 CR1;    //0x00
    u32 CR2;    //0x04
    u32 SMCR;   //0x08
    u32 DIER;   //0x0c
    u32 SR;     //0x10
    u32 EGR;    //0x14
    u32 CCMR1;  //0x18
    u32 CCMR2;  //0x1c
    u32 CCER;   //0x20
    u32 CNT;    //0x24
    u32 PSC;    //0x28
    u32 ARR;    //0x2c
    u32 RCR;    //0x30
    u32 CCR1;   //0x34
    u32 CCR2;   //0x38
    u32 CCR3;   //0x3c
    u32 CCR4;   //0x40
    u32 BDTR;   //0x44
    u32 DCR;    //0x48 
    u32 DMAR;   //0x4c
};
extern struct timer_reg volatile * const pg_timer_reg;

#ifdef __cplusplus
}
#endif

#endif //__timer_hard_h__
