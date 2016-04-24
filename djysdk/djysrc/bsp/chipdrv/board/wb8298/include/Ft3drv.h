/*================================================================================
 * Ԫ�����ƣ�WB800ƽ̨�Ķ���FT3ʵ��
 * Ԫ������������FT3���ա����ʹ���
             ����FPGA�ڴ��������ƣ�ע����շ����Ĵ�С������
 * Ԫ���汾: V1.00
 * ������Ա: ����
 * ����ʱ��:
 * �汾�޶�:
 * �޶���Ա:
 *================================================================================*/

#ifndef _WB828_FT3_H_
#define _WB828_FT3_H_

#define CN_FT3_CHIP_MAX             (20)                    // FT3����оƬ����
#define CN_FT3_MSG_MAX              (15)                    // ��ʵ����RAM�����������Է�15��֡
#define CN_FT3_LEN_RECV_32BIT       (17)                    // FT3�������32λ���ĳ��� 64byte(����)+4byte(ʱ��)
#define CN_FT3_LEN_SEND_32BIT       (16)                    // FT3���ڷ��ͻ��������32λ���ݳ���
#define CN_FT3_MAX_INTERVAL            (7)                        // һ���ܹ����յ��������Ŀ
#define CN_FT3_CRYSTALL_FRQ         (100)                   // FT3Ӳ������Ƶ��(��λ����)
#define CN_FT3_WAIT_IDLE            (200)                   // FT3���տ��еȴ�BIT��Ŀ

// ����������
typedef struct
{
    UINT8           byPacket[CN_FT3_LEN_RECV_32BIT*4];      // ��������
}tagFT3Packet;

// �������ݳ�
typedef struct
{
    tagFT3Packet    tFt3Packet[CN_FT3_MSG_MAX];             // ����������
    UINT32            dwMsgNum;                               // ֡��
    UINT32            dwMsgPtr;                               // ָ֡��
}tagFT3RecMsg;

/*================================================================================*/
// �ⲿ��������
/*================================================================================*/
extern      tagFT3RecMsg    g_tFT3RecMsg[CN_FT3_CHIP_MAX];
extern        void            FT3_Init(void);
extern        BOOL            FT3_Rec(UINT8 byChip);
extern        BOOL            FT3_Send(UINT8 byChip, UINT8 *pbyBuf, UINT32 dwLen);
extern        void            FT3_Delaytime(UINT32 dwMicrosecond);

/*================================================================================*/
#endif /* _WB828_FT3_H_ */





