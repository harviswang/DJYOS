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
#include "gpio_api.h"

//lpc12xx的每个gpio引脚都有一个配置寄存器，用于选择引脚功能，
const u8 u8c_iocon_select_table[] =
{
     17, 18, 19, 21, 22, 23, 24, 25,   26, 27, 36, 37, 38, 39, 40, 41,
     42, 43, 44,  2,  3,  4,  5,  6,    7,  8,  9, 10, 15, 16, 45, 46,
    
     47, 48, 49, 50, 51, 52, 53,255,  255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,  255,255,255,255,255,255,255,255,
    
     28, 29, 30, 31, 32, 33, 34, 35,   56, 57, 58, 59, 11, 12, 13, 14,
    255,255,255,255,255,255,255,255,  255,255,255,255,  0,  1,255,255,
};
union un_iocfg_bit  * const pg_iocfg_reg = 
                        (union un_iocfg_bit *)0x40044000;

//----选择引脚功能-------------------------------------------------------------
//功能: 选择一个引脚的功能，MCU都会把外部引脚划分为若干port，从P0~Px进行编号，每
//      个port有若干引脚。
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//          如果port=cn_invalid_port，则:
//          1、如果该cpu没有未按gpio编号的可配置功能引脚，则不执行任何操作。
//          2、如果该cpu有未按gpio编号的可配置功能引脚，则pinnum自编序号，
//             func_no含义不变。不建议pinnum使用cpu引脚号，因为同一款芯片不同封
//             装引脚好未必相同，特别是BGA封装，你还的定义行列转换规则。
//      pinnum，port内编号，比如要操作P1.5,则pinnum=5.
//      func_no，功能编号，由具体系统定义
//返回: 无
//example:
//      gpio_cfg_pin_func(0,1,cn_p0_0_txd3)     //把p0.1设置为uart3的txd
//-----------------------------------------------------------------------------
void gpio_cfg_pin_func(u32 port,u32 pinnum,u32 func_no)
{
    if((port >3) || (pinnum >32))
        return;
    pg_iocfg_reg[u8c_iocon_select_table[port*32 + pinnum]].bit.func
                                    = func_no;
}

//----选择引脚模式-------------------------------------------------------------
//功能: 选择一个引脚的模式，MCU都会把外部引脚划分为若干port，从P0~Px进行编号，每
//      个port有若干引脚。
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//          如果port=cn_invalid_port，则:
//          1、如果该cpu没有未按gpio编号的可配置功能引脚，则不执行任何操作。
//          2、如果该cpu有未按gpio编号的可配置功能引脚，则pinnum自编序号，
//             func_no含义不变。不建议pinnum使用cpu引脚号，因为同一款芯片不同封
//             装引脚好未必相同，特别是BGA封装，你还的定义行列转换规则。
//      pinnum，port内编号，比如要操作P1.5,则pinnum=5.
//      mode，模式编号，由具体系统定义，在lpc_12xx中，按union un_iocfg_bit const
//            结构组合mode。
//返回: 无
//example:
//      设置p0.1的模式
//      gpio_cfg_pin_mode(0,1,IOCON_PIO_MODE_PULLUP
//                            + IOCON_PIO_INV_NOT
//                            + IOCON_PIO_AD_DIGITAL
//                            + IOCON_PIO_DRV_2MA_12M
//                            + IOCON_PIO_OD_ENABLE
//                            + IOCON_PIO_SMODE_BYPASS
//                            + IOCON_PIO_SEL_FILTER0);
//-----------------------------------------------------------------------------
void gpio_cfg_pin_mode(u32 port,u32 pinnum,u32 mode)
{
    union un_iocfg_bit iocfg;
    u32 offset;
    if((port >3) || (pinnum >32))
        return;
    offset = port*32 + pinnum;
    iocfg.dw = mode;
    iocfg.bit.func = pg_iocfg_reg[u8c_iocon_select_table[offset]].bit.func;
    pg_iocfg_reg[u8c_iocon_select_table[offset]] = iocfg;
}

//----设置gpio为输入-----------------------------------------------------------
//功能: 把某组gpio中msk=1对应的gpio口设为输入口，引脚选为gpio时才有效。
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      mask，port中需要设为输入的未掩码
//返回: 无
//example:
//      gpio_setto_in(1,0x10000010);        //把p1.4,p1.28设为输入
//-----------------------------------------------------------------------------
void gpio_setto_in(u32 port,u32 msk)
{
    if(port >3)
        return;
    pg_gpio_reg[port]->DIR &= ~msk;
}

//----设置gpio为输出-----------------------------------------------------------
//功能: 把某组gpio中msk中是1的bit对应的gpio口设为输出口，引脚选为gpio时才有效。
//参数: port，被操作的port编号，比如要操作P1.5,则port=1.
//      mask，port中需要设为输入的位掩码
//返回: 无
//example:
//      gpio_setto_out(1,0x10000010);       //把p1.4,p1.28设为输出
//-----------------------------------------------------------------------------
void gpio_setto_out(u32 port,u32 msk)
{
    if(port >3)
        return;
    pg_gpio_reg[port]->DIR |= msk;
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
    if(port >3)
        return 0;
    return pg_gpio_reg[port]->PIN;
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
    if(port >3)
        return;
    pg_gpio_reg[port]->PIN = data;
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
    if(port >3)
        return;
    pg_gpio_reg[port]->SET = msk;
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
    if(port >3)
        return;
    pg_gpio_reg[port]->CLR = msk;
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
//      gpio_power_on(1);      //使能p1，但lpc不能按port使能，实际上使能全部gpio
//-----------------------------------------------------------------------------
void gpio_power_on(u32 port)
{
    //lpc_12xx没有关闭gpio电源的功能，只能关掉或打开他们的时钟。
    switch(port)
    {
        case 0:
            sys_enable_peri_clk(cn_peri_clksrc_gpio0);
            break;
        case 1:
            sys_enable_peri_clk(cn_peri_clksrc_gpio1);
            break;
        case 2:
            sys_enable_peri_clk(cn_peri_clksrc_gpio2);
            break;
        case 3:
            sys_enable_peri_clk(cn_peri_clksrc_gpio3);
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
//      gpio_power_off(1);    //禁止p1，
//-----------------------------------------------------------------------------
void gpio_power_off(u32 port)
{
    //lpc_12xx没有但lpc不能按port使能，实际上禁止全部gpio,为了不影响其他模块，
    //本函数不执行任何操作
    return;
}


