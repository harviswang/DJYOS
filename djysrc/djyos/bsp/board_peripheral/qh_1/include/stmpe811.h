//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:��׼����--������ģ��
//����:  ������.
//�汾��V1.1.0
//�ļ�����: ������Ӳ����������
//����˵��:
//�޶���ʷ:
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#if (cfg_single_touch == 1)

#ifndef __STMPE811_H__
#define __STMPE811_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CHIP_ID         0x00
#define SYS_CTRL2       0x04
#define INT_CTRL        0x09
#define INT_EN          0x0A
#define INT_STA         0x0B
#define GPIO_AF         0x17
#define ADC_CTRL1       0x20
#define ADC_CTRL2       0x21
#define TSC_CTRL        0x40
#define TSC_CFG         0x41
#define FIFO_TH         0x4A
#define FIFO_STA        0x4B
#define FIFO_SIZE       0x4C
#define TSC_FRACT_XYZ   0x56
#define TSC_DATA        0x57
#define TSC_I_DRIVE     0x58


#define stmpe811_opmode 1

#ifdef __cplusplus
}
#endif

#endif /* __STMPE811_H__ */

#endif  //for #if (cfg_single_touch == 1)

