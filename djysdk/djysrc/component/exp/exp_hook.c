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
// ģ������: �쳣�Ĺ��Ӵ�����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 2:29:24 PM/Apr 17, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:��Ҫ�����ӵ�ע��͵���
#include "config-prj.h"

#include "stdint.h"
#include "stddef.h"
#include "exp_api.h"
#include "exp_hook.h"

static fnExp_HookDealermodule   s_fnExpHookDealer = NULL;
static fnExp_HookInfoDecodermodule  s_fnExpHookInfoDecoder = NULL;

// =============================================================================
// ��������:Exp_HookDealer
//          ִ��HOOKDealer
// �������:throwpara,�쳣�׳����׳����쳣����
// �������:infoaddr, �洢�쳣��Ϣ�ĵ�ַ
//          infolen,�洢�Ѽ���Ϣ����
//          dealresult,�洢����ṹ����û��HOOKʱ����ֵ��Ч
// ����ֵ  :true�ɹ�  false ʧ�ܣ�û��ע������أ����ӽ�����ò��ɣ�
// =============================================================================
bool_t Exp_HookDealer(struct tagExpThrowPara *throwpara,\
                          ptu32_t *infoaddr,u32 *infolen,u32 *dealresult)
{
    if(NULL != s_fnExpHookDealer)
    {
        *dealresult = s_fnExpHookDealer(throwpara,infoaddr,infolen);
        return true;
    }
    else
    {
        *infoaddr = (ptu32_t)NULL;
        *infolen = 0;
        *dealresult = EN_EXP_DEAL_DEFAULT;
        return false;
    }
}
// =============================================================================
// ��������:Exp_HookInfoDecoder
//          ִ��HOOKDecoder
// �������:throwpara, �쳣������hook�ܽ�ͽ⣬���ܽ������
//          infoaddr, ��Ϣ�洢��ַ
//          infolen, ��Ϣ��Ч����
//          endian,�洢ʱ��Ĵ�С��
// �������:��
// ����ֵ  :true�ɹ�  false ʧ�ܣ�û��ע������أ�
// =============================================================================
bool_t Exp_HookInfoDecoder(struct tagExpThrowPara *throwpara,\
                           ptu32_t infoaddr, u32 infolen,u32 endian)
{
    if(NULL != s_fnExpHookInfoDecoder)
    {
         return  s_fnExpHookInfoDecoder(throwpara,infoaddr,infolen,endian);
    }
    else
    {
        return false;
    }
}

// =============================================================================
// ��������:Exp_RegisterHook
//          ע��APP�ṩ���쳣����HOOK
// �������:fnappdealer�ṩ���쳣������
//          fnappdecoder�ṩ���쳣��Ϣ������
// �������:��
// ����ֵ  ��true�ɹ�  falseʧ��
// =============================================================================
bool_t Exp_RegisterHook(fnExp_HookDealermodule  fnapphookdealer,\
                            fnExp_HookInfoDecodermodule fnapphookdecoder)
{
    s_fnExpHookDealer = fnapphookdealer;
    s_fnExpHookInfoDecoder = fnapphookdecoder;
    return true;
}

// =============================================================================
// ��������:Exp_UnRegisterHook
//          ע��APP�ṩ���쳣����HOOK�Լ��쳣��Ϣ������
// �������:��
// �������:��
// ����ֵ  :true�ɹ�  falseʧ��
// =============================================================================
bool_t Exp_UnRegisterHook(void)
{
    s_fnExpHookDealer = NULL;
    s_fnExpHookInfoDecoder = NULL;

    return true;
}

