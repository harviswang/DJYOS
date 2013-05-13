//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:泛设备管理模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:提供泛设备管理功能
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __driver_H__
#define __driver_H__

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#define cn_dev_name_limit   255     //设备名长度不能超过255字符
typedef struct pan_device * djy_handle_t;
typedef struct pan_device * device_id_t;
struct  pan_device;
struct  rsc_node;

//泛设备模块出错代码，本enum常量从enum_drv_no_error开始依序增1.
enum drv_error_code
{
    enum_drv_no_error = cn_drv_no_error,  //没有错误
    enum_drv_handle_error,                //句柄错误
    enum_drv_homonymy,                    //设备重名错误
    enum_drv_dev_del,                   //删除设备失败
};

typedef ptu32_t (*dev_write_func)(struct  pan_device *dev,ptu32_t src_buf,
                                 ptu32_t des_buf,ptu32_t len);
typedef ptu32_t (*dev_read_func) (struct  pan_device *dev,ptu32_t src_buf,
                                 ptu32_t des_buf,ptu32_t len);
typedef ptu32_t (*dev_ctrl_func) (struct  pan_device *dev,u32 cmd,
                                ptu32_t data1,ptu32_t data2);
//从设备的输入输出结构,左右手各一个
struct dev_io
{
    //所有6个（左右手各三个，故6个）io函数中,操作系统并不限定函数参数的类型
    //含义,而是由设备性质决定,如果src_buf是结构数组,len可能是结构体数量,如果
    //src_buf是通信缓冲区,则len可能是完整数据包数量.总之,src_buf和len的类型应该
    //由driver作者根据本身的特性决定,这里的定义只是推荐性质的.
    //微言:src_buf和des_buf被定义成ptu32_t而不是指针,是从移植兼容性出发考虑的.
    //指针是不确定长度的数据类型,这会带来移植方面的困难.有许多系统,尤其是16位系统,
    //指针的宽度是16位的,甚至有24位的(keilC51中就有可能).当应用程序要把参数当作
    //整数使用时,将面临不知道参数的长度的问题.反过来,除64位系统外,所有的系统指针
    //长度都小于32位,做类型转换没有问题.
    //返回值:由设备特性决定.
    dev_write_func io_write;
    dev_read_func  io_read;
    dev_ctrl_func  io_ctrl;
};

//设备端口分为左端口和右端口,用户使用设备时,须严格区分使用左端口还是右端口,
//一般要象管理事件类型那样,项目中要专门管理.
struct pan_device
{
    struct  rsc_node  node;
    struct  dev_io left_io;
    struct  dev_io right_io;
    struct semaphore_LCB *left_semp; //信号量,控制设备被从左手同时打开的次数，
    struct semaphore_LCB *right_semp; //信号量,控制设备被从右手同时打开的次数，
    ptu32_t private_tag;              //本设备特有的数据结构,
};

ptu32_t module_init_driver(ptu32_t para);
struct  pan_device *dev_add_device(struct  pan_device    *parent_device,
                                   char                  *name,
                                   struct semaphore_LCB  *right_semp,
                                   struct semaphore_LCB  *left_semp,
                                   dev_write_func        right_write ,
                                   dev_read_func         right_read,
                                   dev_ctrl_func         right_ctrl ,
                                   dev_write_func        left_write ,
                                   dev_read_func         left_read ,
                                   dev_ctrl_func         left_ctrl ,
                                   ptu32_t               tag);
bool_t dev_delete_device(struct  pan_device * device);
struct pan_device * dev_get_root_device(void);
struct pan_device * dev_check_device(char * name);
struct pan_device * dev_check_device_scion(djy_handle_t ancestor,char * scion_name);
djy_handle_t dev_open_left(char *name,u32 timeout);
djy_handle_t dev_open_left_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout);
djy_handle_t dev_open_left_fast(djy_handle_t handle,u32 timeout);
djy_handle_t dev_open_right(char *name,u32 timeout);
djy_handle_t dev_open_right_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout);
djy_handle_t dev_open_right_fast(djy_handle_t handle,u32 timeout);
bool_t dev_close_left(djy_handle_t handle);
bool_t dev_close_right(djy_handle_t handle);
ptu32_t dev_read_left(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_write_left(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_ctrl_left(djy_handle_t handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2);
ptu32_t dev_read_right(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_write_right(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_ctrl_right(djy_handle_t handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2);
struct pan_device *dev_get_panp(djy_handle_t handle);
ptu32_t dev_get_my_tag(struct pan_device * pan);

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#ifdef __cplusplus
}
#endif
#endif //__driver_H___
