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
//����ģ��: ϵͳ��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ʱ�Ӻ����߿��ƼĴ����ṹ����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-30
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __sysctl_h__
#define __sysctl_h__

#ifdef __cplusplus
extern "C" {
#endif

//bwscon�Ĵ�������
#define bo_bus_ublb_bank7   31          //1=����UB/LB
#define bo_bus_wait_bank7   30          //1=wait enable
#define bm_bus_width_bank7  0x30000000  //���߿�ȣ�0=8��1=16��2=32��3=����
#define bo_bus_width_bank7  28
#define bo_bus_ublb_bank6   27          //1=����UB/LB
#define bo_bus_wait_bank6   26          //1=wait enable
#define bm_bus_width_bank6  0x03000000  //���߿�ȣ�0=8��1=16��2=32��3=����
#define bo_bus_width_bank6  24
#define bo_bus_ublb_bank5   23          //1=����UB/LB
#define bo_bus_wait_bank5   22          //1=wait enable
#define bm_bus_width_bank5  0x00300000  //���߿�ȣ�0=8��1=16��2=32��3=����
#define bo_bus_width_bank5  20
#define bo_bus_ublb_bank4   19          //1=����UB/LB
#define bo_bus_wait_bank4   18          //1=wait enable
#define bm_bus_width_bank4  0x00030000  //���߿�ȣ�0=8��1=16��2=32��3=����
#define bo_bus_width_bank4  16
#define bo_bus_ublb_bank3   15          //1=����UB/LB
#define bo_bus_wait_bank3   14          //1=wait enable
#define bm_bus_width_bank3  0x00003000  //���߿�ȣ�0=8��1=16��2=32��3=����
#define bo_bus_width_bank3  12
#define bo_bus_ublb_bank2   11          //1=����UB/LB
#define bo_bus_wait_bank2   10          //1=wait enable
#define bm_bus_width_bank2  0x00000300  //���߿�ȣ�0=8��1=16��2=32��3=����
#define bo_bus_width_bank2  8
#define bo_bus_ublb_bank1   7           //1=����UB/LB
#define bo_bus_wait_bank1   6           //1=wait enable
#define bm_bus_width_bank1  0x00000030  //���߿�ȣ�0=8��1=16��2=32��3=����
#define bo_bus_width_bank1  4
#define bm_bus_width_bank0  0x00000006  //���߿�ȣ�0=8��1=16��2=32��3=����

//bankconx�Ĵ�������
#define bm_bankcon_tacs     0x6000      //��ַ�������ڣ�0=0��1=1��2=2��3=4
#define bm_bankcon_tcos     0x1800      //Ƭѡ�������ڣ�0=0��1=1��2=2��3=4
#define bm_bankcon_tacc     0x0700      //�������ڣ�0=1��1=2��2=3��3=4��4=6
                                        //5=8��6=10��7=14����wait����tacc>=4
#define bm_bankcon_tcoh     0x00c0      //Ƭѡ�������ڣ�0=0��1=1��2=2��3=4
#define bm_bankcon_tcah     0x0030      //��ַ����ʱ�䣬0=0��1=1��2=2��3=4
#define bm_bankcon_tacp     0x000c      //ҳģʽ�������ڣ�0=2��1=3��2=4��3=6
#define bm_bankcon_pmc      0x0003      //ҳģʽ���ã�0=normal��1=4bytes
                                        //            2=8bytes��3=16bytes
#define bo_bankcon_tacs     13
#define bo_bankcon_tcos     11
#define bo_bankcon_tacc     8
#define bo_bankcon_tcoh     6
#define bo_bankcon_tcah     4
#define bo_bankcon_tacp     2
#define bo_bankcon_pmc      0

//bank6/7λ����
#define bm_b67_mt   0x18000     //�洢�����ͣ�0=sram,1,2=����,3=SDRAM
#define bm_b67_trcd 0x0000c     //RAS to CAS clock,0=2,2=3,3=4
#define bm_b67_scan 0x00003     //�е�ַ����,0=8,2=9,3=10
#define bo_b67_mt   15
#define bo_b67_trcd 2
#define bo_b67_scan 0

//SDRAM ˢ�¿���
#define SDRAM_refresh_time  1269        //2049-7.8(uS)*CN_CFG_HCLK

#define bo_refresh_refen    23          //1=ʹ��ˢ��
#define bo_refresh_trefmd   22          //ˢ��ģʽ��0=CBR(CAS before RAS)/Auto
                                        //refresh��1=self refresh
#define bm_refresh_trp      0x300000    //RASԤ������ڣ�0=2��1=3,2=4��4=����
#define bm_refresh_trc      0x0c0000    //RC��̶�ʱ��0~3=4~7����
#define bm_refresh_tchr     0x030000
#define bm_refresh_refcnt   0x0007ff    //sdramˢ�¼���=2049-7.8(uS)*CN_CFG_HCLK
#define bo_refresh_trp      20
#define bo_refresh_trc      18
#define bo_refresh_tchr     16
#define bo_refresh_refcnt   0

//BANKSIZE�Ĵ���λ��
#define bo_banksize_bursten 7       //1=����ͻ��
#define bo_banksize_sckeen  5       //1=����ģʽʹ��
#define bo_banksize_sclken  4       //1=���ڷ���ʱ���ʱ��
#define bm_banksize_size    0x07    //bank6/7�ڴ�ߴ磬2=128/128M,1=64/64M
                                    //0=32/32M,7=16/16M,6=8/8M,5=4/4M,4=2/2M
#define bo_banksize_size    0

//MRSRB6/7�Ĵ�������
#define bm_mrsrb_cl         0x70
#define bo_mrsrb_cl         4

struct memory_ctrl_reg   //��ַ��0x48000000~0x48000030
{
    vu32 BWSCON;    //0x48000000
    vu32 BANKCON0;  //0x48000004
    vu32 BANKCON1;  //0x48000008
    vu32 BANKCON2;  //0x4800000C
    vu32 BANKCON3;  //0x48000010
    vu32 BANKCON4;  //0x48000014
    vu32 BANKCON5;  //0x48000018
    vu32 BANKCON6;  //0x4800001C
    vu32 BANKCON7;  //0x48000020
    vu32 REFRESH;   //0x48000024
    vu32 BANKSIZE;  //0x48000028
    vu32 MRSRB6;    //0x4800002C
    vu32 MRSRB7;    //0x48000030
};

//ʱ�ӿ��ƼĴ���λ����
//���¶����ʺ�400Mʱ��
#define cn_mpll_mdiv	0x5c
#define cn_mpll_pdiv	0x1
#define cn_mpll_sdiv	0x1

//pll���ƼĴ���λ����
#define bm_mpll_mdiv    0x000ff000
#define bm_mpll_pdiv    0x000003f0
#define bm_mpll_sdiv    0x00000003
#define bo_mpll_mdiv    12
#define bo_mpll_pdiv    4
#define bo_mpll_sdiv    0
#define bm_upll_mdiv    0x000ff000
#define bm_upll_pdiv    0x000003f0
#define bm_upll_sdiv    0x00000003
#define bo_upll_mdiv    12
#define bo_upll_pdiv    4
#define bo_upll_sdiv    0

//ʱ��ʹ�ܼĴ���λ����
#define bo_clkcon_ac97          20
#define bo_clkcon_camera        19
#define bo_clkcon_spi           18
#define bo_clkcon_iis           17
#define bo_clkcon_iic           16
#define bo_clkcon_adc_tch       15
#define bo_clkcon_rtc           14
#define bo_clkcon_gpio          13
#define bo_clkcon_uart2         12
#define bo_clkcon_uart1         11
#define bo_clkcon_uart0         10
#define bo_clkcon_sdi           9
#define bo_clkcon_pwm           8
#define bo_clkcon_usb_device    7
#define bo_clkcon_usb_host      6
#define bo_clkcon_lcd           5
#define bo_clkcon_nand          4
#define bo_clkcon_sleep         3
#define bo_clkcon_idle          2

//ʱ�ӳ����Ĵ�������
#define bo_clkdivn_udivn    3   //0:uclk=upll�����1:uclk=upll���/2
#define bm_clkdivn_hdivn    0x6 //hclk������0��hclk=fclk,1:hclk=fclk/2
                                //2:hclk=fclk/4,��CAMDIVN[9]=1���ٳ�2
                                //3:hclk=fclk/3,��CAMDIVN[8]=1���ٳ�2
#define bo_clkdivn_hdivn    1
#define bo_clkdivn_pdivn    0   //0��pclk=hclk��1��pclk=hclk/2

struct syscon_reg   //��ַ��0x4c000000
{
    vu32 LOCKTIME;
    vu32 MPLLCON;
    vu32 UPLLCON;
    vu32 CLKCON;
    vu32 CLKSLOW;
    vu32 CLKDIVN;
    vu32 CAMDIVN;
};

#define	SYSCON_REG_BASE	0x4C000000

#ifdef __cplusplus
}
#endif

#endif /*__sysctl_h__*/

