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

#ifndef __GK_DISPLAY_H__
#define __GK_DISPLAY_H__
#include "stdint.h"
#include "gkernel.h"
#ifdef __cplusplus
extern "C" {
#endif
//���Ʋ������Ĵ���:
//1����bitmap�ϻ���
//2����screen�ϻ���
//3��ȡbitmap�ϵ����ػ�bitmap
//4��ȡscreen�ϵ����ػ�bitmap��
//��bitmap�ϣ������ء���ʱ���Ǵ�rop2������ģ�Ӳ��:��֧�֡�����֧�֡�ȫ֧��
//bitmap�������ε�ʱ�򣬲���rop2�����룬Ӳ������֧�֣�Ҳ���ܲ�֧��
//���ϼ����������������NULL��֮������֧�ֵ�ʵ��֮����֧�ֵķ���false����gui
//kernelʵ��֮�������Ļ���Ҳ������check_raster�����ˡ�

//gui kernelȷ��������Ա�����Ĳ�������Ϊ��ͼĿ���bitmap��������win buffer����
//frame buffer�е�bitmap�������û������ṩ��bitmap��
//����Ϊͼ��Դ��bitmap���������cn_sys_pf_xxx�е�����һ�֣������뱾��ʾ����ʹ��
//�����ظ�ʽ��ͬ
struct tagDispDraw
{
    //�ھ���λͼ�л�һ�����أ�����ʾ��ʹ��cn_custom_pf��ʽ��������Ӳ�����ٹ��ܣ�
    //Ӧ��ʵ���������������ֱ�ӷ���false����ʹ��Ӳ�����٣����ü��ٹ��ܲ�֧��
    //r2_code����Ļ���Ҳ����false��color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*set_pixel_bm)(struct tagRectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code);
    //�ھ���λͼ�л�һ������ϸֱ�ߣ������˵㣬����ʾ��ʹ��cn_custom_pf��ʽ������
    //��Ӳ�����ٹ��ܣ�Ӧ��ʵ���������������ֱ�ӷ���false����ʹ��Ӳ�����٣�����
    //���ٹ��ܲ�֧��r2_code����Ļ���Ҳ����false��color��ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_bm)(struct tagRectBitmap *bitmap,struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //�ھ���λͼ�л�һ������ϸֱ�ߣ����˵㣬����ʾ��ʹ��cn_custom_pf��ʽ������
    //��Ӳ�����ٹ��ܣ�Ӧ��ʵ���������������ֱ�ӷ���false����ʹ��Ӳ�����٣�����
    //���ٹ��ܲ�֧��r2_code����Ļ���Ҳ����false��color��ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_bm_ie)(struct tagRectBitmap *bitmap,struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //Bitmap�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬driver���Լ򻯣�ֱ��
    //����false���ɡ�
    //color�ĸ�ʽ��CN_SYS_PF_ERGB8888
    //Target: Ŀ���������,����ɫ���ʱ����
    //Focus: ʵ���������
    //Color0,Color1:Target���ϽǺ����½���ɫֵ,������ǽ������,��ֻ��ҪColor0
    //Mode: ���ģʽ,CN_FILLRECT_MODE_N�峣��
    bool_t (*fill_rect_bm)(    struct tagRectBitmap *DstBitmap,
                               struct tagRectangle *Target,
                               struct tagRectangle *Focus,
                               u32 Color0,u32 Color1,u32 Mode);

    //����������λͼ��λ�鴫�ͣ������ʾ��ʹ�õ����ظ�ʽ��cn_custom_pf������Ӳ��
    //2d����֧��λ�鴫�ͣ���ʵ���������������ֱ�ӷ���false��
    //������Ӳ�����٣�����ֻ֧�ֲ���rop_code��������������ʵ��֧�ֵ�rop_code��
    //��֧�ֵĲ��֣�����false��
    //����������false�Ļ���gui kernel�������������ط���������
    bool_t (*blt_bm_to_bm)( struct tagRectBitmap *dst_bitmap,
                            struct tagRectangle *DstRect,
                            struct tagRectBitmap *src_bitmap,
                            struct tagRectangle *SrcRect,
                            u32 RopCode,u32 transparentcolor);
    //�Ӿ���λͼ��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8�������ʾ����ʽ����
    //cn_custom_pf����ΪNULL��
    u32 (*get_pixel_bm)(struct tagRectBitmap *bitmap,s32 x,s32 y);
    //��srcλͼ��rect������������ݸ��Ƶ�dest�У�����ǰ�������ú�dest��pf_type��
    //�����������src��pf_type��ֱ��ʹ��screen�ĸ�ʽ��
    //���������ڴӴ��ڼ��о��Σ���blt_bm_to_bm������ʾ���Ρ�
    //�����ʾ��ʹ�õ����ظ�ʽ��cn_custom_pf������Ӳ��2d����֧��bm�俽��ͼ�Σ���
    //ʵ���������������ֱ�ӷ���false��
    //����dest->pf_type���ںܶ���ܣ���ʹ��Ӳ�����٣�Ҳ����ֻ֧�ֲ���pf_type����
    //�����Բ�֧�ֵĸ�ʽ������false��
    bool_t (*get_rect_bm)(struct tagRectBitmap *src,struct tagRectangle *rect,
                          struct tagRectBitmap *dest);

    //��������screen�л�ͼ�ĺ���
    //��screen�л�һ�����أ���frame buffer������£�������ʾgui����������������
    //�������direct_screen==true������ܵ��ñ�������
    //��ˣ������Ƿ���frame buffer��driver�������ṩ���ұ���ʵ�ֱ�����
    //������ʾ����driver���ṩ���������Ҳ����ʵ�ֱ�����
    bool_t (*set_pixel_screen)(s32 x,s32 y,u32 color,u32 r2_code);
    //��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�driver
    //���Լ򻯣�ֱ�ӷ���false���ɡ���frame buffer������£�������ʾgui���������
    //���������������direct_screen==true������ܵ��ñ�����������������false��
    //�������һ������set_pixel_screen����
    //������ʾ����driver���ṩ������������ڲ�֪��������ʾ���������������ʾ��
    //driver����ʵ�ֱ����������ܼ򵥷���false��
    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_screen)(struct tagRectangle *limit,
                          s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);

    //��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�driver
    //���Լ򻯣�ֱ�ӷ���false���ɡ���frame buffer������£�������ʾgui���������
    //���������������direct_screen==true������ܵ��ñ�����������������false��
    //�������һ������set_pixel_screen����
    //������ʾ����driver���ṩ������������ڲ�֪��������ʾ���������������ʾ��
    //driver����ʵ�ֱ����������ܼ򵥷���false��
    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_screen_ie)(struct tagRectangle *limit,
                             s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //screen�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬driver���Լ򻯣�ֱ��
    //����false���ɡ�
    //��frame buffer������£�������ʾgui�������������������������direct_screen
    //==true������ܵ��ñ�����������������false�Ļ������һ������set_pixel_screen
    //color�ĸ�ʽ��CN_SYS_PF_ERGB8888
    //Target: Ŀ���������,����ɫ���ʱ����
    //Focus: ʵ���������
    //Color0,Color1:Target���ϽǺ����½���ɫֵ,������ǽ������,��ֻ��ҪColor0
    //Mode: ���ģʽ,CN_FILLRECT_MODE_N�峣��
    bool_t (*fill_rect_screen)(struct tagRectangle *Target,
                               struct tagRectangle *Focus,
                               u32 Color0,u32 Color1,u32 Mode);
    //���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
    //��Ӳ�����ٲ�֧����screen�ϻ���λͼ��driver���Լ򻯣�ֱ��
    //����false���ɡ���frame buffer������£�__gk_output_redraw�л�����������
    //���������direct_screen==true��Ҳ���ܵ��ñ�����������������false��
    //����gui kernel���һ������set_pixel_screen����
    //��ʹӲ������֧�֣��������֧�־����src_bitmap->PixelFormat,Ҳ�ɷ���false
    //����������辵����ʾ̫����������ʾ����driver����ʵ�ֱ����������ܼ򵥷���false��
    bool_t (*bm_to_screen)(struct tagRectangle *dst_rect,
                           struct tagRectBitmap *src_bitmap,s32 xsrc,s32 ysrc);

    //��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
    u32 (*get_pixel_screen)(s32 x,s32 y);
    //��screen�ھ�����������ݸ��Ƶ�bitmap������ǰ�������ú�dest��pf_type
    bool_t (*get_rect_screen)(struct tagRectangle *rect,struct tagRectBitmap *dest);
};

struct tagDisplayRsc
{
    struct tagRscNode  node;  //ע:������ʾ����Ϊ����Դ����������
    float xmm,ymm;          //�ú��ױ�ʾ����ʾ���ߴ�
    s32   width,height;     //�����ر�ʾ����ʾ���ߴ�
    u32   pixel_format;     //���ظ�ʽ����gk_win.h�ж���
    bool_t reset_clip;      //true = ����ʾ����Ҫ�������ɿ�����
    //֡����ֱ����ʾ����Щcpuʹ��ϵͳ�ڴ����Դ棬���driverʱ���ɽ��Դ�ֱ����Ϊ
    //֡�����á�true=�Դ���֡�����غ�
    bool_t framebuf_direct;
    struct tagGkWinRsc  *frame_buffer;  //����ʾ����֡���壬�Դ��ڹ���
    struct tagGkWinRsc  *desktop;       //����ʾ��������
    struct tagGkWinRsc *z_topmost;      //z����ǰ�˴���ָ��

    //A�������ϵͳ���Ϸ��䴰�ڻ��壬DisplayHeap����ΪNULL��
    //B�����Ҫ��ר�õ��Դ�Heap�Ϸ��䴰�ڻ��棬�Կ�������ʹָ��DisplayHeapָ���
    //   Heap
    struct tagHeapCB *DisplayHeap;

    struct tagDispDraw draw;            //��ʾ������

    //������ʾ��
    //������driver�ṩ��һ��Ӧ�ó������ڣ���Ӧ�ó��������ṩһ�����ӻ��ķ�ʽ
    //�趨����ʾ�����п������û��趨�Ĳ���������ֱ��ʺͺ�ɫ�ʲ����������Ĺ���
    //����ͳһ�涨������������ĵ�Ӧ���ṩ������ʹ��˵����
    //���ñ������������ṩ����windows��������ʾ�����ԵĹ��ܡ�
    bool_t (*disp_ctrl)(struct tagDisplayRsc *disp);
};

bool_t GK_InstallDisplay(struct tagDisplayRsc *display,
                         const char *name);
bool_t GK_InstallDisplayMirror(struct tagDisplayRsc *base_display,
                                 struct tagDisplayRsc *mirror_display,char *name);
bool_t GK_SetDefaultDisplay(char *name);
struct tagGkWinRsc *GK_GetRootWin(struct tagDisplayRsc *display);
bool_t GK_SwitchFrameBuffer(struct tagDisplayRsc *display,
                              struct tagRectBitmap *fbuf);
struct tagRectBitmap *GK_CreateFrameBuffer(struct tagDisplayRsc *display);

#ifdef __cplusplus
}
#endif

#endif //__GK_DISPLAY_H__

