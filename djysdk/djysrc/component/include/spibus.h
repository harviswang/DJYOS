// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��SPIbus.h
// ģ������: SPIģ��Ľṹ�弰API����
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 02/07.2014
// =============================================================================

#ifndef __SPIBUS_H__
#define __SPIBUS_H__

#include "rsc.h"

typedef ptu32_t (*TransferFunc)(ptu32_t SpecificFlag,u32 sendlen,u32 recvlen,u32 recvoff);
typedef bool_t (*TransferPoll)(ptu32_t SpecificFlag,u8* srcaddr,u32 sendlen,
        u8* destaddr,u32 recvlen,u32 recvoff);
typedef bool_t (*CsActiveFunc)(ptu32_t SpecificFlag, u8 cs);
typedef bool_t (*CsInActiveFunc)(ptu32_t SpecificFlag, u8 cs);
typedef ptu32_t (*SPIBusCtrlFunc)(ptu32_t SpecificFlag,u32 cmd,ptu32_t data1,ptu32_t data2);

struct tagSPI_Param
{
    char            *BusName;               //�������ƣ���SPI1
    u8              *SPIBuf;                //���߻�����ָ��
    u32             SPIBufLen;              //���߻�������С���ֽ�
    ptu32_t         SpecificFlag;			//SPI˽�б�ǩ������ƼĴ�����ַ
    bool_t          MultiCSRegFlag;         //SPI���ƼĴ����Ƿ��ж���CS���üĴ���
    TransferFunc    pTransferTxRx;			//���ͽ��ջص��������жϷ�ʽ
    TransferPoll    pTransferPoll;			//���ͽ��ջص���������ѯ��ʽ
    CsActiveFunc    pCsActive;				//Ƭѡʹ��
    CsInActiveFunc  pCsInActive;			//Ƭѡʧ��
    SPIBusCtrlFunc  pBusCtrl;				//���ƺ���
};

//SPI���������ṹ��
struct tagSPI_Device
{
    struct tagRscNode DevNode;
    u8 Cs;                                  //Ƭѡ�ź�
    bool_t AutoCs;                          //�Զ�Ƭѡ
    u8 CharLen;                             //���ݳ���
    u8 Mode;                                //ģʽѡ��
    u8 ShiftDir;                            //MSB or LSB
    u32 Freq;                               //�ٶ�,Hz
};

struct tagSPI_DataFrame
{
    u8* SendBuf;        //��������ָ��
    u32 SendLen;        //�������ݳ��ȣ��ֽ�
    u8* RecvBuf;        //��������ָ��
    u32 RecvLen;        //�������ݳ��ȣ��ֽ�
    u32 RecvOff;        //��������ƫ��
};

struct tagSPI_Buf
{
    u32    Offset;         //������ָ��,ָ����һ�ζ���λ��
    u32    MaxLen;        //��������󳤶�,Ԫ�ظ���.
    u8     *pBuf;             //������ָ��,�û��Լ���֤�����ٵĻ������Ƿ����趨
};
//SPI���߿��ƿ�ṹ��,��ģ��ɼ�
struct tagSPI_CB
{
    struct tagRscNode       SPI_BusNode;                //������Դ�ڵ�
    struct tagSPI_Buf       SPI_Buf;                    //������,�����첽����
    struct tagSemaphoreLCB *SPI_BusSemp;                //SPI���߱����ź���
    struct tagSemaphoreLCB *SPI_BlockSemp;              //���׻����������ź���
    struct tagSPI_DataFrame Frame;
    struct tagSPI_Device    *CurrentDev;                //ռ�е�ǰ���ߵ��豸
    u16                     ErrorPopEvtt;               //�������¼�������
    bool_t                  MultiCsReg;                 //�Ƿ���ж���CS�Ĵ���
    u8                      BlockOption;                //��ǰ���߲����Ƿ�����
    u8                      Flag;						//��ѯ�жϱ��
    ptu32_t                 SpecificFlag;               //���Ա��
    TransferFunc            pTransferTxRx;
    TransferPoll            pTransferPoll;
    CsActiveFunc            pCsActive;
    CsInActiveFunc          pCsInActive;
    SPIBusCtrlFunc          pBusCtrl;
};


// SPI mode flags
#define SPI_CPHA    0x01            /* clock phase */
#define SPI_CPOL    0x02            /* clock polarity */
#define SPI_MODE_0  (0|0)           /* (original MicroWire) */
#define SPI_MODE_1  (0|SPI_CPHA)
#define SPI_MODE_2  (SPI_CPOL|0)
#define SPI_MODE_3  (SPI_CPOL|SPI_CPHA)

#define SPI_SHIFT_MSB  0
#define SPI_SHIFT_LSB  1

#define SPI_MODE_SLAVE  0
#define SPI_MODE_MASTER 1

typedef struct SpiConfig
{
   u8 Mode;     //����CPHA,CPOL
   u8 ShiftDir; //MSB or LSB
   u8 CharLen;  //�ַ�����,bits
   u32 Freq;    //ʱ��Ƶ��
} tagSpiConfig;

#define CN_INVALID_CS               0xFF    //��Ч��Ƭѡ

//����BusCtrl()�Ĳ���cmd,0x80������ϵͳ���壬0x80�����û�����
#define CN_SPI_SET_CLK               0
#define CN_SPI_SET_ERROR_POP_EVENT   1
#define CN_SPI_CS_CONFIG             2
#define CN_SPI_SET_AUTO_CS_EN        3
#define CN_SPI_SET_AUTO_CS_DIS       4
#define CN_SPI_SET_POLL				 5
#define CN_SPI_SET_INT               6
#define CN_SPI_OS_USED               0x80

//�����������
#define CN_SPI_EXIT_NOERR                   0
#define CN_SPI_EXIT_UNKNOW_ERR              -1
#define CN_SPI_EXIT_TIMEOUT                 -2
#define CN_SPI_EXIT_PARAM_ERR               -3


//���巢�����󵯳��¼�����ֵ
#define CN_SPI_POP_UNKNOW_ERR               (0x01 << 16)        //δ֪����

//SPIģ��API����
struct tagRscNode *ModuleInstall_SPIBus(ptu32_t para);

struct tagSPI_CB *SPI_BusAdd(struct tagSPI_Param *NewSPIParam);
struct tagSPI_CB *SPI_BusAdd_s(struct tagSPI_Param *NewSPIParam,struct tagSPI_CB *NewSPI);
bool_t SPI_BusDelete(struct tagSPI_CB *DelSPI);
bool_t SPI_BusDelete_s(struct tagSPI_CB *DelSPI);
struct tagSPI_CB *SPI_BusFind(char *BusName);

struct tagSPI_Device *SPI_DevAdd(char *BusName ,char *DevName,u8 cs,u8 charlen,
                                u8 mode,u8 shiftdir,u32 freq,u8 autocs);
struct tagSPI_Device *SPI_DevAdd_s(char *BusName, char *DevName,
                                struct tagSPI_Device *NewDev);
bool_t SPI_DevDelete(struct tagSPI_Device *DelDev);
bool_t SPI_DevDelete_s(struct tagSPI_Device *DelDev);
struct tagSPI_Device *SPI_DevFind(char *DevName);

s32 SPI_Transfer(struct tagSPI_Device *Dev,struct tagSPI_DataFrame *spidata,
                u8 block_option,u32 timeout);
bool_t SPI_CsActive(struct tagSPI_Device *Dev,u32 timeout);
bool_t SPI_CsInactive(struct tagSPI_Device *Dev);
s32  SPI_PortRead( struct tagSPI_CB *pSPI,u8 *buf,u32 len);
s32  SPI_PortWrite(struct tagSPI_CB *pSPI,u8 *buf,u32 len);
s32  SPI_BusCtrl(struct tagSPI_Device *Dev,u32 cmd,ptu32_t data1,ptu32_t data2);
s32  SPI_ErrPop(struct tagSPI_CB *pSPI, u32 ErrNo);

#endif /* __SPIBUS_H__ */
