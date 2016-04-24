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
//========================================================  
// �ļ�ģ��˵��: ϵͳ���ƼĴ�������
// �ļ��汾: v1.00                                          
// ������Ա: lst                                            
// ����ʱ��: 2011.06.01                                     
// Copyright(c) 2011-2011  ��������������޹�˾             
//========================================================  
// �����޸ļ�¼(���µķ�����ǰ��):                            
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>            
//========================================================  
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��

#ifndef __CPU_PERI_SYSCTRL_H__
#define __CPU_PERI_SYSCTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

struct st_sysctrl_reg
{
    vu32 flashcfg;               // flash accelerator module           
    u32 reserved0[31];
    vu32 pll0con;                // clocking and power control         
    vu32 pll0cfg;
    vu32 pll0stat;
    vu32 pll0feed;
    u32 reserved1[4];
    vu32 pll1con;
    vu32 pll1cfg;
    vu32 pll1stat;
    vu32 pll1feed;
    u32 reserved2[4];
    vu32 pcon;
    vu32 pconp;
    u32 reserved3[15];
    vu32 cclkcfg;
    vu32 usbclkcfg;
    vu32 clksrcsel;
    u32 reserved4[12];
    vu32 extint;                 // external interrupts                
    u32 reserved5;
    vu32 extmode;
    vu32 extpolar;
    u32 reserved6[12];
    vu32 rsid;                   // reset                              
    u32 reserved7[7];
    vu32 scs;                    // syscon miscellaneous registers     
    vu32 irctrim;                // clock dividers                     
    vu32 pclksel0;
    vu32 pclksel1;
    u32 reserved8[4];
    vu32 usbintst;               // usb device/otg interrupt register  
    u32 reserved9;
    vu32 clkoutcfg;              // clock output configuration         
 };

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_SYSCTRL_H__

