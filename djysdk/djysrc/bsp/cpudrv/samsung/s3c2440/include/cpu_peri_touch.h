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

#ifndef __CPU_PERI_TOUCH_H__
#define __CPU_PERI_TOUCH_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
  �˴�Ϊ�����ӵĶ��壬���ڴ�����
  2012.09.04
*/

// ����LCD������������LCD_Type�Ķ��壬�Ӷ�ȷ����������������
#include "cpu_peri_lcd.h"


#define TOUCH_X1_BIT            5               // X1��ADC�˿�
#define TOUCH_X2_BIT            7               // X2��ADC�˿�
#define TOUCH_Y1_BIT            4               // Y1��ADC�˿�
#define TOUCH_Y2_BIT            6               // Y2��ADC�˿�

#if (LCD_Type == LCDW35)
#define TOUCH_WIDTH             320             // ��������ˮƽ�ֱ���
#define TOUCH_HEIGHT            240             // �������Ĵ�ֱ�ֱ���
#define TOUCH_DEFAULT_LB        80              // ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_RB        940             // �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_TB        130             // �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_BB        900             // �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)

#elif (LCD_Type == LCDW43)
#define TOUCH_WIDTH             480             // ��������ˮƽ�ֱ���
#define TOUCH_HEIGHT            272             // �������Ĵ�ֱ�ֱ���
#define TOUCH_DEFAULT_LB        70   //80       // ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_RB        970  //950      // �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_TB        150  //200      //x, �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_BB        900  //880      //x, �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)

#elif (LCD_Type == LCDA70)
#define TOUCH_WIDTH             800             // ��������ˮƽ�ֱ���
#define TOUCH_HEIGHT            480             // �������Ĵ�ֱ�ֱ���
#define TOUCH_DEFAULT_LB        65              // ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_RB        960             // �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_TB        135             // �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_BB        905             // �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#endif


#define TOUCH_DEBOUNCE_PIXEL    8  // ȥ�����������������������ȶ��ڰ뾶Ϊ�������صķ�Χ�ڲ�������
#define TOUCH_DEBOUNCE_TIME             5       // ȥ����ʱ�䣬��λΪ32*ADCת��ʱ��

///////////////////////////////////////////////////////////////////
//      ���¹̶�
#define TOUCH_X1                        (0x0001<<TOUCH_X1_BIT)
#define TOUCH_X2                        (0x0001<<TOUCH_X2_BIT)
#define TOUCH_Y1                        (0x0001<<TOUCH_Y1_BIT)
#define TOUCH_Y2                        (0x0001<<TOUCH_Y2_BIT)

/*
bool_t touch_hard_init(void);
ufast_t touch_scan_hard(struct tagSingleTouchMsg *touch_data);
void touch_ratio_adjust(char *display_name,struct tagST_TouchAdjust *adjust);
*/

#ifdef __cplusplus
}
#endif

#endif /* __CPU_PERI_TOUCH_H__ */
