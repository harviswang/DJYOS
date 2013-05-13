//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 原始输入输出
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: nude_io模块的头文件
//其他说明:
//修订历史:
//1. 日期:2009-07-22
//   作者:  罗侍田.
//   新版本号：1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __nude_io_h__
#define __nude_io_h__

#ifdef __cplusplus
extern "C" {
#endif

uint8_t scan_byte_nude(void);
bool_t read_byte_nude(uint8_t *byte);
void printf_byte_nude(uint8_t byte);
void printf_str_nude(uint8_t *str);
bool_t nude_terminal_init(void);

#ifdef __cplusplus
}
#endif

#endif //__nude_io_h__

