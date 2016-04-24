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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
//����ģ��: bsp
//���ߣ�������
//�汾��V1.0.0
//�ļ�����: cortex-m�Ĺ�������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����  : 2014/12/19
//   �޸���: ������
//   �°汾��:
//   �޸�˵��: �½�
//------------------------------------------------------

#ifndef __CORTEX_M_H__
#define __CORTEX_M_H__
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

//------------------------ SystemTick ----------------------------------------
//CTRL �жϿ��Ƽ�״̬�Ĵ���?0xE000_E010,bm_Ϊλ����?bo_Ϊλƫ��
#define bm_systick_ctrl_countflag   (1<<16) //rc,����ֵ��Ϊ0ʱ�Զ���1?������
#define bm_systick_ctrl_clksource   (1<<2)  //rw,ʱ��ѡ��?0=�ⲿʱ��stclk?1=�ں�fclk
#define bm_systick_ctrl_tickint     (1<<1)  //rw,1=SysTick ����0 ʱ����SysTick �쳣����
#define bm_systick_ctrl_enable      (1<<0)  //rw,SysTick ��ʱ����ʹ��λ
#define bo_systick_ctrl_countflag   16   //rc,����ֵ��Ϊ0ʱ�Զ���1?������
#define bo_systick_ctrl_clksource   2    //rw,ʱ��ѡ��?0=�ⲿʱ��stclk?1=�ں�fclk
#define bo_systick_ctrl_tickint     1    //rw,1=SysTick ����0 ʱ����SysTick �쳣����
#define bo_systick_ctrl_enable      0    //rw,SysTick ��ʱ����ʹ��λ

//CTRL SysTickУ׼��ֵ�Ĵ��� 0xE000_E01c,bm_Ϊλ����?bo_Ϊλƫ��
#define bm_systick_calib_NOREF  (1<<31)  //r,1=STCLK ������,0=����
#define bm_systick_calib_SKEW   (1<<30)  //r,1=У׼ֵ����׼ȷ��10ms?0=��
#define bo_systick_calib_NOREF  31  //r,1=STCLK ������,0=����
#define bo_systick_calib_SKEW   30  //r,1=У׼ֵ����׼ȷ��10ms?0=��
//rw,10ms ��ʱ���ڵ������ĸ�����оƬ�����Ӧ��ͨ��Cortex�\M3 �������ź��ṩ����ֵ��
//����ֵ������?���ʾ�޷�ʹ��У׼����
#define bm_systick_calib_TENMS  (0xffffff<<0)
#define bo_systick_calib_TENMS  0

struct tagSystickReg      //0xe000e010
{
    vu32 ctrl;
    vu32 reload;    //����������ʱ?������װ�ص�ֵ
    vu32 current;   //��ȡʱ���ص�ǰ��������ֵ?д������?����CTRL�е�COUNTFLAG ��־
    vuc32 verify;   //SysTickУ׼��ֵ�Ĵ���
};



#ifdef __cplusplus
}
#endif

#endif /*__CORTEX_M_H__*/

