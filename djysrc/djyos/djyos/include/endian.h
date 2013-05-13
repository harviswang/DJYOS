//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:公共函数
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:公共函数
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#ifdef __cplusplus
extern "C" {
#endif

u32 __pick_little_16bit(u8 *buf,u32 index);
u32 __pick_little_32bit(u8 *buf,u32 index);
void __fill_little_16bit(u8 *buf,u32 index,u32 data);
void __fill_little_32bit(u8 *buf,u32 index,u32 data);
u32 __pick_big_16bit(u8 *buf,u32 index);
u32 __pick_big_32bit(u8 *buf,u32 index);
void __fill_big_16bit(u8 *buf,u32 index,u32 data);
void __fill_big_32bit(u8 *buf,u32 index,u32 data);

#ifdef __cplusplus
}
#endif

#endif // _ENDIAN_H_

