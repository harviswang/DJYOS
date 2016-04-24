/*========================================================*
 * �ļ�ģ��˵��:                                           *
 * omapl138��spi�������ļ����ṩ��spi�ڳ�ʼ���������������      *
 * �ļ��汾: v1.00                                         *
 * ������Ա: hm,liq                                        *
 * ����ʱ��: 2014.04.26                                    *
 * Copyright(c) 2011-2011  ��������������޹�˾                                  *
 *========================================================*
 * �����޸ļ�¼(���µķ�����ǰ��):                        *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>       *
 *========================================================*
* ���磺
*  V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
*
*/
#ifndef __CPU_PERI_SPI_H__
#define __CPU_PERI_SPI_H__
#include "stdint.h"


#ifdef __cplusplus
extern "C" {
#endif

//spi0 base address 0x01C41000
typedef struct
{
    u32 mode;       /* eSPI mode */
    u32 event;      /* eSPI event */
    u32 mask;       /* eSPI mask */
    u32 com;        /* eSPI command */
    u32 tx;     /* eSPI transmit FIFO access */
    u32 rx;     /* eSPI receive FIFO access */
    u8  res1[8];    /* reserved */
    u32 csmode[4];  /* 0x2c: sSPI CS0/1/2/3 mode */
    u8  res2[4048]; /* fill up to 0x1000 */
}tagSpiReg;


/* Controller-specific definitions: */

/* SPI mode flags */
#define SPI_CS_HIGH 0x04            /* CS active high */
#define SPI_LSB_FIRST   0x08            /* per-word bits-on-wire */
#define SPI_3WIRE   0x10            /* SI/SO signals shared */
#define SPI_LOOP    0x20            /* loopback mode */


/* SPI transfer flags */
#define SPI_XFER_BEGIN  0x01            /* Assert CS before transfer */
#define SPI_XFER_END    0x02            /* Deassert CS after transfer */


#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_SPI_H__*/

