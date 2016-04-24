//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �������ظ�ʽ���弰�ڻ�������ʾ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-26
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GK_DRAW_H__
#define __GK_DRAW_H__
#include "gk_syscall.h"
#ifdef __cplusplus
extern "C" {
#endif

//ֱ��д����֧��rop����
bool_t __GK_RopIsNeedDestination(u32 RopCode);
u32 __GK_PointSect(s32 x,s32 y,struct tagRectangle *limit);
u32 __GK_OlineSectInter(struct tagRectangle *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2);
void __GK_DrawOline(struct tagGkWinRsc *gkwin,s32 x1,s32 y1,
                        s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_GetRgbBm(u16 PixelFormat,u32 color,u32 *a,u32 *r,
                                            u32 *g,u32 *b,ptu32_t ExColor);
void __GK_CopyPixelRopBm(struct tagRectBitmap *dst_bitmap,
                        struct tagRectBitmap *src_bitmap,
                        s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                        u32 RopCode,u32 KeyColor);
void __GK_SetPixelRop2Bm(struct tagRectBitmap *bitmap,
                        s32 x,s32 y,u32 color,u32 rop2_code);
void __GK_CopyPixelBm(struct tagRectBitmap *dst_bitmap,
                                struct tagRectBitmap *src_bitmap,
                                s32 x_dst,s32 y_dst,s32 x_src,s32 y_src);
void GK_SetPixel(struct tagGkscParaSetPixel *para);
void __GK_ShadingPixel(struct tagGkWinRsc *gkwin,s32 x,s32 y);
void __GK_ShadingRect(struct tagGkWinRsc *gkwin,struct tagRectangle *rect);
void __GK_ShadingLine(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2);
void __GK_ShadingClear(struct tagGkWinRsc *gkwin);
void __GK_VlinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 y2,u32 color,u32 rop2_code);
void __GK_HlinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 x2,u32 color,u32 rop2_code);
void __GK_OlinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_LinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_SetPixelScreen(struct tagDisplayRsc *display,s32 x,s32 y,
                        u32 color,u32 rop2_code);
void __GK_LinetoScreen(struct tagDisplayRsc *display,struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_BltBmToScreen(struct tagDisplayRsc *display,struct tagRectangle *rect,
                        struct tagRectBitmap *bitmap,s32 x,s32 y);
void __GK_FillRect(struct tagRectBitmap *dst_bitmap,
                     struct tagRectangle *dst_rect,u32 color);
void __GK_FillBm(struct tagRectBitmap *dst_bitmap,u32 color);
void __GK_SetPixelsCircle(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,u32 r,
                                struct tagRectBitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 rop2_code);
void __GK_DrawCircleBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void __GK_DrawCircleScreen(struct tagDisplayRsc *display,struct tagRectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void GK_DrawCircle(struct tagGkscParaDrawCircle *para);
void __GK_BezierBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);
void __GK_BezierScreen(struct tagDisplayRsc *display,struct tagRectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);
void GK_Bezier(struct tagGkscParaBezier *para);
void GK_Lineto(struct tagGkscParaLineto *para);
void GK_LinetoIe(struct tagGkscParaLineto *para);
void GK_DrawText(struct tagGkscParaDrawText *para,const char *text);
void GK_DrawBitMap(struct tagGkscParaDrawBitmapRop *para);
void GK_GradientFillRect(struct tagGkscParaGradientFillWin *para);
void GK_FillWin(struct tagGkscParaFillWin *para);


#ifdef __cplusplus
}
#endif

#endif /*__GK_DRAW_H__*/

