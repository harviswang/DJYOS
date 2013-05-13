//----------------------------------------------------
//Copyright (C), 2004-2011,  都江堰操作系统开发团队.
//版权所有 (C), 2004-2011,   都江堰操作系统开发团队.
//所属模块:用户界面模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 鼠标、键盘等输入设备管理
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-01-24
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 创建文件
//------------------------------------------------------
#if (cfg_stdin_device_limit != 0)
#ifndef __STDIN_DEV_H__
#define __STDIN_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

enum stdin_input_type
{
    enum_char_termineral,
    enum_stdin_keyboard,            //键盘
    enum_stdin_mouse_2d,            //2d鼠标
    enum_stdin_mouse_3d,            //3d鼠标
    enum_stdin_single_touch,        //单点触摸屏
    enum_stdin_muti_touch,          //多点触摸屏
    enum_stdin_area_touch,          //区域触摸屏
};

//字符终端设备数据结构
struct char_termineral_msg
{
    u32 input_char;         //输入的字符
    u32 time;               //输入字符的时间,ticks数
};

//键盘消息数据结构
struct key_board_msg
{
    u8 key_value[3];       //键值,每个键由3部分组成
    u32 time;              //按键事件发生时间,ticks数
};

//2维鼠标数据结构，即目前最常用的鼠标
struct mouse_2D_msg
{
    u16 key_no;             //动作的鼠标键号
    u32 time;               //鼠标事件发生时间,ticks数
};

//3维鼠标数据结构
struct mouse_3D_msg
{
    u32 time;              //按键事件发生时间,ticks数
};

//单点触摸屏，即最常用的触摸屏
struct single_touch_msg
{
    s32 x,y,z;  //x,y表示触摸位置，z>0标志触摸压力，0标志未触摸
    u32 time;              //按键事件发生时间,ticks数
};

//多点触摸屏
struct multi_touch_msg
{
    u32 time;              //按键事件发生时间,ticks数
};

//区域触摸屏，表示被触摸的是一个区域。
struct area_touch_msg
{
    u32 time;              //按键事件发生时间,ticks数
};

union input_device_mem
{
    struct char_termineral_msg char_termineral;
    struct key_board_msg key_board;
    struct mouse_2D_msg  mouse_2d;
    struct mouse_3D_msg  mouse_3d;
    struct single_touch_msg  single_touch_msg;
    struct multi_touch_msg   muti_touch;
    struct area_touch_msg    area_touch;
};

struct input_device_msg
{
    enum stdin_input_type input_type;         //输入消息类型，
    u32 device_id;          //输入设备id
    union input_device_mem input_msg;
};

struct stdin_device_rsc
{
    struct rsc_node stdin_device_node;
    enum stdin_input_type input_type;   //输入设备类型
    u32 device_id;                      //输入设备id
    u32 input_counter;                  //本输入设备累计输入了多少消息
    u16 focus_evtt;                     //接收本消息的事件类型
};

ptu32_t module_init_stdin(ptu32_t para);
s32 stdin_install_device(char *device_name,enum stdin_input_type stdin_type);
bool_t stdin_uninstall_device(char *device_name);
bool_t stdin_set_focus(char *device_name,u16 focus_evtt);
void stdin_set_focus_default(u16 focus_evtt);
void stdin_set_focus_all(u16 focus_evtt);
bool_t stdin_input_msg(s32 stdin_id,u8 *msg_data, u32 msg_size);

#ifdef __cplusplus
}
#endif

#endif //__STDIN_DEV_H__

#endif      //for #if (cfg_stdin_device_limit != 0)

