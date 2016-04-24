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
//����ģ��: CPUϵͳ����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ����ʱ�ӡ���Դ�ȿ��ƴ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "stdint.h"
#include "cpu_peri.h"

//----ʹ��һ������-----------------------------------------------------------
//����: ʹ������,
//����: module���������������ţ���cpu_peri_sysctrl.h�ļ��ж���
//����: ��
//-----------------------------------------------------------------------------
void sys_enable_peri(u32 module)
{
    pg_sysctrl_reg->PRESETCTRL |= (u32)1 << module;
}

//----��ֹһ������-----------------------------------------------------------
//����: ʹ������,
//����: module��������������ʱ��Դ��ţ���cpu_peri_sysctrl.h�ļ��ж���
//����: ��
//-----------------------------------------------------------------------------
void sys_disable_peri(u32 module)
{
    pg_sysctrl_reg->PRESETCTRL &= ~((u32)1 << module);
}

//----ʹ��һ������ʱ��---------------------------------------------------------
//����: ʹ������ʱ��,
//����: module��������������ʱ��Դ��ţ���cpu_peri_sysctrl.h�ļ��ж���
//����: ��
//-----------------------------------------------------------------------------
void sys_enable_peri_clk(u32 module)
{
    pg_sysctrl_reg->SYSAHBCLKCTRL |= (u32)1 << module;
}

//----��ֹһ������ʱ��-----------------------------------------------------------
//����: ʹ������ʱ��,
//����: module���������������ţ���cpu_peri_sysctrl.h�ļ��ж���
//����: ��
//-----------------------------------------------------------------------------
void sys_disable_peri_clk(u32 module)
{
    pg_sysctrl_reg->SYSAHBCLKCTRL &= ~((u32)1 << module);
}

//----����uart0ʱ�ӳ���--------------------------------------------------------
//����: ����uart0ʱ�ӳ�����uart������ʱ��= cn_mclk / dev_num
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_uart0_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->UART0CLKDIV = (dev_num & 0xFF);
}

//----����uart1ʱ�ӳ���--------------------------------------------------------
//����: ����uart1ʱ�ӳ�����uart������ʱ��= cn_mclk / dev_num��0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_uart1_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->UART1CLKDIV = (dev_num & 0xFF);
}

//----����io�˲�����ʱ�ӳ���0--------------------------------------------------
//����: lpc12xx��7��ʱ������io�����뿹�����˲���ÿ��ʱ�Ӵ���Ƶ��Ƶ����Ƶ���ɱ���
//      �������ã�ÿ��io�ڿ��Զ���ѡ������һ��ʱ�ӣ�0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_iofilter0_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->FILTERCLKCFG0 = (dev_num & 0xFF);
}

//----����io�˲�����ʱ�ӳ���1--------------------------------------------------
//����: lpc12xx��7��ʱ������io�����뿹�����˲���ÿ��ʱ�Ӵ���Ƶ��Ƶ����Ƶ���ɱ���
//      �������ã�ÿ��io�ڿ��Զ���ѡ������һ��ʱ�ӣ�0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_iofilter1_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->FILTERCLKCFG1 = (dev_num & 0xFF);
}

//----����io�˲�����ʱ�ӳ���2--------------------------------------------------
//����: lpc12xx��7��ʱ������io�����뿹�����˲���ÿ��ʱ�Ӵ���Ƶ��Ƶ����Ƶ���ɱ���
//      �������ã�ÿ��io�ڿ��Զ���ѡ������һ��ʱ�ӣ�0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_iofilter2_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->FILTERCLKCFG2 = (dev_num & 0xFF);
}

//----����io�˲�����ʱ�ӳ���3--------------------------------------------------
//����: lpc12xx��7��ʱ������io�����뿹�����˲���ÿ��ʱ�Ӵ���Ƶ��Ƶ����Ƶ���ɱ���
//      �������ã�ÿ��io�ڿ��Զ���ѡ������һ��ʱ�ӣ�0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_iofilter3_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->FILTERCLKCFG3 = (dev_num & 0xFF);
}

//----����io�˲�����ʱ�ӳ���4--------------------------------------------------
//����: lpc12xx��7��ʱ������io�����뿹�����˲���ÿ��ʱ�Ӵ���Ƶ��Ƶ����Ƶ���ɱ���
//      �������ã�ÿ��io�ڿ��Զ���ѡ������һ��ʱ�ӣ�0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_iofilter4_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->FILTERCLKCFG4 = (dev_num & 0xFF);
}

//----����io�˲�����ʱ�ӳ���5--------------------------------------------------
//����: lpc12xx��7��ʱ������io�����뿹�����˲���ÿ��ʱ�Ӵ���Ƶ��Ƶ����Ƶ���ɱ���
//      �������ã�ÿ��io�ڿ��Զ���ѡ������һ��ʱ�ӣ�0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_iofilter5_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->FILTERCLKCFG5 = (dev_num & 0xFF);
}

//----����io�˲�����ʱ�ӳ���6--------------------------------------------------
//����: lpc12xx��7��ʱ������io�����뿹�����˲���ÿ��ʱ�Ӵ���Ƶ��Ƶ����Ƶ���ɱ���
//      �������ã�ÿ��io�ڿ��Զ���ѡ������һ��ʱ�ӣ�0=��ֹ��ʱ��
//����: dev_num
//����: ��
//-----------------------------------------------------------------------------
void sys_set_iofilter6_clkdiv(u32 dev_num)
{
    pg_sysctrl_reg->FILTERCLKCFG6 = (dev_num & 0xFF);
}

//----������ID-----------------------------------------------------------
//����: ������ID
//����: ��
//����: �������:
//          LPC1227FBD64/301 = 0x3670 002B
//          LPC1227FBD48/301 = 0x3670 002B
//          LPC1226FBD64/301 = 0x3660 002B
//          LPC1226FBD48/301 = 0x3660 002B
//          LPC1225FBD64/321 = 0x3652 002B
//          LPC1225FBD64/301 = 0x3650 002B
//          LPC1225FBD48/321 = 0x3652 002B
//          LPC1225FBD48/301 = 0x3650 002B
//          LPC1224FBD64/121 = 0x3642 C02B
//          LPC1224FBD64/101 = 0x3640 C02B
//          LPC1224FBD48/121 = 0x3642 C02B
//          LPC1224FBD48/101 = 0x3640 C02B
//-----------------------------------------------------------------------------
u32 sys_get_deviceID(void)
{
    return(pg_sysctrl_reg->DEVICE_ID);
}

