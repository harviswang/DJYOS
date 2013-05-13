//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
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
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
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
bool_t gk_install_display(struct display_rsc *display,
//                        struct st_rect_bitmap *frame_bitmap,
                          char *name)
{
    struct rsc_node *rsc;
    struct st_rect_bitmap *frame_bitmap;
    struct gkwin_rsc *frame_buffer;
    u32 msk_size;
    if(display == NULL)
        return false;
    display->desktop = NULL;
    display->reset_clip = false;
    frame_buffer = display->frame_buffer;
    if(frame_buffer != NULL)
    {
        frame_bitmap = &frame_buffer->wm_bitmap;

        frame_buffer->changed_msk.pf_type = cn_sys_pf_gray1;
        frame_buffer->changed_msk.height = frame_buffer->wm_bitmap.height/8;
        frame_buffer->changed_msk.width = frame_buffer->wm_bitmap.width/8;
        frame_buffer->changed_msk.linebytes = frame_buffer->changed_msk.width/8;
        msk_size = frame_buffer->changed_msk.linebytes * 
                                frame_buffer->changed_msk.height;
        frame_buffer->changed_msk.bm_bits = m_malloc_gbl(msk_size,0);
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
        frame_buffer->transparentcolor = 0;
        frame_buffer->rop_code = cn_r3_srccopy;
        frame_buffer->dest_blend = false;
        frame_buffer->bitmsk.bm_bits = NULL;
        frame_buffer->pat_buf = NULL;
        frame_buffer->change_flag = cn_gkwin_change_none;
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
        frame_buffer->pen_color = 0;
    }
    else
        display->frame_buffer = NULL;
    rsc = rsc_search_tree("display");     //ȡ��ʾ����Դ��
    rsc_add_son(rsc,&display->node,sizeof(struct display_rsc),name);
    return true;
}

//----��װ������ʾ��-----------------------------------------------------------
//����: ��һ̨��ʾ����Ϊ��ǰĬ����ʾ���ľ�����ʾ���Ǽǵ���ʾ����Դ�����С�
//����: display������װ����ʾ����Ӧ����driver�н��б�Ҫ�ĳ�ʼ��
//      name����ʾ��������ָ����ַ����ڴ��������Ǿֲ�����
//����: true=�ɹ���ʼ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_install_display_mirror(struct display_rsc *base_display,
                                 struct display_rsc *mirror_display,char *name)
{
    rsc_add_son(&base_display->node,&mirror_display->node,
                            sizeof(struct display_rsc),name);
    return true;
}

//----����Ĭ����ʾ��-----------------------------------------------------------
//����: ����Ĭ����ʾ��
//����: name����ʾ����
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_set_default_display(char *name)
{
    struct rsc_node *rsc;
    rsc = rsc_search_tree("display");     //ȡ��ʾ����Դ��
    rsc = rsc_search_son(rsc,name);     //�ҵ�����������ʾ����Դ
    if(rsc == NULL)
    {
        return false;
    }else
    {
        rsc_moveto_head(rsc);          //��Դ���еĶ���ͷ������Ĭ����ʾ��
        return true;
    }
}

//----ȡ��ʾ����Ĭ����ʾ����---------------------------------------------------
//����: ȡһ����ʾ����Ĭ����ʾ���ã�ʵ���Ͼ������洰�ڵ���Դ�ڵ�
//����: display����ʾ��ָ��
//����: draw_setָ��
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_get_root_win(struct display_rsc *display)
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
bool_t gk_switch_frame_buffer(struct display_rsc *display,
                              struct st_rect_bitmap *fbuf)
{
    return true;
}

//----����frame buffer---------------------------------------------------------
//����: Ϊĳ��ʾ������һ��frame buffer
//����: display������������ʾ��
//����: ��frame buffer ָ�룬NULL=����ʧ��
//-----------------------------------------------------------------------------
struct st_rect_bitmap *gk_create_frame_buffer(struct display_rsc *display)
{
    return NULL;
}

#endif //   for #if (cfg_gui_kernel == 1)

