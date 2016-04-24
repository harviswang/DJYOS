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
// ģ������: DJY�˵�ģ��֮�û��˵���Ը���ݽṹ������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
//          ���ļ���Ҫ������������û������Ĳ˵�����Ը�����ݽṹ
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================


#ifndef _MENU_DATA_H
#define _MENU_DATA_H


#include "MenuDesigner.h"

extern const  struct _tagMenuitemModel g_tMenuitemModelTab[];
extern const  u32 CN_MENU_MAX_ITEM ;


//u32  Menu_func(struct _tagMenuModel *fnpara);//only supplyed by the user and 
//used to fill the struct;

//============================================================================
//����CPU����
//============================================================================
enum _ANA_CPU_  {                   // �ֽڸ���λ�ֱ��ʾ
    EN_CPU0 = 32,					// bit5
    EN_CPU1 = 64,					// bit6
    EN_CPU2 = 128,					// bit7
};

//============================================================================
//���ּ�¼������
//============================================================================
/*
enum _REC_TYPE_
{
    EN_REC_WAVE=0,                  // ¼��
    EN_REC_ACT,                     // �¼�
    EN_REC_ALM,                     // �澯
    EN_REC_OVER,                    // Խ��
    EN_REC_START,                   // ��
    EN_REC_INPUT,                   // ����
    EN_REC_CHK,                     // �Լ�
    EN_REC_RUN,                     // ����
    EN_REC_ONOFF,                   // Ͷ��
    EN_REC_LOCK,                    // ����
    EN_REC_COMBRK,                  // ͨѶ�ж�
    EN_REC_YK,                      // ң��
    EN_REC_SC,                      // ˳��
    EN_REC_SOE,
    EN_REC_COS,
    EN_REC_OPT,						// ����
    EN_REC_TQ,
    EN_REC_PRN,                     // ��ӡ
    EN_REC_MENWAVE,                 // �ֶ�¼��
    EN_NUM_REC_TYP,                 // �ܵļ�¼��������ע�⣺��һ��̶��������

    EN_ACT_WAVE,
    EN_STA_WAVE,
    EN_HAND_WAVE,
};
*/
//============================================================================
//ģ�������������Ͷ��壨Ӧ���ڽ���������������
//============================================================================
enum    _ANA_TYP_  {                // ģ�������Ͷ���
    EN_ANA_TYP_NULL = 0,            // δ����
    EN_ANA_TYP_I,                   // ����
    EN_ANA_TYP_U,                   // ��ѹ
    EN_ANA_TYP_U_S,                 // վ�ñ��ѹ

    EN_ANA_TYP_P,                   // �й�����
    EN_ANA_TYP_Q,                   // �޹�����
    EN_ANA_TYP_S,                   // ���ڹ���
    EN_ANA_TYP_COS,                 // ��������
    EN_ANA_TYP_DC_U,                // ֱ����ѹ
    EN_ANA_TYP_DC_I,                // ֱ������
    EN_ANA_TYP_GEAR,                // ��λ
    EN_ANA_TYP_FR,                  // Ƶ��
    EN_ANA_TYP_T,                   // �¶�
    EN_ANA_TYP_DU,					// ֱ��
    EN_ANA_TYP_MC,					// ����
    EN_ANA_TYP_AMP,
    EN_ANA_TYP_ST,                  // �豸̬
    EN_ANA_TYP_IMP,                 // �迹
};

//===================================================================================================
//
// ���³������û�����
//
//===================================================================================================

enum _INDEX_VALUE_TYPE_{

    EN_SOFT_SET,
    EN_FIGURE_SET,
};

//============================================================================
//�������˵���ӦwParameterִ�в˵�������Ӧ���ڲ˵�������
//============================================================================
enum    _OUTPUT_PARA_  {
    EN_OUTPUT_0 = 1,
    EN_OUTPUT_1,
    EN_OUTPUT_2,
    EN_OUTPUT_3,
};

enum _REV_TYPE_                             // ��������
{
    EN_REV_FREE,                            // ��ǿ�Ƹ���
    EN_REV_FORCE,                           // ǿ�Ƹ���
};

enum _MENUCREAT_TYPE_ {                     // �����˵���������
    EN_SEESET_SOFT_MENU = 0x7c,             // �鿴��ֵ�˵�����
    EN_SETVAL_SOFT_MENU = 0xc7,             // ������ֵ�˵�����
    EN_SEESET_FIGURE_MENU = 0x77,           // �鿴��ֵ�˵�����
    EN_SETVAL_FIGURE_MENU = 0xcc,           // ������ֵ�˵�����
    EN_SEESET_PARA_MENU = 0x66,           	// �鿴��ֵ�˵�����
    EN_SETVAL_PARA_MENU = 0xBB,           	// ������ֵ�˵�����
};

//============================================================================
//ʵʱ�������ͱ�־
//============================================================================
enum _MEA_TYPE_{
    EN_MEA_RLY,                             // ����������1
    EN_MEA_RLY2,                            // ����������2
    EN_MEA_RLY3,                            // ����������3
    EN_MEA_ANA,                             // ����������1
    EN_MEA_ANA2,                            // ����������2
    EN_MEA_ANA3,                            // ����������3
    EN_MEA_DC,                              // ����ֱ����
    EN_MEA_SYN,                             // ����г����
    EN_MEA_POWER,                           // ����������
    EN_MEA_GEAR,                            // ������λ��
    EN_MEA_TQ,                              // ����ͬ����
    EN_MEA_INPUT1,                     		// ����������1
    EN_MEA_INPUT2,                          // ����������2
    EN_MEA_INPUT3,                          // ����������3
	EN_INPUT_RLY_ALL,
	EN_INPUT_RLY_FAULT,
	EN_INPUT_RLY_OTHER,
	EN_INPUT_BS_ALL,
	EN_INPUT_BS_FAULT,
	EN_INPUT_BS_OTHER,
	EN_MEA_AC,								// ң�⽻����
	EN_MEA_ADJ,								// ң��΢��ϵ��
	EN_MEA_YX,								// ң��״̬
	EN_OUTPUT_TRIP, 						// �������ڵ���
	EN_OUTPUT_SIGN, 						// �źų��ڵ���
	EN_MEA_LS,                  			//�����ź�.zhanggl
	EN_MEA_SCRLY,               			//˳�������ź�.zhanggl
};

enum _NO_USER_PASSWORD_{
    EN_NO_USER_PWD = 0x55,
};

enum _FACTORY_PASSWORD_{
    EN_FACTORY_PASSWORD = 0x55,
};

//==============================================================================
// �˵���������س���

//============================================================================
//�������˵���ӦwParameterִ�в˵�������Ӧ���ڲ˵�������
//============================================================================
enum    _ANA_PARA_  {
    EN_ANA_0 = 1,
    EN_ANA_1    ,
};

//============================================================================
//�������˵���ӦwParameterִ�в˵�������Ӧ���ڲ˵�������
//============================================================================
enum    _INPUT_PARA_  {
    EN_INPUT_0 = 1,
    EN_INPUT_1,
};

enum _ANA_TYPE_
{
	EN_TYPE_DIF_CURRENT=0 ,
	EN_TYPE_UNIT_CURRENT ,
	EN_TYPE_UNIT_VOLTAGE ,

};

//==========================================    //
// �ַ������ų�������
enum _STR_FLAG_ {
    EN_STR_FLAG_FUNSET = 0,                     // Ͷ��,�˳�
    EN_STR_FLAG_OKCANCEL,
    EN_VALUE_FLAG_SLOTTYPE,                     // �������
    EN_VALUE_FLAG_YESNO,                        // �Ƿ�
    EN_VALUE_FLAG_CHN_NUM,                      // ��������
    EN_VALUE_FLAG_YXSET,                        // ң������
    EN_VALUE_FLAG_YKSTEP,                       // ң�ز���
    EN_VALUE_FLAG_YKACTION,                     // ң�ز�������
    EN_VALUE_FLAG_CHK,                          // �Լ���Ϣ
    EN_VALUE_FLAG_BEUSED,                       // �Ƿ�ʹ��
    EN_VALUE_FLAG_PORTTYPE,                     // �˿�����
    EN_VALUE_FLAG_MAPNAME,                      // ת��������
    EN_VALUE_FLAG_RECTITLE,                     // ��¼���ͱ�
    EN_VALUE_FLAG_BHINFO,                       // ��������/����
    EN_VALUE_FLAG_BHSET,                        // ��������/����
    EN_VALUE_FLAG_OKCANCEL,                     // ȷ��/ȡ��
    EN_VALUE_FLAG_FILETYPE,                     // �ļ�����
    EN_VALUE_FLAG_ROLE,                         // ���ݽ�ɫ
    EN_VALUE_FLAG_COMMSTATE,                    // ͨѶ״̬
    EN_VALUE_FLAG_ALARM,                        // �澯״̬
    EN_VALUE_FLAG_YXTYPE,                       // ң������
    EN_STR_FLAG_MENUALAUTO,
    EN_STR_FLAG_LINKBREAK,
    EN_STR_FLAG_DONEUNDONE,
    EN_STR_FLAG_ACTBACK,
    EN_STR_FLAG_RESUMEBREAK,
    EN_STR_FLAG_USESTOP,
    EN_STR_FLAG_BAUDRATE,
    EN_STR_FLAG_VERIFY,
    EN_STR_FLAG_PROTOL,
    EN_STR_FLAG_RECORD,
    EN_STR_FLAG_MESSAGE,
    EN_STR_FLAG_COM,
    EN_STR_FLAG_MODE,
    EN_STR_FLAG_COMBAUD,
    EN_STR_FLAG_DATABIT,
    EN_STR_FLAG_STOPBIT,
    EN_STR_FLAG_COMTYPE,
    EN_STR_FLAG_SAMEV,
    EN_STR_FLAG_PT,
    EN_STR_FLAG_CT,
    EN_STR_FLAG_TRANSTYPE,
    EN_STR_FLAG_LINEPHS,
    EN_STR_FLAG_RETOUT,
    EN_STR_FLAG_YXDI,
    EN_STR_FLAG_YKPUL,
    EN_STR_FLAG_WAVE,
    EN_STR_FLAG_MEMTYPE,
    EN_STR_FLAG_CHANNEL,
    EN_STR_FLAG_POWERTYPE,
    EN_STR_FLAG_YCCOMMTYPE,
    EN_STR_FLAG_YXCOMMTYPE,
//    EN_STR_FLAG_HARMOTYPE,
//    EN_STR_FLAG_DWINTYPE,
    EN_STR_FLAG_CTRLWORD,
    EN_STR_FLAG_BUSNAME,
    EN_STR_FLAG_UNITSHOWMODE,
    EN_STR_FLAG_BAYMODE,
    EN_STR_FLAG_BASEANA,
    EN_STR_FLAG_BASEPHASE,
    EN_STR_FLAG_BAYCODE,
    EN_STR_FLAG_SET_NUM,
    EN_STR_FLAG_DCTYPE,
    EN_STR_FLAG_DCINTYPE,
    EN_STR_FLAG_DWINTYPE,
    EN_STR_FLAG_PTCT,
    EN_STR_FLAG_HARMOTYPE,
    EN_STR_FLAG_SHOWANATYPE,
    EN_STR_FLAG_ANAPOLARTYPE,						// ����ͨ������
}enumStrType;

/*================================================================================*/
// Const_Soft.h �����̶�����
/*================================================================================*/
// ��ѹ������
enum _SOFT_TYPE_NUMBER
{
    EN_SOFT_PRO = 0,                            // ����������ѹ��
    EN_SOFT_GOOSE,                              // GOOSE��ѹ��
    EN_SOFT_MU,                                 // MU��ѹ��
    EN_SOFT_SWITCH,                             // ��բǿ����ѹ��
    EN_SOFT_BAK,                                // ������ѹ��
//----------------------------------------------//
    EN_SOFT_TYPE_END,                           // ��ѹ����������Ŀ,���ɸĶ�
};
#define CN_NUM_SOFT_TYPE        (EN_SOFT_TYPE_END)  // ��ѹ�������
// װ����ѹ��
#define CN_SUM_SOFT             (EN_SOFT_END)       // ��ѹ������


enum _SET_SIDE_TYPE_                            // ��ֵ���
{
    EN_SIDE_START = 0,                          // ���ò�
    EN_SIDE_BASIC,                              // ������Ϣ
    EN_SIDE_DEVINF,                             // װ�ò���
    EN_SIDE_COP,                                // �ڲ���ֵ
    EN_SIDE_MATRIX,                             // �ڲ���ֵ ���ھ���
    EN_SIDE_ALL,                                // ȫ��
    EN_SIDE_HIGH,                               // ��ѹ��
    EN_SIDE_MED1,                               // ��ѹ��
    EN_SIDE_MED2,                               // ��ѹ��
    EN_SIDE_LOW1,                               // ��ѹ1��
    EN_SIDE_LOW2,                               // ��ѹ2��
    EN_SIDE_LK,                                 // �翹��
    EN_SIDE_Z,                                  // Z��
    EN_SIDE_DEF,                                // �Զ���
    EN_SIDE_NONE = 0xFF,                        // ������
};

enum _REC_TYPE_
{
    EN_ACT_REC = 0,                             // �¼���¼
    EN_ALM_REC,                                 // �澯��¼
    EN_CHK_REC,                                 // �Լ��¼
    EN_SOE_REC,                                 // SOE��¼
    EN_COS_REC, 								// COS��¼
    EN_LOCK_REC,                                // ˲ʱ������¼
    EN_OVER_REC,                                // Խ�޼�¼
    EN_START_REC,                               // ������¼
    EN_RUN_REC,                                 // ���м�¼
    EN_INPUT_REC,                               // �����λ��¼
    EN_ONOFF_REC,                               // ����Ͷ�˼�¼
    EN_OPT_REC,                                 // ����������¼
    EN_YK_REC,                                 	// ����ң�ؼ�¼
    EN_SC_REC,                                 	// װ��˳�ؼ�¼
    EN_SCSTEPINFO_REC,                          // װ��˳�ص�����¼
    EN_FAULT_REC,                           	// �¹ʱ����¼
    EN_ACTWAVE_REC,                             // ����¼����Ϣ��¼
    EN_STARTWAVE_REC,                           // ����¼����Ϣ��¼
    EN_HANDWAVE_REC,                            // �ֶ�¼����Ϣ��¼
	EN_FAULT_NO,								// �������
    EN_ALL_REC = 0xFF,                      	// ���м�¼
    EN_NO_REC  = 0xFFFF,                    	// ��Ч��¼
};

//============================================================================
//��ӡ������
//============================================================================
enum _PRN_TYPE{
    EN_PRINT_EQUIPINF =1,           // ��ӡװ����Ϣ
    EN_PRINT_ACTINF,                // ��ӡ����������¼
    EN_PRINT_ALARMINF,              // ��ӡ�����澯��¼
    EN_PRINT_STARTINF,              // ��ӡ����������¼
    EN_PRINT_LOCKINF,               // ��ӡ����������¼
    EN_PRINT_SETINF,                // ��ӡ��ֵ��Ϣ
    EN_PRINT_ACTWAVEINF,            // ��ӡ����¼����Ϣ
    EN_PRINT_HANDWAVEINF,           // ��ӡ�ֶ�¼����Ϣ
    EN_PRINT_STARTWAVEINF,          // ��ӡ����¼����Ϣ
    EN_PRINT_ACTWAVEREPORT,         // ��ӡ��������
    EN_PRINT_DSPINF,                // ��ӡ��Ļ��ʾ��Ϣ
    EN_PRINT_UNITINF,               // ��ӡĸ������Ϣ
	EN_PRINT_SOEINF,				// ��ӡ������Ϣ
	EN_PRINT_COSINF,				// ��ӡCOS��Ϣ
    EN_PRINT_RUNINF,                // ��ӡ������Ϣ
    EN_PRINT_OPTINF,                // ��ӡ������Ϣ
    EN_PRINT_CHKINF,                // ��ӡ�����Լ��¼
    EN_PRINT_ONOFFINF,              // ��ӡͶ����Ϣ
    EN_PRINT_PARASETINF,            // ��ӡ�ڲ�������ֵ��Ϣ
    EN_PRINT_ANALOG,                // ��ӡ������
    EN_PRINT_ACT_RPT,               // ������׼�¹ʱ���
    EN_PRINT_RUN_RPT,               // ������׼���б���
    EN_PRINT_SPECIAL_SET,           // ������׼�ڲ���ֵ
    EN_PRINT_SETINF_COMM,           // ͨѶ��ֵ
    EN_PRINT_YCINF,      	        // ң����Ϣ
    EN_PRINT_KI,      	            // ������ʵʱֵ
    EN_PRINT_SOFTYB,      	        // ��ѹ��
    EN_PRINT_OVER,      	        // Խ�޼�¼
    EN_PRINT_YK,      	            // ң�ؼ�¼

    EN_PRINT_IDLE = 0XFF,           // ����
};

/*================================================================================*/
#define CN_NO_STATE             (0x5C5C)            // �ⲿͨѶ��״̬��־
#define	CN_COP_PWD				(483)				// ��������:���޸ı���Ԫ�����á��鿴�ڴ��
#define	CN_USER_PWD				(800)				// �û���������:�������޸���ͨ���롢��ֵ��Ԥ���

/*================================================================================*/

#endif
