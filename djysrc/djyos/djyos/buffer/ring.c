//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �ṩ���λ���������
//����˵��:
//�޶���ʷ:
//2. ����: 2010-02-10
//   ����:  ������.
//   �°汾��: V1.1.0
//   �޸�˵��: ��struct ring_buf��len��Աʵʩԭ�ӱ�����ʹ֧֮�ֶ�д����.�������
//      ���Ķ���д��ֻ��һ���߳�(�����ǲ�ͬ�߳�),����԰�ȫ�ز���������г���1��
//      �̶߳���д������Ҫ���ź���������ȷ���������ʰ�ȫ��
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>

//----�������λ�����----------------------------------------------------------
//����: �������λ���������ʼ����ʹ���������֮ǰ���û�Ӧ�ö��建�����ڴ���
//      ���������ݽṹ��
//����: ring,Ŀ�껷�λ������ṹָ��
//      buf,��������ʼ��ַ
//      len,����������.��λ���ֽ���
//����: ��
//-----------------------------------------------------------------------------
void ring_init(struct ring_buf *ring, u8 *buf, u32 len)
{
    ring->buf = buf;
    ring->max_len = len;
    ring->offset_write = 0;
    ring->offset_read  = 0;
    ring->len = 0;
}

//----��黺��������-----------------------------------------------------------
//����: ���ػ���������
//����: ring,Ŀ�����Ի������ṹָ��
//����: ���������������ǵ���ring_initʱʹ�õ�len������
//-----------------------------------------------------------------------------
u32 ring_capacity(struct ring_buf *ring)
{
    return ring->max_len;
}

//----�����ֽڳص�ַ-----------------------------------------------------------
//����: ��ѯ���������ֽڳص�ַ�������ַ���û�����ring_initʱʹ�õĻ�������ַ��
//����: ring,Ŀ�����Ի�����ָ��.
//����: ���λ��������ֽڳص�ַ
//˵��: ����������������û�������ַ������
//      1��Ҫ�ͷŻ������ڴ棬Ӧ�ó������ʹ�õ���ring_initʱʹ�õ�ָ�룬�����
//         ָ�붪ʧ�����ñ�����ȡ�ظ�ָ�롣
//      2���û����Լ����ַ��ʸû��������ⲻ��һ�����ǵ�ѡ�����ƻ�ģ�������
//         �����ɣ���ʱ��ʹ����Ӧ����ȫ�����Լ��ڸ�ʲô!
//-----------------------------------------------------------------------------
u8 *ring_get_buf(struct ring_buf *ring)
{
    return ring->buf;
}

//----���λ�����д��-----------------------------------------------------------
//����: ���λ�����д�����ɸ��ֽ�,����ʵ��д���������,���ƶ�дָ��,�������
//      ������û���㹻�Ŀռ�,��ʵ��ʣ��ռ�д��
//����: ring,Ŀ�껷�λ������ṹָ��
//      buffer,��д�������ָ��
//      len,��д������ݳ���.��λ���ֽ���
//����: ʵ��д����ֽ���,������������㹻�Ŀռ�,=len
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
    {   //���ݷ�������
        partial = ring->max_len - ring->offset_write;
        memcpy(&ring->buf[ring->offset_write],buffer,partial);    //д��һ����
        memcpy( ring->buf,&buffer[partial],wr_len - partial);    //д�ڶ�����
        ring->offset_write = wr_len - partial;
    }else
    {   //����������
        memcpy( &ring->buf[ring->offset_write],buffer,wr_len);
        ring->offset_write +=wr_len;
    }
    atom_bak = int_low_atom_start();
    ring->len += wr_len;
    int_low_atom_end(atom_bak);
    return wr_len;
}

//----�ӻ��λ�������-----------------------------------------------------------
//����: �ӻ��λ������������ɸ��ֽ�,����ʵ�ʶ�����������,�����ƶ���ָ�����
//      �����������ݲ��㣬��ʵ��������ȡ��
//����: ring,Ŀ�껷�λ������ṹָ��
//      buffer,�������ݵĻ�����ָ��
//      len,�����������ݳ���.��λ���ֽ���
//����: ʵ�ʶ������ֽ���,������������㹻������,=len
//------------------------------------------------------------------------------
u32    ring_read(struct ring_buf *ring,u8 *buffer,u32 len)
{
    u32    wr_len,ring_len;
    atom_low_t  atom_bak;
    ring_len = ring->len;
    wr_len = (ring_len < len)? ring_len : len;
    if((ring->offset_read + wr_len) > ring->max_len)
    {   //���ݷ�������
        u32  partial;
        partial =ring->max_len - ring->offset_read;
        memcpy( buffer,&ring->buf[ring->offset_read],partial);    //д��һ����
        memcpy( &buffer[partial],ring->buf,wr_len - partial);    //д�ڶ�����
        ring->offset_read = wr_len - partial;
    }else
    {   //����������
        memcpy( buffer,&ring->buf[ring->offset_read],wr_len);
        ring->offset_read += wr_len;
    }
    atom_bak = int_low_atom_start();
    ring->len -= wr_len;
    int_low_atom_end(atom_bak);
    return wr_len;
}

//----��黺����������----------------------------------------------------------
//����: ���ָ���Ļ��λ������е�������,�����ֽ���.
//����: ring,Ŀ�껷�λ�����ָ��.
//����: �������е�������
//------------------------------------------------------------------------------
u32    ring_check(struct ring_buf *ring)
{
    return ring->len;       //len��ucpu_t���͵ģ�����ȷ����������ԭ����
}

//----��黺�����Ƿ��-----------------------------------------------------------
//����: ���ָ���Ļ��λ��������Ƿ��Ѿ���.
//����: ring,Ŀ�껷�λ�����ָ��.
//����: ���򷵻�true,�ǿշ���false
//------------------------------------------------------------------------------
bool_t   ring_is_empty(struct ring_buf *ring)
{
    return (ring->len ==0)? true:false;
}

//----��黺�����Ƿ���-----------------------------------------------------------
//����: ���ָ���Ļ��λ��������Ƿ��Ѿ���.
//����: ring,Ŀ�껷�λ�����ָ��.
//����: ���򷵻�true,��������false
//------------------------------------------------------------------------------
bool_t   ring_if_full(struct ring_buf *ring)
{
    return (ring->len == ring->max_len)? true:false;
}

//----��ջ��λ�����-------------------------------------------------------------
//����: �������������������
//����: ring,Ŀ�껷�λ�����ָ��.
//����: ��
//------------------------------------------------------------------------------
void    ring_flush(struct ring_buf *ring)
{
    ring->len = 0;
    ring->offset_write = 0;
    ring->offset_read = 0;
}

//----�ͷ���������---------------------------------------------------------------
//����: �Ӷ�ָ�뿪ʼ,�ͷŵ�ָ����С������,�൱���ƶ���len���ֽ�
//����: ring,Ŀ�껷�λ�����ָ��.
//      len,�ͷŵ���������
//����: ʵ���ͷŵ�������
//------------------------------------------------------------------------------
u32 ring_dumb_read(struct ring_buf *ring,u32 len)
{
    u32    result,ring_len;
    atom_low_t  atom_bak;
    ring_len = ring->len;
    result = (ring_len < len)? ring_len : len;
    if((ring->offset_read + result) > ring->max_len)
    {   //���ݷ�������
        ring->offset_read = result + ring->offset_read - ring->max_len;
    }else
    {   //����������
        ring->offset_read += result;
    }
    atom_bak = int_low_atom_start();
    ring->len -= result;
    int_low_atom_end(atom_bak);
    return result;
}

//----�˻���������-------------------------------------------------------------
//����: ��������ring_dumb_read���������෴���ѻ�����ָ���˻�len�ֽڣ�����˻ص�
//      ���ȳ����������Ŀ��г��ȣ���ȡ���������г��ȡ��൱�ڰѻ��������Ѿ�����
//      �����ݷ��ػ�����������û�ж��������ӡ�ringģ�鲢��У���˻صĲ����Ƿ����
//      ԭ�������ݡ�
//����: ring,Ŀ�껷�λ�����ָ��.
//      len,�˻ص���������
//����: ʵ���˻ص�������
//-----------------------------------------------------------------------------
u32 ring_recede_read(struct ring_buf *ring,u32 len)
{
    u32    result;
    atom_low_t  atom_bak;
    
    atom_bak = int_low_atom_start();
    if((ring->max_len - ring->len) > len)   //���г��ȴ������˻صĳ���
        result = len;
    else
        result = ring->max_len - ring->len;
    if(ring->offset_read < result)
    {   //���ݷ�������
        ring->offset_read = ring->max_len - (result - ring->offset_read);
    }else
    {   //����������
        ring->offset_read -= result;
    }
    ring->len += result;
    int_low_atom_end(atom_bak);
    return result;
}

//----ȡ����������-------------------------------------------------------------
//����: ȡ���Ѿ�д�����Ի��������������ݣ��������û��д��һ����
//����: ring,Ŀ�껷�λ�����ָ��.
//      len,�˻ص���������
//����: ʵ���˻ص�������
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
        {   //���ݷ�������
            ring->offset_write = ring->max_len - (result - ring->offset_write);
        }else
        {   //����������
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

//----�����ַ�------------------------------------------------------------------
//����: ��ring��ǰ��λ�ÿ�ʼ�����ַ�c��λ��
//����: ring,Ŀ�껷�λ�����ָ��
//      c,����ҵ��ַ�
//����: c���ֵ�λ��,���û�г����򷵻� cn_limit_uint32
//------------------------------------------------------------------------------
u32 ring_search_ch(struct ring_buf *ring, char c)
{
    u32    i;
    u8 *buf = ring->buf;
    if(ring->offset_read > ring->offset_write)
    {   //�������л���
        for(i=ring->offset_read;i<ring->max_len;i++)
        {
            if(buf[i] == c)
                return (i - ring->offset_read);
        }
        for(i=0;i<ring->offset_write;i++)
            if(buf[i] == c)
                return (i - ring->offset_read + ring->max_len);
    }else
    {   //������û�л���
        for(i = ring->offset_read; i < ring->offset_write; i++)
            if(buf[i] == c)
                return (i - ring->offset_read);
    }
    return cn_limit_uint32;
}

//----�����ַ�����--------------------------------------------------------------
//����: ��ring��ǰ��λ�ÿ�ʼ�����ַ����е�λ��,�ַ����в���0����,����ָ������
//����: ring,Ŀ�껷�λ�����ָ��
//      string,����ҵ��ַ�����
//      str_len,�ַ����г���
//����: string���ֵ�λ�����offset_read��ƫ����,���û�г��ַ��� cn_limit_uint32
//��ע: ������ܿ��ܱȽϳ���,�����ڱ�дʱע�����ٶ��Ż�,��ȴʹ����������.
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
    {   //������û�л���
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
    {   //�������л���
        //�ȴ������Ʋ���,��������������ĩ��ϲ���
        for(i=ring->offset_read;i <= (ring->max_len - str_len);i++)
        {
            next = false;
            for(j=0;j < str_len;j++)
            {
                if(buf[i+j] != string[j])
                    next = true;    //������ֲ���,���ʾҪ������һ��ѭ��
            }
            if(next == false)
            {   //��ʾ��һ��ѭ�����Ѿ�����ƥ��Ĵ�,
                return (i - ring->offset_read);
            }
        }
        //�ٴ���������ĩ��ϲ����Լ����Ʋ���
        if(ring->offset_write >= str_len)
        {   //�������л��Ʋ��ֵĳ��ȳ������ҳ���,ĩ�˺��׶˷ֱ����
            //�Ȳ��һ�����ĩ�˲���
            for(;i < ring->max_len;i++)
            {
                next = false;
                //string�ֳ�������,end���ַ��ڻ�����ĩ��,start���ַ��ڻ�������
                end = ring->max_len - i;
                start = str_len - end;
                for(j=0;j<end;j++)
                {   //�ȱȽϻ�����ĩ�˲���
                    if(buf[i+j] != string[j])
                        next = true;
                }
                if(next == false)
                {   //������ĩ�˲���ȫ��ƥ��,��Ҫ���׶��Ƿ�Ҳƥ��
                    for(j=0;j<start;j++)
                        if(buf[j] != string[start+j])
                            next = true;
                }
                if(next == false)
                {   //��ʾ��һ��ѭ�����Ѿ�����ƥ��Ĵ�,
                    return (i - ring->offset_read);
                }
            }
            //�ٲ����׶˲���
            for(i=0;i < (ring->offset_write - str_len);i++)
            {
                next = false;
                for(j=0;j < str_len;j++)
                {
                    if(buf[i+j] != string[j])
                        next = true;    //������ֲ���,���ʾҪ������һ��ѭ��
                }
                if(next == false)
                {   //��ʾ��һ��ѭ�����Ѿ�����ƥ��Ĵ�,
                    return (i - ring->offset_read);
                }
            }
        }else
        {   //�������л��Ʋ��ֳ���û�г���str_len,ֻ��Ҫ����ĩ�˼���.
            next = false;
            for(;i < (ring->offset_read + ring->len - str_len);i++)
            {
                u32  end,start;
                next = false;
                //string�ֳ�������,end���ַ��ڻ�����ĩ��,start���ַ��ڻ�������
                end = ring->max_len - i;
                start = str_len - end;
                for(j=0;j<end;j++)
                {   //�ȱȽϻ�����ĩ�˲���
                    if(buf[i+j] != string[j])
                        next = true;
                }
                if(next == false)
                {   //������ĩ�˲���ȫ��ƥ��,��Ҫ���׶��Ƿ�Ҳƥ��
                    for(j=0;j<start;j++)
                        if(buf[j] != string[start+j])
                            next = true;
                }
                if(next == false)
                {   //��ʾ��һ��ѭ�����Ѿ�����ƥ��Ĵ�,
                    return (i - ring->offset_read);
                }
            }
        }
    }
    return cn_limit_uint32;
}
