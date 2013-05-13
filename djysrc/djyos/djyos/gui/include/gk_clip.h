//----------------------------------------------------
//Copyright (C), 2004-2010,  luoshitian.
//版权所有 (C), 2004-2010,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel剪切域处理
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2010-06-12
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)
#ifndef __GK_ClIP_H__
#define __GK_ClIP_H__

#ifdef __cplusplus
extern "C" {
#endif
struct clip_rect *__gk_free_clip_queue(struct clip_rect *clip);
struct clip_rect * __gk_combine_clip_s(struct clip_rect *clipq);
struct clip_rect * __gk_combine_clip(struct clip_rect *clipq);
bool_t __gk_get_rect_ints(struct st_rect *rect1,struct st_rect *rect2,
                     struct st_rect *result);
extern bool_t __gk_scan_new_visible_clip(struct display_rsc *display);
bool_t __gk_get_visible_clip(struct display_rsc *display);
void __gk_copy_visible_clip(struct gkwin_rsc *gkwin);
void __gk_cliplink_connect(struct clip_rect **mlink,struct clip_rect *sub);
void __gk_clip_connect(struct clip_rect **clip_link,struct clip_rect *clip);
struct clip_rect *__gk_get_changed_clip(struct gkwin_rsc *gkwin);
void __gk_cliplink_sub(struct clip_rect *src,struct clip_rect *sub,
                       struct clip_rect **diff,struct clip_rect **cutted);
void __gk_get_new_clip(struct gkwin_rsc *gkwin);
struct clip_rect *__gk_get_cliplink_ints(struct clip_rect **srcclip,
                                             struct clip_rect *desclip);
bool_t __gk_get_redraw_clip_all(struct display_rsc *display);
void __gk_get_clip_section( struct gkwin_rsc *z1,struct gkwin_rsc *z2);

#ifdef __cplusplus
}
#endif

#endif //__GK_ClIP_H__

#endif //   for #if (cfg_gui_kernel == 1)

