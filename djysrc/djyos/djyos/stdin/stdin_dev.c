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
#include "os_inc.h"

#if (cfg_stdin_device_limit != 0)

//事件类型，所有未设置焦点的输入设备，默认以此为焦点
u16 u16g_default_focus = cn_invalid_evtt_id;
struct stdin_device_rsc stdin_device[cfg_stdin_device_limit];
struct mem_cell_pool *pg_stdin_device_pool;
s32 s32g_next_id = 0;       //每次安装输入设备时，以本变量为设备ID，然后本变量++
                            //删除设备，ID不收回。
static struct  stdin_device_rsc *pg_stdin_device_rsc_tree;

//----初始化标准输入模块-------------------------------------------------------
//功能: 初始化标准输入模块，执行初始化后，可以创建输入设备。
//参数: 无
//返回: 总是返回1
//-----------------------------------------------------------------------------
ptu32_t module_init_stdin(ptu32_t para)
{
    //本函数完成的功能:
    //创建"input device"资源
    //创建pg_stdin_device_pool内存池
    static struct  stdin_device_rsc root;

    pg_stdin_device_rsc_tree = (struct  stdin_device_rsc *)
                        rsc_add_tree_root(&root.stdin_device_node,
                        sizeof(struct  stdin_device_rsc),"stdin input device");
    //初始化泛设备控制块内存池
    pg_stdin_device_pool = mb_create_pool((void*)stdin_device,
                                    cfg_stdin_device_limit,
                                    sizeof(struct  stdin_device_rsc),
                                    0,0,
                                    "输入设备控制块内存池");

    return 1;
}

//----安装输入设备-------------------------------------------------------------
//功能: 把输入设备登记到资源结点中
//参数: device_name，目标设备的资源名
//返回: 输入设备ID,-1表示失败
//-----------------------------------------------------------------------------
s32 stdin_install_device(char *device_name,enum stdin_input_type stdin_type)
{
    struct stdin_device_rsc *stdin;

    if(rsc_search_son(&pg_stdin_device_rsc_tree->stdin_device_node,device_name))
    {
        return -1;
    }
    else
    {
        stdin = mb_malloc(pg_stdin_device_pool,0);
        if(stdin != NULL)
        {
            rsc_add_son(&pg_stdin_device_rsc_tree->stdin_device_node,
                    &stdin->stdin_device_node,sizeof(struct stdin_device_rsc),
                    device_name);
            stdin->input_type = stdin_type;
            stdin->input_counter = 0;
            stdin->focus_evtt = u16g_default_focus;
            s32g_next_id += 1;
            stdin->device_id = s32g_next_id;
            return stdin->device_id;
        }
        else
        {
            djy_error_login(enum_mem_tried,"内存不足");
            return -1;
        }

    }
}

//----设置输入焦点---------------------------------------------------------
//功能: 设置输入设备的输入焦点，即把输入消息(例如键盘击键、鼠标点击)传给谁
//参数: device_name，目标设备的资源名
//      focus_evtt，新的焦点
//返回: true =成功，false=输入设备找不到
//-----------------------------------------------------------------------------
bool_t stdin_set_focus(char *device_name,u16 focus_evtt)
{
    struct stdin_device_rsc *result;

    result =
        (struct stdin_device_rsc *)rsc_search_son
        (&pg_stdin_device_rsc_tree->stdin_device_node,device_name);
    if(result != NULL)
    {
        result->focus_evtt = focus_evtt;
        return true;
    }
    else
    {
        return false;
    }
}

//----设置默认输入焦点---------------------------------------------------------
//功能: 所有输入设备，都有焦点，即设备有数据输入时，给哪个事件类型发消息。未设定
//      焦点的设备，使用默认焦点，如果默认焦点是cn_invalid_evtt_id，则未设置焦点
//      的设备便没有焦点
//参数: focus_evtt，默认焦点，
//返回: 无
//-----------------------------------------------------------------------------
void stdin_set_focus_default(u16 focus_evtt)
{
    u16g_default_focus = focus_evtt;
}

//----设置所有设备输入焦点-----------------------------------------------------
//功能: 把所有输入设备的焦点设为指定值
//参数: focus_evtt，新的焦点焦点
//返回: 无
//-----------------------------------------------------------------------------
void stdin_set_focus_all(u16 focus_evtt)
{
    //在这里遍历所有输入设备，即"input device"资源的子孙资源，设置他们的focus_evtt
    struct  rsc_node  *current,*start,*target;
    start = &pg_stdin_device_rsc_tree->stdin_device_node;
    current = &pg_stdin_device_rsc_tree->stdin_device_node;
    while((target =
        rsc_trave_scion(start,current))!=NULL)
    {
        ((struct stdin_device_rsc *)target)->focus_evtt = focus_evtt;
        current = target;
    }
}

//----输入消息-----------------------------------------------------------------
//功能: 输入设备获得新的输入后(例如新的键盘击键、鼠标点击等)，调用本函数，把新的
//      消息传入系统。弹出事件，如果in_device->focus_evtt已初始化，则事件类型是
//      in_device->focus_evtt，如果未初始化，则事件类型是u16g_default_focus，
//      如果u16g_default_focus也没有初始化，则不弹出事件
//参数: in_device，目标输入设备的资源接点
//      msg_data，包含输入的数据
//      msg_size，输入消息的长度，字节数
//返回: true=成功，false=失败，一般是因为输入设备未安装。
//-----------------------------------------------------------------------------
bool_t stdin_input_msg(s32 stdin_id,u8 *msg_data, u32 msg_size)
{
    struct  rsc_node  *current,*start,*target;
    struct input_device_msg input_msg;
    u16 evtt;

    start = &pg_stdin_device_rsc_tree->stdin_device_node;
    current = &pg_stdin_device_rsc_tree->stdin_device_node;

    //在资源队列中查找stdin_id对应的输入设备
    while((target = rsc_trave_scion(start,current))!=NULL)
    {
        if(((struct stdin_device_rsc *)target)->device_id == stdin_id)
            break;
        current = target;
    }
    if(target == NULL)        //未找到输入设备
        return false;

    evtt = ((struct stdin_device_rsc *)target)->focus_evtt;
    if(evtt == cn_invalid_evtt_id)      //该设备的输入焦点未初始化
    {
        evtt = u16g_default_focus;      //取默认焦点(可能也未初始化)
    }

    if(evtt != cn_invalid_evtt_id)
    {
        input_msg.input_msg = *(union input_device_mem *)msg_data;
        input_msg.input_type = ((struct stdin_device_rsc *)target)->input_type;
        input_msg.device_id = ((struct stdin_device_rsc *)target)->device_id;
        djy_event_pop(evtt,enum_para_async,NULL,0,
                        &input_msg,sizeof(input_msg),0,0);
    }
	return true;
}

//----卸载输入设备-------------------------------------------------------------
//功能: 把输入设备从资源接点删除，从"input device"资源的子资源中，找到
//      "device_name"资源接点，删除之，并把内存释放到pg_stdin_device_pool
//      内存池中
//参数: device_name，目标设备的资源名
//返回: true = 成功删除，0=失败。
//-----------------------------------------------------------------------------
bool_t stdin_uninstall_device(char *device_name)
{
	struct  rsc_node *temp;
    struct stdin_device_rsc *stdin;
	stdin = (struct stdin_device_rsc *)rsc_search_son(
                 &pg_stdin_device_rsc_tree->stdin_device_node,device_name);
    if(stdin == NULL)
        return false;

    temp = &stdin->stdin_device_node;
    if(rsc_del_node(temp) != NULL)
    {
        mb_free(pg_stdin_device_pool,stdin);
        return true;
    }
    else
    	return false;
}

#endif      //for #if (cfg_stdin_device_limit != 0)

