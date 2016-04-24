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

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: �������㺯����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    "./include/gdd_private.h"

int RectW(const RECT *prc)
{
    return (prc->right - prc->left);
}

int RectH(const RECT *prc)
{
    return (prc->bottom - prc->top);
}

void    _SetRect(RECT *prc,int x,int y,int w,int h)
{
    prc->left   = x;
    prc->top    = y;
    prc->right  = x+w;
    prc->bottom = y+h;

}

void    SetRect(RECT *prc,int x,int y,int w,int h)
{
    if(prc!=NULL)
    {
        _SetRect(prc,x,y,w,h);
    }
}

void    SetRectEmpty(RECT *prc)
{
    if(prc!=NULL)
    {
        prc->left   = 0;
        prc->top    = 0;
        prc->right  = 0;
        prc->bottom = 0;
    }
}

BOOL    IsRectEmpty(const RECT *prc)
{
    if(prc!=NULL)
    {
        if((prc->right - prc->left)<=0)
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}
/*========================================================================================*/

void    _CopyRect(RECT *dst,const RECT *src)
{
    dst->left =src->left;
    dst->top =src->top;
    dst->right =src->right;
    dst->bottom =src->bottom;
}

BOOL    CopyRect(RECT *dst,const RECT *src)
{
    if(NULL!=dst)
    if(NULL!=src)
    {
        _CopyRect(dst,src);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

void    _OffsetRect(RECT *prc,int dx,int dy)
{
    prc->left += dx;
    prc->top  += dy;
    prc->right += dx;
    prc->bottom  += dy;
}

BOOL    OffsetRect(RECT *prc,int dx,int dy)
{
    if(NULL!=prc)
    {
        _OffsetRect(prc,dx,dy);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

void    _InflateRect(RECT *prc,int dx,int dy)
{
    prc->left   -= dx;
    prc->top    -= dy;
    prc->right  += dx;
    prc->bottom += dy;
}

BOOL    InflateRect(RECT *prc,int dx,int dy)
{
    if(NULL!=prc)
    {
        _InflateRect(prc,dx,dy);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

void    _InflateRectEx(RECT *prc,int l,int t,int r,int b)
{
    prc->left   -= l;
    prc->top    -= t;
    prc->right  += r;
    prc->bottom += b;
}

BOOL    InflateRectEx(RECT *prc,int l,int t,int r,int b)
{
    if(NULL!=prc)
    {
        _InflateRectEx(prc,l,t,r,b);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

BOOL    _PtInRect(const RECT *prc,const POINT *pt)
{
    if(pt->x < prc->left)   return FALSE;
    if(pt->x >= prc->right) return FALSE;

    if(pt->y < prc->top)        return FALSE;
    if(pt->y >= prc->bottom)    return FALSE;
    return TRUE;

}

BOOL    PtInRect(const RECT *prc,const POINT *pt)
{
    if(NULL!=prc)
    if(NULL!=pt)
    {
        return _PtInRect(prc,pt);
    }
    return FALSE;
}

/*============================================================================*/

BOOL    MakeProgressRect(RECT *dst,const RECT *src,u32 Range,u32 Val,EN_PB_MODE mode)
{
    s32 a0,a1;

    if(NULL == dst)
    {
        return FALSE;
    }

    if(NULL == src)
    {
        return FALSE;
    }

    //��ֹ���ݹ���������
    if(Range>0x0FFFFFFF)
    {
        Range >>= 16;
        Val >>= 16;
    }
    else if(Range>0x00FFFFFF)
    {
        Range >>= 12;
        Val >>= 12;
    }
    else if(Range>0x000FFFFF)
    {
        Range >>= 8;
        Val >>= 8;
    }

    //��ֹ����Ϊ0�������쳣
    if(Range==0)
    {
        Range=1;
        Val=1;
    }

    if(Val>Range)
    {
        Val=Range;
    }

    switch(mode)
    {
        case    PBM_LEFT:
                a0 =(Val*RectW(src))/Range;
                a1 =RectW(src)-a0;

                dst[0].left =src->left;
                dst[0].top =src->top;
                dst[0].right =src->left+a0;
                dst[0].bottom =src->bottom;

                dst[1].left =src->left+a0;
                dst[1].top =src->top;
                dst[1].right =src->right;
                dst[1].bottom =src->bottom;
                return TRUE;
                ////
        case    PBM_RIGHT:
                a0 =(Val*RectW(src))/Range;
                a1 =RectW(src)-a0;

                dst[0].left =src->left+a1;
                dst[0].top =src->top;
                dst[0].right =src->right;
                dst[0].bottom =src->bottom;

                dst[1].left =src->left;
                dst[1].top =src->top;
                dst[1].right =src->left+a1;
                dst[1].bottom =src->bottom;
                return TRUE;
                ////
        case    PBM_TOP:
                a0 =(Val*RectH(src))/Range;
                a1 =RectH(src)-a0;

                dst[0].left =src->left;
                dst[0].top =src->top;
                dst[0].right =src->right;
                dst[0].bottom =src->top+a0;

                dst[1].left =src->left;
                dst[1].top =src->top+a0;
                dst[1].right =src->right;
                dst[1].bottom =src->bottom;
                return TRUE;
                ////

        case    PBM_BOTTOM:
                a0 =(Val*RectH(src))/Range;
                a1 =RectH(src)-a0;

                dst[0].left =src->left;
                dst[0].top =src->top+a1;
                dst[0].right =src->right;
                dst[0].bottom =src->bottom;

                dst[1].left =src->left;
                dst[1].top =src->top;
                dst[1].right =src->right;
                dst[1].bottom =src->top+a1;
                return TRUE;
                ////
        default:
                return FALSE;



    }
}

/*============================================================================*/

