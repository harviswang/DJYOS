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
//����ģ��: uart��������(�����ӿڲ���)
//����:  ������.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2013-11-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ring.h"
#include "systime.h"
#include "lock.h"
#include "djyos.h"
#include "multiplex.h"
#include "uart.h"

//����״̬���ƽṹ
 struct tagUartCB
{
    struct tagRingBuf SendRingBuf;              //���η��ͻ�����.
    struct tagRingBuf RecvRingBuf;              //���ν��ջ�����.

    struct tagSemaphoreLCB *SendRingBufSemp;    //���ͻ������ź���
    struct tagSemaphoreLCB *RecvRingBufSemp;    //���ջ������ź���
    struct tagSemaphoreLCB *BlockingSemp;       //�����ź���

    u32 RecvRingTrigLevel;      //����Ring���մ���ˮƽ,���յ���������ʱ�ͷ��ź���
    u32 SendRingTrigLevel;      //����Ring���ʹ���ˮƽ�������ͻ��λ�����������
                                //��SendRingBufSemp������ǰ�̣߳�ֱ�����ͻ���
                                //������������SendRingTrigLevelʱ�ͷ�֮��
    u32 MplReadTrigLevel;       //��·���ÿɶ�����ˮƽ
    u32 MplWriteTrigLevel;      //��·���ÿ�д����ˮƽ
    u32 Baud;                   //���ڵ�ǰ������
    ptu32_t UartPortTag;        //���ڱ�ǩ
    struct tagMultiplexObjectCB * pMultiplexUart;      //��·����Ŀ�����ͷָ��
    u32 MplUartStatus;          //���ڵĵ�ǰ״̬����ɶ�����д��
    UartStartSend StartSend;
    UartDirectSend DirectlySend;
    UartControl UartCtrl;
};

//----�����豸APPд����----------------------------------------------------------
//����: дUART�豸,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_WriteDevice�����ջ���ñ��������������ù���:
//      Driver_WriteDevice() ---> Dev->Write() ---> UART_AppWrite()
//      �ú���ʵ�ֵĹ���˵������:
//      1.��δ�����ȣ������ֱ�ӷ��ͷ�ʽ��ֱ�ӷ���ǰ���Ƚ����������ݷ������.
//      2.����ʼ���ȣ�������д�봮��uart_UCB�Ļ��η��ͻ�������.
//      3.д�����ݺ������������ݣ��������͵ķ�ʽ��Ϊ�жϻ���ѯ������������.
//      4.����������������ȴ������������ݽ��͵�����ˮƽ���ټ���д������.
//      5.�й�timeout��ÿ�εȴ��ź�����ʱ�䶼��ݼ�.
//      6.����������д�뻺�������ж��Ƿ��������ͣ���Ϊ��������ȴ������ź����ٷ���.
//      7.����ǰ�����ô������ڵĶ�·���ü���д��Ӧλ���
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ����ֵ
//      buf������Դָ��.
//      len��������(bytes).
//      offset,ƫ�������ڴ���ģ���У��˱�����Ч
//      block_option,����ѡ�����Ϊ������ʽ.��block_optionΪtrue��������ȴ�����
//         ���������ݷ�����ɺ󷵻أ������������ͷ�ʽ����block_optionΪfalseʱ������
//         ��ȴ��������ݱ����봮��ģ��Ļ������к����̷��أ����������ͷ�ʽ�ں���
//         ����ʱ�����ڿ������ڷ������ݡ�
//      timeout,��ʱ��(us).
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
ptu32_t UART_AppWrite(struct tagUartCB *UCB,u8* src_buf,u32 len,
                    u32 offset,bool_t block_option,u32 timeout)
{
    u32 completed = 0,written;
    uint8_t *buf,ch;
    u32 base_time,rel_timeout = timeout;
    u32 ring_datalen,i;

    if((len==0) || ((u8*)src_buf == NULL) || (UCB == NULL))
        return 0;
    buf = (uint8_t*)src_buf;
    base_time = (u32)DjyGetTime();

    if(Djy_QuerySch() == false)//������Ȳ�δ��ʼ,����ֱ�ӷ��ͷ�ʽ
    {
        if(Ring_Check(&UCB->SendRingBuf))//��������������ݣ����ȷ�����
        {
            ring_datalen = Ring_Read(&UCB->SendRingBuf,&ch,1);
            for(i = 0; i < ring_datalen; i++)
               UCB->DirectlySend(UCB->UartPortTag,&ch,1,rel_timeout);

        }
        return UCB->DirectlySend(UCB->UartPortTag,buf,len,rel_timeout);
    }
    while(1)
    {
        written = Ring_Write(&UCB->SendRingBuf,
                            (uint8_t*)buf+completed,len-completed);
        UCB->StartSend(UCB->UartPortTag,rel_timeout);

        if(written != len-completed)     //����������û���ͳ�ȫ������
        {
            completed += written;
            if(false == Lock_SempPend(UCB->SendRingBufSemp,rel_timeout))
                break;
            rel_timeout = (u32)DjyGetTime() - base_time;
            if(rel_timeout > timeout)
                break;
            else
                rel_timeout = timeout - ((u32)DjyGetTime() - base_time);
        }else
        {
            if(block_option == true)
            {
                //��PENDһ���ź�������ֹ�����Ѿ����ͷŹ�
                Lock_SempPend(UCB->BlockingSemp,0);
                if(Ring_Check(&UCB->SendRingBuf))
                {
                    Lock_SempPend(UCB->BlockingSemp,rel_timeout);
                }
            }
            completed += written;
            break;
        }
    }
    if(Ring_IsFull(&UCB->SendRingBuf))
    {
        UCB->MplUartStatus &= (~CN_MULTIPLEX_SENSINGBIT_WRITE);
        Multiplex_Set(UCB->pMultiplexUart,UCB->MplUartStatus);
    }
    return completed;
}

//----�����豸APP������----------------------------------------------------------
//����: ��UART�豸,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������Driver_ReadDevice,
//      ʵ�ʾ��ǵ����˸ú������������ù���:
//      Driver_ReadDevice() ---> Dev->Read() ---> UART_AppRead()
//      �ú���ʵ�ֵĹ���˵������:
//      1.���������������ݣ����������������㹻����ֱ�ӷ���
//      2.�����ݲ��������Ի���������һ��Ϊ��λ�������ν�������
//      3.�ڵȴ�����ǰ�����Ƚ��ź��������ÿ�εȴ�ʱ������ܳ�ʱʱ��۳�
//      4.����ǰ�����ô������ڵĶ�·���ü��Ķ���Ӧλ���
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ����ֵ
//      dst_buf,Ӧ�ó����ṩ�Ľ��ջ�����
//      len,���볤��,�ֽ�
//      offset,ƫ����,�ڴ���ģ���У��˱�����Ч
//      timeout,����ʱ����,΢��
//����: ʵ�ʶ�������
//------------------------------------------------------------------------------
ptu32_t UART_AppRead(struct tagUartCB *UCB,u8* dst_buf,u32 len,
                    u32 offset,u32 timeout)
{
    uint32_t completed = 0;
    uint32_t result,triglevel;
    uint8_t  index = 0,indexlen = 0;
    u32 base_time,rel_timeout=timeout;

    if((len==0) || ((u8*)dst_buf == NULL) || (UCB == NULL))
        return 0;

    base_time = (u32)DjyGetTime();
    result = Ring_Read(&UCB->RecvRingBuf,(uint8_t*)dst_buf,len);
    if(result >= len)
    {
        completed = result;
    }
    else    //�����������ݲ�������ȴ�����
    {
        completed = result;
        len = len - completed;
        triglevel = Ring_Capacity(&UCB->RecvRingBuf)/2;
        //��ν��գ����Ϊ�뻺���ݻ�
        indexlen = (len/triglevel) + (len%triglevel ?1:0);
        for(index = 0; index < indexlen; index++)
        {
            if(index == indexlen - 1)
                UCB->RecvRingTrigLevel = len - triglevel * index;
            else
                UCB->RecvRingTrigLevel= triglevel;
            if(Lock_SempQueryFree(UCB->RecvRingBufSemp) >= 1)//������е��ź�
            {
               Lock_SempPend(UCB->RecvRingBufSemp,0);
            }
            Lock_SempPend(UCB->RecvRingBufSemp,rel_timeout);
            completed += Ring_Read(&UCB->RecvRingBuf,
                               ((u8*)dst_buf) + result + index * triglevel,
                               UCB->RecvRingTrigLevel);
            //ÿ��pend��ʱ��Ҫ�ݼ�
            rel_timeout = (u32)DjyGetTime() - base_time;
            if(rel_timeout > timeout)
                break;
            else
                rel_timeout = timeout - ((u32)DjyGetTime() - base_time);
        }
    }
    if(Ring_Check(&UCB->RecvRingBuf) == 0)
    {
        UCB->MplUartStatus &= (~CN_MULTIPLEX_SENSINGBIT_READ);
        Multiplex_Set(UCB->pMultiplexUart,UCB->MplUartStatus);
    }

    return completed;
}

//----�����豸�˿�д����-------------------------------------------------------
//����: �Ӷ˿�дUART,�ɵײ���������,�ú���ʵ�ֵĹ���˵������:
//      1.������д�봮��uart_UCB�Ļ��ν��ջ�������.
//      2.���д���豸������δ�ﵽ����ˮƽ��������ռ�����
//      3.�ﵽ����ˮƽ�󣬷����ź�����֪ͨ�ϲ�ȡȡ��
//      4.���������������pop�������̣߳��������¼���Ӧ�ó���ע��
//      5.��������ʱ����λ��·���ü������Ӧλ���ﵽ������ˮƽʱ����λ��·���ü���
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      buf������Դָ�롣
//      len��������(bytes)
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
ptu32_t UART_PortWrite(struct tagUartCB *UCB,u8* buf,u32 len,u32 res)
{
    uint16_t recv_bytes;
    uint32_t check;

    if(NULL == UCB)
        return 0;

    recv_bytes = Ring_Write(&UCB->RecvRingBuf, (u8*)buf,len);
    if(recv_bytes != len)
    {
        UCB->MplUartStatus |= CN_UART_BUF_OVER_ERR;
        Multiplex_Set(UCB->pMultiplexUart,UCB->MplUartStatus);
    }
    check = Ring_Check(&UCB->RecvRingBuf);
    if((check >= UCB->RecvRingTrigLevel) &&
                (check - recv_bytes < UCB->RecvRingTrigLevel))
    {
        Lock_SempPost(UCB->RecvRingBufSemp);
    }
    if((check >= UCB->MplReadTrigLevel) &&
                (check - recv_bytes < UCB->MplReadTrigLevel))
    {
        UCB->MplUartStatus |= CN_MULTIPLEX_SENSINGBIT_READ;
        Multiplex_Set(UCB->pMultiplexUart,UCB->MplUartStatus);
    }
    return (ptu32_t)recv_bytes;
}

//----�����豸�˿ڶ�����---------------------------------------------------------
//����: �Ӷ˿ڶ�ȡUART�豸,�ɵײ���������,�ú���ʵ�ֵĹ���˵������:
//      1.��uart_UCB�������ж�ȡ���ݵ��ײ������ṩ��dst_buf.
//      2.��鱾�δӻ������ж�������ǰ���Ƿ������ʹ���ˮƽ��������˷��ʹ���ˮƽ��
//        �����ź�����֪ͨӦ�ò㣬���Լ���д���ݵ�������.
//      3.�ﵽд����ˮƽʱ����λ��·���ü�����־�о�λ
//      4.������������ʱ����������������Ϊ��ʱ���ͷ������ź���.
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
//------------------------------------------------------------------------------
ptu32_t UART_PortRead(struct tagUartCB *UCB,u8* dst_buf,u32 len,u32 res)
{
    uint32_t check,result=0;

    if(NULL == UCB)
        return result;

    result = (ptu32_t)Ring_Read(&UCB->SendRingBuf,(u8 *)dst_buf,len);
    check = Ring_Check(&UCB->SendRingBuf);
    if((check <= UCB->SendRingTrigLevel)
            && (check + result > UCB->SendRingTrigLevel))
    {
        Lock_SempPost(UCB->SendRingBufSemp);
    }
    if((check <= UCB->MplWriteTrigLevel)
            && (check + result > UCB->MplWriteTrigLevel))
    {
        UCB->MplUartStatus |= CN_MULTIPLEX_SENSINGBIT_WRITE;
        Multiplex_Set(UCB->pMultiplexUart,UCB->MplUartStatus);
    }
    if(result == 0)
    {
        Lock_SempPost(UCB->BlockingSemp);
    }

    return result;
}

//----���ڴ�������----------------------------------------------------------
//����: ������������������ʱ���øú��������ö�·���ü���Ӧ�Ĵ����ʶλ
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      ErrNo,�����ʶ�ı���λ���ñ���λ�����Ƕ�·����ģ��δ�õ��о�λ����3-30���أ�
//����: 0������1����ȷ
//------------------------------------------------------------------------------
ptu32_t UART_ErrHandle(struct tagUartCB *UCB,u32 ErrNo)
{
    u32 Status,result = 0;
    if(NULL != UCB)
    {
        Status = UCB->pMultiplexUart->PendingBit | ErrNo;
        Multiplex_Set(UCB->pMultiplexUart,Status);
        result = 1;
    }
    return result;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_CtrlDevice��ʵ�ʾ��ǵ����˸ú������������ù���:
//      Driver_CtrlDevice() ---> Dev->dCtrl() ---> UART_Ctrl()
//      �ú���ʵ�ֵĹ��ܸ��������ַ�������˵������:
//      1.����ֹͣ���ڣ��ɵײ�����ʵ��
//      2.���ð�˫�����ͻ����״̬���ɵײ�����ʵ��
//      3.��ͣ�ͻָ��������ݣ��ɵײ�����ʵ��
//      4.���ò����ʺ�Ӳ������ˮƽ���ɵײ�����ʵ��
//      5.���ô��󵯳��¼�����
//      ......
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
ptu32_t UART_Ctrl(struct tagUartCB *UCB,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    ptu32_t result = 0;

    if(UCB == NULL)
        return 0;
    switch(cmd)
    {
        case CN_UART_START:
        case CN_UART_STOP:
        case CN_UART_COM_SET:
        case CN_UART_SEND_DATA:     //��Ϊ��˫������״̬
        //��IO���ư�˫��ͨ�ŵķ���ʹ�ܵĻ�,�ڴ�ת���շ�.
        case CN_UART_RECV_DATA:     //��Ϊ��˫������״̬
        //��IO���ư�˫��ͨ�ŵķ���ʹ�ܵĻ�,�ڴ�ת���շ�.
        case CN_UART_RX_PAUSE:      //��ͣ����
        case CN_UART_RX_RESUME:     //�ָ�����
        case CN_UART_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
        case CN_UART_SEND_HARD_LEVEL:    //���÷���fifo����ˮƽ
        case CN_UART_EN_RTS:             //ʹ��rts����
        case CN_UART_DIS_RTS:            //��ֹrts����
        case CN_UART_EN_CTS:             //ʹ��cts����
        case CN_UART_DIS_CTS:            //��ֹcts����
        case CN_UART_DMA_USED:           //ʹ��dma����
        case CN_UART_DMA_UNUSED:         //��ֹdma����
        case CN_UART_HALF_DUPLEX_SEND:
        case CN_UART_HALF_DUPLEX_RECV:
        case CN_UART_SETTO_ALL_DUPLEX:
             UCB->UartCtrl(UCB->UartPortTag,cmd,data1,data2);
             break;
        case CN_UART_SET_BAUD:          //����Baud
            if(UCB->Baud !=data1)
            {
                UCB->Baud = data1;
                UCB->UartCtrl(UCB->UartPortTag,CN_UART_SET_BAUD,data1,data2);
            }
            break;
        case CN_UART_RECV_SOFT_LEVEL:   //���ý����������ˮƽ
            UCB->RecvRingTrigLevel = data1;
            break;
        case CN_UART_SEND_SOFT_LEVEL:   //���÷����������ˮƽ
            UCB->SendRingTrigLevel = data1;
            break;
        case CN_DRV_CTRL_SET_LEVEL:
            if((u32*)data1 != NULL)
            {
                if(*(u32*)data1 != 0)
                {
                    result = Ring_Capacity(&UCB->RecvRingBuf);
                    result = (result > *(u32*)data1)?(*(u32*)data1):result;
                    UCB->MplReadTrigLevel = result;
                }
                else
                {
                    *(u32*)data1 = UCB->MplReadTrigLevel;
                }
            }
            if((u32*)data2 != NULL)
            {
                if(*(u32*)data2 != 0)
                {
                    result = Ring_Capacity(&UCB->SendRingBuf);
                    result = (result > *(u32*)data2)?(*(u32*)data2):result;
                    UCB->MplWriteTrigLevel = result;
                }
                else
                {
                    *(u32*)data2 = UCB->MplWriteTrigLevel;
                }
            }
            break;
        default: break;
    }
    return result;
}

//----UART��·������Ӻ���------------------------------------------------------
//���ܣ���UART��ӵ���·���ü���tagUartCB���ƿ��г�ԱpMultiplexCB�����øú���ʱ��
//      δָ���κ�tagMultiplexSetsCB������踳ֵΪ0
//������PrivateTag,����ģ���˽�б�ǩ���˴�ΪUCB���ƿ�
//      MultiplexSets,��·���ü�
//      DevAlias,��Multiplex�Ĵ��ڵ��豸����
//      SensingBit,��������λ��־��31��bit����Ϊ1��ʾ����������bit��־����
//              bit31��ʾ�������ͣ�CN_SENSINGBIT_AND������CN_SENSINGBIT_OR
//���أ�true,��ӳɹ���false,���ʧ��
//-----------------------------------------------------------------------------
bool_t UART_MultiplexAdd(struct tagUartCB *UCB,
                         struct tagMultiplexSetsCB *MultiplexSets,
                         u32 DevAlias,
                         u32 SensingBit)
{
    bool_t result=false;

    if(UCB == NULL)
        return result;

    result = Multiplex_AddObject(MultiplexSets,
                            &UCB->pMultiplexUart,
                            UCB->MplUartStatus,
                            (ptu32_t)DevAlias,SensingBit);
    return result;
}

//----���UART�豸-------------------------------------------------------------
//���ܣ���Ӵ����豸��ϵͳ�豸����
//      a����ʼ�����λ�����
//      b���������λ������ź������豸������
//      c����������ӵ��豸����ģ�飬����ʼ���豸����ָ��
//������UCB�����ڿ��ƿ�
//      Param,������ʼ��UART������������������鿴tagUartParam�ṹ��
//���أ����ڿ��ƿ�ָ�룬NULLʧ��
//-----------------------------------------------------------------------------
struct tagUartCB * UART_InstallPort(struct tagUartParam *Param)
{
    tagDevHandle uart_dev;
    struct tagUartCB *UCB;
    struct tagMutexLCB *uart_mutexR,*uart_mutexT;
    u8 *pRxRingBuf,*pTxRingBuf;

    if(Param == NULL)
        return NULL;

    UCB = (struct tagUartCB *)M_Malloc(sizeof(struct tagUartCB),0);
    if(UCB == NULL)
        goto exit_from_ucb;
    pRxRingBuf = (u8*)M_Malloc(Param->RxRingBufLen,0);
    if(pRxRingBuf == NULL)
        goto exit_from_rx_ring_buf;
    pTxRingBuf = (u8*)M_Malloc(Param->TxRingBufLen,0);
    if(pTxRingBuf == NULL)
        goto exit_from_tx_ring_buf;

    //������������ʹ�õ��ź���
    UCB->BlockingSemp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"uart blocking");
    if(UCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    UCB->SendRingBufSemp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"uart tx buf");
    if(UCB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;
    UCB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"uart rx buf");
    if(UCB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;
    //Ϊ�豸�������������ṩ�豸�Ļ������
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;
    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;

    UCB->SendRingTrigLevel  = (Param->TxRingBufLen)>>2;  //Ĭ�ϻ��崥��ˮƽΪ1/16
    UCB->RecvRingTrigLevel  = (Param->RxRingBufLen)>>4;
    UCB->MplReadTrigLevel   = (Param->TxRingBufLen)>>4;
    UCB->MplWriteTrigLevel  = (Param->RxRingBufLen)>>2;
    UCB->Baud               = Param->Baud;
    UCB->UartPortTag        = Param->UartPortTag;
    UCB->DirectlySend       = Param->DirectlySend;
    UCB->StartSend          = Param->StartSend;
    UCB->UartCtrl           = Param->UartCtrl;
    UCB->pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
    UCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�
    Ring_Init(&UCB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&UCB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    //���UART�豸
    uart_dev = Driver_DeviceCreate(NULL,(char*)Param->Name,uart_mutexR,uart_mutexT,
                               (devWriteFunc) UART_AppWrite,
                               (devReadFunc ) UART_AppRead,
                               (devCtrlFunc ) UART_Ctrl,
                               (devMultiplexAddFunc)UART_MultiplexAdd,
                               (ptu32_t)UCB
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;

    printk("%s Install Succeeded!\r\n",Param->Name);
    return UCB;
    //������ִ������ͷŴ�������Դ�������ؿ�ָ��
exit_from_add_device:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UCB->RecvRingBufSemp);
exit_from_recv_buf_semp:
    Lock_SempDelete(UCB->SendRingBufSemp);
exit_from_send_buf_semp:
    Lock_SempDelete(UCB->BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(UCB);
exit_from_ucb:
    return NULL;
}

