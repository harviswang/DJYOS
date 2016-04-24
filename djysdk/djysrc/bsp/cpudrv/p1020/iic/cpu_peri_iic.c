// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: IICģ��ĵײ������������ɾ���ƽ̨�ĵײ���������Ա���
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 18/06.2014
// =============================================================================

#include <string.h>
#include "stdint.h"
#include "cpu_peri.h"
#include "endian.h"
#include "iicbus.h"
#include "lock.h"
#include "int_hard.h"

#define debug(x,...)
#define CN_IIC_REGISTER_BADDR0  cn_i2c1_baddr
#define CN_IIC_REGISTER_BADDR1  cn_i2c2_baddr

#define debug(x,...)
#define I2C_Address             (0x14)//����IIC��ַ������

#define I2C_Write_Code          (0xfe)//д����ڴ��豸��ַ���һλ����Ϊ0
#define I2C_Read_Code           (0x01)//������ڴ��豸��ַ���һλ����Ϊ1

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

//�����Ĵ���
#define writeb(reg, data)       (reg = data)
#define readb(reg)              (reg)

//�Ĵ���λ����
//���ƼĴ���
#define I2C_CR_MEN                  (1<<7)//ʹ��
#define I2C_CR_MIEN                 (1<<6)//�ж�ʹ��
#define I2C_CR_MSTA                 (1<<5)//I2C����ֹͣ
#define I2C_CR_MTX                  (1<<4)//���ͽ���ģʽ
#define I2C_CR_TXAK                 (1<<3)//����ACK�ź�
#define I2C_CR_RSTA                 (1<<2)//��������

//״̬�Ĵ���
#define I2C_SR_MCF                  (1<<7)//I2C������ɱ�־
#define I2C_SR_MBB                  (1<<5)//I2C��Ϊ���豸
#define I2C_SR_MAL                  (1<<4)//I2C����æ��־
#define I2C_SR_SRW                  (1<<2)//I2C�жϱ�־λ
#define I2C_SR_MIF                  (1<<1)//I2C�ٲö�ʧ
#define I2C_SR_RXAK                 (1<<0)//I2C���յ�ACK�ź�
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

        csr = readb(dev->I2CSR);
        if (!(csr & I2C_SR_MIF))
            continue;
        /* Read again to allow register to stabilise */
        csr = readb(dev->I2CSR);

        writeb(dev->I2CSR, 0x0);/*��״̬�Ĵ�����־*/

        if (csr & I2C_SR_MAL)                 /*�ж��Ƿ�ʧ�ٲ�*/
        {
            debug("__i2c_wait: MAL\n");
            return -1;
        }

        if (!(csr & I2C_SR_MCF))              /*�ж��Ƿ������*/
        {
            debug("__i2c_wait: unfinished\n");
            return -1;
        }

        if (write == I2C_WRITE_BIT && (csr & I2C_SR_RXAK)) /*дʱ���ж�ack*/
        {
            debug("__i2c_wait: No RXACK\n");
            return -1;
        }

        return 0;
    } while (timeout++ < CONFIG_I2C_TIMEOUT);

    debug("__i2c_wait: timed out\n");
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

    while (readb(dev->I2CSR) & I2C_SR_MBB)
    {
//      writeb(dev->cr, I2C_CR_MEN | I2C_CR_MSTA | I2C_CR_MTX);
//      readb(dev->dr);
        Djy_DelayUs(1);
        writeb(dev->I2CCR, I2C_CR_MEN);
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
// ���أ��������������ֽڵ�λ
// =============================================================================
static int __iic_write_addr (volatile tagI2CReg *dev,
                            u8 devaddr, u8 dir, int rsta)
{
    writeb(dev->I2CCR,
          I2C_CR_MEN | I2C_CR_MSTA | I2C_CR_MTX | (rsta ? I2C_CR_RSTA : 0)
           );/*ʹ��iic��start��mtxģʽ*/

    writeb(dev->I2CDR, (devaddr << 1) | dir);/*д��ַ*/

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
        writeb(dev->I2CDR, data[i]);

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
static void _IIC_IntEnable(volatile tagI2CReg *reg)
{
    SETBIT(reg->I2CCR, I2C_CR_MIEN);
}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void _IIC_IntDisable(volatile tagI2CReg *reg)
{
    CLRBIT(reg->I2CCR, I2C_CR_MIEN);
}

// =============================================================================
// ���ܣ���ʼ��ʱ������,ppc1020�ṩ��������i2c����ʱ������CCB/2���پ���i2cʱ�ӷ�Ƶ��
//        ����I2CFDR�Ĵ�������Ƶϵ�������ض��ļ��㷽������APIͨ��ö�ٲ���ȷ�����ֿ�
//        ���õĲ�����.
// ������reg���Ĵ���ָ��
//      i2cclk��ö������I2C_CLOCK_RATE_50K��ʱ��Ϊ50K
// ���أ�true=�ɹ���false=ʧ��
// =============================================================================
static void __iic_set_clk(volatile tagI2CReg *reg,u8 i2cclk)
{
    //I2C bus clk devider
    switch(i2cclk)
    {
    case I2C_CLOCK_RATE_50K:
        reg->I2CFDR = 0x33;    //clk=CCB/2/4096=400M/2/4096 = 48.8K
        break;
    case I2C_CLOCK_RATE_100K:
        reg->I2CFDR = 0x2F;    //clk=CCB/2/2048=400M/2/2048 = 97.6K
        break;
    case I2C_CLOCK_RATE_200K:
        reg->I2CFDR = 0x07;    //clk=CCB/2/1024=400M/2/1024 = 195.3K
        break;
    case I2C_CLOCK_RATE_400K:
        reg->I2CFDR = 0x26;    //clk=CCB/2/512 = 400M/2/512 = 390.6K
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
    GPIO_CfgPinFunc(GPIO_3,cn_gpio_mode_out_ad);
    GPIO_SettoLow(GPIO_3);//����FRAMд��������

    volatile tagI2CReg *reg;
    reg = (tagI2CReg *)RegBaseAddr;
    reg->I2CFDR = 0x2F;//default 100KHz
    SETBIT(reg->I2CCR,I2C_CR_MEN);
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
    //set the iic active high
    Int_SetLineTrigerType(IntLine,EN_INT_TRIGER_HIGHLEVEL);
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
    __iic_wait4bus(reg);
}

// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      dev_addr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//      mem_addr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
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
    else if(specific_flag == CN_IIC_REGISTER_BADDR1)
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
    reg = (tagI2CReg *)specific_flag;
    _IIC_IntDisable(reg);

    //�ȷ���������ַ���ڲ���ַ��Ȼ���͵�һ������
    if (__iic_wait4bus(reg) >= 0)
    {
        if(__iic_write_addr(reg, dev_addr, I2C_WRITE_BIT, 0) != 0)
        {
            //���������ڲ���ַ
            if(maddr_len - 1 == __iic_write(reg,mem_addr_buf,maddr_len-1))
            {
                reg->I2CDR = mem_addr_buf[maddr_len-1];
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
static bool_t __IIC_GenerateReadStart( ptu32_t  specific_flag,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct tagSemaphoreLCB *iic_semp)
{
    volatile tagI2CReg *reg;
    u8 mem_addr_buf[4];

    if(specific_flag == CN_IIC_REGISTER_BADDR0)
    {
        IntParamset0.TransTotalLen = length;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;               //iic_buf_semp
    }
    else if(specific_flag == CN_IIC_REGISTER_BADDR1)
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
    reg = (tagI2CReg *)specific_flag;
    _IIC_IntDisable(reg);

    //д������ַ��Ѱַ��ַ
    if (__iic_wait4bus(reg) >= 0)
    {
        if(__iic_write_addr(reg, dev_addr, I2C_WRITE_BIT, 0) != 0)
        {
            if(__iic_write(reg, mem_addr_buf, maddr_len) == maddr_len)
            {
                if(length && __iic_write_addr(reg,dev_addr, I2C_READ_BIT,1) != 0)
                {
                    writeb(reg->I2CCR,I2C_CR_MEN |
                        I2C_CR_MSTA | ((length == 1) ? I2C_CR_TXAK : 0));
                    //������������ǳ���Ҫ�����û�л���ɶ�����
                    readb(reg->I2CSR);
                    readb(reg->I2CDR);

                    _IIC_IntEnable(reg);
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
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateEnd(ptu32_t specific_flag)
{
    volatile tagI2CReg *reg;

    reg = (tagI2CReg *)specific_flag;

    if(CHKBIT(reg->I2CCR,I2C_CR_MTX))//����
    {
        __IIC_GenerateStop(reg);
    }
    else //����
    {
        SETBIT(reg->I2CCR, I2C_CR_TXAK);//ֹͣ�ظ�ACK
        __IIC_GenerateStop(reg);
    }
}

// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ�1
// =============================================================================
static s32 __IIC_BusCtrl(ptu32_t spceific_flag,u32 cmd,u32 data1,u32 data2)
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

    reg = (tagI2CReg*)CN_IIC_REGISTER_BADDR0;

    if(CHKBIT(reg->I2CSR, I2C_SR_MIF))//iic1�ж�
    {
        ICB = &s_IIC0_CB;
        IntParam = &IntParamset0;
    }
    else
    {
        reg = (tagI2CReg*)CN_IIC_REGISTER_BADDR1;
        if(CHKBIT(reg->I2CSR, I2C_SR_MIF))//iic2�ж�
        {
            ICB = &s_IIC1_CB;
            IntParam = &IntParamset1;
        }
        else
            return 0;
    }

    CLRBIT(reg->I2CSR, I2C_SR_MIF);            //���жϱ�־λ

    if(CHKBIT(reg->I2CCR, I2C_CR_MSTA))        //�Ѿ���������
    {
        if(CHKBIT(reg->I2CCR,I2C_CR_MTX))      //�����ж�
        {
            if(!CHKBIT(reg->I2CSR, I2C_SR_RXAK))
            {
                //�ӷ��ͻ�������һ���ֽڵ����ݣ�������
                if(IIC_PortRead(ICB,&ch,1) > 0)
                {
                    reg->I2CDR = ch;
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
                                        (CHKBIT(reg->I2CSR, I2C_SR_MCF)))
            {
                // ���һ���ֽ�master����ACK����ʾ��������ֹ
                if(IntParam->TransCount == IntParam->TransTotalLen - 1)
                {
                    SETBIT(reg->I2CCR, I2C_CR_TXAK);
                }
                ch = reg->I2CDR;
                //д����
                IIC_PortWrite(ICB,&ch,1);
                IntParam->TransCount ++;
            }
            if((IntParam->TransCount == IntParam->TransTotalLen)  &&
                                        (CHKBIT(reg->I2CSR, I2C_SR_MCF)))
            {
                __IIC_GenerateStop(reg);
                Lock_SempPost(IntParam->pDrvPostSemp);//�ͷ������ź���
            }
        }
    }
    else//δ����ͨ��
    {
        if(CHKBIT(reg->I2CSR,I2C_SR_MAL))//�ٲö�ʧ�ж�
        {
            CLRBIT(reg->I2CSR,I2C_SR_MAL);
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
    IIC0_Config.SpecificFlag        = CN_IIC_REGISTER_BADDR0;
    IIC0_Config.pGenerateWriteStart = __IIC_GenerateWriteStart;
    IIC0_Config.pGenerateReadStart  = __IIC_GenerateReadStart;
    IIC0_Config.pGenerateEnd        = __IIC_GenerateEnd;
    IIC0_Config.pBusCtrl            = __IIC_BusCtrl;

    __IIC_HardDefaultSet(CN_IIC_REGISTER_BADDR0);
    __IIC_IntConfig(cn_int_line_iic_controllers,__IIC_ISR);

    if(NULL == IIC_BusAdd_r(&IIC0_Config,&s_IIC0_CB))
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
    IIC1_Config.IICBufLen           = IIC0_BUF_LEN;
    IIC1_Config.SpecificFlag        = CN_IIC_REGISTER_BADDR1;
    IIC1_Config.pGenerateWriteStart = __IIC_GenerateWriteStart;
    IIC1_Config.pGenerateReadStart  = __IIC_GenerateReadStart;
    IIC1_Config.pGenerateEnd        = __IIC_GenerateEnd;
    IIC1_Config.pBusCtrl            = __IIC_BusCtrl;

    __IIC_HardDefaultSet(CN_IIC_REGISTER_BADDR0);
    __IIC_IntConfig(cn_int_line_iic_controllers,__IIC_ISR);

    if(NULL == IIC_BusAdd_r(&IIC1_Config,&s_IIC1_CB))
        return 0;
    return 1;
}






