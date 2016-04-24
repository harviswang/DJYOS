/*******************************************************************************/
/*                                                                             */
/*   ���¶ȴ����������ı���VDSP ezboard post�����е�adm1032����                */
/*                                                                             */
/*    FILE:     adm1032.c                                                      */
/*                                                                             */
/*    PURPOSE:  This file tests the ADM1032 Temperature Monitor interface      */
/*               on the EZ-Board.                                              */
/*                                                                             */
/*******************************************************************************/

/* includes */
#include <cdef21469.h>
#include <def21469.h>

#include "stdint.h"
#include "cpu_peri.h"
#include "adm1032.h"
#include "iic.h"

#include <sru.h>
#include <sysreg.h>


struct tagMutexLCB *  ptSemID_adm1032;       // IIC���������־

static djy_handle_t pg_adm1032_lhdl;
static struct tagIIC_Param pg_adm1032_param;

static u32 timeout = 10000;


// ------������--------------------------------------------------------
// ���ܣ�������ָ����ַ��ʼDstlen���ȵ�����
// ������Adm1032_DstAddr��Ŀ��洢��Ԥ��ȡ���ݵĵ�ַ
//         pDstBuf����Ž��յ������ݻ����ַ
//         DstLen�� Ԥ��ȡ���ݵĳ���
// ���أ�ret, ʵ�ʶ������ֽ���
// -------------------------------------------------------------------
u32 ADM1032_Read_Data(u32 Adm1032_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    u32 ret = 0;

    pg_adm1032_param.memory_addr = Adm1032_DstAddr;
    pg_adm1032_param.data_len    = 0;

    //���ʹ洢����ַ
    Driver_DevWriteLeft(pg_adm1032_lhdl,(ptu32_t)pDstBuf,
                (ptu32_t)&pg_adm1032_param,timeout);

    //������
    pg_adm1032_param.data_len    = DstLen;
    ret = Driver_DevReadLeft(pg_adm1032_lhdl,(ptu32_t)pDstBuf,
                    (ptu32_t)&pg_adm1032_param,timeout);
    return ret;
}
// -------д����------------------------------------------------------
// ���ܣ�д����ָ����ַ��ʼDstlen���ȵ�����
// ������Adm1032_DstAddr��Ŀ��洢��Ԥд���ݵ�ַ
//         pSrcBuf���������ݻ����ַ
//         SrcLen�� �������ݳ���
// ���أ�ret, ʵ��д����ֽ���
// -------------------------------------------------------------------
u32 ADM1032_Write_Data(u32 Adm1032_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    u32 ret = 0;


    pg_adm1032_param.memory_addr = Adm1032_DstAddr;
    pg_adm1032_param.data_len    = SrcLen;

    ret = Driver_DevWriteLeft(pg_adm1032_lhdl,(ptu32_t)pSrcBuf,
                        (ptu32_t)&pg_adm1032_param,timeout);

    return ret;
}


//----��ʼ��ADM1032оƬ----------------------------------------------------
//���ܣ���ʼ��ADM1032оƬ
//����: ��
//����: true= �ɹ���false=ʧ��
//--------------------------------------------------------------------------
bool_t module_init_adm1032(ptu32_t para)
{
    ptSemID_adm1032 = Lock_MutexCreate(NULL);// adm1032���������־

    pg_adm1032_param.device_addr  = TEMP_SENSOR_ADDRESS;  //ADM1032�������ַ
    pg_adm1032_param.memory_addr  = 0;
    pg_adm1032_param.mem_addr_len = 1;             //ADM1032�ĵ�ַ�ռ�Ϊ���ֽ�
    pg_adm1032_param.data_len     = 0;

    IIC0_Init();
    pg_adm1032_lhdl = Driver_DevOpenLeft("iic1",0);
    if(pg_adm1032_lhdl != NULL)
    {
        //���ô����ٶ�Ϊ100KHz
       Driver_DevCtrlLeft(pg_adm1032_lhdl,cn_iic_set_clk,I2C_CLOCK_RATE_100K,0);
       return true;
    }
    else
        return false;
}
