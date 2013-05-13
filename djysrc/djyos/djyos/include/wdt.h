//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 看门狗模块
//作者：lst
//版本：V1.0.0
//文件描述: 看门狗模块
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __wdt_h__
#define __wdt_h__

#ifdef __cplusplus
extern "C" {
#endif

#define cn_wdt_action_none          0 //没有行动(一般是yip_remedy函数已经足够的情况)
#define cn_wdt_action_reboot        1 //复位计算机
#define cn_wdt_action_hard_reset    2


//看门狗结构
struct wdt_rsc
{
	struct rsc_node wdt_node;
    struct semaphore_LCB wdt_semp;
    bool_t (* judge)(void);         //用户提供的判断狗叫的函数
    u32 (* yip_remedy)(void);  //狗叫后的补救措施，用户提供
    u32 timeout;               //调用judge的时间间隔
};


//出错信息定义
enum wdt_error_code
{
    enum_wdt_no_error = cn_wdt_no_error,  //没有错误
    enum_wdt_yip,                         //狗叫
};

ptu32_t module_init_wdt(ptu32_t para);
struct wdt_rsc * wdt_create(bool_t (*judge)(void),
                  u32 (*yip_remedy)(void),
                  u32 timeout,char *wdt_name);
void wdt_check(void);

#ifdef __cplusplus
}
#endif

#endif //__wdt_h__

