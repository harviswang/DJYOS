#ifndef __FPGA_H_
#define __FPGA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CN_LOCK_VALUE          0xAA55
//CPLD��ַ    0x60000000              
typedef struct 
{
    uc16 uc16AC_Freq;           //��Ƶ(PT�źŽ���Ƶ��)
    uc16 uc16AD_Busy;           //AD�Ƿ�æ��־
    u16 u16LcdContrast;         //ֻд��Һ���Աȶȿ���
    u16 u16FeedDog;             //ֻд��ι��
    u16 u16KI_RW;               //д�뿪��ͨ����ַ������8������
    u16 u16KO1;                 //ֻд������1
    uc16 uc16KO2;               //ֻд������2
    uc16 uc16AD_DataLen;        //AD���ݳ���
    u16 u16AD_Data;             //AD����
    u16 u16IntInterval;         //ֻд�������жϼ����uS
    u16 u16Lock;                //�����Ĵ���(д��0xaa55)
    u16 u16UnLock;              //�����Ĵ���(д��0xaa55)
    u16 u16EnAD;                //ʹ��AD���ģ��(��ʹ�ܣ���AD�Ͳ�Ƶ�����ܹ���)
    u16 u16Counter10mS;         //10ms������
}tagFpgaReg;

extern tagFpgaReg volatile * const g_ptFpgaReg;

#ifdef __cplusplus
}
#endif

#endif //__FPGA_H_

