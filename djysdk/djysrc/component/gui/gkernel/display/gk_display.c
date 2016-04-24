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
//�ļ�����: gui core����ʾ�����֣�������Ӳ����ز���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "rsc.h"
#include "gkernel.h"
#include "..\include\gk_win.h"
#include "gk_display.h"
#include <string.h>

//----��װ��ʾ��---------------------------------------------------------------
//����: ��һ̨����ʾ���Ǽǵ���ʾ����Դ�����С�
//����: display������װ����ʾ����Ӧ����driver�н��б�Ҫ�ĳ�ʼ��
//      frame_bitmap,֡���壬��ΪNULL�����ʾ����ʾ��û��֡���壬֡������һ����
//          ֱ�����ڴ淽ʽ���ʵĴ洢�����ɣ�������һЩlcdģ�飬��Ȼģ���ڲ���һ
//          ���ڴ�����֡���壬���䲻��ӳ�䵽ϵͳ�ڴ棬�����������
//      name����ʾ��������ָ����ַ����ڴ��������Ǿֲ�����
//����: true=�ɹ���ʼ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_InstallDisplay(struct tagDisplayRsc *display,
//                        struct tagRectBitmap *frame_bitmap,
                          const char *name)
{
    struct tagRscNode *rsc;
    struct tagRectBitmap *frame_bitmap;
    struct tagGkWinRsc *frame_buffer;
    u32 msk_size;
    if(display == NULL)
        return false;
    display->desktop = NULL;
    display->reset_clip = false;
    //�û�û��ָ���Դ�ר�õĶѣ���ʹ��ϵͳ�ѡ�
    if(display->DisplayHeap == NULL)
        display->DisplayHeap = M_FindHeap("sys");
    frame_buffer = display->frame_buffer;
    if(frame_buffer != NULL)
    {
        frame_bitmap = frame_buffer->wm_bitmap;
        if((frame_bitmap->PixelFormat&CN_PF_INDEX_MSK) >= CN_DISPLAY_PF_LIMIT)
            return false;       //���������ʾ����ʽ
        frame_buffer->changed_msk.PixelFormat = CN_SYS_PF_GRAY1 ;
        frame_buffer->changed_msk.height = (frame_bitmap->height+7)/8;
        frame_buffer->changed_msk.width = (frame_bitmap->width+7)/8;
        frame_buffer->changed_msk.linebytes = (frame_buffer->changed_msk.width+7)/8;
        msk_size = frame_buffer->changed_msk.linebytes *
                                frame_buffer->changed_msk.height;
        frame_buffer->changed_msk.bm_bits = M_Malloc(msk_size,0);
        if(frame_buffer->changed_msk.bm_bits== NULL)    //�����޸������ڴ�ʧ��
        {
            return false;
        }
        memset(frame_buffer->changed_msk.bm_bits,0,msk_size);

        frame_buffer->user_private = NULL;
        frame_buffer->z_back = NULL;
        frame_buffer->z_top = NULL;
        frame_buffer->visible_bak = NULL;
        frame_buffer->visible_clip = NULL;
        frame_buffer->redraw_clip = NULL;
        frame_buffer->changed_clip = NULL;
        frame_buffer->copy_clip = NULL;
        frame_buffer->disp = display;
        frame_buffer->z_prio = 0;
        frame_buffer->direct_screen = false;
        frame_buffer->KeyColor = 0;
        frame_buffer->RopCode = 0;
        //֡��������CN_R3_SRCCOPY��dest_blend����FALSE
        frame_buffer->dest_blend = false;
        //֡��������CN_R3_SRCCOPY������Ҫpattern��
//        frame_buffer->pat_buf = NULL;
        frame_buffer->change_flag = CN_GKWIN_CHANGE_NONE;
        frame_buffer->bound_limit = true;
        frame_buffer->absx0 = 0;
        frame_buffer->absy0 = 0;
        frame_buffer->left = 0;
        frame_buffer->top = 0;
        frame_buffer->right = frame_bitmap->width;
        frame_buffer->bottom = frame_bitmap->height;
        frame_buffer->limit_left = 0;
        frame_buffer->limit_top = 0;
        frame_buffer->limit_right = frame_bitmap->width;
        frame_buffer->limit_bottom = frame_bitmap->height;
    }
    rsc = Rsc_SearchTree("display");     //ȡ��ʾ����Դ��
    if(NULL != Rsc_AddSon(rsc,&display->node,sizeof(struct tagDisplayRsc),RSC_DISPLAY,name))
        return true;
    else
        return false;
}

//----��װ������ʾ��-----------------------------------------------------------
//����: ��һ̨��ʾ����Ϊ��ǰĬ����ʾ���ľ�����ʾ���Ǽǵ���ʾ����Դ�����С�
//����: display������װ����ʾ����Ӧ����driver�н��б�Ҫ�ĳ�ʼ��
//      name����ʾ��������ָ����ַ����ڴ��������Ǿֲ�����
//����: true=�ɹ���ʼ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_InstallDisplayMirror(struct tagDisplayRsc *base_display,
                                 struct tagDisplayRsc *mirror_display,char *name)
{
    if(NULL != Rsc_AddSon(&base_display->node,&mirror_display->node,
                            sizeof(struct tagDisplayRsc),RSC_DISPLAY,name))
        return true;
    else
        return false;
}

//----����Ĭ����ʾ��-----------------------------------------------------------
//����: ����Ĭ����ʾ��
//����: name����ʾ����
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_SetDefaultDisplay(char *name)
{
    struct tagRscNode *rsc;
    rsc = Rsc_SearchTree("display");     //ȡ��ʾ����Դ��
    rsc = Rsc_SearchSon(rsc,name);     //�ҵ�����������ʾ����Դ
    if(rsc == NULL)
    {
        return false;
    }else
    {
        Rsc_MoveToHead(rsc);          //��Դ���еĶ���ͷ������Ĭ����ʾ��
        return true;
    }
}

//----ȡ��ʾ����Ĭ����ʾ����---------------------------------------------------
//����: ȡһ����ʾ����Ĭ����ʾ���ã�ʵ���Ͼ������洰�ڵ���Դ�ڵ�
//����: display����ʾ��ָ��
//����: draw_setָ��
//-----------------------------------------------------------------------------
struct tagGkWinRsc *GK_GetRootWin(struct tagDisplayRsc *display)
{
    return display->desktop;
}

//----�л�frame buffer---------------------------------------------------------
//����: ��һ����ʾ���ж��frame bufferʱ���ñ������л���ǰframe buffer����ϵͳ
//      �У�frame buffer��һ����Դ
//����: display������������ʾ��
//      fbuf��������ָ��
//����: true=�ɹ���false=ʧ�ܣ�ʧ�ܵ�ԭ��ֻ��һ��:����ʾ��������֡������
//-----------------------------------------------------------------------------
bool_t GK_SwitchFrameBuffer(struct tagDisplayRsc *display,
                              struct tagRectBitmap *fbuf)
{
    return true;
}

//----����frame buffer---------------------------------------------------------
//����: Ϊĳ��ʾ������һ��frame buffer
//����: display������������ʾ��
//����: ��frame buffer ָ�룬NULL=����ʧ��
//-----------------------------------------------------------------------------
struct tagRectBitmap *GK_CreateFrameBuffer(struct tagDisplayRsc *display)
{
    return NULL;
}


