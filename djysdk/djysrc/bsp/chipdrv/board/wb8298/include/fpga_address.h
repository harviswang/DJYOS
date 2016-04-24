//----------------------------------------------------
//Copyright (C), 2013-2020,  lst.
//��Ȩ���� (C), 2013-2020,   lst.
//����ģ��:�弶��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:FPGA�ĵ�ַ,�ò��ֵ�ַ������ĳ���ض�ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-09-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __FPGA_ADDRESS_H_
#define __FPGA_ADDRESS_H_


#ifdef __cplusplus
extern "C" {
#endif

//-----todo
//����p1020ͨ��PCIE����FPGA,��˱��ļ��еĵ�ַ��PCIE����ĵ�ַ���
//ĿǰPCIE����FPGA�ĵ�ַΪ0xA0000000
#define CN_FPGA_BASE_ADDR                (0xA0000000)

//FPGA������Ϣ��ַ
#define CN_ADDR_BOARDTYPE_LO             (CN_FPGA_BASE_ADDR + 0x3FC08)        // FPGA�����İ�����͵�λ29(WB82901)��WB8����ʡȥ��ĩλ1��ʾFPGAС�汾
#define CN_ADDR_BOARDTYPE_HI             (CN_FPGA_BASE_ADDR + 0x3FC0C)        // FPGA�����İ�����͵�λ01(WB82901)��WB8����ʡȥ��ĩλ1��ʾFPGAС�汾
#define CN_ADDR_FPGADATE_DAY             (CN_FPGA_BASE_ADDR + 0x3FC10)        // FPGA�汾����-��
#define CN_ADDR_FPGADATE_MONTH           (CN_FPGA_BASE_ADDR + 0x3FC14)        // FPGA�汾����-��
#define CN_ADDR_FPGADATE_YEAR            (CN_FPGA_BASE_ADDR + 0x3FC18)        // FPGA�汾����-��
#define CN_ADDR_FPGAINFO_BASE            (CN_FPGA_BASE_ADDR + 0x3FC08)        // FPGA��Ϣ��ʼ��ַ
#define CN_ADDR_FPGAINFO_UINT32LEN       (5)                                  // FPGA��Ϣ��UINT32����

//FPGA������ŵ�ַ
#define CN_HARD_HARD_BASE                (CN_FPGA_BASE_ADDR + 0x3FC1C)

//FPGA��timerģ���ַ
#define FPGA_TIMER_ADDRESS               (CN_FPGA_BASE_ADDR + 0x02000)

//����FPGA���õ�ַ
#define FPGA_MS_ADDRESS                  (CN_FPGA_BASE_ADDR + 0x0C000)

//LVDSIO����
#define LVDS_IO_ADDRESS                  (CN_FPGA_BASE_ADDR + 0x00000)


//B����ؼĴ�����ַ����
#define  CN_FPGA_TIMER_CLR_FLAG          (CN_FPGA_BASE_ADDR + 0x23808)           // B��������Уʱ��־�ĵ�ַ
#define  CN_FPGA_TIMER_FLAG              (CN_FPGA_BASE_ADDR + 0x2380c)           // B�������Уʱ��־�ĵ�ַ
#define  CN_FPGA_TIMER_DATA_L            (CN_FPGA_BASE_ADDR + 0x23810)           // B��Уʱ���յ�ʱ/��/��
#define  CN_FPGA_TIMER_DATA_H            (CN_FPGA_BASE_ADDR + 0x23814)           // B��Уʱ���յ��������


#ifdef __cplusplus
}
#endif


#endif
