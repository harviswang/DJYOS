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
#include "os.h"
#include "cpu_peri.h"
#include "uart.h"

ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}

const u32 gc_u32CfgTimeZone = 8;     //����ʱ��Ϊ��8��(����)

const char *gc_pCfgDefaultCharsetName = "gb2312";  //Ĭ���ַ���
const char *gc_pCfgDefaultFontName = "gb2312_8_16_1616";  //Ĭ������

const u32 gc_u32CfgGuiCmdDeep = 1024;   //gkernel����������(�ֽ���,����������)
const u32 gc_u32CfgGuiStsDeep = 256;    //gkernel״̬���������(�ֽ���,����������)

const u32 gc_u32CfgDeviceLimit = 5;     //�����豸������
const u32 gc_u32CfgLockLimit = 30;      //�������������û�����semp_create��mutex_create�����������������ں��õ�������
const u32 gc_u32CfgEventLimit = 15;     //�¼�����
const u32 gc_u32CfgEvttLimit = 15;      //���¼�������
const u32 gc_u32CfgWdtLimit = 15;       //������������
const u32 gc_u32CfgPipeLimit = 1;       //�ܵ�����
const u32 gc_u32CfgMemPoolLimit = 10;    //������10���ڴ��
const u32 gc_u32CfgStdinDeviceLimit = 2;
const u32 gc_u32CfgMainStackLen = 1024;

const u32 gc_u32CfgOpenedfileLimit = 16;
const u32 gc_u32CfgTimerLimit = 5;

const char *gc_pCfgStddevName = "UART0";  //��׼��������豸
const char *gc_pCfgWorkPath = "sys:\\";   //����·������


//----����ϵͳ���в�������-----------------------------------------------------
//����: ���ò���ϵͳ���в���,������������/�¼�������.ʹ���ı��ļ�����ʱʹ�á�
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Sys_ConfigRunParam(char *CfgfileName)
{
}
void Stddev_KnlInOutInit(u32 para)  //��������δʵ��
{

}
//----�����ʼ��2-----------------------------------------------------------
//���ܣ���ѡ�����ʼ����������3��:
//      1��sys_module_init����sys_init.c�У��μ��ú���ע��
//      2��prj_module_init���������ʼ����ģ�飬��Ҫ���ø����幤����صĴ��룬��
//          �����ģ�飬���˵���module_init_keyboard�����⣬��Ҫ���ø�Ӳ����ص�
//          module_init_keyboard_hard������
//      ǰ������ʼ��ʱ�����������䷽ʽ��û�г�ʼ��������������õ���̬���䣬ʹ��
//      ����׼��̬���䣬����׼��̬�����˵�����μ�mems.c�ļ���
//      3��run_module_init���μ��ú���ע�͡�
//��������
//���أ���
//-----------------------------------------------------------------------------
void Sys_ModuleInit(void)
{
	tagDevHandle char_term_hdl;
	u32 stdinout;
	uint16_t evtt_main;

	//��ʼ��PutChark,Putsk,GetChark,GetsK�ĸ��ں˼������������,�˺�,printk
	//��������ʹ��.printf��scanf������ǰ��4������ִ���������
	Stddev_KnlInOutInit( 0 );
	//shellģ��,����:��
	Sh_MoudleInit(0);
	//�ļ�ϵͳģ��,����:shell
	//    Djyfs_ModuleInit(0);
	//�豸����ģ��
	Driver_ModuleInit(0);
	//��·����ģ��,�ṩ����Linux��epoll��select�Ĺ���
	Multiplex_ModuleInit(0);

	//�ṩ��shell������ں���Ϣ�Ĺ���,����:shellģ��
	Debug_InfoInit(0);
	//�쳣����ģ��,����:shellģ��
	Exp_ModuleInit(0);
	//��װ�쳣��Ϣ������,�����쳣ģ���¼�Ķ���������
	Exp_InfoDecoderInit( );

	UART_ModuleInit(CN_UART0);
	UART_ModuleInit(CN_UART1);

	//��׼IO��ʼ��,��Ӱ��printk����,�˺�,printf��scanf��ʹ��stdin/out�������
	//��,����õ�,��һֱ��PutChark,Putsk,GetChark,GetsKִ��IO
	//����: ��stdin/out/err���ļ�,�������ļ�ϵͳ
	//      �����豸,�������豸����
	//      ͬʱ,���������������Ϣ����ʩ,���紮��,LCD��
	Stddev_ModuleInit( 0 );

	//��IO�豸,��ͬ�İ��,�ⲿ�ֿ�������Ƚϴ�.
	stdinout = Driver_FindDevice(gc_pCfgStddevName);
	char_term_hdl = Driver_OpenDeviceAlias(stdinout,O_RDWR,0);
	if(char_term_hdl != NULL)
	{
		Driver_CtrlDevice(char_term_hdl,CN_UART_START,0,0);
		//���ô��ڲ�����Ϊ115200��
		Driver_CtrlDevice(char_term_hdl,CN_UART_SET_BAUD,115200,0);
		CharTerm_ModuleInit((ptu32_t)stdinout);
		Driver_CloseDevice(char_term_hdl);
	}

	//djybusģ��
	DjyBus_ModuleInit(0);
	//IIC����ģ��,����:djybus
	ModuleInit_IICBus(0);
	//SPI����ģ��,����:djybus
	ModuleInit_SPIBus(0);
	SPI_Init();
	IIC0_Init();
//	IIC1_Init();

	//����ʱ��ģ��
//	TM_ModuleInit(0);
	//ʹ��Ӳ��RTC,ע�͵���ʹ��tick��RTC��ʱ,����:����ʱ��ģ��
//	RTC_ModuleInit(0);

	//��ʱ�����
//	module_init_timersoft(0);

	//��������ģ��
	//    Keyboard_ModuleInit(0);
	//������������,����:��������ģ��
	//    module_init_keyboard_hard(0);

	//�ַ���ģ��
//	Charset_ModuleInit(0);
	//gb2312�ַ�����,����:�ַ���ģ��
//	Charset_Gb2312ModuleInit(0);
	//ascii�ַ���,ע��,gb2312������ascii,��ʼ����gb2312��,���뱾ģ��
	//����:�ַ���ģ��
	//    Charset_AsciiModuleInit(0);
	//��ʼ��utf8�ַ���
	//    Charset_Utf8ModuleInit(0);
	//���ʻ��ַ���֧��,���������ַ���ģ���Լ������ַ�����ʼ��
//	Charset_NlsModuleInit("C");

//	Font_ModuleInit(0);                 //����ģ��
	//8*8�����ascii��������:����ģ��
	//    Font_Ascii8x8FontModuleInit(0);
	//6*12�����ascii��������:����ģ��
	//    Font_Ascii6x12FontModuleInit(0);
	//�����鰲װGB2312��������,������8*16��ascii����.����:����ģ��
	//    Font_Gb2312_816_1616_ArrayModuleInit(0);
	//���ļ���װGB2312��������,������8*16��ascii����.����:����ģ��,�ļ�ϵͳ
	//    Font_Gb2312_816_1616_FileModuleInit("sys:\\gb2312_1616");
	//8*16 ascii�����ʼ��,������128�ֽ�,����:����ģ��
	//ע:�����װ��GB2312,�����ٰ�װ
//	Font_Ascii8x16FontModuleInit(0);


	//���Ź�ģ��,��������˼���ʱιӲ����,���Ź����ģ��Ӵ˿�ʼ�ӹ�Ӳ����.
//	module_init_wdt(0);

	//�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
	evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,__djy_main,
						   NULL,gc_u32CfgMainStackLen,"main function");
	//�¼���������������Ϊ0�������shell�������������ɼ�shell�����в���
	Djy_EventPop(evtt_main,NULL,0,NULL,0,200);

	Heap_DynamicModuleInit(0);//�Դ�malloc����ִ�п������㷨
	//����,��ʼ��ʱʹ�õ�ջ,�Ѿ���ϵͳ����,�������ʹ˽���,��������ú��ѿ�
	return ;
}


