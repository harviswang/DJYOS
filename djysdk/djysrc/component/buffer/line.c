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
#include "stdint.h"
#include "int.h"
#include "line.h"
#include "string.h"

//----�������Ի�����----------------------------------------------------------
//����: �������Ի���������ʼ����ʹ���������֮ǰ���û�Ӧ�ö��建�����ڴ���
//      ���������ݽṹ��
//����: line,Ŀ�����Ի������ṹָ��
//      buf,��������ʼ��ַ
//      len,����������.��λ���ֽ���
//����: ��
//-----------------------------------------------------------------------------
void Line_Init(struct tagLineBuf *line, u8 *buf, u32 len)
{
    line->buf = buf;
    line->limit= len;
    line->current = 0;
}

//----��黺��������-----------------------------------------------------------
//����: ���ػ���������
//����: line,Ŀ�����Ի������ṹָ��
//����: ���������������ǵ���line_initʱʹ�õ�len������
//-----------------------------------------------------------------------------
u32 Line_Capacity(struct tagLineBuf *line)
{
    return line->limit;
}

//----ȡ����������---------------------------------------------------------------
//����: ȡ���Ѿ�д�����Ի��������������ݣ��������û��д��һ����
//����: line,Ŀ�����Ի�����ָ��.
//      len,�˻ص���������
//����: ʵ���˻ص�������
//------------------------------------------------------------------------------
u32 Line_SkipTail(struct tagLineBuf *line,u32 len)
{
    u32 result;
    if(line->current > len)
    {
        line->current -= len;
        result = len;
    }else
    {
        result = line->current;
        line->current = 0;
    }
    return result;
}

//----���Ի�����д��-----------------------------------------------------------
//����: ���Ի�����д�����ɸ��ֽ�,����ʵ��д���������,���ƶ�дָ��,�������
//      ������û���㹻�Ŀռ�,��ʵ��ʣ��ռ�д��
//����: line,Ŀ�����Ի������ṹָ��
//      buffer,��д�������ָ��
//      len,��д������ݳ���.��λ���ֽ���
//����: ʵ��д����ֽ���,������������㹻�Ŀռ�,=len
//-----------------------------------------------------------------------------
u32 Line_Write(struct tagLineBuf *line,u8 *buffer,u32 len)
{
    u32    wr_len;
    atom_low_t  atom_bak;
    wr_len = line->limit - line->current;
    if(wr_len == 0)
        return 0;
    if(wr_len > len)
        wr_len = len;
    memcpy( &line->buf[line->current],buffer,wr_len);
    atom_bak = Int_LowAtomStart();
    line->current += wr_len;
    Int_LowAtomEnd(atom_bak);
    return wr_len;
}

//----�����Ի�������-----------------------------------------------------------
//����: �����Ի���������ȫ�����ݣ�����λcurrentָ��
//����: line,Ŀ�����Ի������ṹָ��
//      buffer,�������ݵĻ�����ָ��
//����: ʵ�ʶ������ֽ���
//-----------------------------------------------------------------------------
u32 Line_Read(struct tagLineBuf *line,u8 *buffer)
{
    u32    wr_len;
    wr_len = line->current;
    memcpy( buffer,line->buf,wr_len);
    line->current = 0;
    return wr_len;
}

//----�����ֽڳص�ַ-----------------------------------------------------------
//����: ��ѯ���������ֽڳص�ַ�������ַ���û�����line_initʱʹ�õĻ�������ַ��
//����: line,Ŀ�����Ի�����ָ��.
//����: ���Ի��������ֽڳص�ַ
//˵��: ����������������û�������ַ������
//      1��Ҫ�ͷŻ������ڴ棬Ӧ�ó������ʹ�õ���line_initʱʹ�õ�ָ�룬�����
//         ָ�붪ʧ�����ñ�����ȡ�ظ�ָ�롣
//      2���û����Լ����ַ��ʸû��������ⲻ��һ�����ǵ�ѡ�����ƻ�ģ�������
//         �����ɣ���ʱ��ʹ����Ӧ����ȫ�����Լ��ڸ�ʲô!
//-----------------------------------------------------------------------------
u8 *Line_GetBuf(struct tagLineBuf *line)
{
    return line->buf;
}
//----��黺����������---------------------------------------------------------
//����: ���ָ�������Ի������е�������,�����ֽ���.
//����: line,Ŀ�����Ի�����ָ��.
//����: �������е�������
//-----------------------------------------------------------------------------
u32    Line_Check(struct tagLineBuf *line)
{
    return line->current;
}

//----��黺�����Ƿ��---------------------------------------------------------
//����: ���ָ�������Ի��������Ƿ��Ѿ���.
//����: line,Ŀ�����Ի�����ָ��.
//����: ���򷵻�true,�ǿշ���false
//-----------------------------------------------------------------------------
bool_t   Line_IsEmpty(struct tagLineBuf *line)
{
    return (line->current ==0)? true:false;
}

//----��黺�����Ƿ���---------------------------------------------------------
//����: ���ָ�������Ի��������Ƿ��Ѿ���.
//����: line,Ŀ�����Ի�����ָ��.
//����: ���򷵻�true,��������false
//-----------------------------------------------------------------------------
bool_t   Line_IsFull(struct tagLineBuf *line)
{
    return (line->current == line->limit)? true:false;
}

//----������Ի�����-----------------------------------------------------------
//����: �������������������
//����: line,Ŀ�����Ի�����ָ��.
//����: ��
//-----------------------------------------------------------------------------
void    Line_Flush(struct tagLineBuf *line)
{
    line->current = 0;
}

//----�����ַ�-----------------------------------------------------------------
//����: ��line��ǰ��λ�ÿ�ʼ�����ַ�c��λ��
//����: line,Ŀ�����Ի�����ָ��
//      c,����ҵ��ַ�
//����: c���ֵ�λ��,���û�г����򷵻� CN_LIMIT_UINT32
//-----------------------------------------------------------------------------
u32 Line_SearchCh(struct tagLineBuf *line, char c)
{
    u32    i;
    u8 *buf = line->buf;
    for(i=0; i<line->current; i++)
    {
        if(buf[i] == c)
            return i;
    }
    return CN_LIMIT_UINT32;
}

//----�����ַ�����-------------------------------------------------------------
//����: ��line��ǰ��λ�ÿ�ʼ�����ַ����е�λ��,�ַ����в���0����,����ָ������
//����: line,Ŀ�����Ի�����ָ��
//      string,����ҵ��ַ�����
//      str_len,�ַ����г���
//����: string���ֵ�λ�����offset_read��ƫ����,���û�г��ַ��� CN_LIMIT_UINT32
//��ע: ������ܿ��ܱȽϳ���,�����ڱ�дʱע�����ٶ��Ż�,��ȴʹ����������.
//-----------------------------------------------------------------------------
u32 Line_SearchStr(struct tagLineBuf *line, char *string,u32 str_len)
{
    u32 i,j;
    bool_t next;
    u8 *buf;

    if(line->current < str_len)
        return CN_LIMIT_UINT32;
    buf = line->buf;
    for(i=0; i <= line->current - str_len; i++)
    {
        next = false;
        for(j=0;j < str_len;j++)
        {
            if(buf[i+j] != string[j])
                next = true;
        }
        if(next == false)
        {
            return i;
        }
    }
    return CN_LIMIT_UINT32;
}

