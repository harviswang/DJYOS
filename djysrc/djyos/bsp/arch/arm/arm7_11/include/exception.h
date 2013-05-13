//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:中断模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: cm3异常处理模块
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-08-30
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明:
//------------------------------------------------------
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void (*usr_top)(ufast_t ufl_line);
extern void (*usr_bottom)(ufast_t ufl_line);
extern void (*svc_top)(ufast_t ufl_line);
extern void (*svc_bottom)(ufast_t ufl_line);
extern void (*irq_top)(ufast_t ufl_line);
extern void (*irq_bottom)(ufast_t ufl_line);
extern void (*fiq_top)(ufast_t ufl_line);
extern void (*fiq_bottom)(ufast_t ufl_line);
extern void (*undef_top)(ufast_t ufl_line);
extern void (*undef_bottom)(ufast_t ufl_line);
extern void (*abort_top)(ufast_t ufl_line);
extern void (*abort_bottom)(ufast_t ufl_line);

extern void (* engine_irq)(ufast_t ufl_line);

//系统异常表


void exp_init(void);

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H_

