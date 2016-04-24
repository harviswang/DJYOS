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

#include "stdint.h"
#include "board-cfg.h"
#include "cpld.h"


#define cn_cpld_led_addr                 (cn_sys_cpld_addr_base + 0x08)
#define cn_cpld_boardno_addr             (cn_sys_cpld_addr_base + 0x02) //��CPLD������ŵ�ַ
#define cn_cpld_info_addr                (cn_sys_cpld_addr_base + 0x20) //��CPLD�������Ϣ��ַ


void  CPLD_LedOn(u8 num)
{
    if(num > 2)
    {
        return;
    }
    else
    {
//      *(char *)(cn_cpld_led_addr + num) = 0x00;
    }
}

void  CPLD_LedOff(u8 num)
{
    if(num > 2)
    {
        return;
    }
    else
    {
//      *(char *)(cn_cpld_led_addr + num) = 0xff;
    }
}
//----�����װ���еı��----------------------------------------------------
//���ܣ���CPLD�ж�ȡ�����װ���еı��
//����: ��
//����: ���
//--------------------------------------------------------------------------
uint8_t Hard_BoardNo_Read( void )
{
    uint8_t   byBoard;
//    __asm__ __volatile__("sync; isync; msync");
    byBoard = (uint8_t)((*(u8*)cn_cpld_boardno_addr) & 0x00FF);
//    __asm__ __volatile__("sync; isync; msync");
//    byBoard = 10;
    return(byBoard);
}

// -----��ȡCPLD�汾��ʱ��------------------------------------------------
//���ܣ���ȡCPLD�İ汾��Ϣ�Ͱ汾ʱ�䣬��FPGA��Ӧ��ַ
//������tgfpgaver���汾��Ϣ�Ľṹ��
//���أ���
// -----------------------------------------------------------------------
void Cpld_GetInfo(struct tagCpldVer *tgfpgaver)
{
    tgfpgaver->dwCpldYearHi    = *(uint8_t *)(cn_cpld_info_addr + 0x00);    //���
    tgfpgaver->dwCpldYearLo    = *(uint8_t *)(cn_cpld_info_addr + 0x01);  //���
    tgfpgaver->dwCpldMonth     = *(uint8_t *)(cn_cpld_info_addr + 0x02);  //��
    tgfpgaver->dwCpldDay       = *(uint8_t *)(cn_cpld_info_addr + 0x03);  //��
    tgfpgaver->dwBoardTypeHi   = *(uint8_t *)(cn_cpld_info_addr + 0x04);  //����ͺŸ�
    tgfpgaver->dwBoardTypeLo   = *(uint8_t *)(cn_cpld_info_addr + 0x05);  //����ͺŵ�
    tgfpgaver->dwProgVerHi     = *(uint8_t *)(cn_cpld_info_addr + 0x06);  //CPLD����汾��
    tgfpgaver->dwProgVerLo     = *(uint8_t *)(cn_cpld_info_addr + 0x07);  //CPLD����汾��
}
