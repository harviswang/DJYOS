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
//�ļ�����: gui kernel���岿��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GK_SYSCALL_H__
#define __GK_SYSCALL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "gkernel.h"


struct tagRectBitmap;
//gui kernel��syscall����,���ϲ�ͨ���ܵ����ݸ�gui kernel
//���ڿ���������
#define CN_GKSC_SET_PEN_COLOR       0x000   //���ô��ڻ�����ɫ
#define CN_GKSC_GET_PEN_COLOR       0x001   //ȡ������ɫ
#define CN_GKSC_SET_CANVAS_COLOR    0x002   //���ô��ڻ�����ɫ
#define CN_GKSC_GET_CANVAS_COLOR    0x003   //ȡ������ɫ
#define CN_GKSC_SET_SIZE            0x004   //���ô��ڷֱ���
#define CN_GKSC_GET_SIZE            0x005   //ȡ���ڷֱ���
#define CN_GKSC_SET_CURSOR          0x006   //���ô��ڹ��λ��
#define CN_GKSC_GET_CURSOR          0x007   //���ô��ڹ��λ��
#define CN_GKSC_CREAT_GKWIN         0x008   //������������
#define CN_GKSC_CREAT_DESKTOP       0x009   //������������
#define CN_GKSC_SET_DIRECT_SCREEN   0x00a   //���ô��ڵ�ֱ��д������
#define CN_GKSC_UNSET_DIRECT_SCREEN 0x00b   //ȡ�����ڵ�ֱ��д������
#define CN_GKSC_FLUSH_WBUF          0x00c   //�Ѵ��ڻ������е�ͼ��ˢ�µ�screen
#define CN_GKSC_FLUSH_FBUF          0x00d   //��֡�������е�ͼ��ˢ�µ�screen
#define CN_GKSC_SET_ROP_CODE        0x00e   //���ô��ڵĹ�դ����
#define CN_GKSC_SET_PAT_BUF         0x00f   //���ô��ڵ�ͼ��λͼ
#define CN_GKSC_SET_TRANSPARENTCOLOR 0x010  //���ô��ڵ�͸��ɫ
#define CN_GKSC_SET_BITMSK          0x011   //���ô��ڵ�����λͼ
#define CN_GKSC_DESTROY_WIN         0x012   //�ͷŴ��ڵ���Դ
#define CN_GKSC_SET_PRIO            0x013   //�ı䴰�ڵ����ȼ�
#define CN_GKSC_SET_BOUND_MODE      0x014   //���ô��ڱ߽�ģʽ
#define CN_GKSC_MOVE_WIN            0x015   //�ƶ�����
#define CN_GKSC_CHANGE_WIN_AREA     0x016   //�ƶ�����
#define CN_GKSC_SYNC_SHOW           0x017   //�����������ʾ��

//����������
#define CN_GKSC_SET_PIXEL           0x100   //��������
#define CN_GKSC_LINETO              0x101   //�������ؿ�����������������˵�
#define CN_GKSC_LINETO_INC_END      0x102   //�������ؿ���������������˵�
#define CN_GKSC_DRAW_BITMAP_ROP     0x104   //��bitmap,����դ����
#define CN_GKSC_FILL_WIN            0x105   //��ָ����ɫ��䴰��
#define CN_GKSC_DRAW_TEXT           0x106
#define CN_GKSC_FILL_RECT           0x107   //��ָ����ɫ��䴰���ھ��Σ�֧�ֽ���
#define CN_GKSC_DRAW_CIRCLE         0x108   //��Բ
#define CN_GKSC_BEZIER              0x109   //������������

struct tagGkscParaCreateDesktop
{
    struct tagDisplayRsc *display;        //������ʾ��
    struct tagGkWinRsc *desktop;          //���洰��,���ϲ����ṹ��
    s32 width,height;           //����ߴ磬��С����ʾ���ߴ������Ϊ��ʾ���ߴ�
    u32 color;                          //��������ʱ������ɫ
    u32 buf_mode;    //���建��ģʽ���μ� CN_WINBUF_PARENT �峣������
    //���³�Աֻ��buf_mode=trueʱ��Ч���������֮��
    u16 PixelFormat;            //���ظ�ʽ
    u32 BaseColor;        //�Ҷ�ͼ��ɫ(��pf_type == CN_SYS_PF_GRAY1 ~8)
                            //pf_typeȡ����ֵʱ����Ҫ�˲���
};
struct tagGkscParaCreateGkwin
{
    struct tagGkWinRsc *parent_gkwin;   //�´��ڵĸ�����
    struct tagGkWinRsc *gkwin;          //�´���,�ɵ����߷���ṹ��
    struct tagGkWinRsc **result;        //���ڷ��ؽ��
    s32 left,top,right,bottom;          //�´��ڵ�λ�ã�����ڸ����ڵ�����
    u32 color;                          //��������ʱ������ɫ
    u32 buf_mode;    //���建��ģʽ���μ� CN_WINBUF_PARENT �峣������
    char *name;             //��������(����)
    //���³�Աֻ��buf_mode=trueʱ��Ч���������֮��
    u16 PixelFormat;            //���ظ�ʽ���������ñ�Ŵ���CN_GKWIN_PF_LIMIT�ĸ�ʽ
    u32  KeyColor;          //͸��ɫ��gkwin�е�rop_code����͸��ɫʱ����
    u32 BaseColor  ;        //�Ҷ�ͼ��ɫ(��pf_type == CN_SYS_PF_GRAY1 ~8)
                            //pf_typeȡ����ֵʱ����Ҫ�˲���
    u32 RopCode;          //���ڵĹ�դ���ԣ���windows�Ĺ�դ������Ļ���������չ
};
struct tagGkscParaFillWin
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    u32 color;                      //���ʹ�õ���ɫ
};

struct tagGkscParaGradientFillWin
{
    struct tagGkWinRsc *gkwin;    //���Ƶ�Ŀ�괰��
    struct tagRectangle rect;     //�����ľ���
    u32 Color0;                   //��ɫ0
    u32 Color1;                   //��ɫ1
    u32 Mode;

};

//�����е��Ե���color�����㲻��ʾ��
//��������ʾ�õ�rop2����
struct tagGkscParaDrawText
{
    struct tagGkWinRsc* gkwin;      //Ŀ�괰��
    struct tagFontRsc *pFont;       //ʹ�õ�����,NULL��ʾ��ϵͳ��ǰ����
    struct tagCharset *pCharset;    //ʹ�õ��ַ���,NULL��ʾ��ϵͳ��ǰ�ַ���
    s32 x, y;                       //��ʾλ�ã������gkwin
    s32 count;                      //�ַ������������ȣ��ֽ�����������������
    u32 color;                      //����ʹ�õ���ɫ
    u32 Rop2Code;                   //rop2����
};
struct tagGkscParaSetPixel
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    s32 x,y;                        //���Ӧ������
    u32 color;                      //����ʹ�õ���ɫ
    u32 Rop2Code;                  //��Ԫ��դ������
};
struct tagGkscParaDrawBitmapRop
{
    struct tagGkWinRsc *gkwin;        //���Ƶ�Ŀ�괰��
    struct tagRectBitmap bitmap;   //�����Ƶ�λͼ
    u32 KeyColor;                   //͸��ɫ,��bitmap��ͬ��ʽ
    s32 x,y;                        //����λ�õ����Ͻ�����
    u32 RopCode;                   //��դ������
};
struct tagGkscParaLineto
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    s32 x1,y1,x2,y2;                //Ŀ��ֱ�ߵ���㣬�յ�����
    u32 color;                      //����ֱ��ʹ�õ���ɫ
    u32 Rop2Code;                  //��Ԫ��դ������
};
struct tagGkscParaDrawCircle
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    s32 x0,y0;                      //Բ������
    s32 r;                          //Բ�İ뾶
    u32 color;                      //��Բʹ�õ���ɫ
    u32 Rop2Code;                  //��Ԫ��դ������
};
struct tagGkscParaBezier
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    float x1,y1,x2,y2,x3,y3,x4,y4;  //����Bezier���ߵ��ĸ����Ƶ�
    u32 color;                      //��Bezier����ʹ�õ���ɫ
    u32 Rop2Code;                  //��Ԫ��դ������
};
struct tagGkscParaMoveWin
{
    struct tagGkWinRsc *gkwin;      //Ŀ�괰��
    s32 left,top;                   //Ŀ�괰����λ�õ����Ͻ�����
};
struct tagGkscParaChangeWinArea
{
    struct tagGkWinRsc *gkwin;      //Ŀ�괰��
    s32 left,top,right,bottom;      //Ŀ�괰���µ�����
};
struct tagGkscParaSetBoundMode
{
    struct tagGkWinRsc *gkwin;      //Ŀ�괰��
    bool_t mode;                    //���ڱ߽�ģʽ��trueΪ���ޣ�falseΪ������
};
struct tagGkscParaSetPrio
{
    struct tagGkWinRsc *gkwin;      //Ŀ�괰��
    sfast_t prio;                   //�µ����ȼ�
};
struct tagGkscParaSetRopCode
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    u32 RopCode;                   //�µĹ�դ������
};
struct tagGkscParaSetPatBuf
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    struct tagRectBitmap pattern; //Ҫ���õ�ͼ��λͼ
};
struct tagGkscParaSetTransparentColor
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    u32 KeyColor;                   //Ҫ���õ�͸��ɫ
};
struct tagGkscParaSetBitmsk
{
    struct tagGkWinRsc *gkwin;      //���Ƶ�Ŀ�괰��
    struct tagRectBitmap Rop4Msk;  //Ҫ���õ�����λͼ
};
struct tagGkscParaSetDirectScreen
{
    struct tagGkWinRsc *gkwin;      //Ŀ�괰��
};
struct tagGkscParaUnsetDirectScreen
{
    struct tagGkWinRsc *gkwin;      //Ŀ�괰��
};
struct tagGkscParaDestroyWin
{
    struct tagGkWinRsc *gkwin;      //Ҫ���ٵ�Ŀ�괰��
};
#ifdef __cplusplus
}
#endif

#endif //__GK_SYSCALL_H__

