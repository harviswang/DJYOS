//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:IO初始化
//作者：lst
//版本：V1.0.0
//文件描述:IO初始化
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

//----选择引脚功能-------------------------------------------------------------
//功能: stm32中，配置引脚功能和模式的过程，完全是混合的，gpio_cfg_pin_func、
//      gpio_cfg_pin_mode、gpio_setto_in、gpio_setto_out几个函数均难于单独实现，
//      这里用一个函数实现全部功能.stm32的io设计比较特殊，作为输入端时，不用区分外设输入
//		还是gpio输入，均设置为输入即可。作为输出时，需要区分gpio输出和外设输出，但无需指
//		定具体外设，只需要设置为外设即可。
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      pinnum，port内编号，比如要操作P1.5,则pinnum=5.
//      func_no，功能编号，见 cn_gpio_mode_in_analog 那一组常量。
//返回: 无
//-----------------------------------------------------------------------------
void gpio_cfg_pin_func(u32 port,u32 pinnum,u32 func_no)
{
    if(port >6)
        return;

    if(pinnum < 8 )
    {
        pg_gpio_reg[port]->CRL  = (pg_gpio_reg[port]->CRL & (~(0xf << (pinnum<<2))))
                                    | ((func_no & 0xf)<< (pinnum<<2));
    }
    else
    {
        pg_gpio_reg[port]->CRH  =(pg_gpio_reg[port]->CRH &(~(0xf <<((pinnum-8)<<2))))
                                    | ((func_no & 0xf)<< ((pinnum-8)<<2));
    }
    
    //输入功能的上下拉是通过输出寄存器来实现的。
    if(func_no == cn_gpio_mode_in_pullup)
    {
        pg_gpio_reg[port]->BSRR = 1<<pinnum;
    }
    else if(func_no == cn_gpio_mode_in_pullup)
    {
        pg_gpio_reg[port]->BRR = 1<<pinnum;
    }
}

//----选择引脚模式-------------------------------------------------------------
//功能: 参见gpio_cfg_pin_func函数
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      pinnum，port内编号，比如要操作P1.5,则pinnum=5.
//      func_no，功能编号，由具体系统定义
//返回: 无
//-----------------------------------------------------------------------------
void gpio_cfg_pin_mode(u32 port,u32 pinnum,u32 mode)
{
    
}


//----设置gpio为输入-----------------------------------------------------------
//功能: 参见gpio_cfg_pin_func函数
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      mask，port中需要设为输入的未掩码
//返回: 无
//-----------------------------------------------------------------------------
void gpio_setto_in(u32 port,u32 msk)
{
}

//----设置gpio为输出-----------------------------------------------------------
//功能: 参见gpio_cfg_pin_func函数
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      mask，port中需要设为输入的位掩码
//返回: 无
//-----------------------------------------------------------------------------
void gpio_setto_out(u32 port,u32 msk)
{
}

//----从gpio读取数据-----------------------------------------------------------
//功能: 把某gpio port整体读入
//参数: port，被操作的port编号，比如要操作P1,则port=1.
//返回: 读得的数据
//example:
//      data = gpio_get_data(1);        //读p1口的数据
//-----------------------------------------------------------------------------
u32 gpio_get_data(u32 port)
{
    if(port >6)
        return 0;
    return pg_gpio_reg[port]->IDR;
}

//----输出数据到某port---------------------------------------------------------
//功能: 把数据整体输出到某gpio port
//参数: port，被操作的port编号，比如要操作P1,则port=1.
//      data，待输出的数据
//返回: 无
//example:
//      gpio_out_data(1,data);      //数据输出到p1口
//-----------------------------------------------------------------------------
void gpio_out_data(u32 port,u32 data)
{
    if(port >6)
        return;
    pg_gpio_reg[port]->ODR = data;
}

//----IO口输出高电平-----------------------------------------------------------
//功能: 在port中msk中是1的bit对应的位输出高电平
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      mask，port中需要输出高电平的位掩码
//返回: 无
//example:
//      gpio_setto_high(1,0x10000010);      //在p1.4,p1.28输出高电平
//-----------------------------------------------------------------------------
void gpio_setto_high(u32 port,u32 msk)
{
    if(port >6)
        return;
    pg_gpio_reg[port]->BSRR = msk & 0xffff;
}

//----IO口输出低电平-----------------------------------------------------------
//功能: 在port中msk中是1的bit对应的位输出低电平
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      mask，port中需要输出高电平的位掩码
//返回: 无
//example:
//      gpio_setto_high(1,0x10000010);      //在p1.4,p1.28输出低电平
//-----------------------------------------------------------------------------
void gpio_setto_low(u32 port,u32 msk)
{
    if(port >6)
        return;
    pg_gpio_reg[port]->BRR = msk;
}

//----使能gpio模块-------------------------------------------------------------
//功能: 使能gpio模块，许多cpu为了控制功耗，其外设模块都是可以被关闭的，gpio模块
//      也是如此。有些cpu是整个gpio模块一起设置，有些则是不同port独立设置。对于
//      没有节能设计的cpu，本函数保持为空即可。
//      控制功耗的功能实现方法，不外乎两大类:
//      1、有一个统一的功耗管理模块。
//      2、功耗管理功能分散到各功能模块中去。
//      无论有无统一的功耗管理模块，djyos建议各外设驱动程序均自行实现一套控制自
//      身功耗的函数，而统一的功耗控制模块则不提供给用户。这样无论是否有统一的
//      功耗控制模块，应用程序都是一致的。
//参数: port，被操作的port编号，比如要操作P1,则port=1.
//返回: 无
//example:
//      gpio_power_on(1);      //使能p1，
//-----------------------------------------------------------------------------
void gpio_power_on(u32 port)
{
    //stm32没有关闭gpio电源的功能，只能关掉或打开他们的时钟。
    switch(port)
    {
        case 0:
            bb_rcc_apb2enr_iopaen = 1;
        break;
        case 1:
            bb_rcc_apb2enr_iopben = 1;
            break;
        case 2:
            bb_rcc_apb2enr_iopcen = 1;
            break;
        case 3:
            bb_rcc_apb2enr_iopden = 1;
            break;
        case 4:
            bb_rcc_apb2enr_iopeen = 1;
            break;
        case 5:
            bb_rcc_apb2enr_iopfen = 1;
            break;
        case 6:
            bb_rcc_apb2enr_iopgen = 1;
            break;

        default:break;
    }
}

//----禁止gpio模块-------------------------------------------------------------
//功能: 使能gpio模块，许多cpu为了控制功耗，其外设模块都是可以被关闭的，gpio模块
//      也是如此。有些cpu是整个gpio模块一起设置，有些则是不同port独立设置。对于
//      没有节能设计的cpu，本函数保持为空即可。
//      控制功耗的功能实现方法，不外乎两大类:
//      1、有一个统一的功耗管理模块。
//      2、功耗管理功能分散到各功能模块中去。
//      无论有无统一的功耗管理模块，djyos建议各外设驱动程序均自行实现一套控制自
//      身功耗的函数，而统一的功耗控制模块则不提供给用户。这样无论是否有统一的
//      功耗控制模块，应用程序都是一致的。
//参数: port，被操作的port编号，比如要操作P1,则port=1.
//返回: 无
//example:
//      gpio_power_off(1);    //禁止p1
//-----------------------------------------------------------------------------
void gpio_power_off(u32 port)
{
    //stm32没有关闭gpio电源的功能，只能关掉或打开他们的时钟。
    switch(port)
    {
        case 0:
            bb_rcc_apb2enr_iopaen = 0;
        break;
        case 1:
            bb_rcc_apb2enr_iopben = 0;
            break;
        case 2:
            bb_rcc_apb2enr_iopcen = 0;
            break;
        case 3:
            bb_rcc_apb2enr_iopden = 0;
            break;
        case 4:
            bb_rcc_apb2enr_iopeen = 0;
            break;
        case 5:
            bb_rcc_apb2enr_iopfen = 0;
            break;
        case 6:
            bb_rcc_apb2enr_iopgen = 0;
            break;

        default:break;
    }
}



