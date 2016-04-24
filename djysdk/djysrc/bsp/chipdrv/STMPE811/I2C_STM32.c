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

#include "stm32f10x.h"


/************************ Local auxiliary functions ***************************/

/*******************************************************************************
* I2C communication status                                                     *
*   Parameter:                                                                 *
*   Return:               status                                               *
*******************************************************************************/

unsigned int I2C2_sr (void) {
  unsigned int sr;

  sr  = I2C2->SR1;
  sr |= I2C2->SR2 << 16;
  return (sr);
}

/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize I2C interface in master mode                                      *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void I2C2_Init (void) {
  unsigned int tout;

  /* Enable clock for I2C1, GPIOB and AFIO                                    */
  //RCC->APB2ENR |= (1 << 4) | (1 << 0);
  RCC->APB2ENR |= (1 << 3) | (1 << 0) ;
  RCC->APB1ENR |= (1 << 22);

  /* I2C2 pins  use PB10, PB11                                         */
  //AFIO->MAPR   |= 0x00000002;
  GPIOB->CRH   |= 0x0000FF00;

  I2C2->CR1     = 0x8000;               /* Reset I2C peripheral               */
  for (tout = 100; tout; tout--);
  I2C2->CR1     = 0x0000;

  /* Configure I2C peripheral                                                 */
  I2C2->CR1     = 0x0001;
  I2C2->CR2     = 0x0024;
  I2C2->CR1     = 0x0000;
  I2C2->TRISE   = 0x0025;
  I2C2->CCR     = 0x00b4;
  I2C2->CR1    |= 0x0401;
  I2C2->OAR1    = 0x40A0;
}


/*******************************************************************************
* Generate start condition on I2C bus                                          *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void I2C2_Start (void) {

  I2C2->CR1 |= 0x0100;
  while (!(I2C2_sr() & 0x0001));
}


/*******************************************************************************
* Generate stop condition on I2C bus                                           *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void I2C2_Stop (void) {

  I2C2->CR1 |= 0x0200;
  while (I2C2_sr() & 0x00020000);        /* Wait until BUSY bit reset          */
}


/*******************************************************************************
* Write address on I2C interface                                               *
*   Parameter:    adr:    address to be written                                *
*   Return:                                                                    *
*******************************************************************************/

void I2C2_Addr (unsigned int adr) {

  I2C2->DR = adr;
  while (!(I2C2_sr() & 0x0002));
}


/*******************************************************************************
* Write a byte to I2C interface                                                *
*   Parameter:    c:      data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

void I2C2_Write (unsigned int c) {

  I2C2->DR = c;
  while (!(I2C2_sr() & 0x00000004));     /* Wait until BTF bit set             */
}


/*******************************************************************************
* Read a byte from I2C interface                                               *
*   Parameter:                                                                 *
*   Return:               read data                                            *
*******************************************************************************/

unsigned char I2C2_Read (int ack) {

  /* Enable/disable Master acknowledge                                        */
  if (ack) I2C2->CR1 |=  0x0400;
  else     I2C2->CR1 &= ~0x0400;

  while (!(I2C2_sr() & 0x00000040));     /* Wait until RxNE bit set            */
  return (I2C2->DR);
}

/******************************************************************************/
