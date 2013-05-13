//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:内核模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 内核使用的一些声明
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __KERNEL_H_
#define __KERNEL_H_

#ifdef __cplusplus
extern "C"{ 
#endif

//出错enum常量的定义，所有的出错常量被定义为enum，enum的成员是一个整型常量，该
//常量的高8位用来区别出错模块名称，高8位中1~127由系统使用，128~255应用程序使用。
//如下表定义:
#define cn_knl_no_error     (1<<24)         //系统调度起始错误号
#define cn_fs_no_error      (2<<24)         //文件系统起始错误号
#define cn_mem_no_error     (3<<24)         //内存模块起始错误号
#define cn_drv_no_error     (4<<24)         //pan driver模块起始错误号
#define cn_wdt_no_error     (5<<24)         //wdt模块起始错误号
#define cn_gk_no_error      (6<<24)         //gui kernel模块起始错误号
#define cn_lock_no_error    (7<<24)         //锁模块起始错误号

#ifdef __cplusplus
}
#endif

#endif // __KERNEL_H_
