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
//2. ����: 2011-07-18
//   ����:  ������.
//   �°汾��: V1.1.0
//   �޸�˵��: �������ݽṹʵ���Ѿ���Ϊ���ϲ��ṩ�������ļ�һֱû�ģ��޸�֮��
//             ����__gk_vmalloc��4��������ʹ���Դ����������Ҫ��ʱ��driver���Բ�
//             �ṩ�����ڴ�ĺ�����
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "config-prj.h"
#include "stdlib.h"
#include "stdio.h"
#include "lock.h"
#include "systime.h"
#include "gkernel.h"
#include "align.h"
#include "pool.h"
#include "djyos.h"
#include "..\include\gk_syscall.h"
#include "..\include\gk_usercall.h"
#include "gk_display.h"
#include "..\include\gk_win.h"
#include "..\include\gk_draw.h"
#include "..\include\gk_clip.h"
#include "charset.h"
#include "font.h"
#include "math.h"
#include "string.h"

struct tagGkWinRsc   *g_ptFocusWin;//��ʾ���㴰��,���е���ʾ���Ƕ�������ڵĲ���
static struct tagRscNode s_tDisplayRootRsc;
static struct tagRscNode s_tWindowRootRsc;
u32 g_u32DefaultBackColor,g_u32DefaultColor;
u16 g_u16GkServerEvtt,g_u16RefreshrEvtt;
struct tagGkChunnel g_tGkChunnel;

extern struct tagMemCellPool *g_ptClipRectPool; //from gk_clip.c

//����������
#define CN_CLIP_INIT_NUM    100
struct tagClipRect g_tClipRect[CN_CLIP_INIT_NUM];
struct tagGkWinRsc *g_ptZ_Topmost;

struct tagSemaphoreLCB *g_ptUsercallSemp;
//������÷�ϣ��gui kernel��������ٷ��أ�ʹ������ź���
struct tagSemaphoreLCB *g_ptSyscallSemp;
struct tagMutexLCB *g_ptGkServerSync;
u8 *draw_chunnel_buf;

u32 __ExecOneCommand(u16 DrawCommand,u8 *ParaAddr);

//gkwin����Դ���ֵ�λ����z�����ϵ:
// 1������Դ���У������Ӵ��ڰ�z�����У���ǰ�˵��Ǹ�����ָ��ĵ�һ����㡣
// 2��z_prio��ԱӰ��win�����ֵܽ���е�λ�ã�ԽСԽǰ��
// 3��z_prioһ����Ϊ0��<=0���ڸǸ����ڣ���>0�򱻸������ڸ�


//----��ʼ��guiģ��---------------------------------------------------------
//����: ��ʼ��guiģ�飬����Դ���������ӱ�Ҫ����Դ���
//����: ģ���ʼ��������û�в���
//����: 1=�ɹ�
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_GK(ptu32_t para)
{
    draw_chunnel_buf = M_Malloc(gc_u32CfgGuiCmdDeep * 2 + gc_u32CfgGuiStsDeep,0);
    if(draw_chunnel_buf == NULL)
        return 0;
    g_tGkChunnel.syscall_buf = draw_chunnel_buf + gc_u32CfgGuiCmdDeep;
    g_tGkChunnel.usercall_buf = draw_chunnel_buf + 2 * gc_u32CfgGuiCmdDeep;

    Rsc_AddTree(&s_tDisplayRootRsc,sizeof(struct tagRscNode),RSC_RSCNODE,"display");
    Rsc_AddTree(&s_tWindowRootRsc,sizeof(struct tagRscNode),RSC_RSCNODE,"gkwindow");

    Ring_Init(&g_tGkChunnel.ring_syscall,
                        g_tGkChunnel.syscall_buf,gc_u32CfgGuiCmdDeep);
    Ring_Init(&g_tGkChunnel.ring_usercall,
                        g_tGkChunnel.usercall_buf,gc_u32CfgGuiStsDeep);

    g_tGkChunnel.syscall_mutex = Lock_MutexCreate("gui chunnel to gk mutex");
    g_tGkChunnel.syscall_semp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"gui chunnel to gk semp");
    g_tGkChunnel.usercall_semp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"gui chunnel from gk semp");
    g_ptGkServerSync = Lock_MutexCreate("gk server sync");
//    g_ptGkServerSync = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"gk server sync");
    g_ptUsercallSemp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"gk wait repaint");
    g_ptSyscallSemp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"gk wait job");
    g_u16GkServerEvtt = Djy_EvttRegist(EN_CORRELATIVE,1,0,0,GK_Server,
                                    NULL,8120,"gui kernel server");
    g_ptClipRectPool = Mb_CreatePool(&g_tClipRect,
                                  CN_CLIP_INIT_NUM,
                                  sizeof(struct tagClipRect),
                                  100,2000, "clip area");

    if(    (g_ptClipRectPool == NULL)
        || (g_u16GkServerEvtt == CN_EVTT_ID_INVALID)
        || (g_ptSyscallSemp == NULL)
        || (g_ptUsercallSemp == NULL)
        || (g_ptGkServerSync == NULL)
        || (g_tGkChunnel.usercall_semp == NULL)
        || (g_tGkChunnel.syscall_semp == NULL)
        || (g_tGkChunnel.syscall_mutex == NULL) )
    {
        goto exit_error;
    }


    Djy_EventPop(g_u16GkServerEvtt,NULL,0,0,0,0);

    return 1;

exit_error:
    Mb_DeletePool(g_ptClipRectPool);
    Djy_EvttUnregist(g_u16GkServerEvtt);
    Lock_SempDelete(g_ptSyscallSemp);
    Lock_SempDelete(g_ptUsercallSemp);
//    Lock_SempDelete(g_ptGkServerSync);
    Lock_MutexDelete(g_ptGkServerSync);
    Lock_SempDelete(g_tGkChunnel.usercall_semp);
    Lock_SempDelete(g_tGkChunnel.syscall_semp);
    Lock_MutexDelete(g_tGkChunnel.syscall_mutex);

    Rsc_DelNode(&s_tDisplayRootRsc);
    Rsc_DelNode(&s_tWindowRootRsc);
    return 0;
}

//�������ظ�ʽ��������ʾ��������Ҫ���ڴ�ߴ硣
u32 __gk_cal_buf_size(u32 xsize,u32 ysize,u32 *linesize,u16 PixelFormat)
{
    u32 size;
    u8 color_bits;
    color_bits = (PixelFormat&CN_PF_BITWIDE_MSK)>>8;
    switch(color_bits)
    {
        case 1:
        {
            *linesize = (xsize + 7)/8;
            size = *linesize*ysize;
        }break;
        case 2:
        {
            *linesize = (xsize + 3)/4;
            size = *linesize*ysize;
        }break;
        case 4:
        {
            *linesize = (xsize + 1)/2;
            size = *linesize*ysize;
        }break;
        case 8:
        {
            *linesize = xsize;
            size = xsize * ysize;
        }break;
        case 16:
        {
            *linesize = 2*xsize;
            size = xsize*ysize*2;
        }break;
        case 24:
        {
            *linesize = 3*xsize;
            size = xsize*ysize*3;
        }break;
        case 32:
        {
            *linesize = 4*xsize;
            size = xsize*ysize*4;
        }break;
        default:
            size = 0;
            *linesize = 0;
            break;
    }
    return size;

}

//----���䴰�ڻ���-------------------------------------------------------------
//����: ���䴰�ڻ��棬������1�����������bitmap,2������ô�����rop4���ԣ���Ҫ��
//      ��bitmsk_buf��3��pattern buffer��
//      ��Ȼ������������Ҫ���������֣���ʵ�����Ǻϲ�һ��Ӷ������룬Ȼ�����ֹ�
//      �ָ�ɼ����������ġ��������������byte��ָ�룬�����뵣�Ķ������⡣
//����: buf�����ڷ��ؽ���Ľṹָ��
//      xsize��ysize����ʾ����ߴ�
//      rop_code�����ڵ�rop_code���룬Ӱ���Ƿ����buf�е�rop4_msk��Ա��
//����: true=�ɹ���false=ʧ��
//ע: 2416��2D���ٲ�֧��rop4�������ж�rop_code��ֻ�����pat��vbuf����
//-----------------------------------------------------------------------------
bool_t __gk_vmalloc(struct tagDisplayRsc *disp,struct tagGkWinRsc *gkwin,
                    u16 PixelFormat)
{
    u8 *mem;
    u32 buf_size,linesize;// pal_size=0;
    s32 xsize,ysize;
    bool_t result;
    xsize = gkwin->right - gkwin->left;
    ysize = gkwin->bottom - gkwin->top;
    if(PixelFormat == CN_SYS_PF_DISPLAY)
        PixelFormat = disp->pixel_format;
    //������ʾ��������Ҫ���ڴ�ߴ硣
    buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,PixelFormat);
    buf_size = align_up_sys(buf_size);
    //ͬʱ����bitmap���ݽṹ��ͼƬλͼ
    mem = (u8*)M_MallocLcHeap(buf_size + sizeof(struct tagRectBitmap),
                            disp->DisplayHeap,0);
    if(mem != NULL)
    {
        gkwin->wm_bitmap = (struct tagRectBitmap*)mem;
        gkwin->wm_bitmap->PixelFormat = PixelFormat;
        gkwin->wm_bitmap->width = xsize;
        gkwin->wm_bitmap->height = ysize;
        gkwin->wm_bitmap->linebytes = linesize;
        mem += sizeof(struct tagRectBitmap);
        gkwin->wm_bitmap->bm_bits = mem;
        result = true;
    }else
    {
        gkwin->wm_bitmap = NULL;
        result = false;
    }
    return result;
}


//----���·��䴰�ڻ���---------------------------------------------------------
//����: �����ڳߴ�ı䣬���ñ��������·��仺�档�߼��ϣ��������Ĺ���="vfree-
//      ����vmalloc"����ϣ�������һ����������Ϊ�����Ч�ʡ���Ϊ�ڴ��ǰ������
//      �ģ��ڴ�������൱��ʱ�Ĺ�����vmalloc����Ĵ��ڻ��棬���ܱ�ʵ�ʵĴ󣬵�
//      ���ڳߴ��������Ļ��������������·��䡣���û�ͨ���϶����ı䴰�ڳߴ�ʱ��
//      ʵ������һ��һ��ı�ģ�ÿ�ζ����·���Ļ���ϵͳ�����ܴ����ײ�����Ƭ��
//      vrmalloc�����ȼ��buf�еĻ������Ƿ���Ȼ�������������㣬�����·��䡣
//      �����ܻ����xsize��ysize����ָ�롣
//����: buf��ԭ���ڻ���ָ�룬Ҳ���ڷ��ؽ��
//      xsize��ysize����ʾ����ߴ�
//      rop_code�����ڵ�rop_code����
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __gk_vrmalloc(struct tagDisplayRsc *disp,struct tagGkWinRsc *gkwin)
{
    u8 *mem;
    u32 buf_size,linesize,oldsize,newsize;//pal_size=0;
    s32 xsize,ysize;
    bool_t remalloc = true,result;
    u16 PixelFormat;
    xsize = gkwin->right - gkwin->left;
    ysize = gkwin->bottom - gkwin->top;
    PixelFormat = gkwin->wm_bitmap->PixelFormat;

    //���·����Դ棬��ԭ�Դ���Ȼ����Ҫ�󣬿��ܲ������·��䡣
    //�������·��䣬�����ȡ�ȷ�����ͷŵİ취����Ϊdjyos�Ƕ��̻߳��������ͷź�
    //����Ļ�����Ȼ���������ӷ���ĳɹ��ʣ����ͷź��������ռ���ڴ汻��ĸ�����
    //���¼���ȥ�Ļ����ͻ���ɷ���ʧ�ܡ��ȷ�����ͷţ��������ʧ�ܣ����ٻ�����
    //ȷ��ԭ�����Դ治��ʧ��
    if(PixelFormat == CN_SYS_PF_DISPLAY)
        PixelFormat = disp->pixel_format;
    //������ʾ��������Ҫ���ڴ�ߴ硣
    buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,PixelFormat);
    buf_size = align_up_sys(buf_size);
    newsize = buf_size + sizeof(struct tagRectBitmap);

    oldsize = M_CheckSize(gkwin->wm_bitmap); //��ѯԭָ��ʵ�ʷ�����ڴ�
    if(newsize <= oldsize)
    {
        //�µ��Դ�������ԭʵ�ʳߴ������1/8���ڵĻ���С����С�ڴ����ߴ�ģ�
        //�����·����Դ�
        if((oldsize - newsize) <= (oldsize>>3))
            remalloc = false;       //�������·���
    }
    if(remalloc)
    {
        mem =(u8*) M_MallocLcHeap(newsize,disp->DisplayHeap,0);
        if(mem != NULL)
        {
            gkwin->wm_bitmap = (struct tagRectBitmap*)mem;
            gkwin->wm_bitmap->width = xsize;
            gkwin->wm_bitmap->height = ysize;
            gkwin->wm_bitmap->linebytes = linesize;
            mem += sizeof(struct tagRectBitmap);
            gkwin->wm_bitmap->bm_bits = mem;

            result = true;
        }else
        {
            result = false;   //�·����ڴ�ʧ�ܣ�����ԭ�����䣬���ͷ�ԭ�ڴ档
        }
    }else                     //�������·���
    {
        result = true;
    }
    return result;
}


//----�ͷŴ��ڻ���-------------------------------------------------------------
//����: �ͷ�һ�����ڵĻ��档
//����: disp������������ʾ��
//      buf�����ڷ��ؽ���Ľṹָ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_vfree(struct tagDisplayRsc *disp,struct tagGkWinRsc *gkwin)
{
    //�����ڴ�ʱ���Դ��bitmap�ṹ��ͬʱ����ģ���ֻ�����
    //m_freeһ�Σ����߾��ͷ�
    M_FreeHeap(gkwin->wm_bitmap,disp->DisplayHeap);
}
//----ȡz��Ƭ�ε���ʼ����(���ڼ����Ӵ���)--------------------------------------
//����: ÿһ�����ڼ����Ӵ��ڣ�����z����ռ��������һ�Σ�������ȡ�öε���ʼ����(
//      ���˵Ĵ���)��
//����: gkwin������ָ��
//����: ��ʼ����ָ��
//-----------------------------------------------------------------------------
struct tagGkWinRsc *__GK_GetZsectionStart(struct tagGkWinRsc *gkwin)
{
    struct tagGkWinRsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct tagGkWinRsc *)Rsc_GetSon(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        temp = (struct tagGkWinRsc *)temp->node.previous;
        if(temp ->z_prio <= 0 )  //�����Ӵ��ھ���resultǰ��
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}
//----ȡz��Ƭ�εĽ�������(���ڼ����Ӵ���)--------------------------------------
//����: ÿһ�����ڼ����Ӵ��ڣ�����z����ռ��������һ�Σ�������ȡ�öεĽ�������(
//      ��ǰ�˵Ĵ���)��
//����: gkwin������ָ��
//����: ��������ָ��
//-----------------------------------------------------------------------------
struct tagGkWinRsc *__GK_GetZsectionEnd(struct tagGkWinRsc *gkwin)
{
    struct tagGkWinRsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct tagGkWinRsc *)Rsc_GetSon(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        else if(temp ->z_prio > 0 )  //�����Ӵ��ھ���result���
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}
//----ȡz��Ƭ�ε���ʼ����(���ڼ������ﴰ��)------------------------------------
//����: ÿһ�����ڼ������ﴰ�ڣ�����z����ռ��������һ�Σ�������ȡ�öε���ʼ����(
//      ���˵Ĵ���)��
//����: gkwin������ָ��
//����: ��ʼ����ָ��
//-----------------------------------------------------------------------------
struct tagGkWinRsc *__GK_GetZsectionAllStart(struct tagGkWinRsc *gkwin)
{
    struct tagGkWinRsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct tagGkWinRsc *)Rsc_GetSon(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        temp = (struct tagGkWinRsc *)Rsc_GetNext(&temp->node);
        if(temp ->z_prio <= 0 )  //�����Ӵ��ھ���resultǰ��
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}

//----ȡz��Ƭ�εĽ�������(���ڼ����Ӵ���)--------------------------------------
//����: ÿһ�����ڼ������ﴰ�ڣ�����z����ռ��������һ�Σ�������ȡ�öεĽ�������(
//      ��ǰ�˵Ĵ���)��
//����: gkwin������ָ��
//����: ��������ָ��
//-----------------------------------------------------------------------------
struct tagGkWinRsc *__GK_GetZsectionAllEnd(struct tagGkWinRsc *gkwin)
{
    struct tagGkWinRsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct tagGkWinRsc *)Rsc_GetSon(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        else if(temp ->z_prio > 0 )  //�����Ӵ��ھ���result���
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}


//----��������-----------------------------------------------------------------
//����: ������������ʾ���ĵ�һ�����ڣ����е���ʾ���������ȴ���������ܴ������ڣ�
//      ����ĳߴ粻��С����ʾ���ĳߴ磬�����Դ�����ʾ���ߴ硣������ˢ��screen
//      �Ĺ����к���ͨ�����ǵ�ͬ�ġ�
//����: para���������ݽṹ�����Ա������ṹ���崦��
//����: �´��ھ��(ָ��)
//-----------------------------------------------------------------------------
struct tagGkWinRsc *GK_CreateDesktop(struct tagGkscParaCreateDesktop *para)
{
    struct tagGkWinRsc *desktop;
    struct tagDisplayRsc *display;
    struct tagRscNode *rsc;
    struct tagClipRect *clip;
    bool_t BufM;
    s32 desktop_x,desktop_y;
    u32 msk_size;
    struct tagGkscParaFillWin para_fill;
    if(para->display == NULL)
    {
        return NULL;
    }
    display = para->display;
    if(para->width >= display->width)
        desktop_x = para->width;            //������Դ�����ʾ���ߴ�
    else
        desktop_x = display->width;   //���治��С����ʾ���ߴ�
    if(para->height >= display->height)
        desktop_y = para->height;
    else
        desktop_y = display->height;
    if(display->frame_buffer == NULL)
    {
        if(para->buf_mode == CN_WINBUF_BUF)
        {
            return NULL;        //��֡�������ʾ��������������������
        }
        BufM = false;
    }
    else
    {
        if(para->buf_mode == CN_WINBUF_NONE)
            BufM = false;
        else
            BufM = true;
    }
    desktop = para->desktop;    //para->desktop�ɵ������ṩ�ڴ棬��ָ�����
    //�����ʼ�����棬����Ҳ�ǰ����ڹ����
    desktop->z_back = desktop;
    desktop->z_top = desktop;
    strcpy(desktop->win_name,"desktop");
    desktop->redraw_clip = NULL;
    desktop->changed_clip = NULL;
    desktop->disp = display;            //������ʾ��
    desktop->z_prio = 0;                //��������ȼ�
    desktop->RopCode = 0;               //����ֻ����0
    desktop->dest_blend = false;        //��������������Ч
    desktop->change_flag = CN_GKWIN_CHANGE_NONE; //���������ʾ
    desktop->absx0 = 0;
    desktop->absy0 = 0;
    desktop->bound_limit = true;
    desktop->left = 0;
    desktop->top = 0;
    desktop->right = desktop_x;
    desktop->bottom = desktop_y;
    desktop->limit_left = 0;
    desktop->limit_top = 0;
    desktop->limit_right = display->width;
    desktop->limit_bottom = display->height;

    display->z_topmost = desktop;
    display->desktop = desktop;
    clip = (struct tagClipRect*)Mb_Malloc(g_ptClipRectPool,0);
    if(clip == NULL)
        return NULL;
    if(BufM)      //����ģʽ��Ϊ����׼��������
    {
        if( __gk_vmalloc(display,desktop,para->PixelFormat) )//�����Դ�
        {
            msk_size = ((desktop_x +7)/8+7)/8*((desktop_y +7)/8);
            //����frame bufferʱ����Ҫchanged_msk
            desktop->changed_msk.bm_bits = M_MallocHeap(msk_size,
                                            display->DisplayHeap,0);
            if( desktop->changed_msk.bm_bits == NULL)   //����changed_msk�ڴ�ʧ��
            {
                display->z_topmost = NULL;
                display->desktop = NULL;
                M_FreeHeap(desktop->changed_msk.bm_bits,display->DisplayHeap);
                Mb_Free(g_ptClipRectPool,clip);
                __gk_vfree(display,desktop);
                Djy_SaveLastError(EN_GK_NO_MEMORY);
                printf("�Դ治��\n\r");
                return NULL;
            }
            else
            {//����changed_msk�ڴ�ɹ������Դ��ٷ���
                memset(desktop->changed_msk.bm_bits,0,msk_size);
                desktop->changed_msk.PixelFormat = CN_SYS_PF_GRAY1;
                desktop->changed_msk.height = desktop_y/8;
                desktop->changed_msk.width = desktop_x/8;
                desktop->changed_msk.linebytes = ((desktop_x+7)/8+7)/8;
                desktop->KeyColor = 0;

                desktop->wm_bitmap->ExColor = para->BaseColor;
            }
        }
        else               //�����Դ�ʧ�ܣ�
        {
            display->z_topmost = NULL;
            display->desktop = NULL;
            Djy_SaveLastError(EN_GK_NO_MEMORY);
            printf("�Դ治��\n\r");
            return NULL;
        }

    }
    else                   //�ǻ���ģʽ������û�л�����
    {
        desktop->wm_bitmap = NULL;
        desktop->changed_msk.bm_bits = NULL;///û����һ����в��������bug
    }
    //��������򣬿������С����ʾ���ߴ��Сһ��
    clip->next = clip;
    clip->previous = clip;
    clip->rect.left = 0;
    clip->rect.top = 0;
    clip->rect.right = display->width;
    clip->rect.bottom = display->height;
    desktop->visible_clip = clip;
    desktop->visible_bak = NULL;

    rsc = Rsc_SearchTree("gkwindow");
    Rsc_AddSon(rsc,&desktop->node,sizeof(struct tagGkWinRsc),RSC_GKWIN,desktop->win_name);

    //�ø�������ɫ�������
    para_fill.gkwin = desktop;
    para_fill.color = para->color;
    GK_FillWin(&para_fill);

    //�����´���ָ��
    return desktop;
}


//----��������-----------------------------------------------------------------
//����: ����һ�����ڣ��´��ڵ����ȼ�Ϊ0��λ���������ȼ�=0�Ĵ��ڵ���ǰ�ˡ�
//����: para���������ݽṹ�����Ա������ṹ���崦��
//����: �´��ھ��(ָ��)
//-----------------------------------------------------------------------------
struct tagGkWinRsc *GK_CreateWin(struct tagGkscParaCreateGkwin *para)
{
    u32 size,RopCode;
    s32 width,height;
    bool_t BufM;
    struct tagGkWinRsc *target_section;
    struct tagGkWinRsc *parent;                       //gkwin�ĸ�����
    struct tagDisplayRsc *display;
    //z���б��ƶ��Ĵ��ڶ����˵Ĵ���
    struct tagGkWinRsc *move_start;
    //z���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ���
    struct tagGkWinRsc *move_end;
    struct tagGkWinRsc *gkwin;
    struct tagGkscParaFillWin para_fill;

    width = para->right - para->left;
    height = para->bottom - para->top;
    display = para->parent_gkwin->disp;
    gkwin = para->gkwin;    //para->gkwin�ɵ������ṩ�ڴ棬��ָ�����
    parent = para->parent_gkwin;
    if((parent == NULL)||(width < 0) || (height < 0))
    {
        return NULL;
    }
    if(display->frame_buffer == NULL)
    {
        if(para->buf_mode == CN_WINBUF_BUF)
        {
            return NULL;        //��֡�������ʾ��������������������
        }
        BufM = false;
    }
    else
    {
        if(para->buf_mode == CN_WINBUF_NONE)
            BufM = false;
        else
            BufM = true;
    }

    RopCode = para->RopCode;
    if((RopCode & (CN_ROP_ROP2_EN + CN_ALPHA_MODE_NEED_AD)) != 0)
        return NULL;   //�������Բ�֧��rop2����ҪDst Alpha�����alpha���

    *gkwin = *parent;               //�´��ڿ��������ڵ�����

    size = strnlen(para->name,CN_GKWIN_NAME_LIMIT);
    memcpy(gkwin->win_name,para->name,size);
    gkwin->win_name[size] = '\0';       //��������\0��

    //�������´��ڵ�����������丸���ڵ�
    gkwin->left = para->left;
    gkwin->top = para->top;
    gkwin->right = para->right;
    gkwin->bottom = para->bottom;
    //���ڵľ���λ�ã����������screen������ԭ��
    gkwin->absx0 = para->left + para->parent_gkwin->absx0;
    gkwin->absy0 = para->top + para->parent_gkwin->absy0;
    gkwin->z_prio = 0;                //�½�����Ĭ��Ϊ0
    display->reset_clip = false;

    gkwin->dest_blend = __GK_RopIsNeedDestination(RopCode);
    gkwin->RopCode = RopCode;

    if(BufM)              //����ģʽ��Ϊ����׼��������
    {
        if( __gk_vmalloc(display,gkwin,para->PixelFormat) )  //�����Դ�
        {
            size = (((width +7)/8+7)/8 *((height +7)/8));
            //����frame bufferʱ����Ҫchanged_msk
            gkwin->changed_msk.bm_bits = M_MallocHeap(size,
                                                    display->DisplayHeap,0);
            if(gkwin->changed_msk.bm_bits == NULL)   //�����޸������ڴ�ʧ��
            {
                __gk_vfree(display,gkwin);
                Djy_SaveLastError(EN_GK_NO_MEMORY);
                printf("�Դ治��\n\r");
                return NULL;
            }
            else
            {
                memset(gkwin->changed_msk.bm_bits,0,size);
                gkwin->changed_msk.PixelFormat = CN_SYS_PF_GRAY1 ;
                gkwin->changed_msk.height = (height + 7)/8;
                gkwin->changed_msk.width = (width + 7)/8;
                gkwin->changed_msk.linebytes = ((width+7)/8+7)/8;
                gkwin->KeyColor = para->KeyColor;
                gkwin->wm_bitmap->ExColor = para->BaseColor;
            }
        }
        else               //�����Դ�ʧ�ܣ�
        {
            Djy_SaveLastError(EN_GK_NO_MEMORY);
            printf("�Դ治��\n\r");
            return NULL;
        }
    }
    else                   //�ǻ���ģʽ������û�л�����
    {
        gkwin->wm_bitmap = NULL;
        gkwin->changed_msk.bm_bits = NULL;///û����һ����в��������bug
    }

    //���°��´������ӵ���Դ�����У������뵽z����
    if(Rsc_GetSon(&para->parent_gkwin->node) == NULL)  //���������Ӵ���
    {
        Rsc_AddSon(&parent->node,&gkwin->node,
                     sizeof(struct tagGkWinRsc),RSC_GKWIN,gkwin->win_name);
        //����4����z���а�win���븸�ڵ��ǰ��
        gkwin->z_back = parent;
        gkwin->z_top = parent->z_top;
        parent->z_top->z_back = gkwin;
        parent->z_top = gkwin;
        if(display->z_topmost == parent)
            display->z_topmost = gkwin;
    }
    else
    {//���������Ӵ���
        //ȡz���б��ƶ�����gkwinͬ���Ĵ��ڶο�ʼ�ͽ������ڶ�
        move_end = (struct tagGkWinRsc*)Rsc_GetSon(&parent->node);
        move_start = (struct tagGkWinRsc*)Rsc_GetPrevious(&move_end->node);
        target_section = move_end;
        while(1)
        {//ɨ��ͬ�������к�Ŀ�괰�����ȼ���ͬ�Ĵ��ڣ�ɨ����z��Ϊ����
            //ɨ��˳��:��z���ǰ�����˽���ɨ�裬ֻɨ�ֵܴ��ڣ���ɨ�Ӵ���
            if(target_section->z_prio >= 0)
                break;
            //ɨ��Ŀ�괰������z������һ���ڣ���ֹ����
            if(target_section == move_start)
                break;
            target_section = (struct tagGkWinRsc*)Rsc_GetNext(&target_section->node);
        }
        if(target_section->z_prio == 0)
        {//ͬ�������д��ں�gkwin��prio��ȵĴ���
            Rsc_AddToPrevious(&target_section->node,&gkwin->node,
                        sizeof(struct tagGkWinRsc),RSC_GKWIN,gkwin->win_name);
            if(Rsc_IsHead(&target_section->node))
                Rsc_RoundPrevious(&parent->node);
            //��ȡtarget_section�����Ӵ�������z��εĽ�������(��ǰ��)
            //�´��ڲ��뵽����ǰ��
            move_end = __GK_GetZsectionAllEnd(target_section);
            if(move_end == display->z_topmost)
                display->z_topmost = gkwin;
            //�嵽Ŀ�괰��ǰ��
            gkwin->z_back = move_end;
            gkwin->z_top = move_end->z_top;
            move_end->z_top->z_back = gkwin;
            move_end->z_top = gkwin;
        }
        else //�ֵܴ�����û�����ȼ�=0�Ĵ���
        {
            //ͬ�������������ȼ�����0�ģ���Դ�������´��ڲ�����ǰ��
            if(target_section->z_prio > 0)
            {
                Rsc_AddToPrevious(&target_section->node,&gkwin->node,
                            sizeof(struct tagGkWinRsc),RSC_GKWIN,gkwin->win_name);
            }
            else    //ͬ���������ȼ��������´���(prio = 0)
            {
                //�´�������Դ�����д���ͬ����������
                Rsc_AddToNext(&target_section->node,&gkwin->node,
                            sizeof(struct tagGkWinRsc),RSC_GKWIN,gkwin->win_name);
            }
            //�´����Ĵ���gkwin���ȼ�Ϊ0������ͬ���������ȼ�����ηֲ�����
            //���ڿ϶��ڸ�����ǰ��
            if(parent == display->z_topmost)
                display->z_topmost = para->gkwin;
            //�嵽Ŀ�괰��ǰ��
            gkwin->z_back = parent;
            gkwin->z_top = parent->z_top;
            parent->z_top->z_back = gkwin;
            parent->z_top = gkwin;
        }
    }
    //���ô��ڵĿɼ��߽磬���ܴ��ڻ����ڵ�Ӱ�죬����������Դ���к���ܵ���
    __GK_SetBound(gkwin);

    gkwin->visible_clip = NULL;
    gkwin->visible_bak = NULL;
    display->reset_clip = true;
    para_fill.gkwin = gkwin;
    para_fill.color = para->color;
    GK_FillWin(&para_fill); //�����޻���Ĵ��ڣ���visible_clip�գ�����ʵ����Ч��

    return gkwin;
}

//----���ڸ���-----------------------------------------------------------------
//����: �޸Ĵ������֣��������127�ַ����������ֽ��ضϡ�
//����: gcwin�����޸ĵĴ��ھ��(ָ��)
//      name���µ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_SetName(struct tagGkWinRsc *gkwin,char *name)
{
    s32 size;
    size = strnlen(name,CN_GKWIN_NAME_LIMIT+1);
    if(size > CN_GKWIN_NAME_LIMIT)     //���ֳ��ȳ�������
        size = CN_GKWIN_NAME_LIMIT;   //ǿ���е���������
    memcpy(gkwin->win_name,name,size);//copy���֣�����ܳ�������δcopy��������\0��
    gkwin->win_name[size] = '\0';     //�����(�ӽ�����)
}

//----��������-----------------------------------------------------------------
//����: ����������ڣ���Ҫ��������������޸Ĺ��Ĳ��֡�����״̬�£�������ʾ����Ļ
//      �ϣ��������ݲ���ı䡣
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void GK_SetUnlock(struct tagGkWinRsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->locked == false)      //������û���������޲���
//        return;
//    gkwin->locked = false;
//    if(gkwin->hided == false)       //�����ڿ��ӣ���Ҫ����ɨ�������
//    {
//        gkwin->disp->reset_clip = true;
//    }
//    return;
//}

//----��������-----------------------------------------------------------------
//����: �������ڣ���Ҫ��������������޸Ĺ��Ĳ���
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void GK_SetLock(struct tagGkWinRsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->locked == true)       //���������������޲���
//        return;
//    gkwin->locked = true;
//}

//----���ش���-----------------------------------------------------------------
//����: �Ѵ�������Ϊ���أ�����������ɨ���������������������ڱ��Լ��ڸǲ��֡�
//      ��Ӱ���Ӵ��ڣ���Ҫͬʱ�����Ӵ��ڣ�Ӧ��gui windows�����
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void GK_SetHide(struct tagGkWinRsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->hided == true)                    //ԭ���������صģ��޲���
//        return;
//    gkwin->hided = true;
//    gkwin->disp->reset_clip = true;
//}

//----��ʾ����-----------------------------------------------------------------
//����: �Ѵ�������Ϊ���ӣ�����������ɨ�������������������ڡ���Ӱ���Ӵ��ڣ�
//      ��Ҫͬʱ�����Ӵ��ڣ�Ӧ��gui windows�����
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void GK_SetShow(struct tagGkWinRsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->hided == false)   //ԭ�����ǿ��ӵģ��޲���
//        return;
//    gkwin->hided = false;
//    gkwin->disp->reset_clip = true;
// }

//----�ı䴰�ڳߴ��λ��-------------------------------------------------------
//����: �ı�һ�����ڵĳߴ��Լ�λ�ã����û�ĳߴ磬ʵ���Ͼ���һ��gk_move_win��
//����: gcwin��Ŀ�괰��
//      new_area���µĴ��ڳߴ��λ��
//      mode���ĳߴ�󣬴���ͼ����ģʽ��0=���ֲ��䣬1=�����ѹ����
//            ���崰�ڲ���Ч
//����: true = �ɹ���false = ʧ��(һ�����ڴ���䵼��)
//ע: �л�����������£����ֻ���ƶ���λ�ã�����Ҫ���»��ơ�
//    ����޸��˳ߴ磬Ӧ�ó���Ӧ���ػ棬ֱ���ػ漴�ɣ�����ȴ��ػ���Ϣ��
//-----------------------------------------------------------------------------
bool_t GK_ChangeWinArea(struct tagGkscParaChangeWinArea *para)
{
    s32 left,top,right,bottom;
    s32 delta_top,delta_left;
    struct tagGkWinRsc *current,*cwawin,bak;
    struct tagDisplayRsc *disp;
    struct tagGkscParaMoveWin movwin_para;

    cwawin = para->gkwin;
    disp = cwawin->disp;
    left = para->left;
    top = para->top;
    right = para->right;
    bottom = para->bottom;
    if(cwawin == disp->desktop)
    {
        if(((right - left) < disp->width) || ((bottom - top) < disp->height))
            return false;                   //����ߴ粻��С����ʾ���ߴ�
    }
    if((right < left) || (bottom < top))    //�ߴ����ݲ��Ϸ�
        return false;

    if(((right-left) == (cwawin->right-cwawin->left))
            && ((bottom-top) == (cwawin->bottom - cwawin->top)))
    {
        movwin_para.gkwin = cwawin;
        movwin_para.left = left;
        movwin_para.top = top;
        GK_MoveWin(&movwin_para);        //ֻ���ƶ��˴��ڣ��ߴ�δ��
        return true;
    }
    if(cwawin->wm_bitmap != NULL)
    {
        bak = *cwawin;
        if( !__gk_vrmalloc(disp,&bak))
        {
            Djy_SaveLastError(EN_GK_NO_MEMORY);
            printf("�Դ治��\n\r");
            return false;   //�����Դ�ʧ�ܣ����ڳߴ�δ�ı�(��Ȼ��Ч)
        }else
        {
            if(bak.wm_bitmap != cwawin->wm_bitmap)
                M_FreeHeap(cwawin->wm_bitmap,disp->DisplayHeap);
            *cwawin = bak;
        }
    }
    if(cwawin == disp->desktop)
    {   //Ŀ�괰�������棬�ж�������:������븲����ʾ������
        s32 dtop,dleft;
        if(para->left > 0)
            dleft = 0;    //������߽���븲����ʾ��
        else if((cwawin->wm_bitmap->width+para->left) < disp->width)
            //�ұ߽���븲����ʾ��
            dleft = disp->width - cwawin->wm_bitmap->width;
        else
            dleft = para->left;
        if(para->top > 0)
            dtop = 0;     //�����ϱ߽���븲����ʾ��
        else if((cwawin->wm_bitmap->height+para->top) < disp->height)
            //�±߽���븲����ʾ��
            dtop =disp->height - cwawin->wm_bitmap->height;
        else
        dtop = para->top;
        delta_left = dleft - cwawin->left;
        delta_top = dtop - cwawin->top;
        cwawin->absx0 += delta_left;//�޸Ĵ��ھ�������
        cwawin->absy0 += delta_top;

        cwawin->top = dtop;
        cwawin->left = dleft;
        cwawin->right = dleft + right - left;
        cwawin->bottom = dtop + bottom - top;

        cwawin->limit_left = dleft;
        cwawin->limit_top = dtop;
        cwawin->limit_right = dleft + disp->width;
        cwawin->limit_bottom = dtop + disp->height;
    }else
    {
        delta_left = para->left-cwawin->left;
        delta_top = para->top-cwawin->top;
        cwawin->absx0 += delta_left;//�޸Ĵ��ھ�������
        cwawin->absy0 += delta_top;

        cwawin->left = left;                      //�޸Ĵ����������
        cwawin->top = top;
        cwawin->right = right;
        cwawin->bottom = bottom;
        __GK_SetBound(cwawin);                        //���ÿ���ʾ�߽�
    }

    current = cwawin;
    cwawin->change_flag = CN_GKWIN_CHANGE_ALL;
    //����gkwin�������Ӵ���,������ɺ�current��=NULL��
    while((current = (struct tagGkWinRsc*)
           Rsc_TraveScion(&(cwawin->node),&(current->node)))
            != NULL)
    {
        current->absx0 += delta_left; //�޸Ĵ��ھ�������
        current->absy0 += delta_top;
        __GK_SetBound(current);
        current->change_flag = CN_GKWIN_CHANGE_ALL;
    }
    cwawin->disp->reset_clip = true;
    return true;
}

//----�ƶ�����-----------------------------------------------------------------
//����: �ı䴰���ڸ������ڵ����λ�ã������Ӵ��ڵ�����������ڸ����ڵģ����ƶ���
//      ��ʱ�����Ӵ���һ���ƶ�����������gk_change_win_area������һ��������
//����: gcwin��Ŀ�괰��
//      left��top���µ����Ͻ����꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_MoveWin(struct tagGkscParaMoveWin *para)
{
    s32 delta_top,delta_left;
    struct tagGkWinRsc *current,*movewin;
    struct tagDisplayRsc *disp;
    movewin = para->gkwin;
    if((para->left == movewin->left) && (para->top == movewin->top))
        return;                                 //λ��û���޸ģ�ֱ�ӷ��ء�
    disp = movewin->disp;
    if(movewin == disp->desktop)
    {   //Ŀ�괰�������棬�ж�������:������븲����ʾ������
        s32 dtop,dleft;
        if(para->left > 0)
            dleft = 0;    //������߽���븲����ʾ��
        else if((movewin->wm_bitmap->width+para->left) < disp->width)
            //�ұ߽���븲����ʾ��
            dleft = disp->width - movewin->wm_bitmap->width;
        else
            dleft = para->left;
        if(para->top > 0)
            dtop = 0;     //�����ϱ߽���븲����ʾ��
        else if((movewin->wm_bitmap->height+para->top) < disp->height)
            //�±߽���븲����ʾ��
            dtop =disp->height - movewin->wm_bitmap->height;
        else
            dtop = para->top;
        if((dtop == movewin->top) || (dleft == movewin->left))
            return;         //����λ��ʵ����δ�����ı�
        else
        {
            delta_left = dleft - movewin->left;
            delta_top = dtop - movewin->top;
            movewin->absx0 += delta_left;//�޸Ĵ��ھ�������
            movewin->absy0 += delta_top;

            movewin->top = dtop;
            movewin->left = dleft;
            movewin->right += delta_left;
            movewin->bottom += delta_top;

            movewin->limit_left = 0-dleft;
            movewin->limit_top = 0-dtop;
            movewin->limit_right = movewin->limit_left + disp->width;
            movewin->limit_bottom = movewin->limit_top + disp->height;
        }
    }else
    {
        delta_left = para->left-movewin->left;
        delta_top = para->top-movewin->top;
        movewin->absx0 += delta_left;//�޸Ĵ��ھ�������
        movewin->absy0 += delta_top;

        movewin->left = para->left;                      //�޸Ĵ����������
        movewin->top = para->top;
        movewin->right += delta_left;
        movewin->bottom += delta_top;
        __GK_SetBound(movewin);                        //���ÿ���ʾ�߽�
    }
    current = movewin;
    current->disp->reset_clip = true;
    movewin->change_flag = CN_GKWIN_CHANGE_ALL;
    //����gkwin�������Ӵ���
    while((current =
             (struct tagGkWinRsc*)Rsc_TraveScion(&(movewin->node),&(current->node)))
             != NULL)
    {
        current->absx0 += delta_left;        //�޸Ĵ��ھ�������
        current->absy0 += delta_top;
        __GK_SetBound(current);
        current->change_flag = CN_GKWIN_CHANGE_ALL;
    }
}

//----���ÿ���ʾ�߽�-----------------------------------------------------------
//����: �κδ��ڣ������ʹ�����Ǳ�z��ǰ�˴��ڼ��У��䴰�ڿ���ʾ��ΧҲ�����޵ģ�
//      ��bound_limit=true�ġ�������������ȴ��ڵĿ���ʾ�߽����ơ�����λ�øı�
//      ����Ҫ���ÿ���ʾ�߽硣
//      gkwin�������õĴ���
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetBound(struct tagGkWinRsc *gkwin)
{
    struct tagGkWinRsc *ancestor;
    s32 gkwin_absx,gkwin_absy,ancestor_absx,ancestor_absy;
    ancestor = gkwin;
    //��ȡ���ȴ����е�һ���߽��ܸ��������Ƶĵ�һ�����ڣ�gkwin�ı߽��ܸô����޶�
    while(ancestor->bound_limit == false)
    {
        ancestor = (struct tagGkWinRsc*)Rsc_GetParent(&ancestor->node);
    }
    ancestor = (struct tagGkWinRsc*)Rsc_GetParent(&ancestor->node);
    //����left��limit_left��������ڸ����ڵģ�����Ҫת���ɾ���������ܱȽ�
    gkwin_absx = gkwin->absx0;  //ȡ�����ڵľ������꣬Ҳ��gkwin->left�ľ�������
    gkwin_absy = gkwin->absy0;
    //ȡ�߽����޴��ڵĿɼ��߽�ԭ���������
    ancestor_absx = ancestor->absx0 +ancestor->limit_left - ancestor->left;
    ancestor_absy = ancestor->absy0 +ancestor->limit_top - ancestor->top;

    //
    if(gkwin_absx < ancestor_absx)
       gkwin->limit_left = gkwin->left + ancestor_absx - gkwin_absx;
    else
        gkwin->limit_left = gkwin->left;

    if(gkwin_absy < ancestor_absy)
       gkwin->limit_top = gkwin->top + ancestor_absy - gkwin_absy;
    else
        gkwin->limit_top = gkwin->top;

    if((gkwin_absx+gkwin->right-gkwin->left)
            < (ancestor_absx+ancestor->limit_right-ancestor->limit_left))
        gkwin->limit_right = gkwin->right;
    else
        gkwin->limit_right = gkwin->right -
                    (  gkwin_absx+gkwin->right-gkwin->left
                   -(ancestor_absx+ancestor->limit_right-ancestor->limit_left));

    if((gkwin_absy+gkwin->bottom-gkwin->top)
            < (ancestor_absy+ancestor->limit_bottom-ancestor->limit_top))
        gkwin->limit_bottom = gkwin->bottom;
    else
        gkwin->limit_bottom = gkwin->bottom -
                    (  gkwin_absy+gkwin->bottom-gkwin->top
                   -(ancestor_absy+ancestor->limit_bottom-ancestor->limit_top));

}
//----���ñ߽�ģʽ-------------------------------------------------------------
//����: �趨���ڵ���ʾ�߽��Ƿ��ܸ��������ƣ����ƺ��Ӵ��ڳ��������ڵĲ��ֽ�����
//      ��ʾ��desktop��ֱ���Ӵ���Ĭ�����ޣ����ܸ��ġ�
//����: gkwin��Ŀ�괰��
//      mode��trueΪ���ޣ�falseΪ������
//����: ��
//-----------------------------------------------------------------------------
void GK_SetBoundMode(struct tagGkscParaSetBoundMode *para)
{
    struct tagGkWinRsc *current;
    if(para->gkwin == NULL)
        return;
    if(para->gkwin == para->gkwin->disp->desktop)   //���治���޸ı߽�ģʽ
        return;
    if(para->gkwin->bound_limit == para->mode)      //ģʽδ�ı�
        return;
    if(&para->gkwin->disp->desktop->node == Rsc_GetParent(&para->gkwin->node))
        return;                         //ֱ�ӷ���������Ӵ��ڣ�����ı�
    //Ŀ�괰�ڱ߽�ģʽ�ı䣬�������Ըı�
    para->gkwin->bound_limit = para->mode;
    __GK_SetBound(para->gkwin);
    current = para->gkwin;
    //����gkwin�������Ӵ���
    while( ( current = (struct tagGkWinRsc*)Rsc_TraveScion
                                    (&para->gkwin->node,&current->node)))
    {
        __GK_SetBound(current);
    }
    para->gkwin->disp->reset_clip = true;
}

//----���ô�����ʾ���ȼ�-------------------------------------------------------
//����: ����һ�����ڵ����ȼ������ȼ�������ͬ�������е�z�����λ�ã�����ԽС����
//      z����Խ��ǰ�����ȼ�<=0�����Ǹ����ڣ���֮�������ڸ��ǡ����ڸı����ȼ���
//      ����Z���е�˳����ܻ�ı䣬��Ļ����Ҳ���ܻ�ı䡣
//      ���ڱ��ı����ȼ��Ĵ��ڿ��ܻ����Ӵ��ڣ����ԣ���z���б��ƶ��ģ�����һ����
//      �ڣ�����������һ�δ��ڡ�
//����: gkwin��Ŀ�괰��
//      prio�������ȼ�
//����: ��
//-----------------------------------------------------------------------------
void GK_SetPrio(struct tagGkscParaSetPrio *para)
{
    struct tagGkWinRsc *target_section,*gkwin;
    struct tagDisplayRsc *display;
    struct tagGkWinRsc *parent;       //gkwin�ĸ�����
    //z���б��ƶ��Ĵ��ڶ����˵Ĵ���
    struct tagGkWinRsc *section_start;
    //z���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ���
    struct tagGkWinRsc *section_end,*target_win;

    gkwin = para->gkwin;
    display = para->gkwin->disp;
    if(gkwin == display->desktop)  //���洰�ڵ����ȼ����ɸı�
        return;


    parent = (struct tagGkWinRsc *)Rsc_GetParent(&gkwin->node);
    //ȡz���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ��ڶ�
    section_end = (struct tagGkWinRsc*)Rsc_GetSon(&parent->node);
    section_start = (struct tagGkWinRsc*)Rsc_GetPrevious(&section_end->node);
    target_section = section_end;
    while(1)
    {//����ͬ�������к����ȼ�����para->prio�ĵĴ��ڣ�ɨ����z��Ϊ����
        //ɨ��˳��:��z���ǰ�����˽���ɨ�裬ֻɨ�ֵܴ��ڣ���ɨ�Ӵ���
        if(target_section->z_prio >= para->prio)
            break;
        //ɨ��Ŀ�괰������z������һ���ڣ���ֹ����
        if(target_section == section_start)
            break;
        target_section = (struct tagGkWinRsc*)Rsc_GetNext(&target_section->node);
    }
    if(target_section->z_prio >= para->prio)
    {// �ҵ�gkwinͬ�����������ȼ����ڻ������prio�Ĵ���
        //����Դ�����������ƶ��������ȼ����п���Ҫ�ƶ�
        if((target_section == gkwin)
                || (target_section == (struct tagGkWinRsc*)Rsc_GetNext(&gkwin->node)))
        {
            if((gkwin->z_prio <= 0) && (para->prio > 0))    //�Ƶ������ں��
            {
                //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
                section_start = __GK_GetZsectionAllStart(gkwin);
                section_end = __GK_GetZsectionAllEnd(gkwin);
                //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
                //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
                if(section_end == display->z_topmost)
                    display->z_topmost = parent;
                //��gkwin����z��δ�z��ȡ����
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;

                section_start->z_top = parent;
                section_end->z_back = parent->z_back;
                parent->z_back->z_top = section_start;
                parent->z_back = section_end;

                gkwin->z_prio = para->prio;
                display->reset_clip = true;
            }
            else if((gkwin->z_prio > 0) && (para->prio <= 0))//�Ƶ�������ǰ��
            {
                //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
                section_start = __GK_GetZsectionAllStart(gkwin);
                section_end = __GK_GetZsectionAllEnd(gkwin);
                //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
                //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
                if(parent == display->z_topmost)
                    display->z_topmost = section_end;
                //��gkwin����z��δ�z��ȡ����
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;

                section_start->z_back = parent;
                section_end->z_top = parent->z_top;
                parent->z_top->z_back = section_end;
                parent->z_top = section_start;

                gkwin->z_prio = para->prio;
                display->reset_clip = true;
            }
            else    //��������������ƶ�z��
            {
                gkwin->z_prio = para->prio;
            }
        }
        else    //��Դ�������ƶ�
        {
            Rsc_MoveToPrevious(&target_section->node,&gkwin->node);

            if((para->prio <= 0) && (target_section->z_prio > 0))
                target_win = parent;
            else
                target_win = __GK_GetZsectionAllEnd(target_section);
            //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
            section_start = __GK_GetZsectionAllStart(gkwin);
            section_end = __GK_GetZsectionAllEnd(gkwin);
            //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
            //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
            if(target_win == display->z_topmost)
                display->z_topmost = section_end;
            //��gkwin����z��δ�z��ȡ����
            section_start->z_back->z_top = section_end->z_top;
            section_end->z_top->z_back = section_start->z_back;

            section_start->z_back = target_win;
            section_end->z_top = target_win->z_top;
            target_win->z_top->z_back = section_end;
            target_win->z_top = section_start;

            gkwin->z_prio = para->prio;
            display->reset_clip = true;
        }
    }
    else
    {//ͬ���������ȼ���������prio
        Rsc_MoveToNext(&target_section->node,&gkwin->node);

        if((para->prio > 0) && (target_section->z_prio <=0))
        {
            target_win = parent;
        }
        else
        {
            //�ҵ�new_location����������ɵ�z��������˵Ĵ���
            target_win = __GK_GetZsectionAllStart(target_section);
        }
        //���°�gkwinȡ��������move_end_temp���
        //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
        section_start = __GK_GetZsectionAllStart(gkwin);
        section_end = __GK_GetZsectionAllEnd(gkwin);

        //��gkwin����z��δ�z��ȡ����
        section_start->z_back->z_top = section_end->z_top;
        section_end->z_top->z_back = section_start->z_back;

        section_start->z_top = target_win;
        section_end->z_back = target_win->z_back;
        target_win->z_back->z_top = section_start;
        target_win->z_back = section_end;

        gkwin->z_prio = para->prio;
        display->reset_clip = true;
    }
}

//----���ù�դ������---------------------------------------------------------
//����: �ı䴰�ڵĹ�դ�����롣�ǻ��崰�ں�����ֻ֧��cn_r3_srccopy��
//        �����޸Ĺ�դ�����롣
//����: para������
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_SetRopCode(struct tagGkscParaSetRopCode *para)
{
    struct tagGkWinRsc *mygkwin;
    struct tagDisplayRsc *mydisplay;

    mygkwin = para->gkwin;
    mydisplay = mygkwin->disp;
    //�ǻ��崰�ں�����ֻ֧��cn_r3_srccopy�������޸�rop_code
    if((mygkwin->wm_bitmap == NULL)
        || (mydisplay->desktop == mygkwin)
        || (para->RopCode == mygkwin->RopCode))
    {
        return false;
    }
    else
    {
        if(para->RopCode != mygkwin->RopCode)
        {
            mygkwin->dest_blend = __GK_RopIsNeedDestination(para->RopCode);
            mygkwin->RopCode = para->RopCode;
            mygkwin->RopCode &= ~CN_ROP_ROP2_MSK;   //�������Բ�֧��rop2
        }
        return true;
    }
}

//----���ô���͸��ɫ-----------------------------------------------------------
//����: �ı䴰��͸��ɫ
//����: gkwin��Ŀ�괰��
//      transparentcolor�����õ�͸��ɫ,��ɫ��ʽ�����ɫ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_SetTransparentColor(struct tagGkscParaSetTransparentColor *para)
{
    //���治��ҪKeyColor
    if(para->gkwin->disp->desktop == para->gkwin)
        return false;
    if(para->gkwin->wm_bitmap == NULL)
        return false;
    para->gkwin->KeyColor = para->KeyColor;

    //����ĳ������ڴ��ڹ�դ��������Ϊ͸����ֻ�ı䵱ǰ͸��ɫ�����
    if(para->gkwin->RopCode & CN_ROP_KEYCOLOR_EN)
    {
        para->gkwin->change_flag = CN_GKWIN_CHANGE_ALL;
    }
    return true;
}

//----���ô���ֱ��д������-----------------------------------------------------
//����: ���ô���ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_SetDirectScreen(struct tagGkscParaSetDirectScreen *para)
{
    if(para->gkwin->direct_screen == true)
        return;
    else
        para->gkwin->direct_screen = true;
}
//----��������ֱ��д������-----------------------------------------------------
//����: ��������ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_UnSetDirectScreen(struct tagGkscParaUnsetDirectScreen *para)
{
    if(para->gkwin->direct_screen == false)
        return;
    else
        para->gkwin->direct_screen = false;
}
//----���ٴ���(�Ӵ���)---------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ��
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void __gk_destroy_win(struct tagGkWinRsc *gkwin)
{
    if(gkwin->disp->z_topmost == gkwin)
        gkwin->disp->z_topmost = gkwin->z_back;
    gkwin->z_back->z_top = gkwin->z_top;
    gkwin->z_top->z_back = gkwin->z_back;
    Rsc_DelNode(&gkwin->node);
    if(gkwin->visible_clip != NULL)
        gkwin->disp->reset_clip = true;
    if(gkwin->disp->frame_buffer != NULL)
    {
        if(gkwin->wm_bitmap != NULL)//��֡�����ڻ���ģʽ��
        {
            __gk_vfree(gkwin->disp,gkwin);
            M_FreeHeap(gkwin->changed_msk.bm_bits,gkwin->disp->DisplayHeap);
            gkwin->visible_clip =
                            __GK_FreeClipQueue(gkwin->visible_clip);
            return;
        }
        else
            return;
    }
    else
        return;
}
//----���ٴ���-------------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ�����ٴ��ڣ�ͬʱ���ٴ��ڵ����ﴰ�ڡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_DestroyWin(struct tagGkscParaDestroyWin *para)
{
    struct tagGkWinRsc *gkwin;

    while((gkwin = (struct tagGkWinRsc *)Rsc_GetTwig(&para->gkwin->node))!= NULL)
    {
        __gk_destroy_win(gkwin);
    }
    __gk_destroy_win(para->gkwin);

}

//----�������redraw����-------------------------------------------------------
//����: �����д������޸ĵĿ������������ʾ��������������Ѿ�׼���á�
//����: display����ɨ�����ʾ��
//����: ��
//-----------------------------------------------------------------------------
void __GK_OutputRedraw(struct tagDisplayRsc *display)
{
    s32 x,y,x_src,y_src,x_dst,y_dst;
    u32 color;
    struct tagClipRect *clip;
    struct tagDisplayRsc *mirror,*current;
    struct tagGkWinRsc *gkwin,*desktop_gkwin,*topwin,*frame_buf;
    struct tagGkucParaRepaint repaint;
    struct tagRectBitmap *src,*dst;
    struct tagRectangle *rect,SrcRect;
    struct tagRectBitmap *bitmap;

    frame_buf = display->frame_buffer;          //ȡ֡���崰��
    desktop_gkwin = display->desktop;
    topwin = display->z_topmost;
    if(frame_buf == NULL)   //��֡���壬��֡�������ʾ������Ȼû�д��ڻ���
    {
        //��û�л�����������£����Ʋ����ǲ�������redraw_clip�ģ�ֻ�д��ڹ���
        //�����Ż�Ҫ���ػ档
        gkwin = desktop_gkwin;
        do{
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                repaint.win = (void*)gkwin;
                repaint.redraw_clip = clip;
                GK_UsercallChunnel(CN_GKUC_REPAINT,g_ptUsercallSemp,NULL,
                                  &repaint,sizeof(struct tagGkucParaRepaint));
                //�ȴ��û�����ػ��������ʱ(1��)����
                Lock_SempPend(g_ptUsercallSemp,0);
                gkwin->redraw_clip = __GK_FreeClipQueue(gkwin->redraw_clip);
            }
            gkwin = gkwin->z_top;
        }while(gkwin != topwin->z_top);
    }else                                           //��֡����
    {
        gkwin = desktop_gkwin;
        do{
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                if(gkwin->wm_bitmap == NULL)   //�޴��ڻ���
                {
                    repaint.win = (void*)gkwin;
                    repaint.redraw_clip = clip;
                    GK_UsercallChunnel(CN_GKUC_REPAINT,g_ptUsercallSemp,NULL,
                                    &repaint,sizeof(struct tagGkucParaRepaint));
                    //�ȴ��û�����ػ��������ʱ(1��)����
                    Lock_SempPend(g_ptUsercallSemp,0);
                }else                                   //�д��ڻ���
                {
                    do
                    {
                        SrcRect.left = clip->rect.left-gkwin->absx0;
                        SrcRect.top = clip->rect.top-gkwin->absy0;
                        SrcRect.right = clip->rect.right -gkwin->absx0;
                        SrcRect.bottom = clip->rect.bottom -gkwin->absy0;
                        if(!display->draw.blt_bm_to_bm(
                                            frame_buf->wm_bitmap,
                                            &clip->rect,
                                            gkwin->wm_bitmap,
                                            &SrcRect,
                                            gkwin->RopCode,
                                            gkwin->KeyColor))
                        {
                            dst = frame_buf->wm_bitmap;
                            src = gkwin->wm_bitmap;

                            y_dst = clip->rect.top;
                            if(gkwin->RopCode == 0)
                            {
                                for(y_src = clip->rect.top-gkwin->absy0;
                                    y_src < clip->rect.bottom-gkwin->absy0;
                                    y_src++)
                                {
                                    x_dst = clip->rect.left;
                                    for(x_src = clip->rect.left-gkwin->absx0;
                                        x_src < clip->rect.right-gkwin->absx0;
                                        x_src++)
                                    {
                                        __GK_CopyPixelBm(dst,src,x_dst,y_dst,
                                                            x_src,y_src);
                                        x_dst ++;
                                    }
                                    y_dst ++;
                                 }
                            }
                            else
                            {
                                for(y_src = clip->rect.top-gkwin->absy0;
                                    y_src < clip->rect.bottom-gkwin->absy0;
                                    y_src++)
                                {
                                    x_dst = clip->rect.left;
                                    for(x_src = clip->rect.left-gkwin->absx0;
                                        x_src < clip->rect.right-gkwin->absx0;
                                        x_src++)
                                    {
                                        __GK_CopyPixelRopBm(dst,src,
                                                x_dst,y_dst,x_src,y_src,
                                                gkwin->RopCode,gkwin->KeyColor);
                                        x_dst ++;
                                    }
                                    y_dst ++;
                                }
                            }
                        }
                        __GK_ShadingRect(frame_buf,&clip->rect);
                        clip = clip->next;
                    }while(clip != gkwin->redraw_clip);
                }
                gkwin->redraw_clip = __GK_FreeClipQueue(gkwin->redraw_clip);
            }
            gkwin = gkwin->z_top;
        }while(gkwin != topwin->z_top);
        //���°�֡�������������ʾ��
        clip = __GK_GetChangedClip(frame_buf);  //��ȡ֡�����б��޸ĵĿ�����
        if(clip != NULL)
        {
            __GK_ShadingClear(frame_buf);
            frame_buf->redraw_clip = clip;
            mirror = (struct tagDisplayRsc *)gkwin->disp->node.child;//������ʾ��
            do{
                if(display->framebuf_direct == false)
                {
                    //�����screen
                    if(display->draw.bm_to_screen(&clip->rect,
                                                frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top))
                    {
                    }
                    else
                    {
                        rect = &clip->rect;
                        bitmap = frame_buf->wm_bitmap;
                        for(y = rect->top;y < rect->bottom;y++)
                        {
                            for(x = rect->left;x < rect->right;x++)
                            {
                                color = GK_GetPixelBm(bitmap,x,y);
                                color =GK_ConvertColorToRGB24(bitmap->PixelFormat,
                                                         color,bitmap->ExColor);
                                display->draw.set_pixel_screen(x,y,color,
                                                                CN_R2_COPYPEN);
                            }
                        }
                    }
                }
                current = mirror;

                while(current != NULL)  //����ȫ�����񴰿�
                {
                    current->draw.bm_to_screen(&clip->rect,
                                                frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top);
                    current = (struct tagDisplayRsc *)Rsc_TraveScion(
                                                &mirror->node,&current->node);
                }
                clip = clip->next;
            }while(clip != frame_buf->redraw_clip);
            frame_buf->redraw_clip =
                            __GK_FreeClipQueue(frame_buf->redraw_clip);
        }
    }
}

//----��ʾ�����������ʾ��-----------------------------------------------------
//����: ��������ʾ������Ҫ����Ĳ����������ʾ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_redraw_all(void)
{
    struct tagDisplayRsc *display,*display_next;
    display = (struct tagDisplayRsc *)Rsc_GetSon(&s_tDisplayRootRsc);
    display_next = display;
    if(display != NULL)
    {
        do{
            __GK_GetRedrawClipAll(display_next);//ɨ��ȡ���µ����ػ������
            __GK_OutputRedraw(display_next);    //�ػ�
            display_next = (struct tagDisplayRsc *)
                                    Rsc_GetNext(&display_next->node);
        }while(display != display_next);  //ɨ��������ʾ��
    }
}
//----ϵͳ���ùܵ�-------------------------------------------------------------
//����: gui api��gui kernel�Ľӿں�������api������Ļ���Ҫ�󴫵ݵ�gui kernel����
//      mp�汾�У����Կ���ʹ�����ж�ʵ�֡�
//����: param��������ַ
//      size����������
//����: ʵ��д��ܵ�����������0����size��
//-----------------------------------------------------------------------------
u16 GK_SyscallChunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2)
{
    u16 completed = 0;
    u8 buf[2];
    Lock_MutexPend(g_tGkChunnel.syscall_mutex,1000*mS);   //�ܵ����ʻ���
    while(1)
    {
        if((Ring_Capacity(&g_tGkChunnel.ring_syscall)
                    - Ring_Check(&g_tGkChunnel.ring_syscall)) <(u32)(size1+size2+2))
        {       //�ܵ�������������
            if(!Lock_SempPend(g_tGkChunnel.syscall_semp,1000*mS))//�ȴ��ܵ��ͷſռ�
                break;                                  //�ȴ��ܵ�ʧ��
            else
                continue;                               //�ٴμ������
        }
        buf[0] = (u8)command;
        buf[1] = (u8)(command>>8);
        completed = Ring_Write(&g_tGkChunnel.ring_syscall,buf,2);
        if(size1 != 0)
            completed += Ring_Write(&g_tGkChunnel.ring_syscall,param1,size1);
        if(size2 != 0)
            completed += Ring_Write(&g_tGkChunnel.ring_syscall,param2,size2);
        break;
    }
    Lock_MutexPost(g_tGkChunnel.syscall_mutex);    //�ܵ����ʻ�����
    if(Djy_IsMultiEventStarted())
    {
        if(Lock_SempQueryFree(g_ptSyscallSemp) == 0)
            Lock_SempPost(g_ptSyscallSemp);
        if(0 != sync_time)      //�趨�˵ȴ�ʱ��
        {
            Lock_MutexPend(g_ptGkServerSync,sync_time);
            Lock_MutexPost(g_ptGkServerSync);
        }
    }
    else
    {
        Ring_Read(&g_tGkChunnel.ring_syscall,(u8*)draw_chunnel_buf,
                                        gc_u32CfgGuiCmdDeep);
        __ExecOneCommand(command,draw_chunnel_buf+2);
    }
    return completed;
}


//----�û����ùܵ�-------------------------------------------------------------
//����: ��������gk_syscall_chunnle�������������������gui kernel��Ҫ�ϲ�Ӧ�ó���
//      ���ĳЩ����ʱ�����ñ��������Ѳ���Ҫ��ͨ���ܵ�������ȥ��
//����: param��������ַ
//      size����������
//����: ʵ��д��ܵ�����������0����size��
//ע: usercall_bufֻ��gkwin������д�����������軥��������
//-----------------------------------------------------------------------------
u16 GK_UsercallChunnel(u16 usercall_id,struct tagSemaphoreLCB *semp,
                        void **result,void *param,u16 size)
{
    u16 completed = 0,loop;
    u8 buf[100];    //�����г���100�ֽڵ�����ġ�
    while(1)
    {
        if((Ring_Capacity(&g_tGkChunnel.ring_usercall)
                    - Ring_Check(&g_tGkChunnel.ring_usercall)) < size)
        {       //�ܵ�������������
            if(!Lock_SempPend(g_tGkChunnel.usercall_semp,0))//�ȴ��ܵ�����ռ�
                break;                                  //�ȴ��ܵ�ʧ��
            else
                continue;                               //�ٴμ������
        }
        buf[0] = (u8)usercall_id;
        buf[1] = (u8)(usercall_id>>8);
        for(loop = 0; loop < sizeof(void*); loop++)
        {
            buf[2+loop] = ((u8*)&semp)[loop] ;
        }
        for(loop = 0; loop < sizeof(void*); loop++)
        {
            buf[2 + sizeof(void*) +loop] = ((u8*)&result)[loop] ;
        }
        for(loop = 0; loop < size-2+2*sizeof(void*); loop++)
        {
            buf[2+2*sizeof(void*)+loop] = ((u8*)param)[loop] ;
        }
        completed = Ring_Write(&g_tGkChunnel.ring_usercall,buf,
                                size+2+sizeof(void*));
    }
    return completed;
}

//����: �����(�ֽ���)
u32 __ExecOneCommand(u16 DrawCommand,u8 *ParaAddr)
{
    u32 result = 0;
    switch(DrawCommand)
    {
        case CN_GKSC_CREAT_GKWIN:
        {
            struct tagGkscParaCreateGkwin para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaCreateGkwin));
            *(para.result) = (void*)GK_CreateWin(&para);
            result = sizeof(struct tagGkscParaCreateGkwin);
        } break;
        case CN_GKSC_SET_PIXEL:
        {
            struct tagGkscParaSetPixel para;
            memcpy(&para,ParaAddr,sizeof(struct tagGkscParaSetPixel));
            GK_SetPixel(&para);
            result = sizeof(struct tagGkscParaSetPixel);
        } break;
        case CN_GKSC_LINETO:
        {
            struct tagGkscParaLineto para;
            memcpy(&para,ParaAddr,sizeof(struct tagGkscParaLineto));
            GK_Lineto(&para);
            result = sizeof(struct tagGkscParaLineto);
        } break;
        case CN_GKSC_LINETO_INC_END:
        {
            struct tagGkscParaLineto para;
            memcpy(&para,ParaAddr,sizeof(struct tagGkscParaLineto));
            GK_LinetoIe(&para);
            result = sizeof(struct tagGkscParaLineto);
        } break;
        case CN_GKSC_DRAW_BITMAP_ROP:
        {
            struct tagGkscParaDrawBitmapRop para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaDrawBitmapRop));
            GK_DrawBitMap(&para);
            result = sizeof(struct tagGkscParaDrawBitmapRop);
        } break;
        case CN_GKSC_FILL_WIN:
        {
            struct tagGkscParaFillWin para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaFillWin));
            GK_FillWin(&para);
            result = sizeof(struct tagGkscParaFillWin);
        } break;
#if 0
        case CN_GKSC_FILL_PART_WIN:
        {
            struct tagGkscParaFillPartWin para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaFillPartWin));
            GK_FillPartWin(&para);
            result = sizeof(struct tagGkscParaFillPartWin);
        } break;
#endif
        case CN_GKSC_FILL_RECT:
        {
            struct tagGkscParaGradientFillWin para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaGradientFillWin));
            GK_GradientFillRect(&para);
            result = sizeof(struct tagGkscParaGradientFillWin);
        } break;
        case CN_GKSC_SET_ROP_CODE:
        {
            struct tagGkscParaSetRopCode para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaSetRopCode));
            GK_SetRopCode(&para);
            result = sizeof(struct tagGkscParaSetRopCode);
        } break;
        case CN_GKSC_DRAW_TEXT:
        {
            struct tagGkscParaDrawText para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaDrawText));
            result = sizeof(struct tagGkscParaDrawText);
            GK_DrawText(&para,(char*)ParaAddr+result);
            result += para.count;
        }break;
        case CN_GKSC_SET_TRANSPARENTCOLOR:
        {
            struct tagGkscParaSetTransparentColor para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaSetTransparentColor));
            GK_SetTransparentColor(&para);
            result = sizeof(struct tagGkscParaSetTransparentColor);
        } break;
        case CN_GKSC_DESTROY_WIN:
        {
            struct tagGkscParaDestroyWin para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaDestroyWin));
            GK_DestroyWin(&para);
            result = sizeof(struct tagGkscParaDestroyWin);
        } break;
        case CN_GKSC_SET_PRIO:
        {
            struct tagGkscParaSetPrio para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaSetPrio));
            GK_SetPrio(&para);
            result = sizeof(struct tagGkscParaSetPrio);
        } break;
        case CN_GKSC_SET_BOUND_MODE:
        {
            struct tagGkscParaSetBoundMode para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaSetBoundMode));
            GK_SetBoundMode(&para);
            result = sizeof(struct tagGkscParaSetBoundMode);
        }break;
        case CN_GKSC_MOVE_WIN:
        {
            struct tagGkscParaMoveWin para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaMoveWin));
            GK_MoveWin(&para);
            result = sizeof(struct tagGkscParaMoveWin);
        }break;
        case CN_GKSC_CHANGE_WIN_AREA:
        {
            struct tagGkscParaChangeWinArea para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaChangeWinArea));
            GK_ChangeWinArea(&para);
            result = sizeof(struct tagGkscParaChangeWinArea);
        }break;
        case CN_GKSC_DRAW_CIRCLE:
        {
            struct tagGkscParaDrawCircle para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaDrawCircle));
            GK_DrawCircle(&para);
            result = sizeof(struct tagGkscParaDrawCircle);
        } break;
        case CN_GKSC_BEZIER:
        {
            struct tagGkscParaBezier para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaBezier));
            GK_Bezier(&para);
            result = sizeof(struct tagGkscParaBezier);
        } break;
        case CN_GKSC_SET_DIRECT_SCREEN:
        {
            struct tagGkscParaSetDirectScreen para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaSetDirectScreen));
            GK_SetDirectScreen(&para);
            result = sizeof(struct tagGkscParaSetDirectScreen);
        } break;
        case CN_GKSC_UNSET_DIRECT_SCREEN:
        {
            struct tagGkscParaUnsetDirectScreen para;
            memcpy(&para,ParaAddr,
                    sizeof(struct tagGkscParaUnsetDirectScreen));
            GK_UnSetDirectScreen(&para);
            result = sizeof(struct tagGkscParaUnsetDirectScreen);
        } break;
        case CN_GKSC_SYNC_SHOW:
        {
            __gk_redraw_all();
        } break;
        default: break;
    }       //for switch(command)
    return result;
}

//----gui kernel������---------------------------------------------------------
//����: �ӻ�����ȡ����ʾ���󣬲�����֮��
//����: ��
//����: ��
//-----------------------------------------------------------------------------
ptu32_t GK_Server(void)
{
    u16 command;
    u32 num,offset;
    Lock_MutexPend(g_ptGkServerSync,CN_TIMEOUT_FOREVER);
    Djy_SetEventPrio(248);
    while(1)
    {
        //һ�ζ�ȡȫ�������Ϊ����ʱ�л��������������Թܵ��е����ݿ϶���������
        //��������ڰ�������Ŀ��ܡ�
        num = Ring_Read(&g_tGkChunnel.ring_syscall,(u8*)draw_chunnel_buf,
                                        gc_u32CfgGuiCmdDeep);

        if(num == 0)
        {
            //���������ִ����󣬼����û��win buffer��Ҫˢ��screen��
//            __gk_redraw_all();

            Djy_SetEventPrio(1);
            Lock_MutexPost(g_ptGkServerSync);
            Lock_MutexPend(g_ptGkServerSync,CN_TIMEOUT_FOREVER);
            Djy_SetEventPrio(248);
            Lock_SempPend(g_ptSyscallSemp,CN_TIMEOUT_FOREVER);
            continue;
        }
        if(Lock_SempQueryFree(g_tGkChunnel.syscall_semp) == 0)
            Lock_SempPost(g_tGkChunnel.syscall_semp);    //�ͷŹܵ�
        offset = 0;
        while(num > offset)
        {
            //���ڹܵ��е����ݿ��ܲ����룬�ʱ��������copy����������ֱ����ָ��
            //ָ���ǿ������ת��
            command = draw_chunnel_buf[offset] + ((u16)draw_chunnel_buf[offset+1]<<8);
            //sizeof(u16)������2�������cn_byte_bits>=16�Ļ����ϣ�sizeof(u16)=1.
            offset += sizeof(u16);
//            sync_draw = draw_chunnel_buf[offset];
//            offset += 1;
            offset += __ExecOneCommand(command,draw_chunnel_buf + offset);
        }   //for while(num != offset)
    }   //for while(1)

}

