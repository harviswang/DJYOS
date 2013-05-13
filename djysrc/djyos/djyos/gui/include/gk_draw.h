//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 本地像素格式定义及在缓冲区显示函数定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-10-26
//   作者:  罗侍田.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __draw_h__
#define __draw_h__

#ifdef __cplusplus
extern "C" {
#endif
struct gksc_para_draw_bitmap_rop;
struct gksc_para_set_pixel;
struct gksc_para_lineto;
struct gksc_para_draw_bitmap;
//二元光栅操作，名称与windows定义的一致，只在前面加了前缀cn_
//windows是从0开始排，改为从1开始
#define cn_r2_black         0   //  0   dest = black（黑色）
#define cn_r2_notmergepen   1   // dpon dest = ~(dest or pen)
#define cn_r2_masknotpen    2   // dpna dest = ~pen and dest
#define cn_r2_notcopypen    3   // pn   dest = ~pen
#define cn_r2_maskpennot    4   // pdna dest = ~dest and pen
#define cn_r2_not           5   // dn   dest = ~dest
#define cn_r2_xorpen        6   // dpx  dest = dest xor pen
#define cn_r2_notmaskpen    7   // dpan dest = ~(dest and pen)
#define cn_r2_maskpen       8   // dpa  dest = dest and pen
#define cn_r2_notxorpen     9   // dpxn dest = ~(dest xor pen)
#define cn_r2_nop           10  // d    dest = dest（原色）
#define cn_r2_mergenotpen   11  // dpno dest = ~pen or dest
#define cn_r2_copypen       12  // p    dest = pen（画笔色）
#define cn_r2_mergepennot   13  // pdno dest = ~dest or pen
#define cn_r2_mergepen      14  // dpo  dest = dest or pen
#define cn_r2_white         15  //  1   dest = white（白色）
#define cn_r2_last          15

//三元光栅操作,名称与windows定义的一致，只在前面加了前缀cn_r3_
#define cn_r3_srccopy       (u32)0x00cc0020 // dest = source
#define cn_r3_srcpaint      (u32)0x00ee0086 // dest = source or dest
#define cn_r3_srcand        (u32)0x008800c6 // dest = source and dest
#define cn_r3_srcinvert     (u32)0x00660046 // dest = source xor dest
#define cn_r3_srcerase      (u32)0x00440328 // dest = source and (not dest )
#define cn_r3_notsrccopy    (u32)0x00330008 // dest = (not source)
#define cn_r3_notsrcerase   (u32)0x001100a6 // dest = (not src) and (not dest)
#define cn_r3_mergecopy     (u32)0x00c000ca // dest = (source and pattern)
#define cn_r3_mergepaint    (u32)0x00bb0226 // dest = (not source) or dest
#define cn_r3_patcopy       (u32)0x00f00021 // dest = pattern
#define cn_r3_patpaint      (u32)0x00fb0a09 // dest = dpsnoo
#define cn_r3_patinvert     (u32)0x005a0049 // dest = pattern xor dest
#define cn_r3_dstinvert     (u32)0x00550009 // dest = (not dest)
#define cn_r3_blackness     (u32)0x00000042 // dest = black
#define cn_r3_whiteness     (u32)0x00ff0062 // dest = white

#define cn_r3_nop           (u32)0x00AA0029 // dest = dest
#define cn_rop_p            0xF0
#define cn_rop_s            0xCC
#define cn_rop_d            0xAA

//绘制操作分四大类:
//1、在bitmap上绘制
//2、在screen上绘制
//3、取bitmap上的像素或bitmap
//4、取screen上的像素或bitmap。
//在bitmap上，绘像素、线时，是带rop2操作码的，硬件:不支持、部分支持、全支持
//bitmap上填充矩形的时候，不带rop2操作码，硬件可能支持，也可能不支持
//bitmap上绘位图时，带rop3操作码，硬件:不支持、部分支持、全支持
//以上三种情况，均不能以NULL了之。而是支持的实现之，不支持的返回false，由gui
//kernel实现之。这样的话，也就无须check_raster函数了。

//gui kernel确保，各成员函数的参数中作为绘图目标的bitmap，均来自win buffer或者
//frame buffer中的bitmap，不是用户任意提供的bitmap。无论是win buffer 还是
//frame buffer，其像素格式都跟该显示器相同
//而作为图像源的bitmap，则可以是cn_sys_pf_xxx中的任意一种，或者与本显示器所使用
//的像素格式相同
struct disp_draw
{
    //在矩形位图中画一个像素，若显示器使用cn_custom_pf格式，或者有硬件加速功能，
    //应该实现这个函数，否则直接返回false。即使有硬件加速，但该加速功能不支持
    //r2_code编码的话，也返回false。color的格式是cn_sys_pf_e8r8g8b8
    bool_t (*set_pixel_bm)(struct st_rect_bitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code);
    //在矩形位图中画一条任意细直线，不含端点，若显示器使用cn_custom_pf格式，或者
    //有硬件加速功能，应该实现这个函数，否则直接返回false。即使有硬件加速，但该
    //加速功能不支持r2_code编码的话，也返回false。color格式是cn_sys_pf_e8r8g8b8
    bool_t (*line_bm)(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //在矩形位图中画一条任意细直线，含端点，若显示器使用cn_custom_pf格式，或者
    //有硬件加速功能，应该实现这个函数，否则直接返回false。即使有硬件加速，但该
    //加速功能不支持r2_code编码的话，也返回false。color格式是cn_sys_pf_e8r8g8b8
    bool_t (*line_bm_ie)(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //在矩形位图中填充矩形，若显示器使用cn_custom_pf格式，或者有硬件加速功能，
    //应该实现这个函数，否则直接返回false。color格式是cn_sys_pf_e8r8g8b8
    //参数color的格式必须是cn_sys_pf_e8r8g8b8
    bool_t (*fill_rect_bm)(struct st_rect_bitmap *dst_bitmap,
                         struct st_rect *dst_rect,u32 color);
    
    //在两个矩形位图中位块传送，如果显示器使用的像素格式是cn_custom_pf，或者硬件
    //2d加速支持位块传送，须实现这个函数，否则直接返回false。即使有硬件加速，但
    //该加速功能不支持rop_code编码的话，也返回false。本函数返回false的
    //话，gui kernel会自行用逐像素方法拷贝。
    //目标位图的像素格式默认为显示器相同的格式。
    //如果rop_code要求执行的是rop4操作，则掩码位图的尺寸和位置与源位图一致
    bool_t (*blt_bm_to_bm)( struct st_rect_bitmap *dst_bitmap,
                            struct st_rect *dst_rect,
                            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                            u8 *pat,struct st_rect_bitmap *bitmsk,u32 rop_code,
                            u32 transparentcolor);
    //从矩形位图中取一像素，并转换成cn_sys_pf_e8r8g8b8，如果显示器格式不是
    //cn_custom_pf，设为NULL。
    u32 (*get_pixel_bm)(struct st_rect_bitmap *bitmap,s32 x,s32 y);
    //把src位图内rect矩形区域的内容复制到dest中，调用前，先设置好dest的pf_type，
    //本函数不理会src的pf_type，直接使用screen的格式。
    //本函数用于从窗口剪切矩形，而blt_bm_to_bm用于显示矩形。
    //如果显示器使用的像素格式是cn_custom_pf，或者硬件2d加速支持bm间拷贝图形，须
    //实现这个函数，否则直接返回false。
    //由于dest->pf_type存在很多可能，即使有硬件加速，也存在只支持部分pf_type的情
    //况，对不支持的格式，返回false。
    bool_t (*get_rect_bm)(struct st_rect_bitmap *src,struct st_rect *rect,
                        struct st_rect_bitmap *dest);
    //查询指定的三元(四元)光栅操作是否支持,这是扩展光栅操作码，含alpha等。
    //OS在库中提供软件模拟光栅操作的代码，如果硬件加速不支持光栅操作，driver是否
    //引用库中函数以支持光栅操作，完全由driver作者决定。也可以部分引用以实现部分
    //光栅操作，比如实现常用的16个光栅操作。
//    bool_t (*check_raster)(u32 rop_code);

    //以下是在screen中绘图的函数
    //在screen中画一个像素，有frame buffer的情况下，正常显示gui不会调用这个函数，
    //如果窗口direct_screen==true，则可能调用本函数，
    //因此，无论是否有frame buffer，driver都必须提供并且必须实现本函数
    //镜像显示器的driver须提供这个函数，也必须实现本函数
    bool_t (*set_pixel_screen)(s32 x,s32 y,u32 color,u32 r2_code);
    //在screen中画一条任意直线，不含端点，如硬件加速不支持在screen上画线，driver
    //可以简化，直接返回false即可。有frame buffer的情况下，正常显示gui不会调用这
    //个函数，如果窗口direct_screen==true，则可能调用本函数，本函数返回false的
    //话，会进一步调用set_pixel_screen函数
    //镜像显示器的driver须提供这个函数，由于不知道本地显示器的情况，镜像显示器
    //driver必须实现本函数，不能简单返回false。
    //color的格式是cn_sys_pf_e8r8g8b8
    bool_t (*line_screen)(struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);

    //在screen中画一条任意直线，不含端点，如硬件加速不支持在screen上画线，driver
    //可以简化，直接返回false即可。有frame buffer的情况下，正常显示gui不会调用这
    //个函数，如果窗口direct_screen==true，则可能调用本函数，本函数返回false的
    //话，会进一步调用set_pixel_screen函数
    //镜像显示器的driver须提供这个函数，由于不知道本地显示器的情况，镜像显示器
    //driver必须实现本函数，不能简单返回false。
    //color的格式是cn_sys_pf_e8r8g8b8
    bool_t (*line_screen_ie)(struct st_rect *limit,
                           s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //screen中矩形填充，如硬件加速不支持在screen上矩形填充，driver可以简化，直接
    //返回false即可。有frame buffer的情况下，正常显示gui不会调用这
    //个函数，如果窗口direct_screen==true，则可能调用本函数，本函数返回false的
    //话，会进一步调用set_pixel_screen函数
    //镜像显示器的driver须提供这个函数，由于不知道本地显示器的情况，镜像显示器
    //driver必须实现本函数，不能简单返回false。
    //color的格式是cn_sys_pf_e8r8g8b8
    bool_t (*fill_rect_screen)(struct st_rect *dst_rect,u32 color);
    //从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
    //如硬件加速不支持在screen上绘制位图，driver可以简化，直接
    //返回false即可。有frame buffer的情况下，__gk_output_redraw中会调用这个函数
    //。如果窗口direct_screen==true，也可能调用本函数。本函数返回false的
    //话，gui kernel会进一步调用set_pixel_screen函数
    //即使硬件加速支持，但如果不支持具体的src_bitmap->pf_type,也可返回false
    //逐个个像素描镜像显示太慢，镜像显示器的driver必须实现本函数，不能简单返回false。
    bool_t (*bm_to_screen)(struct st_rect *dst_rect,
                struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc);
    
    //从screen中取一像素，并转换成cn_sys_pf_e8r8g8b8
    u32 (*get_pixel_screen)(s32 x,s32 y);
    //把screen内矩形区域的内容复制到bitmap，调用前，先设置好dest的pf_type
    bool_t (*get_rect_screen)(struct st_rect *rect,struct st_rect_bitmap *dest);
};
//直接写屏不支持rop操作
bool_t gk_rop_needsno_destination(u32 rop_code);
bool_t gk_rop_needsno_source(u32 rop_code);
bool_t gk_rop_needsno_pattern(u32 rop_code);
u32 __gk_convert_color_to_24(u16 pf_type,u32 color);
u32 __gk_convert_color_to_pf(u16 pf_type,u32 color);
u32 __gk_convert_part_pf_to_pf(u16 spf_type,u16 dpf_type,u32 color);
u32 __gk_blend_rop2(u32 dest,u32 pen,u32 rop2_code);
u32 __gk_blend_rop3(u32 dest,u32 src,u32 pat,u8 rop3_code);
u32 __gk_point_sect(s32 x,s32 y,struct st_rect *limit);
u32 __gk_oline_sect_inter(struct st_rect *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2);
void __gk_draw_oline(struct gkwin_rsc *gkwin,s32 x1,s32 y1,
                        s32 x2,s32 y2,u32 color,u32 rop2_code);
u32 __gk_get_pixel_bm(struct st_rect_bitmap *bitmap,s32 x,s32 y);
void __gk_get_rgb_bm(u16 pf_type,u32 color,u32 *a,u32 *r,
                u32 *g,u32 *b);
u32 __gk_rop_alpha(struct st_rect_bitmap *dst_bitmap,
            struct st_rect_bitmap *src_bitmap,s32 x_dst,s32 y_dst,
            s32 x_src,s32 y_src,u32 fore,u32 back);
void __gk_set_pixel_rop_ext_bm(struct st_rect_bitmap *dst_bitmap,
                        struct st_rect_bitmap *src_bitmap,u8 *pat,
                        s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                        u32 rop_code,struct st_rect_bitmap *bitmsk,
                        u32 transparentcolor);
void __gk_set_pixel_rop2_bm(struct st_rect_bitmap *bitmap,
                        s32 x,s32 y,u32 color,u32 rop2_code);
void __gk_set_pixel_rop3_bm(struct st_rect_bitmap *dst_bitmap,
                        struct st_rect_bitmap *src_bitmap,
                        u8 *pat,s32 x_dst,s32 y_dst,s32 x_src,
                        s32 y_src,u32 rop3_code);
void gk_set_pixel(struct gksc_para_set_pixel *para);
void __gk_shading_pixel(struct gkwin_rsc *gkwin,s32 x,s32 y);
void __gk_shading_rect(struct gkwin_rsc *gkwin,struct st_rect *rect);
void __gk_shading_line(struct gkwin_rsc *gkwin,struct st_rect *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2);
void __gk_shading_clear(struct gkwin_rsc *gkwin);
void __gk_vlineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 y2,u32 color,u32 rop2_code);
void __gk_hlineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,u32 color,u32 rop2_code);
void __gk_olineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __gk_lineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __gk_set_pixel_screen(struct display_rsc *display,s32 x,s32 y,
                        u32 color,u32 rop2_code);
void __gk_lineto_screen(struct display_rsc *display,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __gk_blt_bm_to_screen(struct display_rsc *display,struct st_rect *rect,
                        struct st_rect_bitmap *bitmap,s32 x,s32 y);
void __gk_fill_rect(struct st_rect_bitmap *dst_bitmap,
                     struct st_rect *dst_rect,u32 color);
void __gk_fill_bm(struct st_rect_bitmap *dst_bitmap,u32 color);
void __gk_set_pixels_circle(struct gkwin_rsc *gkwin,struct st_rect *limit,u32 r,
                                struct st_rect_bitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 rop2_code);
void __gk_draw_circle_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void __gk_draw_circle_screen(struct display_rsc *display,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void gk_draw_circle(struct gksc_para_draw_circle *para);
void __gk_Bezier_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);
void __gk_Bezier_screen(struct display_rsc *display,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);
void gk_Bezier(struct gksc_para_Bezier *para);
void gk_lineto(struct gksc_para_lineto *para);
void gk_lineto_ie(struct gksc_para_lineto *para);
void gk_draw_bitmap(struct gksc_para_draw_bitmap *para);
void gk_draw_text(struct gksc_para_draw_text *para,const char *text);
void gk_draw_bitmap_rop(struct gksc_para_draw_bitmap_rop *para);
void gk_fill_part_win(struct gksc_para_fill_part_win *para);
void gk_fill_win(struct gksc_para_fill_win *para);


#ifdef __cplusplus
}
#endif

#endif /*__draw_h__*/

#endif //   for #if (cfg_gui_kernel == 1)

