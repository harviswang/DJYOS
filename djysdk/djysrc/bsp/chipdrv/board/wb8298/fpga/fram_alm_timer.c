//----------------------------------------------------
//Copyright (C), 2013-2020,  lst.
//��Ȩ���� (C), 2013-2020,   lst.
//����ģ��:���FPGA����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:�����дtimer ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-09-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "stdint.h"
#include "cpu_peri.h"
#include "fram_alm_timer.h"
#include "chips.h"

#define CN_AlmTime_Fram_Addr (0x1FF0)//ʵʱ���ݴ���������е�λ��

//------дʱ�ӵ�����----------------------------------------
//���ܣ���ʱ�Ӵ�ŵ�FRAM
//����������Դ
//���أ����ݸ���
//----------------------------------------------------------
STATUS FRAM_Set_Alm_Time(tagAlmTime *pTime)
{
//    return (FRAM_Write_Data(1,CN_AlmTime_Fram_Addr,
//                    (u8 *)pTime,sizeof(tagAlmTime)));
}

//------�������ʱ��----------------------------------------
//���ܣ���FRAM��ȡʱ��
//����������Դ
//����ֵ  �����ݸ���
//----------------------------------------------------------
STATUS FRAM_Get_Alm_Time(tagAlmTime *pTime)
{
//    return (FRAM_Read_Data(1,CN_AlmTime_Fram_Addr,
//                    (u8 *)pTime,sizeof(tagAlmTime)));
}

