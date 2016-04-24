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
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �����жϺ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009--9-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE_H__
#define __CPU_PERI_INT_LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define cn_int_line_wakeup0         0x00  //wakeup from gpio0.0
#define cn_int_line_wakeup1         0x01  //wakeup from gpio0.1
#define cn_int_line_wakeup2         0x02  //wakeup from gpio0.2
#define cn_int_line_wakeup3         0x03  //wakeup from gpio0.3
#define cn_int_line_wakeup4         0x04  //wakeup from gpio0.4
#define cn_int_line_wakeup5         0x05  //wakeup from gpio0.5
#define cn_int_line_wakeup6         0x06  //wakeup from gpio0.6
#define cn_int_line_wakeup7         0x07  //wakeup from gpio0.7
#define cn_int_line_wakeup8         0x08  //wakeup from gpio0.8
#define cn_int_line_wakeup9         0x09  //wakeup from gpio0.9
#define cn_int_line_wakeup10        0x0A  //wakeup from gpio0.10
#define cn_int_line_wakeup11        0x0B  //wakeup from gpio0.11
#define cn_int_line_iic             0x0C
#define cn_int_line_timer16_0       0x0D
#define cn_int_line_timer16_1       0x0E
#define cn_int_line_timer32_0       0x0F
#define cn_int_line_timer32_1       0x10
#define cn_int_line_ssp             0x11
#define cn_int_line_uart0           0x12
#define cn_int_line_uart1           0x13
#define cn_int_line_comparator      0x14
#define cn_int_line_adc             0x15
#define cn_int_line_wdt             0x16
#define cn_int_line_bod             0x17
#define cn_int_line_fmc             0x18
#define cn_int_line_PIOINT0         0x19
#define cn_int_line_PIOINT1         0x1A
#define cn_int_line_PIOINT2         0x1B
#define cn_int_line_pmu             0x1C
#define cn_int_line_dma             0x1D
#define cn_int_line_rtc             0x1E
#define cn_int_line_edm             0x1F
#define CN_INT_LINE_LAST            0x1F

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE_H__

