//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ԭʼ�������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��׼�����������Ҫʹ�ò���ϵͳ���ܣ����ļ��ṩһ���ڳ�ʼ�����֮ǰ��
//          ������������������
//����˵��:
//�޶���ʷ:
//1. ����:2009-07-22
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ԭʼ�汾
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
    //��ʼ��IO�˿�λuart����
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
    pg_uart0_reg->UFCON = 0x57;   //bit0=1ʹ��fifo.
    pg_uart0_reg->UMCON = 0x0;
    pg_uart0_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    return true;
}

*/
