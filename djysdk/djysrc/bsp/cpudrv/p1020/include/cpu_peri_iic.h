// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: IICģ��ĵײ������������ɾ���ƽ̨�ĵײ���������Ա���
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 18/06.2014
// =============================================================================

#ifndef __CPU_PERI_I2C_H__
#define __CPU_PERI_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif


//offset of reg
typedef struct
{
  vu8 I2CADR;        // 0x0000
  vu8 RESERVED0[3];
  vu8 I2CFDR;        // 0x0004
  vu8 RESERVED1[3];
  vu8 I2CCR;        // 0x0008
  vu8 RESERVED2[3];
  vu8 I2CSR;        // 0x000C
  vu8 RESERVED3[3];
  vu8 I2CDR;        // 0x0010
  vu8 RESERVED4[3];
  vu8 I2CDFSRR;      // 0x0014
}tagI2CReg;

typedef enum
{
    I2C_CLOCK_RATE_50K,
    I2C_CLOCK_RATE_100K,
    I2C_CLOCK_RATE_200K,
    I2C_CLOCK_RATE_400K
}enum_I2C_Clock;//����IIC���ߵ�ʱ��


bool_t IIC0_Init(void);
bool_t IIC1_Init(void);


#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_SPI_H__*/

