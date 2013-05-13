//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel所有包含文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2010-06-10
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __GK_INC_H__
#define __GK_INC_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "gk_win.h"
#include "gk_syscall.h"
#include "gk_usercall.h"
#include "gk_draw.h"
#include "gk_clip.h"
#include "..\encoding\encoding.h"
#include "..\font\font.h"
#include "gk_display.h"
#include "gk_api.h"

#ifdef __cplusplus
}
#endif

#endif //__GK_INC_H__

#endif //   for #if (cfg_gui_kernel == 1)

