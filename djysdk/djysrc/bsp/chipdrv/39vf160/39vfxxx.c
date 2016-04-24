//----------------------------------------------------
//Copyright (C), 2004-2009,  �����߲���ϵͳ�����Ŷ�
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�����Ŷ�
//����ģ��:sst39vf160����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:sst39vf160 flash���������ṩflash��ԭʼ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "board-cfg.h"
#include "int.h"

#include "39vfxxx.h"

#define SST_ID                  0x00BF  // SST Manufacturer's ID code

#define SST_39VF160             0x2782
#define SST_39VF1601            0x234B
#define SST_39VF1602            0x234A
#define SST_39VF3201            0x235B
#define SST_39VF3202            0x235A
#define SST_39VF6401            0x236B
#define SST_39VF6402            0x236A

#define SST_39VF1601C           0x234F
#define SST_39VF1602C           0x234E
#define SST_39VF3201B           0x235D
#define SST_39VF3202B           0x235C
#define SST_39VF3201C           0x235F
#define SST_39VF3202C           0x235E
#define SST_39VF6401B           0x236D
#define SST_39VF6402B           0x236C

struct flash_info tg_flash_info;
static u16 cmd_erase_block,cmd_erase_sector;

void __get_device_id(u16 *mft_id,u16 *device_id)
{

    if((mft_id == NULL) || (device_id == NULL))
        return;
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0x90;
    Djy_DelayUs(1);   // Tida Max 150ns for 39VF640XB

    *mft_id  = Flash39VFxxxAddress[0];
    *device_id  = Flash39VFxxxAddress[1];

    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0xF0;
    Djy_DelayUs(1);   // Tida Max 150ns for 39VF640XB

    return ;
}

ptu32_t module_init_sst39vfxxx(ptu32_t para)
{
    Flash39VFxxxAddress = (u16 *)0x60000000;
    __get_device_id(&(tg_flash_info.mft_id),&(tg_flash_info.device_id));
    if(tg_flash_info.mft_id != SST_ID)
        return 0;
    switch(tg_flash_info.device_id)
    {
        case SST_39VF160:
        case SST_39VF1601:
        case SST_39VF1602:
        case SST_39VF3201:
        case SST_39VF3202:
        case SST_39VF6401:
        case SST_39VF6402:
            cmd_erase_sector = 0x30;
            cmd_erase_block = 0x50;
            break;
        case SST_39VF1601C:
        case SST_39VF1602C:
        case SST_39VF3201B:
        case SST_39VF3202B:
        case SST_39VF3201C:
        case SST_39VF3202C:
        case SST_39VF6401B:
        case SST_39VF6402B:
            cmd_erase_sector = 0x50;
            cmd_erase_block = 0x30;
            break;
    }
    return 0;
}
//----�ȴ��������------------------------------------------------------------
//���ܣ�����39vf160��toggle���ܣ�����ڲ�д������������ڽ��У��������ζ�������
//      ��ַ�����õ�0/1����Ľ����
//��������
//ȫ�֣�chip_addr��оƬ����ַ
//���أ�true=��ɲ�ѯ��false=��ʱ��������
//-----------------------------------------------------------------------------
bool_t wait_end_39vfxxx(void)
{
    static u32 i=0;
    volatile u16 a,b;
    u16 c;
    i++;
    for(c=0;c<700;c++)
    {
        a=*Flash39VFxxxAddress;
        b=*Flash39VFxxxAddress;
        if(a==b)
            return(true);
        Djy_DelayUs(100);   //��ʱ100uS
    }
    return(false);  //��70��������Ȼ������ɲ�ѯ�������
}
//----�����------------------------------------------------------------------
//���ܣ�����1��,39VF160ÿ64K�ֽ�Ϊ1�飬ʹ�õ��������м�Դ����
//������block_addr�����ַ��64K�ֽڶ��룬�������64K�����ַ�������Ե�16λ��ַ��
//ȫ�֣�chip_addr��оƬ����ַ
//���أ�true=�ɹ�������false=ʧ�ܣ�Ӧ�����Ϊ���顣nor flash���ڲ���ʱ��⻵
//      �飬������д��ض��ķ����������Ƿ���true
//----------------------------------------------------------------------------
bool_t block_erase_39vfxxx(u16 block_no)
{
    atom_low_t atom_low;
    u16 *block;
    if(block_no >= flash_block_sum)
        return false;
    block=(u16*)(block_no*flash_block_length+flash_base_addr);
    wait_end_39vfxxx();   //�ȴ���һ�β������
    atom_low = Int_LowAtomStart();  //�����в��ܱ���Դ������������������
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0x80;
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;

    *block = cmd_erase_block;
//    wait_end_39vfxxx(); //���ȴ�������ɣ���������һ��ʹ��ǰ�ȴ�
                                    //�������Խ�ʡʱ�䡣
    Int_LowAtomEnd(atom_low);
    return true;
}
//----��������-----------------------------------------------------------------
//���ܣ�����һ������,39VF160ÿ4K�ֽ�Ϊ1����.
//������start�� ������ַ��4K�ֽڶ��룬�������4K�����ַ�������Ե�12λ��ַ��
//ȫ�֣�chip_addr��оƬ����ַ
//���أ�true=�ɹ�������false=ʧ�ܣ�Ӧ�����Ϊ���顣nor flash���ڲ���ʱ��⻵
//      �飬������д��ض��ķ����������Ƿ���true
//----------------------------------------------------------------------------
bool_t sector_erase_39vfxxx(u16 sector_no)
{
    atom_low_t atom_low;
    u16 *sector;
    if(sector_no >= flash_sector_sum)
        return false;
    sector=(u16*)(sector_no*flash_sector_length+flash_base_addr);
    wait_end_39vfxxx();   //�ȴ���һ�β������
    atom_low = Int_LowAtomStart();  //�����в��ܱ���Դ������������������
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0x80;
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    *sector = cmd_erase_sector;
//    wait_end_39vfxxx(); //���ȴ�������ɣ���������һ��ʹ��ǰ�ȴ�
                                    //�������Խ�ʡʱ�䡣
    Int_LowAtomEnd(atom_low);
    return true;
}

//----������Ƭflash------------------------------------------------------------
//���ܣ�������Ƭflash,���:��ǰ������flash��ַ
//��������
//ȫ�֣�chip_addr��оƬ����ַ
//���أ���
//-----------------------------------------------------------------------------
void chip_erase_39vfxxx(void)
{
    atom_low_t atom_low;
    wait_end_39vfxxx();
    atom_low = Int_LowAtomStart();  //�����в��ܱ���Դ������������������
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0x80;
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0x10;
    Int_LowAtomEnd(atom_low);
//    wait_end_39vfxxx();
}

//----дһ���ֽ�--------------------------------------------------------------
//���ܣ�дFlashһ���ֽ�,д���ַӦ�����Ѿ�������
//������addr,�ֶ����д���ַ,
//      Data:д�����
//ȫ�֣�chip_addr��оƬ����ַ
//���أ���
//��ע���������Ĵ��ڱ�Ҫ���д���ȶ����дflash����Ҫʱ�仨�ڵȴ�flash�����ϣ�
//      �ʱ������������÷ǳ����ޣ��������Ű�
//---------------------------------------------------------------------------
bool_t byte_write_39vfxxx(u8 *addr,u8 data)
{
    bool_t result;
    u16 wd;
    u16 *wd_addr;
    wait_end_39vfxxx();
    Int_SaveAsynSignal();      //��ֹ����Ҳ���ǽ�ֹ�첽�¼�
    wd_addr =(u16*)((u32)addr&(~1));
    if((u32)addr&1)
        wd = (data<<8) + 0xff;
    else
        wd = data+0xff00;
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0xa0;
    *wd_addr=wd;
    wait_end_39vfxxx();
    if((u32)addr&1)
        result = (u8)(*wd_addr>>8) == data;
    else
        result = (u8)*wd_addr == data;
    Int_RestoreAsynSignal();
    return result;
}

//----дһ����----------------------------------------------------------------
//���ܣ�дFlashһ����,д���ַӦ�����Ѿ�������
//������addr,�ֶ����д���ַ,
//      Data:д�����
//ȫ�֣�chip_addr��оƬ����ַ
//���أ���
//��ע���������Ĵ��ڱ�Ҫ���д���ȶ����дflash����Ҫʱ�仨�ڵȴ�flash�����ϣ�
//      �ʱ������������÷ǳ����ޣ��������Ű�
//---------------------------------------------------------------------------
bool_t word_write_39vfxxx(u16 * addr,u16 data)
{
    bool_t result;
    wait_end_39vfxxx();
    Int_SaveAsynSignal();      //��ֹ����Ҳ���ǽ�ֹ�첽�¼�
    Flash39VFxxxAddress[0x5555]=0xaa;
    Flash39VFxxxAddress[0x2aaa]=0x55;
    Flash39VFxxxAddress[0x5555]=0xa0;
    *addr=data;
    wait_end_39vfxxx();
    result = (*addr == data);
    Int_RestoreAsynSignal();
    return result;
}

//----дһ������--------------------------------------------------------------
//���ܣ�дflashһ��������У��,���������Ѿ�������
//������sector_no:������.
//      Buf:д������.
//ȫ�֣���
//���أ�true=У����ȷ��false=У�������ַԽ��
//----------------------------------------------------------------------------
bool_t sector_write_39vfxxx(u16 sector_no,u8 *buf)
{
    int i;
    u16 *sector;
    u16 wd;
    if(sector_no >= flash_sector_sum)
        return false;
    sector=(u16*)(sector_no*flash_sector_length+flash_base_addr);
    wait_end_39vfxxx();
    for(i=0;i<flash_sector_length/2;i++)
    {
        wd = buf[2*i] + (buf[2*i+1]<<8);    //buf���ܲ����ֶ���ĵ�ַ
        word_write_39vfxxx(&sector[i],wd);
    }
    wait_end_39vfxxx();
    for(i=0;i<flash_sector_length/2;i++)
    {//�ر���ʾ��39vf160�Ǵ�16λ�ģ��������ֽڷ��ʣ���������ֱȽϡ�
        wd = buf[2*i] + (buf[2*i+1]<<8);    //buf���ܲ����ֶ���ĵ�ַ
        if(sector[i] != wd)
            return false;
    }
    return true;
}

//----дһ����----------------------------------------------------------------
//дflashһ����,�ÿ����Ѿ�������
//������block_no:���
//      Buf:д������
//ȫ�֣���
//���أ�true=У����ȷ��false=У�������ַԽ��
//----------------------------------------------------------------------------
bool_t block_write_39vfxxx(u16 block_no,u8 *buf)
{
    int i;
    u16 *block;
    u16 wd;
    if(block_no >= flash_block_sum)
        return false;
    block=(u16*)(block_no*flash_block_length+flash_base_addr);
    wait_end_39vfxxx();

    for(i=0;i<flash_block_length/2;i++)
    {
        wd = buf[2*i] + (buf[2*i+1]<<8);    //buf���ܲ����ֶ���ĵ�ַ
        word_write_39vfxxx(&block[i],wd);
    }
    wait_end_39vfxxx();
    for(i=0;i<flash_block_length/2;i++)
    {//�ر���ʾ��39vf160�Ǵ�16λ�ģ��������ֽڷ��ʣ���������ֱȽϡ�
        wd = buf[2*i] + (buf[2*i+1]<<8);    //buf���ܲ����ֶ���ĵ�ַ
        if(block[i] != wd)
            return false;
    }
    return true;
}

//----дflash---------------------------------------------------------------
//���ܣ�дFlash,д�����������Ȳ���
//������start����ʼ��ַ.
//      len:д�볤��,
//      buf:��д������ݻ�����
//ȫ�֣�chip_addr��оƬ����ַ
//���أ�true=��ȷд�룬false=У�������ַԽ�硣
//--------------------------------------------------------------------------
//ע�ͱ�ע������δ�����Ĵ��롣
//void write_flash(u8 * start,u32 length,u8 * buf)
//{
//    u16 * pBuf,*pFlash;
//    u32 i,j;
//    i=length;
//    if((u32)buf&1)
//    {
//        pFlash=(u16*)((u32)start-1);
//        byte_write_39vfxxx(pFlash,(*buf<<8)+0xff); //дδ��׼�ĵ�һ���ֽ�
//        pBuf=(u16*)((u32)buf+1);    //ʹ��ַ�ֶ�׼
//        i--;    //��ʼ��ַδ�ֶ�׼,�Ѿ�д��1�ֽ�.���ȼ�1�ֽ�
//    }else
//    {
//        pBuf=(u16*)((u32)buf);
//        pFlash=(u16*)((u32)start);
//    }
//    if (i&1)
//    {
//        byte_write_39vfxxx(&start[i-1],buf[length]+0xff00); //дδ��׼�����һ���ֽ�
//        i--;    //���ȼ�1��δ�ֶ�׼,�Ѿ�д��1�ֽ�.���ȼ�1�ֽ�
//    }
//    //����,iΪż��,��Temp�ֶ�׼��ַ
//    i=i/2;
//    for(j=0;j<i;j++)
//    {
//        byte_write_39vfxxx(&pFlash[j],pBuf[j]);
//    }
//}

//----��дflash---------------------------------------------------------------
//���ܣ���дFlash,�Զ��ж��Ƿ���Ҫ����
//������start����ʼ��ַ.
//      len:д�볤��,
//      buf:��д������ݻ�����
//ȫ�֣�chip_addr��оƬ����ַ
//���أ�true=��ȷд�룬false=У�������ַԽ�硣
//--------------------------------------------------------------------------
bool_t modify_flash(u8 * start,u32 length,u8 * buf)
{
    union
    {
        u16 buf_u16[flash_sector_length/2];
        u8 buf_u8[flash_sector_length];
    }tempbuf;
    u32 odd_len,i;
    u16 sector_offset,sector_no;
    u16 *sector_addr;

    if((u32)start+length > flash_base_addr+flash_size)
        return false;
    sector_addr=(u16*)((u32)start&(~(flash_sector_length-1)));
    sector_no = ((u32)sector_addr-flash_base_addr)/flash_sector_length;
    sector_offset=(u32)start&(flash_sector_length-1);
    if(sector_offset != 0)
        odd_len=flash_sector_length-sector_offset;
    else
        odd_len = 0;
    if(sector_offset != 0)
    {
        //������ʼ��ַ���������б���д���������ǵ����ݵ�������
        for (i=0;i<flash_sector_length/2;i++)
            tempbuf.buf_u16[i]=sector_addr[i];
        //�����û�а�0�ĳ�1��
        for(i=sector_offset;(i<flash_sector_length)&&(i-sector_offset<length);i++)
        {
            if((tempbuf.buf_u8[i] | buf[i-sector_offset]) != tempbuf.buf_u8[i])
            {//����д�0�ĳ�1��λ����������
                sector_erase_39vfxxx(sector_no);
                break;
            }
        }
        //��д���������д����ʼ������������
        for (i=sector_offset;(i<flash_sector_length)&&(i-sector_offset<length);i++)
                    tempbuf.buf_u8[i]=buf[i-sector_offset];
        //д����ʼ����
        if( !sector_write_39vfxxx(sector_no,tempbuf.buf_u8) )
            return false;
        odd_len = 0;
        sector_no++;
        sector_addr += flash_sector_length/2;
    }
    //д�������м�����
    while(odd_len+flash_sector_length<=length)
    {
        u16 wd1,wd2;
        for(i=0;i<flash_sector_length/2;i++)
        {
            wd1 = sector_addr[i];
            wd2 = buf[odd_len+2*i] + (buf[odd_len+2*i+1]<<8);//������ֶ���ĵ�ַ
            if((wd1 | wd2) != wd1)
            {//����д�0�ĳ�1��λ����������
                sector_erase_39vfxxx(sector_no);
                break;
            }
        }
        if( !sector_write_39vfxxx(sector_no,&buf[odd_len]) )
            return false;
        odd_len+=flash_sector_length/2;
        sector_no++;
        sector_addr += flash_sector_length/2;
    }
    //д���һҳ
    if(odd_len<length)
    {
        sector_addr+=flash_sector_length;
        sector_no++;
        for (i=0;i<flash_sector_length/2;i++)
            tempbuf.buf_u16[i]=sector_addr[i];
        for(i=0;i<(length-odd_len);i++)
        {
            if((tempbuf.buf_u8[i] | buf[i+odd_len]) != tempbuf.buf_u8[i])
            {//����д�0�ĳ�1��λ����������
                sector_erase_39vfxxx(sector_no);
                break;
            }
        }
        for(i=0;i+odd_len<length;i++)
            tempbuf.buf_u8[i]=buf[i+odd_len];
        if( !sector_write_39vfxxx(sector_no,tempbuf.buf_u8) )
            return false;
    }
    return true;
}


