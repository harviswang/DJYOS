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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: IICģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 17/09.2014
// =============================================================================

#include "stdio.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "endian.h"
#include "lock.h"

// =============================================================================
#define CN_IIC0_BASE        (0x40066000u)
#define CN_IIC1_BASE        (0x40067000u)

#define tagI2CReg            I2C_Type

static struct tagIIC_CB s_IIC0_CB;
static struct tagIIC_CB s_IIC1_CB;

#define IIC0_BUF_LEN  128
#define IIC1_BUF_LEN  128

struct tagIIC_IntParamSet
{
    struct tagSemaphoreLCB *pDrvPostSemp;   //�ź���
    u32 TransCount;                     //����������������
    u32 TransTotalLen;
};

static struct tagIIC_IntParamSet IntParamset0;
static struct tagIIC_IntParamSet IntParamset1;

// =============================================================================
// ��������д���ݵȴ�
// ���ܣ���/д��һ���ֽڵ����ݺ�ȴ�ʱ�䣬��ʱ��ǿ�Ʒ��أ��ȴ�MIF�ź���λ������鵽
//       MIF�����ж��Ƿ�ʧȥ�ٲ�MAL�ʹ�����ɱ��MCF������ж��Ƿ��յ�ACK�ź�
// ������ dev,�豸ָ��
//       write,д��־����Ϊд�����ж��Ƿ��յ�ACK�ź�
// ���أ�-1����ʱ��0����ȷ����
// =============================================================================
static int __iic_wait(volatile tagI2CReg *dev, int write)
{
    u32 csr;
    unsigned timeout = 0;

    do
    {
        Djy_DelayUs(1);

        csr = dev->S;
        if (!(csr & I2C_S_IICIF_MASK))
            continue;
        /* Read again to allow register to stabilise */
        csr = dev->S;

        dev->S |= I2C_S_IICIF_MASK;/*��״̬�Ĵ�����־*/

        if (csr & I2C_S_ARBL_MASK)                /*�ж��Ƿ�ʧ�ٲ�*/
        {
            #ifdef DEBUG
            printk("__i2c_wait: ARBL\n");
            #endif
            return -1;
        }

        if (!(csr & I2C_S_TCF_MASK))              /*�ж��Ƿ������*/
        {
            #ifdef DEBUG
            printk("__i2c_wait: unfinished\n");
            #endif
            return -1;
        }

        if (write == I2C_WRITE_BIT && (csr & I2C_S_RXAK_MASK)) /*дʱ���ж�ack*/
        {
            #ifdef DEBUG
            printk("__i2c_wait: No RXACK\n");
            #endif
            return -1;
        }

        return 0;
    } while (timeout++ < CONFIG_I2C_TIMEOUT);

    #ifdef DEBUG
    printk("__i2c_wait: timed out\n");
    #endif
    return -1;
}

// =============================================================================
// ���������ߵȴ�
// ���ܣ��ȴ�����״̬���ж�MBB�ж��Ƿ��������iic
// ������ dev,�豸ָ��
// ���أ�-1����ʱ��0����ȷ����
// =============================================================================
static int __iic_wait4bus(volatile tagI2CReg *dev)
{
    unsigned timeout = 0;

    while (dev->S & I2C_S_BUSY_MASK)
    {
//      writeb(dev->cr, I2C_CR_MEN | I2C_CR_MSTA | I2C_CR_MTX);
//      readb(dev->dr);
        Djy_DelayUs(1);
        dev->C1 |= I2C_C1_IICEN_MASK;
        if ( timeout++ > CONFIG_I2C_MBB_TIMEOUT)    /*�ж��Ƿ�ȴ�BB��ʱ*/
            return -1;
    }

    return 0;
}

// =============================================================================
// ������IICѰַ
// ���ܣ�Ѱַiic�豸���������߷�iic���豸��ַ�����һ��bit��ʾ��/дλ
// ������ dev,�豸ָ��
//       devaddr,���������ַ�����һbit���ֶ�д
//       dir,��д��ǣ�Ϊ0ʱΪд��Ϊ1ʱΪ��
//       rsta,�Ƿ�Ϊ��������
// ���أ�1,�ɹ���0,ʧ��
// =============================================================================
static int __iic_write_addr (volatile tagI2CReg *dev,
                            u8 devaddr, u8 dir, int rsta)
{
    dev->C1 = I2C_C1_IICEN_MASK | I2C_C1_MST_MASK | I2C_C1_TX_MASK |
            (rsta ? I2C_C1_RSTA_MASK : 0);/*ʹ��iic��start��mtxģʽ*/

    dev->D = (devaddr << 1) | dir;    /*д��ַ*/

    if (__iic_wait(dev,I2C_WRITE_BIT) < 0)
        return 0;

    return 1;
}

// =============================================================================
// ��������������
// ���ܣ��������ݵ�iic���������Ѿ������iic����Ѱַ���ڲ���ַ��д��Ч����
// ������ dev,����ָ��
//       data,���ݻ�����ָ��
//       length,data����������С���ֽڵ�λ
// ���أ��������������ֽڵ�λ
// =============================================================================
static int __iic_write(volatile tagI2CReg *dev, u8 *data, int length)
{
    int i;

    //����������ѭ�����ͣ�ÿ�η���һ�ֽ����ݣ���ȴ�ack�������Ϣ
    for (i = 0; i < length; i++)
    {
        dev->D = data[i];

        if (__iic_wait(dev,I2C_WRITE_BIT) < 0)
            break;
    }

    return i;
}

// =============================================================================
// ����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntEnable(volatile tagI2CReg *reg)
{
    reg->C1 |= I2C_C1_IICIE_MASK;
}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntDisable(volatile tagI2CReg *reg)
{
    reg->C1 &= ~I2C_C1_IICIE_MASK;
}

// =============================================================================
// ���ܣ���ʼ��ʱ������,ppc1020�ṩ��������i2c����ʱ������CCB/2���پ���i2cʱ�ӷ�Ƶ��
//        ����I2CFDR�Ĵ�������Ƶϵ�������ض��ļ��㷽������APIͨ��ö�ٲ���ȷ�����ֿ�
//        ���õĲ�����.
// ������reg���Ĵ���ָ��
//      i2cclk��ö������I2C_CLOCK_RATE_50K��ʱ��Ϊ50K
// ���أ�true=�ɹ���false=ʧ��
// =============================================================================
static void __IIC_SetClk(volatile tagI2CReg *reg,u32 i2cclk)
{
    //I2C bus clk devider
    if(i2cclk < CN_IIC_SPEED_100KHZ)
    {
        reg->F = 0xA5;  //50kHz
    }
    else if((i2cclk < CN_IIC_SPEED_200KHZ) && (i2cclk >= CN_IIC_SPEED_100KHZ))
    {
        reg->F = 0x9C;  //100kHz
    }
    else if((i2cclk < CN_IIC_SPEED_300KHZ) && (i2cclk >= CN_IIC_SPEED_200KHZ))
    {
        reg->F = 0x60;  //200kHz
    }
    else if((i2cclk < CN_IIC_SPEED_400KHZ) && (i2cclk >= CN_IIC_SPEED_300KHZ))
    {
        reg->F = 0x56;  //300kHz
    }
    else
    {
        reg->F = 0x53;  //400kHz
    }
}

// =============================================================================
// ����: Ӳ��GPIO��ʼ����������Դ��
// ����: IIC_NO,���
// ����: ��
// =============================================================================
void __IIC_GpioConfig(u8 IIC_NO)
{
    switch(IIC_NO)
    {
    case CN_IIC0:
        GPIO_PowerOn(GPIO_PORT_D);
        SIM->SCGC4 |= SIM_SCGC4_IIC0_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(18),PORT_PINMUX_ALT4);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(19),PORT_PINMUX_ALT4);
        break;
    case CN_IIC1:
        GPIO_PowerOn(GPIO_PORT_C);
        SIM->SCGC4 |= SIM_SCGC4_IIC1_MASK;
        PORT_MuxConfig(PORT_PORT_C,PORT_PIN(10),PORT_PINMUX_ALT2);
        PORT_MuxConfig(PORT_PORT_C,PORT_PIN(11),PORT_PINMUX_ALT2);
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: IICĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_HardDefaultSet(ptu32_t RegBaseAddr)
{
    volatile tagI2CReg *reg;
    reg = (tagI2CReg *)RegBaseAddr;

    reg->F = 0x9C;              //default 100KHz =  60M/(4 * 144)
    reg->A1 = 0xA0;             //Address When The IIC As Slaver
    reg->C1 |= I2C_C1_IICEN_MASK;
}

// =============================================================================
// ����: IIC�ж����ú���
// ����: IntLine,�ж���
//       isr,�жϷ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntConfig(u32 IntLine,u32 (*isr)(ufast_t))
{
    //�ж��ߵĳ�ʼ��
    Int_IsrConnect(IntLine,isr);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: ����ֹͣʱ�򣬽���������������֮���ͨ��
// ����: reg,iic����Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateStop(volatile tagI2CReg *reg)
{
    reg->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_IICEN_MASK);
}

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸������
// ������reg,�Ĵ�����ַ
//       devaddr,�豸��ַ
//       addr,���ʵļĴ�����ַ
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static s32 __IIC_ReadPoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u32 i;
    u8 mem_addr_buf[4];
    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,memaddr);

    // Write Device Address
    if(0 == __iic_write_addr(reg,devaddr,I2C_WRITE_BIT,0))
        return -1;

    // Write Register
    if(maddrlen != __iic_write(reg,mem_addr_buf,maddrlen))
        return -1;

    //Write Device Address
    if(0 == __iic_write_addr(reg,devaddr,I2C_READ_BIT,1))
        return -1;

   // Start Recving
    reg->C1 &= ~I2C_C1_TX_MASK;

    //������������ǳ���Ҫ�����û�л���ɶ�����
    reg->S;
    reg->D;

    for(i = 0; i < len; i++)
    {
        if(0 == __iic_wait(reg,I2C_READ_BIT))
            buf[i] = reg->D;
        else
            break;
    }

    reg->C1 = I2C_C1_IICEN_MASK;        // STOP
    // stop
    __iic_wait4bus(reg);
    reg->C1 = 0;    // Disable IIC

    return i;
}
// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸��д����
// ������reg,�Ĵ�����ַ
//       devaddr,�豸��ַ
//       memaddr,�豸�ڲ���ַ
//       maddrlen,�豸�ڲ���ַ����
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static s32 __IIC_WritePoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u8 mem_addr_buf[4];
    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,memaddr);

    // Write Device Address
    if(0 == __iic_write_addr(reg,devaddr,I2C_WRITE_BIT,0))
        return -1;

    // Write Memory Address
    if(maddrlen != __iic_write(reg,mem_addr_buf,maddrlen))
        return -1;

    //Write Data
    if(len != __iic_write(reg,buf,len))
        return -1;

    return len;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��дIIC�豸
// ������reg,�Ĵ�����ַ
//       DevAddr,�豸��ַ
//       MemAddr,�豸�ڲ���ַ
//       MemAddrLen,�豸�ڲ���ַ����
//       Buf,�洢����
//       Length,���ݳ��ȣ��ֽ�
//       WrRdFlag,��д��ǣ�Ϊ0ʱд��1ʱΪ��
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static bool_t __IIC_WriteReadPoll(tagI2CReg *reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    __IIC_IntDisable(reg);
    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //д
    {
        if(Length == __IIC_WritePoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
    else                                //��
    {
        if(Length == __IIC_ReadPoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
}
// =============================================================================
// ����: ����μ�iic_bus.h�й���WriteStartFunc��
// ����: ����μ�iic_bus.h�й���WriteStartFunc��
// ����: ����μ�iic_bus.h�й���WriteStartFunc��
// =============================================================================
static bool_t __IIC_GenerateWriteStart(tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct tagSemaphoreLCB *iic_semp)
{
    u8 mem_addr_buf[4];
    //ͨ��specific_flag��������������
    if((u32)reg == CN_IIC0_BASE)
    {
        IntParamset0.TransTotalLen = length;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;           //iic_bus_semp
    }
    else if((u32)reg == CN_IIC1_BASE)
    {
        IntParamset1.TransTotalLen = length;
        IntParamset1.TransCount = 0;
        IntParamset1.pDrvPostSemp = iic_semp;
    }
    else
    {
        return false;
    }

    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,mem_addr);
    __IIC_IntDisable(reg);

    //�ȷ���������ַ���ڲ���ַ��Ȼ���͵�һ������
    if (__iic_wait4bus(reg) >= 0)
    {
        if(__iic_write_addr(reg, dev_addr, I2C_WRITE_BIT, 0) != 0)
        {
            //���������ڲ���ַ
            if(maddr_len - 1 == __iic_write(reg,mem_addr_buf,maddr_len-1))
            {
                reg->D = mem_addr_buf[maddr_len-1];
                __IIC_IntEnable(reg);
                return true;
            }
        }
    }
    return false;
}

// =============================================================================
// ����: ����μ�iic_bus.h�й���ReadStartFunc��
// ����: ����μ�iic_bus.h�й���ReadStartFunc��
// ����: ����μ�iic_bus.h�й���ReadStartFunc��
// =============================================================================
static bool_t __IIC_GenerateReadStart( tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct tagSemaphoreLCB *iic_semp)
{
    u8 mem_addr_buf[4];

    if((u32)reg == CN_IIC0_BASE)
    {
        IntParamset0.TransTotalLen = length;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;               //iic_buf_semp
    }
    else if((u32)reg == CN_IIC1_BASE)
    {
        IntParamset1.TransTotalLen = length;
        IntParamset1.TransCount = 0;
        IntParamset1.pDrvPostSemp = iic_semp;
    }
    else
    {
        return false;
    }

    fill_little_32bit(mem_addr_buf,0,mem_addr);
    __IIC_IntDisable(reg);

    //д������ַ��Ѱַ��ַ
    if (__iic_wait4bus(reg) >= 0)
    {
        if(__iic_write_addr(reg, dev_addr, I2C_WRITE_BIT, 0) != 0)
        {
            if(__iic_write(reg, mem_addr_buf, maddr_len) == maddr_len)
            {
                if(length && __iic_write_addr(reg,dev_addr, I2C_READ_BIT,1) != 0)
                {
                    reg->C1 = I2C_C1_IICEN_MASK | I2C_C1_MST_MASK |
                            ((length == 1) ? I2C_C1_TXAK_MASK : 0);
                    //������������ǳ���Ҫ�����û�л���ɶ�����
                    reg->S;
                    reg->D;

                    __IIC_IntEnable(reg);
                    return true;
                }
            }
        }
    }

    return false;
}

// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��ֹͣʱ��
//      ��Ϊ��������ֹͣ�ظ�ACK����ֹͣ
// ����: reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateEnd(tagI2CReg *reg)
{
    if(reg == NULL)
        return;
    if(reg->C1 & I2C_C1_TX_MASK)//����
    {
        __IIC_GenerateStop(reg);
    }
    else //����
    {
        reg->C1 |= I2C_C1_TXAK_MASK;//ֹͣ�ظ�ACK
        __IIC_GenerateStop(reg);
    }
}

// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __IIC_BusCtrl(tagI2CReg *reg,u32 cmd,u32 data1,u32 data2)
{
    if( reg == NULL )
        return 0;
    switch(cmd)
    {
    case CN_IIC_SET_CLK:
        __IIC_SetClk(reg,data1);
        break;
    default:
        break;
    }
    return 1;
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
    static struct tagIIC_CB *ICB;
    static struct tagIIC_IntParamSet *IntParam;
    tagI2CReg *reg;
    u8 ch;
    u32 IicErrorNo;

    reg = (tagI2CReg*)CN_IIC0_BASE;

    if(reg->S & I2C_S_IICIF_MASK)//iic1�ж�
    {
        ICB = &s_IIC0_CB;
        IntParam = &IntParamset0;
    }
    else
    {
        reg = (tagI2CReg*)CN_IIC1_BASE;
        if(reg->S & I2C_S_IICIF_MASK)//iic2�ж�
        {
            ICB = &s_IIC1_CB;
            IntParam = &IntParamset1;
        }
        else
            return 0;
    }

    reg->S |= I2C_S_IICIF_MASK;            //���жϱ�־λ

    if(reg->C1 & I2C_C1_MST_MASK)        //�Ѿ���������
    {
        if(reg->C1 & I2C_C1_TX_MASK)      //�����ж�
        {
            if(!(reg->C1 & I2C_S_RXAK_MASK))
            {
                //�ӷ��ͻ�������һ���ֽڵ����ݣ�������
                if(IIC_PortRead(ICB,&ch,1) > 0)
                {
                    reg->D = ch;
                    IntParam->TransCount ++;;
                }
                else if(IntParam->TransCount == IntParam->TransTotalLen)
                {
                  Lock_SempPost(IntParam->pDrvPostSemp);
                  __IIC_GenerateStop(reg);
                }
            }
            else                        //δ�յ�ACK�ź�
            {
                IicErrorNo = CN_IIC_POP_NO_ACK_ERR;//���ô�����API����
                IIC_ErrPop(ICB,IicErrorNo);
                return 1;
            }
        }
        else                            //�����ж�
        {
            while((IntParam->TransCount < IntParam->TransTotalLen) &&
                                        (reg->S & I2C_S_TCF_MASK))
            {
                // ���һ���ֽ�master����ACK����ʾ��������ֹ
                if(IntParam->TransCount == IntParam->TransTotalLen - 1)
                {
                    reg->C1 |= I2C_C1_TXAK_MASK;
                }
                ch = reg->D;
                //д����
                IIC_PortWrite(ICB,&ch,1);
                IntParam->TransCount ++;
            }
            if((IntParam->TransCount == IntParam->TransTotalLen)  &&
                                        (reg->S & I2C_S_TCF_MASK))
            {
                __IIC_GenerateStop(reg);
                Lock_SempPost(IntParam->pDrvPostSemp);//�ͷ������ź���
            }
        }
    }
    else//δ����ͨ��
    {
        if(reg->S & I2C_S_ARBL_MASK)//�ٲö�ʧ�ж�
        {
            reg->C1 |= I2C_S_ARBL_MASK;
            IicErrorNo = CN_IIC_POP_MAL_LOST_ERR;
            IIC_ErrPop(ICB,IicErrorNo);
        }
        else//ֻ������ΪI2C������ʱ�������ж�
        {
        }
    }

    return 0;
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

    IIC0_Config.BusName             = "IIC0";
    IIC0_Config.IICBuf              = (u8*)&s_IIC0Buf;
    IIC0_Config.IICBufLen           = IIC0_BUF_LEN;
    IIC0_Config.SpecificFlag        = CN_IIC0_BASE;
    IIC0_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
    IIC0_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
    IIC0_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
    IIC0_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;
    IIC0_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;

    __IIC_GpioConfig(CN_IIC0);
    __IIC_HardDefaultSet(CN_IIC0_BASE);
    __IIC_IntConfig(CN_INT_LINE_I2C0,__IIC_ISR);

    if(NULL == IIC_BusAdd_s(&IIC0_Config,&s_IIC0_CB))
        return 0;
    return 1;
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
bool_t IIC1_Init(void)
{
    static u8 s_IIC1Buf[IIC1_BUF_LEN];
    struct tagIIC_Param IIC1_Config;

    IIC1_Config.BusName             = "IIC1";
    IIC1_Config.IICBuf              = (u8*)&s_IIC1Buf;
    IIC1_Config.IICBufLen           = IIC1_BUF_LEN;
    IIC1_Config.SpecificFlag        = CN_IIC1_BASE;
    IIC1_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
    IIC1_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
    IIC1_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
    IIC1_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;
    IIC1_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;

    __IIC_GpioConfig(CN_IIC1);
    __IIC_HardDefaultSet(CN_IIC1_BASE);
    __IIC_IntConfig(CN_INT_LINE_I2C1,__IIC_ISR);

    if(NULL == IIC_BusAdd_r(&IIC1_Config,&s_IIC1_CB))
        return 0;
    return 1;
}

