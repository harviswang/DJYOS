// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: FRAMģ�����������������������������Ա���
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 20/06.2014
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "endian.h"
#include "fram_fm24cl64b.h"
#include "iicbus.h"
#include "endian.h"
#include "fram_fm24cl64b.h"

#define FRAM0_Address                 (0x50)
#define FRAM1_Address                 (0x54)//�����豸�ĵ�ַ,���������ַ��
#define FRAM_MAX_ADDR                 (0x20000) //�����С���ֽڵ�λ

struct tagMutexLCB *  ptSemID_Fram0;       // IIC���������־
struct tagMutexLCB *  ptSemID_Fram1;

static struct tagIIC_Device pg_FRAM0_Dev;
static struct tagIIC_Device pg_FRAM1_Dev;

u32 timeout = CN_TIMEOUT_FOREVER;//2000*mS;//;100*mS

// =============================================================================
// ���ܣ�������ָ����ַ��ʼDstlen���ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ��ȡ���ݵĵ�ַ
//       pDstBuf����Ž��յ������ݻ����ַ
//       DstLen�� Ԥ��ȡ���ݵĳ���
// ���أ�ret, ʵ�ʶ������ֽ���
// =============================================================================
u32 FRAM_Read_Data(u8 fram_no,u32 Fram_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    u32 ret = 0,TempLen;

    if(Fram_DstAddr + DstLen> FRAM_MAX_ADDR)
        return 0;
    if(fram_no == 1)
    {
        GPIO_CfgPinFunc(GPIO_3,cn_gpio_mode_out_ad);
        GPIO_SettoLow(GPIO_3);//����FRAMд��������

        //�������������Խ����page
        if((Fram_DstAddr < 0x10000) && (Fram_DstAddr + DstLen > 0x10000))
        {
            TempLen = 0x10000 - Fram_DstAddr - 1;
            IIC_Read(&pg_FRAM0_Dev, Fram_DstAddr, pDstBuf, TempLen,timeout);

            IIC_Read(&pg_FRAM0_Dev, 0x10000, pDstBuf+TempLen, DstLen-TempLen,
                    timeout);
        }
        else
        {
            IIC_Read(&pg_FRAM0_Dev, Fram_DstAddr, pDstBuf, DstLen, timeout);
        }
    }
    else if(fram_no == 2)
    {
        GPIO_CfgPinFunc(GPIO_4,cn_gpio_mode_out_ad);
        GPIO_SettoLow(GPIO_4);//����FRAMд��������

        //�������������Խ����page
        if((Fram_DstAddr < 0x10000) && (Fram_DstAddr + DstLen > 0x10000))
        {
            TempLen = 0x10000 - Fram_DstAddr - 1;
            IIC_Read(&pg_FRAM1_Dev, Fram_DstAddr, pDstBuf, TempLen,timeout);

            IIC_Read(&pg_FRAM1_Dev, 0x10000, pDstBuf+TempLen, DstLen-TempLen,
                    timeout);
        }
        else
        {
            IIC_Read(&pg_FRAM1_Dev, Fram_DstAddr, pDstBuf, DstLen, timeout);
        }
    }
    else
        ret = 0;
    return ret;
}
// =============================================================================
// ���ܣ�д����ָ����ַ��ʼDstlen���ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//      Fram_DstAddr��Ŀ��洢��Ԥд���ݵ�ַ
//      pSrcBuf���������ݻ����ַ
//      SrcLen�� �������ݳ���
// ���أ�ret, ʵ��д����ֽ���
// =============================================================================
u32 FRAM_Write_Data(u8 fram_no, u32 Fram_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    u32 ret = 0,TempLen;

    if(Fram_DstAddr + SrcLen > FRAM_MAX_ADDR)
        return 0;
    if(fram_no == CN_FRAM0_NO)
    {
        //����д����������
        GPIO_CfgPinFunc(GPIO_3,cn_gpio_mode_out_ad);
        GPIO_SettoLow(GPIO_3);//����FRAMд��������

        //�������������Խ����page
        if((Fram_DstAddr < 0x10000) && (Fram_DstAddr + SrcLen > 0x10000))
        {
            TempLen = 0x10000 - Fram_DstAddr - 1;
            IIC_Write(&pg_FRAM0_Dev, Fram_DstAddr, pSrcBuf, TempLen,1,timeout);

            IIC_Write(&pg_FRAM0_Dev, 0x10000, pSrcBuf+TempLen, SrcLen-TempLen,
                    0,timeout);
        }
        else
        {
            IIC_Write(&pg_FRAM0_Dev, Fram_DstAddr, pSrcBuf, SrcLen,0, timeout);
        }
    }
    else if(fram_no == CN_FRAM1_NO)
    {
        GPIO_CfgPinFunc(GPIO_4,cn_gpio_mode_out_ad);
        GPIO_SettoLow(GPIO_4);//����FRAMд��������

        //�������������Խ����page
        if((Fram_DstAddr < 0x10000) && (Fram_DstAddr + SrcLen > 0x10000))
        {
            TempLen = 0x10000 - Fram_DstAddr - 1;
            IIC_Write(&pg_FRAM0_Dev, Fram_DstAddr, pSrcBuf, TempLen,1,timeout);

            IIC_Write(&pg_FRAM0_Dev, 0x10000, pSrcBuf+TempLen, SrcLen-TempLen,
                    0,timeout);
        }
        else
        {
            IIC_Write(&pg_FRAM0_Dev, Fram_DstAddr, pSrcBuf, SrcLen,0, timeout);
        }
    }
    else
        ret = 0;

    return ret;
}
// =============================================================================
// ���ܣ�д�ֵ����硣������ָ����ַ˫�ֳ��ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ����ַ
// ���أ�u32data,���ض���������
// =============================================================================
u32 FRAM_Read_DWord(u8 fram_no,u32 Fram_DstAddr)
{
    u32 u32data,buf[4];
    u32  rLen = 4;
    u32 ret = 0;

    ret = FRAM_Read_Data(fram_no,Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //��С��ת��
    u32data = pick_little_32bit((u8 *)&buf,0);
    return u32data;
}
// =============================================================================
// ���ܣ�д�ֵ����磬д˫�����ݵ�ָ����ַ��FRAM
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥд��ַ
// ���أ�ret, ���������룬�޴���ΪERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_DWord(u8 fram_no,u32 Fram_DstAddr,u32 u32data)
{
    u32  rLen = 4;
    u8 buf[4];
    u32 ret = 0;

    fill_little_32bit((u8 *)&buf,0,u32data);
    ret = FRAM_Write_Data(fram_no,Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}

// =============================================================================
// ���ܣ���˫�֣�������ָ����ַ˫�ֳ��ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ����ַ
// ���أ�u16data,���ض���������
// =============================================================================
u16 FRAM_Read_Word(u8 fram_no,u32 Fram_DstAddr)
{
    u16 u16data,buf[2];
    u8  rLen = 2;
    u32 ret = 0;

    ret = FRAM_Read_Data(fram_no,Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //��С�˴洢�ж�ȡ�������ݣ�ת��Ϊ���ģʽ
    u16data = pick_little_16bit((u8 *)&buf,0);
    return u16data;
}

// =============================================================================
// ���ܣ�д�ֵ����磬д�����ݵ�ָ����ַ��FRAM
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥд��ַ
//       u16data,д�������
// ���أ�ret, ���������룬�޴���ΪERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_Word(u8 fram_no, u32 Fram_DstAddr,u16 u16data)
{
    u8  rLen = 2,buf[2];
    u32 ret = 0;

    //����˸ĳ�С�� ����С�˷�ʽ�洢��FRAM
    fill_little_16bit((u8 *)&buf,0,u16data);
    ret = FRAM_Write_Data(fram_no,Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}
// =============================================================================
// ���ܣ�������д�ֵ����磬������WORD��ȡ��������
// ��������ȡWORD��ַ
// ���أ���ȡ��WORD����
// =============================================================================
u16 FRAM_Read_Word_r(u8 fram_no, u16 wAddress )
{
    u16    wData = 0;

    if(fram_no == CN_FRAM0_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram0, CN_TIMEOUT_FOREVER))  // ��ȡдȨ��
        {
            printf("FRAM1_Read_Word_r ��ȡдȨ��ʧ��\n");
            return  false;
        }
        wData = FRAM_Read_Word(fram_no, wAddress );   // д����
        Lock_MutexPost( ptSemID_Fram0 );
    }
    else if(fram_no == CN_FRAM1_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram1, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
        {
            printf("FRAM2_Read_Word_r ��ȡдȨ��ʧ��\n");
            return  false;
        }
        wData = FRAM_Read_Word(fram_no, wAddress);   // д����
        Lock_MutexPost( ptSemID_Fram1 );
    }
    else
        ;

    return    wData;
}

// =============================================================================
// ���ܣ��źű��������ݣ������u8��ȡ��������
// ��������ȡWORD��ַ, ���滺���� ��ȡ����
// ���أ� ��ȡ���ݳ���
// =============================================================================
u32 FRAM_Read_Data_r( u8 fram_no,u16 wAddress, u8 *pbyBuf, u32 dwLen )
{
    register u32      dwStatus = 0;

    if(fram_no == CN_FRAM0_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram0, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
        {
            printf("FRAM1_Read_Data ��ȡдȨ��ʧ��\n");
            return  false;
        }
        dwStatus = FRAM_Read_Data(fram_no, wAddress, pbyBuf, dwLen );// д����
        Lock_MutexPost( ptSemID_Fram0 );
    }
    else if(fram_no == CN_FRAM1_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram1, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
        {
            printf("FRAM2_Read_Data ��ȡдȨ��ʧ��\n");
            return  false;
        }
        dwStatus = FRAM_Read_Data(fram_no, wAddress, pbyBuf, dwLen ); // д����
        Lock_MutexPost( ptSemID_Fram1 );
    }
    else
        ;

    return    dwStatus;
}

// =============================================================================
// ���ܣ��źű������֣�������WORDд��������
// ��������ȡWORD��ַ, д������
// ���أ�  д�����ݳ���
// =============================================================================
s32 FRAM_Write_Word_r( u8 fram_no,u16 wAddress, u16 wData )
{
    s32    Status = 0;

    if(fram_no == CN_FRAM0_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram0, CN_TIMEOUT_FOREVER))       // ��ȡдȨ��
        {
            printf("FRAM1_Write_Word_r ��ȡдȨ��ʧ��\n");
            return  false;
        }
        Status = FRAM_Write_Word(fram_no, wAddress, wData);   // д����
        Lock_MutexPost( ptSemID_Fram0 );
    }
    else if(fram_no == CN_FRAM1_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram1, CN_TIMEOUT_FOREVER))       // ��ȡдȨ��
        {
            printf("FRAM2_Write_Word_r ��ȡдȨ��ʧ��\n");
            return  false;
        }
        Status = FRAM_Write_Word(fram_no, wAddress, wData);   // д����
        Lock_MutexPost( ptSemID_Fram1 );
    }
    else
        ;

    return    Status;
}

// =============================================================================
// ���ܣ��źű���д���ݣ������u8д��������
// ��������ȡWORD��ַ, д������
// ���أ�  д�����ݳ���
// =============================================================================
u32 FRAM_Write_Data_r(u8 fram_no, u16 wAddress, u8 *pbyBuf, u32 dwLen)
{
    register u32      dwStatus = 0;

    if(fram_no == CN_FRAM0_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram0, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
        {
            printf("FRAM1_Write_Data_r ��ȡдȨ��ʧ��\n");
            return  false;
        }
        dwStatus = FRAM_Write_Data(fram_no, wAddress, pbyBuf, dwLen );// д����
        Lock_MutexPost( ptSemID_Fram0 );
    }
    else if(fram_no == CN_FRAM1_NO)
    {
        if(-1 == Lock_MutexPend( ptSemID_Fram1, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
        {
            printf("FRAM2_Write_Data_r ��ȡдȨ��ʧ��\n");
            return  false;
        }
        dwStatus = FRAM_Write_Data(fram_no, wAddress, pbyBuf, dwLen );// д����
        Lock_MutexPost( ptSemID_Fram1 );
    }
    else
        ;
    return    dwStatus;
}

// =============================================================================
// ���ܣ���ʼ��framоƬ������IIC������ʼ������
// ����: para,����ţ�0��1
// ����: true= �ɹ���false=ʧ��
// =============================================================================
ptu32_t ModuleInit_Fram(ptu32_t para)
{
    struct tagIIC_Device *Dev;
    char *BusName,*DevName;
    switch(para)
    {
    case 0:
        pg_FRAM0_Dev.DevAddr = FRAM0_Address;
        pg_FRAM0_Dev.BitOfMemAddrInDevAddr = 1;
        pg_FRAM0_Dev.BitOfMemAddr = 17;

        BusName = "IIC0";
        DevName = "FRAM0";
        Dev = &pg_FRAM0_Dev;
        IIC0_Init();

        ptSemID_Fram0 = Lock_MutexCreate("Fram0 Lock");
        break;
    case 1:
        pg_FRAM1_Dev.DevAddr = FRAM1_Address;
        pg_FRAM1_Dev.BitOfMemAddrInDevAddr = 1;
        pg_FRAM1_Dev.BitOfMemAddr = 17;

        BusName = "IIC1";
        DevName = "FRAM1";
        Dev = &pg_FRAM1_Dev;
        IIC1_Init();

        ptSemID_Fram1 = Lock_MutexCreate("Fram1 Lock");
        break;
    default:
        return false;
        break;
    }

    if(NULL == IIC_DevAdd_r(BusName,DevName,Dev))
        return false;
    else
        return true;
}



