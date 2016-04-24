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
#include "cpu_peri_gpio.h"
#include "cpu_peri_rcc.h"

//----ѡ�����Ź���-------------------------------------------------------------
//����: stm32��io��ƱȽ����⣬��Ϊ�����ʱ�����������������뻹��gpio���룬
//      ������Ϊ���뼴�ɡ���Ϊ���ʱ����Ҫ����gpio��������������������ָ
//      ���������裬ֻ��Ҫ����Ϊ���輴�ɡ�
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      pinnum��port�ڱ�ţ�����Ҫ����P1.5,��pinnum=5.
//      func_no�����ܱ�ţ��� CN_GPIO_MODE_IN_ANALOG ��һ�鳣����
//����: ��
//-----------------------------------------------------------------------------
void GPIO_CfgPinFunc(u32 port,u32 pinnum,u32 func_no)
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

    //���빦�ܵ���������ͨ������Ĵ�����ʵ�ֵġ�
    if(func_no == CN_GPIO_MODE_IN_PULLUP)
    {
        pg_gpio_reg[port]->BSRR = 1<<pinnum;
    }
    else if(func_no == CN_GPIO_MODE_IN_PULLDOWN)
    {
        pg_gpio_reg[port]->BRR = 1<<pinnum;
    }
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
    if(port >6)
        return 0;
    return pg_gpio_reg[port]->IDR;
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
    if(port >6)
        return;
    pg_gpio_reg[port]->ODR = data;
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
    if(port >6)
        return;
    pg_gpio_reg[port]->BSRR = msk & 0xffff;
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
    if(port >6)
        return;
    pg_gpio_reg[port]->BRR = msk;
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
//����: port����������port��ţ�����Ҫ����PA,��port=CN_GPIO_A.
//����: ��
//example:
//      GPIO_PowerOn(1);      //ʹ��p1��
//-----------------------------------------------------------------------------
void GPIO_PowerOn(u32 port)
{
    //stm32û�йر�gpio��Դ�Ĺ��ܣ�ֻ�ܹص�������ǵ�ʱ�ӡ�
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
//      GPIO_PowerOff(1);    //��ֹp1
//-----------------------------------------------------------------------------
void GPIO_PowerOff(u32 port)
{
    //stm32û�йر�gpio��Դ�Ĺ��ܣ�ֻ�ܹص�������ǵ�ʱ�ӡ�
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



