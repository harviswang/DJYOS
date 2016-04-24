//----------------------------------------------------
//Copyright (C), 2013-20019,  lst.
//��Ȩ���� (C), 2013-2019,   lst.
//����ģ��:���lvds����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:fpga lvds��������
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
#include "lvdsiodrv.h"
#include "fpga_address.h"

const UINT32 * FPGA_32_LVDSIOBASE = (UINT32 *) LVDS_IO_ADDRESS;
#define CN_LVDSIOBASE         FPGA_32_LVDSIOBASE
#define CN_LVDSIO_RFIFO     (LVDS_IO_ADDRESS + 0xE000)
#define CN_LVDSIO_WFIFO     (LVDS_IO_ADDRESS + 0xF000)
#define CN_KOBASE              0x0000
#define CN_STAMSGBASE        0x0000        //for slave sta buff
#define CN_CTLMSGBASE         0x0010        //for slave ctl buff
#define CN_KOBACKBASE         0x0000
#define CN_KIBASE              0x1000
#define CN_REGBASE             0x2000
#define CN_STABASE             0x2100
#define CN_WRLOCKBASE        0x3000
#define CN_BRDNUM            0x3001
#define CN_VERBASE             0xFF00

#define CN_CTLBUFFLG         16
#define CN_CTLBYTELG        12
#define CN_STABUFFLG        16
#define CN_STABYTELG        14
#define CN_MAXBOARD            16

#define CN_LVDSIOERRCNT     0x3F
#define CN_LVDSIOREVBIT     0x40
#define CN_LVDSIOXORBIT     0x80
#define CN_LVDSIOERRSET     5

#define    CN_LVDSCTLLOCK         0x90
#define CN_LVDSCTLVALID     0x40
#define CN_LVDSBOARDVALID     0xa5
#define CN_WRUNLOCK         0xa5


/***************����Ϊlvds�������ݹ��ܺ궨��*����FPGA�ӿڼĴ������塷***************/
#define CN_CONTROL_REG         0x3100     //reg addrs
#define CN_POLL_REG         0x3101
#define CN_TRANS_REG         0x3102
#define CN_MULTICAST         0x3103
#define CN_TRANS_STATUS     0x3104
#define CN_FIFO_STATUS         0x3105
#define CN_REG_LOCK         0x3106
#define CN_FIFO_FIFO_DONE     0x3107

UINT32   g_dwFifo_num;       //fifo�д洢���������ݰ����ܸ���
UINT32   g_dwFifo_point;     //fifo�д洢���������ݵ�λ��


/*========================================================*/
//* �������ܣ�FPGA�Ĵ������ʣ�д����
//* �����������
//* �����������
//* ����ֵ  ����
/*========================================================*/
void LvdsFPGAWrite(UINT32 dwaddroffset,UINT32 dwdata)
{
 *(UINT32 *)(dwaddroffset+CN_LVDSIOBASE) = dwdata;
}
/*========================================================*/
//* �������ܣ�LVDs��������ģ�������λ
//* �����������
//* �����������
//* ����ֵ  ����
/*========================================================*/
void LvdsFPGAReset()
{
    LvdsFPGAWrite(CN_REG_LOCK,0xAAAA5555);
    LvdsFPGAWrite(CN_CONTROL_REG,LvdsFPGARead(CN_CONTROL_REG)&0xFFFeFFFF);
    g_dwFifo_num = 0;       //fifo�д洢���������ݰ����ܸ��� ����
    g_dwFifo_point = 0;     //fifo�д洢���������ݵ�λ�� ����
    LvdsFPGAWrite(CN_CONTROL_REG,LvdsFPGARead(CN_CONTROL_REG)|0x10000);
    LvdsFPGAWrite(CN_REG_LOCK,0);
}
void LvdsFPGAResetNot(){return; }
/*========================================================*/
//* �������ܣ�LVDs��������ģ���ʼ��
//* �����������ţ����ӣ��鲥�б� ����
//* �����������
//* ����ֵ  ����
/*========================================================*/
void LvdsFPGAInit(UINT8 byBnum,UINT8 byms,UINT16 wMulticastnum)
{
   LvdsFPGAReset();
   LvdsFPGAWrite(CN_REG_LOCK,0xAAAA5555);
   LvdsFPGAWrite(CN_CONTROL_REG,(1<<16) + (byBnum << 8) + byms);//���ñ����ţ�����
   LvdsFPGAWrite(CN_MULTICAST,wMulticastnum);//�����鲥�б�
   LvdsFPGAWrite(CN_REG_LOCK,0);
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ����ѯ����
//* ���������
//* �����������
//* ����ֵ  ����
/*========================================================*/
void LvdsFPGApoll(tagLvdsEnable *ptpoll)
{
    LvdsFPGAWrite(CN_REG_LOCK,0xAAAA5555);
    LvdsFPGAWrite(CN_POLL_REG,(ptpoll->blist << 16) + (ptpoll->tx_en << 2)
                    + (ptpoll->rx_en << 1) + ptpoll->poll_en);
    LvdsFPGAWrite(CN_REG_LOCK,0);
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ�鷢�Ϳ���
//* ���������
//* �����������
//* ����ֵ  ����
/*========================================================*/
void LvdsFPGAtrans(tagLvdsTrans *pttrans)
{
    LvdsFPGAWrite(CN_REG_LOCK,0xAAAA5555);
    LvdsFPGAWrite(CN_TRANS_REG,(pttrans->desboard << 20)
                                + (pttrans->transmode << 16)
                                + pttrans->datalength
                                + (pttrans->ready << 19));
    LvdsFPGAWrite(CN_REG_LOCK,0);
}

/*========================================================*/
//* �������ܣ�FPGA�Ĵ������ʣ�������
//* �����������
//* �����������
//* ����ֵ  ����
/*========================================================*/
UINT32 LvdsFPGARead(UINT32 dwaddroffset)
{
    return *(UINT32*)(dwaddroffset + CN_LVDSIOBASE);
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ�鴫��״̬
//* ���������
//* �����������
//* ����ֵ  ����
/*========================================================*/
UINT32 LvdsFPGATransR()
{
    return LvdsFPGARead(CN_TRANS_STATUS);
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ��fifo״̬
//* ���������
//* �����������
//* ����ֵ  ����
/*========================================================*/
UINT32 LvdsFPGAfifoSR()
{
    return LvdsFPGARead(CN_FIFO_STATUS);
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ��д����
//* �������������Դ�����ݳ���
//* �����������
//* ����ֵ  ����
/*========================================================*/
void LvdsFPGADataW(UINT32* dwdata, UINT32 wlength)
{
    UINT32 dwlgh;
    UINT8 sa,da,xorcheck;
	UINT32 times,loop;
	UINT64 LenChk;

    dwlgh = *(UINT32 *) dwdata;
    da = (dwlgh&0xff) + ((dwlgh>>10)&0xc0);         // Ŀ���ַ
    sa = (dwlgh>>8)&0xff;                           // Դ��ַ
    xorcheck = 0xaa ^ wlength ^ sa ^ da;            // Ч����

    LenChk = (sa<<24) + (da<<8) + 0xaa00eb;
    LenChk = (LenChk<<32) + (xorcheck<<16)+wlength;
    dmacpy((unsigned long)CN_LVDSIO_WFIFO, (unsigned long) &LenChk, 8);

//    for(j=0; j < wlength; j++)
//        *(UINT32 *)(CN_LVDSIO_WFIFO) = *(UINT32 *) dwdata++;

	times = (wlength * 4)/128 + ((wlength * 4)%128 ? 1:0);
	for(loop = 0; loop < times; loop++)
        dmacpy((unsigned long)CN_LVDSIO_WFIFO,
              (unsigned long) dwdata + loop *128, 128);

    dwlgh = (LvdsFPGARead(CN_TRANS_REG)&0xffff0000) + wlength;
    LvdsFPGAWrite(CN_REG_LOCK,0xAAAA5555);
    LvdsFPGAWrite(CN_TRANS_REG,dwlgh | 0x80000);     // ����׼�����
    LvdsFPGAWrite(CN_REG_LOCK,0);
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ�������
//* �������������Դ
//* �����������
//* ����ֵ  �����ݸ���
/*========================================================*/
UINT32 LvdsFPGADataR(UINT32* dwdata)
{
    UINT32 i2,i3,dwNum,dwLen;
    UINT32 i1;
    UINT32 times,temp,loop,times2;

    i1 = LvdsFPGAfifoSR();                          // ��ȡĿǰFIFO����״̬
    i2 = i1 & 0xffff;                                 // ��ȡĿǰFIFO���ݰ�����
       i3 = i1 >> 16;                                    // ��ȡĿǰFIFO����ָ��λ��

    if (i2 < g_dwFifo_num)
        g_dwFifo_num = g_dwFifo_num - 65536;          // ����65536�Զ���ת
    dwNum = i2 - g_dwFifo_num;                      // �������ݰ�����

    if (i3 < g_dwFifo_point)
        g_dwFifo_point = g_dwFifo_point - 65536;      // ����65536�Զ���ת
    dwLen = i3 - g_dwFifo_point;                    // ���㱨���ܳ���

    if( dwLen >= 4096 )                                // ���ĳ����ݴ�
        return 0;

//    for(i1=0; i1<dwLen; i1++)
//        * dwdata ++ = *(UINT32 *)(CN_LVDSIO_RFIFO);
    times = (dwLen*8)/128;
    for(loop = 0;loop < times;loop++)
		dmacpy((unsigned long)dwdata + 128*loop,CN_LVDSIO_RFIFO, 128);

    temp = (dwLen*8)%128;
    if(temp)
    {
    	times2 = temp/4;
    	for(loop = 0;loop < times2;loop++)
    		* (dwdata + times*32 + loop) = *(UINT32 *)(CN_LVDSIO_RFIFO);
    }

    i1 = LvdsFPGARead(CN_FIFO_FIFO_DONE);           // FIFO�����Ѷ�����

    g_dwFifo_num = i2;                              // ��¼�Ѷ�FIFO���ݰ�����
    g_dwFifo_point = i3;                            // ��¼�Ѷ�FIFO����ָ��λ��
    return dwNum;
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ�������׼����
//* �����������
//* �����������
//* ����ֵ  ��BOOL
/*========================================================*/
BOOL LvdsFPGAReadReady()
{
    UINT32 dwVal;
    dwVal = LvdsFPGATransR();
    if ( dwVal&0x80000000 )                             // �Ƿ�洢һ����������֡
        return TRUE;
    else
        return FALSE;
}

/*========================================================*/
//* �������ܣ�LVDs��������ģ��д����׼����
//* �����������
//* �����������
//* ����ֵ  ��BOOL
/*========================================================*/
BOOL LvdsFPGAWriteReady()
{
    UINT32 dwVal;
    dwVal = LvdsFPGATransR();
    if ( dwVal&0x40000000 )                             // ��һ֡δ����
        return FALSE;
    else
        return TRUE;
}

/*========================================================*/
//* �������ܣ���ʼ��LVDSIO �ڴ�Ϳ��ƼĴ�������ʼ��������Ч״̬
//* �����������
//* �����������
//* ����ֵ  ����
/*========================================================*/
void Lvdsio_InitReg()
{
    UINT16 i,j;

    *(UINT32 *) (CN_LVDSIOBASE + CN_WRLOCKBASE) = CN_WRUNLOCK; //unlock the buffer write
    for(j=0;j<CN_MAXBOARD;j++)
    {
        for(i=0;i<CN_CTLBUFFLG;i++)
            *(UINT32 *) (CN_LVDSIOBASE + CN_KOBASE + i + j*CN_CTLBUFFLG) = 0;
        *(UINT32 *) (CN_LVDSIOBASE + CN_REGBASE+j) = 0;
    }
    *(UINT32 *) (CN_LVDSIOBASE + CN_KOBASE) = 0;        //fpga bug must be added

    *(UINT32 *) (CN_LVDSIOBASE + CN_WRLOCKBASE) = 0;    //lock the boffer write
}

/*========================================================*/
//* �������ܣ���ʼ����������Ӧ�����LVDSIO����
//* ���������byBoardNum��byBoardType byCtlByteָ�� ������TRUE��Ч��
//* �����������
//* ����ֵ  ����
/*========================================================*/
void Lvdsio_StartBoard(UINT8 byBoardNum,UINT8 byBoardType, UINT8 * pbyCtlByte, BOOL bLock )
{
    tagLvdsioCtlFrm  * ptLvdsioCtl;
    UINT32 i;

    *(UINT32 *) (CN_LVDSIOBASE + CN_WRLOCKBASE) = CN_WRUNLOCK; //unlock the buffer write
    if (byBoardNum < CN_MAXBOARD)
        {
        ptLvdsioCtl = (tagLvdsioCtlFrm  *) (CN_LVDSIOBASE + CN_KOBASE + byBoardNum * CN_CTLBUFFLG);
        ptLvdsioCtl->bySourseAddr = 0xe0;
        ptLvdsioCtl->byBoardType = byBoardType;
        for(i=0; i<CN_CTLBYTELG; i++)
            ptLvdsioCtl->byCtlByte[i] = * (pbyCtlByte + i);
        if ( bLock == TRUE)
            ptLvdsioCtl->byLockByte = CN_LVDSCTLLOCK;
        else
            ptLvdsioCtl->byLockByte = 0;
        ptLvdsioCtl->byValidByte = CN_LVDSCTLVALID | byBoardNum;
        }
    *(UINT32 *) (CN_LVDSIOBASE + CN_KOBASE) = 0;        //fpga bug must be added
    *(UINT32 *) (CN_LVDSIOBASE + CN_WRLOCKBASE) = 0; //lock the buffer write
}

void         Lvdsio_UnlockCtl(UINT8 byBoardNum,BOOL bLock ){return; }
UINT8   g_byKiByte[16];
/*========================================================*/
//* �������ܣ�ȡ��KI��������ָ��
//* ���������byBoardNum
//* �����������
//* ����ֵ  ��KI��������ָ����
/*========================================================*/
UINT8 * Lvdsio_GetStaBuff(UINT8 byBoardNum)
{
    UINT32 * pdwptr;
    UINT32 i;

    if (byBoardNum < CN_MAXBOARD)
    {
        pdwptr = (UINT32 *) (CN_LVDSIOBASE + CN_KIBASE + byBoardNum*CN_STABUFFLG);
        for (i=0; i<16; i++)
            g_byKiByte[i] = (UINT8) pdwptr[i];
        return g_byKiByte;
    }
    else
        return NULL;
}

/*========================================================*/
//* �������ܣ���KI����������ָ�����ڴ�
//* ���������byBoardNum ���
//* ���������UINT32 * kibyte ����˳��Ϊ �����ַ���������롢״̬�루14BYTE)
//* ����ֵ  ����
/*========================================================*/
void Lvdsio_GetStaMsg(UINT8 byBoardNum, UINT8 * pbyKiByte)
{
    UINT32 * pdwptr;
    UINT32 i;

    pdwptr = (UINT32 *)(CN_LVDSIOBASE + CN_KIBASE + byBoardNum*CN_STABUFFLG);
    for (i=0; i<16; i++)
        * pbyKiByte ++ =(UINT8) (* pdwptr ++);
}
/*========================================================*/
//* �������ܣ���ȡ�������״̬��ERRCNT����ERRSET ΪFAULSE
//* ���������byBoardNum
//* �����������
//* ����ֵ  ��true��false����ȷ����״̬֡
/*========================================================*/
BOOL Lvdsio_GetRevSta(UINT8 byBoardNum)
{
      UINT32 * pdwstaptr;
      UINT32 * pdwregptr;

      pdwstaptr = (UINT32 *)(CN_LVDSIOBASE + CN_STABASE + byBoardNum);
      pdwregptr = (UINT32 *)(CN_LVDSIOBASE + CN_REGBASE + byBoardNum);

    if ((byBoardNum < CN_MAXBOARD) &&
        (((*pdwstaptr) & CN_LVDSIOERRCNT) < CN_LVDSIOERRSET) &&  //2008-8-20 8:36:54
        (((*pdwregptr) & 0xff) == CN_LVDSBOARDVALID))    //ctl open
        return TRUE;
    else
        return FALSE;
}
/*========================================================*/
//* �������ܣ���ȡ���̬
//* ���������byBoardNum
//* �����������
//* ����ֵ  ��true��false������
/*========================================================*/
BOOL Lvdsio_GetBoardSta(UINT8 byBoardNum)
{
      UINT32 * pdwstaptr;
      UINT32 * pdwregptr;

      pdwstaptr = (UINT32 *)(CN_LVDSIOBASE + CN_STABASE + byBoardNum);
      pdwregptr = (UINT32 *)(CN_LVDSIOBASE + CN_REGBASE + byBoardNum);

    if ((byBoardNum < CN_MAXBOARD) &&
        (((*pdwstaptr) & CN_LVDSIOREVBIT) == 0) &&  //rev bit
        (((*pdwregptr) & 0xff) == CN_LVDSBOARDVALID))    //ctl open
        return TRUE;
    else
        return FALSE;
}
/*========================================================*/
//* �������ܣ�������Ӧ����Ŀ��ƼĴ���
//* ���������byBoardNum,byCtlReg(TRUE �򿪣�
//* �����������
//* ����ֵ  ����
/*========================================================*/
void Lvdsio_SetCtlReg(UINT8 byBoardNum,UINT8 byCtlReg)
{
    if (byBoardNum < CN_MAXBOARD)
    {
        if (byCtlReg == TRUE)
            *(UINT32 *) (CN_LVDSIOBASE + CN_REGBASE + byBoardNum) = CN_LVDSBOARDVALID;
        else
            *(UINT32 *) (CN_LVDSIOBASE + CN_REGBASE + byBoardNum) = 0;
    }
}

/*========================================================*/
//* �������ܣ�ȡ�ÿ��ƼĴ�����״̬��
//* ���������byBoardNum
//* �����������
//* ����ֵ  ��TRUE/FALSE  ��ذ��LVDS�������߿���
/*========================================================*/
BOOL Lvdsio_GetCtlReg(UINT8 byBoardNum)
{
    if ((byBoardNum < CN_MAXBOARD) &&
        ( *(UINT32 *) (CN_LVDSIOBASE + CN_REGBASE + byBoardNum) == CN_LVDSBOARDVALID))
        return TRUE;
    else
        return FALSE;
}

/*========================================================*/
//* �������ܣ����ôӰ���
//* ������������
//* �����������
//* ����ֵ  ����
/*========================================================*/
void Lvdsio_SetSlaveBrdNum(UINT8 byBoardNum)
{
    if (byBoardNum < CN_MAXBOARD)
        *(UINT32 *) (CN_LVDSIOBASE + CN_BRDNUM) = byBoardNum;
        else
        *(UINT32 *) (CN_LVDSIOBASE + CN_BRDNUM) = 0;
}

/*========================================================*/
//* �������ܣ����ôӰ�״̬֡,
//* ���������UINT32 * pbyStaByte ����˳��Ϊ �����ַ���������롢״̬�루14BYTE)
//* �����������
//* ����ֵ  ����
/*========================================================*/
void Lvdsio_SetSlaveStaMsg(UINT8 * pbyStaByte)
{
    UINT32 * pdwptr;
    UINT32 i;

    pdwptr = (UINT32 *)(CN_LVDSIOBASE + CN_STAMSGBASE);
    for (i=0; i<16; i++)
        * pdwptr ++ =(UINT32) (* pbyStaByte ++);
}

/*========================================================*/
// �������ܣ����մӰ����֡�������巢����,
//* �����������
//* ���������UINT32 * byCtlByte ����˳��Ϊ �����ַ���������롢�����루14BYTE)
//* ����ֵ  ����
/*========================================================*/
void Lvdsio_SetSlaveCtlMsg(UINT8 * pbyCtlByte)
{
    UINT32 * pdwptr;
    UINT32 i;

    pdwptr = (UINT32 *)(CN_LVDSIOBASE + CN_CTLMSGBASE);
    for (i=0; i<16; i++)
        * pbyCtlByte ++ =(UINT8) (* pdwptr ++);
}
