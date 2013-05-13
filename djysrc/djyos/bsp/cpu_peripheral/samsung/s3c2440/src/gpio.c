//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:IO初始化
//作者：lst
//版本：V1.0.0
//文件描述:IO初始化
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

//----配置全部IO扣--------------------------------------------------------------
//功能：除必须的外，全部配置成初始态，各功能口由相应的功能模块自行定义。
//参数：无
//返回：无
//------------------------------------------------------------------------------
void gpio_init(void)
{
    //32bit data bus configuration
    //=== PORT A GROUP
    //Ports  : GPA22 GPA21  GPA20 GPA19 GPA18 GPA17 GPA16 GPA15 GPA14 GPA13 GPA12
    //Signal : nFCE nRSTOUT nFRE   nFWE  ALE   CLE  nGCS5 nGCS4 nGCS3 nGCS2 nGCS1
    //Binary :  1     1      1  , 1   1   1    1   ,  1     1     1     1
    //Ports  : GPA11   GPA10  GPA9   GPA8   GPA7   GPA6   GPA5   GPA4   GPA3   GPA2   GPA1  GPA0
    //Signal : ADDR26 ADDR25 ADDR24 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0
    //Binary :  1       1      1      1   , 1       1      1      1   ,  1       1     1      1
    pg_gpio_reg->GPACON = 0x5fffff;

    //===* PORT B GROUP
    //Ports  : GPB10    GPB9    GPB8    GPB7    GPB6     GPB5    GPB4   GPB3   GPB2     GPB1      GPB0
    //Signal : nXDREQ0 nXDACK0 nXDREQ1 nXDACK1 nSS_KBD nDIS_OFF L3CLOCK L3DATA L3MODE nIrDATXDEN Keyboard
    //Setting: INPUT  OUTPUT   INPUT  OUTPUT   INPUT   OUTPUT   OUTPUT OUTPUT OUTPUT   OUTPUT    OUTPUT
    //Binary :   00  ,  01       00  ,   01      00   ,  01       01  ,   01     01   ,  01        01
    pg_gpio_reg->GPBCON = 0x044555;
    pg_gpio_reg->GPBUP  = 0x7ff;     // The pull up function is disabled GPB[10:0]

    //=== PORT C GROUP
    //Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9 GPC8  GPC7   GPC6   GPC5 GPC4 GPC3  GPC2  GPC1 GPC0
    //Signal : VD7   VD6   VD5   VD4   VD3   VD2   VD1  VD0 LCDVF2 LCDVF1 LCDVF0 VM VFRAME VLINE VCLK LEND
    //Binary :  10   10  , 10    10  , 10    10  , 10   10  , 10     10  ,  10   10 , 10     10 , 10   10
    pg_gpio_reg->GPCCON = 0xaaaa02a8;
    pg_gpio_reg->GPCUP  = 0xffff;     // The pull up function is disabled GPC[15:0]

    //=== PORT D GROUP
    //Ports  : GPD15 GPD14 GPD13 GPD12 GPD11 GPD10 GPD9 GPD8 GPD7 GPD6 GPD5 GPD4 GPD3 GPD2 GPD1 GPD0
    //Signal : VD23  VD22  VD21  VD20  VD19  VD18  VD17 VD16 VD15 VD14 VD13 VD12 VD11 VD10 VD9  VD8
    //Binary : 10    10  , 10    10  , 10    10  , 10   10 , 10   10 , 10   10 , 10   10 ,10   10
    pg_gpio_reg->GPDCON = 0xaaaaaaaa;
    pg_gpio_reg->GPDUP  = 0xffff;     // The pull up function is disabled GPD[15:0]
    *(volatile unsigned *)0x4d000000 = *(volatile unsigned *)0x4d000000 & 0x3fffe;

    //=== PORT E GROUP
    //Ports  : GPE15  GPE14 GPE13   GPE12   GPE11   GPE10   GPE9    GPE8     GPE7  GPE6  GPE5   GPE4
    //Signal : IICSDA IICSCL SPICLK SPIMOSI SPIMISO SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK I2SSDO
    //Binary :  10     10  ,  10      10  ,  10      10   ,  10      10   ,   10    10  , 10     10  ,
    //-------------------------------------------------------------------------------------------------------
    //Ports  :  GPE3   GPE2  GPE1    GPE0
    //Signal : I2SSDI CDCLK I2SSCLK I2SLRCK
    //Binary :  10     10  ,  10      10
    pg_gpio_reg->GPECON = 0xaaaaaaaa;
    pg_gpio_reg->GPEUP  = 0xffff;     // The pull up function is disabled GPE[15:0]

    //=== PORT F GROUP
    //Ports  : GPF7   GPF6   GPF5   GPF4      GPF3     GPF2  GPF1   GPF0
    //Signal : nLED_8 nLED_4 nLED_2 nLED_1 nIRQ_PCMCIA key1  KBDINT key0
    //Setting: Output Output Output Output    EINT3    input EINT1  input
    //Binary :  01      01 ,  01     01  ,     10       00  , 10     00
    pg_gpio_reg->GPFCON = 0x5588;
    pg_gpio_reg->GPFUP  = 0xff;     // The pull up function is disabled GPF[7:0]

    //=== PORT G GROUP
    //Ports  : GPG15 GPG14 GPG13 GPG12 GPG11    GPG10    GPG9     GPG8     GPG7      GPG6
    //Signal : nYPON  YMON nXPON XMON  key3   DMAMODE1 DMAMODE0 DMASTART KBDSPICLK KBDSPIMOSI
    //Setting: nYPON  YMON nXPON XMON  input  Output   Output   Output   SPICLK1    SPIMOSI1
    //Binary :   11    11 , 11    11  , 00      01    ,   01       01   ,    11         11
    //-----------------------------------------------------------------------------------------
    //Ports  :    GPG5       GPG4    GPG3    GPG2    GPG1    GPG0
    //Signal : KBDSPIMISO LCD_PWREN key2    nSS_SPI IRQ_LAN IRQ_PCMCIA
    //Setting:  SPIMISO1  LCD_PWRDN input    nSS0   EINT9    EINT8
    //Binary :     11         11   ,  00      11  ,  10        10
    pg_gpio_reg->GPGCON = 0xff15ff3a;
    pg_gpio_reg->GPGUP  = 0xffff;    // The pull up function is disabled GPG[15:0]

    //=== PORT H GROUP
    //Ports  :  GPH10    GPH9  GPH8 GPH7  GPH6  GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal : CLKOUT1 CLKOUT0 UCLK nCTS1 nRTS1 RXD1 TXD1 RXD0 TXD0 nRTS0 nCTS0
    //Binary :   10   ,  10     10 , 11    11  , 10   10 , 10   10 , 10    10
    pg_gpio_reg->GPHCON = 0x2afaaa;
    pg_gpio_reg->GPHUP  = 0x7ff;    // The pull up function is disabled GPH[10:0]
}

