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
//�ļ�����: gui kernelӦ�ó���ӿڲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-10-07
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "lock.h"
#include <string.h>
#include "systime.h"
#include "gkernel.h"
#include "..\include\gk_syscall.h"
#include "..\include\gk_win.h"
#include "gk_display.h"
//----��������-----------------------------------------------------------------
//����: �������棬����ʾ�����������Ҫ�����������ʹ�á�������ʵ��һ����ͨ����
//      �ǳ����ƣ������Ҫ����:
//          1�����治��С����ʾ���ߴ磬�����ڿ��ԡ�
//          2�������rop����ֻ����0,���ڸ����Щ��
//          3����������ȼ�ֻ����0��
//          4��һ����ʾ���£�����ֻ��һ������display��desktopָ��ָ��
//      ��һ����ʾ���µ����д��ڶ���������Ӵ��ڡ�
//����: display�������������ʾ��
//      desktop���´��������棬�������ݽṹ�����ڴ��ɵ������ṩ
//      width��height������ĳߴ磬С����ʾ���ߴ������Ϊ��ʾ���ߴ�
//      color����������ʱ������ɫ
//      buf_mode�����建��ģʽ���μ� CN_WINBUF_PARENT �峣������
//      PixelFormat,���ظ�ʽ��CN_SYS_PF_DISPLAY��ʾ����ʾ����ͬ���Ƽ���
//      BaseColor, �Ҷ�ͼ��ɫ��(����pf_type == CN_SYS_PF_GRAY1 ~8ʱ����)
//����: true=�����ɹ���false=����ʧ��
//-----------------------------------------------------------------------------
bool_t GK_ApiCreateDesktop(struct tagDisplayRsc *display,
                           struct tagGkWinRsc *desktop,
                           s32 width,s32 height,u32 color,
                           u32 buf_mode,u16 PixelFormat,
                           u32 BaseColor)
{
    struct tagGkscParaCreateDesktop para;
    struct tagGkWinRsc *result;
    if((NULL == display) || (NULL == desktop))
        return false;
    para.display = display;
    para.desktop = desktop;
    para.width = width;
    para.height = height;
    para.color = color;
    para.buf_mode = buf_mode;
    para.PixelFormat = PixelFormat;
    para.BaseColor = BaseColor;
    result = (void*)GK_CreateDesktop(&para);
    if(result == NULL)
        return false;
    else
        return true;
}

//----��ȡ����ָ��-------------------------------------------------------------
//����: ������ʾ��������������ָ�롣��ָ��ָ��һ�����ڿ��ƿ�
//����: display_name������������ʾ��������
//����: ����ҵ����򷵻�ָ�룬���򷵻�NULL
//-----------------------------------------------------------------------------
struct tagGkWinRsc *GK_ApiGetDesktop(char *display_name)
{
    struct tagDisplayRsc *display;
    display = (struct tagDisplayRsc *)Rsc_SearchTree("display");
    if(display != NULL)
        display = (struct tagDisplayRsc *)Rsc_SearchSon(
                (struct tagRscNode*)display,display_name);
    if(display != NULL)
        return display->desktop;
    else
        return NULL;
}
//----��������-----------------------------------------------------------------
//����: ����һ�����ڣ��´����Ĵ���ֻ��һ����������û����䣬Ҳû�б߿��һ��
//      װ�Ρ�
//����: parent��������ָ��
//      newwin���´����Ĵ��ڣ��������ݽṹ�����ڴ��ɵ������ṩ
//      left��top��right��bottom������ĳߴ磬����ڸ�����
//      color����������ʱ������ɫ
//      buf_mode�����建��ģʽ���μ� CN_WINBUF_PARENT �峣������
//      name���´��ڵ����֣���copy��newwin��
//      PixelFormat,���ظ�ʽ��CN_SYS_PF_DISPLAY��ʾ����ʾ����ͬ���Ƽ���
//      KeyColor,�趨����͸��ɫ,��RopMode��ʹ����͸��ɫʱ,������KeyColor
//      BaseColor, �Ҷ�ͼ��ɫ��(����PixelFormat == CN_SYS_PF_GRAY1 ~8ʱ����)
//      RopMode, ����룬�μ� CN_ROP_ALPHA_SRC_MSK �峣������,0��ʾ�����⹦��
//����: true=�����ɹ���false=����ʧ��
//-----------------------------------------------------------------------------
bool_t GK_ApiCreateGkwin(struct tagGkWinRsc *parent,
                         struct tagGkWinRsc *newwin,
                         s32 left,s32 top,s32 right,s32 bottom,
                         u32 color,u32 buf_mode,
                         char *name,u16 PixelFormat,u32 KeyColor,
                         u32 BaseColor,u32 RopMode)
{
    struct tagGkscParaCreateGkwin para;
    struct tagGkWinRsc *result;
    if((NULL == parent) || (NULL == newwin))
        return false;
    para.parent_gkwin = parent;
    para.gkwin = newwin;
    para.result = &result;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    para.color = color;
    para.buf_mode = buf_mode;
    para.name = name;
    para.PixelFormat = PixelFormat;
    para.KeyColor = KeyColor;
    para.BaseColor = BaseColor;
    para.RopCode = RopMode;
    GK_SyscallChunnel(CN_GKSC_CREAT_GKWIN,CN_TIMEOUT_FOREVER,
                            &para,sizeof(para),NULL,0);
    if(result == NULL)
        return false;
    else
    {
        //gui kernel��������ʱ,�����buf����,���ڴ�����ͬʱ��������,����,
        //������ִ�����.
        //��buf���ڲ����ٴ���ͬʱ����ԭ����,����buf������ֱ�ӻ�����screen
        //����framebuffer�ϵ�,����������ʱ,��������δ����.ֱ������ GK_ApiSyncShow
        //�󣬿�������ܴ����á�
        if(newwin->wm_bitmap == NULL)   //wm_bitmap==NULL ��ʾ�޴��ڻ�������
        {
            GK_ApiSyncShow(CN_TIMEOUT_FOREVER);
            GK_ApiFillWin(newwin,color,0);
        }
        return true;
    }
}
//----�첽��䴰��-------------------------------------------------------------
//����: ��ָ����ɫ���һ�����ڡ�
//����: gkwin�������Ĵ���
//      color�������ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiFillWin(struct tagGkWinRsc *gkwin,u32 color,u32 SyncTime)
{
    struct tagGkscParaFillWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.color = color;
    GK_SyscallChunnel(CN_GKSC_FILL_WIN,SyncTime,&para,sizeof(para),NULL,0);
    return;
}
//----ˢ�´���-----------------------------------------------------------------
//����: ִ�л�����ͨ���е����
//����: gkwin�������Ĵ���
//      color�������ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiSyncShow(u32 SyncTime)
{
    GK_SyscallChunnel(CN_GKSC_SYNC_SHOW,SyncTime,NULL,0,NULL,0);
    return;
}
#if 0
//----��䴰�ڵ�һ����---------------------------------------------------------
//����: ��ָ����ɫ���һ�����ڵ�һ���֣���ɫ��alphaֵ�������
//����: gkwin��������Ŀ�괰��
//      rect��������λ��
//      AlphaL,AlphaR,AlphaT,AlphaB,�������µ�Alphaֵ
//      color��Ҫ�����ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiFillPartWin(struct tagGkWinRsc *gkwin,
                       struct tagRectangle rect,u32 color,
                       u32 SyncTime)
{
    struct tagGkscParaFillPartWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.rect = rect;
    para.color = color;
    GK_SyscallChunnel(CN_GKSC_FILL_PART_WIN,SyncTime,&para,
                        sizeof(para),NULL,0);
    return;
}
#endif

//----����ɫ��䴰�ڵ�һ����---------------------------------------------------
//����: ��ָ����ɫ���һ�����ڵ�һ���֣���ɫ��alphaֵ�������
//����: gkwin��������Ŀ�괰��
//      rect��������λ��
//      Color0,��ɫ1�������CN_FILLRECT_MODE_H�峣������
//      Color1,��ɫ2�������CN_FILLRECT_MODE_H�峣������
//      Mode,����ģʽ,�����CN_FILLRECT_MODE_H�峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiFillRect(struct tagGkWinRsc *gkwin,struct tagRectangle *rect,
                            u32 Color0,u32 Color1,u32 Mode,u32 SyncTime)
{
    struct tagGkscParaGradientFillWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.rect = *rect;
    para.Color0 = Color0;
    para.Color1 = Color1;
    para.Mode = Mode;
    GK_SyscallChunnel(CN_GKSC_FILL_RECT,SyncTime,&para,
                        sizeof(para),NULL,0);
    return;
}

//----��ʾ�ı�-----------------------------------------------------------------
//����: ��Ĭ�������Ĭ���ַ�����ʾһ���ı�����
//����: gkwin����ʾ��Ŀ�괰��
//      pFont, ʹ�õ�����,NULL��ʾ��ϵͳ��ǰ����
//      pCharset, ʹ�õ��ַ���,NULL��ʾ��ϵͳ��ǰ�ַ���
//      x��y����ʾλ�ã������gkwin
//      text������ʾ���ַ���
//      count���ַ������ȣ�����������
//      color, ��ʾ��ɫ��RGB888��ʽ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiDrawText(struct tagGkWinRsc *gkwin,
                    struct tagFontRsc *pFont,
                    struct tagCharset *pCharset,
                    s32 x,s32 y,
                    const char *text,u32 count,u32 color,
                    u32 Rop2Code,u32 SyncTime)
{
    struct tagGkscParaDrawText para;
    if((NULL == gkwin) || (NULL == text) || (0 == count))
        return;
    para.gkwin = gkwin;
    para.pFont = pFont;
    para.pCharset = pCharset;
    para.x = x;
    para.y = y;
    para.count = count;
    para.color = color;
    para.Rop2Code = Rop2Code;
    GK_SyscallChunnel(CN_GKSC_DRAW_TEXT,SyncTime,
                        &para,sizeof(para),(void*)text,count);
    return;
}
//----����---------------------------------------------------------------------
//����: �������ء�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x��y�����Ƶ�λ�ã������gkwin
//      color����������ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiSetPixel(struct tagGkWinRsc *gkwin,s32 x,s32 y,
                        u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct tagGkscParaSetPixel para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x = x;
    para.y = y;
    para.color = color;
    para.Rop2Code = Rop2Code;
    GK_SyscallChunnel(CN_GKSC_SET_PIXEL,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�������ؿ�ֱ�ߣ������������˵�-------------------------------------------
//����: ����ֱ�ߣ������������˵�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2,Ҫ���Ƶ�ֱ�ߵ����յ�����
//      color�����Ƶ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiLineto(struct tagGkWinRsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct tagGkscParaLineto para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.Rop2Code = Rop2Code;
    GK_SyscallChunnel(CN_GKSC_LINETO,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�������ؿ�ֱ�ߣ����������˵�---------------------------------------------
//����: ����ֱ�ߣ����������˵�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2,Ҫ���Ƶ�ֱ�ߵ����յ�����
//      color�����Ƶ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiLinetoIe(struct tagGkWinRsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct tagGkscParaLineto para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.Rop2Code = Rop2Code;
    GK_SyscallChunnel(CN_GKSC_LINETO_INC_END,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----����դλͼ---------------------------------------------------------------
//����: ����bitmap������դ������
//����: gkwin�����Ƶ�Ŀ�괰��
//      bitmap�������Ƶ�λͼ
//      pattern��Ҫ���õ�ͼ��λͼ
//      Rop4Msk��rop4��������Ҫ������λͼ��һ����ɫλͼ
//      KeyColor��͸��ɫ
//      x��y�����Ƶ�Ŀ��λ��
//      RopMode, ��դ�룬�μ� CN_ROP_ALPHA_SRC_MSK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiDrawBitMap(struct tagGkWinRsc *gkwin,
                                struct tagRectBitmap *bitmap,
                                s32 x,s32 y,
                                u32 KeyColor,u32 RopCode,u32 SyncTime)
{
    struct tagGkscParaDrawBitmapRop para;
    memset(&para,sizeof(para),0);
    if(NULL == gkwin)
        return;
    else
        para.gkwin = gkwin;
    if(NULL == bitmap)
        return;
    else
        para.bitmap = *bitmap;

    para.KeyColor = KeyColor;
    para.x = x;
    para.y = y;
    para.RopCode = RopCode;
    GK_SyscallChunnel(CN_GKSC_DRAW_BITMAP_ROP,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�������ؿ��Բ-----------------------------------------------------------
//����: �������ؿ��Բ��
//����: gkwin�����Ƶ�Ŀ�괰��
//      x0��y0��Բ�����ĵ����꣬����ڴ���
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiDrawCircle(struct tagGkWinRsc *gkwin,s32 x0,s32 y0,
                    u32 r,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct tagGkscParaDrawCircle para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x0 = x0;
    para.y0 = y0;
    para.r = r;
    para.color = color;
    para.Rop2Code = Rop2Code;
    GK_SyscallChunnel(CN_GKSC_DRAW_CIRCLE,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----��Bezier����-------------------------------------------------------------
//����: ��Bezier���ߡ�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2��x3��y3��x4��y4������Bezier���ߵ��ĸ����Ƶ�
//      color�����Ƶ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiDrawBezier(struct tagGkWinRsc *gkwin,float x1,float y1,
                    float x2,float y2,float x3,float y3,float x4,float y4,
                    u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct tagGkscParaBezier para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.x3 = x3;
    para.y3 = y3;
    para.x4 = x4;
    para.y4 = y4;
    para.color = color;
    para.Rop2Code = Rop2Code;
    GK_SyscallChunnel(CN_GKSC_BEZIER,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�ƶ�����-----------------------------------------------------------------
//����: �ƶ����ڡ�
//����: gkwin��Ŀ�괰��
//      left��top�������ƶ�������Ͻ����꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiMoveWin(struct tagGkWinRsc *gkwin, s32 left,s32 top,u32 SyncTime)
{
    struct tagGkscParaMoveWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    GK_SyscallChunnel(CN_GKSC_MOVE_WIN,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�ı䴰��λ�óߴ�---------------------------------------------------------
//����: �ı䴰�ڵ�λ�úͳߴ磬�������������ڸ�����
//����: gkwin��Ŀ�괰��
//      left��top��right��bottom�����ڵ������꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiChangeWinArea(struct tagGkWinRsc *gkwin, s32 left,s32 top,
                                       s32 right,s32 bottom, u32 SyncTime)
{
    struct tagGkscParaChangeWinArea para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    GK_SyscallChunnel(CN_GKSC_CHANGE_WIN_AREA,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----���ô��ڵı߽�ģʽ-------------------------------------------------------
//����: ���ô��ڵı߽�ģʽ��
//����: gkwin��Ŀ�괰��
//      mode�� ���ڵı߽�ģʽ��cn_bound_limitΪ���ޣ�cn_bound_unlimitΪ������
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiSetBoundMode(struct tagGkWinRsc *gkwin, bool_t mode)
{
    struct tagGkscParaSetBoundMode para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.mode = mode;
    GK_SyscallChunnel(CN_GKSC_SET_BOUND_MODE,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return;
}
//----���ô�����ʾ���ȼ�-------------------------------------------------------
//����: ���ô�����ʾ���ȼ���
//����: gkwin��Ŀ�괰��
//      prio�������ȼ�
//      sync_time������ͼ��ʱ��ͬ�����ڴ�ʱ���ڲ����Ƿ������ɶ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiSetPrio(struct tagGkWinRsc *gkwin, sfast_t prio,u32 SyncTime)
{
    struct tagGkscParaSetPrio para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.prio = prio;
    GK_SyscallChunnel(CN_GKSC_SET_PRIO,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----���ô��ڵĹ�դ����-------------------------------------------------------
//����: ���ô��ڵĹ�դ���ԡ�
//����: gkwin��Ŀ�괰��
//      Rop4Msk,����λͼ
//      RopMode, ����룬�μ� CN_ROP_ALPHA_SRC_MSK �峣�����壬��֧�ֶ�Ԫ��դ���
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_ApiSetRopCode(struct tagGkWinRsc *gkwin,
                            u32 RopCode,u32 SyncTime)
{
    struct tagGkscParaSetRopCode para;
    memset(&para,sizeof(para),0);
    if(NULL == gkwin)
        return false;
    if(RopCode == gkwin->RopCode)
        return true;
    para.gkwin = gkwin;
    para.RopCode = RopCode;
    GK_SyscallChunnel(CN_GKSC_SET_ROP_CODE,SyncTime,
                        &para,sizeof(para),NULL,0);
    return true;
}
#if 0       //ɾ����Ԫ��դ��Ϲ���
//----���ô���ͼ��λͼ---------------------------------------------------------
//����: �ı䴰�ڵ�ͼ��λͼ��
//����: gkwin��Ŀ�괰��
//      pattern���µ�ͼ��λͼ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_ApiSetPatBuf(struct tagGkWinRsc *gkwin,
                            struct tagRectBitmap *pattern,
                            u32 SyncTime)
{
    struct tagGkscParaSetPatBuf para;
    memset(&para,sizeof(para),0);
    if(NULL == gkwin)
        return false;
    para.gkwin = gkwin;
    if(NULL != pattern)
        para.pattern = *pattern;
    GK_SyscallChunnel(CN_GKSC_SET_PAT_BUF,SyncTime,
                        &para,sizeof(para),NULL,0);
    return true;
}
//----���ô�������λͼ---------------------------------------------------------
//����: ���ô�������λͼ��
//����: gkwin��Ŀ�괰��
//      bitmsk�����õ�����λͼ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_ApiSetRop4Msk(struct tagGkWinRsc *gkwin,
                                struct tagRectBitmap *Rop4Msk,u32 SyncTime)
{
    struct tagGkscParaSetBitmsk para;
    memset(&para,sizeof(para),0);
    if(NULL == gkwin)
        return false;
    para.gkwin = gkwin;
    if(NULL != Rop4Msk)
        para.Rop4Msk = *Rop4Msk;
    GK_SyscallChunnel(CN_GKSC_SET_BITMSK,SyncTime,
                        &para,sizeof(para),NULL,0);
        return true;
}
#endif
//----���ô���͸��ɫ-----------------------------------------------------------
//����: ���ô��ڵ�͸��ɫ��
//����: gkwin��Ŀ�괰��
//      KeyColor��Ҫ���õ�͸��ɫ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_ApiSetTransparentColor(struct tagGkWinRsc *gkwin,
                                        u32 KeyColor)
{
    struct tagGkscParaSetTransparentColor para;
    if(NULL == gkwin)
        return false;
    if(KeyColor == gkwin->KeyColor)
        return true;
    para.gkwin = gkwin;
    para.KeyColor = KeyColor;
    GK_SyscallChunnel(CN_GKSC_SET_TRANSPARENTCOLOR,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return true;
}

//----���ô���ֱ��д������-----------------------------------------------------
//����: ���ô���ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiSetDirectScreen(struct tagGkWinRsc *gkwin,u32 SyncTime)
{
    struct tagGkscParaSetDirectScreen para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    GK_SyscallChunnel(CN_GKSC_SET_DIRECT_SCREEN,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----��������ֱ��д������-----------------------------------------------------
//����: ��������ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiCancelDirectScreen(struct tagGkWinRsc *gkwin,u32 SyncTime)
{
    struct tagGkscParaUnsetDirectScreen para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    GK_SyscallChunnel(CN_GKSC_UNSET_DIRECT_SCREEN,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return;
}
//----���ٴ���-----------------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ��
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_ApiDestroyWin(struct tagGkWinRsc *gkwin)
{
    struct tagGkscParaDestroyWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    GK_SyscallChunnel(CN_GKSC_DESTROY_WIN,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return;
}

//----��ѯ�Կ���ʽ-------------------------------------------------------------
//����: ��ѯ�Կ�ʹ�õ���ɫ��ʽ����λͼʱ�����ʹ�ø��Կ���ͬ����ɫ��ʽ�������
//      ��������
//����: display������ѯ���Կ�
//����: ��ɫ��ʽ
//-----------------------------------------------------------------------------
u16 GK_ApiGetPixelFormat(struct tagDisplayRsc *display)
{
    if(display != NULL)
        return display->pixel_format;
    else
        return 0;
}

