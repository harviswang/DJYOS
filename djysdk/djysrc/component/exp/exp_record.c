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
// ģ������: �쳣��Ϣ�洢�����ӿ�
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 10:27:22 AM/Mar 26, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:�û�ע����쳣��Ϣ������

#include "stdint.h"
#include "stddef.h"
#include "exp_api.h"
#include "exp_record.h"

static struct tagExpRecordOperate s_tExpRecordOpt = {NULL};
static struct tagExpRecordOperate s_tExpRecordDefaultOpt = {NULL};

// =============================================================================
// �������ܣ�__memcpyByteByByte
//           �ڴ濽��
// ���������
// ���������
// ����ֵ  ����_EN_EXP_RECORDRESULT����
// =============================================================================
void __memcpyByteByByte(u8 *dest, u8 *src, u32 len)
{
    u32 i = 0;
    while(i < len)
    {
        *dest = *src;
        dest++;
        src++;
        i++;
    }
}

// =============================================================================
// �������ܣ�Exp_Record
//           ��¼һ֡�쳣��Ϣ
// ���������recordpara����Ҫ��¼���쳣��Ϣ
// ���������
// ����ֵ  ����_EN_EXP_RECORDRESULT����
// =============================================================================
u32 Exp_Record(struct tagExpRecordPara *recordpara)
{
    u32 result;
    if(NULL != s_tExpRecordOpt.fnexprecord)
    {
        result = (u32)(s_tExpRecordOpt.fnexprecord(recordpara));
    }
    else
    {
        result = (u32)EN_EXP_DEAL_RECORD_NOMETHOD;
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RecordClear
//          ������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��true �ɹ��� falseʧ��
// =============================================================================
bool_t Exp_RecordClear(void)
{
    if(NULL != s_tExpRecordOpt.fnexprecordclear)//������ע���
    {
        return s_tExpRecordOpt.fnexprecordclear();
    }
    else//Ĭ�Ϸ�ʽ����
    {
        return false;
    }
}
// =============================================================================
// �������ܣ�Exp_RecordCheckNum
//          �鿴һ���洢�˶������쳣��Ϣ
// �����������
// ���������recordnum,���ص��쳣��Ϣ��Ŀ��
// ����ֵ  ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t Exp_RecordCheckNum(u32 *recordnum)
{
    if(NULL != s_tExpRecordOpt.fnexprecordchecknum)//������ע���
    {
        return s_tExpRecordOpt.fnexprecordchecknum(recordnum);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        *recordnum = 0;
        return false;
    }
}

// =============================================================================
// �������ܣ�Exp_RecordCheckLen
//          �鿴ָ����Ŀ���쳣��Ϣ�ĳ���
// ���������assignedno,ָ�����쳣֡��Ŀ
// ���������recordlen,���ڴ洢ָ���쳣��Ŀ�ĳ���
// ����ֵ  ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t Exp_RecordCheckLen(u32 assignedno, u32 *recordlen)
{
    bool_t result;
    if(NULL != s_tExpRecordOpt.fnexprecordchecklen)//������ע���
    {
        result = s_tExpRecordOpt.fnexprecordchecklen(assignedno, recordlen);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        result = false;
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RecordGet
//          �Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ(�ڼ����쳣��Ŀ)
//          buflenlimit,ϵͳ�쳣֡�ĳ��ȣ����Ƴ�������Ϊ���⿽��Խ��
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t Exp_RecordGet(u32 assignedno, u32 buflenlimit, u8 *buf, \
                     struct tagExpRecordPara  *recordpara)
{
    bool_t result;
    if(NULL != s_tExpRecordOpt.fnexprecordget)//������ע���
    {
        result = s_tExpRecordOpt.fnexprecordget(assignedno, buflenlimit,buf,recordpara);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        result = false;
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RegisterRecordOpt
//          ע���쳣��Ϣ������
// ���������opt,��Ҫע����쳣��Ϣ������
// �����������
// ����ֵ  ��false,ʧ��  true�ɹ���ʧ�ܵĻ���ʹ��BSPĬ�ϵĴ�����
// ע��    ��������opt�ṹ����ָ���Ĵ�������Ӧ���ṩ������Ļ���ע�᲻�ɹ�
//           OPT�ĸ������������Լ��ṩ����У���
// =============================================================================
bool_t  Exp_RegisterRecordOpt(struct tagExpRecordOperate *opt)
{
    bool_t result;
    if(NULL == opt)
    {
        result = false;
    }
    else
    {
        if((NULL == opt->fnexprecord) ||\
            (NULL == opt->fnexprecordchecklen)||\
            (NULL == opt->fnexprecordchecknum)||\
            (NULL == opt->fnexprecordclear)||\
            (NULL == opt->fnexprecordget))
        {
            result = false;
        }
        else
        {
            //ע��
            __memcpyByteByByte((u8 *)&s_tExpRecordOpt, \
                               (u8 *)opt, sizeof(s_tExpRecordOpt));
            //��һ�εĻ���������
            if(NULL == s_tExpRecordDefaultOpt.fnexprecord)
            {
                __memcpyByteByByte((u8 *)&s_tExpRecordDefaultOpt, \
                                   (u8 *)&s_tExpRecordOpt, \
                                   sizeof(s_tExpRecordOpt));
            }
            //�洢������ʼ��ɨ��
            if(NULL != s_tExpRecordOpt.fnexprecordscan)
            {
                s_tExpRecordOpt.fnexprecordscan();
            }
            result = true;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_UnRegisterRecordOpt
//           ע���쳣��Ϣ������
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// =============================================================================
bool_t Exp_UnRegisterRecordOpt(void)
{
    //�ָ���Ĭ�ϵĴ洢����
    __memcpyByteByByte((u8 *)&s_tExpRecordOpt, \
                       (u8 *)&s_tExpRecordDefaultOpt, \
                       sizeof(s_tExpRecordOpt));
    //�洢������ʼ��ɨ��
    if(NULL != s_tExpRecordOpt.fnexprecordscan)
    {
        s_tExpRecordOpt.fnexprecordscan();
    }
    return true;
}

