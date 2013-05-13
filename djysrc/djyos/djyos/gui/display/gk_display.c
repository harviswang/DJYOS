//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui core的显示器部分，不包含硬件相关部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include <string.h>

//----安装显示器---------------------------------------------------------------
//功能: 把一台新显示器登记到显示器资源队列中。
//参数: display，待安装的显示器，应该在driver中进行必要的初始化
//      frame_bitmap,帧缓冲，若为NULL，则表示该显示器没有帧缓冲，帧缓冲由一块能
//          直接以内存方式访问的存储器构成，比如有一些lcd模块，虽然模块内部有一
//          块内存用于帧缓冲，但其不能映射到系统内存，不能用在这里。
//      name，显示器名，所指向的字符串内存区不能是局部变量
//返回: true=成功初始化，false=失败
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
        if(frame_buffer->changed_msk.bm_bits== NULL)    //分配修改掩码内存失败
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
    rsc = rsc_search_tree("display");     //取显示器资源树
    rsc_add_son(rsc,&display->node,sizeof(struct display_rsc),name);
    return true;
}

//----安装镜像显示器-----------------------------------------------------------
//功能: 把一台显示器作为当前默认显示器的镜像显示器登记到显示器资源队列中。
//参数: display，待安装的显示器，应该在driver中进行必要的初始化
//      name，显示器名，所指向的字符串内存区不能是局部变量
//返回: true=成功初始化，false=失败
//-----------------------------------------------------------------------------
bool_t gk_install_display_mirror(struct display_rsc *base_display,
                                 struct display_rsc *mirror_display,char *name)
{
    rsc_add_son(&base_display->node,&mirror_display->node,
                            sizeof(struct display_rsc),name);
    return true;
}

//----设置默认显示器-----------------------------------------------------------
//功能: 设置默认显示器
//参数: name，显示器名
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_set_default_display(char *name)
{
    struct rsc_node *rsc;
    rsc = rsc_search_tree("display");     //取显示器资源树
    rsc = rsc_search_son(rsc,name);     //找到被操作的显示器资源
    if(rsc == NULL)
    {
        return false;
    }else
    {
        rsc_moveto_head(rsc);          //资源树中的队列头结点就是默认显示器
        return true;
    }
}

//----取显示器的默认显示设置---------------------------------------------------
//功能: 取一个显示器的默认显示设置，实际上就是桌面窗口的资源节点
//参数: display，显示器指针
//返回: draw_set指针
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_get_root_win(struct display_rsc *display)
{
    return display->desktop;
}

//----切换frame buffer---------------------------------------------------------
//功能: 当一个显示器有多个frame buffer时，用本函数切换当前frame buffer，在系统
//      中，frame buffer是一种资源
//参数: display，被操作的显示器
//      fbuf，缓冲区指针
//返回: true=成功，false=失败，失败的原因只有一个:该显示驱动不是帧缓冲型
//-----------------------------------------------------------------------------
bool_t gk_switch_frame_buffer(struct display_rsc *display,
                              struct st_rect_bitmap *fbuf)
{
    return true;
}

//----创建frame buffer---------------------------------------------------------
//功能: 为某显示器创建一个frame buffer
//参数: display，被操作的显示器
//返回: 新frame buffer 指针，NULL=创建失败
//-----------------------------------------------------------------------------
struct st_rect_bitmap *gk_create_frame_buffer(struct display_rsc *display)
{
    return NULL;
}

#endif //   for #if (cfg_gui_kernel == 1)

