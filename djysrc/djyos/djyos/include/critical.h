//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:安全钩子
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:安全钩子是必须在开机后尽可能快地执行的部分代码
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __CRITICAL_H__
#define __CRITICAL_H__

#ifdef __cplusplus
extern "C" {
#endif
extern const ufast_t tg_int_used[];
extern const ufast_t ufg_int_used_num;
void critical(void);

#ifdef __cplusplus
}
#endif

#endif /*__CRITICAL_H__*/

