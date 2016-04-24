//----------------------------------------------------
//Copyright (C), 2013-2020,  lst.
//��Ȩ���� (C), 2013-2020,   lst.
//����ģ��:
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:��flash��FPGA��ȡ���IP��FPGA���ڡ��汾����Ϣ
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-09-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _BOARDIP_FPGA_FLASH__H
#define _BOARDIP_FPGA_FLASH__H

#define CN_FLASH_MAC_LEN                0x06
#define CN_FLASH_IP_LEN                    0x04
#include "sunri_types.h"
//��ȡFPGA�汾��Ϣ�İ汾ʱ��,�ĵ�cpld
/*struct FpgaVer
{
    UINT32 dwBoardTypeLo;           //FPGA�汾��Ϣ��
    UINT32 dwBoardTypeHi;           //FPGA�汾��Ϣ��
    UINT32 dwFpgaDay;               //FPGA�汾ʱ����
    UINT32 dwFpgaMonth;             //FPGA�汾ʱ����
    UINT32 dwFpgaYear;                //FPGA�汾ʱ����
};*/


// д��flashֵ
extern         void        WriteFcc0Mac  ( UINT8  *pbybuf );//��������δʵ��
extern         void        WriteFcc0IP   ( UINT8  *pbybuf );
extern         void        WriteFcc1Mac  ( UINT8  *pbybuf );//��������δʵ��
extern         void        WriteFcc1IP   ( UINT8  *pbybuf );
extern         void        WriteFXMac    ( UINT8  *pbybuf );
extern         void        WriteFXIP     ( UINT8  *pbybuf );

// ��ȡflash�б���ֵ
extern         void        GetFcc0Mac    ( UINT8  *pbybuf );
extern         void        GetFcc0IP     ( UINT8  *pbybuf );
extern         void        GetFcc1Mac    ( UINT8  *pbybuf );
extern         void        GetFcc1IP     ( UINT8  *pbybuf );
extern         void        GetFXMac      ( UINT8  *pbybuf );
extern         void        GetFXIP       ( UINT8  *pbybuf ,UINT8 *fcc0_ip);

// ��ȡ��ǰIP��MAC
extern         void        GetNetFcc0Mac ( UINT8  *pbybuf );   //for pub
extern         void        GetNetFcc0IP  ( UINT8  *pbybuf );   //for pub
extern         void        GetNetFcc1Mac ( UINT8  *pbybuf );   //for pub
extern         void        GetNetFcc1IP  ( UINT8  *pbybuf );   //for pub
extern         void        GetNetFXMac   ( UINT8  *pbybuf );   //for 827
extern         void        GetNetFXIP    ( UINT8  *pbybuf ,UINT8 *fcc0_ip);   //for 827 is used


//void FpgaVer_GetInfo(struct FpgaVer *tgfpgaver);
void FpgaTmr_Tmr_ClrFlag(void);
BOOLEAN FpgaTmr_Tmr_GetPPSFlg(void);
UINT32 FpgaTmr_TrigB_GetDate(void);
UINT32 FpgaTmr_TrigB_GetTime(void);

void GetNetFXIP( UINT8  *pbybuf ,UINT8 *fcc0_ip);
//UINT8 Hard_BoardNo_Read( void );

#endif
