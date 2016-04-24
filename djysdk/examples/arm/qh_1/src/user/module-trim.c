#include "stdint.h"
#include "config-prj.h"
#include "driver.h"
#include "cpu_peri_uart.h"
#include "uart.h"
#include "gkernel.h"
#include "djyos.h"
extern ptu32_t ModuleInstall_DebugInfo(ptu32_t para);

ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}

const u32 gc_u32CfgTimeZone = 8;     //����ʱ��Ϊ��8��(����)

const char *gc_pCfgDefaultCharsetName = "gb2312";  //Ĭ���ַ���
const char *gc_pCfgDefaultFontName = "gb2312_8_16_1616";  //Ĭ������

const u32 gc_u32CfgGuiCmdDeep = 512;   //gkernel����������(�ֽ���,����������)
const u32 gc_u32CfgGuiStsDeep = 256;    //gkernel״̬���������(�ֽ���,����������)

const u32 gc_u32CfgDeviceLimit = 5;     //�����豸������
const u32 gc_u32CfgLockLimit = 20;      //�������������û�����semp_create��mutex_create�����������������ں��õ�������
const u32 gc_u32CfgEventLimit = 12;     //�¼�����
const u32 gc_u32CfgEvttLimit = 12;      //���¼�������
const u32 gc_u32CfgWdtLimit = 10;       //������������
const u32 gc_u32CfgPipeLimit = 1;       //�ܵ�����
const u32 gc_u32CfgMemPoolLimit = 10;    //������10���ڴ��
const u32 gc_u32CfgStdinDeviceLimit = 2;
const u32 gc_u32CfgMainStackLen = 2000;

const u32 gc_u32CfgOpenedfileLimit = 16;
const u32 gc_u32CfgTimerLimit = 5;

const char *gc_pCfgStddevName = "UART1";  //��׼��������豸
const char *gc_pCfgWorkPath = "sys:\\";   //����·������

bool_t Exp_InfoDecoderInit(void);

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
//----����ϵͳ�ں��������-----------------------------------------------------
//���ܣ����úͳ�ʼ����ѡ������������û�����Ŀ¼�б���ʵ�ֱ����������ں˳�ʼ��
//      �׶ε��á�
//      ������ʵ���ں˲ü����ܣ�����ֻҪע�͵�
//          ModuleInstall_Multiplex(0);
//      ��һ�У���·��������ͱ��ü����ˡ�
//      �û��ɴ�example��copy���ļ����Ѳ�Ҫ�����ע�͵���ǿ�ҽ��飬��Ҫɾ��,Ҳ
//      ��Ҫ�޸ĵ���˳�򡣿��԰��û�ģ��ĳ�ʼ������Ҳ���뵽��������,�������
//      ϵͳģ���ʼ������.
//      ��Щ�����������ϵ,�ü�ʱ,ע���Ķ�ע��.
//��������
//���أ���
//---------------------------------------------------------------------------
void Sys_ModuleInit(void)
{
    tagDevHandle char_term_hdl;
    u32 stdinout;
    uint16_t evtt_main;

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
    module_init_fs_k9f1208xxx("extram");
    //���ù���·��,����:�ļ�ϵͳ,�����·������.
    Djyfs_SetWorkPath(gc_pCfgWorkPath);

    ModuleInstall_UART(CN_UART1);
//    ModuleInstall_UART(CN_UART2);
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

    //����ʱ��ģ��
    ModuleInstall_TM(0);
    //ʹ��Ӳ��RTC,ע�͵���ʹ��tick��RTC��ʱ,����:����ʱ��ģ��
    ModuleInstall_RTC(0);

    //��ʱ�����
    ModuleInstall_TimerSoft(CN_TIMER_SOURCE_TICK);

//    ModuleInstall_DjyIp(0);

    //��������ģ��
    ModuleInstall_KeyBoard(0);
    //������������,����:��������ģ��
    ModuleInstall_KeyBoardHard(0);

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
//    ModuleInstall_FontGb2312_816_1616_Array(0);
    //���ļ���װGB2312��������,������8*16��ascii����.����:����ģ��,�ļ�ϵͳ
//    ModuleInstall_FontGb2312_816_1616_File("sys:\\gb2312_1616");
    //8*16 ascii�����ʼ��,������128�ֽ�,����:����ģ��
    //ע:�����װ��GB2312,�����ٰ�װ
    ModuleInstall_FonAscii8x16Font(0);


    //��ʼ��gui kernelģ��
    static struct tagGkWinRsc desktop;
    struct tagDisplayRsc *ili9325;

    ModuleInstall_GK(0);           //gkernelģ��
    //lcd������ʼ��,�����ϵͳ�ѵĻ�,�ڶ���������NULL
    //�ѵ�����,����lds�ļ���������,ע�ⲻҪ���.
    //����: gkernelģ��
    ili9325 = (struct tagDisplayRsc*)ModuleInstall_ili9325("ili9325","extram");

    //��������,����:��ʾ������
    GK_ApiCreateDesktop(ili9325,&desktop,0,0,
                        CN_COLOR_RED+CN_COLOR_GREEN,CN_WINBUF_BUF,0,0);

    //������ģ��,����:gkernelģ�����ʾ������
    ModuleInstall_Touch(0);
    //����������,
    //����:������ģ��,������ʾ������,�Լ���������Ӳ��,����qh_1��IIC����.
    //     ����������ݴ����ļ���,�������ļ�ϵͳ.
//    ModuleInstall_Touch_Stmpe811((ptu32_t)"ili9325");

    //���Ź�ģ��,��������˼���ʱι��,���Ź����ģ��Ӳ���Ӵ˿�ʼ�ӹ�Ӳ����.
    ModuleInstall_Wdt(0);

    evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                __djy_main,NULL,gc_u32CfgMainStackLen,
                                "main function");
    //�¼���������������Ϊ0?�����shell����?�������ɼ�shell�����в���
    Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

    //�û��Լ�����ĳ�ʼ��ģ��,�����������.
    Heap_DynamicModuleInit(0);  //�Դ�malloc����ִ�п������㷨
    //����,��ʼ��ʱʹ�õ�ջ,�Ѿ���ϵͳ����,�������ʹ˽���,��������ú��ѿ�

    return ;
}
