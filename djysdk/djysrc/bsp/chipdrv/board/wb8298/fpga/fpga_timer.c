//----------------------------------------------------
//Copyright (C), 2013-2020,  lst.
//��Ȩ���� (C), 2013-2020,   lst.
//����ģ��:���FPGA����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:fpga timer��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-09-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdlib.h"
#include "fpga_timer.h"
#include "sunri_types.h"

#include "fpga_address.h"

const UINT32 * FPGA_32_FPGATIMERBASE = (UINT32 *)FPGA_TIMER_ADDRESS;//FPGA TIMER��ַ����FPGA�ṩ
#define CN_FPGATIMERBASE         FPGA_32_FPGATIMERBASE
#define CN_FPGACINTCNT            (0x00)
#define CN_FPGA10MCNT             (0x02)
#define CN_FPGA100MCNT             (0x03)
#define CN_FPGASECCNT             (0x04)
#define CN_FPGAMILLCNT             (0x05)
#define CN_FPGAMILHCNT             (0x06)
#define CN_FPGACNTLAT            (0x07)
#define CN_FPGAAUTORELOAD         (0x08)
#define CN_FPGACNTADD            (0x0a)

#define CN_FPGACINTSET            (0x10)
#define CN_FPGA10MSET             (0x12)
#define CN_FPGA100MSET             (0x13)
#define CN_FPGASECSET             (0x14)
#define CN_FPGAMILLSET             (0x15)
#define CN_FPGAMILHSET             (0x16)
#define CN_FPGAMANULOAD         (0x17)

#define CN_FPGASAMCMP            (0x18)
#define CN_FPGA10MCMP             (0x1A)
#define CN_FPGA10MCPS             (0x1B)
#define CN_FPGA100MCPS             (0x1C)
#define CN_FPGASECCPS             (0x1D)
#define CN_FPGAREVERR             (0x1E)
#define CN_FPGAMANI             (0x1F)

#define CN_FPGA1588REG            (0x20)
#define CN_FPGA1588STA            (0x24)
#define CN_FPGACINTREG             (0x2A)
#define CN_FPGALVDSERRNUM        (3)

#define CN_FPGA1588MAC            (0x30)

#define CN_FPGACLKFRQ            (25000000)
#define CN_FPGATMRSETVALID         (0xa5)

const UINT32 * FPGA_32_FPGAMSBASE = (UINT32 *) FPGA_MS_ADDRESS;
#define CN_FPGAMSBASE     FPGA_32_FPGAMSBASE

#define CN_FPGAMASTER           (0x02)
#define CN_FPGAMSETVALID        (0xa5)

#define CN_1588_GETFLAG 0xA0010C00      //��ȡ1588���¼���־
#define CN_1588_CLRFLAG 0xA0010C04      //���1588�¼���־
#define CN_1588_MAC1RT0 0xA0011000      //��ȡMAC1�յ���synctime
#define CN_1588_MAC1RT1 0xA0011004      //��ȡMAC1�յ���dreqtime
#define CN_1588_MAC1RT2 0xA0011008      //��ȡMAC1�յ���pdreqtime
#define CN_1588_MAC1RT3 0xA001100C      //��ȡMAC1�յ���pdresptime
#define CN_1588_MAC1ST0 0xA0011010      //��ȡMAC1������synctime
#define CN_1588_MAC1ST1 0xA0011014      //��ȡMAC1������dreqtime
#define CN_1588_MAC1ST2 0xA0011018      //��ȡMAC1������pdreqtime
#define CN_1588_MAC1ST3 0xA001101C      //��ȡMAC1������pdresptime
#define CN_1588_MAC2RT0 0xA0011020      //��ȡMAC2�յ���synctime
#define CN_1588_MAC2RT1 0xA0011024      //��ȡMAC2�յ���dreqtime
#define CN_1588_MAC2RT2 0xA0011028      //��ȡMAC2�յ���pdreqtime
#define CN_1588_MAC2RT3 0xA001102C      //��ȡMAC2�յ���pdresptime
#define CN_1588_MAC2ST0 0xA0011030      //��ȡMAC2������synctime
#define CN_1588_MAC2ST1 0xA0011034      //��ȡMAC2������dreqtime
#define CN_1588_MAC2ST2 0xA0011038      //��ȡMAC2������pdreqtime
#define CN_1588_MAC2ST3 0xA001103C      //��ȡMAC2������pdresptime


//------ȡʱ��Ƶ��--------------------------------------------
//���ܣ�ȡFPGAϵͳʱ��
//��������
//���أ�FPGAϵͳʱ�� UINT32
//-----------------------------------------------------------
UINT32 FpgaTmr_GetFrq(void)
{
    return CN_FPGACLKFRQ;
}

//------��ʼ�������Ĵ���-------------------------------------
//���ܣ���ʼ�������Ĵ�����
//������byCps10m��10ms����������byCps100m��100ms����������
//      byCpss���벹������
//���أ���
//----------------------------------------------------------
void FpgaTmr_SetCps(UINT8 byCps10m,    UINT8 byCps100m, UINT8 byCpss)
{
    UINT32 * pdwptr;
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGA10MCPS;

    * pdwptr ++    = (UINT32) byCps10m;    //10MS ������
    * pdwptr ++    = (UINT32) byCps100m;    //100MS ������
    * pdwptr ++    = (UINT32) byCpss;        //S ������
}

//------��ʼ������/�ȽϼĴ���------------------------------
//���ܣ���ʼ���ȽϼĴ����Ͳ����Ĵ�����
//������dwcintfrq���ж�Ƶ�ʣ�byCps10m��10ms����������
//      byCps100m��100ms����������byCpss���벹������
//���أ���
//----------------------------------------------------------
void FpgaTmr_InitReg(UINT32 dwcintfrq, UINT8 byCps10m,
                            UINT8 byCps100m, UINT8 byCpss)
{
    UINT32 * pdwptr;
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGASAMCMP;

    * pdwptr ++    = (UINT32)((CN_FPGACLKFRQ/dwcintfrq) & 0x0ff);        //�жϼ�����
    * pdwptr ++    = (UINT32)(((CN_FPGACLKFRQ/dwcintfrq) >>8)  & 0x0ff);//�жϼ�����
    * pdwptr ++    = (UINT32)(((dwcintfrq/100) & 0x0ff) - 1);        //10ms�ж���
    * pdwptr ++    = (UINT32) byCps10m;    //10MS ������
    * pdwptr ++    = (UINT32) byCps100m;    //100MS ������
    * pdwptr ++    = (UINT32) byCpss;        //S ������
}

//------���ñȽϼĴ���--------------------------------------
//���ܣ�����FPGASAMCMP�Ĵ���
//������wSamCmp�����жϼ���ֵ
//���أ���
//----------------------------------------------------------
void FpgaTmr_SetSamCmp(UINT16 wSamCmp)
{
    UINT32 * pdwptr;
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGASAMCMP;
    * pdwptr ++    = (UINT32)(wSamCmp & 0x0ff);        //�жϼ�����
    * pdwptr ++    = (UINT32)((wSamCmp >>8) & 0x0ff);//�жϼ�����
}

//------�ֶ�����Уʱ��ֵ------------------------------------
//���ܣ�����ʱ��Ԥ��Ĵ������ֶ����Уʱ��ֵ
//������wSamCmp�����жϼ���ֵ��ֵ
//���أ���
//----------------------------------------------------------
void FpgaTmr_ManuTimeAdd(SINT16 nSamCmp)
{
    UINT32 * pdwptr;
    UINT16 i;

    i = abs(nSamCmp);
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGACNTADD;
    * pdwptr ++    = (UINT32)(i & 0x0ff);        //�жϼ�����
    * pdwptr ++    = (UINT32)((i >>8)  & 0x0ff); //�жϼ�����

    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGAMANULOAD;
    if (nSamCmp>0)
        * pdwptr ++    = 0xAC;        //װ��
    else
        * pdwptr ++    = 0xAD;        //װ��
}

//------ȡFPGASAMCMP�Ĵ���----------------------------------
//���ܣ�ȡFPGASAMCMP�Ĵ���
//��������
//���أ����жϼ���ֵ
//----------------------------------------------------------
UINT16 FpgaTmr_GetSamCmp(void)
{
    UINT32 dwSamCmpl,dwSamCmph;

    dwSamCmpl = (UINT32)((*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGASAMCMP))&0xff);
    dwSamCmph = (UINT32)((*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGASAMCMP + 1))&0xff);

    return ((UINT16)(dwSamCmpl + (dwSamCmph << 8)));
}

//------���ȽϼĴ���----------------------------------------
//���ܣ�ȡ�ȽϼĴ����Ͳ����Ĵ��������ڴ�˳�����ָ����ַ��
//      ��6��BYTE
//������pbyGetPtr ָ��
//���أ���
//----------------------------------------------------------
void FpgaTmr_GetCmpReg(UINT8 * pbyGetPtr)
{
    UINT32 i;
    UINT32 * pdwptr;
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGASAMCMP;

    for( i = 0; i < 6 ; i++)
        * pbyGetPtr ++ = (UINT8) (* pdwptr ++);
}

//------����Уʱ����----------------------------------------
//���ܣ�����Уʱ��������ΪУʱ��������LVDS���߷�ʱ��
//������TRUE ��Ϊ����
//���أ���
//----------------------------------------------------------
void FpgaTmr_SetMain(BOOL bSetMain)
{
    if (TRUE == bSetMain)
        *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAMANI) = CN_FPGATMRSETVALID;
    else
        *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAMANI) = 0;
}

//------ȡУʱ����״̬--------------------------------------
//���ܣ�ȡУʱ����״̬
//��������
//���أ�true: �ɹ�
//      false: ʧ��
//----------------------------------------------------------
BOOL FpgaTmr_GetMainSta(void)
{
    if (CN_FPGATMRSETVALID == (UINT8)*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAMANI))
        return TRUE;
    else
        return FALSE;
}

//------�����Զ�װ��-----------------------------------------
//���ܣ�����Уʱ�ӻ��Զ�װ�أ�LVDS�Զ�ˢ��Ԥ��Ĵ������Զ�װ
//      �������������Զ�Уʱ
//������TRUE ��Ϊ�Զ�װ��
//���أ���
//----------------------------------------------------------
void FpgaTmr_SetAutoLoad(BOOL bSetAutoLoad)
{
    if (TRUE == bSetAutoLoad)
        *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAAUTORELOAD) = CN_FPGATMRSETVALID;
    else
        *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAAUTORELOAD) = 0;
}

//------ȡ�Զ�װ��״̬--------------------------------------
//���ܣ�ȡ�Զ�װ��״̬
//��������
//���أ�
//----------------------------------------------------------
BOOL FpgaTmr_GetAutoReloadSta(void)
{
    if (CN_FPGATMRSETVALID == (UINT8)*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAAUTORELOAD))
        return TRUE;
    else
        return FALSE;
}

//------ȡУʱ����״̬--------------------------------------
//���ܣ�ȡУʱ����״̬�����쳣����������CN_FPGAREVERR������FALSE
//��������
//���أ�TRUE: �ɹ�
//      FALSE: ʧ��
//----------------------------------------------------------
BOOL FpgaTmr_GetLvdsSta(void)
{
    if (*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAREVERR) <= CN_FPGALVDSERRNUM )
        return TRUE;
    else
        return FALSE;
}

//------�ֶ�װ�������-------------------------------------
//���ܣ��������жϼ�����Ԥ��Ĵ������ֶ�װ���������
//������wSetCnt�������ƣ�:�жϣ�bySet10ms�������ƣ���10���룻
//      bySet100ms��BCD�룩����/ʰ��
//���أ���
//----------------------------------------------------------
void FpgaTmr_ManuLoad(UINT16 wSetCnt,UINT8 bySet10ms, UINT8 bySet100ms)
{
    UINT32 * pdwptr;
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGACINTSET;

    * pdwptr ++    = (UINT32)((wSetCnt) & 0x0ff);
    * pdwptr ++    = (UINT32)((wSetCnt >>8)  & 0x0ff);
    * pdwptr ++    = (UINT32) bySet10ms;    //10ms �ж���
    * pdwptr ++    = (UINT32) bySet100ms;    //10 ms 100ms BCD

    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGAMANULOAD;
    * pdwptr ++    = CN_FPGATMRSETVALID;        //װ��
}
//------�ֶ����Уʱ---------------------------------------
//���ܣ�����ʱ��Ԥ��Ĵ������ֶ����Уʱ
//������bySetSec��BCD�룩����  wSetMilSec��BCD�룩: ����
//���أ���
//----------------------------------------------------------
void FpgaTmr_ManuTimeLoad(UINT8 bySetSec, UINT8 bySetMilSecL, UINT8 bySetMilSecH)
{
    UINT32 * pdwptr;
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGASECSET;

    * pdwptr ++    = (UINT32) bySetSec;    //��
    * pdwptr ++    = (UINT32) bySetMilSecL;
    * pdwptr ++    = (UINT32) bySetMilSecH;

    * pdwptr ++    = 0xA3;        //װ��
}
//------ȡԤ��Ĵ���----------------------------------------
//���ܣ�ȡԤ��Ĵ�������ԭʼ�ڴ�˳�����ָ����ַ����7��BYTE
//������pbyGetPtr ָ��
//���أ���
//----------------------------------------------------------
void FpgaTmr_GetSetReg(UINT8 * pbyGetPtr)
{
    UINT32 i;
    UINT32 * pdwptr;
    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGACINTSET;

    for( i = 0; i < 7 ; i++)
        * pbyGetPtr ++ =  (UINT8) (* pdwptr ++);
}

//------ȡ�����Ĵ���----------------------------------------
//���ܣ�ȡ�����Ĵ�������ԭʼ�ڴ�˳�����ָ����ַ����7��BYTE
//������pbyGetPtr ָ��
//���أ���
// ��ʱ��1.8-2.5us
//----------------------------------------------------------
void FpgaTmr_GetCntReg(UINT8 * pbyGetPtr)
{
    UINT32 i;
    UINT32 * pdwptr;

    pdwptr = (UINT32 *) CN_FPGATIMERBASE + CN_FPGACINTCNT;

    for( i = 0; i < 7 ; i++)
        * pbyGetPtr ++ = (UINT8)((* (pdwptr ++))&0xff);

}

//------��ʱ��-----------------------------------------------
//���ܣ�ȡʱ�ӣ��������룩����3��BYTE��˳��ΪMS1��MS2,S
//      ����BCD��ʽ��һ�����ڴ洢ʱ��
//������pbyGetPtr ָ��
//���أ���
// ��ʱ1us
//----------------------------------------------------------
void FpgaTmr_GetTime(UINT8 * pbyGetPtr)
{
    //�ն�1������,��ֹǡ�ý�λʱ������
    *(volatile UINT32 *) (CN_FPGATIMERBASE + CN_FPGACINTCNT);

    * pbyGetPtr++ = (UINT8) (*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAMILLCNT));    //�����
    * pbyGetPtr++ = (UINT8) (*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGAMILHCNT));    //�����
    * pbyGetPtr++ = (UINT8) (*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGASECCNT));    //��
}

//------ȡ�жϼ����Ĵ���------------------------------------
//���ܣ�ȡ�жϼ����Ĵ���
//��������
//���أ�UINT16 ����ֵ
//----------------------------------------------------------
UINT16 FpgaTmr_GetCintCnt(void)
{
    UINT32 dwcnl,dwcnh;

    dwcnl = (UINT32)((*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGACINTCNT))&0xff);
    dwcnh = (UINT32)((*(UINT32 *) (CN_FPGATIMERBASE + CN_FPGACINTCNT + 1))&0xff);

    return ((UINT16)(dwcnl + (dwcnh << 8)));
}

//------ȡ�жϱ�־λ----------------------------------------
//���ܣ�ȡ�жϱ�־λ
//��������
//���أ�TURE ���жϱ�־
//----------------------------------------------------------
BOOL FpgaTmr_GetCintFlg(void)
{
    if (0 == *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGACINTREG))
        return FALSE;
    else
        return TRUE;
}

//------���жϱ�־λ----------------------------------------
//���ܣ����жϱ�־λ
//��������
//���أ���
//----------------------------------------------------------
void FpgaTmr_SetCintFlg(void)
{
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGACINTREG) = 0;
}

//------ȡ1588��־λ----------------------------------------
//���ܣ�ȡ1588��־λ
//��������
//���أ�TURE ��1588���ĵ�
//----------------------------------------------------------
BOOL FpgaTmr_Get1588Flg(void)
{
    if (0 == *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588STA))
        return FALSE;
    else
        return TRUE;
}

//------��1588��־λ----------------------------------------
//���ܣ���1588��־λ
//��������
// �����������
//���أ���
//----------------------------------------------------------
void FpgaTmr_Set1588Flg(void)
{
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588STA) = 0;
}

//------MAC��ַ����--------------------------------------------
//���ܣ����㲥�����⣬1588���սڵ�MAC��ַ
//������MAC��ַ
//���أ���
//----------------------------------------------------------
void FpgaTmr_Set1588Mac(UINT8 * pbyMacAddr)
{
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588MAC) = (UINT32) (* pbyMacAddr);
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588MAC + 1) = (UINT32) (* (pbyMacAddr + 1));
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588MAC + 2) = (UINT32) (* (pbyMacAddr + 2));
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588MAC + 3) = (UINT32) (* (pbyMacAddr + 3));
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588MAC + 4) = (UINT32) (* (pbyMacAddr + 4));
    *(UINT32 *) (CN_FPGATIMERBASE + CN_FPGA1588MAC + 5) = (UINT32) (* (pbyMacAddr + 5));
}

BOOL         FpgaTmr_Get1588FlgAB(UINT32 dwPortNum){return FALSE; }
void         FpgaTmr_Set1588FlgAB(UINT32 dwPortNum){ }
UINT16         FpgaTmr_Get1588RegAB(UINT32 dwPortNum,UINT8 * pbyCintCntLatch,UINT8 * pbyMsCntLatch){return 0; }
void         FpgaTmr_Set1588MacAB(UINT32 dwPortNum,UINT8 * pbyMacAddr){ }

//------��������FPGA----------------------------------------
//���ܣ���������FPGA
//��������
//���أ���
//----------------------------------------------------------
void Fpga_SetMFun( void )
{
    *(UINT32 *) (CN_FPGAMSBASE + CN_FPGAMASTER) = CN_FPGAMSETVALID;
}

//------����FPGA����1588��־--------------------------------
//���ܣ�����FPGA����1588��־
//��������
//���أ�1588��־
//----------------------------------------------------------
UINT32 IEEE1588_GETFLAG(UINT8 macip)
{
    if ( macip ==0 )
    {
        return (*(UINT32*)(CN_1588_GETFLAG))&0xff;
    }
    else
    {
        return (*(UINT32*)(CN_1588_GETFLAG))>>8;
    }
}

//------��FPGA�Ѷ�1588��־----------------------------------
//���ܣ���FPGA�Ѷ�1588��־
//������1588��־
//���أ���
//----------------------------------------------------------
void IEEE1588_SETFLAG(UINT32 data,UINT8 macip)
{
    if ( macip ==0 )
    {
        *(UINT32 *)(CN_1588_CLRFLAG) = data;
    }
    else
    {
        *(UINT32 *)(CN_1588_CLRFLAG) = ((data<<8) | 0xff);
    }
    *(UINT32 *)(CN_1588_CLRFLAG) = 0xffff;
}

//------����FPGA����1588ʱ��--------------------------------
//���ܣ�����FPGA����1588ʱ��
//������1588ʱ����
//���أ�1588ʱ��
//----------------------------------------------------------
UINT32 IEEE1588_GETTIME(UINT8 data,UINT8 macip)
{
    if ( macip ==0 )
    {
        switch (data)
        {
            case 0x00:
                return *(UINT32*)(CN_1588_MAC1RT0);
            break;
            case 0x01:
                return *(UINT32*)(CN_1588_MAC1RT1);
            break;
            case 0x02:
                return *(UINT32*)(CN_1588_MAC1RT2);
            break;
            case 0x03:
                return *(UINT32*)(CN_1588_MAC1RT3);
            break;
            case 0x04:
                return *(UINT32*)(CN_1588_MAC1ST0);
            break;
            case 0x05:
                return *(UINT32*)(CN_1588_MAC1ST1);
            break;
            case 0x06:
                return *(UINT32*)(CN_1588_MAC1ST2);
            break;
            case 0x07:
                return *(UINT32*)(CN_1588_MAC1ST3);
            break;
            default:
                return 0;
            break;
        }
    }
    else
    {
        switch (data)
        {
            case 0x00:
                return *(UINT32*)(CN_1588_MAC2RT0);
            break;
            case 0x01:
                return *(UINT32*)(CN_1588_MAC2RT1);
            break;
            case 0x02:
                return *(UINT32*)(CN_1588_MAC2RT2);
            break;
            case 0x03:
                return *(UINT32*)(CN_1588_MAC2RT3);
            break;
            case 0x04:
                return *(UINT32*)(CN_1588_MAC2ST0);
            break;
            case 0x05:
                return *(UINT32*)(CN_1588_MAC2ST1);
            break;
            case 0x06:
                return *(UINT32*)(CN_1588_MAC2ST2);
            break;
            case 0x07:
                return *(UINT32*)(CN_1588_MAC2ST3);
            break;
            default:
                return 0;
            break;
        }
    }
    return 0;
}




















