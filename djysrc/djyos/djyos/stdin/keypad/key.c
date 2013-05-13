//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:键盘
//作者:  罗侍田.
//版本：V1.1.0
//文件描述: 提供键盘设备管理功能的公共部分，移植需要修改的见key_xxx.c文件
//其他说明:
//修订历史:
//3. 日期:20120324
//   作者:  罗侍田.
//   新版本号：v1.1.0
//   修改说明:
//      1.按照新的stdin模块要求重写
//      2.增加key_read函数和key_read_direct函数
//2. 日期:20090131
//   作者:  罗侍田.
//   新版本号：v1.1.0
//   修改说明:
//      1.把硬件相关的函数key_scan_hard移到key_xxx.c中.
//      2.增加key_read函数和key_read_direct函数
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if cfg_keyboard == 1

#include "key.h"
static s32 s32g_keyboard_stdin_id;
static u16 u16g_evtt_key;
//----初始化键盘模块----------------------------------------------------------
//功能: 初始化键盘模块，包括:安装键盘标准输入设备；登记键盘扫描事件
//      类型；pop键盘扫描事件以启动扫描(实际扫描工作要等多线程启动后才开始)
//参数: 无
//返回: 键盘设备号，-1表示安装键盘设备失败
//----------------------------------------------------------------------------
ptu32_t module_init_keyboard(ptu32_t para)
{
    if( !key_hard_init())
        return -1;
    s32g_keyboard_stdin_id = stdin_install_device("key board",enum_stdin_keyboard);
    if(s32g_keyboard_stdin_id == -1)
        return -1;
    u16g_evtt_key = djy_evtt_regist(enum_correlative,cn_prio_RRS,0,1,
                                    key_scan,512,"keyboard");
    if(u16g_evtt_key == cn_invalid_evtt_id)
    {
        stdin_uninstall_device("key board");
        return -1;
    }
    if(djy_event_pop(u16g_evtt_key,enum_para_async,NULL,0,NULL,0,0,0) 
                        == (uint16_t)cn_invalid_event_id)
    {
        djy_evtt_unregist(u16g_evtt_key);
        stdin_uninstall_device("key board");
        return -1;
    }
    return s32g_keyboard_stdin_id;
}

//----键盘扫描任务-------------------------------------------------------------
//功能: 周期性地调用硬件扫描程序，获取用户敲击信息，合成标准键盘码，送到标准
//      输入设备中。键盘码按照PS/2标准扫描码。扫描码有两种不同的类型：通码
//      (make code)和断码(break code)。当一个键被按下或持续按住时,就产生一个通
//      码;而当一个键被释放时,就产生一个断码。根据键盘按键扫描码的不同,可将按键
//      分为如下几类：
//      第一类键,通码为1字节,断码为0xF0+通码。如A键,通码为0x1C,断码为0xF0 0x1C。
//      第二类键,通码为2字节0xE0+0xXX形式,断码为0xE0+0xF0+0xXX形式。如right ctrl
//              键,其通码为0xE0 0x14,断码为0xE0 0xF0 0x14。
//      第三类特殊按键有两个,print screen键通码为0xE0 0x12 0xE0 0x7C,断码为0xE0
//              0xF0 0x7C 0xE0 0xF0 0x12; pause键通码为0x E1 0x14 0x77 0xE1 0xF0
//              0x14 0xF0 0x77,断码为空。
//      djyos的键盘驱动支持前两类按键，不考虑第三类特殊按键。
//参数: my_event，键盘服务事件类型。
//返回: 无
//注意: 本驱动已经考虑了按键防抖，硬件读按键的函数key_scan_hard可不防抖
//----------------------------------------------------------------------------
void key_scan(void)
{
    struct key_board_msg key_msg;
    uint16_t key_value_now[2]={0,0};
    uint16_t key_value_bak[2]={0,0};
    uint16_t key_value_temp[2]={0,0};
    static ufast_t holding=5;
    ufast_t get_key=0;
    ufast_t key_num,key_num_bak=0;

    while(1)
    {
        djy_event_delay(10*mS);
        key_num = key_scan_hard(key_value_now);

        if((key_value_now[0] != key_value_temp[0])
                ||(key_value_now[1] != key_value_temp[1])
                ||(key_num != key_num_bak))
        {
            holding = 0;
            key_num_bak = key_num;
            key_value_temp[0] = key_value_now[0];
            key_value_temp[1] = key_value_now[1];
        }
        //状态改变后保持相同状态7次就往下走,超过7次不计.
        holding++;
        if(holding < 7)
            continue;
        if(holding > 7)
        {
            holding = 7;
            continue;
        }
        if(key_num == 0)
        {//没有扫描到按键
            if(get_key == 0)
                continue;
            key_msg.time = djy_get_time_tick();
            key_msg.key_value[1] = cn_break_code;
            key_msg.key_value[2] = key_value_bak[0]>>8;
            key_msg.key_value[0] = key_value_bak[0];
            stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            if(get_key == 2)
            {//从有2个按键变成无按键,发出第2个断码
                key_msg.key_value[2] = key_value_bak[1]>>8;
                key_msg.key_value[0] = key_value_bak[1];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }
            get_key = 0;
            continue;
        }

        key_msg.time = djy_get_time_tick();
        if(key_num==1)
        {//扫描到有1个按键
            if(get_key == 0 )
            {//从无按键到有按键,发通码
                key_msg.key_value[1] = key_value_now[0]>>8;
                key_msg.key_value[0] = key_value_now[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 1)
            {//持续有按键, 两次按键必不相同,发送备份按键的断码和新按键的通码
                key_msg.key_value[2] = key_value_bak[0]>>8;
                key_msg.key_value[1] = cn_break_code;
                key_msg.key_value[0] = key_value_bak[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));

                key_msg.key_value[1] = key_value_now[0]>>8;
                key_msg.key_value[0] = key_value_now[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 2)
            {//原来有两个按键,查看哪个按键弹起,发送断码
                if(key_value_bak[0] == key_value_now[0])
                {// key_value_bak[1]键弹起
                    key_msg.key_value[2] = key_value_bak[1]>>8;
                    key_msg.key_value[0] = key_value_bak[1];

                }else
                {// key_value_bak[0]键弹起
                    key_msg.key_value[2] = key_value_bak[0]>>8;
                    key_msg.key_value[0] = key_value_bak[0];
                }
                key_msg.key_value[1] = cn_break_code;
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }
            key_value_bak[0] = key_value_now[0];
            get_key = 1;
        }if(key_num == 2)
        {//扫描到有2个按键
            if(get_key == 0 )
            {//从无按键到有2按键,发2个通码
                key_msg.key_value[1] = key_value_now[0]>>8;
                key_msg.key_value[0] = key_value_now[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));

                key_msg.key_value[1] = key_value_now[1]>>8;
                key_msg.key_value[0] = key_value_now[1];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 1)
            {//从1个按键到有2个按键,发新按键通码
                if(key_value_bak[0] == key_value_now[0])
                {//新按键是 key_value_now[1]
                    key_msg.key_value[1] = key_value_now[1]>>8;
                    key_msg.key_value[0] = key_value_now[1];
                }else
                {//新按键是 key_value_now[0]
                    key_msg.key_value[1] = key_value_now[0]>>8;
                    key_msg.key_value[0] = key_value_now[0];
                }
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 2)
            {//原来有两个按键,查看哪个按键不同,分别发送断码和通码
                if((key_value_bak[0] != key_value_now[0])
                        &&(key_value_bak[0] != key_value_now[1]))
                {// key_value_bak[0]键弹起
                    key_msg.key_value[2] = key_value_bak[0]>>8;
                    key_msg.key_value[0] = key_value_bak[0];
                    key_msg.key_value[1] = cn_break_code;
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
                if((key_value_bak[1] != key_value_now[0])
                        &&(key_value_bak[1] != key_value_now[1]))
                {// key_value_bak[1]键弹起
                    key_msg.key_value[2] = key_value_bak[1]>>8;
                    key_msg.key_value[0] = key_value_bak[1];
                    key_msg.key_value[1] = cn_break_code;
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
                if((key_value_now[0] != key_value_bak[0])
                        &&(key_value_now[0] != key_value_bak[1]))
                {// key_value_now[0]键按下
                    key_msg.key_value[1] = key_value_now[0]>>8;
                    key_msg.key_value[0] = key_value_now[0];
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
                if((key_value_now[1] != key_value_bak[0])
                        &&(key_value_now[1] != key_value_bak[1]))
                {// key_value_now[0]键按下
                    key_msg.key_value[1] = key_value_now[1]>>8;
                    key_msg.key_value[0] = key_value_now[1];
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
            }
            key_value_bak[0] = key_value_now[0];
            key_value_bak[1] = key_value_now[1];
            get_key = 2;
        }
        continue;
    }
}

#endif   //for #if cfg_keyboard == 1

