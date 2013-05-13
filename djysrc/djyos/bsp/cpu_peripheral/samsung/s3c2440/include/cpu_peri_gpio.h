//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//版权所有 (C), 2005-2009,   lst.
//所属模块:IO初始化
//作者：lst
//版本：V1.0.0
//文件描述:IO初始化
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-006-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __gpio_h_
#define __gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "os_inc.h"

struct gpio_reg         //地址在0x56000000
{
    u32 GPACON;     //0x56000000
    u32 GPADAT;     //0x56000004
    u32 rev1;      //0x56000008
    u32 rev2;      //0x5600000c
    u32 GPBCON;     //0x56000010
    u32 GPBDAT;     //0x56000014
    u32 GPBUP ;     //0x56000018
    u32 rev4;      //0x5600001c
    u32 GPCCON;     //0x56000020
    u32 GPCDAT;     //0x56000024
    u32 GPCUP ;     //0x56000028
    u32 rev5;      //0x5600002c
    u32 GPDCON;     //0x56000030
    u32 GPDDAT;     //0x56000034
    u32 GPDUP ;     //0x56000038
    u32 rev6;      //0x5600003c
    u32 GPECON;     //0x56000040
    u32 GPEDAT;     //0x56000044
    u32 GPEUP ;     //0x56000048
    u32 rev7;      //0x5600001c
    u32 GPFCON;     //0x56000050
    u32 GPFDAT;     //0x56000054
    u32 GPFUP ;     //0x56000058
    u32 rev8;      //0x5600005c
    u32 GPGCON;     //0x56000060
    u32 GPGDAT;     //0x56000064
    u32 GPGUP ;     //0x56000068
    u32 rev9;      //0x5600006c
    u32 GPHCON;     //0x56000070
    u32 GPHDAT;     //0x56000074
    u32 GPHUP ;     //0x56000078
    u32 rev10;      //0x5600007c
    u32 MISCCR;     //0x56000080
    u32 DCLKCON;     //0x56000084
    u32 EXTINT0;     //0x56000088
    u32 EXTINT1;      //0x5600008c
    u32 EXTINT2;      //0x56000090
    u32 EINTFLT0;     //0x56000094
    u32 EINTFLT1;     //0x56000098
    u32 EINTFLT2;      //0x5600009c
    //u32 EINTFLT3;      //0x4C0000a0???
    u32 rev11;			//0x560000a0
    u32 EINTMASK;		//0X560000a4
    u32 EINTPEND;		//0X560000a8
};

#ifdef __cplusplus
}
#endif

#endif /*__gpio_h_*/
