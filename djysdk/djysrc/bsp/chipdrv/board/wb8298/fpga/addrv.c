/*=======================================================    *
 * �ļ�ģ��˵��:                                                *
 *     wb82xϵ�а��AD��������                        *
 * �ļ��汾: v1.00 (˵�����ļ��ĵ�ǰ�汾��Ϣ)                   *
 * ������Ա: ������                                           *
 * ����ʱ��:                                                    *
 * Copyright(c) 2007-2008  Shenzhen NARI Limited Co.            *
 *========================================================    *
 * �����޸ļ�¼(���µķ�����ǰ��):                *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>        *
 *========================================================    *
 * 110504,��Ǭ�գ����ӵ���ƬAD
*/

#include "stdlib.h"
#include "addrv.h"
#include "sunri_types.h"

#define CN_ADADDR1 0x12000000
#define CN_ADADDR2 0x12000002
#define CN_ADSHCTL 0x14800003
#define CN_ADSHSTA2 0x14800006 //add by xg 110504
#define CN_ADSHSTA 0x14800007

#define CN_ADSH1    0x40
#define CN_ADSH2    0x80

#define CN_ADBUSY1    0x01
#define CN_ADBUSY2    0x02
#define CN_ADBUSY3    0x20   //add by xg 110504

static UINT16 wAdSampleBuff[16][6];      //���16ƬAD7656
static UINT16 wADChipNum = 1;        //ȱʡ1Ƭ

/*========================================================*/
//* �������ܣ���ʼ��AD7656����
//* ���������chipnum,AD7656оƬ����Ŀ
//* �����������
//* ����ֵ  ����
/*========================================================*/

void Ad_InitReg(UINT16 wchipnum)
{
wADChipNum = wchipnum;    //init chips
}

/*========================================================*/
//* �������ܣ���ȡAD������ˢ�²���������
//* �����������
//* �����������
//* ����ֵ  �� ������ָ�룬��Ϊ�գ���AD�쳣
/*========================================================*/
UINT16 * Ad_Read()
{
    UINT16 i,j;

    *(UINT8 *)(CN_ADSHCTL) &= ~ (CN_ADSH1 | CN_ADSH2);                    //sample

    if (( 0 == ((CN_ADBUSY1 | CN_ADBUSY2)  & (*(UINT8 *) CN_ADSHSTA)))
        && ( 0 == (CN_ADBUSY3 & (*(UINT8 *) CN_ADSHSTA2))))            //busy?  modify  by xg 110504
    {
        for(j=0;j<wADChipNum;j++)                        //chips
            for(i=0;i<6;i++)                        //6 channels per chip
                wAdSampleBuff[j][i] = *(UINT16 *) (CN_ADADDR1 + j*2);    //get results
        return     (UINT16 *) wAdSampleBuff;                    //return buff
    }
    else
        return NULL;                                //busy return 0

}

/*========================================================*/
//* �������ܣ����ֲ�����
//* �����������
//* �����������
//* ����ֵ  �� ��
/*========================================================*/
void Ad_SampleHold(void)
{

//    *(UINT8 *)(CN_ADSHCTL) |= (CN_ADSH1 | CN_ADSH2);    //hold
}
