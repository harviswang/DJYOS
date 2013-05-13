//----------------------------------------------------
//Copyright (C), 2004-2010,  luoshitian.
//版权所有 (C), 2004-2010,   罗侍田.
//所属模块: 锁模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 管道模块
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2010-10-01
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

static struct  pan_device *pg_pipe_parent_device;
#if(cn_pipes_limit != 0)
static struct pipe_PCB tg_pipe_mem_block[cn_pipes_limit];
#endif
static struct mem_cell_pool *pg_pipe_pool;  //信号量结构内存池头指针

//----初始化pipe模块模块-------------------------------------------------------
//功能：在"dev"设备下创建"pipe"设备，作为后续创建pipe设备的父设备，这是一个哑
//      设备，无任何具体的功能。
//参数：para，调用者传入的参数，本模块未使用
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_pipe(ptu32_t para)
{
    static struct mem_cell_pool pipe_pool;
    
    pg_pipe_parent_device = dev_add_device( dev_get_root_device(),"pipe",
                                 NULL,NULL,      //无信号量保护
                                 (dev_write_func) NULL_func ,
                                 (dev_read_func ) NULL_func,
                                 (dev_ctrl_func ) NULL_func ,
                                 (dev_write_func ) NULL_func ,
                                 (dev_read_func  ) NULL_func ,
                                 (dev_ctrl_func  ) NULL_func,
                                 (ptu32_t)NULL   //key设备私有数据
                                 ); 
    if(pg_pipe_parent_device == NULL)
        return 0;
    
    //初始化pipe控制块内存池
#if(cn_pipes_limit != 0)
    pg_pipe_pool = mb_create_pool_knl( &pipe_pool,
                                    (void*)tg_pipe_mem_block,
                                    cn_pipes_limit,
                                    sizeof(struct pipe_PCB),
                                    0,0,
                                    "pipe控制块池");
#else
    pg_pipe_pool = mb_create_pool_knl( &pipe_pool,
                                    NULL,
                                    cn_pipes_limit,
                                    sizeof(struct pipe_PCB),
                                    0,0,
                                    "pipe控制块池");
#endif
    return 1;
}


//----创建一个管道-----------------------------------------------------------
//功能：创建一个管道
//参数：buf,管道使用的缓冲区，须由使用者分配提供。
//      size，缓冲区大小
//      whole，true=一次性操作，即要求读写操作一次性完成，false=非一次性操作，
//          写入时，有多少空位写入多少，读取时，管道中有多少数据读取多少
//      name，管道的名字，所指向的字符串内存区不能是局部变量，可以是空
//返回：新建立的信号量指针
//-----------------------------------------------------------------------------
struct pipe_PCB *pipe_create(u8* buf,u32 size,bool_t whole,
                             u32 write_level,u32 read_level,char *name)
{
    struct pipe_PCB *pipe;
    struct semaphore_LCB *read_dev_semp,*write_dev_semp;
    struct pan_device *pipe_device;
    
    pipe = (struct pipe_PCB *)mb_malloc(pg_pipe_pool, 0);
    if(pipe == NULL)
        goto exit_from_malloc_PCB;
    //初始处于无信号状态，须读操作释放才处于有信号状态
    pipe->pipe_write_semp = semp_create(1,0,name);
    if(pipe->pipe_write_semp == NULL)
        goto exit_from_pipe_write_semp;
    //初始处于无信号状态，须写操作释放才处于有信号状态
    pipe->pipe_read_semp = semp_create(1,0,name);
    if(pipe->pipe_read_semp == NULL)
        goto exit_from_pipe_read_semp;
    read_dev_semp = semp_create(1,1,NULL);
    if(read_dev_semp == NULL)
        goto exit_from_dev_read_semp;
    write_dev_semp = semp_create(1,1,NULL);
    if(write_dev_semp == NULL)
        goto exit_from_dev_write_semp;
    pipe_device = dev_add_device(pg_pipe_parent_device,
                                 name,
                                 read_dev_semp,write_dev_semp,
                                 NULL,
                                 pipe_dev_read,
                                 pipe_dev_ctrl,
                                 pipe_dev_write,
                                 NULL,
                                 pipe_dev_ctrl,
                                 (ptu32_t)pipe       );
    if(pipe_device == NULL)
        goto exit_from_create_device;
                                 
    ring_init(&pipe->pipe_buf,buf,size);
    pipe->write_level = write_level;
    pipe->read_level = read_level;
    pipe->whole_mode = whole;
    return pipe;

exit_from_create_device:
    semp_delete(write_dev_semp);
exit_from_dev_write_semp:
    semp_delete(read_dev_semp);
exit_from_dev_read_semp:
    semp_delete(pipe->pipe_read_semp);
exit_from_pipe_read_semp:
    semp_delete(pipe->pipe_write_semp);
exit_from_pipe_write_semp:
    mb_free(pg_pipe_pool,pipe);
exit_from_malloc_PCB:
    return NULL;
}

//----读管道-------------------------------------------------------------------
//功能: 从管道中读数据
//参数: pipe_dev，目标管道设备
//      res，保留参数
//      dst_buf，读出数据存放地址
//      len，读出长度
//返回: 总是返回len
//-----------------------------------------------------------------------------
ptu32_t pipe_dev_read(struct  pan_device *pipe_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len)
{
    struct pipe_PCB *pipe;
    u32 completed=0,temp;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    if(len > ring_capacity(&pipe->pipe_buf))    //一次读取数据不能超过管道容量
        return 0;
    temp = ring_check(&pipe->pipe_buf);
    while(1)
    {
        completed += ring_read(&pipe->pipe_buf,(u8*)(dst_buf+completed),
                               (u32)len-completed);
        if(completed < (u32)len)    //管道中的数据量不足
        {
            semp_pend(pipe->pipe_read_semp,cn_timeout_forever);
        }else
        {
            break;
        }
    } 
    //如果读之前超过写入门槛且读之后低于写入门槛，post信号量
    if( (temp >= pipe->write_level)
          &&((temp - completed) < pipe->write_level)) 
    {
        semp_post(pipe->pipe_write_semp);
    }
    return len;
}

//----写管道-------------------------------------------------------------------
//功能: 把数据写入管道中
//参数: pipe_dev，目标管道设备
//      src_buf，写入数据存放地址
//      res，保留参数
//      len，写入长度
//返回: 实际写入数据量
//-----------------------------------------------------------------------------
u32 pipe_dev_write(struct  pan_device *pipe_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    struct pipe_PCB *pipe;
    u32 completed = 0, temp;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    temp = ring_check(&pipe->pipe_buf);
    while(1)
    {
        if(pipe->whole_mode == true)        //整体模式
        {
            //管道中的剩余空间不足
            if((ring_capacity(&pipe->pipe_buf)-ring_check(&pipe->pipe_buf))
                        < (u32)len)
            {
                semp_pend(pipe->pipe_write_semp,cn_timeout_forever);
            }else
            {
                ring_write(&pipe->pipe_buf,(u8*)src_buf,(u32)len);
                break;
            }
        }else
        {
            completed +=ring_write(&pipe->pipe_buf,(u8*)src_buf,len -completed);
            if(completed >= len)
                break;
        }
    }

    //如果写之前不到读取门槛且读之后超过写入门槛，post信号量
    if( (temp < pipe->read_level)
          &&((temp + completed) >= pipe->read_level)) 
    {
        semp_post(pipe->pipe_read_semp);
    }
    return completed;
}

//----控制管道-----------------------------------------------------------------
//功能: 空函数
//-----------------------------------------------------------------------------
ptu32_t pipe_dev_ctrl(struct  pan_device *pipe_dev,u32 cmd,
                        u32 data1,u32 data2)
{
    return 0;
}
//----删除一个管道-----------------------------------------------------------
//功能：删除一个管道
//参数：mutex，被删除的管道设备
//返回：无
//-----------------------------------------------------------------------------
bool_t pipe_delete(struct  pan_device *pipe_dev)
{
    struct pipe_PCB *pipe;
    struct semaphore_LCB *pipe_left_semp,*pipe_right_semp;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    pipe_left_semp = pipe_dev->left_semp;
    pipe_right_semp = pipe_dev->right_semp;
    if(dev_delete_device(pipe_dev))
    {
        semp_delete(pipe_left_semp);
        semp_delete(pipe_right_semp);
        semp_delete(pipe->pipe_read_semp);
        semp_delete(pipe->pipe_write_semp);
        mb_free(pg_pipe_pool,pipe);
        return true;
    }else
        return false;
}

//----查询管道容量-----------------------------------------------------------
//功能：查询一个管道设备的容量，
//参数：pipe_dev，被查询的管道句柄
//返回：管道容量
//-----------------------------------------------------------------------------
u32 pipe_capacity(struct  pan_device *pipe_dev)
{
    struct pipe_PCB *pipe;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    return ring_capacity(&pipe->pipe_buf);
}

//----查询管道中数据量---------------------------------------------------------
//功能：查询一个管道设备中的数据量，
//参数：pipe_dev，被查询的管道句柄
//返回：管道中的数据量
//-----------------------------------------------------------------------------
u32 pipe_check(struct  pan_device *pipe_dev)
{
    struct pipe_PCB *pipe;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    return ring_check(&pipe->pipe_buf);
}

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))


