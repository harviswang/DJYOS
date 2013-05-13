//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 系统初始化
//作者：lst
//版本：V1.0.0
//文件描述: 片内闪存控制寄存器结构定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-08-21
//   作者: lst
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __INFLASH_H__
#define __INFLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

struct st_inflash_fpec_reg     //at 0x40022000
{
  vu32 ACR;
  vu32 KEYR;
  vu32 OPTKEYR;
  vu32 SR;
  vu32 CR;
  vu32 AR;
  vu32 RESERVED;
  vu32 OBR;
  vu32 WRPR;
};

struct st_inflash_ob_reg         //at 0x1FFFF800
{
  vu16 RDP;
  vu16 USER;
  vu16 Data0;
  vu16 Data1;
  vu16 WRP0;
  vu16 WRP1;
  vu16 WRP2;
  vu16 WRP3;
} ;

#ifdef __cplusplus
}
#endif

#endif /*__INFLASH_H__*/


