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
#include "sunri_types.h"
#include "Ft3drv.h"
#include "fpga_address.h"

#define FT3_DATA_BASE        (CN_FPGA_BASE_ADDR + 0x50400)
#define FT3_RXPLEN_BASE        (CN_FPGA_BASE_ADDR + 0x57400)
#define FT3_TXSTA            (CN_FPGA_BASE_ADDR + 0x57800)
#define FT3_TXDATA            (CN_FPGA_BASE_ADDR + 0x58000)
#define FT3_TXCTL            (CN_FPGA_BASE_ADDR + 0x58800)
#define FT3_RXKEEP            (CN_FPGA_BASE_ADDR + 0x58804)
#define FT3_RSTCTL            (CN_FPGA_BASE_ADDR + 0x58808)
#define FT3_REGLOCK            (CN_FPGA_BASE_ADDR + 0x5880C)
#define REGLOCK_OFF_VALUE        (0xAAAA6666)
#define REGLOCK_ON_VALUE        (0x6666AAAA)

tagFT3RecMsg   g_tFT3RecMsg[CN_FT3_CHIP_MAX];

// ============================================================================
// �������ܣ�FT3���ƽ�����ʱ����ֹ���������󡣽�����ʱ��СΪFPGA����һ֡����ʱ��
// ���������ʱ�䣬��λΪus
// �����������
// ����ֵ��  ��
// ============================================================================
void FT3_Delaytime(UINT32 dwMicrosecond)
{
    register UINT32     dwDelay;                    // ʵ����ʱticket��

    if( dwMicrosecond>=200 )
    {
        dwMicrosecond -= CN_FT3_WAIT_IDLE*2/10;     // ����λ�ȴ���ʱ(һ��BIT��ʱ0.2΢��)
    }
    dwDelay = CN_FT3_CRYSTALL_FRQ*dwMicrosecond;    // FT3Ӳ������Ƶ��
    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_OFF_VALUE;    //д�Ĵ���֮ǰ���Ƚ���
    (*(volatile UINT32*)(FT3_RXKEEP)) = dwDelay;
    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_ON_VALUE;    //д�Ĵ���������
}

// ============================================================================
// �������ܣ�FT3��λ����������FT3��ͳһ��λ
//           �����ڳ�ʼ�����շ��쳣ʱ����
// �����������
// �����������
// ����ֵ��  ��
// ============================================================================
void FT3_Init(void)
{
    register UINT32     dwLoop1;                    // ��ʱ����
    tagFT3RecMsg       *ptFT3RecMsg;                // �������ݳ�

    // ��0��λ
    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_OFF_VALUE;    //д�Ĵ���֮ǰ���Ƚ���
    (*(volatile UINT32*)(FT3_RSTCTL)) = 0;
    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_ON_VALUE;    //д�Ĵ���������

    // �������ݳ�
    for (dwLoop1=0; dwLoop1<CN_FT3_CHIP_MAX; dwLoop1++)
    {
        ptFT3RecMsg = &g_tFT3RecMsg[dwLoop1];       // ��ÿ��ͨ�������ݳؽ��г�ʼ��
        ptFT3RecMsg->dwMsgNum = 0;                  // ���ĸ���
        ptFT3RecMsg->dwMsgPtr = 0;                  // ����ָ��
    }

    // ��1����
    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_OFF_VALUE;    //д�Ĵ���֮ǰ���Ƚ���
    (*(volatile UINT32*)(FT3_RSTCTL)) = 1;
    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_ON_VALUE;    //д�Ĵ���������

    // ����������ʱ��������(ȫ����)
    FT3_Delaytime( 0 );
}

// ============================================================================
// �������ܣ�FT3���պ���
// ��������������ն˿ڣ�2~13
// ���������
// ============================================================================
BOOL FT3_Rec(UINT8 byChip)
{
    register UINT32     dwLoop1, dwLoop2, dwVal;            // ��ʱ����
    register UINT32     *pdwptr;                            // ���ݳض�ȡ��ַ
    register UINT32     dwMsgNow, dwMsgOld;                 // ֡λ��
    register UINT32     dwMsgNum;                            // ���İ���
    register UINT8      *pbyMsgBuf;                         // ���ı����ַ
    tagFT3RecMsg           *ptFT3RecMsg;                       // �������ݳ�
    tagFT3Packet           *ptFT3Packet;                       // ����������

    // ����FT3��Ŀ�ݴ�
    if( byChip>=CN_FT3_CHIP_MAX )
    {
        return FALSE;
    }

    // ָ���ʼ��
    ptFT3RecMsg = &(g_tFT3RecMsg[byChip]);

    // �ж��Ƿ����±���
    dwMsgNow = (*(UINT32*)(FT3_RXPLEN_BASE+(4*byChip)));    // �õ���ǰָ֡��
    dwMsgOld = ptFT3RecMsg->dwMsgPtr;                        // ��һ�ε�ָ֡��
    ptFT3RecMsg->dwMsgPtr = dwMsgNow;                       // ���浱ǰָ֡��
    if( dwMsgNow == dwMsgOld )
    {
        return FALSE;                                       // û���±���
    }
    else
    {
        if( dwMsgNow > dwMsgOld )
        {
            if( (dwMsgNow-dwMsgOld) <= CN_FT3_MAX_INTERVAL )
            {
                dwMsgNum = dwMsgNow - dwMsgOld;
            }
            else
            {
                return FALSE;                               // ���յ�֡��������̫��
            }
        }
        else                                                // ��ʱ�Ѿ���ת
        {
            if( (dwMsgNow+CN_FT3_MSG_MAX-dwMsgOld) <= CN_FT3_MAX_INTERVAL )
            {
                dwMsgNum = dwMsgNow + CN_FT3_MSG_MAX - dwMsgOld;
            }
            else
            {
                return FALSE;                               // ���յ�֡��������̫��
            }
        }
    }

    // ���ı���
    ptFT3RecMsg->dwMsgNum = dwMsgNum;                       // ֡��
    for(dwLoop1=0; dwLoop1<dwMsgNum; dwLoop1++)
    {
        if( (dwMsgOld+1) >= CN_FT3_MSG_MAX )
        {
            dwMsgOld = 0;                                   // �������߽磬��Ҫ��ת
        }
        else
        {
            dwMsgOld = dwMsgOld + 1;
        }

        // ��RAM��������ݶ�������
        pdwptr      =  (UINT32 *)(FT3_DATA_BASE + (byChip << 10) + (dwMsgOld*4*CN_FT3_LEN_RECV_32BIT));
        ptFT3Packet = &ptFT3RecMsg->tFt3Packet[dwLoop1];
        pbyMsgBuf   = &ptFT3Packet->byPacket[0];
        for (dwLoop2=0; dwLoop2<CN_FT3_LEN_RECV_32BIT; dwLoop2++)
        {
            dwVal        = *pdwptr++;
            *pbyMsgBuf++ = (dwVal>>24)&0xff;
            *pbyMsgBuf++ = (dwVal>>16)&0xff;
            *pbyMsgBuf++ = (dwVal>>8)&0xff;
            *pbyMsgBuf++ = dwVal&0xff;
        }
    }
    return TRUE;
}

// ============================================================================
// �������ܣ�FT3���ͺ���
// ��������������Ͷ˿ڣ�2;�����͵�����;�������ݳ���(һ��д�̶�64�ֽڣ����ȹ̶�Ϊ64������ҪCPU��CRC���ײ���)
// ���������
// ============================================================================
BOOL FT3_Send(UINT8 byChip, UINT8 *pbyBuf, UINT32 dwLen)
{
    register UINT32     dwVal_LL, dwVal_LH;         // ��ʱ����(˫�ֵ�16λ)
    register UINT32     dwVal_HL, dwVal_HH;         // ��ʱ����(˫�ָ�16λ)
    register UINT32     *pdwptr;                    // 32λ���ķ��ͻ�ַ
    register UINT32     dwLoop1;                      // ��ʱ����

    // ���ĳ����ݴ�
    if( dwLen>CN_FT3_LEN_SEND_32BIT*4 )
    {
        return FALSE;
    }

    // ����FT3��Ŀ�ݴ�
    if( byChip>=CN_FT3_CHIP_MAX )
    {
        return FALSE;
    }

    // ��ʵFT3���
    if( byChip>=2 )
    {
        byChip -= 2;                                    // �˿ںŴ�2��ʼ
    }
    else
    {
        byChip = 0;
    }

    // ����ʹ������
    (*(volatile UINT32*)(FT3_TXCTL)) = 0;

    // ����д��FPGA�ڴ�
    pdwptr = (UINT32 *)(FT3_TXDATA);
    for(dwLoop1=0; dwLoop1<dwLen; dwLoop1+=4)       // ��ѭ��64/4=16��
    {
        dwVal_LL  = *pbyBuf++;
        dwVal_LH  = *pbyBuf++;
        dwVal_HL  = *pbyBuf++;
        dwVal_HH  = *pbyBuf++;
        *pdwptr++ = dwVal_LL + (dwVal_LH<<8) + (dwVal_HL<<16) + (dwVal_HH<<24);
    }

    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_OFF_VALUE;    //д�Ĵ���֮ǰ���Ƚ���
    // ����ʹ�ܿ���
    (*(volatile UINT32*)(FT3_TXCTL)) = 1 << byChip;
    (*(volatile UINT32*)(FT3_REGLOCK)) = REGLOCK_ON_VALUE;    //д�Ĵ���������
    return TRUE;
}
