//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:标准输入--触摸屏模块
//作者:  罗侍田.
//版本：V1.1.0
//文件描述: 触摸屏设备驱动程序
//其他说明:
//修订历史:
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include "os_inc.h"

#if (cfg_single_touch == 1)

#include "Touch.h"
#include "string.h"

s16 s16g_touch_scan_evtt;
struct st_touch_adjust tg_touch_adjust;
static s32 s32g_touch_stdin_id;


//----触摸屏扫描任务-----------------------------------------------------------
//功能: 周期性地调用硬件扫描程序，获取用户用户触摸信息，并把获得的信息输入到标准
//      输入缓冲区(调用函数stdin_input_msg)，注意，抬起触摸笔后，还要发送一条触
//      摸笔离开的信息
//参数: my_event，键盘服务事件类型。
//返回: 无
//----------------------------------------------------------------------------
void touch_scan(void)
{
    struct single_touch_msg now = {0,0,0},old = {0,0,0};
    bool_t getted=false;
    char **display_name;

    //事件参数是触摸屏对应的显示器的名字(字符串)
    display_name = (char**)djy_get_event_para(NULL);
    touch_ratio_adjust((char *)(*display_name),&tg_touch_adjust);

    while(1)
    {
        if(touch_scan_hard(&now) != 0)
        {
            now.time = djy_get_time_tick();
            stdin_input_msg(s32g_touch_stdin_id,(u8*)&now,sizeof(now));
            old = now;
            getted = true;
        }
        else
        {
            if(getted == true)
            {
                now.time = djy_get_time_tick();
                now.z = 0;
                stdin_input_msg(s32g_touch_stdin_id,(u8*)&now,sizeof(now));
                getted = false;
            }
        }
        djy_event_delay(10*mS);
    }
}

//----初始化触摸屏模块---------------------------------------------------------
//功能: 初始化触摸屏模块，包括:安装触摸屏标准输入设备；登记触摸屏扫描事件
//      类型；pop触摸屏扫描事件以启动扫描(实际扫描工作要等多线程启动后才开始)
//参数: 无
//返回: 触摸屏设备号，-1表示安装触摸屏设备失败
//----------------------------------------------------------------------------
ptu32_t module_init_touch(ptu32_t para)
{
    if( !touch_hard_init())
        return -1;
    s32g_touch_stdin_id = stdin_install_device("touch scan",
                            enum_stdin_single_touch);
    if(s32g_touch_stdin_id == -1)
        return -1;
    s16g_touch_scan_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,1,
                            touch_scan,1024,"touch");
    if(s16g_touch_scan_evtt == cn_invalid_evtt_id)
    {
        stdin_uninstall_device("touch scan");
        return -1;
    }
    if(djy_event_pop(s16g_touch_scan_evtt,enum_para_async,
                                        NULL,0,&para,sizeof(ptu32_t),0,0)
                        == (uint16_t)cn_invalid_event_id)
    {
        djy_evtt_unregist(s16g_touch_scan_evtt);
        stdin_uninstall_device("touch scan");
        return -1;
    }
    return s32g_touch_stdin_id;
}

#endif  //for #if (cfg_single_touch == 1)
