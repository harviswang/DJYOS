//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �ṩ���λ���������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __ring_h__
#define __ring_h__

#ifdef __cplusplus
extern "C" {
#endif

struct ring_buf
{
    u32    len;            //�������е��ֽ���
    u32    offset_read;    //��������ָ��,ָ����һ�ζ���λ��
    u32    offset_write;   //������дָ��,ָ����һ��д��λ��
    u32    max_len;        //��������󳤶�,Ԫ�ظ���.
    u8   *buf;           //������ָ��,�û��Լ���֤�����ٵĻ������Ƿ����趨
                              //����һ��,djyos�������.
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
