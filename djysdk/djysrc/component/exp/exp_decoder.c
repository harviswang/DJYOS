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
// ģ������: �׳��쳣��Ϣ������ע�ἰ���ڲ�����ʵ��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 3:16:13 PM/May 8, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:

#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "int.h"
#include "string.h"

#include "exp_decoder.h"

static fnExp_ThrowinfoDecoderModule s_fnExpThrowinfoDecoderTab[CN_EXP_DECODERNUM_LIMIT] ={NULL};
//����������,���е��쳣����ʹ���쳣����������decoder��
//������úܶ��쳣�������Ƚ����ƣ�����ע��һ��NAME,Ȼ�����쳣��Ϣ�ڲ��Լ���
//�쳣�ŵ�����,�����е�Ӳ���쳣����ע��ΪCPU_HARD��Ȼ���Լ�����MAJOR��MINOR�Լ�����
static const char *s_pcExpThrowinfoDecoderNameTab[CN_EXP_DECODERNUM_LIMIT]= {NULL};

// =============================================================================
// ��������:__Exp_CheckDecoderName
//           �����쳣�����Ƿ��Ѿ���ע��
// �������:name,����������
// �������:
// ����ֵ  :���Ա�ע����쳣��
// ˵��    :�����decoder�������Ѿ���ע�ᣬ�򷵻���Ч�Ľ�������
// =============================================================================
u32  __Exp_CheckDecoderName(const char *name)
{
    u32 i = 0;
    u32 result = 0;
    int cmpresult;
    while(i < CN_EXP_DECODERNUM_LIMIT)
    {
        if(NULL == s_pcExpThrowinfoDecoderNameTab[i])//���õ�
        {
            result = i;
        }
        else
        {
            cmpresult = strcmp(s_pcExpThrowinfoDecoderNameTab[i],name);
            if(0 == cmpresult)
            {
                break;
            }
        }
        i++;
    }
    if(i < CN_EXP_DECODERNUM_LIMIT)//��������
    {
        result = CN_EXP_DECODERNUM_LIMIT;
    }
    return result;
}
// =============================================================================
// �������ܣ�__Exp_FindDecoderNoByName
//          �����ֲ����쳣��������
// �������:name,ע����쳣����
// ���������
// ����ֵ  :��Ӧ���ֵ��쳣�ţ�������Ч�쳣��
// ˵��    ��
// =============================================================================
u32  __Exp_FindDecoderNoByName(char *name)
{
    u32 i = 0;
    int cmpresult;
    while(i < CN_EXP_DECODERNUM_LIMIT)
    {
        if(NULL != s_pcExpThrowinfoDecoderNameTab[i])//���õ�
        {
            cmpresult = strcmp(s_pcExpThrowinfoDecoderNameTab[i],name);
            if(0 == cmpresult)
            {
                break;
            }
        }
        i++;
    }
    return i;
}
// =============================================================================
// ��������:Exp_Throwinfodecoder
//          �׳��쳣��Ϣ����
// �������:parahead,�׳��쳣ʱ����
//          endian, ��Ϣ�Ĵ�С��
// ���������
// ����ֵ  :true,�ɹ�����;false,����ʧ�ܣ���Ҫ����Ϊ��������
// ˵��    :�ڲ�����
// =============================================================================
bool_t  Exp_Throwinfodecoder(struct tagExpThrowPara *parahead,u32 endian)
{
    bool_t result;
    u32 decoder_num;

    decoder_num = __Exp_FindDecoderNoByName(parahead->name);
    if(decoder_num != CN_EXP_DECODERNUM_LIMIT)//ע���У���ô�ͽ�����
    {
        result = s_fnExpThrowinfoDecoderTab[decoder_num](parahead,endian);
    }
    else
    {
        result = false;
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RegisterThrowinfoDecoder
//          ע������쳣��Ϣ������
// ���������decoder,�쳣������
//          name,�쳣����������,���ٱ�֤��ȫ�ֵ��Ҳ�����
// ���������
// ����ֵ  ��true,�ɹ�ע��;false,ע��ʧ�ܣ�
// ˵��    ��
//          1,����Ӧ���쳣���Ѿ���ע���˵�ʱ�򣬻����δ��ע����쳣�Ž���ע��
//          2,���������Ѿ����ڶ�Ӧ�������쳣�ᵼ��ע��ʧ���(ֻ�ǡ�\0��Ҳ����Ч��)
//          3,��ע��ʧ�ܵĿ��ܣ����ע���鷵�ؽ��
//          4,Ϊ�˱�֤�ú�����ͨ���ԣ��������ԭ�Ӳ�������ͬ����
// =============================================================================
bool_t Exp_RegisterThrowinfoDecoder(fnExp_ThrowinfoDecoderModule decoder,const char *name)
{
    bool_t  result;
    u32 num_search;
    atom_low_t  atom2operate;

    if((NULL == name)||(NULL == decoder) ||('\0')== *name)
    {
        printk("Exp_RegisterThrowinfoDecoder:Invalid parameter!\n\r");
        return false;
    }
    atom2operate = Int_LowAtomStart();

    num_search = __Exp_CheckDecoderName(name);
    if(num_search != CN_EXP_DECODERNUM_LIMIT)//������Ч
    {
        s_fnExpThrowinfoDecoderTab[num_search] = decoder;
        s_pcExpThrowinfoDecoderNameTab[num_search] = name;
        result = true;
    }
    else
    {
        result = false;
    }
    Int_LowAtomEnd(atom2operate);

    return result;
}

// =============================================================================
// ��������:Exp_UnRegisterThrowinfoDecoder
//          ע������쳣��Ϣ������
// �������:name,�Ѿ���ע����쳣����
// ���������
// ����ֵ  :true,�ɹ�ע��;false,ע��ʧ�ܣ�
// ˵��    ��
// =============================================================================
bool_t Exp_UnRegisterThrowinfoDecoder(char *name)
{
    bool_t  result;
    u32 num_search;
    atom_low_t  atom2operate;

    if(NULL == name)
    {
        printk("Exp_UnRegisterThrowinfoDecoder:Invalid parameter!\n\r");
        result = false;
    }
    else
    {
        atom2operate = Int_LowAtomStart();

        num_search = __Exp_FindDecoderNoByName(name);
        if(num_search != CN_EXP_DECODERNUM_LIMIT)//������Ч
        {
            s_fnExpThrowinfoDecoderTab[num_search] = NULL;
            s_pcExpThrowinfoDecoderNameTab[num_search] = NULL;
            result = true;
        }
        else
        {
            result = false;
        }
        Int_LowAtomEnd(atom2operate);
    }
    return result;
}

