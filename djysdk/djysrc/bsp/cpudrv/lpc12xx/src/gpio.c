//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:IO��ʼ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"

//lpc12xx��ÿ��gpio���Ŷ���һ�����üĴ���������ѡ�����Ź��ܣ�
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

//----ѡ�����Ź���-------------------------------------------------------------
//����: ѡ��һ�����ŵĹ��ܣ�MCU������ⲿ���Ż���Ϊ����port����P0~Px���б�ţ�ÿ
//      ��port���������š�
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      pinnum��port�ڱ�ţ�����Ҫ����P1.5,��pinnum=5.
//      func_no�����ܱ�ţ��ɾ���ϵͳ����
//����: ��
//example:
//      GPIO_CfgPinFunc(0,1,cn_p0_0_txd3)     //��p0.1����Ϊuart3��txd
//-----------------------------------------------------------------------------
void GPIO_CfgPinFunc(u32 port,u32 pinnum,u32 func_no)
{
    if((port >3) || (pinnum >32))
        return;
    pg_iocfg_reg[u8c_iocon_select_table[port*32 + pinnum]].bit.func
                                    = func_no;
}

//----ѡ������ģʽ-------------------------------------------------------------
//����: ѡ��һ�����ŵ�ģʽ��MCU������ⲿ���Ż���Ϊ����port����P0~Px���б�ţ�ÿ
//      ��port���������š�
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      pinnum��port�ڱ�ţ�����Ҫ����P1.5,��pinnum=5.
//      mode��ģʽ��ţ��ɾ���ϵͳ���壬��lpc_12xx�У���union un_iocfg_bit const
//            �ṹ���mode��
//����: ��
//example:
//      ����p0.1��ģʽ
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

//----����gpioΪ����-----------------------------------------------------------
//����: ��ĳ��gpio��msk=1��Ӧ��gpio����Ϊ����ڣ�����ѡΪgpioʱ����Ч��
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ��Ϊ�����δ����
//����: ��
//example:
//      gpio_setto_in(1,0x10000010);        //��p1.4,p1.28��Ϊ����
//-----------------------------------------------------------------------------
void gpio_setto_in(u32 port,u32 msk)
{
    if(port >3)
        return;
    pg_gpio_reg[port]->DIR &= ~msk;
}

//----����gpioΪ���-----------------------------------------------------------
//����: ��ĳ��gpio��msk����1��bit��Ӧ��gpio����Ϊ����ڣ�����ѡΪgpioʱ����Ч��
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ��Ϊ�����λ����
//����: ��
//example:
//      gpio_setto_out(1,0x10000010);       //��p1.4,p1.28��Ϊ���
//-----------------------------------------------------------------------------
void gpio_setto_out(u32 port,u32 msk)
{
    if(port >3)
        return;
    pg_gpio_reg[port]->DIR |= msk;
}

//----��gpio��ȡ����-----------------------------------------------------------
//����: ��ĳgpio port�������
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//����: ���õ�����
//example:
//      data = GPIO_GetData(1);        //��p1�ڵ�����
//-----------------------------------------------------------------------------
u32 GPIO_GetData(u32 port)
{
    if(port >3)
        return 0;
    return pg_gpio_reg[port]->PIN;
}

//----������ݵ�ĳport---------------------------------------------------------
//����: ���������������ĳgpio port
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//      data�������������
//����: ��
//example:
//      GPIO_OutData(1,data);      //���������p1��
//-----------------------------------------------------------------------------
void GPIO_OutData(u32 port,u32 data)
{
    if(port >3)
        return;
    pg_gpio_reg[port]->PIN = data;
}

//----IO������ߵ�ƽ-----------------------------------------------------------
//����: ��port��msk����1��bit��Ӧ��λ����ߵ�ƽ
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ����ߵ�ƽ��λ����
//����: ��
//example:
//      GPIO_SettoHigh(1,0x10000010);      //��p1.4,p1.28����ߵ�ƽ
//-----------------------------------------------------------------------------
void GPIO_SettoHigh(u32 port,u32 msk)
{
    if(port >3)
        return;
    pg_gpio_reg[port]->SET = msk;
}

//----IO������͵�ƽ-----------------------------------------------------------
//����: ��port��msk����1��bit��Ӧ��λ����͵�ƽ
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ����ߵ�ƽ��λ����
//����: ��
//example:
//      GPIO_SettoHigh(1,0x10000010);      //��p1.4,p1.28����͵�ƽ
//-----------------------------------------------------------------------------
void GPIO_SettoLow(u32 port,u32 msk)
{
    if(port >3)
        return;
    pg_gpio_reg[port]->CLR = msk;
}

//----ʹ��gpioģ��-------------------------------------------------------------
//����: ʹ��gpioģ�飬���cpuΪ�˿��ƹ��ģ�������ģ�鶼�ǿ��Ա��رյģ�gpioģ��
//      Ҳ����ˡ���Щcpu������gpioģ��һ�����ã���Щ���ǲ�ͬport�������á�����
//      û�н�����Ƶ�cpu������������Ϊ�ռ��ɡ�
//      ���ƹ��ĵĹ���ʵ�ַ����������������:
//      1����һ��ͳһ�Ĺ��Ĺ���ģ�顣
//      2�����Ĺ����ܷ�ɢ��������ģ����ȥ��
//      ��������ͳһ�Ĺ��Ĺ���ģ�飬djyos����������������������ʵ��һ�׿�����
//      ���ĵĺ�������ͳһ�Ĺ��Ŀ���ģ�����ṩ���û������������Ƿ���ͳһ��
//      ���Ŀ���ģ�飬Ӧ�ó�����һ�µġ�
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//����: ��
//example:
//      GPIO_PowerOn(1);      //ʹ��p1����lpc���ܰ�portʹ�ܣ�ʵ����ʹ��ȫ��gpio
//-----------------------------------------------------------------------------
void GPIO_PowerOn(u32 port)
{
    //lpc_12xxû�йر�gpio��Դ�Ĺ��ܣ�ֻ�ܹص�������ǵ�ʱ�ӡ�
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

//----��ֹgpioģ��-------------------------------------------------------------
//����: ʹ��gpioģ�飬���cpuΪ�˿��ƹ��ģ�������ģ�鶼�ǿ��Ա��رյģ�gpioģ��
//      Ҳ����ˡ���Щcpu������gpioģ��һ�����ã���Щ���ǲ�ͬport�������á�����
//      û�н�����Ƶ�cpu������������Ϊ�ռ��ɡ�
//      ���ƹ��ĵĹ���ʵ�ַ����������������:
//      1����һ��ͳһ�Ĺ��Ĺ���ģ�顣
//      2�����Ĺ����ܷ�ɢ��������ģ����ȥ��
//      ��������ͳһ�Ĺ��Ĺ���ģ�飬djyos����������������������ʵ��һ�׿�����
//      ���ĵĺ�������ͳһ�Ĺ��Ŀ���ģ�����ṩ���û������������Ƿ���ͳһ��
//      ���Ŀ���ģ�飬Ӧ�ó�����һ�µġ�
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//����: ��
//example:
//      GPIO_PowerOff(1);    //��ֹp1��
//-----------------------------------------------------------------------------
void GPIO_PowerOff(u32 port)
{
    //lpc_12xxû�е�lpc���ܰ�portʹ�ܣ�ʵ���Ͻ�ֹȫ��gpio,Ϊ�˲�Ӱ������ģ�飬
    //��������ִ���κβ���
    return;
}


