
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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
#include "config-prj.h"
#include "stdint.h"
#include "driver.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_lcd.h"
#include "uart.h"
#include "gkernel.h"
#include "djyos.h"

//=======ȫ�ֱ�������============================================================
const u32 gc_u32CfgTimeZone = 8;     //����ʱ��Ϊ��8��(����)

const char *gc_pCfgDefaultCharsetName = "gb2312";  //Ĭ���ַ���
const char *gc_pCfgDefaultFontName = "gb2312_8_16_1616";  //Ĭ������

const u32 gc_u32CfgGuiCmdDeep = 1024;   //gkernel����������(�ֽ���,����������)
const u32 gc_u32CfgGuiStsDeep = 256;    //gkernel״̬���������(�ֽ���,����������)

const u32 gc_u32CfgDeviceLimit = 10;     //�����豸������
const u32 gc_u32CfgLockLimit = 50;      //�������������û�����semp_create��mutex_create�����������������ں��õ�������
const u32 gc_u32CfgEventLimit = 50;     //�¼�����
const u32 gc_u32CfgEvttLimit = 50;      //���¼�������
const u32 gc_u32CfgWdtLimit = 5;       //������������
const u32 gc_u32CfgPipeLimit = 5;       //�ܵ�����
const u32 gc_u32CfgMemPoolLimit = 20;    //������10���ڴ��
const u32 gc_u32CfgStdinDeviceLimit = 3;
const u32 gc_u32CfgMainStackLen = 4096;

const u32 gc_u32CfgOpenedfileLimit = 16;
const u32 gc_u32CfgTimerLimit = 5;

const char *gc_pCfgStddevName = "UART2";  //��׼��������豸
const char *gc_pCfgWorkPath = "sys:\\";   //����·������

//Э��ջ����
const u32 gNetPkgMemSize = 8*4096;

//==============================================================================

#define	LCD_DEV_NAME	"LCD_TWR_K70"
#define	TOUCH_DEV_NAME	"TOUCH_TWR_K70"

static	const char *gdd_input_dev[]={

	TOUCH_DEV_NAME,
	NULL, //����Ҫ��NULL��Ϊ�������

};

//==============================================================================
bool_t ModuleInstall_TOUCH_TWR_K70(struct tagGkWinRsc *desktop,const char *touch_dev_name);

//==============================================================================

ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}
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
    static struct tagGkWinRsc desktop;
    struct tagDisplayRsc *lcd_k70;
    //struct DisplayPara K70LcdPara = {LCD_XSIZE,LCD_YSIZE,"LCD_TWRK70","extram"};

    //��ʼ��PutChark,Putsk,GetChark,GetsK�ĸ��ں˼������������,�˺�,printk
    //��������ʹ��.printf��scanf������ǰ��4������ִ���������
    Stddev_KnlInOutInit( 0 );
    //shellģ��,����:��
    ModuleInstall_Sh(0);
    //�ļ�ϵͳģ��,����:shell
    ModuleInstall_Djyfs(0);
    //�豸����ģ��
    ModuleInstall_Driver(0);
    //��·����ģ��,�ṩ����Linux��epoll��select�Ĺ���
    ModuleInstall_Multiplex(0);
  //��Ϣ����ģ��
    ModuleInstall_MsgQ(0);
    //�ṩ��shell������ں���Ϣ�Ĺ���,����:shellģ��
    ModuleInstall_DebugInfo(0);
    //�쳣����ģ��,����:shellģ��
    ModuleInstall_Exp(0);
    //��װ�쳣��Ϣ������,�����쳣ģ���¼�Ķ���������
    Exp_InfoDecoderInit( );

    //flash�ļ�ϵͳ��ʼ��,����:�ļ�ϵͳģ��,shellģ��
    ModuleInstall_DFFSD(0);
    //nand flash����,����:flash�ļ�ϵͳģ��
    ModuleInstall_FsNandFlash("extram");
    //���ù���·��,����:�ļ�ϵͳ,�����·������.
    Djyfs_SetWorkPath(gc_pCfgWorkPath);

//    ModuleInstall_UART(CN_UART0);
//    ModuleInstall_UART(CN_UART1);
    ModuleInstall_UART(CN_UART2);
//    ModuleInstall_UART(CN_UART3);
//    ModuleInstall_UART(CN_UART4);
//    ModuleInstall_UART(CN_UART5);

    //��׼IO��ʼ��,��Ӱ��printk����,�˺�,printf��scanf��ʹ��stdin/out�������
    //��,����õ�,��һֱ��PutChark,Putsk,GetChark,GetsKִ��IO
    //����: ��stdin/out/err���ļ�,�������ļ�ϵͳ
    //      �����豸,�������豸����
    //      ͬʱ,���������������Ϣ����ʩ,���紮��,LCD��
    ModuleInstall_Stddev( 0 );

    //��IO�豸,��ͬ�İ��,�ⲿ�ֿ�������Ƚϴ�.
    stdinout = Driver_FindDevice(gc_pCfgStddevName);
    char_term_hdl = Driver_OpenDeviceAlias(stdinout,O_RDWR,0);
    if(char_term_hdl != NULL)
    {
        Driver_CtrlDevice(char_term_hdl,CN_UART_START,0,0);
        Driver_CtrlDevice(char_term_hdl,CN_UART_DMA_USED,0,0);
        //���ô��ڲ�����Ϊ115200��
        Driver_CtrlDevice(char_term_hdl,CN_UART_SET_BAUD,115200,0);
        ModuleInstall_CharTerm((ptu32_t)stdinout);
        Driver_CloseDevice(char_term_hdl);
    }

    //djybusģ��
    ModuleInstall_DjyBus(0);
    //IIC����ģ��,����:djybus
    ModuleInstall_IICBus(0);
    //SPI����ģ��,����:djybus
    ModuleInstall_SPIBus(0);
    IIC0_Init();

    //����ʱ��ģ��
    ModuleInstall_TM(0);
    //ʹ��Ӳ��RTC,ע�͵���ʹ��tick��RTC��ʱ,����:����ʱ��ģ��
    ModuleInstall_RTC(0);

    //��ʱ�����
    //ModuleInstall_TimerSoft(CN_TIMER_SOURCE_TICK);

    //��������ģ��
//    ModuleInstall_KeyBoard(0);
    //������������,����:��������ģ��
//    ModuleInstall_KeyBoardHard(0);

    //�ַ���ģ��
    ModuleInstall_Charset(0);
    //gb2312�ַ�����,����:�ַ���ģ��
    ModuleInstall_CharsetGb2312(0);
    //ascii�ַ���,ע��,gb2312������ascii,��ʼ����gb2312��,���뱾ģ��
    //����:�ַ���ģ��
//    ModuleInstall_CharsetAscii(0);
    //��ʼ��utf8�ַ���
//    ModuleInstall_CharsetUtf8(0);
    //���ʻ��ַ���֧��,���������ַ���ģ���Լ������ַ�����ʼ��
    ModuleInstall_CharsetNls("C");


    ModuleInstall_Font(0);                 //����ģ��
    //8*8�����ascii��������:����ģ��
//    ModuleInstall_FontAscii8x8Font(0);
    //6*12�����ascii��������:����ģ��
//    ModuleInstall_FontAscii6x12Font(0);
    //�����鰲װGB2312��������,������8*16��ascii����.����:����ģ��
    ModuleInstall_FontGb2312_816_1616_Array(0);
    //���ļ���װGB2312��������,������8*16��ascii����.����:����ģ��,�ļ�ϵͳ
//    ModuleInstall_FontGb2312_816_1616_File("sys:\\gb2312_1616");
    //8*16 ascii�����ʼ��,������128�ֽ�,����:����ģ��
    //ע:�����װ��GB2312,�����ٰ�װ
//    ModuleInstall_FonAscii8x16Font(0);

//    extern ptu32_t ModuleInstall_DjyIp(ptu32_t para);
//    ModuleInstall_DjyIp(0);
//    bool_t Enet1_ModuleInit(ptu32_t para);
//    Enet1_ModuleInit(0);

    ModuleInstall_GK(0);

    //����: gkernelģ��
    lcd_k70 = ModuleInstall_LCD(480,272,LCD_DEV_NAME,"extram");

    //��������,����:��ʾ������
    GK_ApiCreateDesktop(lcd_k70,&desktop,0,0,
                        CN_COLOR_RED+CN_COLOR_GREEN,CN_WINBUF_BUF,0,0);

    //������ģ��,����:gkernelģ�����ʾ������
    ModuleInstall_Touch(0);
     //����������,
     //����:������ģ��,������ʾ������,�Լ���������Ӳ��,����qh_1��IIC����.
     //     ����������ݴ����ļ���,�������ļ�ϵͳ.
     ModuleInstall_TOUCH_TWR_K70(&desktop,TOUCH_DEV_NAME);

    //���Ź�ģ��,��������˼���ʱιӲ����,���Ź����ģ��Ӵ˿�ʼ�ӹ�Ӳ����.
//    ModuleInstall_Wdt(0);

    ModuleInstall_GDD(&desktop,gdd_input_dev);
    //�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
    evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,__djy_main,
                                    NULL,gc_u32CfgMainStackLen,"main function");
    //�¼���������������Ϊ0�������shell�������������ɼ�shell�����в���
    Djy_EventPop(evtt_main,NULL,0,NULL,0,200);


    //�û��Լ�����ĳ�ʼ��ģ��,�����������.
    Heap_DynamicModuleInit(0);//�Դ�malloc����ִ�п������㷨
    //����,��ʼ��ʱʹ�õ�ջ,�Ѿ���ϵͳ����,�������ʹ˽���,��������ú��ѿ�
    return ;
}


