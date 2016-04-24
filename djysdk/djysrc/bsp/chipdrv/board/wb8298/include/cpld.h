//----------------------------------------------------
//Copyright (C), 2013-2020,  lst.
//��Ȩ���� (C), 2013-2020,   lst.
//����ģ��:
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:WB8298�弶cpld�й���ģ�飬���ȡ����š������Ϣ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-02-12
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef __CPLD_H__
#define __CPLD_H__

//��ȡFPGA�汾��Ϣ�İ汾ʱ��
struct tagCpldVer
{
	uint8_t dwCpldYearHi;               //CPLD�汾ʱ����H
	uint8_t dwCpldYearLo;               //CPLD�汾ʱ����L
    uint8_t dwCpldMonth;                //CPLD�汾ʱ����
    uint8_t dwCpldDay;                  //CPLD�汾ʱ����
    uint8_t dwBoardTypeHi;              //������͸�
    uint8_t dwBoardTypeLo;              //������͵�
    uint8_t dwProgVerHi;                //CPLD����汾��
    uint8_t dwProgVerLo;                //CPLD����汾��
};


uint8_t Hard_BoardNo_Read( void );
void CpldVer_GetInfo(struct tagCpldVer *tgfpgaver);




#endif

