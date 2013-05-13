//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 内核模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 预加载操作系统
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>
#include "copy_table.h"

#if cn_cache_used == 1
#include "cache.h"
#endif

void pre_start(void);

extern struct copy_table preload_copy_table;

//----预加载程序---------------------------------------------------------------
//功能：加载主加载器、中断管理模块，紧急代码
//参数: 无。
//返回: 无。
//----------------------------------------------------------------------------
//备注: 本函数移植敏感，与开发系统有关，也与目标硬件配置有关
void load_preload(void)
{
    void (*volatile pl_1st)(void) = pre_start;

    u32 *src,*des;
    u32 i, j;
    for(i=0; i<preload_copy_table.record_cnt; i++) {
		src = (u32*) preload_copy_table.record[i].load_start_address;
		des = (u32*) preload_copy_table.record[i].run_start_address;
		if(preload_copy_table.record[i].type == 1) {	//copy
	    	if(src != des) {
				for(j=0; j<preload_copy_table.record[i].size; src++,des++) {
					*des=*src;
					j+=4;
				}
	    	}
		} else if(preload_copy_table.record[i].type == 0) {	//zero init
			for(j=0; j<preload_copy_table.record[i].size; des++) {
				*des=0;
				j+=4;
			}
		}
    }

#if cn_cache_used == 1
    cache_clean_data();
    cache_invalid_inst();
#endif
    pl_1st();   //用指针做远程调用
}

