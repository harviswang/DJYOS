//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 内核模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 加载操作系统
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者:  罗侍田.
//   新版本号：V1.1.0
//   修改说明: 原版本中，把系统初始化也放在这里了，本版本分离之
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
//#include <string.h>
#include "int_hard.h"
#include "copy_table.h"

#if cn_cache_used == 1
#include "cache.h"
#endif

extern struct copy_table sysload_copy_table;

//----操作系统内核加载程序-----------------------------------------------------
//功能：加载所有操作系统内核代码，以及在si模式下全部应用程序代码。
//参数: 无。
//返回: 无。
//----------------------------------------------------------------------------
//备注: 本函数移植敏感，与开发系统有关，也与目标硬件配置有关
void loader(void)
{
    u32 *src,*des;
    u32 i, j;

    for(i=0; i<sysload_copy_table.record_cnt; i++) {
		src = (u32*) sysload_copy_table.record[i].load_start_address;
		des = (u32*) sysload_copy_table.record[i].run_start_address;
		if(sysload_copy_table.record[i].type == 1) {	//copy
	    	if(src != des) {
				for(j=0; j<sysload_copy_table.record[i].size; src++,des++) {
					*des=*src;
					j+=4;
				}
	    	}
		} else if(sysload_copy_table.record[i].type == 0) {	//zero init
			for(j=0; j<sysload_copy_table.record[i].size; des++) {
				*des=0;
				j+=4;
			}
		}
    }

#if cn_cache_used == 1
    cache_clean_data();
    cache_invalid_inst();
#endif
}

//----操作系统内核加载程序-----------------------------------------------------
//功能：加载所有操作系统内核代码，以及在si模式下全部应用程序代码。
//参数: 无。
//返回: 无。
//备注: 本函数移植敏感，与开发系统有关，也与目标硬件配置有关
//----------------------------------------------------------------------------
void pre_start(void)
{
#ifdef debug
        loader();
#endif
        int_init();

        critical();
#ifndef debug
        loader();
#endif
    start_sys();        //开始启动系统
}
