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


#ifndef __LCDDRV_H_
#define __LCDDRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include	"gkernel.h"

#define cn_lcd_xsize        320
#define cn_lcd_ysize        240
//#define cn_lcd_line_size    (cn_lcd_xsize*2)


#define cn_frame_buffer_size    (cn_lcd_xsize*cn_lcd_ysize*2)
//hjj
//#define cn_lcd_frame_rate       60
#define cn_lcd_pf       CN_SYS_PF_RGB565

#define LCDCON4_MVAL        (13)
#define LCDCON1_MMODE       (0)         //0=ÿ֡   1=��MVAL�����Ƶ��
#define LCDCON5_INVVDEN     (1)         //0=����       1=��ת
#define LCDCON5_BSWP        (0)         //�ֽڽ�������λ 
#define LCDCON5_HWSWP       (1)         //���ֽڽ�������λ 
#define LCDCON1_PNRMODE     (3)         // ����ΪTFT��
#define LCDCON1_BPPMODE     (12)        // ����Ϊ16bppģʽ

#define LCDCON2_VBPD        (12)        //��ֱͬ���źŵĺ��
#define LCDCON2_VFPD        (4)         //��ֱͬ���źŵ�ǰ��
#define LCDCON2_VSPW        (5)         //��ֱͬ���źŵ�����

#define LCDCON3_HBPD        (22)        //ˮƽͬ���źŵĺ��
#define LCDCON3_HFPD        (33)        //ˮƽͬ���źŵ�ǰ��
#define LCDCON4_HSPW        (44)        //ˮƽͬ���źŵ�����

#define LCDCON1_CLKVAL      (6) 

//hjj
#define HOZVAL_TFT          (cn_lcd_xsize-1)
#define LINEVAL_TFT         (cn_lcd_ysize-1)


// LCD CONTROLLER

#define	LCD_REG_BASE	0x4D000000

struct lcd_reg            //0x4d000000
{
    vu32 LCDCON1;            //0x00,LCD����1�Ĵ���
    vu32 LCDCON2;            //0x04,LCD����2�Ĵ���
    vu32 LCDCON3;            //0x08,LCD����3�Ĵ���
    vu32 LCDCON4;            //0x0c,LCD����4�Ĵ���
    vu32 LCDCON5;            //0x10,LCD����5�Ĵ���
    vu32 LCDSADDR1;          //0x14,֡��������ʼ��ַ1�Ĵ���
    vu32 LCDSADDR2;          //0x18,֡��������ʼ��ַ2�Ĵ���
    vu32 LCDSADDR3;          //0x1c,��������ַ����
    vu32 REDLUT;             //0x20,��ɫ���ұ�Ĵ���
    vu32 GREENLUT;           //0x24,��ɫ���ұ�Ĵ���
    vu32 BLUELUT;            //0x28,��ɫ���ұ�Ĵ���
    vu32 revs[8];
    vu32 DITHMODE;           //0x4c,����ģʽ�Ĵ���
    vu32 TPAL;               //0x50,��ʱ��ɫ��Ĵ���
    vu32 LCDINTPND;          //0x54,LCD�жϹ���Ĵ���
    vu32 LCDSRCPND;          //0x58,LCDԴ����Ĵ���
    vu32 LCDINTMSK;          //0x5c,LCD�ж����μĴ���
    vu32 TCONSEL;            //0x60,���� LPC3600/LCC3600ģʽ�Ĵ���
    
};
#define PALETTE         0x4d000400            //0x400,��ɫ�忪ʼ��ַ


ptu32_t module_init_lcd(s32 width,s32 height,const char *DisplayName,const char* HeapName);

#ifdef __cplusplus
}
#endif

#endif   //__LCDDRV_H_
