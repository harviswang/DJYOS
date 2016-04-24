//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: �ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: �ַ�������Դ��ʼ��
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "rsc.h"
#include "stdio.h"

#include "charset.h"

static struct tagCharset *s_ptCurCharEncoding;    // ������Դ
//----��װ�ַ�����-------------------------------------------------------------
//����: �����ַ����밲װ��ϵͳ��Դ������
//����: encoding���������ַ�������Դָ��
//      name�������ַ�������
//����: true
//-----------------------------------------------------------------------------
bool_t  Charset_NlsInstallCharset(struct tagCharset *encoding, char* name)
{
    struct tagRscNode *rsc;
    rsc = Rsc_SearchTree(CN_CHAR_ENCODING_RSC_TREE);
    if(rsc == NULL)
        return false;       //�ַ���Դ�����δ����

    Rsc_AddSon(rsc,&encoding->node,sizeof(struct tagCharset),RSC_CHARSET,name);

    return true;
}

//----�ַ�����ģ���ʼ��-------------------------------------------------------
//����: ��ʼ���ַ�����ģ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Charset(ptu32_t para)
{
    static struct tagRscNode encoding_root_rsc;    // ������Դ

    s_ptCurCharEncoding = NULL;
    // ����ַ�������Դ���ڵ�
    if(Rsc_AddTree(&encoding_root_rsc,
                            sizeof(struct tagRscNode),RSC_RSCNODE,
                            CN_CHAR_ENCODING_RSC_TREE))
    {
        return 1;
    }else
    {
        printf("install charset module fail\r\n");
        return 0;
    }
}

//----��ȡ��ǰ�ַ�����---------------------------------------------------------
//����: ��ȡ��ǰʹ�õ��ַ�����
//����: ��ǰ�ַ�����
//-----------------------------------------------------------------------------
struct tagCharset* Charset_NlsGetCurCharset(void)
{
    return s_ptCurCharEncoding;
}

//----�趨��ǰ�ַ�����---------------------------------------------------------
//����: �����ַ�������Ϊ��ǰʹ�õ��ַ�����,���ַ�����������Ȱ�װ��ϵͳ�У����
//      encoding��Դδ��װ��������ʲô������������ԭ���ĵ�ǰ�ַ�����
//����: encoding��ָ�����ַ�����
//����: NULL���趨ʧ��
//      �趨֮ǰ���ַ�����
//-----------------------------------------------------------------------------
struct tagCharset* Charset_NlsSetCurCharset(
                                struct tagCharset* encoding)
{
    struct tagRscNode *rsc;
    if(encoding == NULL)
        return NULL;
    rsc = Rsc_SearchTree(CN_CHAR_ENCODING_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //�ַ���Դ��δ����
    rsc = Rsc_SearchSon(rsc,encoding->node.name);
    if(rsc != NULL)
    {
        s_ptCurCharEncoding = (struct tagCharset*)rsc;
    }
    return s_ptCurCharEncoding;
}

//----�����ַ�������Դ---------------------------------------------------------
//����: �������������ַ�������Դ
//����: name��ָ�����ַ���������
//����: NULL���޴˱�����Դ
//      Ҫ�ҵ��ַ�������Դ
//-----------------------------------------------------------------------------
struct tagCharset* Charset_NlsSearchCharset(const char* name)
{
    struct tagRscNode *rsc;

    rsc = Rsc_SearchTree(CN_CHAR_ENCODING_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //�ַ���Դ��δ����

    return (struct tagCharset*)Rsc_SearchSon(rsc, (char *)name);
}

