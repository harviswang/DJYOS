
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
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */
#if 0
//#include "os_inc.h"
#include "cpu_peri.h"
#include "printf.h"
#include "string.h"
#include "string.h"

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN_NO(x)              (((1)<<(x & GPIO_PIN_MASK)))

ptu32_t Led1Task(void)
{
    /* make sure port A clock is on */
//      SIM_SCGC5 = SIM_SCGC5_PORTA_MASK ;

    /* Set PTA10 (connected to LED) for GPIO functionality  */
//      PORTA_PCR29=(0|PORT_PCR_MUX(1));
//      PORTA_PCR28=(0|PORT_PCR_MUX(1));
	PORT_MuxConfig(PORT_PORT_A,PORT_PIN(28),PORT_PINMUX_GPIO);
	PORT_MuxConfig(PORT_PORT_A,PORT_PIN(29),PORT_PINMUX_GPIO);

    /* Change PTA10 to output  */
//      GPIOA_PDDR=GPIO_PDDR_PDD(GPIO_PIN_NO(29) | GPIO_PIN_NO(28));
    GPIO_PinConfig(GPIO_PORT_A,GPIO_PIN(28),GPIO_FUNC_OUTPUT);
    GPIO_PinConfig(GPIO_PORT_A,GPIO_PIN(29),GPIO_FUNC_OUTPUT);

	while(1)
	{
		//Toggle the green LED on PTC9
//		GPIOA_PTOR|=GPIO_PDOR_PDO(GPIO_PIN_NO(28));
		GPIO_PinToggle(GPIO_PORT_A,GPIO_PIN(28));
		Djy_EventDelay(1000000);
	}
	return 0;
}

void LedTest(void)
{
	uint16_t evtt_led;
	evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,2,
                                Led1Task,NULL,0x100,"led1 blink");

    Djy_EventPop(evtt_led,NULL,0,NULL,0,0,0);
}

ptu32_t port_int(ufast_t intline)
{
	GPIO_PinSet(GPIO_PORT_A,GPIO_PIN(29));
	Djy_EventDelay(1000000);

	PORT_IntFlagClear(PORT_PORT_E,PORT_PIN(26));
	return 0;
}

void GPIO_IntInit(void)
{
	//sw2
	PORT_IntConfig(PORT_PORT_E,PORT_PIN(26),IQC_INT_RISING_EDGE,PORT_PULL_UP);
	PORT_MuxConfig(PORT_PORT_E,PORT_PIN(26),PORT_PINMUX_GPIO);

    Int_SetClearType(CN_INT_LINE_PORTE,CN_INT_CLEAR_PRE);
    Int_IsrConnect(CN_INT_LINE_PORTE,port_int);
    Int_SettoAsynSignal(CN_INT_LINE_PORTE);
    Int_ClearLine(CN_INT_LINE_PORTE);
    Int_RestoreAsynLine(CN_INT_LINE_PORTE);
}

#include "timer.h"
TIMERSOFT_HANDLE timerhl;
void test_timer_isr(void)
{
	djy_printk("timer test\r\n");
}
void test_timer_isr0(void)
{
	djy_printk("timer test0\r\n");
}
void Timer_Test(void)
{
	timerhl = TimerSoft_Create("testtimer",2000*mS,test_timer_isr);
	TimerSoft_Ctrl(timerhl,EN_TIMER_STARTCOUNT,0);
	timerhl = TimerSoft_Create("testtimer0",3000*mS,test_timer_isr0);
	TimerSoft_Ctrl(timerhl,EN_TIMER_STARTCOUNT,0);
}
bool_t ddr_test(char *param)
{
	char *str_addr,*next_param;
	u32 addr,data=0;

	str_addr = Sh_GetWord(param,&next_param);
	if(str_addr == NULL)
	{
		djy_printk("��������\r\n");
		return false;
	}
	addr = atol(str_addr);

	*((u32*)addr) = 0xaabb5566;
	data = *((u32*)addr);

	djy_printk("addr = 0x%8x, data = 0x%8x \r\n",addr,data);
	return true;
}

bool_t gui_test(void)
{
//    __lcd_hard_init();
//    __lcd_power_enable(0,1);
//    __lcd_envid_of(1);
//    color_demo_data16();
}

bool_t ddr2_test(void)
{
	u32 i,j = 0x08000000,addr;
	u8 result = 0;

	for(i = j; i < j + 0x7FF000;)
	{
		addr = i;
		*((u32*)addr) = 0xaabb5566;
		if( 0xaabb5566 != *((u32*)addr))
		{
			djy_printk("ddr2 test error!\r\n");
			result = 1;
			break;
		}
		i += 4;
	}

	if(result == 0)
		djy_printk("ddr2 test sucessed !\r\n");

	return true;
}

bool_t nand_erase(char *param)
{
	char *erase_block,*next_param;
	u32 block;

	erase_block = Sh_GetWord(param,&next_param);
	if(erase_block == NULL)
	{
		djy_printk("��������\r\n");
		return false;
	}
	block = atol(erase_block);
	erase_block_nand(block);
	djy_printk("������ɣ�\r\n");
	return true;
}


extern bool_t CRT_Test(void);
extern bool_t nand_test(void);
extern bool_t nand_reset(void);

extern bool_t test_flash_file(void);
bool_t create_ptt(void)
{
	static created = 0;
	if(created == 0)
	{
		test_flash_file();
		created = 1;
	}
}
//------------------------------------------------------------------------------
//monitor���Ա�
//------------------------------------------------------------------------------
struct tagShellCmdTab  shell_monitordebug_table[] =
{
    {
    		"ddr_test",
    		ddr_test,
    		"ddr����",
    		NULL
    },
    {
    		"gui_test",
    		gui_test,
    		"gui����",
    		NULL
    },
    {
    		"crt_test",
    		CRT_Test,
    		"crt����",
    		NULL
    },
    {
    		"nand_test",
    		nand_test,
    		"nand����",
    		NULL
    },
    {
    		"nand_erase",
    		nand_erase,
    		"nand����",
    		NULL
    },
    {
    		"create_ptt",
    		create_ptt,
    		"�����ļ���",
    		NULL
    }

};
#define cn_monitor_test_num  ((sizeof(shell_monitordebug_table))/(sizeof(struct tagShellCmdTab)))
static struct tagShellCmdRsc tg_monitorshell_cmd_rsc[cn_monitor_test_num];

ptu32_t MainTestEntry(void)
{
	LedTest();
	djy_printk("FreeScale KENITIS Kxx Serials Board!\n\r");

//	CRT_Init();

	Sh_InstallCmd(shell_monitordebug_table,tg_monitorshell_cmd_rsc,
			cn_monitor_test_num);
//	extern void gui_app(void);
//	gui_app();
//	while(1)
//	{
//		Djy_EventDelay(1000000);
//	}
	return 0;
}
#else
#include "cpu_peri.h"

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN_NO(x)              (((1)<<(x & GPIO_PIN_MASK)))

ptu32_t Led1Task(void)
{
    /* make sure port A clock is on */
//      SIM_SCGC5 = SIM_SCGC5_PORTA_MASK ;

    /* Set PTA10 (connected to LED) for GPIO functionality  */
//      PORTA_PCR29=(0|PORT_PCR_MUX(1));
//      PORTA_PCR28=(0|PORT_PCR_MUX(1));
	PORT_MuxConfig(PORT_PORT_A,PORT_PIN(28),PORT_PINMUX_GPIO);
	PORT_MuxConfig(PORT_PORT_A,PORT_PIN(29),PORT_PINMUX_GPIO);

    /* Change PTA10 to output  */
//      GPIOA_PDDR=GPIO_PDDR_PDD(GPIO_PIN_NO(29) | GPIO_PIN_NO(28));
    GPIO_PinConfig(GPIO_PORT_A,GPIO_PIN(28),GPIO_FUNC_OUTPUT);
    GPIO_PinConfig(GPIO_PORT_A,GPIO_PIN(29),GPIO_FUNC_OUTPUT);

	while(1)
	{
		//Toggle the green LED on PTC9
//		GPIOA_PTOR|=GPIO_PDOR_PDO(GPIO_PIN_NO(28));
		GPIO_PinToggle(GPIO_PORT_A,GPIO_PIN(28));
		Djy_EventDelay(1000000);
	}
	return 0;
}

void LedTest(void)
{
	uint16_t evtt_led;
	evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                Led1Task,NULL,0x100,"led1 blink");

    Djy_EventPop(evtt_led,NULL,0,NULL,0,0);
}
ptu32_t MainTestEntry(void)
{
	LedTest();
//	gui_app();
}
#endif


