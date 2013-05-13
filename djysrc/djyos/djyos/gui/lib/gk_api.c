//----------------------------------------------------
//Copyright (C), 2011-2011,  luoshitian.
//版权所有 (C), 2004-2010,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel应用程序接口部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-10-07
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include "gk_api.h"

//----创建桌面-----------------------------------------------------------------
//功能: 创建桌面，新显示器加入后，首先要创建桌面才能使用。桌面其实和一个普通窗口
//      非常类似，差别主要在于:
//          1、桌面不能小于显示器尺寸，而窗口可以。
//          2、桌面的rop属性只能是cn_r3_srccopy，窗口可以任意。
//          3、桌面的优先级只能是0。
//          4、一个显示器下，桌面只有一个，由display的desktop指针指向。
//      在一个显示器下的所有窗口都是桌面的子窗口。
//参数: display，桌面的宿主显示器
//      desktop，新创建的桌面，保存数据结构所需内存由调用者提供
//      width，height，桌面的尺寸，小于显示器尺寸则调整为显示器尺寸
//      color，创建桌面时填充的颜色
//      buf_mode，true=缓冲模式，false=非缓冲模式
//返回: true=创建成功，false=创建失败
//-----------------------------------------------------------------------------
bool_t gk_api_create_desktop(struct display_rsc *display,
                                        struct gkwin_rsc *desktop,
                                        s32 width,s32 height,u32 color,
                                        bool_t buf_mode)
{
    struct gksc_para_create_desktop para;
    struct gkwin_rsc *result;
    if((NULL == display) || (NULL == desktop))
        return false;
    para.display = display;
    para.desktop = desktop;
    para.width = width;
    para.height = height;
    para.color = color;
    para.buf_mode = buf_mode;
    result = (void*)gk_create_desktop(&para);
    if(result == NULL)
        return false;
    else
        return true;
}

//----获取桌面指针-------------------------------------------------------------
//功能: 根据显示器名，获得桌面的指针。该指针指向一个窗口控制块
//参数: display_name，桌面所在显示器的名字
//返回: 如果找到，则返回指针，否则返回NULL
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_api_get_desktop(char *display_name)
{
    struct display_rsc *display;
    display = (struct display_rsc *)rsc_search_tree("display");
    if(display != NULL)
        display = (struct display_rsc *)rsc_search_scion(
        		(struct rsc_node*)display,display_name);
    if(display != NULL)
        return display->desktop;
    else
        return NULL;
}
//----创建窗口-----------------------------------------------------------------
//功能: 创建一个窗口，新创建的窗口只是一个矩形区域，没有填充，也没有边框等一切
//      装饰。
//参数: parent，父窗口指针
//      newwin，新创建的窗口，保存数据结构所需内存由调用者提供
//      left、top、right、bottom，桌面的尺寸，相对于父窗口
//      color，创建窗口时填充的颜色
//      buf_mode，true=缓冲模式，false=非缓冲模式
//      name，新窗口的名字，将copy到newwin中
//      rop_code，新窗口的rop属性
//      transparentcolor,设定窗口透明色
//返回: true=创建成功，false=创建失败
//-----------------------------------------------------------------------------
bool_t gk_api_create_gkwin(struct gkwin_rsc *parent,
                                        struct gkwin_rsc *newwin,
                                        s32 left,s32 top,s32 right,s32 bottom,
                                        u32 color,bool_t buf_mode,
                                        char *name,u32 rop_code,
                                        u32 transparentcolor)
{
    struct gksc_para_create_gkwin para;
    struct gkwin_rsc *result;
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
    para.rop_code = rop_code;
    para.transparentcolor = transparentcolor;
    gk_syscall_chunnel(cn_gksc_create_gkwin,cn_timeout_forever,
                            &para,sizeof(para),NULL,0);
    if(result != NULL)
        return false;
    else
        return true;
}
//----异步填充窗口-------------------------------------------------------------
//功能: 用指定颜色填充一个窗口。
//参数: gkwin，待填充的窗口
//      color，填充颜色
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_fill_win(struct gkwin_rsc *gkwin,u32 color,u32 sync_time)
{
    struct gksc_para_fill_win para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.color = color;
    gk_syscall_chunnel(cn_gksc_fill_win,sync_time,&para,sizeof(para),NULL,0);
    return;
}
//----填充窗口的一部分---------------------------------------------------------
//功能: 用指定颜色填充一个窗口的一部分。
//参数: gkwin，待填充的目标窗口
//      rect，待填充的位置
//      color，要填充颜色
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_fill_part_win(struct gkwin_rsc *gkwin,
                            	struct st_rect rect,u32 color,u32 sync_time)
{
    struct gksc_para_fill_part_win para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.rect = rect;
    para.color = color;
    gk_syscall_chunnel(cn_gksc_fill_part_win,sync_time,&para,
                    	sizeof(para),NULL,0);
    return;
}

//----显示文本-----------------------------------------------------------------
//功能: 用默认字体和默认字符集显示一个文本串。
//参数: gkwin，显示的目标窗口
//      x、y，显示位置，相对于gkwin
//      text，待显示的字符串
//      count，字符串长度，含串结束符
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_draw_text(struct gkwin_rsc *gkwin,s32 x,s32 y,
                        const char *text,u32 count,u32 sync_time)
{
    struct gksc_para_draw_text para;
    if((NULL == gkwin) || (NULL == text) || (0 == count))           
        return;
    para.gkwin = gkwin;
    para.x = x;
    para.y = y;
    para.count = count;
    gk_syscall_chunnel(cn_gksc_draw_text,sync_time,
                        &para,sizeof(para),(void*)text,count);
    return;
}
//----画点---------------------------------------------------------------------
//功能: 绘制像素。
//参数: gkwin，绘制的目标窗口
//      x、y，绘制的位置，相对于gkwin
//      color，给定的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_set_pixel(struct gkwin_rsc *gkwin,s32 x,s32 y,
                        u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_set_pixel para;
    if(NULL == gkwin)           
        return;    
    para.gkwin = gkwin;
    para.x = x;
    para.y = y;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_set_pixel,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----画单像素宽直线，不包含结束端点-------------------------------------------
//功能: 绘制直线，不包括结束端点
//参数: gkwin，绘制的目标窗口
//      x1、y1、x2、y2,要绘制的直线的起、终点坐标
//      color，绘制的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_lineto(struct gkwin_rsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_lineto para;
    if(NULL == gkwin)          
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_lineto,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----画单像素宽直线，包含结束端点---------------------------------------------
//功能: 绘制直线，包括结束端点
//参数: gkwin，绘制的目标窗口
//      x1、y1、x2、y2,要绘制的直线的起、终点坐标
//      color，绘制的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_lineto_ie(struct gkwin_rsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_lineto para;
    if(NULL == gkwin)          
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_lineto_inc_end,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----画bitmap，无光栅操作-----------------------------------------------------
//功能: 绘制bitmap，无光栅操作。
//参数: gkwin，绘制的目标窗口
//      bitmap，待绘制的位图      
//      x、y，绘制位置的左上角坐标
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_draw_bitmap(struct gkwin_rsc *gkwin,
                                struct st_rect_bitmap *bitmap,
                                s32 x,s32 y,u32 sync_time)
{
    struct gksc_para_draw_bitmap para;
    if(NULL == gkwin)         
        return;
    para.gkwin = gkwin;
    para.bitmap = *bitmap;
    para.x = x;
    para.y = y;
    gk_syscall_chunnel(cn_gksc_draw_bitmap,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}

//----画光栅位图---------------------------------------------------------------
//功能: 绘制bitmap，带光栅操作。
//参数: gkwin，绘制的目标窗口
//      bitmap，待绘制的位图
//      pattern，要设置的图案位图    
//      bitmsk，rop4操作才需要的掩码位图，一个单色位图
//      transparentcolor，要设置的透明色   
//      x、y，绘制的目标位置
//      rop_code，光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_draw_bitmap_rop(struct gkwin_rsc *gkwin, 
                                struct st_rect_bitmap *bitmap,
                                struct st_rect_bitmap *pattern,
                                struct st_rect_bitmap *bitmsk,
                                u32 transparentcolor,
                                s32 x,s32 y,u32 rop_code,u32 sync_time)
{
    struct gksc_para_draw_bitmap_rop para;
    if(NULL == gkwin)           
        return;
    para.gkwin = gkwin;
    para.bitmap = *bitmap;
    para.pattern = *pattern;
    para.bitmsk = *bitmsk;
    para.transparentcolor = transparentcolor;
    para.x = x;
    para.y = y;
    para.rop_code = rop_code;
    gk_syscall_chunnel(cn_gksc_draw_bitmap_rop,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----画单像素宽的圆-----------------------------------------------------------
//功能: 画单像素宽的圆。
//参数: gkwin，绘制的目标窗口
//      x0、y0，圆的中心点坐标，相对于窗口
//      r，圆的半径
//      color，画圆使用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_draw_circle(struct gkwin_rsc *gkwin,s32 x0,s32 y0,
                    u32 r,u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_draw_circle para;
    if(NULL == gkwin)          
        return;
    para.gkwin = gkwin;
    para.x0 = x0;
    para.y0 = y0;
    para.r = r;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_draw_circle,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----画Bezier曲线-------------------------------------------------------------
//功能: 画Bezier曲线。
//参数: gkwin，绘制的目标窗口
//      x1、y1、x2、y2、x3、y3、x4、y4，绘制Bezier曲线的四个控制点
//      color，绘制的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_draw_bezier(struct gkwin_rsc *gkwin,float x1,float y1,
                	float x2,float y2,float x3,float y3,float x4,float y4,
                    u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_Bezier para;
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
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_Bezier,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----移动窗口-----------------------------------------------------------------
//功能: 移动窗口。
//参数: gkwin，目标窗口
//      left、top，窗口移动后的左上角坐标，相对于父窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_move_win(struct gkwin_rsc *gkwin, s32 left,s32 top,u32 sync_time)
{
    struct gksc_para_move_win para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    gk_syscall_chunnel(cn_gksc_move_win,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}
//----改变窗口位置尺寸---------------------------------------------------------
//功能: 改变窗口的位置和尺寸，所有坐标均相对于父窗口
//参数: gkwin，目标窗口
//      left、top、right、bottom，窗口的新坐标，相对于父窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_change_win_area(struct gkwin_rsc *gkwin, s32 left,s32 top,
                                       s32 right,s32 bottom, u32 sync_time)
{
    struct gksc_para_change_win_area para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    gk_syscall_chunnel(cn_gksc_change_win_area,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}

//----设置窗口的边界模式-------------------------------------------------------
//功能: 设置窗口的边界模式。
//参数: gkwin，目标窗口
//      mode， 窗口的边界模式，cn_bound_limit为受限，cn_bound_unlimit为不受限
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_set_bound_mode(struct gkwin_rsc *gkwin, bool_t mode)
{
    struct gksc_para_set_bound_mode para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.mode = mode;
    gk_syscall_chunnel(cn_gksc_set_bound_mode,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return;
}
//----设置窗口显示优先级-------------------------------------------------------
//功能: 设置窗口显示优先级。
//参数: gkwin，目标窗口
//      prio，新优先级
//      sync_time，绘制图形时间同步，在此时间内不管是否绘制完成都返回
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_set_prio(struct gkwin_rsc *gkwin, sint16_t prio,u32 sync_time)
{
    struct gksc_para_set_prio para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.prio = prio;
    gk_syscall_chunnel(cn_gksc_set_prio,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}
//----设置窗口的光栅属性-------------------------------------------------------
//功能: 设置窗口的光栅属性。
//参数: gkwin，目标窗口
//      bitmsk,掩码位图
//      rop_code,新的光栅操作码    
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_api_set_rop_code(struct gkwin_rsc *gkwin,
                            struct st_rect_bitmap *bitmsk,
                            u32 rop_code,u32 sync_time)
{
    struct gksc_para_set_rop_code para;
    if(NULL == gkwin)        
        return false;
    if(rop_code == gkwin->rop_code)
        return true;
    para.gkwin = gkwin;
    para.bitmsk = *bitmsk;
    para.rop_code = rop_code;
    gk_syscall_chunnel(cn_gksc_set_rop_code,sync_time,
                        &para,sizeof(para),NULL,0);
    return true;    
}
//----设置窗口图案位图---------------------------------------------------------
//功能: 改变窗口的图案位图。
//参数: gkwin，目标窗口
//      pattern，新的图案位图   
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_api_set_pat_buf(struct gkwin_rsc *gkwin,
                            struct st_rect_bitmap *pattern,
                            u32 sync_time)
{
    struct gksc_para_set_pat_buf para;
    if(NULL == gkwin)        
        return false; 
    para.gkwin = gkwin;
    para.pattern = *pattern;
    gk_syscall_chunnel(cn_gksc_set_pat_buf,sync_time,
                        &para,sizeof(para),NULL,0);
    return true;    
}
//----设置窗口透明色-----------------------------------------------------------
//功能: 设置窗口的透明色。
//参数: gkwin，目标窗口
//      transparentcolor，要设置的透明色 
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_api_set_transparentcolor(struct gkwin_rsc *gkwin,
                                    	u32 transparentcolor)
{
    struct gksc_para_set_transparentcolor para;
    if(NULL == gkwin)           
        return false;
    if(transparentcolor == gkwin->transparentcolor)
        return true;
    para.gkwin = gkwin;
    para.transparentcolor = transparentcolor;
    gk_syscall_chunnel(cn_gksc_set_transparentcolor,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return true;    
}
//----设置窗口掩码位图---------------------------------------------------------
//功能: 设置窗口掩码位图。
//参数: gkwin，目标窗口
//      bitmsk，设置的掩码位图
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t gk_api_set_bitmsk(struct gkwin_rsc *gkwin,
                            	struct st_rect_bitmap *bitmsk,u32 sync_time)
{
    struct gksc_para_set_bitmsk para;
    if(NULL == gkwin)          
        return false;
	para.gkwin = gkwin;
	para.bitmsk = *bitmsk;
	gk_syscall_chunnel(cn_gksc_set_bitmsk,sync_time,
                        &para,sizeof(para),NULL,0);
        return true;
}

//----设置窗口直接写屏属性-----------------------------------------------------
//功能: 设置窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_set_direct_screen(struct gkwin_rsc *gkwin,u32 sync_time)
{
    struct gksc_para_set_direct_screen para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    gk_syscall_chunnel(cn_gksc_set_direct_screen,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}
//----撤销窗口直接写屏属性-----------------------------------------------------
//功能: 撤销窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_unset_direct_screen(struct gkwin_rsc *gkwin,u32 sync_time)
{
    struct gksc_para_unset_direct_screen para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    gk_syscall_chunnel(cn_gksc_unset_direct_screen,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return;
}
//----销毁窗口-----------------------------------------------------------------
//功能: 释放窗口所占用的资源。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void gk_api_destroy_win(struct gkwin_rsc *gkwin)
{
    struct gksc_para_destroy_win para;
    if(NULL == gkwin)           
        return;        
    para.gkwin = gkwin;
    gk_syscall_chunnel(cn_gksc_destroy_win,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return;
}

//----查询显卡格式-------------------------------------------------------------
//功能: 查询显卡使用的颜色格式，画位图时，如果使用跟显卡相同的颜色格式，将获得
//      最优性能
//参数: display，被查询的显卡
//返回: 颜色格式
//-----------------------------------------------------------------------------
u16 gk_api_get_pixel_format(struct display_rsc *display)
{
    if(display != NULL)
        return display->pixel_format;
    else
        return 0;
}

#endif //   for #if (cfg_gui_kernel == 1)

