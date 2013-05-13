//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:泛设备管理模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:提供泛设备管理功能
//其他说明:
//修订历史:
//2. 日期:2009-03-03
//   作者:  罗侍田.
//   新版本号：1.0.1
//   修改说明: 修正了dev_close_left函数和dev_close_right函数的错误，该bug由
//             网友sniper提交
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#include <string.h>

static struct  pan_device *pg_device_rsc_tree;

static struct pan_device tg_mem_of_device[cn_device_limit];//泛设备控制块内存池
static struct mem_cell_pool *pg_pan_device_pool;     //设备控制块内存池头指针
static struct mutex_LCB tg_dev_mutex;       //保护设备资源树的并发访问安全

//----初始化泛设备驱动---------------------------------------------------------
//功能：1.在资源管理链中增加名为"dev"的根结点.
//      2.初始化泛设备控制块内存池
//      3.初始化泛设备句柄内存池
//参数：无
//返回：1
//-----------------------------------------------------------------------------
ptu32_t module_init_driver(ptu32_t para)
{
    static struct  pan_device root;

    pg_device_rsc_tree = (struct  pan_device *)
            rsc_add_tree_root(&root.node,sizeof(struct  pan_device),"dev");
    //初始化泛设备控制块内存池
    pg_pan_device_pool = mb_create_pool((void*)tg_mem_of_device,
                                    cn_device_limit,
                                    sizeof(struct  pan_device),
                                    0,0,
                                    "泛设备控制块池");
    mutex_create_knl(&tg_dev_mutex,"device tree");
    return 1;
}

//----添加设备-----------------------------------------------------------------
//功能: 新设备加入到兄弟设备的尾部,
//参数: parent_device,待添加设备的父设备
//      name，新设备的名字，所指向的字符串内存区不能是局部变量，也不能是空
//      right_semp，设备右手接口信号量指针
//      left_semp，设备左手接口信号量指针
//      right_write，右手写函数指针
//      right_read，右手读函数指针
//      right_ctrl，右手控制函数指针
//      left_write，左手写函数指针
//      left_read，左手读函数指针
//      left_ctrl，左手控制函数指针
//      tag,相应设备的驱动给定的一个数，常指向该设备特征数据结构
//返回:新加入的设备的资源结点指针.资源不足则返回NULL
//本函数由泛设备driver作者调用，不建议应用程序使用，故不使用djy_handle_t
//而是用struct  pan_device类型参数
//-----------------------------------------------------------------------------
struct pan_device * dev_add_device(struct pan_device * parent_device,
                                   char               *name,
                                   struct semaphore_LCB   *right_semp,
                                   struct semaphore_LCB   *left_semp,
                                   dev_write_func     right_write ,
                                   dev_read_func      right_read,
                                   dev_ctrl_func      right_ctrl ,
                                   dev_write_func     left_write ,
                                   dev_read_func      left_read ,
                                   dev_ctrl_func      left_ctrl ,
                                   ptu32_t            tag)
{
    struct  pan_device  *new_device,*result;
    if((parent_device == NULL) || (name == NULL))
        return NULL;        //设备不能没有名字
    if(strchr(name,'\\'))   //名字中不能包含字符 \.
        return NULL;
    mutex_pend(&tg_dev_mutex,cn_timeout_forever);
    if(rsc_search_son(&parent_device->node,name) != NULL)
    {
        djy_error_login(enum_drv_homonymy,"设备重名");
        result = NULL;
    }else
    {
        if(parent_device != NULL)
        {
            //分配泛设备控制块给新设备
            new_device = mb_malloc(pg_pan_device_pool,0);
            if(new_device != NULL)
            {
                rsc_add_son(&parent_device->node,&new_device->node,
                                        sizeof(struct  pan_device),name);
                new_device->left_semp = left_semp;
                new_device->right_semp = right_semp;
                if(right_write != NULL)
                    new_device->right_io.io_write = right_write;
                else
                    new_device->right_io.io_write=(dev_write_func)NULL_func;
                if(right_read != NULL)
                    new_device->right_io.io_read = right_read;
                else
                    new_device->right_io.io_read = (dev_read_func)NULL_func;
                if(right_ctrl != NULL)
                    new_device->right_io.io_ctrl = right_ctrl;
                else
                    new_device->right_io.io_ctrl = (dev_ctrl_func)NULL_func;
                if(left_write != NULL)
                    new_device->left_io.io_write = left_write;
                else
                    new_device->left_io.io_write =(dev_write_func)NULL_func;
                if(left_read != NULL)
                    new_device->left_io.io_read = left_read;
                else
                    new_device->left_io.io_read = (dev_read_func)NULL_func;
                if(left_ctrl != NULL)
                    new_device->left_io.io_ctrl = left_ctrl;
                else
                    new_device->left_io.io_ctrl = (dev_ctrl_func)NULL_func;
                new_device->private_tag = tag;
                result = new_device;
            }else
            {
                djy_error_login(enum_mem_tried,"内存不足");
                result = NULL;
            }
        }else
            result = NULL;
    }
    mutex_post(&tg_dev_mutex);
    return result;
}

//----卸载设备------------------------------------------------------------------
//功能: 把设备从设备链表中删除,并释放其占用的内存.该设备需符合以下条件:
//      1.该设备没有子设备.
//      2.正在使用该设备的用户数为0.
//参数: device,待释放的设备
//返回: 成功释放返回true,否则返回false
//与对称函数dev_add_device一样，本函数由泛设备driver作者调用，不建议应用程序
//使用，故不使用struct  dev_handle而是用struct  pan_device类型参数
//-----------------------------------------------------------------------------
bool_t dev_delete_device(struct pan_device * device)
{
    struct  rsc_node *node;
    bool_t en_del = true;
    mutex_pend(&tg_dev_mutex,cn_timeout_forever);
    if(device->left_semp != NULL)
        if(semp_query_used(device->left_semp) != 0)
            en_del = false;
    if(device->right_semp != NULL)
        if(semp_query_used(device->right_semp) != 0)
            en_del = false;
    if(en_del)
    {
        node = &device->node;
        if(rsc_del_node(node) != NULL)
        {
            mutex_post(&tg_dev_mutex);
            mb_free(pg_pan_device_pool,device);
            return(true);           //成功卸载设备
        }
    }
    mutex_post(&tg_dev_mutex);
    djy_error_login(enum_drv_dev_del,NULL);
    return false;
}

//----取根设备指针-------------------------------------------------------------
//功能: 获取根设备ID，实际上就是设备的pan_device指针，但并不打开，事实上根设备也
//      无法打开。
//参数: 无
//返回: 根设备的泛设备指针
//-----------------------------------------------------------------------------
struct pan_device *dev_get_root_device(void)
{
    return (djy_handle_t)pg_device_rsc_tree;
}

//----查询设备-----------------------------------------------------------------
//功能: 查询设备，若存在返回设备的ID，但并不打开设备，不存在则返回0
//参数: name，设备名字，包含路径名，但不必包含'dev\'这4个字符
//返回: 设备存在则返回ID，不存在则返回0
//-----------------------------------------------------------------------------
device_id_t dev_check_device(char * name)
{
    struct  pan_device     *pan = NULL;
    if( ! mutex_pend(&tg_dev_mutex,0))    //这是保护设备树的互斥量
        return NULL;
    //在设备树中搜索设备
    pan = (struct  pan_device *)rsc_search(&pg_device_rsc_tree->node,name);
    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan;
}

//----查询后代设备-------------------------------------------------------------
//功能: 某设备的后代设备中，查询名字为scion_name的设备是否存在，存在则返回其设
//      备ID，但并不打开，不存在则返回0.
//参数: ancestor，被查询设备的祖先设备。
//      scion_name,设备名字符串,包含路径名,
//返回: 设备存在则返回ID，不存在则返回0
//-----------------------------------------------------------------------------
device_id_t dev_check_device_scion(djy_handle_t ancestor,
                                           char * scion_name)
{
    struct  pan_device *pan_scion;
    if(ancestor == NULL)
        return NULL;
    if( ! mutex_pend(&tg_dev_mutex,0))    //这是保护设备树的互斥量
        return NULL;
    //在设备树中搜索设备
    pan_scion = (struct pan_device*)rsc_search(&ancestor->node,scion_name);

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan_scion;
}

//----打开设备左手接口---------------------------------------------------------
//功能: 根据设备名打开设备的左手接口，搜索整个设备资源树，找到名称与name匹配的
//      资源结点，然后从泛设备句柄池中分配控制块,把左手接口指针赋值给句柄后,
//      返回该句柄指针。
//参数: name,设备名字符串,包含路径名，但不必包含'dev\'这4个字符
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: 成功打开设备(含经过等待后打开)返回设备句柄,否则返回NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_left(char *name,u32 timeout)
{
    struct  pan_device     *pan;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //这是保护设备树的互斥量
        return NULL;
    //在设备树中搜索设备
    pan = (struct  pan_device *)rsc_search(&pg_device_rsc_tree->node,name);
    if(pan != NULL)     //如果没有找到name设备,返回空
    {
        if(semp_pend(pan->left_semp,timeout)==false)//获取信号量,这是保护设备的
            pan = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan;
}

//----打开后代设备左手接口-----------------------------------------------------
//功能: 打开后代设备的左手接口，搜索ancestor设备的整个子设备树，找到名称与
//      scion_name匹配的资源结点，然后从泛设备句柄池中分配控制块,把左手接口指针
//      赋值给句柄后,返回该句柄指针。
//参数: ancestor，被打开设备的祖先设备。
//      scion_name,设备名字符串,包含路径名,
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: 成功打开设备(含经过阻塞后打开)返回设备句柄,否则返回NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_left_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout)
{
    struct  pan_device     *pan_scion;
    if(ancestor == NULL)
        return NULL;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //这是保护设备树的互斥量
        return NULL;
    //在设备树中搜索设备
    pan_scion = (struct pan_device*)rsc_search(&ancestor->node,scion_name);

    if(pan_scion != NULL)     //如果没有找到name设备,返回空
    {
        if(semp_pend(pan_scion->left_semp,timeout)==false)//获取保护设备的信号量
            pan_scion = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan_scion;
}

//----快速打开设备左手接口-----------------------------------------------------
//功能: 快速打开设备左手接口,用设备ID为参数，不用按设备名查找设备,速度很快。本函
//      数与dev_check_device或dev_check_device_scion函数以及dev_close_xxx函数配
//      合使用，可实现设备快速打开，及时关闭，不长期占据设备。
//参数: device_id,待打开的设备id
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: 成功打开设备则返回句柄,否则返回NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_left_fast(device_id_t device_id,u32 timeout)
{
    if (device_id == NULL)                                 //句柄空
        return NULL;
    if(semp_pend(device_id->left_semp,timeout)==false)     //获取信号量
    {
        return NULL;
    }else
    {
        return device_id;
    }
}

//----打开设备右手接口---------------------------------------------------------
//功能: 根据设备名打开设备的右手接口，搜索整个设备资源树，找到名称与name匹配的
//      资源结点，然后从泛设备句柄池中分配控制块,把右手接口指针赋值给句柄后,
//      返回该句柄指针。
//参数: name,设备名字符串,包含路径名，但不包含'dev\'这4个字符
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: 成功打开设备返回设备句柄,否则返回NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_right(char *name,u32 timeout)
{
    struct  pan_device     *pan;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //这是保护设备树的互斥量
        return NULL;
    //在设备树中搜索设备
    pan = (struct  pan_device *)rsc_search(&pg_device_rsc_tree->node,name);
    if(pan != NULL)     //如果没有找到name设备,返回空
    {
        if(semp_pend(pan->right_semp,timeout)==false)//获取信号量,这是保护设备的
            pan = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan;
}

//----打开后代设备右手接口-----------------------------------------------------
//功能: 打开后代设备的右手接口，搜索ancestor设备的整个子设备树，找到名称与
//      scion_name匹配的资源结点，然后从泛设备句柄池中分配控制块,把右手接口指针
//      赋值给句柄后,返回该句柄指针。
//参数: ancestor，被打开设备的祖先设备。
//      scion_name,设备名字符串,包含路径名,
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: 成功打开设备(含经过阻塞后打开)返回设备句柄,否则返回NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_right_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout)
{
    struct  pan_device     *pan_scion;
    if(ancestor == NULL)
        return NULL;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //这是保护设备树的互斥量
        return NULL;
    //在设备树中搜索设备
    pan_scion = (struct pan_device*)rsc_search(&ancestor->node,scion_name);

    if(pan_scion != NULL)     //如果没有找到name设备,返回空
    {
        if(semp_pend(pan_scion->right_semp,timeout)==false)//获取保护设备的信号量
            pan_scion = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan_scion;
}

//----快速打开设备左手接口-----------------------------------------------------
//功能: 快速打开设备左手接口,用设备ID为参数，不用按设备名查找设备,速度很快。本函
//      数与dev_check_device或dev_check_device_scion函数以及dev_close_xxx函数配
//      合使用，可实现设备快速打开，及时关闭，不长期占据设备。
//参数: device_id,待打开的设备id
//      timeout，超时设置,单位是微秒，cn_timeout_forever=无限等待，0则立即按
//      超时返回。非0值将被向上调整为cn_tick_us的整数倍
//返回: 成功打开设备则返回句柄,否则返回NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_right_fast(device_id_t device_id,u32 timeout)
{
    if (device_id == NULL)                                 //句柄空
        return NULL;
    if(semp_pend(device_id->right_semp,timeout)==false)     //获取信号量
    {
        return NULL;
    }else
    {
        return device_id;
    }
}

//----关闭设备左手接口---------------------------------------------------------
//功能: 关闭设备左手接口
//参数: handle,被关闭的设备句柄
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t dev_close_left(djy_handle_t handle)
{
    if (handle == NULL)
        return false;
    semp_post(handle->left_semp);
    return true;
}

//----关闭设备右手接口---------------------------------------------------------
//功能: 关闭设备右手接口
//参数: handle,被关闭的设备句柄
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t dev_close_right(djy_handle_t handle)
{
    if (handle == NULL)
        return false;
    semp_post(handle->right_semp);
    return true;
}

//----左手读设备函数-----------------------------------------------------------
//功能: 从左手侧调用设备的读函数读取数据.
//参数: left_handle,设备左手句柄
//      其他参数:由设备开发者定义
//返回: 由设备开发者定义，推荐是实际读取的数据长度.
//-----------------------------------------------------------------------------
ptu32_t dev_read_left(djy_handle_t left_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (left_handle == NULL)
        return 0;
    else
    {
        return (left_handle->left_io.io_read(left_handle,src_buf,des_buf,len));
    }
}

//----右手读设备函数-----------------------------------------------------------
//功能: 从右手侧调用设备的读函数读取数据.
//参数: right_handle,设备右手句柄
//      其他参数:由设备开发者定义
//返回: 由设备开发者定义，推荐是实际读取的数据长度.
//-----------------------------------------------------------------------------
ptu32_t dev_read_right(djy_handle_t right_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (right_handle == NULL)
        return 0;
    else
    {
        return (right_handle->right_io.io_read(right_handle,src_buf,des_buf,len));
    }
}

//----左手写设备函数-----------------------------------------------------------
//功能: 从左手侧调用设备的写函数写入数据.
//参数: left_handle,设备左手句柄
//      其他参数:由设备开发者定义
//返回: 由设备开发者定义，推荐是实际写入的数据长度.
//-----------------------------------------------------------------------------
ptu32_t dev_write_left(djy_handle_t left_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (left_handle == NULL)
        return 0;
    else
    {
        return (left_handle->left_io.io_write(left_handle,src_buf,des_buf,len));
    }
}

//----右手写设备函数-----------------------------------------------------------
//功能: 从右手侧调用设备的写函数写入数据.
//参数: right_handle,设备右手句柄
//      其他参数:由设备开发者定义
//返回: 由设备开发者定义，推荐是实际写入的数据长度.
//-----------------------------------------------------------------------------
ptu32_t dev_write_right(djy_handle_t right_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (right_handle == NULL)
        return 0;
    else
    {
        return (right_handle->right_io.io_write(right_handle,src_buf,des_buf,len));
    }
}

//----左手控制设备函数---------------------------------------------------------
//功能: 从左手侧调用设备的控制函数.
//参数: left_handle,设备句柄
//      其他参数:由设备开发者定义
//返回: 由设备开发者定义
//-----------------------------------------------------------------------------
ptu32_t dev_ctrl_left(djy_handle_t left_handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2)
{
    if (left_handle == NULL)
        return 0;
    else
    {
        return (left_handle->left_io.io_ctrl(left_handle,cmd,data1,data2));
    }
}

//----右手控制设备函数---------------------------------------------------------
//功能: 从右手侧调用设备的控制函数.
//参数: right_handle,设备右手句柄
//      其他参数:由设备开发者定义
//返回: 由设备开发者定义
//-----------------------------------------------------------------------------
ptu32_t dev_ctrl_right(djy_handle_t right_handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2)
{
    if (right_handle == NULL)
        return 0;
    else
    {
        return (right_handle->right_io.io_ctrl(right_handle,cmd,data1,data2));
    }
}

//----提取设备标签-------------------------------------------------------------
//功能: 每个设备都有一个标签，该标签一般是一个指向描述该设备的数据结构指针。本函
//      数一般由设备驱动程序调用，而设备驱动程序往往只知道自己的泛设备指针，而不
//      知道句柄，因此本函数用struct pan_device为参数
//参数: pan,设备指针，注意不是句柄
//返回: 设备标签
//-----------------------------------------------------------------------------
ptu32_t dev_get_my_tag(struct pan_device * pan)
{
    if (pan == NULL)
        return 0;
    else
    {
        return pan->private_tag;
    }
}


//----获取设备指针-------------------------------------------------------------
//功能: 通过设备handle逆向查找设备的泛设备指针。
//参数: handle,设备句柄
//返回: 泛设备设备指针
//-----------------------------------------------------------------------------
struct pan_device *dev_get_panp(djy_handle_t handle)
{
    return (struct pan_device *)handle;
};

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))


