//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel主体部分
//其他说明:
//修订历史:
//2. 日期: 2011-07-18
//   作者:  罗侍田.
//   新版本号: V1.1.0
//   修改说明: 窗口数据结构实体已经改为由上层提供，但本文件一直没改，修改之。
//             增加__gk_vmalloc等4个函数，使在显存分配无特殊要求时，driver可以不
//             提供分配内存的函数。
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include "gk_api.h"
#include "gk_clip.h"
#include <math.h>
#include <string.h>

struct gkwin_rsc   *pg_focus_win;//显示焦点窗口,所有的显示都是对这个窗口的操作
static struct rsc_node tg_display_root_rsc;
static struct rsc_node tg_window_root_rsc;
u32 u32g_default_back_color,u32g_default_color;
u16 u16g_gk_server_evtt,u16g_refresh_evtt;
struct gk_chunnel tg_gk_chunnel;

extern struct mem_cell_pool *pg_clip_rect_pool; //from gk_clip.c

//剪切域数量
#define cn_clip_init_num    100
struct clip_rect tg_clip_rect[cn_clip_init_num];
struct gkwin_rsc *pg_z_topmost;

struct semaphore_LCB *pg_usercall_semp;
//如果调用方希望gui kernel服务完成再返回，使用这个信号量
struct semaphore_LCB *pg_syscall_semp;
static struct mutex_LCB *pg_gk_server_sync;

//gkwin在资源树种的位置与z序的联系:
// 1、在资源树中，所有子窗口按z序排列，最前端的是父窗口指向的第一个结点。
// 2、z_prio成员影响win在其兄弟结点中的位置，越小越前端
// 3、z_prio一般设为0，<=0均遮盖父窗口，若>0则被父窗口遮盖


//----初始化gui模块---------------------------------------------------------
//功能: 初始化gui模块，在资源队列中增加必要的资源结点
//参数: 模块初始化函数，没有参数
//返回: 1=成功
//-----------------------------------------------------------------------------
ptu32_t module_init_gk(ptu32_t para)
{
    rsc_add_tree_root(&tg_display_root_rsc,sizeof(struct rsc_node),"display");
    rsc_add_tree_root(&tg_window_root_rsc,sizeof(struct rsc_node),"gkwindow");

    module_init_char_encoding(0);
    module_init_font(0);
    
    ring_init(&tg_gk_chunnel.ring_syscall,
                        tg_gk_chunnel.syscall_buf,cfg_gk_down_chunnel_deep);
    ring_init(&tg_gk_chunnel.ring_usercall,
                        tg_gk_chunnel.usercall_buf,cfg_gk_up_chunnel_deep);
     
    tg_gk_chunnel.syscall_mutex = mutex_create("gui chunnel to gk mutex");
    tg_gk_chunnel.syscall_semp = semp_create(1,0,"gui chunnel to gk semp");
    tg_gk_chunnel.usercall_semp = semp_create(1,0,"gui chunnel from gk semp");
    pg_gk_server_sync = mutex_create("gk server sync");
//    pg_gk_server_sync = semp_create(1,0,"gk server sync");
    pg_usercall_semp = semp_create(1,0,"gk wait repaint");
    pg_syscall_semp = semp_create(1,0,"gk wait job");
    u16g_gk_server_evtt = djy_evtt_regist(enum_correlative,1,0,10,gk_server,
                                    8120,"gui kernel server");
    pg_clip_rect_pool = mb_create_pool(&tg_clip_rect,
                                  cn_clip_init_num,
                                  sizeof(struct clip_rect),
                                  100,2000, "clip area");

    if(    (pg_clip_rect_pool == NULL)
        || (u16g_gk_server_evtt == cn_invalid_evtt_id)
        || (pg_syscall_semp == NULL)
        || (pg_usercall_semp == NULL)
        || (pg_gk_server_sync == NULL)
        || (tg_gk_chunnel.usercall_semp == NULL)
        || (tg_gk_chunnel.syscall_semp == NULL)
        || (tg_gk_chunnel.syscall_mutex == NULL) )
    {
        goto exit_error;
    }


    semp_set_sync_sort(pg_syscall_semp,sort_time);
    djy_event_pop(u16g_gk_server_evtt,enum_para_async,NULL,0,NULL,0,0,0);


    return 1;

exit_error:
    mb_delete_pool(pg_clip_rect_pool);
    djy_evtt_unregist(u16g_gk_server_evtt);
    semp_delete(pg_syscall_semp);
    semp_delete(pg_usercall_semp);
//    semp_delete(pg_gk_server_sync);
    mutex_delete(pg_gk_server_sync);
    semp_delete(tg_gk_chunnel.usercall_semp);
    semp_delete(tg_gk_chunnel.syscall_semp);
    mutex_delete(tg_gk_chunnel.syscall_mutex);
    
    rsc_del_node(&tg_display_root_rsc);
    rsc_del_node(&tg_window_root_rsc);
    return 0;
}

u32 __gk_cal_buf_size(u32 xsize,u32 ysize,u32 *linesize,u16 pf_type)
{
    u32 size;
    u8 color_bits;
    color_bits = pf_type >>8;
    switch(color_bits)
    {
        case 1:
        {
            *linesize = xsize/8;
            if(xsize%8)
                (*linesize)++;
            size = *linesize*ysize;
        }break;
        case 2:
        {
            *linesize = xsize/4;
            if(xsize%4)
                (*linesize)++;
            size = *linesize*ysize;
        }break;
        case 4:
        {
            *linesize = xsize/2;
            if(xsize%2)
                (*linesize)++;
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
//----分配窗口缓存-------------------------------------------------------------
//功能: 分配窗口缓存，包括：1、窗口所需的bitmap,2、如果该窗口有rop4属性，还要分
//      配bitmsk_buf，3、pattern buffer。
//      虽然本函数可能需要分配三部分，但实际上是合并一起从堆中申请，然后再手工
//      分割成几个缓冲区的。因各缓冲区都是byte型指针，故无须担心对齐问题。
//参数: buf，用于返回结果的结构指针
//      xsize、ysize，显示区域尺寸
//      rop_code，窗口的rop_code代码，影响是否分配buf中的rop4_msk成员。
//返回: true=成功，false=失败
//注: 2416的2D加速不支持rop4，无须判断rop_code，只须分配pat和vbuf即可
//-----------------------------------------------------------------------------
bool_t __gk_vmalloc(struct display_rsc *disp,struct bmbuf_info *buf,
                    u32 xsize,u32 ysize,u32 rop_code)
{
    u8 *mem;
    u32 buf_size,linesize,pat_size,rop4_size;
    bool_t result;
    //有些显卡对分配窗口缓存有特殊要求，则调用显卡提供的vmalloc函数，编写显卡驱
    //动程序可参考本函数的实现方法。
    //导致显卡对缓存有特殊要求的原因，包括(不限于):
    //	1、有加速引擎的显卡，缓存可能要参与显卡的图形加速计算，加速引擎对缓存的地
    //   址、对齐边界可能会有所限制。
    // 2、有些系统专门划定了缓存的区域，分配缓存必须在该区域内。
    //若无特殊要求，则在driver中把vmalloc设为NULL，将由本函数实现显存分配
    if(disp->vmalloc != NULL)
    {
        result = disp->vmalloc(buf,xsize,ysize,rop_code);
    }else
    {
        buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,buf->vbuf.pf_type);
        buf_size = align_up_sys(buf_size);
        //rop_code中包含4元光栅操作，须分配rop4_msk
        if(( (rop_code & (cn_rop_ext_en + cn_rop_rop4_en)) 
                        ==(cn_rop_ext_en + cn_rop_rop4_en)) )
        {
            rop4_size = (xsize+7)/8 * ysize;
            rop4_size = align_up_sys(rop4_size);
        }else
            rop4_size = 0;
        pat_size = disp->patx * (disp->pixel_format >>8)>>3; //计算1行占多少字节
        pat_size = pat_size * disp->paty;
        mem = m_malloc(buf_size + rop4_size + pat_size,0);
        if(mem != NULL)
        {
            buf->vbuf.bm_bits = mem;
            buf->vbuf.width = xsize;
            buf->vbuf.height = ysize;
            buf->vbuf.linebytes = linesize;
            
            buf->rop4_msk.bm_bits = mem + buf_size;
            buf->rop4_msk.width = xsize;
            buf->rop4_msk.height = ysize;
            buf->rop4_msk.linebytes = (xsize+7)/8;
            
            buf->pat = mem + buf_size + rop4_size;
            result = true;
        }else
        {
            buf->vbuf.bm_bits = NULL;
            buf->rop4_msk.bm_bits = NULL;
            buf->pat = NULL;
            buf->vbuf.width = 0;
            buf->vbuf.height = 0;
            buf->vbuf.linebytes = 0;
            result = false;
        }
    }
    return result;
}

//----重新分配窗口缓存---------------------------------------------------------
//功能: 当窗口尺寸改变，调用本函数重新分配缓存。逻辑上，本函数的功能="vfree-
//      重新vmalloc"的组合，新设置一个函数，是为了提高效率。因为内存是按块分配
//      的，内存分配是相当耗时的工作，vmalloc分配的窗口缓存，可能比实际的大，当
//      窗口尺寸调整不大的话，可能无需重新分配。而用户通过拖动来改变窗口尺寸时，
//      实际上是一点一点改变的，每次都重新分配的话，系统开销很大，且易产生碎片。
//      vrmalloc函数先检查buf中的缓冲区是否仍然满足需求，若满足，则不重新分配。
//      但可能会跟据xsize和ysize调整指针。
//参数: buf，原窗口缓存指针，也用于返回结果
//      xsize、ysize，显示区域尺寸
//      rop_code，窗口的rop_code代码
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __gk_vrmalloc(struct display_rsc *disp,struct bmbuf_info *buf,
                    u32 xsize,u32 ysize,u32 rop_code)
{
    u8 *mem;
    u32 buf_size,linesize,pat_size,oldsize,newsize,rop4_size;
    bool_t remalloc = true,result;
    //这里请参照lcd_vmalloc函数相应部分的注释
    if(disp->vrmalloc != NULL)
    {
        result = disp->vrmalloc(buf,xsize,ysize,rop_code);
    }else
    {
        buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,buf->vbuf.pf_type);
        buf_size = align_up_sys(buf_size);
        //rop_code中包含4元光栅操作，须分配rop4_msk
        if( ( (rop_code & (cn_rop_ext_en + cn_rop_rop4_en)) 
                        ==(cn_rop_ext_en + cn_rop_rop4_en)) )
        {
            rop4_size = (xsize+7)/8 * ysize;
            rop4_size = align_up_sys(rop4_size);
        }else
            rop4_size = 0;
        pat_size = disp->patx * (disp->pixel_format >>8)>>3; //计算1行占多少字节
        pat_size = pat_size * disp->paty;
        newsize = buf_size + rop4_size + pat_size;

        oldsize = m_check_size(buf->vbuf.bm_bits); //查询原指针实际分配的内存
        if(newsize <= oldsize)
        {
            //新的显存需求在原实际尺寸相差在1/8以内的或者小于最小内存分配尺寸的，
            //不重新分配显存
            if(((oldsize - newsize) <= (oldsize>>3)) 
                    || ((oldsize - newsize) <= m_get_page_size() ))
                remalloc = false;       //无需重新分配
        }
        if(remalloc)
        {
            mem = m_malloc(newsize,0);
            if(mem != NULL)
            {
                m_free(buf->vbuf.bm_bits);
                buf->vbuf.bm_bits = mem;
                buf->vbuf.width = xsize;
                buf->vbuf.height = ysize;
                buf->vbuf.linebytes = linesize;
                
                buf->rop4_msk.bm_bits = mem + buf_size;
                buf->rop4_msk.width = xsize;
                buf->rop4_msk.height = ysize;
                buf->rop4_msk.linebytes = (xsize+7)/8;
                
                buf->pat = mem + buf_size + rop4_size;
                result = true;
            }else
            {
                result = false;   //新分配内存失败，保持原来不变，不释放原内存。
            }
        }else       //无须重新分配
        {
            buf->vbuf.width = xsize;
            buf->vbuf.height = ysize;
            buf->vbuf.linebytes = linesize;
            
            buf->rop4_msk.bm_bits = buf->vbuf.bm_bits + buf_size;
            buf->rop4_msk.width = xsize;
            buf->rop4_msk.height = ysize;
            buf->rop4_msk.linebytes = (xsize+7)/8;
            
            buf->pat = buf->vbuf.bm_bits + buf_size + rop4_size;
            result = true;
        }
    }
    return result;
}

//----释放窗口缓存-------------------------------------------------------------
//功能: 释放一个窗口的缓存。
//参数: buf，用于返回结果的结构指针
//返回: 无
//-----------------------------------------------------------------------------
void __gk_vfree(struct display_rsc *disp,struct bmbuf_info *buf)
{
    if(disp->vfree != NULL)
    {
        disp->vfree(buf);
    }else
    {
        m_free(buf->vbuf.bm_bits);  //分配内存时，显存和rop4_msk和pattern是同时
                                    //分配的，故只须调用m_free一次，三者均释放
    }
}
//----取z轴片段的起始窗口(窗口及其子窗口)--------------------------------------
//功能: 每一个窗口及其子窗口，都在z轴中占据连续的一段，本函数取该段的起始窗口(
//      最后端的窗口)。
//参数: gkwin，窗口指针
//返回: 起始窗口指针
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_start(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result已经没有子窗口了
            break;
        temp = (struct gkwin_rsc *)temp->node.previous;
        if(temp ->z_prio <= 0 )  //所有子窗口均在result前端
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}
//----取z轴片段的结束窗口(窗口及其子窗口)--------------------------------------
//功能: 每一个窗口及其子窗口，都在z轴中占据连续的一段，本函数取该段的结束窗口(
//      最前端的窗口)。
//参数: gkwin，窗口指针
//返回: 结束窗口指针
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_end(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result已经没有子窗口了
            break;
        else if(temp ->z_prio > 0 )  //所有子窗口均在result后端
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}
//----取z轴片段的起始窗口(窗口及其子孙窗口)------------------------------------
//功能: 每一个窗口及其子孙窗口，都在z轴中占据连续的一段，本函数取该段的起始窗口(
//      最后端的窗口)。
//参数: gkwin，窗口指针
//返回: 起始窗口指针
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_all_start(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result已经没有子窗口了
            break;
        temp = (struct gkwin_rsc *)rsc_get_next(&temp->node);
        if(temp ->z_prio <= 0 )  //所有子窗口均在result前端
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}

//----取z轴片段的结束窗口(窗口及其子窗口)--------------------------------------
//功能: 每一个窗口及其子孙窗口，都在z轴中占据连续的一段，本函数取该段的结束窗口(
//      最前端的窗口)。
//参数: gkwin，窗口指针
//返回: 结束窗口指针
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_all_end(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result已经没有子窗口了
            break;
        else if(temp ->z_prio > 0 )  //所有子窗口均在result后端
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}


//----创建桌面-----------------------------------------------------------------
//功能: 桌面是所有显示器的第一个窗口；所有的显示器，必须先创建桌面才能创建窗口，
//      桌面的尺寸不能小于显示器的尺寸，但可以大于显示器尺寸。桌面在刷新screen
//      的过程中和普通窗口是等同的。
//参数: para，参数数据结构，其成员含义见结构定义处。
//返回: 新窗口句柄(指针)
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_create_desktop(struct gksc_para_create_desktop *para)
{
    struct gkwin_rsc *desktop;
    struct display_rsc *display;
    struct rsc_node *rsc;
    struct clip_rect *clip;
    u32 msk_size;
    s32 desktop_x,desktop_y;
    struct bmbuf_info buf_info;
    struct gksc_para_fill_win para_fill;
    if(para->display == NULL)
    {
        return NULL;
    }
    display = para->display;
    if(para->width >= display->width)
        desktop_x = para->width;            //桌面可以大于显示器尺寸
    else
        desktop_x = display->width;   //桌面不能小于显示器尺寸
    if(para->height >= display->height)
        desktop_y = para->height;
    else
        desktop_y = display->height;
    if((display->frame_buffer == NULL) && (para->buf_mode == true))
    {
        return NULL;        //无帧缓冲的显示器，不允许创建缓冲桌面
    }
    desktop = para->desktop;    //para->desktop由调用者提供内存，传指针过来
    if(para->buf_mode)      //缓冲模式，为每个窗口准备缓冲区
    {
        buf_info.vbuf.pf_type = display->pixel_format;
        if( __gk_vmalloc(display,&buf_info,
                        desktop_x,desktop_y,cn_r3_srccopy)) //分配显存
        {
            msk_size = ((desktop_x +7)/8+7)/8*((desktop_y +7)/8);
            //存在frame buffer时才需要changed_msk
            desktop->changed_msk.bm_bits = m_malloc_gbl(msk_size,0);
            if(desktop->changed_msk.bm_bits == NULL)   //分配修改掩码内存失败
            {
                __gk_vfree(display,&buf_info);
                djy_error_login(enum_gk_no_memory,"显存不足");
                return NULL;
            }else
            {///分配修改掩码内存成功，将显存再分配
                memset(desktop->changed_msk.bm_bits,0,msk_size);
                desktop->changed_msk.pf_type = cn_sys_pf_gray1;
                desktop->changed_msk.height = desktop_y/8;
                desktop->changed_msk.width = desktop_x/8;
                desktop->changed_msk.linebytes = ((desktop_x+7)/8+7)/8;
                desktop->wm_bitmap = buf_info.vbuf;
                desktop->bitmsk = buf_info.rop4_msk;
                desktop->pat_buf = buf_info.pat;
            }
        }
        else               //分配显存失败，
        {
            djy_error_login(enum_gk_no_memory,"显存不足");
            return NULL;
        }
    }
    else                   //非缓冲模式，窗口没有缓冲区
    {
        desktop->wm_bitmap.bm_bits = NULL;
        desktop->wm_bitmap.height = desktop_y;
        desktop->wm_bitmap.width = desktop_x;
        desktop->pat_buf = NULL;
    }
    //gui kernel是单线程运行的，无需担心mb_query_free后mb_malloc之前会被其他线程
    //抢占。
    if(mb_query_free(pg_clip_rect_pool) < 2)
    {
        __gk_vfree(display,&buf_info);
        return NULL;
    }
    else
    {
        //桌面可视域，可视域大小与显示器尺寸大小一致
        clip = (struct clip_rect*)mb_malloc(pg_clip_rect_pool,0);
        clip->next = clip;
        clip->previous = clip;
        clip->rect.left = 0;
        clip->rect.top = 0;
        clip->rect.right = display->width;
        clip->rect.bottom = display->height;
        desktop->visible_clip = clip;
        desktop->visible_bak = NULL;
    }
    //下面初始化桌面，桌面也是按窗口管理的
    desktop->z_back = desktop;
    desktop->z_top = desktop;
    strcpy(desktop->win_name,"desktop");
    desktop->redraw_clip = NULL;
    desktop->changed_clip = NULL;
    desktop->disp = display;        //所属显示器
//    desktop->locked = false;            //桌面不锁定
//    desktop->hided = false;            //桌面不隐藏
    desktop->z_prio = 0;                //桌面的优先级
    desktop->transparentcolor = 0;        //本参数对桌面无效
    desktop->rop_code = cn_r3_srccopy;
    desktop->dest_blend = false;        //本参数对桌面无效
//    desktop->bitmsk.bm_bits = NULL;
//    desktop->pat_buf = NULL;
    desktop->change_flag = cn_gkwin_change_none; //清屏后才显示
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
    rsc = rsc_search_tree("gkwindow");
    rsc_add_son(rsc,&desktop->node,sizeof(struct gkwin_rsc),desktop->win_name);

    //用给定的颜色填充桌面
    para_fill.gkwin = desktop;
    para_fill.color = para->color;
    gk_fill_win(&para_fill);
    
    //返回新窗口指针
    return desktop;
}

//----创建窗口-----------------------------------------------------------------
//功能: 创建一个窗口，新窗口的优先级为0，位于所有优先级=0的窗口的最前端。
//参数: para，参数数据结构，其成员含义见结构定义处。
//返回: 新窗口句柄(指针)
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_create_win(struct gksc_para_create_gkwin *para)
{
    u32 size;
    s32 width,height;
    struct gkwin_rsc *target_section;  
    struct gkwin_rsc *parent;                       //gkwin的父窗口
    struct display_rsc *display;
    //z轴中被移动的窗口段最后端的窗口
    struct gkwin_rsc *move_start;
    //z轴中被移动的窗口段最前端的窗口
    struct gkwin_rsc *move_end;      
    struct gkwin_rsc *gkwin;
    struct bmbuf_info buf_info;
    struct gksc_para_fill_win para_fill;
    
    width = para->right - para->left;
    height = para->bottom - para->top;
    display = para->parent_gkwin->disp;
    gkwin = para->gkwin;    //para->gkwin由调用者提供内存，传指针过来
    parent = para->parent_gkwin; 
    if((parent == NULL)||(width < 0) || (height < 0))
    {
        return NULL;
    }
    if((display->frame_buffer == NULL) && (para->buf_mode == true))
    {
        return NULL;        //无帧缓冲的显示器，不允许创建缓冲窗口
    }
    if(mb_query_free(pg_clip_rect_pool) < 2)
    {
        return NULL;
    }
    
    *gkwin = *parent;             //新窗口拷贝父窗口的属性
    if(para->buf_mode)      //缓冲模式，为窗口准备缓冲区
    {
        buf_info.vbuf.pf_type = display->pixel_format;
        if( __gk_vmalloc(display,&buf_info,
                        para->right-para->left,
                        para->bottom-para->top,para->rop_code)) //分配显存
        {
            size = (((width +7)/8+7)/8 *((height +7)/8));
            //存在frame buffer时才需要changed_msk
            gkwin->changed_msk.bm_bits = m_malloc_gbl(size,0);
            if(gkwin->changed_msk.bm_bits == NULL)   //分配修改掩码内存失败
            {
                __gk_vfree(display,&buf_info);
                djy_error_login(enum_gk_no_memory,"显存不足");
                return NULL;
            }
            else
            {
                memset(gkwin->changed_msk.bm_bits,0,size);
                gkwin->changed_msk.pf_type = cn_sys_pf_gray1;
                gkwin->changed_msk.height = height/8;
                gkwin->changed_msk.width = width/8;
                gkwin->changed_msk.linebytes = ((width+7)/8+7)/8;
                gkwin->wm_bitmap = buf_info.vbuf;
                gkwin->bitmsk = buf_info.rop4_msk;
                gkwin->pat_buf = buf_info.pat;
            }
        }
        else               //分配显存失败，
        {
            djy_error_login(enum_gk_no_memory,"显存不足");
            return NULL;
        }
    }
    else                   //非缓冲模式，窗口没有缓冲区
    {
        gkwin->wm_bitmap.bm_bits = NULL;
        gkwin->wm_bitmap.height = height;
        gkwin->wm_bitmap.width =width;
        gkwin->pat_buf = NULL;
    }

    size = strnlen(para->name,cn_gkwin_name_limit);
    if(size == cn_limit_uint32)         //名字长度超过限制
    {
        size = cn_gkwin_name_limit;     //强制切掉超长部分
    }
    memcpy(gkwin->win_name,para->name,size);
    gkwin->win_name[size] = '\0';       //串结束符\0。

    //给定的新窗口的坐标是相对其父窗口的
    gkwin->left = para->left;
    gkwin->top = para->top;
    gkwin->right = para->right;
    gkwin->bottom = para->bottom;
    //窗口的绝对位置，相对于所属screen的桌面原点
    gkwin->absx0 = para->left + para->parent_gkwin->absx0;
    gkwin->absy0 = para->top + para->parent_gkwin->absy0;
    gkwin->z_prio = 0;                //新建窗口默认为0
    display->reset_clip = false;
    
    gkwin->dest_blend = gk_rop_needsno_destination(para->rop_code);
    gkwin->rop_code = para->rop_code;
    //以下把新窗口连接到资源队列中，并插入到z轴中
    if(rsc_get_son(&para->parent_gkwin->node) == NULL)  //父窗口无子窗口
    {
        rsc_add_son(&parent->node,&gkwin->node,
                     sizeof(struct gkwin_rsc),gkwin->win_name);
        //以下4句在z轴中把win插入父节点的前端
        gkwin->z_back = parent;
        gkwin->z_top = parent->z_top;
        parent->z_top->z_back = gkwin;
        parent->z_top = gkwin;
        if(display->z_topmost == parent)
            display->z_topmost = gkwin;
    }
    else
    {//父窗口有子窗口
        //取z轴中被移动的与gkwin同级的窗口段开始和结束窗口段
        move_end = (struct gkwin_rsc*)rsc_get_son(&parent->node);
        move_start = (struct gkwin_rsc*)rsc_get_previous(&move_end->node);
        target_section = move_end;
        while(1)
        {//扫描同级窗口中和目标窗口优先级相同的窗口，扫描以z轴为对象   
            //扫描顺序:从z轴段前端向后端进行扫描，只扫兄弟窗口，不扫子窗口
            if(target_section->z_prio >= 0)
                break;
            //扫描目标窗口所在z轴段最后一窗口，终止跳出
            if(target_section == move_start)
                break;
            target_section = (struct gkwin_rsc*)rsc_get_next(&target_section->node);
        }
        if(target_section->z_prio == 0)
        {//同级窗口中存在和gkwin的prio相等的窗口
            rsc_add_node(&target_section->node,&gkwin->node,
                        sizeof(struct gkwin_rsc),gkwin->win_name);
            if(rsc_is_head(&target_section->node))
                rsc_round_next(&parent->node);
            //获取target_section和其子窗口所在z轴段的结束窗口(最前端)
            //新窗口插入到它的前端
            move_end = __gk_get_zsection_all_end(target_section);
            if(move_end == display->z_topmost)
                display->z_topmost = gkwin;
            //插到目标窗口前端
            gkwin->z_back = move_end;
            gkwin->z_top = move_end->z_top;
            move_end->z_top->z_back = gkwin;
            move_end->z_top = gkwin;        
        }
        else //兄弟窗口中没有优先级=0的窗口
        {   
            //同级窗口中有优先级均大于0的，资源队列中新窗口插入其前端
            if(target_section->z_prio > 0)
            {
                rsc_add_node(&target_section->node,&gkwin->node,
                            sizeof(struct gkwin_rsc),gkwin->win_name);
            }
            else    //同级窗口优先级均高于新窗口(prio = 0)
            {
                //新窗口在资源队列中处于同级窗口最后端
                rsc_insert_node(&target_section->node,&gkwin->node,
                            sizeof(struct gkwin_rsc),gkwin->win_name);
            }
            //新创建的窗口gkwin优先级为0，无论同级窗口优先级均如何分布，新
            //窗口肯定在父窗口前端
            if(parent == display->z_topmost)
                display->z_topmost = para->gkwin;
            //插到目标窗口前端
            gkwin->z_back = parent;
            gkwin->z_top = parent->z_top;
            parent->z_top->z_back = gkwin;
            parent->z_top = gkwin;        
        }
    }
    //设置窗口的可见边界，不受窗口互相遮挡影响，必须设置资源队列后才能调用
    __gk_set_bound(gkwin);

    gkwin->visible_clip = NULL;    
    gkwin->visible_bak = NULL; 
	__gk_scan_new_visible_clip(display);

    para_fill.gkwin = gkwin;
    para_fill.color = para->color;
    gk_fill_win(&para_fill);
    return gkwin;
}

//----窗口改名-----------------------------------------------------------------
//功能: 修改窗口名字，窗口最多127字符，超过部分将截断。
//参数: gcwin，被修改的窗口句柄(指针)
//      name，新的名字
//返回: 无
//-----------------------------------------------------------------------------
void gk_set_name(struct gkwin_rsc *gkwin,char *name)
{
    s32 size;
    size = strnlen(name,cn_gkwin_name_limit);
    if(size == cn_limit_uint32)     //名字长度超过限制
        size = cn_gkwin_name_limit;   //强制切掉超长部分
    memcpy(gkwin->win_name,name,size);//copy名字，因可能超长，故未copy串结束符\0。
    gkwin->win_name[size] = '\0';     //串封口(加结束符)
}

//----解锁窗口-----------------------------------------------------------------
//功能: 解除锁定窗口，需要重新输出本窗口修改过的部分。锁定状态下，窗口显示在屏幕
//      上，但不内容不会改变。
//参数: gcwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
//void gk_set_unlock(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //桌面不可锁定
//        return;
//    if(gkwin->locked == false)      //本来就没有锁定，无操作
//        return;
//    gkwin->locked = false;
//    if(gkwin->hided == false)       //若窗口可视，则要重新扫描可视域
//    {
//        gkwin->disp->reset_clip = true;
//    }
//    return;
//}

//----锁定窗口-----------------------------------------------------------------
//功能: 锁定窗口，需要重新输出本窗口修改过的部分
//参数: gcwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
//void gk_set_lock(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //桌面不可锁定
//        return;
//    if(gkwin->locked == true)       //本来就已锁定，无操作
//        return;
//    gkwin->locked = true;
//}

//----隐藏窗体-----------------------------------------------------------------
//功能: 把窗口设置为隐藏，会引起重新扫描可视域，重新输出其他窗口被自己遮盖部分。
//      不影响子窗口，若要同时设置子窗口，应在gui windows中完成
//参数: gcwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
//void gk_set_hide(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //桌面不可隐藏
//        return;
//    if(gkwin->hided == true)                    //原来就是隐藏的，无操作
//        return;
//    gkwin->hided = true;
//    gkwin->disp->reset_clip = true;
//}

//----显示窗口-----------------------------------------------------------------
//功能: 把窗口设置为可视，会引起重新扫描可视域，重新输出本窗口。不影响子窗口，
//      若要同时设置子窗口，应在gui windows中完成
//参数: gcwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
//void gk_set_show(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //桌面不可隐藏
//        return;
//    if(gkwin->hided == false)   //原来就是可视的，无操作
//        return;
//    gkwin->hided = false;
//    gkwin->disp->reset_clip = true;
// }
//----移动窗口-----------------------------------------------------------------
//功能: 改变窗口在父窗口内的相对位置，由于子窗口的坐标是相对于父窗口的，故移动窗
//      口时，连子窗口一起移动。
//参数: gcwin，目标窗口
//      left、top，新的左上角坐标，相对于父窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_move_win(struct gksc_para_move_win *para)
{
    u32 width,height;
    struct gkwin_rsc *current;
    struct display_rsc *disp;
    if((para->left == para->gkwin->left) && (para->top == para->gkwin->top))
        return;                                 //位置没有修改，直接返回。
    disp = para->gkwin->disp;
    if(para->gkwin == disp->desktop)
    {   //目标窗口是桌面，有额外限制:桌面必须覆盖显示器区域
        s32 dtop,dleft;
        if(para->left > 0)
            dleft = 0;    //桌面左边界必须覆盖显示器
        else if((para->gkwin->wm_bitmap.width+para->left) < disp->width) 
            //右边界必须覆盖显示器   
            dleft = disp->width - para->gkwin->wm_bitmap.width;
        else
            dleft = para->left;
        if(para->top > 0)
            dtop = 0;     //桌面上边界必须覆盖显示器
        else if((para->gkwin->wm_bitmap.height+para->top) < disp->height)
            //下边界必须覆盖显示器
            dtop =disp->height - para->gkwin->wm_bitmap.height;
        else
            dtop = para->top;
        if((dtop == para->gkwin->top) && (dleft == para->gkwin->left))
            return;         //桌面位置实际上未发生改变
        else
        {
            para->gkwin->top = dtop;
            para->gkwin->left = dleft;
         }
        para->gkwin->limit_left = 0-para->left;
        para->gkwin->limit_top = 0-para->top;
        para->gkwin->limit_right = para->gkwin->limit_left + disp->width;
        para->gkwin->limit_bottom = para->gkwin->limit_top + disp->height;
    }else
    {
        para->gkwin->absx0 -= para->gkwin->left-para->left;//修改窗口绝对坐标
        para->gkwin->absy0 -= para->gkwin->top-para->top;
        width = para->gkwin->right-para->gkwin->left;
        height = para->gkwin->bottom-para->gkwin->top;
        para->gkwin->left = para->left;                      //修改窗口相对坐标
        para->gkwin->top = para->top;
        para->gkwin->right = para->gkwin->left+width;
        para->gkwin->bottom = para->gkwin->top+height;
        __gk_set_bound(para->gkwin);                        //设置可显示边界
    }
    current = para->gkwin;
    current->disp->reset_clip = true;
    current->change_flag = cn_gkwin_change_all;
    //遍历gkwin的所有子窗口
    while(rsc_trave_scion(&para->gkwin->node,&current->node)) 
    {
        current->absx0 -= current->left -para->left;        //修改窗口绝对坐标
        current->absy0 -= current->top -para->top;
        __gk_set_bound(current);
        para->gkwin->change_flag = cn_gkwin_change_all;
    }
}

//----设置可显示边界-----------------------------------------------------------
//功能: 任何窗口，如果即使不考虑被z轴前端窗口剪切，其窗口可显示范围也是有限的，
//      受bound_limit=true的、世代最近的祖先窗口的可显示边界限制。窗口位置改变
//      后，需要重置可显示边界。
//      gkwin，被重置的窗口
//返回: 无
//-----------------------------------------------------------------------------
void __gk_set_bound(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *ancestor;
    s32 gkwin_absx,gkwin_absy,ancestor_absx,ancestor_absy;
    ancestor = gkwin;
    //获取祖先窗口中第一个边界受父窗口限制的第一个窗口，gkwin的边界受该窗口限定
    while(ancestor->bound_limit == false)
    {
        ancestor = (struct gkwin_rsc*)rsc_get_parent(&ancestor->node);
    }
    ancestor = (struct gkwin_rsc*)rsc_get_parent(&ancestor->node);
    //由于left和limit_left都是相对于父窗口的，所以要转换成绝对坐标才能比较
    gkwin_absx = gkwin->absx0;  //取本窗口的绝对坐标，也即gkwin->left的绝对坐标
    gkwin_absy = gkwin->absy0;
    //取边界受限窗口的可见边界原点绝对坐标
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
//----设置边界模式-------------------------------------------------------------
//功能: 设定窗口的显示边界是否受父窗口限制，限制后，子窗口超出父窗口的部分将不予
//      显示，desktop的直接子窗口默认受限，不能更改。
//参数: gkwin，目标窗口
//      mode，true为受限，false为不受限
//返回: 无
//-----------------------------------------------------------------------------
void gk_set_bound_mode(struct gksc_para_set_bound_mode *para)
{
    struct gkwin_rsc *current;
    if(para->gkwin == NULL)
        return;
    if(para->gkwin == para->gkwin->disp->desktop)   //桌面不可修改边界模式
        return;
    if(para->gkwin->bound_limit == para->mode)      //模式未改变
        return;
    if(&para->gkwin->disp->desktop->node == rsc_get_parent(&para->gkwin->node))
        return;                         //直接放在桌面的子窗口，不许改变
    //目标窗口边界模式改变，窗口属性改变
    para->gkwin->bound_limit = para->mode;
    __gk_set_bound(para->gkwin);
    current = para->gkwin;
    //遍历gkwin的所有子窗口
    while(rsc_trave_scion(&para->gkwin->node,&current->node))
    {
        __gk_set_bound(current);
    }
    para->gkwin->disp->reset_clip = true;
}

//----设置窗口显示优先级-------------------------------------------------------
//功能: 设置一个窗口的优先级，优先级决定在同级窗口中的z轴相对位置，数字越小，在
//      z轴中越靠前，优先级<=0将覆盖父窗口，反之被父窗口覆盖。窗口改变优先级后，
//      它在Z轴中的顺序可能会改变，屏幕内容也可能会改变。
//      由于被改变优先级的窗口可能还有子窗口，所以，在z轴中被移动的，不是一个窗
//      口，而是连续的一段窗口。
//参数: gkwin，目标窗口
//      prio，新优先级
//返回: 无
//-----------------------------------------------------------------------------
void gk_set_prio(struct gksc_para_set_prio *para)
{
    struct gkwin_rsc *target_section,*gkwin;  
    struct display_rsc *display;
    struct gkwin_rsc *parent;       //gkwin的父窗口
    //z轴中被移动的窗口段最后端的窗口
    struct gkwin_rsc *section_start;
    //z轴中被移动的窗口段最前端的窗口
    struct gkwin_rsc *section_end,*target_win;     

    gkwin = para->gkwin;
    display = para->gkwin->disp;
    if(gkwin == display->desktop)  //桌面窗口的优先级不可改变
        return;

    
    parent = (struct gkwin_rsc *)rsc_get_parent(&gkwin->node); 
    //取z轴中被移动的窗口段最前端的窗口段
    section_end = (struct gkwin_rsc*)rsc_get_son(&parent->node);
    section_start = (struct gkwin_rsc*)rsc_get_next(&section_end->node);
    target_section = section_end;
    while(1)
    {//查找同级窗口中和优先级等于para->prio的的窗口，扫描以z轴为对象   
        //扫描顺序:从z轴段前端向后端进行扫描，只扫兄弟窗口，不扫子窗口
        if(target_section->z_prio >= para->prio)
            break;
        //扫描目标窗口所在z轴段最后一窗口，终止跳出
        if(target_section == section_start)
            break;
        target_section = (struct gkwin_rsc*)rsc_get_previous(&target_section->node);
    }
    if(target_section->z_prio >= para->prio)
    {// 找到gkwin同级窗口中优先级低于或等于新prio的窗口
        //在资源队列中无须移动，但优先级队列可能要移动
        if((target_section == gkwin) 
                || (target_section == (struct gkwin_rsc*)rsc_get_previous(&gkwin->node)))
        {
            if((gkwin->z_prio <= 0) && (para->prio > 0))    //移到父窗口后端
            {
                //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
                section_start = __gk_get_zsection_all_start(gkwin);
                section_end = __gk_get_zsection_all_end(gkwin);
                //考虑gkwin要插入的窗口是否是最前端窗口,
                //是,则将gkwin所在Z轴段最前端的窗口调整为整个Z轴的最前端窗口
                if(section_end == display->z_topmost)
                    display->z_topmost = parent;
                //将gkwin所在z轴段从z轴取出来
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;
                
                section_start->z_top = parent;
                section_end->z_back = parent->z_back;
                parent->z_back->z_top = section_start;
                parent->z_back = section_end;
                
                gkwin->z_prio = para->prio;
                display->reset_clip = true;
            }
            else if((gkwin->z_prio > 0) && (para->prio <= 0))//移到父窗口前端
            {
                //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
                section_start = __gk_get_zsection_all_start(gkwin);
                section_end = __gk_get_zsection_all_end(gkwin);
                //考虑gkwin要插入的窗口是否是最前端窗口,
                //是,则将gkwin所在Z轴段最前端的窗口调整为整个Z轴的最前端窗口
                if(parent == display->z_topmost)
                    display->z_topmost = section_end;
                //将gkwin所在z轴段从z轴取出来
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;
                
                section_start->z_back = parent;
                section_end->z_top = parent->z_top;
                parent->z_top->z_back = section_end;
                parent->z_top = section_start;
                
                gkwin->z_prio = para->prio;
                display->reset_clip = true;
            }
            else    //其他情况，无须移动z轴
            {
                gkwin->z_prio = para->prio;
            }
        }
        else    //资源队列须移动
        {
            rsc_moveto_next(&target_section->node,&gkwin->node);
																  
            if((para->prio <= 0) && (target_section->z_prio > 0))
                target_win = parent;
            else
                target_win = __gk_get_zsection_all_end(target_section);
            //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
            section_start = __gk_get_zsection_all_start(gkwin);
            section_end = __gk_get_zsection_all_end(gkwin);
            //考虑gkwin要插入的窗口是否是最前端窗口,
            //是,则将gkwin所在Z轴段最前端的窗口调整为整个Z轴的最前端窗口
            if(target_win == display->z_topmost)
                display->z_topmost = section_end;
            //将gkwin所在z轴段从z轴取出来
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
    {//同级窗口优先级均高于新prio
        rsc_moveto_previous(&target_section->node,&gkwin->node);

        if((para->prio > 0) && (target_section->z_prio <=0))
        {
            target_win = parent;
        }
        else
        {
            //找到new_location及其子孙组成的z轴段中最后端的窗口
            target_win = __gk_get_zsection_all_start(target_section);
        }
        //以下把gkwin取出并插入move_end_temp后端
        //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
        section_start = __gk_get_zsection_all_start(gkwin);
        section_end = __gk_get_zsection_all_end(gkwin);
        
        //将gkwin所在z轴段从z轴取出来
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

//----设置光栅操作码---------------------------------------------------------
//功能: 改变窗口的光栅操作码。非缓冲窗口和桌面只支持cn_r3_srccopy，
//    	不可修改光栅操作码。
//参数: gkwin，目标窗口
//      bitmsk，要设置的掩码位图 
//      rop_code，新光栅操作码
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_set_rop_code(struct gksc_para_set_rop_code *para)
{
    u32 buf_size;
    struct bmbuf_info buf_info,buf_bak;
    struct gkwin_rsc *mygkwin;
    struct display_rsc *mydisplay;
    bool_t blend;
    
    mygkwin = para->gkwin;
    mydisplay = mygkwin->disp;
    buf_info.vbuf.pf_type = mydisplay->pixel_format;
    //非缓冲窗口和桌面只支持cn_r3_srccopy，不可修改rop_code
    if((mygkwin->wm_bitmap.bm_bits == NULL)
        || (mydisplay->desktop == mygkwin)
        || (para->rop_code == mygkwin->rop_code))
    {
        return false;        
    }
    //新设置的rop_code是rop4，而给定的bitmsk为NULL，不可以设置rop_code
    if((para->rop_code & cn_rop_ext_en)
                    && (para->rop_code & cn_rop_rop4_en)
                    && (para->bitmsk.bm_bits == NULL))
    {
    	return false;            
    }
    buf_bak.vbuf = mygkwin->wm_bitmap;
    buf_bak.rop4_msk = mygkwin->bitmsk;
    buf_bak.pat = mygkwin->pat_buf;
    buf_info = buf_bak;
    if(__gk_vrmalloc(mydisplay,&buf_info,mygkwin->wm_bitmap.width,
                        mygkwin->wm_bitmap.height,para->rop_code))
    {
        blend = gk_rop_needsno_destination(para->rop_code);
        mygkwin->rop_code = para->rop_code;
        if(mygkwin->dest_blend != blend)
        {
            mygkwin->dest_blend = blend;
            mydisplay->reset_clip = true;
        }
        mygkwin->change_flag = cn_gkwin_change_all; 
        if(mygkwin->wm_bitmap.bm_bits != buf_info.vbuf.bm_bits)
        {
            buf_size = buf_info.vbuf.linebytes * buf_info.vbuf.height;
            memcpy(buf_info.vbuf.bm_bits,mygkwin->wm_bitmap.bm_bits,buf_size);
            mygkwin->wm_bitmap = buf_info.vbuf;
        }
        
        if(mygkwin->bitmsk.bm_bits != buf_info.rop4_msk.bm_bits)
        {
            buf_size = buf_info.rop4_msk.linebytes * buf_info.rop4_msk.height;
            memcpy(buf_info.rop4_msk.bm_bits,mygkwin->bitmsk.bm_bits,buf_size);
            mygkwin->bitmsk = buf_info.rop4_msk;
        }
        if(mygkwin->pat_buf != buf_info.pat)
        {
            buf_size = mydisplay->patx 
                    *(mydisplay->pixel_format>>8)>>3;//计算1行占多少字节
            buf_size = buf_size * mydisplay->paty;
            memcpy(buf_info.pat,mygkwin->pat_buf,buf_size);
            mygkwin->pat_buf = buf_info.pat;
        }
        __gk_vfree(mydisplay,&buf_bak);
        return true;
    }else
    {
        djy_error_login(enum_gk_no_memory,"显示内存不足");
        return false;
    }
}
//----设置窗口图案位图---------------------------------------------------------
//功能: 改变窗口的图案位图。非缓冲窗口和桌面只支持cn_r3_srccopy，
//    	不需要pat_buf。
//参数: gkwin，目标窗口
//      pat_buf，新的图案位图
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_set_pat_buf(struct gksc_para_set_pat_buf *para)
{
    u32 pat_size;
    //非缓冲窗口和桌面只支持cn_r3_srccopy,不需要pat_buf
    if((para->gkwin->disp->frame_buffer == NULL)
        || (para->gkwin->disp->desktop == para->gkwin))
        return false;               
    //计算1行占多少字节    
    pat_size = para->gkwin->disp->patx 
                * (para->gkwin->disp->pixel_format >>8)>>3; 
    pat_size = pat_size * para->gkwin->disp->paty; 
    para->gkwin->disp->format_pattern(para->pattern.bm_bits,&para->pattern);    
    memcpy(para->gkwin->pat_buf,para->pattern.bm_bits,pat_size);
    para->gkwin->dest_blend = 
                	gk_rop_needsno_destination(para->gkwin->rop_code);
    //在窗口光栅操作需要pat_buf的情况下，只改变pat_buf ，需要设置change_flag
    if(gk_rop_needsno_pattern(para->gkwin->rop_code))
        para->gkwin->change_flag = cn_gkwin_change_all;
    return true;
}
//----设置窗口透明色-----------------------------------------------------------
//功能: 改变窗口透明色
//参数: gkwin，目标窗口
//      transparentcolor，设置的透明色,颜色格式是真彩色
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_set_transparentcolor(struct gksc_para_set_transparentcolor *para)
{
    //桌面不需要transparentcolor
    if(para->gkwin->disp->desktop == para->gkwin)
        return false;               
    para->transparentcolor = __gk_convert_color_to_pf(
                                                para->gkwin->wm_bitmap.pf_type,
                                                para->transparentcolor);
    para->gkwin->transparentcolor = para->transparentcolor;
    //下面的程序用于窗口光栅操作属性为透明，只改变当前透明色的情况
    if((para->gkwin->rop_code & cn_rop_ext_en) 
        && (para->gkwin->rop_code & cn_rop_transparentcolor))
    {
        para->gkwin->change_flag = cn_gkwin_change_all;
        para->gkwin->dest_blend = true;
    }
    return true;
}
//----设置窗口掩码位图---------------------------------------------------------
//功能: 设置窗口掩码位图。只有支持rop4的窗口才需要掩码位图
//参数: gkwin，目标窗口
//      bitmsk，设置的掩码位图
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_set_bitmsk(struct gksc_para_set_bitmsk *para)
{
    //非缓冲窗口不需要bitmsk
    if(para->gkwin->disp->frame_buffer == NULL)
        return false;               
    para->gkwin->bitmsk = para->bitmsk;
    para->gkwin->dest_blend = 
                	gk_rop_needsno_destination(para->gkwin->rop_code);
    para->gkwin->change_flag = cn_gkwin_change_all;
    return true;
}
//----设置窗口直接写屏属性-----------------------------------------------------
//功能: 设置窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_set_direct_screen(struct gksc_para_set_direct_screen *para)
{
    if(para->gkwin->direct_screen == true)
        return;
    else
        para->gkwin->direct_screen = true;
}
//----撤销窗口直接写屏属性-----------------------------------------------------
//功能: 撤销窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_unset_direct_screen(struct gksc_para_unset_direct_screen *para)
{
    if(para->gkwin->direct_screen == false)
        return;
    else
        para->gkwin->direct_screen = false;
}
//----销毁窗口(子窗口)---------------------------------------------------------
//功能: 释放窗口所占用的资源。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void __gk_destroy_win(struct gkwin_rsc *gkwin)
{
    struct bmbuf_info buf_info;
    if(gkwin->disp->z_topmost == gkwin)
        gkwin->disp->z_topmost = gkwin->z_back;
    gkwin->z_back->z_top = gkwin->z_top;
    gkwin->z_top->z_back = gkwin->z_back;
    rsc_del_node(&gkwin->node);
    if(gkwin->visible_clip != NULL)
        gkwin->disp->reset_clip = true;
    if(gkwin->disp->frame_buffer != NULL)         
    {
        if(gkwin->changed_msk.bm_bits != NULL)//有帧缓冲在缓冲模式下
        {
            buf_info.vbuf.bm_bits = gkwin->wm_bitmap.bm_bits;
            buf_info.rop4_msk.bm_bits = gkwin->bitmsk.bm_bits;
            buf_info.pat = gkwin->pat_buf;
            __gk_vfree(gkwin->disp,&buf_info);
            m_free(gkwin->changed_msk.bm_bits);
            gkwin->visible_clip = 
                            __gk_free_clip_queue(gkwin->visible_clip);
            return;
        }
        else 
            return;
    }
    else
        return;
}
//----销毁窗口-------------------------------------------------------------
//功能: 释放窗口所占用的资源。销毁窗口，同时销毁窗口的子孙窗口。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_destroy_win(struct gksc_para_destroy_win *para)
{
    struct gkwin_rsc *gkwin;

    while((gkwin = (struct gkwin_rsc *)rsc_get_twig(&para->gkwin->node))!= NULL)
    {
        __gk_destroy_win(gkwin);
    }
    __gk_destroy_win(para->gkwin);

}

//----改变窗口尺寸和位置-------------------------------------------------------
//功能: 改变一个窗口的尺寸以及位置，如果没改尺寸，实际上就是一个gk_move_win。
//参数: gcwin，目标窗口
//      new_area，新的窗口尺寸和位置
//      mode，改尺寸后，窗口图像处理模式，0=保持不变，1=拉伸或压缩，
//            缓冲窗口才有效
//返回: true = 成功，false = 失败(一般是内存分配导致)
//注: 有缓冲区的情况下，如果只是移动了位置，不需要重新绘制。
//    如果修改了尺寸，应用程序应该重绘，直接重绘即可，无须等待重绘消息。
//-----------------------------------------------------------------------------
bool_t gk_change_win_area(struct gksc_para_change_win_area *para)
{
    s32 left,top,right,bottom;
    struct gkwin_rsc *gkwin;
    struct bmbuf_info buf_info;
    struct gkwin_rsc *current;
    struct gksc_para_move_win movwin_para;

    gkwin = para->gkwin;
    left = para->left;
    top = para->top;
    right = para->right;
    bottom = para->bottom;
    if((right < left) || (bottom < top))    //尺寸数据不合法
        return false;

    if(((right-left) == (gkwin->right-gkwin->left))
            && ((bottom-top) == (gkwin->bottom - gkwin->top)))
    {
        movwin_para.gkwin = gkwin;
        movwin_para.left = left;
        movwin_para.top = top;
        gk_move_win(&movwin_para);        //只是移动了窗口，尺寸未变
        return true;
    }
    if(gkwin->wm_bitmap.bm_bits != NULL)
    {
        buf_info.vbuf = gkwin->wm_bitmap;
        if( !__gk_vrmalloc(gkwin->disp,&buf_info,right-left,
                                    bottom-top,gkwin->rop_code))
        {
            djy_error_login(enum_gk_no_memory,"显存不足");
            return false;   //分配显存失败，窗口尺寸未改变(仍然有效)
        }else
        {
            gkwin->change_flag = cn_gkwin_change_none;
            gkwin->wm_bitmap = buf_info.vbuf;
            gkwin->bitmsk = buf_info.rop4_msk;
            gkwin->pat_buf = buf_info.pat;
        }
    }
    current = gkwin;
    gkwin->absx0 -= gkwin->left-left;          //修改窗口绝对坐标
    gkwin->absy0 -= gkwin->top-top;
    gkwin->left = left;
    gkwin->right = right;
    gkwin->top = top;
    gkwin->bottom = bottom;
    __gk_set_bound(gkwin);                      //设置可显示边界
    while(rsc_trave_scion(&gkwin->node,&current->node))//遍历gkwin的所有子窗口
    {
        current->absx0 -= current->left -left; //修改窗口绝对坐标
        current->absy0 -= current->top -top;
        __gk_set_bound(current);
        gkwin->change_flag = cn_gkwin_change_all;
    }
    gkwin->disp->reset_clip = true;
    return true;
}

//----设置画布颜色-------------------------------------------------------------
//功能: 设置画布颜色，俗称背景色，文本也用这个当背景色。
//参数: gkwin，目标窗口
//      color，新的画笔颜色
//返回: 无
//-----------------------------------------------------------------------------
//void gk_set_canvas_color(struct gkwin_rsc *gkwin,u32 color)
//{
//    gkwin->canvas_color = color;
//}

//----取画布颜色-------------------------------------------------------------
//功能: 读取画布颜色，俗称背景色，文本也用这个当背景色。
//参数: gkwin，目标窗口
//      color，新的画笔颜色
//返回: 无
//-----------------------------------------------------------------------------
//void gk_get_canvas_color(struct gkwin_rsc *gkwin,u32 *color)
//{
//    *color = gkwin->canvas_color;
//}

//----输出窗口redraw部分-------------------------------------------------------
//功能: 把所有窗口有修改的可视域输出至显示器，假设可视域已经准备好。
//参数: display，被扫描的显示器
//返回: 无
//-----------------------------------------------------------------------------
void __gk_output_redraw(struct display_rsc *display)
{
    s32 x,y,x_src,y_src,x_dst,y_dst;
    u32 color;
    struct clip_rect *clip;
    struct display_rsc *mirror,*current;
    struct gkwin_rsc *gkwin,*desktop_gkwin,*topwin,*frame_buf;
    struct gkuc_para_repaint repaint;
    struct st_rect_bitmap *src,*dst;
    struct st_rect *rect;
    struct st_rect_bitmap *bitmap;
    
    frame_buf = display->frame_buffer;          //取帧缓冲窗口
    desktop_gkwin = display->desktop;
    topwin = display->z_topmost;
    if(frame_buf == NULL)   //无帧缓冲，无帧缓冲的显示器，必然没有窗口缓冲
    {
        //在没有缓冲区的情况下，绘制操作是不会生成redraw_clip的，只有窗口管理
        //操作才会要求重绘。
        gkwin = desktop_gkwin;
        do{
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                repaint.win = (void*)gkwin;
                repaint.redraw_clip = clip;
                gk_usercall_chunnel(cn_gkuc_repaint,pg_usercall_semp,NULL,
                                  &repaint,sizeof(struct gkuc_para_repaint));
                //等待用户完成重绘操作，超时(1秒)不候
                semp_pend(pg_usercall_semp,0);
                gkwin->redraw_clip = __gk_free_clip_queue(gkwin->redraw_clip);
            }
            gkwin = gkwin->z_top;
        }while(gkwin != topwin->z_top);
    }else                                           //有帧缓冲
    {
        gkwin = desktop_gkwin;
        do{
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                if(gkwin->wm_bitmap.bm_bits == NULL)   //无窗口缓冲
                {
                    repaint.win = (void*)gkwin;
                    repaint.redraw_clip = clip;
                    gk_usercall_chunnel(cn_gkuc_repaint,pg_usercall_semp,NULL,
                                    &repaint,sizeof(struct gkuc_para_repaint));
                    //等待用户完成重绘操作，超时(1秒)不候
                    semp_pend(pg_usercall_semp,0);   
                }else                                   //有窗口缓冲
                {
                    do
                    {
                        if(!display->draw.blt_bm_to_bm(
                                            &frame_buf->wm_bitmap,
                                            &clip->rect,
                                            &gkwin->wm_bitmap,
                                            clip->rect.left-gkwin->absx0,
                                            clip->rect.top-gkwin->absy0,
                                            gkwin->pat_buf,
                                            &gkwin->bitmsk,
                                            gkwin->rop_code,
                                            gkwin->transparentcolor))
                        {
                            dst = &frame_buf->wm_bitmap;
                            src = &gkwin->wm_bitmap;
                            
                            y_dst = clip->rect.top;
                            if((gkwin->rop_code & cn_rop_ext_en) != 0)
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
                                        __gk_set_pixel_rop_ext_bm(dst,src,
                                                gkwin->pat_buf,
                                                x_dst,y_dst,x_src,y_src,
                                                gkwin->rop_code,&gkwin->bitmsk,
                                                gkwin->transparentcolor);
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
                                        __gk_set_pixel_rop3_bm(dst,src,
                                            	gkwin->pat_buf,x_dst,y_dst,
                                            	x_src,y_src,gkwin->rop_code);
                                        x_dst ++; 
                                    }
                                    y_dst ++;
                                 }                                 
                            }
                        }
                        __gk_shading_rect(frame_buf,&clip->rect);
                        clip = clip->next;
                    }while(clip != gkwin->redraw_clip);
                }
                gkwin->redraw_clip = __gk_free_clip_queue(gkwin->redraw_clip);
            }
            gkwin = gkwin->z_top;
        }while(gkwin != topwin->z_top);
        //以下把帧缓冲区输出到显示器
        clip = __gk_get_changed_clip(frame_buf);  //获取帧缓冲中被修改的可视域
        if(clip != NULL)
        {
            __gk_shading_clear(frame_buf);
            frame_buf->redraw_clip = clip;
            mirror = (struct display_rsc *)gkwin->disp->node.child;//镜像显示器
            do{
                if(display->framebuf_direct == false)
                {
                    //输出到screen
                    if(display->draw.bm_to_screen(&clip->rect,
                                                &frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top))
                    {
                    }
                    else
                    {
                        rect = &clip->rect;
                        bitmap = &frame_buf->wm_bitmap;
                        for(y = rect->top;y < rect->bottom;y++)
                        {
                            for(x = rect->left;x < rect->right;x++)
                            {
                                color = __gk_get_pixel_bm(bitmap,x,y);
                                color =__gk_convert_color_to_24(bitmap->pf_type,
                                                                color);
                                display->draw.set_pixel_screen(x,y,color,
                                                                cn_r2_copypen);
                            }
                        }
                    }
                }
                current = mirror;
                
                while(current != NULL)  //遍历全部镜像窗口
                {
                    current->draw.bm_to_screen(&clip->rect,
                                                &frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top);                                                
                    current = (struct display_rsc *)rsc_trave_scion(
                                                &mirror->node,&current->node);
                }
                clip = clip->next;
            }while(clip != frame_buf->redraw_clip);
            frame_buf->redraw_clip =
                            __gk_free_clip_queue(frame_buf->redraw_clip);
        }
    }
}

//----显示内容输出到显示器-----------------------------------------------------
//功能: 把所有显示器中需要输出的部分输出到显示器。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __gk_redraw_all(void)
{
    struct display_rsc *display,*display_next;
    display = (struct display_rsc *)rsc_get_son(&tg_display_root_rsc);
    display_next = display;
    if(display != NULL)
    {
        do{
            __gk_get_redraw_clip_all(display);//扫描取得新的需重绘的区域
            __gk_output_redraw(display);    //重绘
            display_next = (struct display_rsc *)
                                    rsc_get_next(&display_next->node);
        }while(display != display_next);  //扫描所有显示器
    }
}
//----系统调用管道-------------------------------------------------------------
//功能: gui api与gui kernel的接口函数，把api所请求的绘制要求传递到gui kernel，在
//      mp版本中，可以考虑使用软中断实现。
//参数: param，参数地址
//      size，参数长度
//返回: 实际写入管道的数据量，0或者size。
//-----------------------------------------------------------------------------
u16 gk_syscall_chunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2)
{
    u16 completed = 0;
    u8 buf[2];
    mutex_pend(tg_gk_chunnel.syscall_mutex,1000*mS);   //管道访问互斥
    while(1)
    {
        if((ring_capacity(&tg_gk_chunnel.ring_syscall)
                    - ring_check(&tg_gk_chunnel.ring_syscall)) <(size1+size2+2))
        {       //管道空闲容量不够
            if(!semp_pend(tg_gk_chunnel.syscall_semp,1000*mS))//等待管道释放空间
                break;                                  //等待管道失败
            else
                continue;                               //再次检查容量
        }
        buf[0] = (u8)command;
        buf[1] = (u8)(command>>8);
        completed = ring_write(&tg_gk_chunnel.ring_syscall,buf,2);
        if(size1 != 0)
            completed += ring_write(&tg_gk_chunnel.ring_syscall,param1,size1);
        if(size2 != 0)
            completed += ring_write(&tg_gk_chunnel.ring_syscall,param2,size2);
        break;
    }
    mutex_post(tg_gk_chunnel.syscall_mutex);    //管道访问互斥解除
    if(semp_query_free(pg_syscall_semp) == 0)
        semp_post(pg_syscall_semp);
    if(0 != sync_time)      //设定了等待时间
    {
        mutex_pend(pg_gk_server_sync,sync_time);
        mutex_post(pg_gk_server_sync);
    }
    return completed;
}


//----用户调用管道-------------------------------------------------------------
//功能: 本函数是gk_syscall_chunnle函数的逆操作函数，当gui kernel需要上层应用程序
//      完成某些操作时，调用本函数，把操作要求通过管道传送上去。
//参数: param，参数地址
//      size，参数长度
//返回: 实际写入管道的数据量，0或者size。
//注: usercall_buf只有gkwin会往里写东西，故无需互斥量保护
//-----------------------------------------------------------------------------
u16 gk_usercall_chunnel(u16 usercall_id,struct semaphore_LCB *semp,
                        void **result,void *param,u16 size)
{
    u16 completed = 0,loop;
    u8 buf[100];    //不会有超过100字节的命令的。
    while(1)
    {
        if((ring_capacity(&tg_gk_chunnel.ring_usercall)
                    - ring_check(&tg_gk_chunnel.ring_usercall)) < size)
        {       //管道空闲容量不够
            if(!semp_pend(tg_gk_chunnel.usercall_semp,0))//等待管道充足空间
                break;                                  //等待管道失败
            else
                continue;                               //再次检查容量
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
        completed = ring_write(&tg_gk_chunnel.ring_usercall,buf,
                                size+2+sizeof(void*));
    }
    return completed;
}

//----gui kernel服务器---------------------------------------------------------
//功能: 从缓冲区取出显示请求，并处理之。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
u8 draw_chunnel_buf[cfg_gk_down_chunnel_deep];
void gk_server(void)
{
    u16 command;
    u32 num,offset;
    mutex_pend(pg_gk_server_sync,cn_timeout_forever);
    djy_set_event_prio(248);
    while(1)
    {
        //一次读取全部命令，因为发送时有互斥量保护，所以管道中的数据肯定是完整的
        //命令，不存在半条命令的可能。
        num = ring_read(&tg_gk_chunnel.ring_syscall,(u8*)draw_chunnel_buf,cfg_gk_down_chunnel_deep);

        if(num == 0)
        {
            //所有命令均执行完后，检查有没有win buffer需要刷到screen上
            __gk_redraw_all();

            int_save_asyn_signal();
            mutex_post(pg_gk_server_sync);
            mutex_pend(pg_gk_server_sync,cn_timeout_forever);
            int_restore_asyn_signal();
            semp_pend(pg_syscall_semp,cn_timeout_forever);
            continue;
        }
        if(semp_query_used(tg_gk_chunnel.syscall_semp) != 0)
            semp_post(tg_gk_chunnel.syscall_semp);    //释放管道
        offset = 0;
        while(num != offset)
        {
            //由于管道中的数据可能不对齐，故必须把数据copy出来，不能直接用指针
            //指向或强制类型转换
            command = draw_chunnel_buf[offset] + ((u16)draw_chunnel_buf[offset+1]<<8);
            //sizeof(u16)不可用2替代，在cn_byte_bits>=16的机器上，sizeof(u16)=1.
            offset += sizeof(u16);
//            sync_draw = draw_chunnel_buf[offset];
//            offset += 1;  
            switch(command)
            {
                case cn_gksc_create_gkwin:
                {
                    struct gksc_para_create_gkwin para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_create_gkwin));
                    *(para.result) = (void*)gk_create_win(&para);
                    offset += sizeof(struct gksc_para_create_gkwin);
                } break;
                case cn_gksc_set_pixel:
                {
                    struct gksc_para_set_pixel para;
                    memcpy(&para,draw_chunnel_buf+offset,sizeof(struct gksc_para_set_pixel));
                    gk_set_pixel(&para);
                    offset += sizeof(struct gksc_para_set_pixel);
                } break;
                case cn_gksc_lineto:
                {
                    struct gksc_para_lineto para;
                    memcpy(&para,draw_chunnel_buf+offset,sizeof(struct gksc_para_lineto));
                    gk_lineto(&para);
                    offset += sizeof(struct gksc_para_lineto);
                } break;
                case cn_gksc_lineto_inc_end:
                {
                    struct gksc_para_lineto para;
                    memcpy(&para,draw_chunnel_buf+offset,sizeof(struct gksc_para_lineto));
                    gk_lineto_ie(&para);
                    offset += sizeof(struct gksc_para_lineto);
                } break;
                case cn_gksc_draw_bitmap:
                {
                    struct gksc_para_draw_bitmap para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_bitmap));
                    gk_draw_bitmap(&para);
                    offset += sizeof(struct gksc_para_draw_bitmap);
                } break;
                case cn_gksc_draw_bitmap_rop:
                {
                    struct gksc_para_draw_bitmap_rop para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_bitmap_rop));
                    gk_draw_bitmap_rop(&para);
                    offset += sizeof(struct gksc_para_draw_bitmap_rop);
                } break;
                case cn_gksc_fill_win:
                {
                    struct gksc_para_fill_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_fill_win));
                    gk_fill_win(&para);
                    offset += sizeof(struct gksc_para_fill_win);
                } break;
                case cn_gksc_set_rop_code:
                {
                    struct gksc_para_set_rop_code para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_rop_code));
                    gk_set_rop_code(&para);
                    offset += sizeof(struct gksc_para_set_rop_code);
                } break;
                case cn_gksc_set_pat_buf:
                {
                    struct gksc_para_set_pat_buf para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_pat_buf));
                    gk_set_pat_buf(&para);                    
                    offset += sizeof(struct gksc_para_set_pat_buf);
                } break;
                case cn_gksc_draw_text:
                {
                    struct gksc_para_draw_text para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_text));
                    offset += sizeof(struct gksc_para_draw_text);
                    gk_draw_text(&para,(char*)draw_chunnel_buf+offset);                    
                    offset += para.count;
                }break;
                case cn_gksc_set_transparentcolor:
                {
                    struct gksc_para_set_transparentcolor para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_transparentcolor));
                    gk_set_transparentcolor(&para);                    
                    offset += sizeof(struct gksc_para_set_transparentcolor);
                } break;
                case cn_gksc_set_bitmsk:
                {
                    struct gksc_para_set_bitmsk para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_bitmsk));
                    gk_set_bitmsk(&para);                    
                    offset += sizeof(struct gksc_para_set_bitmsk);
                } break;
                case cn_gksc_destroy_win:
                {
                    struct gksc_para_destroy_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_destroy_win));
                    gk_destroy_win(&para);                    
                    offset += sizeof(struct gksc_para_destroy_win);
                } break;
                case cn_gksc_fill_part_win:
                {
                    struct gksc_para_fill_part_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_fill_part_win));
                    gk_fill_part_win(&para);
                    offset += sizeof(struct gksc_para_fill_part_win);
                } break;
                case cn_gksc_set_prio:
                {
                    struct gksc_para_set_prio para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_prio));
                    gk_set_prio(&para);
                    offset += sizeof(struct gksc_para_set_prio);
                } break;
                case cn_gksc_set_bound_mode:
                {
                    struct gksc_para_set_bound_mode para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_bound_mode));
                    gk_set_bound_mode(&para);
                    offset += sizeof(struct gksc_para_set_bound_mode);
                }break;
                case cn_gksc_move_win:
                {
                    struct gksc_para_move_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_move_win));
                    gk_move_win(&para);
                    offset += sizeof(struct gksc_para_move_win);
                }break;
                case cn_gksc_change_win_area:
                {
                    struct gksc_para_change_win_area para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_change_win_area));
                    gk_change_win_area(&para);
                    offset += sizeof(struct gksc_para_change_win_area);
                }break;
                case cn_gksc_draw_circle:
                {
                    struct gksc_para_draw_circle para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_circle));
                    gk_draw_circle(&para);
                    offset += sizeof(struct gksc_para_draw_circle);
                } break;
                case cn_gksc_Bezier:
                {
                    struct gksc_para_Bezier para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_Bezier));
                    gk_Bezier(&para);
                    offset += sizeof(struct gksc_para_Bezier);
                } break;
                case cn_gksc_set_direct_screen:
                {
                    struct gksc_para_set_direct_screen para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_direct_screen));
                    gk_set_direct_screen(&para);
                    offset += sizeof(struct gksc_para_set_direct_screen);
                } break;
                case cn_gksc_unset_direct_screen:
                {
                    struct gksc_para_unset_direct_screen para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_unset_direct_screen));
                    gk_unset_direct_screen(&para);
                    offset += sizeof(struct gksc_para_unset_direct_screen);
                } break;
                default: break;                
            }   //for switch(command)
        }   //for while(num != offset)
    }   //for while(1)
    
}

#endif //   for #if (cfg_gui_kernel == 1)

