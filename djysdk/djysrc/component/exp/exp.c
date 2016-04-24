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
// ģ������: �쳣���׳��Լ��쳣��Ϣ����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 5:59:37 PM/Apr 17, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:�����쳣��ͳһ���
// ���ļ�Ϊ�쳣���׳��������׳��쳣֮ǰ����ر�֤��Ը��쳣�����⴦���Ѿ�����
// ���ļ�ֻ�Ǹ��õĹ����쳣��Ϣ����ܹ�������ͳϽ�����쳣����
#include "config-prj.h"

#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "int.h"
#include "arch_feature.h"
#include "exp.h"
#include "endian.h"
#include "exp_api.h"
#include "exp_decoder.h"
#include "exp_hook.h"
#include "exp_osstate.h"
#include "Exp_Record.h"

//ȫ����ͬ����ô���漰����С�˵�����
#define CN_EXP_HEADINFO_MAGICNUMBER    ((u32)(0x77777777))
#define CN_EXP_PARTIALINFO_VALID       ((u32)(0x12345678))
struct tagExpHeadInfo       //todo: ����Ա����ע��
{
    u32   recordendian;
    u32   magicnumber;
    u32   osstatevalid;
    u32   osstateinfolen;
    u32   hookvalid;
    u32   hookinfolen;
    u32   hookresult;
    u32   throwinfovalid;
    u32   throwinfolen;
    u32   throwresult;
    u32   dealresult;
    char  decodername[CN_EXP_NAMELEN_LIMIT];
};//���Կ���ʹ��bitλ��ʡ�洢�ռ䣬���ǿ�������������ʡ���˼����ֽڣ��Ķ����岻��

static bool_t s_bExpMoDuleInitState = false;
// =============================================================================
// ��������:__Exp_ResultMerge
//           ��HOOK�����THROW������ۺϴ���
// �������:Result_Throw,�趨���쳣������
//          Result_Hook,HOOK���쳣������
// �������:��
// ����ֵ  :_SYSEXP_RESULT_TYPE,�쳣�Ĵ�����
// ˵��    :�ڲ����ú���
// =============================================================================
static enum _EN_EXP_DEAL_TYPE_  __Exp_ResultMerge(enum _EN_EXP_DEAL_TYPE_ Result_Throw,\
                                             enum _EN_EXP_DEAL_TYPE_ Result_Hook)
{
    enum _EN_EXP_DEAL_TYPE_ result;

    if(EN_EXP_DEAL_DEFAULT == Result_Hook)
    {
        result = Result_Throw;
    }
    else
    {
        result = Result_Hook;
    }
    return result;
}
extern void reset(void);
extern void reboot(void);
extern void restart_system(void);
// =============================================================================
// ��������:__Exp_ResultDeal
//          �����쳣�����ս��
// �������:expdealresult,�쳣���
//          expinfo,�Ѽ����쳣��Ϣ
// �������:��
// ����ֵ  :�쳣�����մ�����(��enum _EN_EXP_DEAL_REASULT����)
// ˵��    :�ڲ����ú���,����쳣���������Ҫ����������ô�Ͳ��᷵�صġ�
// =============================================================================
static u32  __Exp_ResultDeal(u32 expdealresult,\
                            struct tagExpRecordPara *expinfo)
{
    u32 result;
    if(expdealresult < EN_EXP_DEAL_RECORD)
    {
        result = EN_EXP_DEALT_IGNORE;
    }
    else
    {
        result = Exp_Record(expinfo);
        if(result != EN_EXP_DEAL_RECORD_SUCCESS)
        {
            printk("expresultdealer:RECORD FAILED!\n\r");
            result = EN_EXP_DEALT_RECORDFAILED;
#ifdef DEBUG                 //����ʱ�����
            while(1)
            {

            }
#endif
        }
        else
        {
            result = EN_EXP_DEALT_SUCCESS;
        }
        switch (expdealresult)
        {
            //�����ǲ��᷵�صģ���Ȼ��Ȼ�ǳ�����
            case EN_EXP_DEAL_RESET:
                reset();
                break;
            case EN_EXP_DEAL_REBOOT:
                reboot();
                break;
            case EN_EXP_DEAL_RESTART:
                restart_system();
                break;
            default:
                break;
        }
    }
    return result;
}
// =============================================================================
// ��������:Exp_Throw
//          ���������쳣�����
// �������:throwpara,�׳����쳣��Ϣ����
// �������:dealresult,���쳣�����մ�����
// ����ֵ  :true���ɹ��� false��ʧ��(�������ߴ洢��δ֪ԭ��)
// ˵��    :�쳣�׳���Ψһ���;���Ҫ��������ú���������
// =============================================================================
bool_t  Exp_Throw(struct tagExpThrowPara *throwpara, u32 *dealresult)
{
    bool_t result;
    u32   result_hook;
    u32   result_throw;
    u32   result_deal;
    ptu32_t infoaddr;
    u32 infolen;
    struct tagExpHeadInfo  headinfo;
    struct tagExpRecordPara recordpara;
    if(s_bExpMoDuleInitState == false)  //���δ��ʼ����֤���ܶ�ϵͳ���ö�����ʹ��
    {
        return false;
    }
    Int_SaveAsynSignal();
    if((NULL != throwpara) &&( true == throwpara->validflag))//�׳���Ч
    {
        //�׳���Ϣ����
        if(NULL != throwpara->name)
        {
            strncpy(&headinfo.decodername[0], throwpara->name, CN_EXP_NAMELEN_LIMIT);
        }
        else
        {
            headinfo.decodername[0] = '\0';
        }
        headinfo.throwinfolen = throwpara->para_len;
        headinfo.throwresult = throwpara->dealresult;
        headinfo.throwinfovalid = CN_EXP_PARTIALINFO_VALID;
        result_throw = throwpara->dealresult;
        recordpara.throwinfoaddr = (ptu32_t)(throwpara->para);
        recordpara.throwinfolen = throwpara->para_len;
        recordpara.headinfoaddr = (ptu32_t)(&(headinfo));
        recordpara.headinfolen = (u32)(sizeof(headinfo));

        //HOOK��Ϣ���Ѽ�������Ѽ����ɹ��������׳��������¼����ֱ�ӷ��أ�
        result = Exp_HookDealer(throwpara, &infoaddr, &infolen, &result_hook);
        if(false == result)
        {
            result_hook = EN_EXP_DEAL_DEFAULT;
            headinfo.hookvalid = ~CN_EXP_PARTIALINFO_VALID;
            infoaddr = 0;
            infolen = 0;
        }
        else
        {
            headinfo.hookvalid = CN_EXP_PARTIALINFO_VALID;
        }
        result_deal = __Exp_ResultMerge(result_throw, result_hook);

        if(result_deal < EN_EXP_DEAL_RECORD)
        {
            //����¼������Ļ�����ô��ֱ�ӷ��ذ�,��ֱ����Ц
            result = true;
            *dealresult = EN_EXP_DEALT_IGNORE;
            goto throw_logic_exit;
        }
        headinfo.dealresult = result_deal;
        headinfo.hookinfolen = infolen;
        headinfo.hookresult = result_hook;
        recordpara.hookinfoaddr = infoaddr;
        recordpara.hookinfolen = infolen;

        //OSSTATE��Ϣ���Ѽ�(����Ѽ����ɹ����޹ش��)
        result = Exp_OsStateInfoGather(throwpara, &infoaddr, &infolen);
        if(false == result)
        {
            infoaddr = 0;
            infolen = 0;
            headinfo.osstatevalid = ~CN_EXP_PARTIALINFO_VALID;
        }
        else
        {
            headinfo.osstatevalid = CN_EXP_PARTIALINFO_VALID;
        }
        headinfo.osstateinfolen = infolen;
        headinfo.recordendian = CN_CFG_BYTE_ORDER;
        headinfo.magicnumber = CN_EXP_HEADINFO_MAGICNUMBER;
        recordpara.osstateinfoaddr = infoaddr;
        recordpara.osstateinfolen = infolen;

        //���Ѽ�����Ϣ���Ѽ���ϣ���ô�ʹ����
        *dealresult = __Exp_ResultDeal(result_deal, &recordpara);
        result = true;
    }
    else
    {
        *dealresult = EN_EXP_DEALT_PARAERR;
        result = true;
    }
throw_logic_exit:
    Int_RestoreAsynSignal();
    return result;
}

// =============================================================================
// ��������:__Exp_HeadinfoSwapByEndian
//         ��Ϣͷ�ֽ���ת��
// �������:expinfo,�쳣��Ϣ
// �������:��
// ����ֵ  :��
// ˵��    :��expinfo����֪��Ϣת���ֽ���,name��ǰ���ֽڱ�����4�ֽڶ����
// =============================================================================
void  __Exp_HeadinfoSwapByEndian(struct tagExpHeadInfo *headinfo)
{
    u32 temp;
    u32 limit;
    u32 *addr;

    temp = (u32)headinfo;
    limit = (u32)(&(headinfo->decodername[0]));
    while(temp < limit)
    {
        addr = (u32 *)temp;
        *addr = swapl(*addr);
        temp = temp +4;
    }
}
// =============================================================================
// ��������:__Exp_HeadinfoDecoder
//          �쳣��Ϣͷ����
// �������:headinfo,�쳣��Ϣͷ
// �������:��
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :�ڲ�����
// =============================================================================
bool_t __Exp_HeadinfoDecoder(struct tagExpHeadInfo *headinfo)
{
    printk("exp_headinfo:recordendian    = 0x%08x\n\r",headinfo->recordendian);
    printk("exp_headinfo:magicnumber     = 0x%08x\n\r",headinfo->magicnumber);
    printk("exp_headinfo:osstatevalid    = 0x%08x\n\r",headinfo->osstatevalid);
    printk("exp_headinfo:osstateinfolen  = 0x%08x\n\r",headinfo->osstateinfolen);
    printk("exp_headinfo:hookvalid       = 0x%08x\n\r",headinfo->hookvalid);
    printk("exp_headinfo:hookinfolen     = 0x%08x\n\r",headinfo->hookinfolen);
    printk("exp_headinfo:hookresult      = 0x%08x\n\r",headinfo->hookresult);
    printk("exp_headinfo:throwinfovalid  = 0x%08x\n\r",headinfo->throwinfovalid);
    printk("exp_headinfo:throwinfolen    = 0x%08x\n\r",headinfo->throwinfolen);
    printk("exp_headinfo:throwresult     = 0x%08x\n\r",headinfo->throwresult);
    printk("exp_headinfo:dealresult      = 0x%08x\n\r",headinfo->dealresult);
    printk("exp_headinfo:expdecodername  = %s\n\r",headinfo->decodername);

    return true;
}

// =============================================================================
// ��������:Exp_InfoDecoder
//          ����һ���쳣��Ϣ
// �������:info,�쳣��Ϣ
//          infolen, �쳣��Ϣ����
// �������:��
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :�쳣��Ϣһ���Ѿ����������ڴ浱�У�������ܻ��޸�ԭ��Ϣ
// =============================================================================
bool_t  Exp_InfoDecoder(struct tagExpRecordPara *recordpara)
{
    bool_t  result = false;
    u32  endian;
    struct tagExpHeadInfo *headinfo;
    struct tagExpThrowPara throwpara;
    if(NULL == recordpara)
    {
        result = false;
        printk("Exp_InfoDecoder:invalid parameter!\n\r");
    }
    else if(( 0 == recordpara->headinfoaddr ) || (sizeof(struct tagExpHeadInfo) != recordpara->headinfolen))
    {
        result = false;
        printk("Exp_InfoDecoder:incomplete exception headinfo!\n\r");
    }
    else
    {
        headinfo = (struct tagExpHeadInfo *)(recordpara->headinfoaddr);

        //��С����Ϊ��׼��Ҫ����ΪС����0����С�˶�һ��
        if(((headinfo->recordendian == CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER != CN_CFG_LITTLE_ENDIAN))||\
           ((headinfo->recordendian != CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)))
        {
            //��Ҫ��С��ת��
            __Exp_HeadinfoSwapByEndian(headinfo);
        }
        if(CN_EXP_HEADINFO_MAGICNUMBER != headinfo->magicnumber)
        {
            printk("Exp_InfoDecoder:headinfo has been destroyed!\n\r");
            result = false;
        }
        else
        {
            //�����׳���Ϣ����
            endian = headinfo->recordendian;
            throwpara.dealresult = headinfo->throwresult;
            throwpara.name = headinfo->decodername;
            throwpara.para = (u8 *)(recordpara->throwinfoaddr);
            throwpara.para_len = recordpara->throwinfolen;
            throwpara.validflag = true;
            //��Ϣͷ����
            if(recordpara->headinfolen != sizeof(struct tagExpHeadInfo))
            {
                printk("Exp_InfoDecoder:headinfo incomplete!\n\r");
            }
            else
            {
                result = __Exp_HeadinfoDecoder(headinfo);
            }
            //OS״̬����,�������Լ��жϰ��������Ƿ��޸�
            if(recordpara->osstateinfolen != headinfo->osstateinfolen)
            {
                printk("Exp_InfoDecoder:osstateinfo incomplete!\n\r");
            }
            else
            {
                result = Exp_OsStateInfoDecoder(&throwpara,recordpara->osstateinfoaddr,\
                                             recordpara->osstateinfolen, endian);
            }
            //�׳��쳣����,�������Լ��жϰ��ĳ����Ƿ����������������Ƿ��ƻ�
            if(recordpara->throwinfolen != headinfo->throwinfolen)
            {
                printk("Exp_InfoDecoder:throwinfo incomplete!\n\r");
            }
            else
            {
                result = Exp_Throwinfodecoder(&throwpara,endian);
            }
            //HOOK����,�������Լ��жϰ��ĳ����Ƿ����������������Ƿ��ƻ�
            if(recordpara->hookinfolen != headinfo->hookinfolen)
            {
                printk("Exp_InfoDecoder:hookinfo incomplete!\n\r");
            }
            else
            {
                result = Exp_HookInfoDecoder(&throwpara,recordpara->hookinfoaddr,\
                                             recordpara->hookinfolen, endian);
            }
        }
    }
    return result;
}

// =============================================================================
// ��������:ModuleInstall_Exp
//         �쳣�����ʼ��
// �������:para
// �������:��
// ����ֵ  :ptu32_t ��ʱ�޶���
// ˵��    :��Ҫ�ǳ�ʼ���洢����
// =============================================================================
ptu32_t ModuleInstall_Exp(ptu32_t para)
{
    extern bool_t Exp_ShellInit();
    Exp_ShellInit();
    s_bExpMoDuleInitState = true;

    return para;
}

