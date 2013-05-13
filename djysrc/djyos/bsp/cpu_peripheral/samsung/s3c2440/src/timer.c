//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 硬件定时器
//作者：lst
//版本：V1.1.0
//文件描述: 用于44b0硬件定时器操作
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者: lst
//   新版本号: V1.1.0
//   修改说明: 原djyos.c中定时功能与硬件相关的部分转移到这里。
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 移植自44b0版本
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

//2440的定时器功能简述:
//1、减计数方式工作
//2、每个定时器可选5个时钟源，分别为主频的1/2分频~1/16分频，或EXTCLK。
//3、有两个8位预分频器，01共享一个，234共享一个。
//4、每个定时器都有比较寄存器，用于产生PWM波形。
//5、每个定时器可以设定为单次运行和重复运行，可以手动启动和停止。

void timer_init(void)
{
    //DMA:No select(All Interrupt),
    pg_timer_reg->TCFG1 = 0;
}

//----设置定时器时钟源----------------------------------------------------------
//功能: 设置各定时器时钟源.
//参数: timer,定时器号
//      value,定时器输入时钟源,timer和value的对应表如下:
//      value:  0           1           2            3          4
//      timer0: 1/2分频     1/4分频     1/8分频      1/16分频   EXTCLK0
//      timer1: 1/2分频     1/4分频     1/8分频      1/16分频   EXTCLK0
//      timer2: 1/2分频     1/4分频     1/8分频      1/16分频   EXTCLK1
//      timer3: 1/2分频     1/4分频     1/8分频      1/16分频   EXTCLK1
//      timer4: 1/2分频     1/4分频     1/8分频      1/16分频   EXTCLK1
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_clk_source(ufast_t timer,ufast_t value)
{
    pg_timer_reg->TCFG1 &= ~(0xf << (timer<<2));
    pg_timer_reg->TCFG1 |= value << (timer<<2);
}

//----设置定时器预分频数-------------------------------------------------------
//功能: 设置各定时器预分频数.
//参数: group,定时器组号,01为1组,234为1组
//      value,定时器预分频数,0~255对应1~256分频
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_precale(ufast_t group,uint16_t value)
{
    pg_timer_reg->TCFG0 &= ~(0xff << (group<<3));
    pg_timer_reg->TCFG0 |= value << (group<<3);
}

//----设置定时器计数值----------------------------------------------------------
//功能: 设置各定时器计数值.定时器的溢出中断时间为:value*(预分频+1)/时钟源分频数
//参数: timer,定时器号
//      value,计数值
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_counter(ufast_t timer,uint16_t value)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCNTB0 = value;
            break;
        case 1:
            pg_timer_reg->TCNTB1 = value;
            break;
        case 2:
            pg_timer_reg->TCNTB2 = value;
            break;
        case 3:
            pg_timer_reg->TCNTB3 = value;
            break;
        case 4:
            pg_timer_reg->TCNTB4 = value;
            break;
        default:break;
    }
}

//----设置定时器比较值----------------------------------------------------------
//功能: 用于pwm占空比设计
//参数: timer,定时器号
//      value,比较值
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_compare(ufast_t timer,uint16_t value)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCMPB0 = value;
            break;
        case 1:
            pg_timer_reg->TCMPB1 = value;
            break;
        case 2:
            pg_timer_reg->TCMPB2 = value;
            break;
        case 3:
            pg_timer_reg->TCMPB3 = value;
            break;
        default:break;
    }
}

//----设置定时器工作方式-------------------------------------------------------
//功能: 设定定时器是连续工作还是单次工作
//参数: timer,定时器号
//      type,0=单次工作,1=自动加载连续工作
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_type(ufast_t timer,ufast_t type)
{
    type &= 0x1;
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON &= ~(1<<3);
            pg_timer_reg->TCON |= type<<3;
            break;
        case 1:
            pg_timer_reg->TCON &= ~(1<<11);
            pg_timer_reg->TCON |= type<<11;
            break;
        case 2:
            pg_timer_reg->TCON &= ~(1<<15);
            pg_timer_reg->TCON |= type<<15;
            break;
        case 3:
            pg_timer_reg->TCON &= ~(1<<19);
            pg_timer_reg->TCON |= type<<19;
            break;
        case 4:
            pg_timer_reg->TCON &= ~(1<<22);
            pg_timer_reg->TCON |= type<<22;
            break;
        default:break;
    }
}

//----重载定时器计数和比较值---------------------------------------------------
//功能: 手动重新加载定时器的计数和比较寄存器值
//参数: timer,定时器号
//返回: 无
//-----------------------------------------------------------------------------
void timer_reload(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON |= 1<<1;
            pg_timer_reg->TCON &= ~(1<<1);
            break;
        case 1:
            pg_timer_reg->TCON |= 1<<9;
            pg_timer_reg->TCON &= ~(1<<9);
            break;
        case 2:
            pg_timer_reg->TCON |= 1<<13;
            pg_timer_reg->TCON &= ~(1<<13);
            break;
        case 3:
            pg_timer_reg->TCON |= 1<<17;
            pg_timer_reg->TCON &= ~(1<<17);
            break;
        case 4:
            pg_timer_reg->TCON |= 1<<21;
            pg_timer_reg->TCON &= ~(1<<21);
            break;
        default:break;
    }
}

//----启动定时器--------------------------------------------------------
//功能: 启动定时器
//参数: timer,定时器号
//返回: 无
//-----------------------------------------------------------------------------
void timer_start(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON |=1;
            break;
        case 1:
            pg_timer_reg->TCON |=1<<8;
            break;
        case 2:
            pg_timer_reg->TCON |=1<<12;
            break;
        case 3:
            pg_timer_reg->TCON |=1<<16;
            break;
        case 4:
            pg_timer_reg->TCON |=1<<20;
            break;
        default:break;
    }
}

//----停止定时器--------------------------------------------------------
//功能: 停止定时器
//参数: timer,定时器号
//返回: 无
//-----------------------------------------------------------------------------
void timer_stop(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON &=~1;
            break;
        case 1:
            pg_timer_reg->TCON &=~(1<<8);
            break;
        case 2:
            pg_timer_reg->TCON &=~(1<<12);
            break;
        case 3:
            pg_timer_reg->TCON &=~(1<<16);
            break;
        case 4:
            pg_timer_reg->TCON &=~(1<<20);
            break;
        default:break;
    }
}

//----读定时器当前值--------------------------------------------------------
//功能: 读出定时器的当前计数值
//参数: timer,定时器号
//返回: 当前计数值
//-----------------------------------------------------------------------------
uint16_t timer_read(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            return pg_timer_reg->TCNTO0;
            break;
        case 1:
            return pg_timer_reg->TCNTO1;
            break;
        case 2:
            return pg_timer_reg->TCNTO2;
            break;
        case 3:
            return pg_timer_reg->TCNTO3;
            break;
        case 4:
            return pg_timer_reg->TCNTO4;
            break;
        default:break;
    }
    return 0;
}

