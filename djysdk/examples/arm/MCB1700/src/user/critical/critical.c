//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:��ȫ����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:��ȫ�����Ǳ����ڿ����󾡿��ܿ��ִ�еĲ��ִ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "cpu_peri.h"

const ufast_t tg_IntUsed[] =
{
    CN_INT_LINE_UART0,
    CN_INT_LINE_UART1,
    CN_INT_LINE_RTC,
    CN_INT_LINE_WDT,
    CN_INT_LINE_ENET,
    CN_INT_LINE_TIMER0,
    CN_INT_LINE_TIMER1,
    CN_INT_LINE_TIMER2,
    CN_INT_LINE_TIMER3,
    CN_INT_LINE_SSP0,
    CN_INT_LINE_SSP1
};

const ufast_t tg_IntUsedNum = sizeof(tg_IntUsed)/sizeof(ufast_t);

//----����ȫ��IO��--------------------------------------------------------------
//���ܣ���������⣬ȫ�����óɳ�ʼ̬�������ܿ�����Ӧ�Ĺ���ģ�����ж��塣
//��������
//���أ���
//------------------------------------------------------------------------------
void gpio_init(void)
{
}

void critical(void)
{
#if 0   //ARM�汾��Ϊ������ع�������
    //��ʱ����ʼ��
#if(CN_CFG_SYSTIMER == 1)
    extern void ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();
#endif
#if(CN_CFG_WDT == 1)
    //  ��ʼ��Ӳ�����Ź�
    extern bool_t WDT_LpcInitial(u32 setcycle);
    WDT_LpcInitial(1000);

    // ���Ź����HALι��
    //  �����жϲ�ע����صķ�����
    bool_t WdtHal_BootStart(u32 bootfeedtimes);
    WdtHal_BootStart(20);
#endif
#endif
}
