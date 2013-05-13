//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//版权所有 (C), 2005-2009,   lst.
//所属模块:IO初始化
//作者：lst
//版本：V1.0.0
//文件描述:IO初始化
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

//定义IO模式
//输入模式不区分是外设输入还是gpio输入
//作为片内外设输出时，多个外设连接到一个引脚上的，只能使能一个外设，或者用
//remap功能把它们映射到别的引脚上。
#define cn_gpio_mode_in_analog          0x00    //模拟输入
#define cn_gpio_mode_in_floating        0x04    //浮空输入
#define cn_gpio_mode_in_pulldown        0x08    //下拉输入
#define cn_gpio_mode_in_pullup          0x18    //上拉输入

#define cn_gpio_mode_gpio_out_od_10Mhz  0x05    //gpio开漏输出，速度10Mhz
#define cn_gpio_mode_gpio_out_od_2Mhz   0x06    //gpio开漏输出，速度2Mhz
#define cn_gpio_mode_gpio_out_od_50Mhz  0x07    //gpio开漏输出，速度50Mhz
#define cn_gpio_mode_gpio_out_pp_10Mhz  0x01    //gpio推挽输出，速度10Mhz 
#define cn_gpio_mode_gpio_out_pp_2Mhz   0x02    //gpio推挽输出，速度2Mhz  
#define cn_gpio_mode_gpio_out_pp_50Mhz  0x03    //gpio推挽输出，速度50Mhz 

#define cn_gpio_mode_peri_out_od_10Mhz  0x0d    //片内外设开漏输出，速度10Mhz
#define cn_gpio_mode_peri_out_od_2Mhz   0x0e    //片内外设开漏输出，速度2Mhz 
#define cn_gpio_mode_peri_out_od_50Mhz  0x0f    //片内外设开漏输出，速度50Mhz
#define cn_gpio_mode_peri_out_pp_10Mhz  0x09    //片内外设推挽输出，速度10Mhz
#define cn_gpio_mode_peri_out_pp_2Mhz   0x0a    //片内外设推挽输出，速度2Mhz 
#define cn_gpio_mode_peri_out_pp_50Mhz  0x0b    //片内外设推挽输出，速度50Mhz

#define cn_gpio_a   0
#define cn_gpio_b   1
#define cn_gpio_c   2
#define cn_gpio_d   3
#define cn_gpio_e   4
#define cn_gpio_f   5
#define cn_gpio_g   6

//gpioa at 0x40010800;
//gpiob at 0x40010c00;
//gpioc at 0x40011000;
//gpiod at 0x40011400;
//gpioe at 0x40011800;
//gpiof at 0x40011c00;
//gpiog at 0x40012000;

struct st_gpio_reg
{
  vu32 CRL;
  vu32 CRH;
  vu32 IDR;
  vu32 ODR;
  vu32 BSRR;
  vu32 BRR;
  vu32 LCKR;
};

//afio位域定义，bo_为位偏移，1位的用位带地址，bb_前缀，多位用掩码，bm_前缀
#define bb_afio_mapr_base           (0x42000000 + 0x10000*32)        //位带基址
#define bb_afio_mapr_spi1           (*(vu32*)(bb_afio_mapr_base+4*0))
#define bb_afio_mapr_i2c1           (*(vu32*)(bb_afio_mapr_base+4*1))
#define bb_afio_mapr_usart1         (*(vu32*)(bb_afio_mapr_base+4*2))
#define bb_afio_mapr_usart2         (*(vu32*)(bb_afio_mapr_base+4*3))
#define bm_afio_mapr_usart3         0x00000030
#define bm_afio_mapr_tim1           0x000000c0
#define bm_afio_mapr_tim2           0x00000300
#define bm_afio_mapr_tim3           0x00000c00
#define bb_afio_mapr_tim4           (*(vu32*)(bb_afio_mapr_base+4*12))
#define bm_afio_mapr_can            0x00006000
#define bb_afio_mapr_pd01           (*(vu32*)(bb_afio_mapr_base+4*15))
#define bb_afio_mapr_tim5ch4        (*(vu32*)(bb_afio_mapr_base+4*16))
#define bb_afio_mapr_adc1_etrginj   (*(vu32*)(bb_afio_mapr_base+4*17))
#define bb_afio_mapr_adc1_etrgreg   (*(vu32*)(bb_afio_mapr_base+4*18))
#define bb_afio_mapr_adc2_etrginj   (*(vu32*)(bb_afio_mapr_base+4*19))
#define bb_afio_mapr_adc2_etrgreg   (*(vu32*)(bb_afio_mapr_base+4*20))
#define bm_afio_mapr_swj_cfg        0x07000000
#define bo_afio_mapr_spi1           0
#define bo_afio_mapr_i2c1           1
#define bo_afio_mapr_usart1         2
#define bo_afio_mapr_usart2         3
#define bo_afio_mapr_usart3         4
#define bo_afio_mapr_tim1           6
#define bo_afio_mapr_tim2           8
#define bo_afio_mapr_tim3           10
#define bo_afio_mapr_tim4           12
#define bo_afio_mapr_can            13
#define bo_afio_mapr_pd01           15
#define bo_afio_mapr_tim5ch4        16
#define bo_afio_mapr_adc1_etrginj   17
#define bo_afio_mapr_adc1_etrgreg   18
#define bo_afio_mapr_adc2_etrginj   19
#define bo_afio_mapr_adc2_etrgreg   20
#define bo_afio_mapr_swj_cfg        24
struct st_afio_reg     //at 0x40010000;
{
  vu32 EVCR;
  vu32 MAPR;
  vu32 EXTICR[4];
} ;

extern struct st_gpio_reg volatile * const pg_gpio_rega;
extern struct st_gpio_reg volatile * const pg_gpio_regb;
extern struct st_gpio_reg volatile * const pg_gpio_regc;
extern struct st_gpio_reg volatile * const pg_gpio_regd;
extern struct st_gpio_reg volatile * const pg_gpio_rege;
extern struct st_gpio_reg volatile * const pg_gpio_regf;
extern struct st_gpio_reg volatile * const pg_gpio_regg;
extern struct st_afio_reg volatile * const pg_afio_reg;
#ifdef __cplusplus
}
#endif

#endif /*__cpu_peri_gpio_h_*/
