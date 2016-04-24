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
//����ģ��: iic��������(������������)
//����:  zhuhb-cyg.
//�汾��V1.0.0
//�ļ�����:
//����˵��:

//------------------------------------------------------
#include <string.h>
#include "stdint.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "endian.h"
#include "def21469.h"
#include "Cdef21469.h"

#define CN_IIC_REGISTER_BADDR0   CN_IIC0_BADDR
#define debug(x,...)
#define I2C_Address             (0x14)//����IIC��ַ������

#define CHKBIT(dest,mask)       (dest & mask)
#define SETBIT(dest,mask)       (dest |= mask)
#define CLRBIT(dest,mask)       (dest &= ~mask)


//�������ߵȴ���ʱʱ��
#ifndef CONFIG_I2C_MBB_TIMEOUT
#define CONFIG_I2C_MBB_TIMEOUT  100000
#endif

//�����д��ʱʱ��
#ifndef CONFIG_I2C_TIMEOUT
#define CONFIG_I2C_TIMEOUT  10000
#endif


//�����д��������λ
#define I2C_READ_BIT  1
#define I2C_WRITE_BIT 0


static struct tagIIC_CB s_IIC0_CB;

//��������С���ƣ�2-254.
//SHARC21469��Ϊ��������ʱ�������ϲ��Է������ݳ��������ƣ�
//��Ϊ��������ʱ��һ��IICʱ���ڿ��Խ��յ����ݳ�������Ϊ254�������Ҫ���ո�������ݱ�����������һ��IICʱ��
#define IIC0_BUF_LEN  254


#define TWI_ADDR_MASK           (0x7F)
#define TWI_ADDR_GET(n)         ((n>>1)& TWI_ADDR_MASK)


struct tagIIC_IntParamSet
{
    struct tagSemaphoreLCB *pDrvPostSemp;   //�ź���
    u32 TransCount;                     //����������������
    u32 TransTotalLen;
};

static struct tagIIC_IntParamSet IntParamset0;


//��������
 static int __TWI_WaitForTransOver(void);
 static int __TWI_WaitForRxReady(void);
 static int __TWI_WaitForTxReady(void);
 static int __TWI_WaitForTwiFree(void);
 static int __TWI_WaitForAck(int type);
 static int __TWI_WriteAddr(volatile tagI2CReg *dev,u8 *data,int length);
 void _IIC_IntEnable(volatile tagI2CReg *reg);
 void _IIC_IntDisable(volatile tagI2CReg *reg);
 void __IIC_GenerateStop(volatile tagI2CReg *reg);
 void _IIC_GenerateDisable(volatile tagI2CReg *reg);
 static u32 __IIC_ISR(ufast_t i2c_int_line);
 void __iic_set_clk(volatile tagI2CReg *reg,enum_I2C_Clock i2cclk);
 static void __IIC_HardDefaultSet(ptu32_t RegBaseAddr);
 static void __IIC_IntConfig(u32 IntLine,u32 (*isr)(ufast_t));
 static void __IIC_BusCtrl(ptu32_t spceific_flag,u32 cmd,u32 data1,u32 data2);
 static void __IIC_GenerateEnd(ptu32_t specific_flag);
 static bool_t __IIC_GenerateWriteStart(ptu32_t  specific_flag,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct tagSemaphoreLCB *iic_semp);
 static bool_t __IIC_GenerateReadStart(ptu32_t  specific_flag,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct tagSemaphoreLCB *iic_semp);



// =====================��д���ݵȴ�===========================================
// ���ܣ���/д��һ���ֽڵ����ݺ�ȴ�ʱ�䣬��ʱ��ǿ�Ʒ��أ��ȴ�MIF�ź���λ������鵽
//       MIF�����ж��Ƿ�ʧȥ�ٲ�MAL�ʹ�����ɱ��MCF������ж��Ƿ��յ�ACK�ź�
// ������ dev,�豸ָ��
//       write,д��־����Ϊд�����ж��Ƿ��յ�ACK�ź�
// ���أ�-1����ʱ��0����ȷ����
// =============================================================================
  static int __TWI_WaitForTransOver(void)
  {
    unsigned timeout = CONFIG_I2C_TIMEOUT;
    while( !(*pTWIIRPTL & TWIMCOM) )
    {
        if( timeout-- < 1 )
        {
            return -1;
        }
    }

    return 0;

  }

 // =================��ѯReceive buffer�Ƿ�������=================================
 //��������  : �ȴ�Rx FIFO�����ݣ�����һ������ʱ����-1.
 //�������  : void
 //�� �� ֵ  : TWI FIFO������һ�����ݼ�����0����ʱ����-1.
 // =============================================================================
  static int __TWI_WaitForRxReady(void)
  {
    unsigned timeout = CONFIG_I2C_TIMEOUT;
    while(!(*pTWIIRPTL&TWIRXINT))
    {
            if( timeout-- <1 )
        {
            return -1;
        }
    }

    return 0;
  }

// =================��ѯTransmit buffer�Ƿ�Ϊ����=================================
//��������  : �ȴ�Tx FIFO�ճ�(����FIFO���ճ�)����ʱ����-1.
//�������  : void
//�� �� ֵ  : TWI FIFO�շ���0����ʱ����-1.
// =============================================================================
  static int __TWI_WaitForTxReady(void)
  {
    unsigned timeout = CONFIG_I2C_TIMEOUT;
    //TWITXS��Transfer FIFO Status,00��ʾFIFO empty;01��ʾFIFO����һ���ֽ����ݣ�
    //10 Reserved��11,��ʾFIFO full��
    while(*pTWIIRPTL&TWITXINT)
    {
        if( timeout-- <1 )
        {
            return -1;
        }
    }

    return 0;
  }


// =====================IIC���ߵȴ�===========================================
// ���ܣ��ȴ�����״̬����ʱ����-1
// ������ void
// ���أ�-1����ʱ��0�����з���
// =============================================================================
static int __TWI_WaitForTwiFree(void)
{
    unsigned timeout = CONFIG_I2C_MBB_TIMEOUT;

    while(*pTWIMSTAT & TWIBUSY)
    {
        if(timeout--<1)
        {
            return -1;
        }
    }
    return 0;
}

// =====================��ѯ�Ƿ��յ�ACK�ź�====================================
// ���ܣ�I2C��Ϊ�������͵�ַ������ʱ���ӻ��ᷢ��ACK�ź�ȷ�ϣ��˺�����ѯ����
// �Ƿ��յ�ACK�ź�.
// ������ type�����������������ͣ�0��ʾ��ַ��1��ʾ����.
// ���أ��յ�ACK�źţ�����0�����򷵻�-1.
// =============================================================================

static int __TWI_WaitForAck(int type)
{
    unsigned timeout=CONFIG_I2C_TIMEOUT;
    if(type==0)
    {
        while(*pTWIMSTAT&TWIANAK)
        {
            if(timeout--<1)
            {
                return -1;
            }
        }
        return 0;
    }
    else if (type==1)
    {
        while(*pTWIMSTAT&TWIDNAK)
        {
            if(timeout--<1)
            {
                return -1;
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

// ==========================���Ͷ�/д�洢��ַ============================================
// ���ܣ����Ͷ�/д�洢��ַ��
// ������ dev,�豸ָ��(IIC�Ĵ�����ṹ��ָ��)
//      mem_addr,�洢���ڲ���ַ,�ú����跢�͵�maddr_len�ֽڵ�����
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��
// ���أ���δ�ɹ����ʹ洢��ַ������-1�����򷵻ط������������ֽڵ�λ.
// =============================================================================
static int __TWI_WriteAddr(volatile tagI2CReg *dev,u8 *data,int length)

{
    unsigned i=0;
    //����������ѭ�����ͣ�ÿ�η���һ�ֽ����ݣ���ȴ�ack�������Ϣ
    for(i=0;i<length;i++)
    {
        *pTXTWI8=data[i];
        if(__TWI_WaitForAck(0)!=0)
        {
            return -1;
        }

    }
    return i;

}

//----ʹ���ж�--------------------------------------------------------
//����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
//����: reg,�������ļĴ�����ָ��
//����: ��
//--------------------------------------------------------------------
void _IIC_IntEnable(volatile tagI2CReg *reg)
{
    SETBIT(reg->rTWIIMASK, TWIMCOM);
    SETBIT(reg->rTWIIMASK, TWITXINT);   //enable TX FIFO service interrupts
    SETBIT(reg->rTWIIMASK, TWIRXINT);   //enable RX FIFO service interrupts

}
//----��ֹ�ж�--------------------------------------------------------
//����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
//����: reg,�������ļĴ�����ָ��
//����: ��
//--------------------------------------------------------------------
void _IIC_IntDisable(volatile tagI2CReg *reg)
{
    CLRBIT(reg->rTWIIMASK, TWIMCOM);
    CLRBIT(reg->rTWIIMASK, TWITXINT);   //disable TX FIFO service interrupts
    CLRBIT(reg->rTWIIMASK, TWIRXINT);   //disable RX FIFO service interrupts
}

//----����ֹͣʱ��-----------------------------------------------------------
//����: ����ֹͣʱ�򣬽������������豸֮���ͨ��(����SHARC��˵��ֻ֧��MASTER TX MODE��ʹ��,��δʹ��DCNT)
//����: reg,iic����Ĵ�����ַ
//����: ��
//--------------------------------------------------------------------------
void __IIC_GenerateStop(volatile tagI2CReg *reg)
{
    SETBIT(reg->rTWIMCTL, TWISTOP);
}
//----�ر�IICģ��-----------------------------------------------------------
//����: �ر�IICģ�飬��ֹ�ж�(��������������ֹͣʱ��)
//����: reg,iic����Ĵ�����ַ
//����: ��
//--------------------------------------------------------------------------
void _IIC_GenerateDisable(volatile tagI2CReg *reg)
{
    CLRBIT(reg->rTWIIMASK, TWITXINT);   //disable TX FIFO service interrupts
    CLRBIT(reg->rTWIIMASK, TWIRXINT);   //disable RX FIFO service interrupts
    CLRBIT(reg->rTWIIMASK, TWIMCOM);
    CLRBIT(reg->rTWIMITR, TWIEN);
}
// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      dev_addr,������ַ��ǰ7���أ�������������λ�Ѿ����£��ú����ڲ��轫�õ�ַ��
//               ��һλ�����޸����Ķ�д����λ�����һ����д0����ʾд�����һ����д
//               1,��ʾ��;
//      mem_addr,�洢���ڲ���ַ,�ú����跢�͵�maddr_len�ֽڵ�����
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��
//      length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�
//          �����յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      iic_semp,�������ʱ�������ͷŵ��ź��������ͳ����IIC_PortReadʱ������������
//          �����ͷŸ��ź�������������ʱ��
// ����: TRUE��������ʱ��ɹ���FALSEʧ��

// =============================================================================
static bool_t __IIC_GenerateWriteStart(ptu32_t  specific_flag,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct tagSemaphoreLCB *iic_semp)
{
   volatile tagI2CReg *reg;
    u8 mem_addr_buf[4];
    //ͨ��specific_flag��������������
    if(specific_flag == CN_IIC_REGISTER_BADDR0)
    {
        IntParamset0.TransTotalLen = length;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;           //iic_bus_semp
    }
    else
    {
        return false;
    }
    fill_little_32bit(mem_addr_buf,0,mem_addr);
    reg=(tagI2CReg *)specific_flag;
    _IIC_IntDisable(reg);

    SETBIT(reg->rTWIMITR, TWIEN);                     //generate START
    //step1:���ȷ���start�ź�
    reg->rTWIMCTL |= TWIMEN;                //MASTER MODE

    //step2:����������ַ�����λд0.

     if(__TWI_WaitForTwiFree()>=0)
     {
        //reg->rTWIMADDR = TWI_ADDR_GET(dev_addr);              //device addr
        reg->rTWIMADDR =dev_addr;
        reg->rTWIMCTL = 0xFF<<6;                //clean MCTL reg

        //step3:�ж��Ƿ��յ�ACK.
         if(__TWI_WaitForAck(0)==0)
        {
        //step4:���յ��ӻ�����ACK����ʼ���洢��ַ.
           if(maddr_len==__TWI_WriteAddr(reg, mem_addr_buf,maddr_len-1));
           {
            *pTXTWI8=mem_addr_buf[maddr_len-1];
             _IIC_IntEnable(reg);
             return true;
           }

         }
     }

    return false;
}

// =============================================================================
// ����: ������ʱ��������ʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д����������ַ������
//       ��������ַ���������ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ��
//       ���������жϣ����ж��н����յ������ݵ���IIC_PortWriteд�뻺�壬���յ�len��
//       ���������ݺ��ͷ��ź���iic_semp
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      dev_addr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//      mem_addr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      len,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�����
//          �յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      iic_semp,�����ʱ���������ͷŵ��ź������������ź�����
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t __IIC_GenerateReadStart(ptu32_t  specific_flag,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct tagSemaphoreLCB *iic_semp)
{
     volatile tagI2CReg *reg;
      u8 mem_addr_buf[4];
      u32 Recv_Times=0;
      u32 Recv_Index=0;
      u32 i=0;
      u32 Single_Length_Max=0;
      Single_Length_Max=0xFE-maddr_len;
      Recv_Times=(u32)(length/Single_Length_Max);
      Recv_Index=Recv_Times;
      if (length % Single_Length_Max!=0)
      {
        Recv_Times++;
      }

      for(i=0;i<Recv_Index;i++)
  {

      if(specific_flag == CN_IIC_REGISTER_BADDR0)
    {

        if(i!=Recv_Index-1)
        {
        IntParamset0.TransTotalLen = Single_Length_Max;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;           //iic_bus_semp
        mem_addr=mem_addr+i*Single_Length_Max;
        }
        else
        {
        IntParamset0.TransTotalLen = length-(Recv_Index-1)*Single_Length_Max;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;           //iic_bus_semp

        }

    }
    else
    {
        return false;
    }


    fill_little_32bit(mem_addr_buf,0,mem_addr);
    reg=(tagI2CReg *)specific_flag;
    _IIC_IntDisable(reg);
    SETBIT(reg->rTWIMITR, TWIEN);                     //generate START

    //step1:���ȷ���start�ź�
    SETBIT(reg->rTWIMCTL,TWIMEN);                //MASTER MODE

     //step2:����������ַ�����λ��0.


     if(__TWI_WaitForTwiFree()>=0)
     {
        //reg->rTWIMADDR = TWI_ADDR_GET(dev_addr);              //device addr
        reg->rTWIMADDR =dev_addr;
        reg->rTWIMCTL = 0xFF<<6;                //clean MCTL reg
     //step3:�ж��Ƿ��յ�ACK.
         if(__TWI_WaitForAck(0)==0)
        {
      //step4:���յ��ӻ�����ACK����ʼ���洢��ַ.
            if(__TWI_WriteAddr(reg, mem_addr_buf,maddr_len)==0);
            {
      //step5:���ɹ�������洢��ַ�����ŷ���һ��Repeat Start�ź�
                SETBIT(reg->rTWIMCTL,TWIRSTART);
      //step6:����������ַ�����λ��1.
                SETBIT(reg->rTWIMCTL,TWIMDIR);
                reg->rTWIMADDR = TWI_ADDR_GET(dev_addr);
      //step7:���ж�
                _IIC_IntEnable(reg);

                if(i!=Recv_Index-1)
                {
                   Lock_SempPend(iic_semp,CN_TIMEOUT_FOREVER);
                }
                else
                {
                   return true;
                }

            }
       }
     }
  }
     return false;
}

// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��
//       ֹͣʱ����Ϊ��������ֹͣ�ظ�ACK����ֹͣ.
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateEnd(ptu32_t specific_flag)
{
    volatile tagI2CReg *reg;
    reg = (tagI2CReg *)specific_flag;

    /*if(CHKBIT(reg->TWIMCTL,TWIMDIR))
    {
      //����ģʽ

    }*/

}

// =============================================================================
// ���ܣ�IIC�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.ÿ���������һ���������ֽڷ���һ���жϣ�
//       2.���ж���ж�ʹ��ͬһ���жϺţ�������ݾ����������ʹ�õ����ĸ��жϣ�
//       3.���жϱ�־�����ж�ACK�źţ�ÿ��д�ֽڣ�������������Ӧ�޸ģ�
//       4.���մﵽ������һ���ֽ�ʱ�������ò�����ACK�źţ�
//       5.���ջ������ʱ����post�ź���IntParam->pDrvPostSemp��
//       6.���ջ������ʱ�������ֹͣʱ��
// ������i2c_int_line,�жϺţ�������û�õ�
// ���أ�������
// =============================================================================
static u32 __IIC_ISR(ufast_t i2c_int_line)
{

    static struct tagIIC_IntParamSet *IntParam;
    static struct tagIIC_CB *ICB;
    tagI2CReg *reg;
    u8 ch;
    u32 IicErrorNo;
    u32 irptl_temp=*rTWIIRPTL;//read IRPTL

    reg = (tagI2CReg*)CN_IIC_REGISTER_BADDR0;
    ICB=&s_IIC0_CB;
    IntParam=&IntParamset0;

        //MASTER TX\RX COMPLETE
        if( (irptl_temp & TWITXINT) != 0 )      //�����ж�
        {
            if(!(CHKBIT(reg->rTWIMSTAT, TWIANAK)|CHKBIT(reg->rTWIMSTAT, TWIDNAK)))
            {
                //�ӷ��豸��һ���ֽڵ����ݣ�������
                if(IIC_PortRead(ICB,&ch,1) > 0)
                {
                    *rTXTWI8 = ch;
                    IntParam->TransCount++;
                }
                else if(IntParam->TransCount == IntParam->TransTotalLen)
                {
                    //in Master TX Mode , we need to STOP TWI by ourself
                    Lock_SempPost(IntParam->pDrvPostSemp);
                     __IIC_GenerateStop(reg);
                }
                else
                {
                 IicErrorNo = CN_IIC_NO_ACK_ERR;//���ô�����API����
                 IIC_ErrPop(ICB,IicErrorNo);
                }
            }
            else    //TX no ACK
            {
                IicErrorNo = CN_IIC_NO_ACK_ERR;//���ô�����API����
                IIC_ErrPop(ICB,IicErrorNo);
                return 1;
            }
            //clear IIC interrupt
            irptl_temp = TWITXINT;
            *rTWIIRPTL = irptl_temp;
        }
        else if( (irptl_temp & TWIRXINT) != 0 )      //�����ж�
        {
            if(!(CHKBIT(reg->rTWIMSTAT, TWIANAK)|CHKBIT(reg->rTWIMSTAT, TWIDNAK)))
            {
                ch = *rRXTWI8;
                IIC_PortWrite(ICB,&ch,1);
                IntParam->TransCount ++;
                if(IntParam->TransCount == IntParam->TransTotalLen)
                {
                    __IIC_GenerateStop(reg);
                    Lock_SempPost(IntParam->pDrvPostSemp);//�ͷ������ź���
                }
            }

            else    //RX no ACK
            {

            }
            //clear IIC interrupt
            irptl_temp = TWIRXINT;
            *rTWIIRPTL = irptl_temp;
        }
        else    //TWIMERR
        {
        }

    irptl_temp==*rTWIIRPTL; //update TWI_IRPTL

        //MASTER TRANS COMPLETE
        if( (irptl_temp & TWIMCOM) != 0 )
        {
            _IIC_GenerateDisable(reg);

            Lock_SempPost(ICB->iic_bus_semp);//�ͷ������ź���

            //clear STOP
            CLRBIT(reg->rTWIMCTL, TWISTOP);
            //clear IIC interrupt
            irptl_temp |= TWIMCOM;
            *rTWIIRPTL = irptl_temp;
        }

    return 0;
}

// --------I2Cʱ�ӳ�ʼ��-----------------------------------------------------
//���ܣ�I2C��ʼ������������I2CЭ�飬��ʼ��ʱֻ��Ҫ����ʱ��,
//        ppc1020�ṩ��������i2c����ʱ������CCB/2���پ���i2c
//        ʱ�ӷ�Ƶ������I2CFDR�Ĵ�������Ƶϵ�������ض���
//        ���㷽������APIͨ��ö�ٲ���ȷ�����ֿ����õĲ�����.
//������I2Cn����������i2c�豸��Ϊö�����Un��Ϊ1��2��
//      tagI2CConfig��ö������I2C_CLOCK_RATE_50K��ʱ��Ϊ50K
//���أ�true=�ɹ���false=ʧ��
// -------------------------------------------------------------------------
void __iic_set_clk(volatile tagI2CReg *reg,enum_I2C_Clock i2cclk)
{
    //I2C bus clk devider

    reg->rTWIMITR = CN_CFG_PCLK1/(10*Mhz);      // reference clock = 10 MHz

    switch(i2cclk)
    {
    case I2C_CLOCK_RATE_50K:
        reg->rTWIDIV = 0xCDCD;  //clk=10.227 MHz/0xCD=10.227 MHz/205 = 49.9K
        //I2C_clk_cycle = 20;     //һ��ʱ������20us
        break;
    case I2C_CLOCK_RATE_100K:
        reg->rTWIDIV = 0x6666;  //clk=10.227 MHz/0x66=10.227 MHz/102 = 102.3K
        //I2C_clk_cycle = 10;     //һ��ʱ������10us
        break;
    case I2C_CLOCK_RATE_200K:
        reg->rTWIDIV = 0x3333;  //clk=10.227 MHz/0x33=10.227 MHz/51 = 200.5K
        //I2C_clk_cycle = 5;        //һ��ʱ������5us
        break;
    case I2C_CLOCK_RATE_400K:
        reg->rTWIDIV = 0x1A1A;  //clk=10.227 MHz/0x1A=10.227 MHz/26 = 393.3K
        //I2C_clk_cycle = 3;        //һ��ʱ������2.5us,����ȡ��
        break;
    default:
        break;
    }
}



// =============================================================================
// ����: IICĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ü�ʹ��IICģ�顣
// ����: RegBaseAddr,�Ĵ�����ַ��
// ����: ��
// =============================================================================

static void __IIC_HardDefaultSet(ptu32_t RegBaseAddr)
{
    volatile tagI2CReg *reg;
    reg=(tagI2CReg *)RegBaseAddr;
    SETBIT(reg->rTWIMITR,TWIEN);
    __iic_set_clk(reg,I2C_CLOCK_RATE_100K);

}


// =============================================================================
// ����: IIC�ж����ú���
// ����: IntLine,�ж���
//       isr,�жϷ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntConfig(u32 IntLine,u32 (*isr)(ufast_t))
{
     Int_IsrConnect(cn_int_line_TWII,__IIC_ISR);
    Int_SettoAsynSignal(cn_int_line_TWII);
    Int_ClearLine(cn_int_line_TWII);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(cn_int_line_TWII);

}


// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static void __IIC_BusCtrl(ptu32_t spceific_flag,u32 cmd,u32 data1,u32 data2)
{
    volatile tagI2CReg *reg;
    reg = (tagI2CReg *)spceific_flag;
    switch(cmd)
    {
    case CN_IIC_SET_CLK:
        __iic_set_clk(reg,data1);
        break;
    default:
        break;
    }
}








// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������para,�޾�������
// ���أ���
// =============================================================================
bool_t IIC0_Init(void)
{
    static u8 s_IIC0Buf[IIC0_BUF_LEN];
    struct tagIIC_Param IIC0_Config;
    IIC0_Config.BusName="IIC0";
    IIC0_Config.SimpleBuf=(u8 *)&s_IIC0Buf;
    IIC0_Config.SimpleBufLen=IIC0_BUF_LEN;
    IIC0_Config.SpecificFlag=CN_IIC0_BADDR;
    IIC0_Config.GenerateWriteStart=__IIC_GenerateWriteStart;
    IIC0_Config.GenerateReadStart  = __IIC_GenerateReadStart;
    IIC0_Config.GenerateEnd        = __IIC_GenerateEnd;
    IIC0_Config.BusCtrl            = __IIC_BusCtrl;

    __IIC_HardDefaultSet(CN_IIC_REGISTER_BADDR0);
    __IIC_IntConfig(cn_int_line_TWII,__IIC_ISR);

    if(NULL==IIC_BusAdd_r(&IIC0_Config,&s_IIC0_CB))
    {
        return 0;
    }
    return 1;

}





