//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 原始输入输出
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 标准输入输出口需要使用操作系统功能，本文件提供一个在初始化完成之前的
//          最基本的输入输出功能
//其他说明:
//修订历史:
//1. 日期:2009-07-22
//   作者:  罗侍田.
//   新版本号：1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <string.h>
#include "os_inc.h"
#include "cpu_peri.h"

/*
static volatile struct uart_reg *pg_uart0_reg
                        = (struct uart_reg *)0x50000000;
uint8_t scan_byte_nude(void)
{
    uint32_t fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart0_reg->UFSTAT;
        if((fifo_reg & 0x3f) != 0)
            return pg_uart0_reg->URXH;
    }
}
bool_t read_byte_nude(uint8_t *byte)
{
    uint32_t fifo_reg;
    fifo_reg = pg_uart0_reg->UFSTAT;
    if((fifo_reg & 0x3f) != 0)
    {
        *byte = pg_uart0_reg->URXH;
        return true;
    }else
        return false;
}

void printf_byte_nude(uint8_t byte)
{
    uint32_t fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart0_reg->UFSTAT;
        if(((fifo_reg>>8) & 0x3f) < 0x3f)
        {
            pg_uart0_reg->UTXH = byte;
            return ;
        }
    }
}

void printf_str_nude(uint8_t *str)
{
    uint32_t i=0;
    while(str[i] != 0)
    {
        printf_byte_nude(str[i]);
        i++;
    }
}

void printf_hex_nude(uint32_t hex,uint32_t size)
{
    uint8_t uni;
    if((size < 1 )|| (size >8))
        return ;
    for(;size > 0; size--)
    {
        uni = (hex >>(size*4-4))&0xf;
        if(uni < 10)
            printf_byte_nude(uni+0x30);
        else
            printf_byte_nude(uni+0x37);
    }
}

bool_t nude_terminal_init(void)
{
    uint32_t temp;
    //初始化IO端口位uart功能
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal :   xx    xx   xx   xx   xx   xx   xx  RXD0 TXD0 nRTS0 nCTS0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx   xx   xx
    temp = g_ptGpioReg->GPHCON;
    temp &= ~0xff;
    temp |= 0xaa;
    g_ptGpioReg->GPHCON = temp;
    g_ptGpioReg->GPHUP |= 0xf;   // The pull up function is disabled GPH[3:0]

    pg_uart0_reg->ULCON = 0x3;
    pg_uart0_reg->UCON =  0x5;
    pg_uart0_reg->UFCON = 0x57;   //bit0=1使能fifo.
    pg_uart0_reg->UMCON = 0x0;
    pg_uart0_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    return true;
}

*/
