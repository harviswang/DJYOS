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
#ifndef __pipe_h__
#define __pipe_h__

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

//管道控制块
//当读管道时，如果管道中没有足够的数据量，将阻塞在信号量中，写入数据时使管道中的
//数据量超过level，则立即释放信号量。
//写管道时，如果写满了，将阻塞在信号量中，读管道使得管道中的数据量低于level时，
//将释放信号量。
//管道是单线程组件，

//特别注意:管道可能死锁，程序员应该自己注意不要发生死锁。死锁条件为:
//1、请求从管道读的数据量超过管道长度，管道写满后将阻塞，因为读数据量一直得不到
//   满足，故阻塞将持续下去。
//2、整体模式下，请求的数据量超过(管道长度-最后一次整体写入数据量)，将出现和1一
//   样的效果。
struct pipe_PCB
{
    struct ring_buf pipe_buf;               //环形发送缓冲区.
    struct semaphore_LCB *pipe_write_semp;  //写缓冲区锁
    struct semaphore_LCB *pipe_read_semp;   //读缓冲区锁
    u32    write_level;                     //写触发水平
    u32    read_level;                      //读触发水平
    bool_t whole_mode;          //整体模式，true=确保每次写入完整记录，比如要写
                                //入10个字节，但pipe_buf中只有9个空位时，将阻塞
                                //false=先写入9个，然后阻塞
};

#define pipe_open_write(name)       dev_open_left(name,cn_timeout_forever)
#define pipe_open_read(name)        dev_open_right(name,cn_timeout_forever)
#define pipe_open_write_fast(pipe)  dev_open_left_fast(pipe,cn_timeout_forever)
#define pipe_open_read_fast(pipe)   dev_open_right_fast(pipe,cn_timeout_forever)
#define pipe_close_write(pipe)      dev_close_left(pipe)
#define pipe_close_read(pipe)       dev_close_right(pipe)
#define pipe_write(pipe,buf,len)    dev_write_left(pipe,buf,0,len)
#define pipe_read(pipe,buf,len)     dev_read_right(pipe,0,buf,len)

ptu32_t module_init_pipe(ptu32_t para);
struct pipe_PCB *pipe_create(u8* buf,u32 size,bool_t whole,
                             u32 write_level,u32 read_level,char *name);
ptu32_t pipe_dev_read(struct  pan_device *pipe_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len);
ptu32_t pipe_dev_write(struct  pan_device *pipe_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t pipe_dev_ctrl(struct  pan_device *pipe_dev,u32 cmd,
                        u32 data1,u32 data2);
bool_t pipe_delete(struct  pan_device *pipe_dev);
u32 pipe_capacity(struct  pan_device *pipe_dev);
u32 pipe_check(struct  pan_device *pipe_dev);

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#ifdef __cplusplus
}
#endif

#endif //__pipe_h__

