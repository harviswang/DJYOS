//----------------------------------------------------
//Copyright (C), 2013-2020,  lst.
//��Ȩ���� (C), 2013-2020,   lst.
//����ģ��:�弶��������SMAC����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-09-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "cpu_peri.h"
#include "string.h"
/******************* SMAC	��ַ����***********************/
//0x00_0000						�˿�0���ճ���
//0x00_0004						�˿�0���ͳ���
//0x01_0000						�˿�1���ճ���
//0x01_0004						�˿�1���ͳ���
//0x02_0000						�˿�2���ճ���
//0x02_0004						�˿�2���ͳ���
//0x03_0000						�˿�3���ճ���
//0x03_0004						�˿�3���ͳ���
//0x04_0000						�˿�4���ճ���
//0x04_0004						�˿�4���ͳ���
//0x05_0000						�˿�5���ճ���
//0x05_0004						�˿�5���ͳ���
//0x00_1000 ~ 0x00_1FFF			�˿�0���ջ���(4k)
//0x00_2000 ~ 0x00_2FFF			�˿�0���ͻ���(4k)
//0x01_1000 ~ 0x01_1FFF			�˿�1���ջ���(4k)
//0x01_2000 ~ 0x01_2FFF			�˿�1���ͻ���(4k)
//0x02_1000 ~ 0x02_1FFF			�˿�2���ջ���(4k)
//0x02_2000 ~ 0x02_2FFF			�˿�2���ͻ���(4k)
//0x03_1000 ~ 0x03_1FFF			�˿�3���ջ���(4k)
//0x03_2000 ~ 0x03_2FFF			�˿�3���ͻ���(4k)
//0x04_1000 ~ 0x04_1FFF			�˿�4���ջ���(4k)
//0x04_2000 ~ 0x04_2FFF			�˿�4���ͻ���(4k)
//0x05_1000 ~ 0x05_1FFF			�˿�5���ջ���(4k)
//0x05_2000 ~ 0x05_2FFF			�˿�5���ͻ���(4k)
#define SMAC_BASE_ADDR          (0xA0000000)

#define PORT_RXLEN(x)   (SMAC_BASE_ADDR + 0x010000 * x + 0x000000) //(RO) �˿�x���ճ���
#define PORT_TXLEN(x)   (SMAC_BASE_ADDR + 0x010000 * x + 0x000004) //(WO) �˿�x���ͳ���
#define PORT_RXBUF(x)   (SMAC_BASE_ADDR + 0x010000 * x + 0x001000) //(RO) �˿�x���ջ���(4k)
#define PORT_TXBUF(x)   (SMAC_BASE_ADDR + 0x010000 * x + 0x002000) //(WO) �˿�x���ͻ���(4k)

#define PORT_MAX_NUM    5               //��port������
#define DMA_PACK_LEN    128             //DMAÿ�ΰ������ݴ�С

#define SMAC_BUF_LENTH   0x1000         //��������С���ֽ���
uint8_t smac_recvbuf[SMAC_BUF_LENTH];   //���ջ�������С
uint8_t smac_sendbuf[SMAC_BUF_LENTH];   //���ͻ�������С

//-------SMAC�������ݺ���----------------------------------------------------
//���ܣ�SMAC�������ݣ��ú�����Ӧ�ò���ã�FPGA���յ����������ݶ���Ӧ�ò�ͨ��
//      ��RXLEN�ĸ�16λ���ж��Ƿ��������ݣ����ݳ���(bytes)= 4*��16bit�����
//      �����ݣ����ȡ֮�����򷵻�0����ȡ���ݣ�ͨ��DMA���˵ķ�ʽ
//������port:���͵Ķ˿ںţ�����0~5�ܹ������˿ں�
//      Buf:�������ݵĻ�����ָ�룬ʵ�ʵĻ������ɱ���������
//���أ����������ȡ���ݣ����ض�ȡ���ݴ�С�����򷵻�0
//--------------------------------------------------------------------------
uint32_t smac_recv(uint8_t port, uint8_t *buf)
{
    uint32_t src_addr;
    uint32_t recvlen = 0,readtimes = 0;
    uint8_t i;

    if(port > PORT_MAX_NUM)
        return 0;

    recvlen = (uint32_t)(*(uint32_t *)(PORT_RXLEN(port)));  //��ȡ�Ƿ��н��յ�����
    recvlen = (u16)((recvlen>>16) & 0xFFFF);                //��16bit�ǽ��յ����ݳ���
                                                            //���ݳ���(bytes)= 4*��16bit
    recvlen = recvlen *4;
    src_addr = PORT_RXBUF(port);
    if((recvlen>0) &&(recvlen < SMAC_BUF_LENTH)) //�ж��Ƿ������ݣ������ݴ�С������������С
    {
        readtimes = recvlen/DMA_PACK_LEN + (recvlen%DMA_PACK_LEN ? 1:0);
        for(i = 0; i < readtimes; i++)                      //�ֶ�ΰ��ˣ��ݶ�
        {
            dmacpy((unsigned long)&smac_recvbuf[i*DMA_PACK_LEN],
                    src_addr + i*DMA_PACK_LEN, DMA_PACK_LEN);
        }
        buf = smac_recvbuf;                                 //���ص�ַ��ֵ
    }
    else
    {
        recvlen = 0;
        buf = NULL;
    }

    return recvlen;
}

//-------SMAC�������ݺ���----------------------------------------------------
//���ܣ�SMAC�������ݣ��ú�����Ӧ�ò���ã������ݰ����͵�FPGA,���ĳ��Ȳ�����
//      2048�ֽڣ��Ƚ����͵����ݷֶ��ͨ��DMA���ˣ�Ȼ�����ݳ���д����Ӧ�Ĵ���
//������port:���͵Ķ˿ںţ�����0~5�ܹ������˿ں�
//      pbyBuf:�������ݵ�ָ��
//      len:���͵����ݳ��ȣ��ֽ�Ϊ��λ
//���أ���ȷ���ͷ���len�����򷵻�0
//--------------------------------------------------------------------------
uint32_t smac_send(uint8_t port, uint8_t *pbyBuf, uint32_t len)
{
    uint32_t dst_addr,*writelen;
    uint32_t sendlen = 0,sendtimes = 0;
    uint8_t i;
    int status;

    if((len > SMAC_BUF_LENTH) || (port > PORT_MAX_NUM))
        return 0;

    dst_addr = PORT_TXBUF(port);
    if(len)
    {
        memcpy(smac_sendbuf, pbyBuf, len);
        sendtimes = len/DMA_PACK_LEN + (len%DMA_PACK_LEN ?1:0);
        for(i = 0; i < sendtimes; i++)                  //DMA�ֶ�ΰ��ˣ��ݶ�
        {
            status = dmacpy(dst_addr + i*DMA_PACK_LEN,
                        (unsigned long)&smac_sendbuf[i*DMA_PACK_LEN],DMA_PACK_LEN);
            if(status == -1)
            {
                sendlen = 0;
                len = 0;
                break;
            }
            else
                sendlen += DMA_PACK_LEN;
        }
        writelen = (uint32_t *)(PORT_TXLEN(port));
        *writelen = (sendlen << 16) + len;            //������д�����ȵ�ַ��
    }

    return len;
}



