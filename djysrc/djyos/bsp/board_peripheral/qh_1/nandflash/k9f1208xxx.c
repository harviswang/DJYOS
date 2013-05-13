//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: �ļ�ϵͳоƬ����
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: ��������nand flash���ļ�ϵͳ����ģ��
//����˵��:
//�޶���ʷ:
//2. ����:20090131
//   ����:lst
//   �°汾�ţ�v1.1.0
//   �޸�˵��: ����ö�ʱ���ж����ȴ�������ɵĴ��롣
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "os_inc.h"
#include "cpu_peri.h"
#include <string.h>
#include "stm32f10x.h"
#include "k9f1208xxx.h"

struct flash_chip tg_samsung_nand;    //оƬ

u8 *pg_nand_cmd = (u8*)(Bank_NAND_ADDR | CMD_AREA);
u8 *pg_nand_addr = (u8*)(Bank_NAND_ADDR | ADDR_AREA);
u8 *pg_nand_data = (u8*)(Bank_NAND_ADDR);

#define ce_active() 
#define ce_inactive() 

//����������ָ�룬������д�����ڲ�ר�ã�djyffs�У�������������flash�Ĺ��ԣ��ʲ�
//������struct flash_chip �ṹ�С���̬�����ڴ��������ɾ�̬���飬����Ϊchip��
//Ϊһ����Դ���ǿ���ɾ���ģ�ɾ��ʱ���Ի�����Դ��
static u8 *pg_sector_buf;      //����������ָ��
static u32 u32g_sectors_per_block; //ÿ�������������
static u32 u32g_sector_size;      //�������ߴ�
static u16 u16g_oob_size;

struct nand_table tg_nand_table[] =
{
    {0x9876,16,32,4096,16384,"Toshiba TH58512FT,1.8v,64Mbytes"},
    {0xec36,16,32,4096,16384,"samsung k9f1208,1.8v,64Mbytes"},
    {0xec76,16,32,4096,16384,"samsung k9f1208,3.3v,64Mbytes"},
    {0xec73,16,32,1024,16384,"samsung k9f2808,3.3v,16Mbytes"},
    {0xec33,16,32,1024,16384,"samsung k9f2808,1.8v,16Mbytes"},
    {0xecda,64,64,2048,131072,"samsung K9F2G08,3.3v,256Mbytes"}
};
//----д������-----------------------------------------------------------------
//����: д��оƬд������
//����: val��д�����֣�
//����: ��
//-----------------------------------------------------------------------------
#define __write_command_nand(cmd)   *pg_nand_cmd = cmd 
//----д���ַ-----------------------------------------------------------------
//����: д��оƬд������,5�зֱ�д��A0 ~ A7��A9 ~ A11��A12 ~ A19,A20~A27
//����: val��д�����֣�
//����: ��
//----------------------------------------------------------------------------
void __write_address_nand(uint32_t addr)   
{
    *pg_nand_addr = (u8)addr;        
    *pg_nand_addr = (u8)(addr>>9);   
    *pg_nand_addr = (u8)(addr>>17);  
    *pg_nand_addr = (u8)(addr>>25);  

}

//----д����ָ���-------------------------------------------------------------
//����: ��оƬ����д������ʱ������߸�λ����ʹ����д��Ŀ����ݶ�ʧ��Ϊ���������
//      �±������ݣ����õĲ�����:
//      1.���д������¿飬��ֱ��д�룬��Ϊд���¿�ʧ�ܺ�д�����ָ���ʧ�ܵ�
//        �����һ���ġ�
//      2.������޸�һ����ߴ�һ�����������ݣ����������д�뵽����ָ��飬��
//        �ڵ���ָ�����ʵ�λ�ñ�������ָ��鱣��������һ�����ݡ�
//      3.д�����ָ���󣬽��Ű�����д�뵽Ŀ����У���ɺ󣬲�������ָ��顣
//      4.���������������⵽����ָ�������Ч���ݣ���ָ���Ŀ���
//      ��������ְ��:�����������£����ñ�����������д�����ָ��顣
//����: PCRB_block,���ڱ������ָ����ݵĿ�ţ�оƬ�ľ��Կ��
//      protected_block����������Ŀ���ţ�оƬ�ľ��Կ�š�
//����: true = �ɹ�д�룬false = д��ʧ�ܣ����ܵ�PCRB_block���ǻ��顣
//-----------------------------------------------------------------------------
bool_t write_PCRB_nand(u32 PCRB_block,
                       u32 protected_block,u8 *buf)
{
    u32 cur_sector;    //��ǰ����д������
    u32 completed = 0;
    u32 verify;
    u32 address;
    u32 loop;

    if(PCRB_block >= tg_samsung_nand.block_sum)
        return false;
    cur_sector = PCRB_block * u32g_sectors_per_block;
    for(loop = 0; loop < u32g_sectors_per_block; loop++)
    {
        verify = __write_sector_nand_with_ecc(cur_sector,0,
                                  buf+completed,u32g_sector_size);
        if((verify == cn_all_right_verify) || (verify == cn_ecc_right_verify))
        {
            completed += u32g_sector_size;
        }else
            return false;
        cur_sector++;
    }
    //��һҳ�����4�ֽڱ����ܱ�����Ŀ���š�
    address =tg_samsung_nand.block_size*PCRB_block + 12;
    ce_active();                        //����Ƭѡ
    __write_command_nand(cn_nand_select_oob);    //����oobҳ
    __write_command_nand(cn_nand_page_program); //�����������
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    //�ܱ�����Ŀ����д��flash��ע��:û����ECCУ�飬�Ժ���---db
    *pg_nand_data = (u8)protected_block;
    *pg_nand_data = (u8)(protected_block>>8);
    *pg_nand_data = (u8)(protected_block>>16);
    *pg_nand_data = (u8)(protected_block>>24);
    __write_command_nand(cn_nand_startup_write);  //����оƬ�ڲ�д�����
    __wait_ready_nand_slow(cn_wait_page_write);   //�ȴ�оƬ�ڲ��������

    if(__read_status_nand() & cn_nand_failure)
    {
        ce_inactive();
        return false;
    }
    ce_inactive();

    return true;
}

//----�ӵ���ָ���ָ�����-----------------------------------------------------
//����: �ӵ���ָ���ָ����ݣ��������ָ���������Ч���ݣ��Ƚ�����Ŀ����������
//      ��һ�£������һ�����õ���ָ�������ݸ���Ŀ��顣�����Ƿ���Ҫ�ָ������
//      ����������ָ��顣
//����: PCRB_block,����ָ����ţ�оƬ�ľ��Կ��
//      restored����ָ�뷵�ر��ָ���Ŀ���
//����: true = ����ָ�������ȷ�ָ���false = ��������һ������ΪĿ����ǻ��顣
//-----------------------------------------------------------------------------
bool_t restore_PCRB_nand(u32 PCRB_block,u32 *restored)
{
    u32 verify;
    u32 address;
    u32 loop;
    u32 protected_block;
    u32 sector_from,sector_to;
    u8 *sector_buf;   //��������������̬���䣬����ջ�з���
    bool_t result = true;

    if(PCRB_block >= tg_samsung_nand.block_sum)
        return false;
    //��һҳ�����4�ֽڱ����ܱ�����Ŀ���š�
    address =tg_samsung_nand.block_size*PCRB_block + 12;
    ce_active();                        //����Ƭѡ
    __write_command_nand(cn_nand_select_oob);    //����oobҳ
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    //�ܱ�����Ŀ����д��flash��ע��:û����ECCУ�飬�Ժ���---db
    protected_block = *pg_nand_data;
    protected_block += (u32)(*pg_nand_data)<<8;
    protected_block += (u32)(*pg_nand_data)<<16;
    protected_block += (u32)(*pg_nand_data)<<24;
    *restored = protected_block;

    ce_inactive();
    if(protected_block > tg_samsung_nand.block_sum)
    {
        erase_block_nand(PCRB_block);
        return true;
    }
    sector_buf = m_malloc(u32g_sector_size,0);
    if(sector_buf == NULL)
        return false;
    erase_block_nand(protected_block);
    sector_from = PCRB_block * u32g_sectors_per_block;
    sector_to = protected_block * u32g_sectors_per_block;
    for(loop = 0; loop < u32g_sectors_per_block; loop++)
    {
        verify = __read_sector_nand_with_ecc(
                        sector_from,0,sector_buf,u32g_sector_size);
        if((verify != cn_all_right_verify) && (verify != cn_ecc_right_verify))
        {
            result = false;
            break;
        }
        verify = __write_sector_nand_with_ecc(
                                    sector_to,0,sector_buf,u32g_sector_size);
        if((verify != cn_all_right_verify) && (verify != cn_ecc_right_verify))
        {
            result = false;
            break;
        }
        sector_from++;
        sector_to++;
    }
    m_free(sector_buf);
    erase_block_nand(PCRB_block);
    return result;
}

//----�ȴ�оƬ�ڲ���ɲ���----------------------------------------------------
//����: ��оƬִ��д������ҪЧ�ñ������ȴ�������ɲ��ܽ�һ��������
//����: ��
//����: true = ��ȷ��ɲ�����false = ��������
//-----------------------------------------------------------------------------
bool_t __wait_ready_nand(void)
{
    u32 timestart,timeend;

    timestart = djy_get_fine_time( );
    timeend = timestart;


    while ( (timeend - timestart) < 100)
    {
      if(GPIO_ReadInputDataBit(GPIOD, NandFlash_RB_GPIOD6))
       {
     	  break;
       }
      timeend = djy_get_fine_time( );
    }

    if( (timeend - timestart) >= 100)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//----�ȴ�оƬ�ڲ����ٲ������-------------------------------------------------
//����: ��оƬִ��д������Ҫ���ñ������ȴ�������ɲ��ܽ�һ������������һ������
//      ͬ���������˷Ѻܶ�ʱ�䣬����һЩ�ܿ�����Ĳ�������ʹ��__wait_ready_nand
//      ������
//����: wait_time�����Ƶȴ�ʱ�䣬΢����
//����: true = ��ȷ��ɲ�����false = ��������
//ע:�˴�Ӧ��Ϊ�ö�ʱ��
//-----------------------------------------------------------------------------
bool_t __wait_ready_nand_slow(u16 wait_time)
{

//    timer_set_counter(1,wait_time);     //����ֵ��Ϊwait_time
//    timer_reload(1);                    //���ض�ʱֵ
//    timer_start(1);                     //������ʱ��
//    int_asyn_signal_sync(cn_int_line_timer1);
//    timer_stop(1);
//    while((pg_nand_reg->NFSTAT & 1) ==0 );

//    return true;
    
    u32 timestart,timeend;

    timestart = djy_get_fine_time( );
    timeend = timestart;


    while ( (timeend - timestart) < wait_time)
    {
      if(GPIO_ReadInputDataBit(GPIOD, NandFlash_RB_GPIOD6))
       {
     	  break;
       }
      timeend = djy_get_fine_time( );
    }

    if( (timeend - timestart) >= wait_time)
    {
        return false;
    }
    else
    {
        return true;
    }
}
//----������(��eccУ��)----------------------------------------------------
//����: ����eccУ���һ�����ڶ�ȡ���ݣ���ַ���ܿ������߽�
//����: sector��������
//      offset��������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//����: ��ȷ��ȡ
//-----------------------------------------------------------------------------
u32 __read_sector_nand_no_ecc(u32 sector,u32 offset,
                                 u8 *data,u32 size)
{
    u32 i;
    u32 address;
    address =u32g_sector_size*sector + offset;     //����ʵ�ʵ�ַ
    ce_active();                        //����Ƭѡ
    if(address & 0x100)
        __write_command_nand(cn_nand_select_page1);    //д���ģʽ����
    else
        __write_command_nand(cn_nand_select_page0);    //д���ģʽ����
    __write_address_nand(address);

    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i < size; i++)
    {
        data[i] = *pg_nand_data;     //��ȡ����
    }
    ce_inactive();                      //�ر�Ƭѡ
    return cn_all_right_verify;
}

//----����(��eccУ��)----------------------------------------------------
//����: ����eccУ���һ���ڶ�ȡ���ݣ���ַ���ܿ��߽�
//����: sector��������
//      offset������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//����: ��ȷ��ȡ
//-----------------------------------------------------------------------------
u32 read_block_ss_no_ecc(u32 block,u32 offset,
                         u8 *buf,u32 size)
{
    u32 start_sector;  //��������
    u32 start_offset;  //�׵�ַ����������ƫ����
    u32 end_sector;    //������ַ��������
    u32 end_offset;    //������ַ�������е�ƫ����
    u32 cur_sector;    //��ǰ���ڶ�������
    u32 read_size;     //�ӵ�ǰ������ȡ��������
    u32 completed = 0;

    if(block >= tg_samsung_nand.block_sum)
        return cn_limit_uint32;
    if((size + offset) > tg_samsung_nand.block_size)
        return cn_limit_uint32;
    if(size == 0)
        return 0;

    //��ʼ������
    start_sector = offset / u32g_sector_size + u32g_sectors_per_block * block;
    //��ʼ��ַ����ʼ�������е�ƫ����
    start_offset = offset % u32g_sector_size;
    //����������
    end_sector =(offset + size-1)/u32g_sector_size+u32g_sectors_per_block*block;
    //������ַ�ڽ��������е�ƫ����
    end_offset = (offset + size -1) % u32g_sector_size;
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        if(cur_sector != end_sector)    //��ǰ�����������һ������
            read_size = u32g_sector_size - start_offset;
        else    //��ǰ���������һ������
            //+1����Ϊend_offset��������Ҫд���
            read_size = end_offset - start_offset +1;
        __read_sector_nand_no_ecc(cur_sector,start_offset,
                                  buf+completed,read_size);
        completed += read_size;
        start_offset = 0;   //�ӵڶ���������ʼ���϶���0��ʼ��
    }
    return completed;
}

//----ECC��鲢����-----------------------------------------------------------
//����: eccУ��һ������
//����: data����У�����ݵĻ�����
//      ecc��У����
//���أ�0=��ȷ��1=��������2=�����Ҳ�������
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
u32 __correct_sector(u8 *data,const u8 *ecc)
{
    s32 j;
    u32 i,result = cn_all_right_verify;
    for(i = 0; i < u32g_sector_size/256; i++)
    {
        j = ecc_corect_256(data + i*256, ecc + i*3);
        if(( j == -1) || (j == 2))
            return cn_ecc_error_verify;   //�޷�������ֱ�ӷ���
        if( j == 1)
            result = cn_ecc_right_verify;
    }
    return result;
}

//----������(��eccУ��)----------------------------------------------------
//����: ��eccУ���һ�����ڶ�ȡ���ݣ���ַ���ܿ������߽�
//����: sector��������
//      offset��������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//���أ�0=��ȷ��1=��������2=�����Ҳ�������
//-----------------------------------------------------------------------------
u32 __read_sector_nand_with_ecc(u32 sector,u32 offset,
                                     u8 *data,u32 size)
{
    u32 i;
    u32 address,result;
    u8 *ecc;
    address =u32g_sector_size*sector;     //����ʵ�ʵ�ַ
    ecc = pg_sector_buf+u32g_sector_size;
    ce_active();                        //����Ƭѡ
    __write_command_nand(cn_nand_select_page0);    //д���ģʽ����
    __write_address_nand(address);

    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i < u32g_sector_size; i++)
    {
        pg_sector_buf[i] = *pg_nand_data;     //��ȡ����
    }
    if(u32g_sector_size > 256)
    {
        for(i = 0; i < 5; i++)              //��ȡУ���룬�����־ǰ��5�ֽ�
            ecc[i] = *pg_nand_data;
        ecc[i] = *pg_nand_data;          //�����־����ȡ������
        for(; i < u32g_sector_size/256*3; i++)//��ȡУ���룬�����־��Ĳ���
            ecc[i] = *pg_nand_data;
    }else
    {
        for(i = 0; i < 3; i++)              //��ȡУ���룬ֻ��3�ֽ�
            ecc[i] = *pg_nand_data;
    }
    ce_inactive();                      //�ر�Ƭѡ
    result = __correct_sector(pg_sector_buf,ecc);
    //����У������Σ���ִ������copy����ʹ����Ҳ�Ѵ������ݸ����û�������֪��
    //�������
    memcpy(data, pg_sector_buf + offset, size);
    return result;
}

//----����(��eccУ��)----------------------------------------------------
//����: ��eccУ���һ���ڶ�ȡ���ݣ���ַ���ܿ��߽�
//����: sector�����
//      offset������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//���أ�ʵ�ʶ�ȡ��������
//-----------------------------------------------------------------------------
u32 read_block_ss_with_ecc(u32 block,u32 offset,
                         u8 *buf,u32 size)
{
    u32 start_sector;  //��������
    u32 start_offset;  //�׵�ַ����������ƫ����
    u32 end_sector;    //������ַ��������
    u32 end_offset;    //������ַ�������е�ƫ����
    u32 cur_sector;    //��ǰ���ڶ�������
    u32 read_size;     //�ӵ�ǰ������ȡ��������
    u32 completed = 0;
    u32 verify;

    if(block >= tg_samsung_nand.block_sum)
        return cn_limit_uint32;
    if((size + offset) > tg_samsung_nand.block_size)
        return cn_limit_uint32;
    if(size == 0)
        return 0;

    //��ʼ������
    start_sector = offset / u32g_sector_size + u32g_sectors_per_block * block;
    //��ʼ��ַ����ʼ�������е�ƫ����
    start_offset = offset % u32g_sector_size;
    //����������
    end_sector =(offset + size-1)/u32g_sector_size+u32g_sectors_per_block*block;
    //������ַ�ڽ��������е�ƫ����
    end_offset = (offset + size -1) % u32g_sector_size;
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        if(cur_sector != end_sector)    //��ǰ�����������һ������
            read_size = u32g_sector_size - start_offset;
        else    //��ǰ���������һ������
            //+1����Ϊend_offset��������Ҫ��ȡ��
            read_size = end_offset - start_offset +1;
        verify = __read_sector_nand_with_ecc(cur_sector,start_offset,
                                  buf+completed,read_size);
        if((verify == cn_all_right_verify) || (verify == cn_ecc_right_verify))
        {
            completed += read_size;
            start_offset = 0;   //�ӵڶ���������ʼ���϶���0��ʼ��
        }else
            break;
    }
    return completed;
}

//----��оƬ״̬-------------------------------------------------------------
//����: ��оƬ״̬
//����: ��
//���أ�оƬ״̬
//-----------------------------------------------------------------------------
u8 __read_status_nand(void)
{
    u8 chip_status ;

    __write_command_nand(cn_nand_read_status);
    chip_status = *pg_nand_data;

    return chip_status;
}

//----дһ������(no ecc)-------------------------------------------------------------
//����: дһ������������eccУ��
//����: sector��������
//      offset��������ƫ�Ƶ�ַ
//      data�����ݻ�����
//      size��д��ĳߴ�
//���أ�cn_all_right_verify=��ȷд�룬cn_ecc_error_verify=д�����
//-----------------------------------------------------------------------------
u32 __write_sector_nand_no_ecc(u32 sector,u32 offset,
                                  u8 *data,u32 size)
{
    u32 i;
    u32 address;
    address =u32g_sector_size*sector + offset;     //����ʵ�ʵ�ַ
    ce_active();                        //����Ƭѡ
    if(address & 0x100)//256,������1�Ļ����Ǵ�256�ֽڿ�ʼ��ȡ
        __write_command_nand(cn_nand_select_page1);    //д���ģʽ����
    else
        __write_command_nand(cn_nand_select_page0);    //д���ģʽ����
    __write_command_nand(cn_nand_page_program); //�����������
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i < size; i++)
    {//����Ѵ�д�������д�뵽����������������
        *pg_nand_data = data[i];
       // printf("WriteFlash: data = 0x%x\n", data);
    }
    __write_command_nand(cn_nand_startup_write);  //����оƬ�ڲ�д�����
    __wait_ready_nand_slow(cn_wait_page_write);  //�ȴ�оƬ�ڲ��������

    if(__read_status_nand() & cn_nand_failure)
    {
        ce_inactive();
        return cn_ecc_error_verify;
    }
    ce_inactive();
    return cn_all_right_verify;
}

//----д��(����eccУ��)----------------------------------------------------
//����: ����eccУ��ѻ�����д����ڣ���ַ���ܿ��߽�
//����: sector�����
//      offset������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//���أ�ʵ��д�����������cn_limit_uint32��ʾ����
//-----------------------------------------------------------------------------
u32 write_block_ss_no_ecc(u32 block,u32 offset,
                               u8 *buf,u32 size)
{
    u32 start_sector;  //��������
    u32 start_offset;  //�׵�ַ����������ƫ����
    u32 end_sector;    //������ַ��������
    u32 end_offset;    //������ַ�������е�ƫ����
    u32 cur_sector;    //��ǰ���ڶ�������
    u32 write_size;    //�ӵ�ǰ������ȡ��������
    u32 completed = 0;
    u32 verify;

    if(block >= tg_samsung_nand.block_sum)
        return cn_limit_uint32;
    if((size + offset) > tg_samsung_nand.block_size)
        return cn_limit_uint32;

    //��ʼ������
    start_sector = offset / u32g_sector_size + u32g_sectors_per_block * block;
    //��ʼ��ַ����ʼ�������е�ƫ����
    start_offset = offset % u32g_sector_size;
    //����������
    end_sector =(offset + size-1)/u32g_sector_size+u32g_sectors_per_block*block;
    //������ַ�ڽ��������е�ƫ����
    end_offset = (offset + size -1) % u32g_sector_size;
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        if(cur_sector != end_sector)    //��ǰ�����������һ������
            write_size = u32g_sector_size - start_offset;
        else    //��ǰ���������һ������
            //+1����Ϊend_offset��������Ҫд���
            write_size = end_offset - start_offset +1;
        verify = __write_sector_nand_no_ecc(cur_sector,start_offset,
                                  buf+completed,write_size);
        if((verify == cn_all_right_verify) || (verify == cn_ecc_right_verify))
        {
            completed += write_size;
            start_offset = 0;   //�ӵڶ���������ʼ���϶���0��ʼ��
        }else
            break;
    }
    return completed;
}


//----дһ������(with ecc)-------------------------------------------------------------
//����: дһ����������eccУ��
//����: sector��������
//      offset��������ƫ�Ƶ�ַ
//      data�����ݻ�����
//      size��д��ĳߴ�
//���أ�cn_all_right_verify=��ȷд�룬cn_ecc_error_verify=д�����
//-----------------------------------------------------------------------------
u32 __write_sector_nand_with_ecc(u32 sector,u32 offset,
                                      u8 *data,u32 size)
{
    u32 i;
    u32 address;
    u8 *ecc;
    if((offset != 0) || (size != u32g_sector_size))
    {
        __read_sector_nand_with_ecc(sector,0,pg_sector_buf,u32g_sector_size);
        //ִ��ECCУ�飬���ǲ��ж�У��������Ϊ����д��ǰ������������ݣ�У���
        //�󲢲���˵�������˴���
    }
    ecc = pg_sector_buf+u32g_sector_size;
	//debug
    memcpy(pg_sector_buf+offset,data,size);
    __make_sector_ecc(pg_sector_buf, ecc);               //����ecc����
    ce_active();
    __write_command_nand(cn_nand_select_page0);

    __write_command_nand(cn_nand_page_program); //�����������
    address=u32g_sector_size*sector;  //д����ʼ��ַ
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i<(u32g_sector_size); i++)
    {//����Ѵ�д�������д�뵽����������������
        *pg_nand_data = pg_sector_buf[i];
       // printf("WriteFlash: data = 0x%x\n", data);
    }
    if(u32g_sector_size > 256)
    {
        for(i = 0; i < 5; i++)              //д��У���룬�����־ǰ��5�ֽ�
            *pg_nand_data = ecc[i];
        *pg_nand_data = 0xff;            //�����־��д0xff�൱�ڱ���ԭֵ
        for(; i < u32g_sector_size/256*3; i++)//д��У���룬�����־��Ĳ���
            *pg_nand_data = ecc[i];
    }else
    {
        for(i = 0; i < 3; i++)              //д��У���룬ֻ��3�ֽ�
            *pg_nand_data = ecc[i];
    }
    __write_command_nand(cn_nand_startup_write);  //����оƬ�ڲ�д�����
    __wait_ready_nand_slow(cn_wait_page_write);  //�ȴ�оƬ�ڲ��������

    if(__read_status_nand() & cn_nand_failure)
    {
        ce_inactive();
        return cn_ecc_error_verify;
    }
    ce_inactive();
    return cn_all_right_verify;
}

//----д��(��eccУ��)----------------------------------------------------
//����: ��eccУ��ѻ�����д����ڣ���ַ���ܿ��߽�
//����: block�����
//      offset������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//���أ�ʵ��д�����������cn_limit_uint32��ʾ����
//-----------------------------------------------------------------------------
u32 write_block_ss_with_ecc(u32 block,u32 offset,
                                 u8 *buf,u32 size)
{
    u32 start_sector;  //��������
    u32 start_offset;  //�׵�ַ����������ƫ����
    u32 end_sector;    //������ַ��������
    u32 end_offset;    //������ַ�������е�ƫ����
    u32 cur_sector;    //��ǰ����д������
    u32 write_size;     //�ӵ�ǰ����д���������
    u32 completed = 0;
    u32 verify;

    if(block >= tg_samsung_nand.block_sum)
        return cn_limit_uint32;
    if((size + offset) > tg_samsung_nand.block_size)
        return cn_limit_uint32;

    //��ʼ������
    start_sector = offset / u32g_sector_size + u32g_sectors_per_block * block;
    //��ʼ��ַ����ʼ�������е�ƫ����
    start_offset = offset % u32g_sector_size;
    //����������
    end_sector =(offset + size-1)/u32g_sector_size+u32g_sectors_per_block*block;
    //������ַ�ڽ��������е�ƫ����
    end_offset = (offset + size -1) % u32g_sector_size;
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        if(cur_sector != end_sector)    //��ǰ�����������һ������
            write_size = u32g_sector_size - start_offset;
        else    //��ǰ���������һ������
            //+1����Ϊend_offset��������Ҫд���
            write_size = end_offset - start_offset +1;
        verify = __write_sector_nand_with_ecc(cur_sector,start_offset,
                                  buf+completed,write_size);
        if((verify == cn_all_right_verify) || (verify == cn_ecc_right_verify))
        {
            completed += write_size;
            start_offset = 0;   //�ӵڶ���������ʼ���϶���0��ʼд
        }else
            break;
    }
    return completed;
}


//----����һ��-------------------------------------------------------------
//����: ����һ��
//����: block_no��Ŀ����
//����: true = �ɹ�������false = ����
//-----------------------------------------------------------------------------
bool_t erase_block_nand(u32 block_no)
{
    u32 page_no;
    page_no = block_no * u32g_sectors_per_block;

    ce_active();

    __write_command_nand(cn_nand_block_erase);

    *pg_nand_addr = (u8)page_no;         // A9 ~ A16
    *pg_nand_addr = (u8)(page_no>>8);    // A17 ~ A24
    *pg_nand_addr = (u8)(page_no>>16);   // A25

    __write_command_nand(cn_nand_startup_erase);

    __wait_ready_nand_slow(cn_wait_block_erase);  //�ȴ�оƬ�ڲ��������

    ce_inactive();
    return true;
//    if(readStatus() & cn_nand_failure)
//      return flWriteFault;
}

//----��������оƬ-------------------------------------------------------------
//����: ��������оƬ��
//����: ��
//����: true = �ɹ�������false = ����
//-----------------------------------------------------------------------------
bool_t __erase_all_nand(char *param)
{
    u32 i;
    for(i=0; i<tg_samsung_nand.block_sum; i++)
    {
        erase_block_nand(i);
    }
    return true;
}

//˵��: ����������ѯ�Ƿ���Ҫ�����ĺ���������flash�����������ۿ��Ƿ�գ�ֻҪ��1
//      ��λ�Ϳ��Ը�Ϊ0����flash�������ɲ�������������
//----��ѯ�Ƿ���Ҫ����(with ecc)-----------------------------------------------
//����: ��ѯ��block��offset��д��buf�е�size��������ǰ�Ƿ���Ҫ�����顣���ڲ���
//      ����������ĥ��flash���ʱ������Լӿ��ٶȺ��ӳ�flash��������Ч���ڲ�ͬ���
//      �£����ֵ�Ч������һ�¡�
//      1��norflash�����д����������ʱ�ǳ���Ч��
//      2��nandflash���߿ɿ�Ӧ�ã����ڴ�������ecc�����⣬��Ҫд��sector�߽����
//        �����ݲ���Ч��
//      3��nandflash��ý��Ӧ�ã�����Ƶý��Ӧ�ÿɲ���ecc��Ч����norflashһ����
//      4����nandflashֻ�ж��Ƿ�ȫFF

//      �Եڶ��������һ���Ľ�������ÿ��д��������ʱ���¿�һ��������������������
//      �������ѹ����������̫���鷳���Ժ���˵�ɡ�
//����: sector�����
//      offset������ƫ�Ƶ�ַ
//      data�����ݻ�����
//      size�����ݳߴ�
//����: flase = ��Ҫ������true = ��׼���ã�����Ҫ����
//-----------------------------------------------------------------------------
bool_t query_block_ready_ss_with_ecc(u32 block,u32 offset,
                              u8 *buf,u32 size)
{
    u32 start_sector;  //��������
    u32 end_sector;    //������ַ��������
    u32 cur_sector;    //��ǰ���ڶ�������
    u32 address;
    u32 loop;
    u8 data;

    if(block >= tg_samsung_nand.block_sum)
        return false;
    if((size + offset) > tg_samsung_nand.block_size)
        return false;
    if(size == 0)
        return true;
    if(buf == NULL)
        return false;

    //��ʼ������
    start_sector = offset / u32g_sector_size + u32g_sectors_per_block * block;
    //����������
    end_sector =(offset + size-1)/u32g_sector_size+u32g_sectors_per_block*block;
    address = u32g_sector_size*start_sector;     //����ʵ�ʵ�ַ
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        ce_active();                        //����Ƭѡ
        __write_command_nand(cn_nand_select_page0);    //д���ģʽ����
        __write_address_nand(address);
        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

        for(loop=0; loop < u32g_sector_size+cn_oob_size; loop++)
        {
            data = *pg_nand_data;    //��ȡ����
            if(data != 0xff)            //ֻҪ��1�ֽڷ�oxff,�����ش���
            {
                ce_inactive();          //�ر�Ƭѡ
                return false;
            }
        }
        ce_inactive();                      //�ر�Ƭѡ
        address += u32g_sector_size;
    }
    return true;
}

//----��ѯ�Ƿ���Ҫ����(no ecc)-----------------------------------------------
//����: ͬ��һ������������ecc
//����: sector�����
//      offset������ƫ�Ƶ�ַ
//      data�����ݻ�����
//      size�����ݳߴ�
//����: flase = ��Ҫ������true = ��׼���ã�����Ҫ����
//-----------------------------------------------------------------------------
bool_t query_block_ready_nand_no_ecc(u32 block,u32 offset,
                              u8 *buf,u32 size)
{
    u32 start_sector;  //��������
    u32 start_offset;  //�׵�ַ����������ƫ����
    u32 end_sector;    //������ַ��������
    u32 end_offset;    //������ַ�������е�ƫ����
    u32 cur_sector;    //��ǰ���ڶ�������
    u32 read_size;
    u32 address;
    u32 completed = 0;
    u32 loop;
    u8 data;

    if(block >= tg_samsung_nand.block_sum)
        return false;
    if((size + offset) > tg_samsung_nand.block_size)
        return false;
    if(size == 0)
        return true;
    if(buf == NULL)
        return true;

    //��ʼ������
    start_sector = offset / u32g_sector_size + u32g_sectors_per_block * block;
    //��ʼ��ַ����ʼ�������е�ƫ����
    start_offset = offset % u32g_sector_size;
    //����������
    end_sector =(offset + size-1)/u32g_sector_size+u32g_sectors_per_block*block;
    //������ַ�ڽ��������е�ƫ����
    end_offset = (offset + size -1) % u32g_sector_size;
    address = block * tg_samsung_nand.block_size + offset;
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        if(cur_sector != end_sector)    //��ǰ�����������һ������
            read_size = u32g_sector_size - start_offset;
        else    //��ǰ���������һ������
            //+1����Ϊend_offset��������Ҫд���
            read_size = end_offset - start_offset +1;
        if(address & 0x100)
            __write_command_nand(cn_nand_select_page1);    //д���ģʽ����
        else
            __write_command_nand(cn_nand_select_page0);    //д���ģʽ����
        __write_address_nand(address);

        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

        for(loop=0; loop < read_size; loop++)
        {
            data = *pg_nand_data;    //��ȡ����
            if((data | buf[completed]) != data)   //�鿴�з�0->1��λ
            {
                ce_inactive();                      //�ر�Ƭѡ
                return false;       //��0->1��λ��������Ҫ����
            }
            completed++;
        }
        ce_inactive();                      //�ر�Ƭѡ
        address += read_size;
        start_offset = 0;   //�ӵڶ���������ʼ���϶���0��ʼ��
    }
    return true;
}

//----��ѯ�Ƿ���Ҫ����(buf)-----------------------------------------------
//����: flash�е�ԭ����������org_data�У����Ҫ�ĳ���new_data�е����ݣ��鿴�Ƿ�
//      ��Ҫ������
//����: new_data�������ݻ�����
//      new_data�����ݻ�����
//      size�����ݳߴ�
//����: flase = ��Ҫ������true = ��׼���ã�����Ҫ����
//-----------------------------------------------------------------------------
bool_t query_ready_with_data_nand(u8 *new_data,u8 *org_data,
                                        u32 size)
{
    u32 loop;
    if(org_data == NULL)
        return false;
    if(new_data != NULL)
    {
        for(loop = 0; loop < size; loop++)
        {
            if((org_data[loop] | new_data[loop]) != org_data[loop])
                return false;   //���������0��1�����ݣ�����false
        }
    }else
    {
        for(loop = 0; loop < size; loop++)
        {
            if(org_data[loop] != 0xFF)
                return false;
        }
    }
    return true;
}

//�������㷨:
//1�����ǳ���ʱ������OOB��ͷ��ҳ��6���ֽڱ�д���0xFF���ݣ��ÿ���֤��0xFF��
//2��ʹ�ù����е��������飬Ҳ��ͷ��ҳ�ĵ�6�ֽ�д��0.
//3����Զ��Ҫ�������飬�������ڻ����ǵĵ�6�ֽ�Ҳ��������
//----��Ч����--------------------------------------------------------------
//����: ���һ�����Ƿ���Ч�飬�ο��������㷨��
//����: block_no�����
//����: true=�ÿ飬false=���顣
//----------------------------------------------------------------------------
bool_t check_block_nand(u32 block_no)
{
    uint16_t j;
    u32 address;
    u8 data;
    ce_active();
    for(j=0; j<2; j++)
    {
        address = tg_samsung_nand.block_size * block_no + 0x200 * j + (517&0xf);

        __write_command_nand(cn_nand_select_oob);

        __write_address_nand(address);

        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
        data = *pg_nand_data;

        if(data != 0xFF)
        {
            ce_inactive();
            return  false;
        }
    }
    ce_inactive();
    return true;
}

//----���һ������-----------------------------------------------------------
//���ܣ��ڶ���д�������Ⱥ����е��ã�������������з���ĳ���ǻ��飬�������ñ�����
//      �������Ϊ���飬�����������ĵ�517�ֽ�д0.
//����: block_no�����
//����: true=�ɹ���ǣ�false=���ʧ�ܡ�
//----------------------------------------------------------------------------
bool_t __mark_invalid_block(u32 block)
{
    uint16_t j;
    u32 address;
    __write_command_nand(cn_nand_select_oob);
    for(j=0; j<2; j++)
    {
        address = tg_samsung_nand.block_size * block + 0x200 * j + (517&0xf);
        ce_active();

        __write_command_nand(cn_nand_page_program);

        __write_address_nand(address);

        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
        *pg_nand_data = 0;
        __write_command_nand(cn_nand_startup_write);
        __wait_ready_nand_slow(cn_wait_page_write);  //�ȴ�оƬ�ڲ��������
        if(__read_status_nand() & cn_nand_failure)
        {
            ce_inactive();
            return false;
        }

    }
    ce_inactive();
    return true;
}

//----��flash�кÿ�����--------------------------------------------------------
//����: ��flash�кÿ�����
//����: ��
//����: �ÿ�����
//-----------------------------------------------------------------------------
u32 __check_all_ss(void)
{
    u32 i, j,sum = 0;
    u32 address;
    u8 data;

    for(i=0; i<tg_samsung_nand.block_sum; i++)
    {
        ce_active();
        for(j=0; j<2; j++)
        {
            address = tg_samsung_nand.block_size * i + 0x200 * j + (517&0xf);

            __write_command_nand(cn_nand_select_oob);

            __write_address_nand(address);

            __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
            data = *pg_nand_data;
            if(data != 0xFF)
            {
                sum++;
                break;
            }
        }
    }
    ce_inactive();
    return sum;
}

//----��������ecc��-----------------------------------------------------------
//����: data�а���256�ֽ����ݣ���������ecc��
//����: data�����ݻ�����
//      ecc: 3�ֽڵĻ�����������ecc��
//����: ��
//-----------------------------------------------------------------------------
void __make_sector_ecc(const u8 *data,u8 *ecc)
{
    u32 i;
    for(i = 0; i < u32g_sector_size/256; i++)
    {
        ecc_make_256(data + i*256, ecc + i*3);
    }
}

//----��flash оƬid-----------------------------------------------------------
//����: ��ȡflashоƬ��id
//����: ��
//����: оƬid
//-----------------------------------------------------------------------------
u16 __read_chip_id(struct nand_chip_id * NAND_ID)
{

  *pg_nand_cmd = cn_nand_read_id;
  *pg_nand_addr = 0x00;

   NAND_ID->Maker_ID   = *pg_nand_data;
   NAND_ID->Device_ID  = *pg_nand_data;
   NAND_ID->Third_ID   = *pg_nand_data;
   NAND_ID->Fourth_ID  = *pg_nand_data;
   return (NAND_ID->Maker_ID<<8)+NAND_ID->Device_ID;
}

//----����оƬid---------------------------------------------------------------
//����: ����оƬidʶ��оƬ,ʶ������д��tg_samsung_nand��
//����: id,��ʶ���оƬid
//����: true = ��ȷʶ��false = ����ʶ��
//-----------------------------------------------------------------------------
bool_t __parse_chip(uint16_t id,char **name)
{
    u32 no,sum;
    sum = sizeof(tg_nand_table)/sizeof(struct nand_table);
    for(no = 0; no < sum; no++)
    {
        if(id == tg_nand_table[no].vendor_chip_id)
        {
            u32g_sectors_per_block = tg_nand_table[no].pages_per_block;
            tg_samsung_nand.block_sum = tg_nand_table[no].blocks_sum;
            tg_samsung_nand.block_size = tg_nand_table[no].block_size;
            tg_samsung_nand.DBX_read_buf_size = tg_nand_table[no].block_size;
            tg_samsung_nand.DBX_write_buf_size = tg_nand_table[no].block_size;
            *name = tg_nand_table[no].chip_name;
            u16g_oob_size = tg_nand_table[no].oob_size;
            u32g_sector_size =tg_samsung_nand.block_size/u32g_sectors_per_block;
            return true;
        }
    }
    return false;
}

//----��λоƬ-----------------------------------------------------------------
//����: ��λоƬ
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __reset_nand(void)
{
    *pg_nand_cmd = cn_nand_reset;
    __wait_ready_nand_slow(cn_wait_reset);  //�ȴ�оƬ�ڲ��������

}

void NandFlash_FSMCConfig(void)
{
  FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
  FSMC_NAND_PCCARDTimingInitTypeDef  p;

  /*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_SetupTime = 0x0;
  p.FSMC_WaitSetupTime = 0x3;
  p.FSMC_HoldSetupTime = 0x9;
  p.FSMC_HiZSetupTime = 0x0;

  FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank3_NAND;
  FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Enable;
  FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable;
  FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_512Bytes;
  FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;
  FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;
  FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;
  FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;

  FSMC_NANDInit(&FSMC_NANDInitStructure);

  /*!< FSMC NAND Bank Cmd Test */
  FSMC_NANDCmd(FSMC_Bank3_NAND, ENABLE);
}

void NandFlash_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-- GPIO Configuration ------------------------------------------------------*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*NCE3  NAND pin configuration  */
      GPIO_InitStructure.GPIO_Pin =  NandFlash_CE_GPIOG9;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOG, &GPIO_InitStructure);

    /*!< NWAIT NAND pin configuration */
      GPIO_InitStructure.GPIO_Pin = NandFlash_RB_GPIOD6 ;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOD, &GPIO_InitStructure);

      /*!< INT2 NAND pin configuration */
      //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
      //GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin =NandFlash_WP_GPIOD3;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    NandFlash_FSMCConfig();
}
void NandFlash_On(void)
{
	GPIO_SetBits(GPIOD, NandFlash_WP_GPIOD3);
}

u32 __write_sector_and_oob(u32 sector, u8 *data)
{
    u32 i;
    u32 address;
    memcpy(pg_sector_buf,data,528);
    ce_active();
    __write_command_nand(cn_nand_select_page0);

    __write_command_nand(cn_nand_page_program); //�����������
    address=u32g_sector_size*sector;  //д����ʼ��ַ
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i<528; i++)
    {//����Ѵ�д�������д�뵽����������������
        *pg_nand_data = pg_sector_buf[i];
       // printf("WriteFlash: data = 0x%x\n", data);
    }
    __write_command_nand(cn_nand_startup_write);  //����оƬ�ڲ�д�����
    __wait_ready_nand_slow(cn_wait_page_write);  //�ȴ�оƬ�ڲ��������

    if(__read_status_nand() & cn_nand_failure)
    {
        ce_inactive();
        return cn_ecc_error_verify;
    }
    ce_inactive();
    return cn_all_right_verify;
}

//----������(��oob)------------------------------------------------------------
//����: ��һ����������oob�е�����һ���
//����: sector��������������
//      data���������ݵĻ�����
//����: ��
//-----------------------------------------------------------------------------
void __read_sector_and_oob(u32 sector,u8 *data)
{
    u32 i;
    u32 address;
    address =u32g_sector_size*sector;     //����ʵ�ʵ�ַ
    ce_active();                        //����Ƭѡ
    __write_command_nand(cn_nand_select_page0);    //д���ģʽ����
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i < 528; i++)
    {
        data[i] = *pg_nand_data;     //��ȡ����
    }
    ce_inactive();                      //�ر�Ƭѡ
    return ;
}

//----��ʼ��nandоƬ-----------------------------------------------------------
//���ܣ���ʼ��nandоƬ�����ļ�ϵͳ
//����: ��
//����: 1= �ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_fs_nandflash(ptu32_t para)
{
    struct nand_chip_id chip_id;
    u16 myid;
    char *name;

    NandFlash_GPIO_Init();
	NandFlash_On();

    __reset_nand();
    myid = __read_chip_id(&chip_id);
    if( __parse_chip(myid,&name) == false)
        return 0;
    
    pg_sector_buf = (u8*)m_malloc_gbl(u32g_sector_size+u16g_oob_size,0);
    if(pg_sector_buf == NULL)
    {
        return 0;
    }
    tg_samsung_nand.query_block_ready_with_ecc= query_block_ready_ss_with_ecc;
    tg_samsung_nand.query_block_ready_no_ecc = query_block_ready_nand_no_ecc;
    tg_samsung_nand.query_ready_with_data = query_ready_with_data_nand;
    tg_samsung_nand.erase_block = erase_block_nand;
    tg_samsung_nand.check_block = check_block_nand;
    tg_samsung_nand.read_data_with_ecc = read_block_ss_with_ecc;
    tg_samsung_nand.write_data_with_ecc = write_block_ss_with_ecc;
    tg_samsung_nand.read_data_no_ecc = read_block_ss_no_ecc;
    tg_samsung_nand.write_data_no_ecc = write_block_ss_no_ecc;
    tg_samsung_nand.write_PCRB = write_PCRB_nand;
    tg_samsung_nand.restore_PCRB = restore_PCRB_nand;
    if(DFFSD_install_chip(&tg_samsung_nand,name,cn_reserve_blocks))
        return 1;
    else
    {
        m_free(pg_sector_buf);
        return 0;
    }
}


