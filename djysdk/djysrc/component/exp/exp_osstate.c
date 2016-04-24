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
// ģ������: �쳣ʱOS����״̬�Լ��ؼ�ģ�����Ϣ�Ѽ�
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 7:14:43 PM/Apr 16, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "systime.h"
#include "exp_osstate.h"
#include "endian.h"
#include "time.h"
#include "djyos.h"
#define CN_EXP_OSSTATEINFO_MAGICNUMBER  ((u32)0x11111111)
//�ܳ��쳣ʱ�Ѽ���ϵͳ��Ϣ
struct tagExpOsState
{
    u32   magicnumber;                               //ϵͳ��Ϣ��Ч��־
    u16   eventrunning_id;                         //�������е��¼�ID
    u16   evttrunning_id;                          //���¼�����Id
    char  evttrunningname[CN_EXP_NAMELEN_LIMIT];//�¼���������
    s64   exptime;                                 //�쳣ʱ��
};
static struct tagExpOsState s_tExpOsstateInfo;

// =============================================================================
// ��������:Exp_OsStateInfoGather
//         �쳣ʱ��ϵͳ������״̬
// �������:throwpara,�쳣�׳����׳����쳣��Ϣ
//          ���ݹ�����Ϊ�˸��õ��Ѽ��쳣��Ϣ
// �������:infoaddr,�洢OS״̬��Ϣ�ĵ�ַ
//          infolen,�洢OS״̬��Ϣ�ĳ���
// ����ֵ  :true�ɹ�����false ����ʧ��
// ˵��    :����쳣��ͨ����Ϣ������Ҫ���Ѽ���Ϣ�ȡ�
//          �쳣ģ���ڲ�ʹ�ã������⿪��
//          Ŀǰδ����쳣�����׳����Ѽ��ر���Ϣ����ͨ�ô���
// =============================================================================
bool_t  Exp_OsStateInfoGather(struct tagExpThrowPara *throwpara,\
                              ptu32_t *infoaddr,u32 *infolen)
{
    bool_t  result;

    if(NULL != throwpara)
    {
        s_tExpOsstateInfo.exptime = DjyGetTime();
        s_tExpOsstateInfo.eventrunning_id = Djy_MyEventId();
        s_tExpOsstateInfo.evttrunning_id  = Djy_MyEvttId();
        Djy_GetEvttName(s_tExpOsstateInfo.evttrunning_id,\
                &s_tExpOsstateInfo.evttrunningname[0],CN_EXP_NAMELEN_LIMIT);
        s_tExpOsstateInfo.magicnumber = CN_EXP_OSSTATEINFO_MAGICNUMBER;
        *infoaddr = (ptu32_t)(&s_tExpOsstateInfo);
        *infolen = sizeof(s_tExpOsstateInfo);
        result = true;
    }
    else
    {
        *infoaddr = 0;
        *infolen = 0;
        result = false;
    }
    return result;
}

// =============================================================================
// ��������:__Exp_OsstateinfoSwapByEndian
//          ת��Ӳ��CPU�쳣��Ϣ�ֽ���
// �������:osstate, �쳣OSͨ����Ϣ
// �������:osstate, �쳣OSͨ����Ϣ��ת����
// ����ֵ  :��
// ˵��    :�ڲ�����
// =============================================================================
void __Exp_OsstateinfoSwapByEndian(struct tagExpOsState *osstate)
{
    osstate->magicnumber = swapl(osstate->magicnumber);
    osstate->eventrunning_id = swaps(osstate->eventrunning_id);
    osstate->evttrunning_id = swaps(osstate->evttrunning_id);
}
// =============================================================================
// ��������:Exp_OsStateInfoDecoder
//          ���ͨ���쳣��Ϣ����
// �������:throwpara,�׳��ߵĲ���
//          infoaddr,�쳣��Ϣ�洢��ַ
//          infolen,�쳣��Ϣ����
// �������:
// ����ֵ  :true,�ɹ�����;false,����ʧ�ܣ���Ҫ����Ϊ��������
// ˵��    :�ڲ�����
// =============================================================================
bool_t  Exp_OsStateInfoDecoder(struct tagExpThrowPara *throwpara,\
                               ptu32_t infoaddr,u32 infolen,u32 endian)
{
    bool_t  result;
    char buf[100];//��Ϊ����ʱ���������Ѿ��㹻��
	struct tm  exptime;
    struct tagExpOsState osstate;
    if((NULL == throwpara)||(infoaddr == 0)||(infolen != (sizeof(s_tExpOsstateInfo))))
    {
        printk("osstate:Invalid para or incomplete info!\n\r");
        result = false;
    }
    else
    {
        memcpy((char *)(&osstate),(char *)infoaddr,infolen);
        if(((u32)CN_CFG_BYTE_ORDER) != endian)
        {
            __Exp_OsstateinfoSwapByEndian(&osstate);
        }
        if(CN_EXP_OSSTATEINFO_MAGICNUMBER == osstate.magicnumber)
        {
            printk("osstate:EventRunningId = 0x%04x\n\r",\
                                   osstate.eventrunning_id);
            printk("osstate:EvttRunningId  = 0x%04x\n\r",\
                                      (osstate.eventrunning_id)&(~CN_EVTT_ID_MASK));
            printk("osstate:EvttRunningName= %s\n\r",\
                                       osstate.evttrunningname);

            Tm_LocalTimeUs_r((s64 *)&osstate.exptime,&exptime);
            Tm_AscTime(&exptime,buf);
            printk("osstate:OsTime         = %s(DEC) \n\r",buf);
            result = true;
        }
        else
        {
            printk("osstate:DESTROYED PACKAGE \n\r");
            result = false;
        }
    }
    return result;
}

