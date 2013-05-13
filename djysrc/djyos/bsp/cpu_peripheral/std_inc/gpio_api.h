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
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __gpio_api_h_
#define __gpio_api_h_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_invalid_port     (-1)        //非gpio引脚
void gpio_cfg_pin_func(u32 port,u32 pinnum,u32 func_no);
void gpio_cfg_pin_mode(u32 port,u32 pinnum,u32 mode);
void gpio_setto_in(u32 port,u32 msk);
void gpio_setto_out(u32 port,u32 msk);
u32 gpio_get_data(u32 port);
void gpio_out_data(u32 port,u32 data);
void gpio_setto_high(u32 port,u32 msk);
void gpio_setto_low(u32 port,u32 msk);
void gpio_power_on(u32 port);
void gpio_power_off(u32 port);

#ifdef __cplusplus
}
#endif

#endif /*__gpio_api_h_*/
