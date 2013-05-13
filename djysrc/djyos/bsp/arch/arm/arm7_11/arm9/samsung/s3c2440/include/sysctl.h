//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 系统初始化
//作者：lst
//版本：V1.0.0
//文件描述: 时钟和总线控制寄存器结构定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-10-30
//   作者: lst
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __sysctl_h__
#define __sysctl_h__

#ifdef __cplusplus
extern "C" {
#endif

//bwscon寄存器定义
#define bo_bus_ublb_bank7   31          //1=启用UB/LB
#define bo_bus_wait_bank7   30          //1=wait enable
#define bm_bus_width_bank7  0x30000000  //总线宽度，0=8，1=16，2=32，3=保留
#define bo_bus_width_bank7  28
#define bo_bus_ublb_bank6   27          //1=启用UB/LB
#define bo_bus_wait_bank6   26          //1=wait enable
#define bm_bus_width_bank6  0x03000000  //总线宽度，0=8，1=16，2=32，3=保留
#define bo_bus_width_bank6  24
#define bo_bus_ublb_bank5   23          //1=启用UB/LB
#define bo_bus_wait_bank5   22          //1=wait enable
#define bm_bus_width_bank5  0x00300000  //总线宽度，0=8，1=16，2=32，3=保留
#define bo_bus_width_bank5  20
#define bo_bus_ublb_bank4   19          //1=启用UB/LB
#define bo_bus_wait_bank4   18          //1=wait enable
#define bm_bus_width_bank4  0x00030000  //总线宽度，0=8，1=16，2=32，3=保留
#define bo_bus_width_bank4  16
#define bo_bus_ublb_bank3   15          //1=启用UB/LB
#define bo_bus_wait_bank3   14          //1=wait enable
#define bm_bus_width_bank3  0x00003000  //总线宽度，0=8，1=16，2=32，3=保留
#define bo_bus_width_bank3  12
#define bo_bus_ublb_bank2   11          //1=启用UB/LB
#define bo_bus_wait_bank2   10          //1=wait enable
#define bm_bus_width_bank2  0x00000300  //总线宽度，0=8，1=16，2=32，3=保留
#define bo_bus_width_bank2  8
#define bo_bus_ublb_bank1   7           //1=启用UB/LB
#define bo_bus_wait_bank1   6           //1=wait enable
#define bm_bus_width_bank1  0x00000030  //总线宽度，0=8，1=16，2=32，3=保留
#define bo_bus_width_bank1  4
#define bm_bus_width_bank0  0x00000006  //总线宽度，0=8，1=16，2=32，3=保留

//bankconx寄存器定义
#define bm_bankcon_tacs     0x6000      //地址建立周期，0=0，1=1，2=2，3=4
#define bm_bankcon_tcos     0x1800      //片选建立周期，0=0，1=1，2=2，3=4
#define bm_bankcon_tacc     0x0700      //访问周期，0=1，1=2，2=3，3=4，4=6
                                        //5=8，6=10，7=14，若wait允许，tacc>=4
#define bm_bankcon_tcoh     0x00c0      //片选保持周期，0=0，1=1，2=2，3=4
#define bm_bankcon_tcah     0x0030      //地址保持时间，0=0，1=1，2=2，3=4
#define bm_bankcon_tacp     0x000c      //页模式访问周期，0=2，1=3，2=4，3=6
#define bm_bankcon_pmc      0x0003      //页模式配置，0=normal，1=4bytes
                                        //            2=8bytes，3=16bytes
#define bo_bankcon_tacs     13
#define bo_bankcon_tcos     11
#define bo_bankcon_tacc     8
#define bo_bankcon_tcoh     6
#define bo_bankcon_tcah     4
#define bo_bankcon_tacp     2
#define bo_bankcon_pmc      0

//bank6/7位域定义
#define bm_b67_mt   0x18000     //存储器类型，0=sram,1,2=保留,3=SDRAM
#define bm_b67_trcd 0x0000c     //RAS to CAS clock,0=2,2=3,3=4
#define bm_b67_scan 0x00003     //列地址线数,0=8,2=9,3=10
#define bo_b67_mt   15
#define bo_b67_trcd 2
#define bo_b67_scan 0

//SDRAM 刷新控制
#define SDRAM_refresh_time  1269        //2049-7.8(uS)*cn_hclk

#define bo_refresh_refen    23          //1=使能刷新
#define bo_refresh_trefmd   22          //刷新模式，0=CBR(CAS before RAS)/Auto
                                        //refresh，1=self refresh
#define bm_refresh_trp      0x300000    //RAS预充电周期，0=2，1=3,2=4，4=保留
#define bm_refresh_trc      0x0c0000    //RC最短定时，0~3=4~7周期
#define bm_refresh_tchr     0x030000
#define bm_refresh_refcnt   0x0007ff    //sdram刷新计数=2049-7.8(uS)*cn_hclk
#define bo_refresh_trp      20
#define bo_refresh_trc      18
#define bo_refresh_tchr     16
#define bo_refresh_refcnt   0

//BANKSIZE寄存器位域
#define bo_banksize_bursten 7       //1=允许突发
#define bo_banksize_sckeen  5       //1=掉电模式使能
#define bo_banksize_sclken  4       //1=仅在访问时输出时钟
#define bm_banksize_size    0x07    //bank6/7内存尺寸，2=128/128M,1=64/64M
                                    //0=32/32M,7=16/16M,6=8/8M,5=4/4M,4=2/2M
#define bo_banksize_size    0

//MRSRB6/7寄存器设置
#define bm_mrsrb_cl         0x70
#define bo_mrsrb_cl         4

struct memory_ctrl_reg   //地址在0x48000000~0x48000030
{
    u32 BWSCON;    //0x48000000
    u32 BANKCON0;  //0x48000004
    u32 BANKCON1;  //0x48000008
    u32 BANKCON2;  //0x4800000C
    u32 BANKCON3;  //0x48000010
    u32 BANKCON4;  //0x48000014
    u32 BANKCON5;  //0x48000018
    u32 BANKCON6;  //0x4800001C
    u32 BANKCON7;  //0x48000020
    u32 REFRESH;   //0x48000024
    u32 BANKSIZE;  //0x48000028
    u32 MRSRB6;    //0x4800002C
    u32 MRSRB7;    //0x48000030
};

//时钟控制寄存器位域定义
//以下定义适合400M时钟
#define cn_mpll_mdiv	0x5c
#define cn_mpll_pdiv	0x1
#define cn_mpll_sdiv	0x1

//pll控制寄存器位域定义
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

//时钟使能寄存器位域定义
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

//时钟除数寄存器定义
#define bo_clkdivn_udivn    3   //0:uclk=upll输出，1:uclk=upll输出/2
#define bm_clkdivn_hdivn    0x6 //hclk除数，0：hclk=fclk,1:hclk=fclk/2
                                //2:hclk=fclk/4,若CAMDIVN[9]=1则再除2
                                //3:hclk=fclk/3,若CAMDIVN[8]=1则再除2
#define bo_clkdivn_hdivn    1
#define bo_clkdivn_pdivn    0   //0：pclk=hclk，1：pclk=hclk/2

struct clk_ctrl_reg   //地址在0x4c000000
{
    u32 LOCKTIME;
    u32 MPLLCON;
    u32 UPLLCON;
    u32 CLKCON;
    u32 CLKSLOW;
    u32 CLKDIVN;
    u32 CAMDIVN;
};


#ifdef __cplusplus
}
#endif

#endif /*__sysctl_h__*/

