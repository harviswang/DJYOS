//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel向user(通常是gui window)传送操作请求
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __GK_USERCALL_H__
#define __GK_USERCALL_H__

#ifdef __cplusplus
extern "C" {
#endif

//gui kernel的user call编码,由上层通过管道传递给gui kernel
#define cn_gkuc_repaint         0x000   //重绘窗口

struct gkuc_para_repaint
{
    void *win;               //绘制的目标窗口
    struct clip_rect *redraw_clip;  //需重绘的剪切域链表
};

#ifdef __cplusplus
}
#endif

#endif //__GK_USERCALL_H__

#endif //   for #if (cfg_gui_kernel == 1)

