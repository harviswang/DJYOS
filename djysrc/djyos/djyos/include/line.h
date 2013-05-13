//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:线性缓冲区
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 提供线性缓冲区服务
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __line_h__
#define __line_h__

#ifdef __cplusplus
extern "C" {
#endif

struct line_buf
{
    ucpu_t    current;      //缓冲区中的字节数/当前指针
    ucpu_t    limit;        //缓冲区最大长度,元素个数.
    u8   *buf;         //缓冲区指针,用户自己保证所开辟的缓冲区是否与设定
                              //参数一致,djyos不做检查.
};

void line_init(struct line_buf *line, u8 *buf, u32 len);
u32 line_capacity(struct line_buf *line);
u32 line_skip_tail(struct line_buf *line,u32 len);
u32 line_write(struct line_buf *line,u8 *buffer,u32 len);
u32 line_read(struct line_buf *line,u8 *buffer);
u8 *line_get_buf(struct line_buf *line);
u32    line_check(struct line_buf *line);
bool_t   line_if_empty(struct line_buf *line);
bool_t   line_if_full(struct line_buf *line);
void    line_flush(struct line_buf *line);
u32 line_search_ch(struct line_buf *line, char c);
u32 line_search_str(struct line_buf *line, char *string,u32 str_len);

#ifdef __cplusplus
}
#endif

#endif //__line_h__

