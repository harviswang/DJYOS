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
// ģ������: �쳣��API���ݽṹ����ӿ�����(�쳣�������ӿ�)
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 3:17:17 PM/May 8, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:

#ifndef EXP_API_H_
#define EXP_API_H_

#include "stdint.h"
#define CN_EXP_NAMELEN_LIMIT            0x10
#define CN_EXP_DECODERNUM_LIMIT         0x10

//�쳣��������,���������λ��
enum _EN_EXP_DEAL_TYPE_
{
    EN_EXP_DEAL_IGNORE = 0,      //���Ը��쳣��Ϣ
    EN_EXP_DEAL_DEFAULT,         //��Ĭ�Ϸ�ʽ�����׳��쳣��ʱ����ʹ�ø�ѡ��
    EN_EXP_DEAL_RECORD,          //��Ҫ��¼���쳣
    EN_EXP_DEAL_RESET,           //Ӳ���������൱���ϵ�����
    EN_EXP_DEAL_REBOOT,          //��ת������ģʽѡ��
    EN_EXP_DEAL_RESTART,         //��ת���������з�ʽ���ʼ����
};

//�쳣��ʾ��Ϣ�������׳��쳣��ʱ��ʹ�ã�
struct tagExpThrowPara
{
    bool_t  validflag;                 //true��ʾ���쳣������Ч��������Ч
    char    *name;                     //�쳣��Ϣ����������
    u8      *para;                     //�쳣��Ϣ��ַ
    u32     para_len;                  //��Ϣ����
    u32     dealresult;                //�쳣��������,�ο�_SYSEXP_DEAL_TYPE
};

////////////////////////�쳣�׳���Ϣ������ģ�ͼ���ע��ע��////////////////////////
//�����쳣��Ϣ�Ľ�����ģ�ͣ�����������׳�ʱһģһ�����������Լ��Ѽ�����Ϣһ��
//�ܹ������Լ�һ���ܹ��жϳ��ǲ����Լ��Ѽ�����Ϣ
typedef bool_t (*fnExp_ThrowinfoDecoderModule)(struct tagExpThrowPara *para, u32 endian);
// =============================================================================
// �������ܣ�Exp_RegisterThrowinfoDecoder
//          ע������쳣��Ϣ������
// ���������
//          decoder,�쳣������
//          name,�쳣����������,���ٱ�֤��ȫ�ֵ��Ҳ�����
// ���������
// ����ֵ  ��true,�ɹ�ע��;false,ע��ʧ�ܣ�
// ˵��    ��
//          1,����Ӧ���쳣���Ѿ���ע���˵�ʱ�򣬻����δ��ע����쳣�Ž���ע��
//          2,���������Ѿ����ڶ�Ӧ�������쳣�ᵼ��ע��ʧ���(ֻ�ǡ�\0��Ҳ����Ч��)
//          3,��ע��ʧ�ܵĿ��ܣ����ע���鷵�ؽ��
// =============================================================================
bool_t Exp_RegisterThrowinfoDecoder(fnExp_ThrowinfoDecoderModule decoder,const char *name);
// =============================================================================
// ��������:Exp_UnRegisterThrowinfoDecoder
//          ע������쳣��Ϣ������
// �������:name,�Ѿ���ע����쳣����
// ���������
// ����ֵ  :true,�ɹ�ע��;false,ע��ʧ�ܣ�
// ˵��    ��
// =============================================================================
bool_t Exp_UnRegisterThrowinfoDecoder(char *name);


//HOOK��Ϣ�Ѽ����Լ��ṩ�洢HOOK�쳣��Ϣ�Ŀռ�
//////////////////////////////HOOKԭ��//////////////////////////////////////////
// =============================================================================
// �������ܣ�HOOKDealerԭ��
// ���������throwpara, �쳣�׳����׳����쳣������hook�ܽ�ͽ⣬���ܽ������
// ���������infoaddr,�洢�쳣��Ϣ�ĵ�ַ
//           infolen,�洢�Ѽ���Ϣ����
// ����ֵ  �� _SYSEXP_RESULT_TYPE���ô������Ḳ�ǵ�BSP�Ĵ����쳣�����
// ˵��    �� �쳣ʱ���õ�APP�ṩ���쳣����������������쳣ʱϵͳӦ�ó��������״̬
//           Ӧ�ó����ṩ�Ĺ��Ӻ�����Ӧ�ó�����ڴ���һЩ�ƺ��������ɷ��ظ��ӵ��쳣
//           ��Ϣ����Щ��Ϣ����BSP�ռ���Ӳ����ϵͳ�쳣��Ϣһ�𱣴�
//           �ù��ӵķ��ؽ�������BSP�Ĵ�����һ����Ϊ�쳣�ṹ���ж�����(һ�����
//           ����ľͰԭ�������س̶ȴ��������)
// =============================================================================
typedef  enum _EN_EXP_DEAL_TYPE_ (*fnExp_HookDealermodule)( \
                           struct tagExpThrowPara *throwpara,\
                           ptu32_t *infoaddr,u32 *infolen);

// =============================================================================
// �������ܣ�HOOKDecoderԭ��
// ���������throwpara, �쳣�׳����׳����쳣������hook�ܽ�ͽ⣬���ܽ������
//          infoaddr,HOOK��Ϣ�洢��ַ
//          infolen, HOOK��Ϣ��Ч����
//          endian, hook��Ϣ�Ĵ洢��С��
// �����������
// ����ֵ  ��true�ɹ�  false ʧ�ܣ�û��ע������أ�
// =============================================================================
typedef  bool_t (*fnExp_HookInfoDecodermodule)(struct tagExpThrowPara *throwpara,\
                                        ptu32_t infoaddr, u32 infolen,u32 endian);

//�û�ע�����ע��HOOK�ķ���
bool_t Exp_RegisterHook(fnExp_HookDealermodule  fnappdealer,\
                            fnExp_HookInfoDecodermodule fnappdecoder);
bool_t Exp_UnRegisterHook(void);

//�׳��쳣������
enum _EN_EXP_DEAL_REASULT_
{
    EN_EXP_DEALT_PARAERR = 0,          //�׳���������
    EN_EXP_DEALT_IGNORE,                    //�������վ���--����
    EN_EXP_DEALT_RECORDFAILED,              //�洢����
    EN_EXP_DEALT_SUCCESS,                   //����ɹ���������¼
};
////////////////////////�洢����ģ�ͼ���ע��ע��//////////////////////
 struct tagExpRecordPara
{
    u32     headinfolen;     //ͷ��Ϣ����
    ptu32_t headinfoaddr;    //ͷ��Ϣ��ַ
    u32     osstateinfolen;  //OS״̬��Ϣ����
    ptu32_t osstateinfoaddr; //OS״̬��Ϣ��ַ
    u32     hookinfolen;     //HOOK��Ϣ����
    ptu32_t hookinfoaddr;    //HOOK��Ϣ��ַ
    u32     throwinfolen;    //throw��Ϣ����
    ptu32_t throwinfoaddr;   //throw��Ϣ��ַ
};
//�洢���ؽ��
enum _EN_EXP_RECORDRESULT_
{
    EN_EXP_DEAL_RECORD_SUCCESS,       //��¼�ɹ�
    EN_EXP_DEAL_RECORD_NOSPACE,       //�洢�ռ䲻��
    EN_EXP_DEAL_RECORD_HARDERR,       //�洢Ӳ������
    EN_EXP_DEAL_RECORD_PARAERR,       //�洢��������
    EN_EXP_DEAL_RECORD_NOMETHOD,      //�洢��������,�޴洢����
};
// =============================================================================
// �������ܣ���¼һ֡�쳣��Ϣ
//           ��¼�쳣��Ϣ���洢����������������߼�ˣ����쳣�洢��������߾���
// ���������pSysExpFram,һ֡�쳣��Ϣ
//          dwFramvalidLen,֡��Ϣ����Ч����
//          ֮����ָ���������ȣ���Ҫ��Ϊ�˴洢�ķ��㣨�п���ֻ��¼��Ч���ȼ��ɣ�
// ���������
// ����ֵ  ����en_sysExp_Record_result
// =============================================================================
typedef  enum _EN_EXP_RECORDRESULT_ (*fnExp_RecordModule)(\
                                       struct tagExpRecordPara  *recordpara);
// =============================================================================
// �������ܣ�fnExp_RecordCleanModule
//          ������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��TRUE �ɹ��� FALSEʧ��
// =============================================================================
typedef  bool_t (*fnExp_RecordCleanModule)(void);

// =============================================================================
// �������ܣ�fnExp_RecordCheckNumModule
//          �鿴һ���洢�˶������쳣��Ϣ
// ���������
// ���������recordnum,�洢���쳣��Ϣ��Ŀ��
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fnExp_RecordCheckNumModule)(u32 *recordnum);

// =============================================================================
// �������ܣ�fnExp_RecordCheckLenModule
//          ����ָ���쳣��Ŀ�ĳ���
// ���������assignedno,ָ������Ŀ
// ���������recordlen,����Ŀ�ĳ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fnExp_RecordCheckLenModule)(u32 assignedno, u32 *recordlen);

// =============================================================================
// �������ܣ�fnExp_RecordGetModule
//          �Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ
//           buflen,����������
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fnExp_RecordGetModule)(u32 assignedno,u32 buflen,u8 *buf,\
                                         struct tagExpRecordPara *recordpara);

// =============================================================================
// �������ܣ�fnExp_RecordScanModule
//           ������ʱ��ɨ���쳣�洢��¼����ȡ�ؼ���Ϣ�����Ժ�洢
// �����������
// �����������
// ����ֵ  ����
// =============================================================================
typedef  void (*fnExp_RecordScanModule)(void);

 struct tagExpRecordOperate
{
    fnExp_RecordScanModule        fnexprecordscan;    //�����洢��ɨ�裬
    fnExp_RecordModule            fnexprecord;        //��¼һ���쳣��Ϣ
    fnExp_RecordCleanModule       fnexprecordclear;   //��������쳣��Ϣ
    fnExp_RecordCheckNumModule    fnexprecordchecknum;//��ȡ�洢�쳣��Ϣ����Ŀ��
    fnExp_RecordCheckLenModule    fnexprecordchecklen;//��ȡָ����Ŀ�ĳ���
    fnExp_RecordGetModule         fnexprecordget;     //��ȡָ����Ŀ���쳣��Ϣ
};
// =============================================================================
// �������ܣ�Exp_RegisterRecordOpt
//          ע���쳣��Ϣ������
// ���������opt,��Ҫע����쳣��Ϣ������
// ���������
// ����ֵ  ��FALSE,ʧ��  TRUE�ɹ���ʧ�ܵĻ���ʹ��BSPĬ�ϵĴ�����
// ע��    �������ϲ����ṹ����ָ���Ĵ�������Ӧ���ṩ������Ļ���ע�᲻�ɹ�
// =============================================================================
bool_t  Exp_RegisterRecordOpt(struct tagExpRecordOperate *opt);

// =============================================================================
// �������ܣ�Exp_UnRegisterRecordOpt
//           ע���쳣��Ϣ������
// �����������
// �����������
// ����ֵ  ��TRUE�ɹ�  FALSEʧ��,��ʹ��Ĭ�ϵĴ洢����
// =============================================================================
bool_t Exp_UnRegisterRecordOpt(void);

////////////////////////////�쳣�׳�ģ��////////////////////////////////////////
// =============================================================================
// ��������:Exp_Throw
//          ���������쳣�����
// �������:throwpara,�׳����쳣��Ϣ����
// �������:dealresult,���쳣�����մ�����
// ����ֵ  :true���ɹ��� false��ʧ��(�������ߴ洢��δ֪ԭ��)
// ˵��    :������������bsp�е��ã�Ҳ������ϵͳ�е���
//          �������Ϊ���쳣ģ�鴦���ͳһ���汾֮���������Ϊ��ͣ�ͬʱ������ֲ
//          �����쳣������֮���໥���������໥����
// =============================================================================
bool_t  Exp_Throw(struct tagExpThrowPara *throwpara, u32 *dealresult);
// =============================================================================
// ��������:ModuleInstall_Exp
//         ϵͳ�쳣�����ʼ��
// �������:para
// �������:��
// ����ֵ  :ptu32_t ��ʱ�޶���
// ˵��    :��Ҫ�ǳ�ʼ���洢����
// =============================================================================
ptu32_t ModuleInstall_Exp(ptu32_t para);

#endif /* EXP_API_H_ */
