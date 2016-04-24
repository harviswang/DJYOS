/* Copyright 2007-2010 ShenZhen Nari, Inc.*/
/*modification history
struct for app...

Writen By zhangji, 2007,10,19
*/
//****************************************************************************************
// LVDSͨѶ����
//****************************************************************************************

#ifndef _LVDSIODRV_H
#define _LVDSIODRV_H

#ifdef __cplusplus
extern "C" {
#endif
#include "sunri_types.h"
typedef struct
{
    UINT32          bySourseAddr;
    UINT32         byBoardType;
    UINT32          byCtlByte[12];
    UINT32        byLockByte;
    UINT32        byValidByte;
} tagLvdsioCtlFrm;  //����֡��ʽ
typedef struct
{
    UINT32       bySourseAddr;
    UINT32        byBoardType;
    UINT32       byStaByte[14];
} tagLvdsioStaFrm;    //״̬֡��ʽ
/********************************************************************/
typedef struct
{
    UINT16        blist;//����б������ٻ�����ʱʹ��
    UINT8       tx_en;//����ʹ�ܣ�CPU�˿���,1 ʹ�ܣ�or 0
    UINT8       rx_en;//��ȡʹ�ܣ�CPU�˿���,1 ʹ�ܣ�or 0
    UINT8        poll_en;//�ٻ���ѯʹ�ܣ�����ʹ�ã��Ӱ岻ʹ��,1 ʹ�ܣ�or 0
} tagLvdsEnable;  //LVDS ��������ģ�����

typedef struct
{
    UINT8        desboard;   //Ŀ������ַ
    UINT8        transmode;  //����ģʽ
    UINT16        datalength; //���ݳ��ȣ���λ byte
    UINT8        ready;      //����׼����=1 ��or 0
} tagLvdsTrans;//LVDS�������ߣ��������ò���

extern void LvdsFPGAReset();
extern void LvdsFPGAResetNot();
extern void LvdsFPGAInit(UINT8 byBnum,UINT8 byms,UINT16 wMulticastnum);
extern void LvdsFPGApoll(tagLvdsEnable *ptpoll);
extern void LvdsFPGAtrans(tagLvdsTrans *pttrans);
extern UINT32 LvdsFPGARead(UINT32 dwaddroffset);
extern UINT32 LvdsFPGATransR();
extern UINT32 LvdsFPGAfifoSR();
extern void LvdsFPGADataW(UINT32* dwdata,UINT32 wlength);
extern UINT32 LvdsFPGADataR(UINT32* dwdata);
extern BOOL LvdsFPGAWriteReady();
extern BOOL LvdsFPGAReadReady();

/********************************************************************/


extern  void         Lvdsio_InitReg();
extern  void         Lvdsio_StartBoard(UINT8 byBoardNum,UINT8 byBoardType, UINT8 * pbyCtlByte, BOOL bLock );
extern  void         Lvdsio_UnlockCtl(UINT8 byBoardNum,BOOL bLock );
//extern  UINT8 *     Lvdsio_GetCtlBuff(UINT8 byBoardNum);
extern  UINT8 *     Lvdsio_GetStaBuff(UINT8 byBoardNum);
extern  void        Lvdsio_SetStaMsg(UINT8 byBoardNum, UINT8 * pbyKiByte);
extern  BOOL         Lvdsio_GetRevSta(UINT8 byBoardNum);
extern  BOOL         Lvdsio_GetBoardSta(UINT8 byBoardNum);
extern  BOOL         Lvdsio_GetCtlReg(UINT8 byBoardNum);
extern  void        Lvdsio_SetSlaveBrdNum(UINT8 byBoardNum);
extern  void        Lvdsio_SetSlaveStaMsg(UINT8 * pbyStaByte);
extern  void        Lvdsio_SetSlaveCtlMsg(UINT8 * pbyCtlByte);
extern     void         Lvdsio_SetCtlReg(UINT8 byBoardNum,UINT8 byCtlReg);

#ifdef __cplusplus
}
#endif

#endif /* _LVDSIODRV_H */

