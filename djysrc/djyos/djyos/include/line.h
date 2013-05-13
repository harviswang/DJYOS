//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:���Ի�����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �ṩ���Ի���������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __line_h__
#define __line_h__

#ifdef __cplusplus
extern "C" {
#endif

struct line_buf
{
    ucpu_t    current;      //�������е��ֽ���/��ǰָ��
    ucpu_t    limit;        //��������󳤶�,Ԫ�ظ���.
    u8   *buf;         //������ָ��,�û��Լ���֤�����ٵĻ������Ƿ����趨
                              //����һ��,djyos�������.
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

