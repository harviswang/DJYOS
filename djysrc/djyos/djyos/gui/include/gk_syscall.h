//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel主体部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __GK_SYSCALL_H__
#define __GK_SYSCALL_H__

#ifdef __cplusplus
extern "C" {
#endif

struct st_rect_bitmap;
//gui kernel的syscall编码,由上层通过管道传递给gui kernel
//窗口控制命令组
#define cn_gksc_set_pen_color       0x000   //设置窗口画笔颜色
#define cn_gksc_get_pen_color       0x001   //取画笔颜色
#define cn_gksc_set_canvas_color    0x002   //设置窗口画布颜色
#define cn_gksc_get_canvas_color    0x003   //取画布颜色
#define cn_gksc_set_size            0x004   //设置窗口分辨率
#define cn_gksc_get_size            0x005   //取窗口分辨率
#define cn_gksc_set_cursor          0x006   //设置窗口光标位置
#define cn_gksc_get_cursor          0x007   //设置窗口光标位置
#define cn_gksc_create_gkwin        0x008   //创建窗口命令
#define cn_gksc_create_desktop      0x009   //创建桌面命令
#define cn_gksc_set_direct_screen	0x00a   //设置窗口的直接写屏属性
#define cn_gksc_unset_direct_screen	0x00b   //取消窗口的直接写屏属性
#define cn_gksc_flush_wbuf          0x00c   //把窗口缓冲区中的图像刷新到screen
#define cn_gksc_flush_fbuf          0x00d   //把帧缓冲区中的图像刷新到screen
#define cn_gksc_set_rop_code        0x00e   //设置窗口的光栅属性
#define cn_gksc_set_pat_buf         0x00f   //设置窗口的图案位图
#define cn_gksc_set_transparentcolor 0x010   //设置窗口的透明色
#define cn_gksc_set_bitmsk          0x011   //设置窗口的掩码位图
#define cn_gksc_destroy_win         0x012   //释放窗口的资源
#define cn_gksc_set_prio            0x013   //改变窗口的优先级
#define cn_gksc_set_bound_mode      0x014   //设置窗口边界模式
#define cn_gksc_move_win            0x015   //移动窗口
#define cn_gksc_change_win_area     0x016   //移动窗口

//绘制命令组
#define cn_gksc_set_pixel           0x100   //画点命令
#define cn_gksc_lineto              0x101   //画单像素宽线命令，不包含结束端点
#define cn_gksc_lineto_inc_end      0x102   //画单像素宽线命令，包含结束端点
#define cn_gksc_draw_bitmap         0x103   //画bitmap,无光栅操作
#define cn_gksc_draw_bitmap_rop     0x104   //画bitmap,带光栅操作
#define cn_gksc_fill_win            0x105   //用指定颜色填充窗口
#define cn_gksc_draw_text           0x106
#define cn_gksc_fill_part_win       0x107   //用指定颜色填充一个窗口的一部分
#define cn_gksc_draw_circle         0x108   //画圆
#define cn_gksc_Bezier              0x109   //画贝塞尔曲线

struct gksc_para_create_desktop
{
//    u16 command;
    struct display_rsc *display;        //所属显示器
    struct gkwin_rsc *desktop;          //桌面窗口,由上层分配结构体
    s32 width,height;           //桌面尺寸，若小于显示器尺寸则调整为显示器尺寸
    u32 color;                          //创建桌面时填充的颜色
    bool_t buf_mode;    //true=创建带缓冲的窗口，只有带frame buffer显示器才有效
};
struct gksc_para_create_gkwin
{
//    u16 command;
    struct gkwin_rsc *parent_gkwin;     //新窗口的父窗口
    struct gkwin_rsc *gkwin;            //新窗口,由调用者分配结构体
    struct gkwin_rsc **result;          //用于返回结果
    s32 left,top,right,bottom;          //新窗口的位置，相对于父窗口的坐标
    u32 color;                          //创建窗口时填充的颜色
    bool_t buf_mode;    //true=创建带缓冲的窗口，只有带frame buffer显示器才有效
    char *name;                         //窗口名字(标题)
    u32  transparentcolor;     //透明色，rop_code中允许透明色时有用
    u32 rop_code;       //窗口的光栅属性，在windows的光栅操作码的基础上有扩展
};
struct gksc_para_fill_win
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    u32 color;                      //填充使用的颜色
};
struct gksc_para_fill_part_win
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    struct st_rect rect;            //待填充的矩形
    u32 color;                      //填充使用的颜色
};
struct gksc_para_draw_text
{
//    u16 command;
    struct gkwin_rsc* gkwin;        //目标窗口
    s32 x, y;                       //显示位置，相对于gkwin
    s32 count;                      //缓冲区长度，字节数，含传结束符。
};
struct gksc_para_set_pixel
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    s32 x,y;                        //点对应的坐标
    u32 color;                      //画点使用的颜色
    u32 rop2_code;                  //二元光栅操作码
};
struct gksc_para_draw_bitmap
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    struct st_rect_bitmap bitmap;   //待绘制的位图
    s32 x,y;                        //绘制位置的左上角坐标
};
struct gksc_para_draw_bitmap_rop
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    struct st_rect_bitmap bitmap;   //待绘制的位图
    struct st_rect_bitmap pattern;  //要设置的图案位图   
    struct st_rect_bitmap bitmsk;   //rop4操作才需要的掩码位图，一个单色位图
    u32 transparentcolor;           //要设置的透明色   
    s32 x,y;                        //绘制位置的左上角坐标
    u32 rop_code;                   //光栅操作码
};
struct gksc_para_lineto
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    s32 x1,y1,x2,y2;                //目标直线的起点，终点坐标
    u32 color;                      //绘制直线使用的颜色
    u32 rop2_code;                  //二元光栅操作码
};
struct gksc_para_draw_circle
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    s32 x0,y0;                      //圆心坐标
    s32 r;                          //圆的半径              
    u32 color;                      //画圆使用的颜色
    u32 rop2_code;                  //二元光栅操作码
};
struct gksc_para_Bezier
{
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    float x1,y1,x2,y2,x3,y3,x4,y4;  //绘制Bezier曲线的四个控制点
    u32 color;                      //画Bezier曲线使用的颜色
    u32 rop2_code;                  //二元光栅操作码
};
struct gksc_para_move_win
{
    struct gkwin_rsc *gkwin;        //目标窗口
    s32 left,top;                   //目标窗口新位置的左上角坐标
};
struct gksc_para_change_win_area
{
    struct gkwin_rsc *gkwin;        //目标窗口
    s32 left,top,right,bottom;      //目标窗口新的坐标
};
struct gksc_para_set_bound_mode
{
    struct gkwin_rsc *gkwin;        //目标窗口
    bool_t mode;                    //窗口边界模式，true为受限，false为不受限
};
struct gksc_para_set_prio
{
    struct gkwin_rsc *gkwin;        //目标窗口
    sfast_t prio;                  //新的优先级
};
struct gksc_para_set_rop_code
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    struct st_rect_bitmap bitmsk;   //要设置的掩码位图 
    u32 rop_code;                   //新的光栅操作码
}; 
struct gksc_para_set_pat_buf
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口           
    struct st_rect_bitmap pattern;  //要设置的图案位图
};
struct gksc_para_set_transparentcolor
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    u32 transparentcolor;           //要设置的透明色   
};
struct gksc_para_set_bitmsk
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //绘制的目标窗口
    struct st_rect_bitmap bitmsk;   //要设置的掩码位图   
};
struct gksc_para_set_direct_screen
{
    struct gkwin_rsc *gkwin;        //目标窗口
};
struct gksc_para_unset_direct_screen
{
    struct gkwin_rsc *gkwin;        //目标窗口
};
struct gksc_para_destroy_win
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //要销毁的目标窗口
};
#ifdef __cplusplus
}
#endif

#endif //__GK_SYSCALL_H__

#endif //   for #if (cfg_gui_kernel == 1)

