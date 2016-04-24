//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
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
#include "stdint.h"
#include "int.h"
#include "ring.h"
#include <string.h>

//----�������λ�����----------------------------------------------------------
//����: �������λ���������ʼ����ʹ���������֮ǰ���û�Ӧ�ö��建�����ڴ���
//      ���������ݽṹ��
//����: ring,Ŀ�껷�λ������ṹָ��
//      buf,��������ʼ��ַ
//      len,����������.��λ���ֽ���
//����: ��
//-----------------------------------------------------------------------------
void Ring_Init(struct tagRingBuf *ring, u8 *buf, u32 len)
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
u32 Ring_Capacity(struct tagRingBuf *ring)
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
u8 *Ring_GetBuf(struct tagRingBuf *ring)
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
u32 Ring_Write(struct tagRingBuf *ring,u8 *buffer,u32 len)
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
    atom_bak = Int_LowAtomStart();
    ring->len += wr_len;
    Int_LowAtomEnd(atom_bak);
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
u32    Ring_Read(struct tagRingBuf *ring,u8 *buffer,u32 len)
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
    atom_bak = Int_LowAtomStart();
    ring->len -= wr_len;
    Int_LowAtomEnd(atom_bak);
    return wr_len;
}

//----��黺����������----------------------------------------------------------
//����: ���ָ���Ļ��λ������е�������,�����ֽ���.
//����: ring,Ŀ�껷�λ�����ָ��.
//����: �������е�������
//------------------------------------------------------------------------------
u32    Ring_Check(struct tagRingBuf *ring)
{
    return ring->len;       //len��ucpu_t���͵ģ�����ȷ����������ԭ����
}

//----��黺�����Ƿ��-----------------------------------------------------------
//����: ���ָ���Ļ��λ��������Ƿ��Ѿ���.
//����: ring,Ŀ�껷�λ�����ָ��.
//����: ���򷵻�true,�ǿշ���false
//------------------------------------------------------------------------------
bool_t   Ring_IsEmpty(struct tagRingBuf *ring)
{
    return (ring->len ==0)? true:false;
}

//----��黺�����Ƿ���-----------------------------------------------------------
//����: ���ָ���Ļ��λ��������Ƿ��Ѿ���.
//����: ring,Ŀ�껷�λ�����ָ��.
//����: ���򷵻�true,��������false
//------------------------------------------------------------------------------
bool_t   Ring_IsFull(struct tagRingBuf *ring)
{
    return (ring->len == ring->max_len)? true:false;
}

//----��ջ��λ�����-------------------------------------------------------------
//����: �������������������
//����: ring,Ŀ�껷�λ�����ָ��.
//����: ��
//�ر�ע��: ����ǰ����ȷ��û�������߳���ʹ�û�������������ܳ��ֲ���Ԥ�ϵĽ��
//------------------------------------------------------------------------------
void    Ring_Flush(struct tagRingBuf *ring)
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
u32 Ring_DumbRead(struct tagRingBuf *ring,u32 len)
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
    atom_bak = Int_LowAtomStart();
    ring->len -= result;
    Int_LowAtomEnd(atom_bak);
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
u32 Ring_RecedeRead(struct tagRingBuf *ring,u32 len)
{
    u32    result;
    atom_low_t  atom_bak;

    atom_bak = Int_LowAtomStart();
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
    Int_LowAtomEnd(atom_bak);
    return result;
}

//----ȡ����������-------------------------------------------------------------
//����: ȡ���Ѿ�д�����Ի��������������ݣ��������û��д��һ����
//����: ring,Ŀ�껷�λ�����ָ��.
//      len,�˻ص���������
//����: ʵ���˻ص�������
//-----------------------------------------------------------------------------
u32 Ring_SkipTail(struct tagRingBuf *ring,u32 size)
{
    u32 result;
    atom_low_t  atom_bak;

    atom_bak = Int_LowAtomStart();
    if(ring->len > size)
    {
        result = size;
        if(ring->offset_write < result)
        {   //���ݷ�������
            ring->offset_write = ring->max_len - (result - ring->offset_write);
        }else
        {   //����������
            ring->offset_write -= result;
        }
        ring->len -=result;
    }else
    {
        result = ring->len;
        Ring_Flush(ring);
    }
    Int_LowAtomEnd(atom_bak);
    return result;
}

//----�����ַ�------------------------------------------------------------------
//����: ��ring��ǰ��λ�ÿ�ʼ�����ַ�c��λ��
//����: ring,Ŀ�껷�λ�����ָ��
//      c,����ҵ��ַ�
//����: c���ֵ�λ����Զ�ָ���ƫ����,���û�г����򷵻� CN_LIMIT_UINT32
//------------------------------------------------------------------------------
u32 Ring_SearchCh(struct tagRingBuf *ring, char c)
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
    return CN_LIMIT_UINT32;
}

//----�����ַ�����--------------------------------------------------------------
//����: ��ring��ǰ��λ�ÿ�ʼ�����ַ����е�λ��,�ַ����в���0����,����ָ������
//����: ring,Ŀ�껷�λ�����ָ��
//      string,����ҵ��ַ�����
//      str_len,�ַ����г���
//����: string���ֵ�λ�����offset_read��ƫ����,���û�г��ַ��� CN_LIMIT_UINT32
//��ע: ������ܿ��ܱȽϳ���,�����ڱ�дʱע�����ٶ��Ż�,��ȴʹ����������.
//------------------------------------------------------------------------------
u32 Ring_SearchStr(struct tagRingBuf *ring, char *string,u32 str_len)
{
    u32 i,j;
    bool_t next;
    u8 *buf;
    u32  end,start;

    if(ring->len < str_len)
        return CN_LIMIT_UINT32;
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
    return CN_LIMIT_UINT32;
}
