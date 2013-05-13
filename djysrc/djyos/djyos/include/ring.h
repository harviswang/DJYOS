//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 缓冲区
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 提供环形缓冲区服务
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __ring_h__
#define __ring_h__

#ifdef __cplusplus
extern "C" {
#endif

struct ring_buf
{
    u32    len;            //缓冲区中的字节数
    u32    offset_read;    //缓冲区读指针,指向下一次读的位置
    u32    offset_write;   //缓冲区写指针,指向下一次写的位置
    u32    max_len;        //缓冲区最大长度,元素个数.
    u8   *buf;           //缓冲区指针,用户自己保证所开辟的缓冲区是否与设定
                              //参数一致,djyos不做检查.
};

void ring_init(struct ring_buf *ring, u8 *buf, u32 len);
u32 ring_capacity(struct ring_buf *ring);
u8 *ring_get_buf(struct ring_buf *ring);
u32    ring_write(struct ring_buf *ring,u8 *buffer,u32 len);
u32    ring_read(struct ring_buf *ring,u8 *buffer,u32 len);
u32    ring_check(struct ring_buf *ring);
bool_t   ring_is_empty(struct ring_buf *ring);
bool_t   ring_if_full(struct ring_buf *ring);
void    ring_flush(struct ring_buf *ring);
u32 ring_dumb_read(struct ring_buf *ring,u32 len);
u32 ring_recede_read(struct ring_buf *ring,u32 len);
u32 ring_skip_tail(struct ring_buf *ring,u32 size);
u32 ring_search_ch(struct ring_buf *ring, char c);
u32 ring_search_str(struct ring_buf *ring, char *string,u32 str_len);

#ifdef __cplusplus
}
#endif

#endif //__ring_h__
