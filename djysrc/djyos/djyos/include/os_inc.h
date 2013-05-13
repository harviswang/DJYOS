//----------------------------------------------------
//Copyright (C), 2005-2009,  luoshitian.
//版权所有 (C), 2005-2009,   罗侍田.
//所属模块:包含文件
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:包含了所有操作系统模块所用到的头文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __os_inc_h__
#define __os_inc_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "config-prj.h"
#include "stdint.h"
#include "arch.h"
#include "endian.h"
#include "kernel.h"
#include "critical.h"
#include "sys_init.h"
#include "int.h"
#include "int_hard.h"
#include "djyos.h"
#include "cpu.h"
#include "rsc.h"
#include "ring.h"
#include "line.h"
#include "lock.h"
#include "memory.h"
#include "memb.h"
#include "mems.h"
#include "driver.h"
#include "clock.h"
#include "pipe.h"
#include "wdt.h"
#include "file.h"
#include "flashfile.h"
#include "stdin_dev.h"
#include "key.h"
#include "touch.h"
#include "char_term.h"
#if(cfg_gui_kernel == 1)
    #include "gk_inc.h"
#endif
#if(cfg_nude_terminal == 1)
    #include "nude_io.h"
#endif
#if(cfg_mmu_used == 1)
    #include "mmu.h"
#endif
#include "version.h"

#ifdef __cplusplus
}
#endif

#endif //__os_inc_h__

