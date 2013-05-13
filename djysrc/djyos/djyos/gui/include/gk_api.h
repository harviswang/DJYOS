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
#if (cfg_gui_kernel == 1)

#ifndef __GK_API_H__
#define __GK_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#define cn_gk_async     0       //gk异步，应用发出gk命令后，不等待gk处理
#define cn_gk_sync      1       //gk同步，应用发出gk命令后，阻塞等待gk处理

bool_t gk_api_create_desktop(struct display_rsc *display,
                                        struct gkwin_rsc *desktop,
                                        s32 width,s32 height,u32 color,
                                        bool_t buf_mode);
struct gkwin_rsc *gk_api_get_desktop(char *display_name);
bool_t gk_api_create_gkwin(struct gkwin_rsc *parent,
                                        struct gkwin_rsc *newwin,
                                        s32 left,s32 top,s32 right,s32 bottom,
                                        u32 color,bool_t buf_mode,
                                        char *name,u32 rop_code,
                                        u32 transparentcolor);
void gk_api_fill_win(struct gkwin_rsc *gkwin,u32 color,u32 sync_time);
void gk_api_fill_part_win(struct gkwin_rsc *gkwin,
                            	struct st_rect rect,u32 color,u32 sync_time);
void gk_api_draw_text(struct gkwin_rsc *gkwin,s32 x,s32 y,
                        const char *text,u32 count,u32 sync_time);
void gk_api_set_pixel(struct gkwin_rsc *gkwin,s32 x,s32 y,
                        u32 color,u32 rop2_code,u32 sync_time);
void gk_api_lineto(struct gkwin_rsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time);
void gk_api_lineto_ie(struct gkwin_rsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time);
void gk_api_draw_bitmap(struct gkwin_rsc *gkwin,
                                struct st_rect_bitmap *bitmap,
                                s32 x,s32 y,u32 sync_time);
void gk_api_draw_bitmap_rop(struct gkwin_rsc *gkwin, 
                                struct st_rect_bitmap *bitmap,
                                struct st_rect_bitmap *pattern,
                                struct st_rect_bitmap *bitmsk,
                                u32 transparentcolor,
                                s32 x,s32 y,u32 rop_code,u32 sync_time);
void gk_api_draw_circle(struct gkwin_rsc *gkwin,s32 x0,s32 y0,
                    u32 r,u32 color,u32 rop2_code,u32 sync_time);
void gk_api_draw_bezier(struct gkwin_rsc *gkwin,float x1,float y1,
                	float x2,float y2,float x3,float y3,float x4,float y4,
                    u32 color,u32 rop2_code,u32 sync_time);
void gk_api_move_win(struct gkwin_rsc *gkwin,s32 left,s32 top,u32 sync_time);
void gk_api_set_bound_mode(struct gkwin_rsc *gkwin,bool_t mode);
void gk_api_set_prio(struct gkwin_rsc *gkwin,sint16_t prio,u32 sync_time);
bool_t gk_api_set_rop_code(struct gkwin_rsc *gkwin,
                            struct st_rect_bitmap *bitmsk,
                            u32 rop_code,u32 sync_time);
bool_t gk_api_set_pat_buf(struct gkwin_rsc *gkwin,
                            struct st_rect_bitmap *pattern,
                            u32 sync_time);
bool_t gk_api_set_transparentcolor(struct gkwin_rsc *gkwin,
                                	u32 transparentcolor);
bool_t gk_api_set_bitmsk(struct gkwin_rsc *gkwin,
                            	struct st_rect_bitmap *bitmsk,u32 sync_time);
void gk_api_set_direct_screen(struct gkwin_rsc *gkwin,
                                            	u32 sync_time);
void gk_api_unset_direct_screen(struct gkwin_rsc *gkwin,
                                            	u32 sync_time);
void gk_api_destroy_win(struct gkwin_rsc *gkwin);
u16 gk_api_get_pixel_format(struct display_rsc *display);
#ifdef __cplusplus
}
#endif

#endif //__GK_API_H__

#endif //   for #if (cfg_gui_kernel == 1)

