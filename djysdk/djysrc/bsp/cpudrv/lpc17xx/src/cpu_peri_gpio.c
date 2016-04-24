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
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:���ļ�ʵ����������:
//          1�����Ź���ѡ������MCU�����ţ����Ǹ��õģ�ʹ��ǰ��Ҫ��ȷ��ѡ������
//             �Ĺ��ܣ�������Щϵͳ�����й����л�Ҫ��̬�л����ܣ���ЩCPU�ж�����
//             ģ���������Ź���ѡ����Щ�����Щ���ܸ�����gpioģ���ϣ�djyosͳһ
//             ��gpioģ����ʵ�֡�
//          2��gpio����ģʽ���ã����Ǻ;���Ӳ�������ȷǳ��ߵĹ���.���CPU������
//             ��ѡ��pin���������������������ٶȡ����������ѡ�
//          3����gpio���ŵ�ƽ���Լ�����gpio�������
//          5������δ��֮���ܣ��û�������ӡ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------------------------------

#include "cpu_peri.h"

//----ѡ�����Ź���--------------------------------------------------------------
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
    vu32  *pinsel;
    u32 bitoffset,portoffset;
    pinsel = &LPC_PINCON->PINSEL0;
    portoffset = (port<<1) + (pinnum>>4);
    bitoffset = pinnum % 16 <<1;
    pinsel[portoffset] = (pinsel[portoffset] & (~((u32)0x3<<bitoffset)))
                        | (((u32)0x3 & func_no)<<bitoffset);
}

//----ѡ������ģʽ-------------------------------------------------------------
//����: ѡ��һ�����ŵ�ģʽ��MCU������ⲿ���Ż���Ϊ����port����P0~Px���б�ţ�ÿ
//      ��port���������š�
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      pinnum��port�ڱ�ţ�����Ҫ����P1.5,��pinnum=5.
//      mode��ģʽ��ţ��ɾ���ϵͳ���壬��175x�У�bit0~1��������pinmode��bit2��
//            ������pinmode_od�����Ƿ�©��
//����: ��
//example:
//      ��p0.1����Ϊ����+��©���
//      GPIO_CfgPinMode(0,1,cn_gpio_mode_od_pullup)
//-----------------------------------------------------------------------------
void GPIO_CfgPinMode(u32 port,u32 pinnum,u32 mode)
{
    vu32  *pinmode;
    u32 bitoffset,portoffset;
    //������pinmode�Ĵ���
    pinmode = &LPC_PINCON->PINMODE0;
    portoffset = (port<<1) + (pinnum>>4);
    bitoffset = pinnum % 16 <<1;
    pinmode[portoffset] = (pinmode[portoffset] & (~((u32)0x3<<bitoffset)))
                        | (((u32)0x3 & mode)<<bitoffset);
    //������pinmode_od�Ĵ���
    pinmode = &LPC_PINCON->PINMODE_OD0;
    portoffset = port;
    bitoffset = pinnum;
    pinmode[port] = (pinmode[port] & (~((u32)1<<pinnum)))
                        | (((u32)1 & (mode>>2))<<pinnum);
}

//----����gpioΪ����-----------------------------------------------------------
//����: ��ĳ��gpio��msk=1��Ӧ��gpio����Ϊ����ڣ�����ѡΪgpioʱ����Ч��
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ��Ϊ�����δ����
//����: ��
//example:
//      GPIO_SetToIn(1,0x10000010);        //��p1.4,p1.28��Ϊ����
//-----------------------------------------------------------------------------
void GPIO_SetToIn(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
        	LPC_GPIO0->FIODIR &= ~msk;
            break;
        case 1:
        	LPC_GPIO1->FIODIR &= ~msk;
            break;
        case 2:
        	LPC_GPIO2->FIODIR &= ~msk;
            break;
        case 3:
        	LPC_GPIO3->FIODIR &= ~msk;
            break;
        case 4:
        	LPC_GPIO4->FIODIR &= ~msk;
            break;
        default:break;
    }
}

//----����gpioΪ���-----------------------------------------------------------
//����: ��ĳ��gpio��msk����1��bit��Ӧ��gpio����Ϊ����ڣ�����ѡΪgpioʱ����Ч��
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ��Ϊ�����λ����
//����: ��
//example:
//      GPIO_SetToOut(1,0x10000010);       //��p1.4,p1.28��Ϊ���
//-----------------------------------------------------------------------------
void GPIO_SetToOut(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
        	LPC_GPIO0->FIODIR |= msk;
            break;
        case 1:
        	LPC_GPIO1->FIODIR |= msk;
            break;
        case 2:
        	LPC_GPIO2->FIODIR |= msk;
            break;
        case 3:
        	LPC_GPIO3->FIODIR |= msk;
            break;
        case 4:
        	LPC_GPIO4->FIODIR |= msk;
            break;
        default:break;
    }
}

//----��gpio��ȡ����-----------------------------------------------------------
//����: ��ĳgpio port�������
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//����: ���õ�����
//example:
//      data = GPIO_DataGet(1);        //��p1�ڵ�����
//-----------------------------------------------------------------------------
u32 GPIO_DataGet(u32 port)
{
    u32 temp;
    switch(port)
    {
        case 0:
            temp = LPC_GPIO0->FIOPIN;
            break;
        case 1:
            temp = LPC_GPIO1->FIOPIN;
            break;
        case 2:
            temp = LPC_GPIO2->FIOPIN;
            break;
        case 3:
            temp = LPC_GPIO3->FIOPIN;
            break;
        case 4:
            temp = LPC_GPIO4->FIOPIN;
            break;
        default:break;
    }
    return temp;
}

//----������ݵ�ĳport---------------------------------------------------------
//����: ���������������ĳgpio port
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//      data�������������
//����: ��
//example:
//      GPIO_DataOut(1,data);      //���������p1��
//-----------------------------------------------------------------------------
void GPIO_DataOut(u32 port,u32 data)
{
    switch(port)
    {
        case 0:
        	LPC_GPIO0->FIOPIN = data;
            break;
        case 1:
        	LPC_GPIO1->FIOPIN = data;
            break;
        case 2:
        	LPC_GPIO2->FIOPIN = data;
            break;
        case 3:
        	LPC_GPIO3->FIOPIN = data;
            break;
        case 4:
        	LPC_GPIO4->FIOPIN = data;
            break;
        default:break;
    }
    return ;
}

//----IO������ߵ�ƽ-----------------------------------------------------------
//����: ��port��msk����1��bit��Ӧ��λ����ߵ�ƽ
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ����ߵ�ƽ��λ����
//����: ��
//example:
//      GPIO_SetToHigh(1,0x10000010);      //��p1.4,p1.28����ߵ�ƽ
//-----------------------------------------------------------------------------
void GPIO_SetToHigh(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
        	LPC_GPIO0->FIOSET = msk;
            break;
        case 1:
        	LPC_GPIO1->FIOSET = msk;
            break;
        case 2:
        	LPC_GPIO2->FIOSET = msk;
            break;
        case 3:
        	LPC_GPIO3->FIOSET = msk;
            break;
        case 4:
        	LPC_GPIO4->FIOSET = msk;
            break;
        default:break;
    }
}

//----IO������͵�ƽ-----------------------------------------------------------
//����: ��port��msk����1��bit��Ӧ��λ����͵�ƽ
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ����ߵ�ƽ��λ����
//����: ��
//example:
//      GPIO_SetToLow(1,0x10000010);      //��p1.4,p1.28����͵�ƽ
//-----------------------------------------------------------------------------
void GPIO_SetToLow(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
        	LPC_GPIO0->FIOCLR = msk;
            break;
        case 1:
        	LPC_GPIO1->FIOCLR = msk;
            break;
        case 2:
        	LPC_GPIO2->FIOCLR = msk;
            break;
        case 3:
        	LPC_GPIO3->FIOCLR = msk;
            break;
        case 4:
        	LPC_GPIO4->FIOCLR = msk;
            break;
        default:break;
    }
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
    //ʹ������gpioģ�飬gpioģ��ֻ��һ������λ�����ܰ�portʹ�ܡ�
    LPC_SC->PCONP |= (u32)1<<15;
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
//      GPIO_PowerOff(1);    //��ֹp1����lpc���ܰ�portʹ�ܣ�ʵ���Ͻ�ֹȫ��gpio
//-----------------------------------------------------------------------------
void GPIO_PowerOff(u32 port)
{
    //lpc_17xxû�е�lpc���ܰ�portʹ�ܣ�ʵ���Ͻ�ֹȫ��gpio,Ϊ�˲�Ӱ������ģ�飬
    //��������ִ���κβ���
    return;
}

// =============================================================================
// ���ܣ�����GPIO�ⲿ�жϵĴ���ģʽΪ�����ػ��½��ش���
// ������ port����������port��ţ�����Ҫ����P1.5,��port=1.
//      pinnum��port�ڱ�ţ�����Ҫ����P1.5,��pinnum=5.
//      mode��1��ʾ�����ش�����0��ʾ�½��ش���
// ���أ���
// =============================================================================
void GPIO_CfgIntMode(u32 port,u32 pinnum,u32 mode)
{
	vu32 intmode;
	if((port != 0) && (port != 2))
		return;
	if(port == 0)
	{
		if(mode)			//rising mode
		{
			intmode = LPC_GPIOINT->IO0IntEnR;
			intmode |= 1<<pinnum;
			LPC_GPIOINT->IO0IntEnR = intmode;
		}
		else
		{
			intmode = LPC_GPIOINT->IO0IntEnR;
			intmode &= ~(1<<pinnum);
			LPC_GPIOINT->IO0IntEnR = intmode;
		}
	}
	else
	{
		if(mode)			//rising mode
		{
			intmode = LPC_GPIOINT->IO2IntEnR;
			intmode |= 1<<pinnum;
			LPC_GPIOINT->IO2IntEnR = intmode;
		}
		else
		{
			intmode = LPC_GPIOINT->IO2IntEnR;
			intmode &= ~(1<<pinnum);
			LPC_GPIOINT->IO2IntEnR = intmode;
		}
	}
}

// =============================================================================
// ���ܣ���ȡ�ⲿ�жϵ�״̬��0��ʾ���жϹ��𣬷�0��ʾ�жϹ���
// ������ port����������port��ţ�����Ҫ����P1.5,��port=1.
//      msk�����룬�����1.5��msk = 1<<5
//      mode��1��ʾ�����ش�����0��ʾ�½��ش���
// ���أ�0�����жϣ���0���жϹ���
// =============================================================================
u32 GPIO_GetIntStatus(u32 port,u32 msk,u32 mode)
{
	u32 result = 0xFF;
	if(port == 0)
	{
		if(mode)			//rising mode
		{
			result = LPC_GPIOINT->IO0IntStatR & msk;
		}
		else
		{
			result = LPC_GPIOINT->IO0IntStatF & msk;
		}
	}
	else
	{
		if(mode)			//rising mode
		{
			result = LPC_GPIOINT->IO2IntStatR & msk;
		}
		else
		{
			result = LPC_GPIOINT->IO2IntStatF & msk;
		}
	}
	return result;
}

// =============================================================================
// ���ܣ����ж�״̬λ
// ������ port����������port��ţ�����Ҫ����P1.5,��port=1.
//      msk�����룬�����1.5��msk = 1<<5
// ���أ���
// =============================================================================
void GPIO_ClearIntStatus(u32 port,u32 msk)
{
	if(port == 0)
	{
		LPC_GPIOINT->IO0IntClr |= msk;
	}
	else
	{
		LPC_GPIOINT->IO2IntClr |= msk;
	}
}

