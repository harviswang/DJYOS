//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: SD������ģ��
//���ߣ�wjz
//�汾��V1.0.0
//�ļ�����: ����tq2440��sd����������֧��sdv2.0���ϵ�sd��(2G���ϲ�֧��)
//����˵��:������ģ��Դ���������ǳ������ӵ�sd�������޸Ķ���
//�޶���ʷ:
// 1. ����: 2011-06-19
//   ����: wjz
//   �°汾��: V1.0.0
//   �޸�˵��: ��ֲsd��������tq2440
//------------------------------------------------------

#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include "cpu_peri.h"

/************************SD������˵��****************************************
Class0 :(����ʶ�𡢳�ʼ���Ȼ������)
cmd0:��λSD ��.
CMD1:��OCR�Ĵ���.
cmd9:��CSD�Ĵ���.
CMD10:��CID�Ĵ���.
CMD12:ֹͣ�����ʱ�����ݴ���
cmd13:�� Card_Status �Ĵ���

Class2 (�������):
CMD16:���ÿ�ĳ���
CMD17:������.
CMD18:�����,ֱ����������CMD12Ϊֹ .

Class4(д�����) :
CMD24:д����.
CMD25:д���.
CMD27:дCSD�Ĵ��� .

Class5 (���������):
CMD32:���ò��������ʼ��ַ.
CMD33:���ò��������ֹ��ַ.
CMD38: ������ѡ��Ŀ�.

Class6(д�������):
CMD28:����д������ĵ�ַ.
CMD29:����д������ĵ�ַ.
CMD30: Ask the card for the status of the write protection bits

class7�����������������������
class8�������ض���� ��
class10 ��11 ������
*/

unsigned int Tx_buf[128];   //128[word]*4bit=512[byte]
unsigned int Rx_buf[128];   //128[word]*4bit=512[byte]
volatile unsigned int rd_cnt;
volatile unsigned int wt_cnt;
int MMC=0;  // 0:SD  , 1:MMC ���ţ��Ժ�MMC��ʹ��
volatile int RCA;

//----��С���໥ת��---------------------------------------------------
//���ܣ�int���͵ģ���С������ת��(��С�˿���תΪ��ˣ����Ҳ����תΪС��)
//������inta�� int���Ͳ���
//      intb�� int���Ͳ���
//���أ�
//---------------------------------------------------------------------
void byte_conversion(uint32_t *inta,uint32_t *intb)
{
    uint8_t *a_byte;
    uint8_t *b_byte;

    a_byte=(uint8_t *)inta;
    b_byte=(uint8_t *)intb;
    b_byte[3]=a_byte[0];
    b_byte[2]=a_byte[1];
    b_byte[1]=a_byte[2];
    b_byte[0]=a_byte[3];

}


//----SD��ѡ��--------------------------------------------------------
//���ܣ�������������ѡ��һ��SD��
//������sel_desel
//���أ�1���ɹ�
//      0��ʧ��
//---------------------------------------------------------------------

void card_sel_desel(char sel_desel)
{
    //-- Card select or deselect
    if(sel_desel)
    {
RECMDS7:
    rSDICARG=RCA<<16;   // CMD7(RCA,stuff bit)
    rSDICCON= (0x1<<9)|(0x1<<8)|0x47;   // sht_resp, wait_resp, start, CMD7
    //����CMD7��ʵ�ֶԿ���ѡ��
    if(!chk_cmd_end(7, 1))
        goto RECMDS7;
    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    //--State(transfer) check
    if( (rSDIRSP0 & 0x1e00) !=0x800 )
        goto RECMDS7;
    }
    else
    {
RECMDD7:
        rSDICARG=0<<16;     //CMD7(RCA,stuff bit)
        rSDICCON=(0x1<<8)|0x47; //no_resp, start, CMD7
        //����CMD7��ʵ�ֶԿ���ѡ��
        if(!chk_cmd_end(7, 0))
        goto RECMDD7;
        rSDICSTA=0x800; // Clear cmd_end(no rsp)
    }
}
//----��һ����--------------------------------------------------------
//���ܣ���������������ȡSD��ָ��ĳһ����
//������addr��������
//      buf��������
//���أ�
//---------------------------------------------------------------------

void read_one_sector(u32 sector_addr,unsigned int *buf)
{
    unsigned int value;
    int status;

    rd_cnt=0;
    rSDIFSTA=rSDIFSTA|(1<<16);  // FIFO reset
/*
    ���� [31:25]
    Burst4 enable   [24]    ��DMAģʽ��ʹ��Burst4���������ݴ�С����ʱ��λ��
     (Burst4)               ��λ��0����Ч 1��Burst4ʹ��

    Data Size       [23:22] ָ����FIFO����Ĵ�С���ĸ����ͣ����ֻ��֡�00 = �ֽ�
    (DataSize)              ���䣬01 =���ִ��� 10 = �ִ��䣬 11 = ����

    SDIO Interrupt  [21]    ����SDIO���ж������� 2�����ڻ����ⲿ�������ڣ������ݿ�
    Period Type(PrdType)    ��󱻷��ͣ���SDIO����0=������������ 1=�������ڣ������ڣ�

    Transmit After  [20]    �������ݴ�������Ӧ�յ���ʼ�򲻿�ʼ��0= ��DatMode���ú�ֱ��
    Response(TARSP)         1= ����Ӧ�յ��󣨼ٶ�����DatMode��Ϊ 2b11��

    Receive After   [19]    �������ݴ������������ʼ�򲻿�ʼ 0= ��DatMode���ú�ֱ��
    Command(RACMD)          1= ��������󣨼ٶ�����DatMode��Ϊ2b10��

    Busy After      [18]    ����æ�������������ʼ�򲻿�ʼ 0= ��DatMode���ú�ֱ��
    Command(BACMD)          1= ��������󣨼ٶ�����DatMode��Ϊ2b01��

    Block mode      [17]    ���ݴ���ģʽ 0=�����ݴ��� 1=ģ�����ݴ���
    (BlkMode)

    Wide bus        [16]    ����ʹ�ܿ�����ģʽ 0����׼����ģʽ����ʹ��SDIDAT[0]��
    enable(WideBus)         1��������ģʽ��ʹ��SDIDAT[3]��

    DMA Enable      [15]    ʹ��DMA����DMA�������ʱ����λӦ����Ч��
    (EnDMA)                 0����Ч����ѯ�� 1��DMAʹ��

    Data Transfer   [14]    �������ݴ����Ƿ�ʼ����λ�Զ������
    Start(DTST)             0������׼���ã� 1�����ݿ�ʼ

    Data Transfer   [13:12] �������ݴ���ķ��� 00 =�޲����� 01 = ��æ���ģʽ
    Mode (DatMode)          10 =���ݽ���ģʽ��11 =���ݷ���ģʽ

    BlkNum          [11:0] ģ����(0~4095)������ģʽʱ������
*/
    rSDIDCON=(2<<22)|(1<<19)|(1<<17)|(1<<16)|(1<<14)|(2<<12)|(1<<0);
    //rSDIDCON��[11:0] ��������1����������block_addr�������ǵ�ַ��������������
    //�ƶ�9λ��512�ֽڡ�������block_addr�ͱ����"������"��
    rSDICARG=sector_addr<<9;    // CMD17/18(addr)

    //��ʼ׼����ȡ����

    rSDICCON=(0x1<<9)|(0x1<<8)|0x51;    // sht_resp, wait_resp, dat, start, CMD17
    if(!chk_cmd_end(17, 1)) //-- Check end of CMD17
        return ;
    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    while(rd_cnt<128)   // 128word=512byte
    {
        if((rSDIDSTA&0x20)==0x20) // Check timeout
        {
            rSDIDSTA=(0x1<<0x5);  // Clear timeout flag
            break;
        }
        status=rSDIFSTA;
        if((status&0x1000)==0x1000) // Is Rx data?
        {
            value = rSDIDAT;
            byte_conversion(&value,buf);
            *buf++;
            rd_cnt++;
        }
    }
    //-- Check end of DATA
    if(!chk_dat_end())
        return ;

    rSDIDCON=rSDIDCON&~(7<<12);
    rSDIFSTA=rSDIFSTA&0x200;    //Clear Rx FIFO Last data Ready, YH 040221
    rSDIDSTA=0x10;  // Clear data Tx/Rx end detect

}
//----дһ����--------------------------------------------------------
//���ܣ�������������������д��SD��ָ��ĳһ����
//������addr��������
//      buf��������
//���أ�
//---------------------------------------------------------------------
void write_one_sector(u32 sector_addr,unsigned int *buf)
{
    u32 value;
    int status;

    buf=(u32*)buf;//����FAT������u8���롣��֪�������Ƿ�ǿ��ת����
    wt_cnt=0;
    rSDIFSTA=rSDIFSTA|(1<<16);  //YH 040223 FIFO reset
    /*
    ���� [31:25]
    Burst4 enable   [24]    ��DMAģʽ��ʹ��Burst4���������ݴ�С����ʱ��λ��
     (Burst4)               ��λ��0����Ч 1��Burst4ʹ��

    Data Size       [23:22] ָ����FIFO����Ĵ�С���ĸ����ͣ����ֻ��֡�00 = �ֽ�
    (DataSize)              ���䣬01 =���ִ��� 10 = �ִ��䣬 11 = ����

    SDIO Interrupt  [21]    ����SDIO���ж������� 2�����ڻ����ⲿ�������ڣ������ݿ�
    Period Type(PrdType)    ��󱻷��ͣ���SDIO����0=������������ 1=�������ڣ������ڣ�

    Transmit After  [20]    �������ݴ�������Ӧ�յ���ʼ�򲻿�ʼ��0= ��DatMode���ú�ֱ��
    Response(TARSP)         1= ����Ӧ�յ��󣨼ٶ�����DatMode��Ϊ 2b11��

    Receive After   [19]    �������ݴ������������ʼ�򲻿�ʼ 0= ��DatMode���ú�ֱ��
    Command(RACMD)          1= ��������󣨼ٶ�����DatMode��Ϊ2b10��

    Busy After      [18]    ����æ�������������ʼ�򲻿�ʼ 0= ��DatMode���ú�ֱ��
    Command(BACMD)          1= ��������󣨼ٶ�����DatMode��Ϊ2b01��

    Block mode      [17]    ���ݴ���ģʽ 0=�����ݴ��� 1=ģ�����ݴ���
    (BlkMode)

    Wide bus        [16]    ����ʹ�ܿ�����ģʽ 0����׼����ģʽ����ʹ��SDIDAT[0]��
    enable(WideBus)         1��������ģʽ��ʹ��SDIDAT[3]��

    DMA Enable      [15]    ʹ��DMA����DMA�������ʱ����λӦ����Ч��
    (EnDMA)                 0����Ч����ѯ�� 1��DMAʹ��

    Data Transfer   [14]    �������ݴ����Ƿ�ʼ����λ�Զ������
    Start(DTST)             0������׼���ã� 1�����ݿ�ʼ

    Data Transfer   [13:12] �������ݴ���ķ��� 00 =�޲����� 01 = ��æ���ģʽ
    Mode (DatMode)          10 =���ݽ���ģʽ��11 =���ݷ���ģʽ

    BlkNum          [11:0] ģ����(0~4095)������ģʽʱ������
*/
    rSDIDCON=(2<<22)|(1<<20)|(1<<17)|(1<<16)|(1<<14)|(3<<12)|(1<<0);
    //������תΪ��ַ���ƶ�9λ����512�ֽڵ���˼
    rSDICARG=sector_addr<<9;        // CMD24/25(addr)
    rSDICCON=(0x1<<9)|(0x1<<8)|0x58;    //sht_resp, wait_resp, dat, start, CMD24
    if(!chk_cmd_end(24, 1)) //-- Check end of CMD24
        return;
    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    while(wt_cnt<128)
    {
        status=rSDIFSTA;
        if((status&0x2000)==0x2000)
        {
            byte_conversion(buf,&value);
            rSDIDAT=value;
            *buf++;
            wt_cnt++;
        }
    }
    if(!chk_dat_end())
        return;
    //Clear Data Transfer mode => no operation,Cleata
    //Data Transfer start
    rSDIDCON=rSDIDCON&~(7<<12);
    rSDIDSTA=0x10;  // Clear data Tx/Rx end
}

//----��������-----------------------------------------------------
//���ܣ��ж������Ƿ��ͳɹ�
//������cmd��sd������������ļ�ͷ��ע��[sd������˵��]
//      be_resp��1 ��Ӧ��0��Ӧ��
//���أ�1���ɹ�
//      0��time out
//---------------------------------------------------------------------

int chk_cmd_end(int cmd, int be_resp)
{
    int finish0;

    if(!be_resp)    // No response
    {
        finish0=rSDICSTA;
        while((finish0&0x800)!=0x800)   // Check cmd end
        finish0=rSDICSTA;

        rSDICSTA=finish0;// Clear cmd end state

        return 1;
    }
    else    // With response
    {
        finish0=rSDICSTA;
        // Check cmd/rsp end
        //debug �����bug�����û��SD������ô������������һֱѭ��
        //�п����޸�һ��
        while( !( ((finish0&0x200)==0x200) | ((finish0&0x400)==0x400) ))
        finish0=rSDICSTA;
        if((cmd==1) | (cmd==41))    // CRC no check, cmd9 is a long Resp. command.
        {
            if( (finish0&0xf00) != 0xa00 )  // Check error
            {
                rSDICSTA=finish0;   // Clear error state

                if(((finish0&0x400)==0x400))
                return 0;   // Timeout error
            }
            rSDICSTA=finish0;   // Clear cmd & rsp end state
        }
        else    // CRC check
        {
            if( (finish0&0x1f00) != 0xa00 ) // Check error
            {
                rSDICSTA=finish0;   // Clear error state

                if(((finish0&0x400)==0x400))
                    return 0;   // Timeout error
            }
            rSDICSTA=finish0;
        }
        return 1;
    }
}
//----������ݽ���-----------------------------------------------------
//���ܣ�������ݷ��ͻ��߽�����ϣ��Լ��Ƿ�ʱ
//������cmd��sd������������ļ�ͷ��ע��[sd������˵��]
//      be_resp���Ƿ��Ӧ
//���أ�1���ɹ�
//      0��ʧ�ܣ�
//---------------------------------------------------------------------
int chk_dat_end(void)
{
    int finish;

    finish=rSDIDSTA;
    while( !( ((finish&0x10)==0x10) | ((finish&0x20)==0x20) ))
        // Chek timeout or data end
        finish=rSDIDSTA;

    if( (finish&0xfc) != 0x10 )
    {
        rSDIDSTA=0xec;  // Clear error state
        return 0;
    }
    return 1;
}

//---ʹ������IDEL״̬-----------------------------------------------------
//���ܣ�ʹ������IDEL״̬
//��������
//���أ���
//---------------------------------------------------------------------

void cmd0(void)
{
    //-- Make card idle state
    rSDICARG=0x0;       // cmd0(stuff bit)
    rSDICCON=(1<<8)|0x40;   // No_resp, start, cmd0

    //-- Check end of cmd0
    chk_cmd_end(0, 0);
    rSDICSTA=0x800;     // Clear cmd_end(no rsp)
}
//---����Ƿ���MMC��---------------------------------------------------
//���ܣ�����Ƿ���MMC������������Ŀ�깤����ѹ
//��������
//���أ���
//---------------------------------------------------------------------
int chk_MMC_OCR(void)
{
    int i;

    for(i=0;i<100;i++)  //Negotiation time is dependent on CARD Vendors.
    {
        rSDICARG=0xff8000;              //CMD1(SD OCR:2.7V~3.6V)
        rSDICCON=(0x1<<9)|(0x1<<8)|0x41;    //sht_resp, wait_resp, start, CMD1
        //YH 0903 [31]:Card Power up status bit (busy)
        if(chk_cmd_end(1, 1) && (rSDIRSP0>>16)==0x80ff)
        {
            rSDICSTA=0xa00; // Clear cmd_end(with rsp)
            return 1;   // Success
        }
    }
    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    return 0;       // Fail
}
//---����Ƿ���SD��---------------------------------------------------
//���ܣ�����Ƿ���SD������������Ŀ�깤����ѹ
//��������
//���أ���
//---------------------------------------------------------------------
int chk_SD_OCR(void)
{
    int i;

    //-- Negotiate operating condition for SD, it makes card ready state
    for(i=0;i<50;i++)   //If this time is short, init. can be fail.
    {
        cmd55();    // Make ACMD

        rSDICARG=0xff8000;  //ACMD41(SD OCR:2.7V~3.6V)
        rSDICCON=(0x1<<9)|(0x1<<8)|0x69;//sht_resp, wait_resp, start, ACMD41

        //-- Check end of ACMD41
        if( (chk_cmd_end(41, 1) & rSDIRSP0)==0x80ff8000 )
        {
            rSDICSTA=0xa00; // Clear cmd_end(with rsp)

            return 1;   // Success
        }
        Djy_DelayUs(200000); // Wait Card power up status
    }
    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    return 0;       // Fail
}
//---��⿨�Ƿ����---------------------------------------------------
//���ܣ���⿨�Ƿ����
//��������
//���أ�1 �ɹ���0 ʧ��
//---------------------------------------------------------------------

int cmd55(void)
{
    //--Make ACMD
    rSDICARG=RCA<<16;           //CMD7(RCA,stuff bit)
    rSDICCON=(0x1<<9)|(0x1<<8)|0x77;    //sht_resp, wait_resp, start, cmd55

    //-- Check end of cmd55
    if(!chk_cmd_end(55, 1))
        return 0;

    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    return 1;
}
//---��ȡ����״̬---------------------------------------------------
//���ܣ���ȡ����״̬
//��������
//���أ�1 �ɹ�
//      0 ʧ��
//---------------------------------------------------------------------

int cmd13(void)//SEND_STATUS
{
    int response0;

    rSDICARG=RCA<<16;           // cmd13(RCA,stuff bit)
    rSDICCON=(0x1<<9)|(0x1<<8)|0x4d;    // sht_resp, wait_resp, start, cmd13

    //-- Check end of cmd13
    if(!chk_cmd_end(13, 1))
        return 0;
    //if(rSDIRSP0&0x100)
    response0=rSDIRSP0;
    response0 &= 0x3c00;
    response0 = response0 >> 9;
    if(response0==6)
        //Test_SDI();

    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    return 1;
}
//---��ȡ����CSD�Ĵ�����ֵ---------------------------------------------------
//���ܣ���ȡ����CSD�Ĵ�����ֵ
//��������
//���أ�1���ɹ� 0��ʧ��
//---------------------------------------------------------------------

int cmd9(void)//SEND_CSD
{
    rSDICARG=RCA<<16;               // cmd9(RCA,stuff bit)
    rSDICCON=(0x1<<10)|(0x1<<9)|(0x1<<8)|0x49;  // long_resp, wait_resp, start, cmd9

    if(!chk_cmd_end(9, 1))
        return 0;
    else
        return 1;
}
//---���ÿ�ͨ�ſ��---------------------------------------------------
//���ܣ����ÿ�ͨ�ſ��Ϊ4bit
//��������
//���أ���
//---------------------------------------------------------------------

void set_4bit_bus(void)
{
    cmd55();    // Make ACMD
    //-- CMD6 implement
    rSDICARG=1<<1;       //Wide 0: 1bit, 1: 4bit
    rSDICCON=(0x1<<9)|(0x1<<8)|0x46;    //sht_resp, wait_resp, start, cmd55
    if(!chk_cmd_end(6, 1))   // ACMD6
        return;
    rSDICSTA=0xa00;     // Clear cmd_end(with rsp)
}
//----SD����ʼ��--------------------------------------------------------
//���ܣ�sd����������ʼ����ʹSD�����Կ�ʼ����
//��������
//���أ�1���ɹ�
//      0��ʧ��
//��ע: MMC����Ҫע�⣬Cmd & Data ����ʹ�ܣ�����(������MMC��û���Թ�)
//---------------------------------------------------------------------

int module_init_SD(void)
{

    int i;

    RCA=0;
    MMC=0;
    //GPIO��ʼ��
    rGPEUP  = 0xf83f;     // SDCMD, SDDAT[3:0] => PU En.
    rGPECON = 0xaaaaaaaa;   //SDCMD, SDDAT[3:0]
    //����Ƶ��Ϊ400KHz
    rSDIPRE=CN_CFG_PCLK/(INICLK)-1;
    rSDICON=(1<<4)|1;   // Type B, clk enable
    rSDIFSTA=rSDIFSTA|(1<<16);  //YH 040223 FIFO reset
    rSDIBSIZE=0x200;        // 512byte(128word)
    rSDIDTIMER=0x7fffff;        // Set timeout count
    // Wait 74SDCLK for MMC card
    for(i=0;i<0x1000;i++);
    //��������0��ʹsd������idle״̬
    cmd0();

    //����Ƿ���MMC������������Ŀ�깤����ѹ
    //����MMC��Ϊ���Ժ����MMC
    if(chk_MMC_OCR())
    {
        MMC=1;
        goto RECMD2;
    }
    //����Ƿ���SD������������Ŀ�깤����ѹ
    if(!chk_SD_OCR())
        return 0;
RECMD2:
    rSDICARG=0x0;   // CMD2(stuff bit)
    rSDICCON=(0x1<<10)|(0x1<<9)|(0x1<<8)|0x42; //lng_resp, wait_resp, start, CMD2
    //����CMD2����ȡSD�������Ϣ������ʹ������identificationģʽ
    if(!chk_cmd_end(2, 1))
        goto RECMD2;
    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
RECMD3:
    //--Send RCA
    rSDICARG=MMC<<16;       // CMD3(MMC:Set RCA, SD:Ask RCA-->SBZ)
    rSDICCON=(0x1<<9)|(0x1<<8)|0x43;    // sht_resp, wait_resp, start, CMD3
    //����CMD3��SD����ȡRCA,����stand_by״̬
    if(!chk_cmd_end(3, 1))
        goto RECMD3;

    rSDICSTA=0xa00; // Clear cmd_end(with rsp)
    RCA=( rSDIRSP0 & 0xffff0000 )>>16;
    rSDIPRE=CN_CFG_PCLK/(SDCLK)-1;  // Normal clock=25MHz
    //--State(stand-by) check
    if( (rSDIRSP0 & 0x1e00) !=0x600 )  // CURRENT_STATE check
        goto RECMD3;
    card_sel_desel(1);  // Select
    set_4bit_bus();
    return 1;
}

void test_sdi()
{
    char buf[512] = "djyos test";
    char buf1[512];
    memset(buf1,0,512);
    write_one_sector(1, (unsigned int *)buf);
    read_one_sector(1, (unsigned int *)buf1);
}
