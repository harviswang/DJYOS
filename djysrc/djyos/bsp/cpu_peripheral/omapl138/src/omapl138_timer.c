//========================================================  
// 文件模块说明:                                            
// omapl138定时器操作函数                         
// 文件版本: v1.00                                          
// 开发人员: lst                                            
// 创建时间: 2011.06.01                                     
// Copyright(c) 2011-2011  深圳鹏瑞软件有限公司             
//========================================================  
// 程序修改记录(最新的放在最前面):							
//  <版本号> <修改日期>, <修改人员>: <修改功能概述>			
//========================================================  
//例如：
// V1.02 2004-07-14, 张三: 接收函数增加对报文长度有效性校核

#include "os_inc.h"
#include "gpio_api.h"
#include "cpu_peri.h"

//omapl138的定时器功能简述:
//有个64位定时器，每个64位定时器又可以作为2个32位定时器(t12和t34)。
//Timer64P1的64位模式可作为看门狗使用，但32位模式不可以。
//32位方式，又分为两两连接成菊花链的方式和独立32位方式。
//菊花链方式，实际上是把t34作为t12的预分频器。
//独立模式下，t34有一个4bit的预分频器，t12没有。
//非菊花链模式下，t12有8个32位的比较寄存器

tagTimerReg *__timer_get_timer_reg(u32 t64_no)
{
    switch(t64_no)
    {
        case 0:
            return (tagTimerReg *)g_ptTimer0Reg;
        case 1:
            return (tagTimerReg *)g_ptTimer1Reg;
        case 2:
            return (tagTimerReg *)g_ptTimer2Reg;
        case 3:
            return (tagTimerReg *)g_ptTimer3Reg;
        default:
            return NULL;
    }
}


//----设置64还是双32位模式-----------------------------------------------------
//功能: 设置定时器工作在64位模式还是双32位模式下。
//参数: t64_no,被设置的64位timer，0~3
//      mode,工作模式，0=64bit，1=双32模式，2=64位看门狗(仅t64-1),3=32位链接式。
//返回: 无
//-----------------------------------------------------------------------------
void timer_counter_len_set(u32 t64_no,ufast_t mode)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    timer->TGCR &= ~((u32)0x0c);
    timer->TGCR |= mode<<2;
}

//----使能/禁止中断------------------------------------------------------------
//功能: 使能或禁止timer产生中断，注意，这里只是控制timer模块本身，要想响应timer
//      中断，还要在中断控制器中也使能相应的中断线。
//参数: t64_no,被设置的64位timer，0~3
//      msk,被操作的中断功能:
//          =EVTINTEN34  t34事件捕获中断
//          =PRDINTEN34  t34周期匹配中断
//          =EVTINTEN12  t12事件捕获中断
//          =PRDINTEN12  t12周期匹配中断
//      en，false=禁止，true=使能
//返回: 无
//-----------------------------------------------------------------------------
void timer_int_ctrl(u32 t64_no,u32 msk,bool_t en)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(en)
        timer->INTCTLSTAT |= msk;
    else
        timer->INTCTLSTAT &= ~msk;
}

//----设置时钟源---------------------------------------------------------------
//功能: 设置定时器的时钟源，内部晶振或者外部引脚。
//参数: t64_no,被设置的64位timer，0~3
//      sub，被设置的是哪个32位定时器:0=t12或者64位模式，1=t34
//      src，时钟源，0=内部时钟，1=外部时钟
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_clksrc(u32 t64_no,u32 sub,u32 src)
{
    tagTimerReg * timer;
    u32 offset;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(sub ==0 )
        offset = CLKSRC12_64_OFF;
    else 
        offset = CLKSRC34_OFF;
    
    if(src == 0)
        timer->TCR &= ~(u32)(1<<offset);
    else
        timer->TCR |= 1<<offset;
}

//----设置工作模式-------------------------------------------------------------
//功能: 设置定时器的工作模式。
//参数: t64_no,被设置的64位timer，0~3
//      sub，被设置的是哪个32位定时器:0=t12，1=t34
//      mode,工作模式，0=不工作，1=单次计数，2=周期工作，3=周期工作并重载
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_mode(u32 t64_no,u32 sub,u32 mode)
{
    tagTimerReg * timer;
    u32 offset;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(sub ==0 )
        offset = ENAMODE12_OFF;
    else 
        offset = ENAMODE34_OFF;

    timer->TCR &= ~(u32)(0x3<<offset);
    timer->TCR |= mode<<offset;
}

//----设置定时周期-------------------------------------------------------------
//功能: 设置定时周期
//参数: t64_no,被设置的64位timer，0~3
//      sub，被设置的是哪个32位定时器:0=t12，1=t34
//      value,周期值
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_Period(u32 t64_no,u32 sub,u32 value)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(sub == 0)
        timer->PRD12 = value;
    else
        timer->PRD34 = value;
}

//----设置重载周期-------------------------------------------------------------
//功能: 设置重载周期，当定时器工作在周期性重载方式时有用
//参数: t64_no,被设置的64位timer，0~3
//      sub，被设置的是哪个32位定时器:0=t12，1=t34
//      value,周期值
//返回: 无
//-----------------------------------------------------------------------------
void timer_set_reload(u32 t64_no,u32 sub,u32 value)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(sub == 0)
        timer->REL12 = value;
    else
        timer->REL34 = value;
}

//----停止定时器-------------------------------------------------------------
//功能: 停止定时器
//参数: t64_no,被设置的64位timer，0~3
//      sub，被设置的是哪个32位定时器:0=t12，1=t34，2=t64
//返回: 无
//-----------------------------------------------------------------------------
void timer_stop(u32 t64_no,u32 sub)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(sub == 0)
        timer->TGCR &= ~(u32)1;
    else if(sub == 1)
        timer->TGCR &= ~(u32)2;
    else
        timer->TGCR &= ~(u32)3;
}

//----启动定时器-------------------------------------------------------------
//功能: 启动定时器
//参数: t64_no,被设置的64位timer，0~3
//      sub，被设置的是哪个32位定时器:0=t12，1=t34，2=t64
//返回: 无
//-----------------------------------------------------------------------------
void timer_start(u32 t64_no,u32 sub)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(sub == 0)
        timer->TGCR |= 1;
    else if(sub == 1)
        timer->TGCR |= 2;
    else
        timer->TGCR |= 3;
}

//----清除计数值---------------------------------------------------------------
//功能: 清除定时器当前计数值，
//参数: t64_no,被设置的64位timer，0~3
//      sub，清除哪个32位定时器:0=t12，1=t34
//返回: 无
//注: 定时器运行中无法清除计数值，必须先停下。
//-----------------------------------------------------------------------------
void timer_clear(u32 t64_no,u32 sub)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return;
    timer = __timer_get_timer_reg(t64_no);
    if(sub == 0)
        timer->TIM12 = 0;
    else if(sub == 1)
        timer->TIM34 = 0;
}

//----读计数值---------------------------------------------------------------
//功能: 清除定时器当前计数值，
//参数: t64_no,被设置的64位timer，0~3
//      sub，读哪个32位定时器:0=t12，1=t34
//返回: 计数值
//-----------------------------------------------------------------------------
u32 timer_read(u32 t64_no,u32 sub)
{
    tagTimerReg * timer;
    if(t64_no > 3)
        return 0;
    timer = __timer_get_timer_reg(t64_no);
    if(sub == 0)
        return timer->TIM12;
    else
        return timer->TIM34;
}


