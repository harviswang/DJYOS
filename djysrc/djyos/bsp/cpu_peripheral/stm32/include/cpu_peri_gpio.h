//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//��Ȩ���� (C), 2005-2009,   lst.
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:IO��ʼ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

//����IOģʽ
//����ģʽ���������������뻹��gpio����
//��ΪƬ���������ʱ������������ӵ�һ�������ϵģ�ֻ��ʹ��һ�����裬������
//remap���ܰ�����ӳ�䵽��������ϡ�
#define cn_gpio_mode_in_analog          0x00    //ģ������
#define cn_gpio_mode_in_floating        0x04    //��������
#define cn_gpio_mode_in_pulldown        0x08    //��������
#define cn_gpio_mode_in_pullup          0x18    //��������

#define cn_gpio_mode_gpio_out_od_10Mhz  0x05    //gpio��©������ٶ�10Mhz
#define cn_gpio_mode_gpio_out_od_2Mhz   0x06    //gpio��©������ٶ�2Mhz
#define cn_gpio_mode_gpio_out_od_50Mhz  0x07    //gpio��©������ٶ�50Mhz
#define cn_gpio_mode_gpio_out_pp_10Mhz  0x01    //gpio����������ٶ�10Mhz 
#define cn_gpio_mode_gpio_out_pp_2Mhz   0x02    //gpio����������ٶ�2Mhz  
#define cn_gpio_mode_gpio_out_pp_50Mhz  0x03    //gpio����������ٶ�50Mhz 

#define cn_gpio_mode_peri_out_od_10Mhz  0x0d    //Ƭ�����迪©������ٶ�10Mhz
#define cn_gpio_mode_peri_out_od_2Mhz   0x0e    //Ƭ�����迪©������ٶ�2Mhz 
#define cn_gpio_mode_peri_out_od_50Mhz  0x0f    //Ƭ�����迪©������ٶ�50Mhz
#define cn_gpio_mode_peri_out_pp_10Mhz  0x09    //Ƭ����������������ٶ�10Mhz
#define cn_gpio_mode_peri_out_pp_2Mhz   0x0a    //Ƭ����������������ٶ�2Mhz 
#define cn_gpio_mode_peri_out_pp_50Mhz  0x0b    //Ƭ����������������ٶ�50Mhz

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

//afioλ���壬bo_Ϊλƫ�ƣ�1λ����λ����ַ��bb_ǰ׺����λ�����룬bm_ǰ׺
#define bb_afio_mapr_base           (0x42000000 + 0x10000*32)        //λ����ַ
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
