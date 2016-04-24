//----------------------------------------------------
//Copyright (C), 2013-2020,  lst.
//��Ȩ���� (C), 2013-2020,   lst.
//����ģ��:
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:��flash��FPGA��ȡ���IP�Ͱ��,��ȡFPGA�汾�ż�ʱ�䣬
//         ��ȡB��Уʱ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-09-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdlib.h"
#include "sunri_types.h"
#include "fpga_info.h"
#include "easynorflash\easynorflash.h"
#include "fpga_address.h"
#include "string.h"
#include "chips.h"
#include "cpld.h"

//----�����װ���еı��----------------------------------------------------
//���ܣ���FPGA�ж�ȡ�����װ���еı��
//����: ��
//����: ���
//--------------------------------------------------------------------------
/*UINT8 Hard_BoardNo_Read( void )
{
    UINT8   byBoard;
//    byBoard = (UINT8)((*(UINT32*)CN_HARD_HARD_BASE) & 0x00FF);

    byBoard = 10;
    return(byBoard);
}
*/

//----��ȡFCC0��IP��ַ-------------------------------------------------------
//���ܣ���flash�ж�ȡ��ȡFCC0��IP��ַ
//����: ��
//����: ���
//--------------------------------------------------------------------------
void GetFXIP(UINT8*  pbybuf,UINT8 *fcc0_ip)
{
//    extern u8 fcc0_ip[4];
    if((fcc0_ip[4] > 0) && (fcc0_ip[4] < 0xFF))
    {
        memcpy(pbybuf,fcc0_ip,CN_FLASH_IP_LEN);
    }
    else
        memset(pbybuf,0xFF,CN_FLASH_IP_LEN);
}

//----��ȡ��ǰmac/ipֵ-------------------------------------------------------
//���ܣ���FPGA/flash�ж�ȡ�����װ���еı��
//����: ��
//����: ���
//--------------------------------------------------------------------------
void GetNetFXIP( UINT8  *pbybuf,UINT8 *fcc0_ip )
{
    UINT8 byBoardNo = Hard_BoardNo_Read();

    if(0==byBoardNo)
        byBoardNo = 204;

    GetFXIP(pbybuf,fcc0_ip);
    if(0xff==pbybuf[0])
    {
        pbybuf[0] = 192;
        pbybuf[1] = 168;
        pbybuf[2] = 253;
        pbybuf[3] = byBoardNo;
    }
}

//------��ȡʱ��------------------------------------------------------------
//���ܣ���ȡB��Уʱ���յ�ʱ/��/�룬��FPGA
//��������
//���أ�����ʱ��
//--------------------------------------------------------------------------
UINT32 FpgaTmr_TrigB_GetTime(void)
{
    return (*(volatile UINT32*)(CN_FPGA_TIMER_DATA_L));
}
//-------��ȡУ׼ʱ��-------------------------------------------------------
//���ܣ���ȡB��Уʱ���յ������������FPGA
//��������
//���أ� �����������
//--------------------------------------------------------------------------
UINT32 FpgaTmr_TrigB_GetDate(void)
{
    return (*(volatile UINT32*)(CN_FPGA_TIMER_DATA_H));
}

//-------B��1��ȡ����Уʱ��־-----------------------------------------------
// ���ܣ�B��1��ȡ����Уʱ��־
// ��������
//����ֵ��  TRUE:PPS����ɹ�
//             FALSE��PPSδ����
//--------------------------------------------------------------------------
BOOLEAN FpgaTmr_Tmr_GetPPSFlg(void)
{
    register UINT32     *pTicksFlg;                     // �����־��ʱ����
    register UINT8      iGetFlg;

    pTicksFlg = (UINT32 *)(CN_FPGA_TIMER_FLAG);         // ��ȡ�����־
    iGetFlg   = (UINT8)(*pTicksFlg & 0x000000ff);
    if( iGetFlg !=0 )
    {
        return TRUE;                                    // PPS����ɹ�
    }
    else
    {
        return FALSE;                                   // δ����
    }
}

// -----B���������Уʱ��־-----------------------------------------------
//���ܣ�B���������Уʱ��־
//��������
//���أ���
// -----------------------------------------------------------------------
void FpgaTmr_Tmr_ClrFlag(void)
{
    (*(volatile UINT32*)(CN_FPGA_TIMER_CLR_FLAG)) = 0;      // ������Уʱ��־
}


