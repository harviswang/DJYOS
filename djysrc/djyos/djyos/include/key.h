//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:键盘
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 提供键盘设备管理功能
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _KEY_H_
#define _KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

#if cfg_keyboard == 1

#define cn_break_code       0xf0    //键盘断码

//函数定义
ptu32_t module_init_keyboard(ptu32_t para);
void key_scan(void);
bool_t key_hard_init(void);
ufast_t key_scan_hard(uint16_t *key);
#endif   //for #if cfg_keyboard == 1

#ifdef __cplusplus
}
#endif

#endif // _KEY_H_

