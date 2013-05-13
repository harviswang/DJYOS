//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 缓冲区
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 提供环形缓冲区服务
//其他说明:
//修订历史:
//2. 日期: 2010-02-10
//   作者:  罗侍田.
//   新版本号: V1.1.0
//   修改说明: 对struct ring_buf的len成员实施原子保护，使之支持读写并发.如果缓冲
//      区的读和写均只有一个线程(可以是不同线程),则可以安全地操作。如果有超过1个
//      线程读或写，则还需要做信号量保护以确保并发访问安全。
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>

//----建立环形缓冲区----------------------------------------------------------
//功能: 建立环形缓冲区并初始化，使用这个函数之前，用户应该定义缓冲区内存块和
//      缓冲区数据结构。
//参数: ring,目标环形缓冲区结构指针
//      buf,缓冲区起始地址
//      len,缓冲区长度.单位是字节数
//返回: 无
//-----------------------------------------------------------------------------
void ring_init(struct ring_buf *ring, u8 *buf, u32 len)
{
    ring->buf = buf;
    ring->max_len = len;
    ring->offset_write = 0;
    ring->offset_read  = 0;
    ring->len = 0;
}

//----检查缓冲区容量-----------------------------------------------------------
//功能: 返回缓冲区容量
//参数: ring,目标线性缓冲区结构指针
//返回: 缓冲区容量，就是调用ring_init时使用的len参数。
//-----------------------------------------------------------------------------
u32 ring_capacity(struct ring_buf *ring)
{
    return ring->max_len;
}

//----返回字节池地址-----------------------------------------------------------
//功能: 查询缓冲区的字节池地址，这个地址是用户调用ring_init时使用的缓冲区地址。
//参数: ring,目标线性缓冲区指针.
//返回: 环形缓冲区的字节池地址
//说明: 有两种情况会产生获得缓冲区地址的需求
//      1、要释放缓冲区内存，应用程序可以使用调用ring_init时使用的指针，如果该
//         指针丢失，可用本函数取回该指针。
//      2、用户想自己动手访问该缓冲区，这不是一个明智的选择，有破坏模块独立性
//         的嫌疑，这时候，使用者应该完全明白自己在干什么!
//-----------------------------------------------------------------------------
u8 *ring_get_buf(struct ring_buf *ring)
{
    return ring->buf;
}

//----环形缓冲区写入-----------------------------------------------------------
//功能: 环形缓冲区写入若干个字节,返回实际写入的数据量,并移动写指针,如果环形
//      缓冲区没有足够的空间,按实际剩余空间写入
//参数: ring,目标环形缓冲区结构指针
//      buffer,待写入的数据指针
//      len,待写入的数据长度.单位是字节数
//返回: 实际写入的字节数,如果缓冲区有足够的空间,=len
//-----------------------------------------------------------------------------
u32    ring_write(struct ring_buf *ring,u8 *buffer,u32 len)
{
    u32    wr_len;
    u32    partial;
    atom_low_t  atom_bak;
    wr_len = ring->max_len - ring->len;
    if(wr_len == 0)
        return 0;
    if(wr_len > len)
        wr_len = len;
    if((ring->offset_write + wr_len) > ring->max_len)
    {   //数据发生环绕
        partial = ring->max_len - ring->offset_write;
        memcpy(&ring->buf[ring->offset_write],buffer,partial);    //写第一部分
        memcpy( ring->buf,&buffer[partial],wr_len - partial);    //写第二部分
        ring->offset_write = wr_len - partial;
    }else
    {   //不发生环绕
        memcpy( &ring->buf[ring->offset_write],buffer,wr_len);
        ring->offset_write +=wr_len;
    }
    atom_bak = int_low_atom_start();
    ring->len += wr_len;
    int_low_atom_end(atom_bak);
    return wr_len;
}

//----从环形缓冲区读-----------------------------------------------------------
//功能: 从环形缓冲区读出若干个字节,返回实际读出的数据量,并且移动读指针如果
//      缓冲区内数据不足，按实际数量读取。
//参数: ring,目标环形缓冲区结构指针
//      buffer,接收数据的缓冲区指针
//      len,待读出的数据长度.单位是字节数
//返回: 实际读出的字节数,如果缓冲区有足够的数据,=len
//------------------------------------------------------------------------------
u32    ring_read(struct ring_buf *ring,u8 *buffer,u32 len)
{
    u32    wr_len,ring_len;
    atom_low_t  atom_bak;
    ring_len = ring->len;
    wr_len = (ring_len < len)? ring_len : len;
    if((ring->offset_read + wr_len) > ring->max_len)
    {   //数据发生环绕
        u32  partial;
        partial =ring->max_len - ring->offset_read;
        memcpy( buffer,&ring->buf[ring->offset_read],partial);    //写第一部分
        memcpy( &buffer[partial],ring->buf,wr_len - partial);    //写第二部分
        ring->offset_read = wr_len - partial;
    }else
    {   //不发生环绕
        memcpy( buffer,&ring->buf[ring->offset_read],wr_len);
        ring->offset_read += wr_len;
    }
    atom_bak = int_low_atom_start();
    ring->len -= wr_len;
    int_low_atom_end(atom_bak);
    return wr_len;
}

//----检查缓冲区数据量----------------------------------------------------------
//功能: 检查指定的环形缓冲区中的数据量,返回字节数.
//参数: ring,目标环形缓冲区指针.
//返回: 缓冲区中的数据量
//------------------------------------------------------------------------------
u32    ring_check(struct ring_buf *ring)
{
    return ring->len;       //len是ucpu_t类型的，可以确保读操作的原子性
}

//----检查缓冲区是否空-----------------------------------------------------------
//功能: 检查指定的环形缓冲区中是否已经空.
//参数: ring,目标环形缓冲区指针.
//返回: 空则返回true,非空返回false
//------------------------------------------------------------------------------
bool_t   ring_is_empty(struct ring_buf *ring)
{
    return (ring->len ==0)? true:false;
}

//----检查缓冲区是否满-----------------------------------------------------------
//功能: 检查指定的环形缓冲区中是否已经满.
//参数: ring,目标环形缓冲区指针.
//返回: 满则返回true,非满返回false
//------------------------------------------------------------------------------
bool_t   ring_if_full(struct ring_buf *ring)
{
    return (ring->len == ring->max_len)? true:false;
}

//----清空环形缓冲区-------------------------------------------------------------
//功能: 清除缓冲区中所有数据
//参数: ring,目标环形缓冲区指针.
//返回: 无
//------------------------------------------------------------------------------
void    ring_flush(struct ring_buf *ring)
{
    ring->len = 0;
    ring->offset_write = 0;
    ring->offset_read = 0;
}

//----释放若干数据---------------------------------------------------------------
//功能: 从读指针开始,释放掉指定大小的数据,相当于哑读了len个字节
//参数: ring,目标环形缓冲区指针.
//      len,释放的数据数量
//返回: 实际释放的数据量
//------------------------------------------------------------------------------
u32 ring_dumb_read(struct ring_buf *ring,u32 len)
{
    u32    result,ring_len;
    atom_low_t  atom_bak;
    ring_len = ring->len;
    result = (ring_len < len)? ring_len : len;
    if((ring->offset_read + result) > ring->max_len)
    {   //数据发生环绕
        ring->offset_read = result + ring->offset_read - ring->max_len;
    }else
    {   //不发生环绕
        ring->offset_read += result;
    }
    atom_bak = int_low_atom_start();
    ring->len -= result;
    int_low_atom_end(atom_bak);
    return result;
}

//----退回若干数据-------------------------------------------------------------
//功能: 本函数与ring_dumb_read函数正好相反，把缓冲区指针退回len字节，如果退回的
//      长度超过缓冲区的空闲长度，则取缓冲区空闲长度。相当于把缓冲区中已经读出
//      的数据返回缓冲区，好像没有读过的样子。ring模块并不校验退回的部分是否包含
//      原来的数据。
//参数: ring,目标环形缓冲区指针.
//      len,退回的数据数量
//返回: 实际退回的数据量
//-----------------------------------------------------------------------------
u32 ring_recede_read(struct ring_buf *ring,u32 len)
{
    u32    result;
    atom_low_t  atom_bak;
    
    atom_bak = int_low_atom_start();
    if((ring->max_len - ring->len) > len)   //空闲长度大于欲退回的长度
        result = len;
    else
        result = ring->max_len - ring->len;
    if(ring->offset_read < result)
    {   //数据发生环绕
        ring->offset_read = ring->max_len - (result - ring->offset_read);
    }else
    {   //不发生环绕
        ring->offset_read -= result;
    }
    ring->len += result;
    int_low_atom_end(atom_bak);
    return result;
}

//----取消若干数据-------------------------------------------------------------
//功能: 取消已经写入线性缓冲区的若干数据，就像从来没有写入一样。
//参数: ring,目标环形缓冲区指针.
//      len,退回的数据数量
//返回: 实际退回的数据量
//-----------------------------------------------------------------------------
u32 ring_skip_tail(struct ring_buf *ring,u32 size)
{
    u32 result;
    atom_low_t  atom_bak;
    
    atom_bak = int_low_atom_start();
    if(ring->len > size)
    {
        result = size;
        if(ring->offset_write < result)
        {   //数据发生环绕
            ring->offset_write = ring->max_len - (result - ring->offset_write);
        }else
        {   //不发生环绕
            ring->offset_read -= result;
        }
        ring->len -=result;
    }else
    {
        result = ring->len;
        ring_flush(ring);
    }
    int_low_atom_end(atom_bak);
    return result;
}

//----查找字符------------------------------------------------------------------
//功能: 从ring当前读位置开始查找字符c的位置
//参数: ring,目标环形缓冲区指针
//      c,需查找的字符
//返回: c出现的位置,如果没有出现则返回 cn_limit_uint32
//------------------------------------------------------------------------------
u32 ring_search_ch(struct ring_buf *ring, char c)
{
    u32    i;
    u8 *buf = ring->buf;
    if(ring->offset_read > ring->offset_write)
    {   //缓冲区有回绕
        for(i=ring->offset_read;i<ring->max_len;i++)
        {
            if(buf[i] == c)
                return (i - ring->offset_read);
        }
        for(i=0;i<ring->offset_write;i++)
            if(buf[i] == c)
                return (i - ring->offset_read + ring->max_len);
    }else
    {   //缓冲区没有回绕
        for(i = ring->offset_read; i < ring->offset_write; i++)
            if(buf[i] == c)
                return (i - ring->offset_read);
    }
    return cn_limit_uint32;
}

//----查找字符序列--------------------------------------------------------------
//功能: 从ring当前读位置开始查找字符序列的位置,字符序列不以0结束,而是指定长度
//参数: ring,目标环形缓冲区指针
//      string,需查找的字符序列
//      str_len,字符序列长度
//返回: string出现的位置相对offset_read的偏移量,如果没有出现返回 cn_limit_uint32
//备注: 这个功能可能比较常用,所以在编写时注意了速度优化,但却使代码量大增.
//------------------------------------------------------------------------------
u32 ring_search_str(struct ring_buf *ring, char *string,u32 str_len)
{
    u32 i,j;
    bool_t next;
    u8 *buf;
    u32  end,start;

    if(ring->len < str_len)
        return cn_limit_uint32;
    buf = ring->buf;
    if(ring->offset_read <= ring->offset_write)
    {   //缓冲区没有回绕
        for(i=ring->offset_read;i <= ring->offset_write - str_len;i++)
        {
            next = false;
            for(j=0;j < str_len;j++)
            {
                if(buf[i+j] != string[j])
                    next = true;
            }
            if(next == false)
            {
                return (i - ring->offset_read);
            }
        }
    }else
    {   //缓冲区有回绕
        //先处理不环绕部分,不包括缓冲区首末结合部分
        for(i=ring->offset_read;i <= (ring->max_len - str_len);i++)
        {
            next = false;
            for(j=0;j < str_len;j++)
            {
                if(buf[i+j] != string[j])
                    next = true;    //如果发现不等,则表示要进行下一个循环
            }
            if(next == false)
            {   //表示上一个循环中已经发现匹配的串,
                return (i - ring->offset_read);
            }
        }
        //再处理缓冲区首末结合部分以及回绕部分
        if(ring->offset_write >= str_len)
        {   //缓冲区中回绕部分的长度超过查找长度,末端和首端分别查找
            //先查找缓冲区末端部分
            for(;i < ring->max_len;i++)
            {
                next = false;
                //string分成两部分,end个字符在缓冲区末端,start个字符在缓冲区首
                end = ring->max_len - i;
                start = str_len - end;
                for(j=0;j<end;j++)
                {   //先比较缓冲区末端部分
                    if(buf[i+j] != string[j])
                        next = true;
                }
                if(next == false)
                {   //缓冲区末端部分全部匹配,需要看首端是否也匹配
                    for(j=0;j<start;j++)
                        if(buf[j] != string[start+j])
                            next = true;
                }
                if(next == false)
                {   //表示上一个循环中已经发现匹配的串,
                    return (i - ring->offset_read);
                }
            }
            //再查找首端部分
            for(i=0;i < (ring->offset_write - str_len);i++)
            {
                next = false;
                for(j=0;j < str_len;j++)
                {
                    if(buf[i+j] != string[j])
                        next = true;    //如果发现不等,则表示要进行下一个循环
                }
                if(next == false)
                {   //表示上一个循环中已经发现匹配的串,
                    return (i - ring->offset_read);
                }
            }
        }else
        {   //缓冲区中回绕部分长度没有超过str_len,只需要查找末端即可.
            next = false;
            for(;i < (ring->offset_read + ring->len - str_len);i++)
            {
                u32  end,start;
                next = false;
                //string分成两部分,end个字符在缓冲区末端,start个字符在缓冲区首
                end = ring->max_len - i;
                start = str_len - end;
                for(j=0;j<end;j++)
                {   //先比较缓冲区末端部分
                    if(buf[i+j] != string[j])
                        next = true;
                }
                if(next == false)
                {   //缓冲区末端部分全部匹配,需要看首端是否也匹配
                    for(j=0;j<start;j++)
                        if(buf[j] != string[start+j])
                            next = true;
                }
                if(next == false)
                {   //表示上一个循环中已经发现匹配的串,
                    return (i - ring->offset_read);
                }
            }
        }
    }
    return cn_limit_uint32;
}
