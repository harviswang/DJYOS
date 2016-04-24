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
// ģ������:����P1020��DJYOS���жϿ�����ʵ�֣��жϵײ������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:zhangqf_cyg
// ����ʱ��:2013-7-26--����10:15:29
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע�����ļ�ʵ�ֵĶ���ӿں���������ֲ���е�
//       ���������Ǹ���API��ͨ�õģ���Υ���ù���ĵ��øŲ����𣬸���API�����ظ�˵����
//     1���������жϺ�ʱ����ر�֤���жϺ�����ʹ�õķ�Χ֮��ģ���Ȼ�Ļ��ܿ��������쳣
//
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "os.h"
#include "Pic.h"
#include "cpu.h"
#include "int_hard.h"

extern const ufast_t tg_IntUsed[];          //save the int number used  
extern const ufast_t tg_IntUsedNum;         //��������int_config.c�ж���
extern ufast_t tg_int_lookup_table[];       //�ж��߲��ұ�
extern struct tagIntLine *tg_pIntSrcTable;   //��Int_Init�����з����ڴ�
struct tagIntMasterCtrl  tg_int_global;     //���岢��ʼ�����жϿ��ƽṹ
//P1020��PICʵ���ǹ��ڸ��ӣ������жϵĿ������Ĵ���λ�ò��Ǻܹ��ɣ��ش˿���
//������������ַ
static u32 sgPicIvprAddrTab[CN_INT_LINE_LAST+1]; //IVPR��ַ
static u32 sgPicIdrAddrTab[CN_INT_LINE_LAST+1];  //IDR��ַ

// =============================================================================
// �������ܣ�__Int_ClearAllLine
//          ������й�����ж�
// �����������
// �����������
// ����ֵ     ��true�ɹ�  falseʧ��
// ˵��          ��
// =============================================================================
void __Int_ClearAllLine(void)
{
    //may be we could use the eoi to do some thing,another problem is that you should
    //clear the int signal as well(IACK)
    u32 irq_number;
    u32 frr;
    u32 frrvalue;
    u32 i;
    frr = cn_pic_frr_reg;
    frrvalue = read32(frr);
    irq_number = (frrvalue >> cn_pic_frr_nirq_shift) & cn_pic_frr_nirq_mask;
    for (i =0; i< irq_number; i++)
    {
        //��IACK дEOI�������й�����ж϶��ҷ�����
        read32(cn_core_iack_addr);
        write32(cn_int_eoi_addr, 0);
    }
    return;
}
// =============================================================================
// �������ܣ�__Int_GetIvpr
//          ��ȡ��Ӧ�жϺŵ�IVPR��ַ
// ���������
//          ufl_line,�жϺ�
// ���������addr,�洢IVPR�ĵ�ַ
// ����ֵ     ��true�ɹ�  falseʧ��
// ˵��          ������û�л�ȡ�жϼĴ�����ַʱ���ø÷��������Ѿ���ʼ���ж�������ַ����
//           ʹ�ø÷�������Ҫ�����жϳ�ʼ������
// =============================================================================
bool_t __Int_GetIvpr(ufast_t ufl_line,u32 *addr)
{
    ufast_t   int_offset;//����ڸ��ж����ͣ�Ƭ���жϡ�Ƭ���жϵȣ���ƫ����
    u32  ivpr_addr = 0;//�Ĵ�����ַ

    if(ufl_line < cn_int_line_msgbase)//external interrupt
    {
        int_offset = ufl_line - cn_int_line_extbase;
        ivpr_addr = (u32)(cn_int_ext_baddr + int_offset*2*cn_int_register_step);
    }
    else if(ufl_line < cn_int_line_smsgbase)//msg
    {
        int_offset = ufl_line - cn_int_line_msgbase;
        ivpr_addr = (u32)(cn_int_msg_baddr + int_offset*2*cn_int_register_step);
    }
    else if(ufl_line < cn_int_line_ipibase0)//share msg
    {
        int_offset = ufl_line - cn_int_line_smsgbase;
        ivpr_addr = (u32)(cn_int_smsg_baddr + int_offset*cn_int_register_step);
    }
    else if(ufl_line < cn_int_line_ipibase1)//ipi0
    {
        int_offset = ufl_line - cn_int_line_ipibase0;
        ivpr_addr = (u32)(cn_int_ipi_vp_baddr+ int_offset*cn_int_register_step);
    }
    else if(ufl_line < cn_int_line_gbtimer_abase)//ipi1,use the same pv register
    {
        int_offset = ufl_line - cn_int_line_ipibase1;
        ivpr_addr =(u32)(cn_int_ipi_vp_baddr+ int_offset*cn_int_register_step);
    }
    else if(ufl_line < cn_int_line_gbtimer_bbase)//gbtimer_groupa
    {
        int_offset = ufl_line - cn_int_line_gbtimer_abase;
        ivpr_addr = (u32)(cn_int_gbtimer_ga_baddr+ int_offset*4*cn_int_register_step);
    }
    else if(ufl_line < cn_int_line_interbase)//gbtimer_groupb
    {
        int_offset = ufl_line - cn_int_line_gbtimer_bbase;
        ivpr_addr = (u32)(cn_int_gbtimer_gb_baddr+ int_offset*4*cn_int_register_step);
    }

    else if(ufl_line <= CN_INT_LINE_LAST)//interal interrupt
    {
        int_offset = ufl_line - cn_int_line_interbase;
        ivpr_addr = (u32)(cn_int_interal_baddr + int_offset*2*cn_int_register_step);
    }
    else
    {
        ivpr_addr = 0;//invalid uf_line;
    }
    if(0 != ivpr_addr)
    {
        *addr = ivpr_addr;
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// �������ܣ�__Int_RstPic
//          ��λPIC������
// �����������
// �����������
// ����ֵ     ����
// ˵��          ��ֱ��PIC������ŷ���
// =============================================================================
void __Int_RstPic(void)
{
    u32 value;
    u32 addr;

    addr = cn_pic_gcr;
    write32(addr,cn_pic_gcr_rst);
    value = read32(addr);
    while(value & cn_pic_gcr_rst)//�����ɹ�
    {
        value = read32(addr);
    }
    return;
}
// =============================================================================
// �������ܣ�int_init
//          �жϳ�ʼ��
// �����������
// �����������
// ����ֵ     ��true�ɹ�  falseʧ��
// ˵��          �� 1.��ʼ���ж�Ӳ��,��ʼ���ж������ݽṹ
//           2.�첽�źű��ֽ�ֹ,�������߳����������д�.
//           3.���ж�����
//           4.��ʼ�������е��жϣ�Ӳ����������ֻ��ʹ�õ��жϲŻ��п��ƽṹ��
//            �û���ʼ������Ӧ���������¹���:
//           1.ϵͳ��ʼʱ���Ѿ���ֹ�����첽�ź�,�û���ʼ��ʱ���뵣���첽�źŷ���.
//           2.��ʼ�������������Ҫ�������ж�/ʵʱ�ж�/�첽�ź�,Ӧ�óɶ�ʹ��.��ֹʹ
//             �첽�ź�ʵ�ʴ�������״̬(���첽�����жϿ���ͬʱ����).
//           3.���Բ����ж���,�������ӡ�������ֹ��,��Ӧ�óɶ�ʹ��.
//           4.����ʹ��save/restore������,��Ҫʹ��enable/disable������.
// =============================================================================
void Int_Init(void)
{
    bool_t result_bool;
    ufast_t i;
    ufast_t ufl_line;
    u32 value;
    u32 addr;
    u32 len ;
    u8 *src;
    
    Int_CutTrunk();//close the main int key
    len = tg_IntUsedNum*sizeof(struct tagIntLine );
    tg_pIntSrcTable = M_Malloc(len,CN_TIMEOUT_FOREVER);
    if(tg_pIntSrcTable == NULL)
    {
        return;
    }
    else
    {
    	src = (u8 *)tg_pIntSrcTable;
    	for(i =0; i < len;i++)
    	{
    		*src = 0;
    		src++;
    	}
    }
    __Int_RstPic();
    //��ʼ�����ұ�
    for(i=0;i <= CN_INT_LINE_LAST;i++)
    {
        tg_int_lookup_table[i] = (ufast_t)CN_LIMIT_UFAST;
    }
    //��tg_int_lookup_table���tg_int_table����ӳ��
    for(i=0;i < tg_IntUsedNum;i++)
    {
        tg_int_lookup_table[tg_IntUsed[i]] = i;
        tg_pIntSrcTable[i].en_counter = 1;               //��ֹ�ж�,����Ϊ1
        tg_pIntSrcTable[i].int_type = CN_ASYN_SIGNAL;    //��Ϊ�첽�ź�
        tg_pIntSrcTable[i].clear_type = CN_INT_CLEAR_PRE;//��Ϊ����ISRǰӦ��
        //�����жϺ���ָ��ָ��պ���
        tg_pIntSrcTable[i].ISR = (u32 (*)(ufast_t))NULL;
        tg_pIntSrcTable[i].sync_event = NULL;                //ͬ���¼���
        tg_pIntSrcTable[i].my_evtt_id = CN_EVTT_ID_INVALID;  //�������¼�
        tg_pIntSrcTable[i].prio = cn_prior_asyn_signal;      //�첽�źŵĹ̶����ȼ�
    }
    //�����е��жϽ��г�ʼ������¼��Ĵ�����ַ
    for(ufl_line=0;ufl_line <= CN_INT_LINE_LAST;ufl_line++)
    {
        result_bool =  __Int_GetIvpr(ufl_line, &addr);
        if(result_bool)
        {
            sgPicIvprAddrTab[ufl_line] = addr;
            sgPicIdrAddrTab[ufl_line] = addr + cn_pic_ivpr2idr;
            //����IVPR,ֻ��Ҫ����VECTOR�������ͻ���CTPR=0��MSK=0��LEVE=0
            value = ufl_line;
            value = value & cn_ivpr_vector_msk;
            write32(addr,value );
            //����IDR,������ΪINT0
            addr = addr + cn_pic_ivpr2idr;
            value = cn_pic_idr_p0;
            write32(addr,value);
        }
    }
    for(ufl_line=0; ufl_line < CN_INT_BITS_WORDS; ufl_line++)
    {
        //����λͼ����,ȫ����Ϊ�첽�źŷ�ʽ
        tg_int_global.property_bitmap[ufl_line] = 0;
        //�ж�ʹ��λͼ��0,ȫ�����ڽ�ֹ״̬
        tg_int_global.enable_bitmap[ufl_line] = 0;
    }

    tg_int_global.nest_asyn_signal =0;
    tg_int_global.nest_real=0;

    tg_int_global.en_asyn_signal_counter = 1;   //�첽�źż���
    __Int_ClearAllLine();//clear all the suspend interrupt
    
    Int_CutAsynSignal();
    //set the cpu ctpr to zero
    write32(cn_core_ctpr_addr,cn_prior_core_event);
//  //��ʼ�����еĶ�ʱ��
//  extern void module_init_pictimer(void);
//  module_init_pictimer();
    //set the spuriouse vector
    write32(cn_pic_svr, cn_pic_svr_vector);
    //set the pic to the mixed mode
    write32(cn_pic_gcr_addr,cn_pic_gcr_mode_m);
//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;       //���жϼ���  
    Int_ContactTrunk(); //��ͨ���жϿ���   
}


bool_t __Int_ChkIntValid(ufast_t ufl_line)
{
	bool_t result = false;
	if(ufl_line <= CN_INT_LINE_LAST)
	{
		result = true;
	}
	return result;
}
// =============================================================================
// �������ܣ�int_set_line_triger_type
//          �����жϵĴ�����ʽ
// ���������ufl_line,�жϺ�
//           mode,��enum_int_trigertype�е�ö�ٶ���
// �����������
// ����ֵ     ��
// ˵��          ����Ч�жϺŻ���û��ʹ�õ��жϻ���û�г�ʼ�����ж϶����᲻�ɹ��������쳣
// =============================================================================
bool_t Int_SetLineTrigerType(ufast_t ufl_line, ufast_t mode)
{
    bool_t result =false;
    u32 addr;
    u32 value;
    if( __Int_ChkIntValid(ufl_line) && (mode <=EN_INT_TRIGER_TAP))
    {
        addr = sgPicIvprAddrTab[ufl_line];
        value = read32(addr);
        value = value & (~cn_ivpr_trigertype_msk);
        result = true;
        switch (mode)
        {
            case EN_INT_TRIGER_HIGHLEVEL:
                value = value |(cn_ivpr_sense_msk | cn_ivpr_polarity_msk);
                write32(addr,value);
                break;
            case EN_INT_TRIGER_LOWLEVEL:
                value = value | cn_ivpr_sense_msk ;
                write32(addr,value);
                break;
            case EN_INT_TRIGER_RISINGEDGE:
                value = value | cn_ivpr_polarity_msk ;
                write32(addr,value);
                break;
            case EN_INT_TRIGER_FALLINGEDGE:
                write32(addr,value);
                break;
            case EN_INT_TRIGER_TAP:
            default:
                result = false;
                break;
        }
    }

    return result;
}

// =============================================================================
// �������ܣ�int_contact_line
//          ʹ���ж�
// ���������ufl_line���жϺ�
// �����������
// ����ֵ     ��true�ɹ�  falseʧ��
// ˵��          ���жϵ�ʹ�������β��õ��Ƕ������ȼ��Ĳ���
//          ���ֱ��ʹ��MSK���������ڹر��жϵĹ����е��ж϶�ʧ��������OS����
//          ֱ�ӻظ����жϵ�PRIOR����
// =============================================================================
bool_t  Int_ContactLine(ufast_t ufl_line)
{
    bool_t result = false;
    u32 addr;
    u32 value = 0;
    u32 prior = 0;

    if(__Int_ChkIntValid(ufl_line))
    {
        addr =sgPicIvprAddrTab[ufl_line];
        value = read32(addr);
		//�첽�źź�ʵʱ�жϵĹ��жϷ�ʽ��һ��,�첽�ź���prior���ƣ�ʵʱ�ж���MSK
    	if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
    	{
            value = value &(~cn_ivpr_prior_msk);
            prior = tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].prio<<16;
            prior = prior&cn_ivpr_prior_msk;
            value = value|prior;
            value = value &(~cn_ivpr_en_msk); //alse must clear the msk
    	}
    	else
    	{
            value = value &(~cn_ivpr_en_msk);
    	}
        write32(addr, value);
        result = true;
    }

    return result;
}

// =============================================================================
// �������ܣ�Int_CutLine
//          �����ж�
// ���������ufl_line���жϺ�
// �����������
// ����ֵ     ��true�ɹ�  falseʧ��
// ˵��          ���ر��ж�Դ�����ý����ж�Դ�����ȼ��Ĳ���
//          ���ֱ��ʹ��MSK���������ڹر��жϵĹ����е��ж϶�ʧ��������OS����
//          ����PRIOE����Ϊ0����
// =============================================================================
bool_t  Int_CutLine(ufast_t ufl_line)
{
    bool_t result = false;
    u32 addr;
    u32 value = 0;
    
    if(__Int_ChkIntValid(ufl_line))
    {
        addr =sgPicIvprAddrTab[ufl_line];
        value = read32(addr);
		//�첽�źź�ʵʱ�жϵĹ��жϷ�ʽ��һ��,�첽�ź���prior���ƣ�ʵʱ�ж���MSK
    	if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
    	{
            value = value &(~cn_ivpr_prior_msk);
    	}
    	else
    	{
            value = value |cn_ivpr_en_msk;
    	}
        write32(addr, value);
        result = true;
    }

    return result;
}
// =============================================================================
// �������ܣ�Int_GetEIntNumber ��ȡ�첽�źź���
// ���������
// ���������
// ����ֵ    ���жϺţ�������Ӧ���жϵĺ���
// ˵��       ������������ֲ���к���
//          ��E500V2ϵ�д������У��üĴ������ܱ���ȡ֮���Զ���λ;����������ж�״̬��ȡ��
//          �Ĵ�������ȡ�Ľ�������Чֵ
// =============================================================================
ufast_t  Int_GetEIntNumber(void)
{
	u32       addr;
	ufast_t   result;
	
	addr = cn_core_iack_addr;
	result = read32(addr);
	return result;
}
// =============================================================================
// �������ܣ�Int_GetCIntNumber  ��ȡʵʱ�źź���
//          ����ж�
// ���������
// ���������
// ����ֵ    ���жϺţ�������Ӧ���жϵĺ���
// ˵��       ������������ֲ���к���
//          ��E500V2ϵ�д������У��üĴ������ܱ���ȡ֮���Զ���λ;����������ж�״̬��ȡ��
//          �Ĵ�������ȡ�Ľ�������Чֵ
// =============================================================================
ufast_t  Int_GetCIntNumber(void)
{
	u32       addr;
	ufast_t   result;
	
	addr = cn_core_iack_addr;
	result = read32(addr);
	return result;
}

// =============================================================================
// �������ܣ�Int_ClearLine
//          ����ж�
// ���������ufl_line���жϺ�
// �����������
// ����ֵ     ��true�ɹ�  falseʧ��
// ˵��          ��Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ��������ǰ��������Ӧͬһ�ж���
//          �ĺ����жϣ�����󣬲ſ�����Ӧ�����жϡ�����������ж��߱�����Ϊʵʱ�ж�
//          �����첽�ź��޹�.
//��ע          ��P1020�У�����supriouse�жϺ�ʱ����дEOI
//          ����������ֲ���к���
// =============================================================================
bool_t Int_ClearLine(ufast_t ufl_line)
{
    bool_t result = false;
    u32 value;
    u32 addr; 
    
    if(__Int_ChkIntValid(ufl_line))
    {
       if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_ASYN_SIGNAL)
       {
           addr = sgPicIvprAddrTab[ufl_line];
           value = read32(addr);
           if(value&cn_ivpr_activity_msk) //�ڹ���״̬
           {
               write32(cn_int_eoi_addr, 0);
           }
       }
       result = true;
    }

    return result;
}

// =============================================================================
// �������ܣ�Int_TapLine
//          �����ж�
// ���������ufl_line���жϺ�
// �����������
// ����ֵ     ��true�ɹ�  falseʧ��
// ˵��          ����ֲ���У���P1020�����Ƿ���ʧ�ܣ�δʵ�֣�
// =============================================================================
bool_t Int_TapLine(ufast_t ufl_line)
{
    return false;
}
// =============================================================================
// �������ܣ�int_query_line
//           ��ѯ�ж�״̬�������������
// ���������ufl_line,ָ���жϺ�
// �����������
// ����ֵ     ��true���ж��ڴ���״̬   ����false
// ˵��          ����ֲ���к���,
//          ��ѯ�������Ӧ�ж���״̬�������ڲ�ѯʽ�жϳ���
// ��ע         : ��Ӳ���ṹ���,��Щ�ṹ���ܲ��ṩ�������,����!
// =============================================================================
bool_t Int_QueryLine(ufast_t ufl_line)
{
   bool_t result = false;
   u32 addr;
   u32 value;

   if(__Int_ChkIntValid(ufl_line))
   {
       addr = sgPicIvprAddrTab[ufl_line];
       value = read32(addr);
       result = false;
       while(value&cn_ivpr_activity_msk)
       {
           value = read32(cn_core_iack_addr);
           write32(cn_int_eoi_addr, 0);
           result = true;
       }
   }
   return result;
}

// =============================================================================
// �������ܣ�Int_SettoAsynSignal
//           ��ָ�����ж�����Ϊ�첽�ź�
// ���������ufl_line,ָ���жϺ�
// �����������
// ����ֵ     ��true���óɹ��� false ����ʧ��
// ˵��          ��
// =============================================================================
bool_t Int_SettoAsynSignal(ufast_t ufl_line)
{
    bool_t result = false;
    u32 addr;
    u32 value;
    atom_high_t  high_atom;

    if(__Int_ChkIntValid(ufl_line))
    {
        high_atom = Int_HighAtomStart();
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type = CN_ASYN_SIGNAL;
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = false;
        tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].prio = cn_prior_asyn_signal;
        tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
                        &= ~(1<<(ufl_line % CN_CPU_BITS));   //����λͼ
        //set the ivpr,the prior trigger type and int vector could not be set
        //set the idr
        addr = sgPicIdrAddrTab[ufl_line];
        value = read32(addr);
        value &= (~CN_PIC_IDR_C0);
        value |= cn_pic_idr_p0;
        write32(addr,value);
        
        Int_HighAtomEnd(high_atom);
        result = true;
    }
    return result;
}
// =============================================================================
// �������ܣ�Int_SettoReal
//           ��ָ�����ж�����Ϊʵʱ�ź�
// ���������ufl_line,ָ���жϺ�
// �����������
// ����ֵ     ��true���óɹ��� false ����ʧ��
// ˵��          �����ô���CINIT����
// =============================================================================
bool_t Int_SettoReal(ufast_t ufl_line)
{
    bool_t result = false;
    u32 addr;
    u32 value;
    atom_high_t  high_atom;

    if(__Int_ChkIntValid(ufl_line))
    {
        high_atom = Int_HighAtomStart();

        if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].sync_event != NULL)
        {
            result = false;//���߳��ڵȴ�����жϣ�������Ϊʵʱ�ж�
        }
        else
        {
            tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type = CN_REAL;    //�ж�������
            tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = true;
            tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].prio = cn_prior_real_int_default;
            tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
                    |= 1<<(ufl_line % CN_CPU_BITS);   //����λͼ
            //set the ivpr
            addr = sgPicIvprAddrTab[ufl_line];       
            value = read32(addr);
            value = value&cn_ivpr_trigertype_msk;  //save the trigger type 
            value |= (cn_prior_asyn_signal<<16);
            write32(addr,value);
            //set the idr
            addr = sgPicIdrAddrTab[ufl_line];
            value = read32(addr);
            value &= (~cn_pic_idr_p0);
            value |= CN_PIC_IDR_C0;
            write32(addr,value);
 
            result = true;
        }
        Int_HighAtomEnd(high_atom);
    }

    return result;
}
// =============================================================================
// �������ܣ�int_enable_nest
//          ����ָ�����ж�ΪǶ��ģʽ
// ���������ufl_line,ָ���жϺ�
// �����������
// ����ֵ     ��true���óɹ��� false ����ʧ��
// ˵��          ��ֻ����ʵʱ�ź�--P1020
// =============================================================================
bool_t Int_EnableNest(ufast_t ufl_line)
{
    bool_t result = false;
    if(__Int_ChkIntValid(ufl_line))
    {
        if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        {
            tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = true;
            result = true;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�Int_DisableNest
//          ����ָ�����ж�Ϊ��Ƕ��ģʽ
// ���������ufl_line,ָ���жϺ�
// �����������
// ����ֵ     ��true���óɹ��� false ����ʧ��
// ˵��          ��ֻ����ʵʱ�ź�--P1020
// =============================================================================
bool_t Int_DisableNest(ufast_t ufl_line)
{
    bool_t result = false;
    if(__Int_ChkIntValid(ufl_line))
    {
        if(tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].int_type == CN_REAL)
        {
            tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].enable_nest = false;
            result = true;
        }

    }

    return result;
}


// =============================================================================
// �������ܣ�int_set_prio
//          ����ָ�����жϵ����ȼ�
// ���������ufl_line,ָ���жϺ�
//          prior,ָ�����ȼ�
// �����������
// ����ֵ     ��true���óɹ��� false ����ʧ��
// ˵��          ��ֻ����ʵʱ�ź�--P1020
// =============================================================================
bool_t Int_SetPrio(ufast_t ufl_line,u32 prior)
{
    bool_t result = false;
    u32 addr;
    u32 value;
    u32 myprior;
    atom_high_t  high_atom;

    if(__Int_ChkIntValid(ufl_line))
    {
         high_atom = Int_HighAtomStart();

         tg_pIntSrcTable[tg_int_lookup_table[ufl_line]].prio = prior;
         //����ԭ����״̬���޸ĺ����ά��ԭ����״̬
         addr = sgPicIvprAddrTab[ufl_line];
         value = read32(addr);
         myprior = (value&cn_ivpr_prior_msk)>>16;
         if(myprior != cn_prior_core_event)//ԭ����״̬
         {
             value = value |((prior<<16)& cn_ivpr_prior_msk);
             write32(addr, value);
         }
         result = true;
     
         Int_HighAtomEnd(high_atom);
    }

    return result;
}

static atom_high_t  sgTrunkKey;
// =============================================================================
// �������ܣ�int_contact_trunk
//          ʹ�����ж�
// �����������
// �����������
// ����ֵ     ��
// ˵��          ��ʹ��MSR��EEbit,��Ϊʵʱ�źź��첽�źŶ�ʹ�õ���EE
// =============================================================================
void Int_ContactTrunk(void)
{
	Int_HighAtomEnd(sgTrunkKey);
}

// =============================================================================
// �������ܣ�int_cut_trunk
//          �ر����ж�
// �����������
// �����������
// ����ֵ     ��
// ˵��          ��ʹ��MSR��EEbit,��Ϊʵʱ�źź��첽�źŶ�ʹ�õ���EE
// =============================================================================
void Int_CutTrunk(void)
{
	sgTrunkKey = Int_HighAtomStart();	
}



