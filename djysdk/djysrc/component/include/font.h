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

//����ģ��: ��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ������Դͷ�ļ�
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2009-11-21
//       ����:lst
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#ifndef __FONT_H__
#define __FONT_H__
#include "stdint.h"
#include "gkernel.h"
#ifdef __cplusplus
extern "C" {
#endif

struct tagRectBitmap;
#define CN_FONT_RSC_TREE            "font"

/* �������� */
#define CN_FONT_TYPE_DOT            1       // ��������
#define CN_FONT_TYPE_TRUETYPE       2       //ʸ������

struct tagFontRsc
{
    struct  tagRscNode node;

    s32 MaxWidth;       //����ַ��Ŀ��,������ʾʱ������Ϊ���п�
    s32 MaxHeight;      //����ַ��ĸ߶�
    u32 Attr;

    bool_t (*LoadFont)(void *zk_addr);      //��������

    void (*UnloadFont)(void);      //ж������

// ��ȡucs4����Ϊcharcode�ַ�����ʾ���󣬰ѵ�����䵽font_bitmap�У�������Ӧ����
// ��font_bitmap�����ڴ档
// ���font_bitmap�����е�bm_bits����ΪNULL���򱾺����˻�Ϊ��ѯ��������
// font_bitmap�����з���ͼ����������������Լ���bm_bits�����ڴ�ߴ磬����һ����
// ����ʹ�ö��ֳߴ������ʱ���ر����á�
// resv,����������
// ����: true = �ɹ�ִ�У�false=��֧���ַ�
    bool_t (*GetBitmap)(u32 charcode, u32 size, u32 resv,
                        struct tagRectBitmap *font_bitmap);
    s32 (*GetCharWidth)(u32 Charcode);      //��ȡĳ�ַ����
    s32 (*GetCharHeight)(u32 CharCode);     //��ȡĳ�ַ��߶�
};

bool_t Font_InstallFont(struct tagFontRsc *font,char *name);

ptu32_t ModuleInstall_Font(ptu32_t para);

struct tagFontRsc* Font_GetCurFont(void);
struct tagFontRsc* Font_SetCurFont(struct tagFontRsc* font);
struct tagFontRsc* Font_SearchFont(const char* name);
s32 Font_GetFontLineHeight(struct tagFontRsc* font);
s32 Font_GetFontLineWidth(struct tagFontRsc* font);
s32 Font_GetFontAttr(struct tagFontRsc* font);

#ifdef __cplusplus
}
#endif

#endif //__FONT_H__

