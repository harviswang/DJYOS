//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:中断模块
//作者：lst
//版本：V1.0.0
//文件描述: cm3异常处理模块
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-08-30
//   作者: lst
//   新版本号: V1.0.0
//   修改说明:
//------------------------------------------------------
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

//系统异常表
#define cn_sysvect_reset            1
#define cn_sysvect_nmi              2
#define cn_sysvect_hard_fault       3
#define cn_sysvect_memmanage_fault  4
#define cn_sysvect_bus_fault        5
#define cn_sysvect_usage_fault      6
#define cn_sysvect_svc              11
#define cn_sysvect_debug            12
#define cn_sysvect_pendsv           14
#define cn_sysvect_systick          15

extern u32 u32g_exp_table[];
void exp_init(void);
void exp_connect_nmi(void (*esr)(void));
void exp_connect_systick(void (*tick)(void));
void exp_esr_tick(void);

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H_

