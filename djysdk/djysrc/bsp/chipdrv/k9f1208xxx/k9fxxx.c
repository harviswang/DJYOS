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

#include "stdint.h"
#include "stdlib.h"
#include "file.h"
#include "flashfile.h"
#include "cpu_peri.h"
#include <string.h>
#include "k9fxxx.h"

static  struct tagFlashChip tg_samsung_nand;    //оƬ

//����������ָ�룬������д�����ڲ�ר�ã�djyffs�У�������������flash�Ĺ��ԣ��ʲ�
//������struct tagFlashChip �ṹ�С���̬�����ڴ��������ɾ�̬���飬����Ϊchip��
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

#if (cn_nand_little_block == cn_nand_block)

//----д������-----------------------------------------------------------------
//����: д��оƬд������
//����: val��д�����֣�
//����: ��
//-----------------------------------------------------------------------------
#define __write_command_nand(cmd)   pg_nand_reg->NFCMD = cmd
//----д���ַ-----------------------------------------------------------------
//����: д��оƬд������,5�зֱ�д��A0 ~ A7��A9 ~ A11��A12 ~ A19,A20~A27
//����: val��д�����֣�
//����: ��
//----------------------------------------------------------------------------
void __write_address_nand(uint32_t addr)
{
    pg_nand_reg->NFADDR = (u8)addr;
    pg_nand_reg->NFADDR = (u8)(addr>>9);
    pg_nand_reg->NFADDR = (u8)(addr>>17);
    pg_nand_reg->NFADDR = (u8)(addr>>25);

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
        data[i] = pg_nand_reg->NFDATA;     //��ȡ����
    }
    ce_inactive();                      //�ر�Ƭѡ
    return ;
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
    pg_nand_reg->NFDATA = (u8)protected_block;
    pg_nand_reg->NFDATA = (u8)(protected_block>>8);
    pg_nand_reg->NFDATA = (u8)(protected_block>>16);
    pg_nand_reg->NFDATA = (u8)(protected_block>>24);
    __write_command_nand(cn_nand_startup_write);  //����оƬ�ڲ�д�����
    __wait_ready_nand_slow(cn_wait_page_write);        //�ȴ�оƬ�ڲ��������

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
    protected_block = pg_nand_reg->NFDATA;
    protected_block += (u32)(pg_nand_reg->NFDATA)<<8;
    protected_block += (u32)(pg_nand_reg->NFDATA)<<16;
    protected_block += (u32)(pg_nand_reg->NFDATA)<<24;
    *restored = protected_block;

    ce_inactive();
    if(protected_block > tg_samsung_nand.block_sum)
    {
        erase_block_nand(PCRB_block);
        return true;
    }
    sector_buf = M_MallocLc(u32g_sector_size,0);
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
    free(sector_buf);
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
    while((pg_nand_reg->NFSTAT & 1) ==0 );
    return true;
}

//----�ȴ�оƬ�ڲ����ٲ������-------------------------------------------------
//����: ��оƬִ��д������Ҫ���ñ������ȴ�������ɲ��ܽ�һ������������һ������
//      ͬ���������˷Ѻܶ�ʱ�䣬����һЩ�ܿ�����Ĳ�������ʹ��__wait_ready_nand
//      ������
//����: wait_time�����Ƶȴ�ʱ�䣬΢����
//����: true = ��ȷ��ɲ�����false = ��������
//-----------------------------------------------------------------------------
bool_t __wait_ready_nand_slow(uint16_t wait_time)
{
    Timer_SetCounter(1,wait_time);     //����ֵ��Ϊwait_time
    Timer_Reload(1);                    //���ض�ʱֵ
    Timer_Start(1);                     //������ʱ��
    Int_AsynSignalSync(cn_int_line_timer1);
    Timer_Stop(1);
    while((pg_nand_reg->NFSTAT & 1) ==0 );

    return true;
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
        data[i] = pg_nand_reg->NFDATA;     //��ȡ����
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;
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
        j = Ecc_Corect256(data + i*256, ecc + i*3);
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
        pg_sector_buf[i] = pg_nand_reg->NFDATA;     //��ȡ����
    }
    if(u32g_sector_size > 256)
    {
        for(i = 0; i < 5; i++)              //��ȡУ���룬�����־ǰ��5�ֽ�
            ecc[i] = pg_nand_reg->NFDATA;
        ecc[i] = pg_nand_reg->NFDATA;          //�����־����ȡ������
        for(; i < u32g_sector_size/256*3; i++)//��ȡУ���룬�����־��Ĳ���
            ecc[i] = pg_nand_reg->NFDATA;
    }else
    {
        for(i = 0; i < 3; i++)              //��ȡУ���룬ֻ��3�ֽ�
            ecc[i] = pg_nand_reg->NFDATA;
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;
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
    chip_status = pg_nand_reg->NFDATA;

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
        pg_nand_reg->NFDATA = data[i];
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;

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
    __write_command_nand(0x00);

    __write_command_nand(cn_nand_page_program); //�����������
    address=u32g_sector_size*sector;  //д����ʼ��ַ
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i<(u32g_sector_size); i++)
    {//����Ѵ�д�������д�뵽����������������
        pg_nand_reg->NFDATA = pg_sector_buf[i];
       // printf("WriteFlash: data = 0x%x\n", data);
    }
    if(u32g_sector_size > 256)
    {
        for(i = 0; i < 5; i++)              //д��У���룬�����־ǰ��5�ֽ�
            pg_nand_reg->NFDATA = ecc[i];
        pg_nand_reg->NFDATA = 0xff;            //�����־��д0xff�൱�ڱ���ԭֵ
        for(; i < u32g_sector_size/256*3; i++)//д��У���룬�����־��Ĳ���
            pg_nand_reg->NFDATA = ecc[i];
    }else
    {
        for(i = 0; i < 3; i++)              //д��У���룬ֻ��3�ֽ�
            pg_nand_reg->NFDATA = ecc[i];
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
//����: sector�����
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;

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

    pg_nand_reg->NFADDR = (u8)page_no;         // A9 ~ A16
    pg_nand_reg->NFADDR = (u8)(page_no>>8);    // A17 ~ A24
    pg_nand_reg->NFADDR = (u8)(page_no>>16);   // A25

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
bool_t __erase_all_nand(void)
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
            data = pg_nand_reg->NFDATA;    //��ȡ����
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
        return false;

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
            data = pg_nand_reg->NFDATA;    //��ȡ����
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
        data = pg_nand_reg->NFDATA;

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
        pg_nand_reg->NFDATA = 0;
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
            data = pg_nand_reg->NFDATA;
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
        Ecc_Make256(data + i*256, ecc + i*3);
    }
}

//----��flash оƬid-----------------------------------------------------------
//����: ��ȡflashоƬ��id
//����: ��
//����: оƬid
//-----------------------------------------------------------------------------
uint16_t __read_chip_id (void)
{
    uint16_t id;
    ce_active();

    __write_command_nand(cn_nand_reset);
    __wait_ready_nand();  //�ȴ�оƬ�ڲ��������
    __write_command_nand(cn_nand_read_id);

    pg_nand_reg->NFADDR = 0;  // Address. 1cycle

    id = pg_nand_reg->NFDATA<<8;
    id |= pg_nand_reg->NFDATA;

    ce_inactive();
    return id ;
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
            tg_samsung_nand.PTT_read_buf_size = tg_nand_table[no].block_size;
            tg_samsung_nand.PTT_write_buf_size = tg_nand_table[no].block_size;
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
  ce_active();

  __write_command_nand(cn_nand_reset);
  __wait_ready_nand_slow(cn_wait_reset);  //�ȴ�оƬ�ڲ��������
  ce_inactive();

}

//----��ʼ��nandоƬ-----------------------------------------------------------
//���ܣ���ʼ��nandоƬ�����ļ�ϵͳ
//����: ��
//����: 1= �ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_FsNandFlash(ptu32_t para)
{
    uint16_t chip_id;
    char *name;

    //��ʼ��timer4��Ϊ�Ⱥ�flash�ڲ�������ɵ��жϣ�����ISR
    Timer_SetClkSource(1,0);              //��Ƶ��1/2��Ƶ
    //Ԥ��Ƶ��:���ö�ʱ������ʱ��1Mhz
    Timer_SetPrecale(0,(u32)CN_CFG_TIMER_CLK/1000000/2 -1);
    Timer_SetType(1,1);                //���ö�ʱ����������
    Int_SettoAsynSignal(cn_int_line_timer1);     //��Ϊ�첽�ź�
//    Int_RestoreAsynLine(cn_int_line_timer1);//�����ж�,

    //nand config register
    // TACLS        [14:12] CLE&ALE duration = HCLK*TACLS.
    // TWRPH0       [10:8]  TWRPH0 duration = HCLK*(TWRPH0+1)
    // TWRPH1       [6:4]   TWRPH1 duration = HCLK*(TWRPH1+1)
    // AdvFlash(R)  [3]     Advanced NAND, 0:256/512, 1:1024/2048
    // PageSize(R)  [2]     NAND memory page size
    //                      when [3]==0, 0:256, 1:512 bytes/page.
    //                      when [3]==1, 0:1024, 1:2048 bytes/page.
    // AddrCycle(R) [1]     NAND flash addr size
    //                      when [3]==0, 0:3-addr, 1:4-addr.
    //                      when [3]==1, 0:4-addr, 1:5-addr.
    // BusWidth(R/W) [0]    NAND bus width. 0:8-bit, 1:16-bit.
    pg_nand_reg->NFCONF =(cn_talcs<<12)|(cn_twrph0<<8)|(cn_twrph1<<4);

    //nand control register
    // Lock-tight   [13]    0:Disable lock, 1:Enable lock.
    // Soft Lock    [12]    0:Disable lock, 1:Enable lock.
    // EnablillegalAcINT[10] Illegal access interupt control.0:Disable,1:Enable
    // EnbRnBINT    [9] RnB interrupt. 0:Disable, 1:Enable
    // RnB_TrandMode[8] RnB transition detection config.0:Low->High,1:High->Low
    // SpareECCLock [6]     0:Unlock, 1:Lock
    // MainECCLock  [5]     0:Unlock, 1:Lock
    // InitECC(W)   [4]     1:Init ECC decoder/encoder.
    // Reg_nCE      [1]     0:nFCE=0, 1:nFCE=1.
    // NANDC Enable [0]     operating mode. 0:Disable, 1:Enable.
    pg_nand_reg->NFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)
                            |(1<<5)|(1<<4)|(1<<1)|(1<<0);
    __reset_nand();
    chip_id = __read_chip_id();
    if( __parse_chip(chip_id,&name) == false)
        return 0;
    pg_sector_buf = (u8*)M_Malloc(u32g_sector_size+u16g_oob_size,0);
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
    if(DFFSD_InstallChip(&tg_samsung_nand,name,cn_reserve_blocks))
        return 1;
    else
    {
        free(pg_sector_buf);
        return 0;
    }
}
#endif

#if (cn_nand_big_block == 1)
//----д������-----------------------------------------------------------------
//����: д��оƬд������
//����: val��д�����֣�
//����: ��
//-----------------------------------------------------------------------------
#define __write_command_nand(cmd)   pg_nand_reg->NFCMD = cmd

//----д���ַ-----------------------------------------------------------------
//����: д��оƬд������,5�зֱ�д��A0 ~ A7��A9 ~ A11��A12 ~ A19,A20~A27
//����: val��д�����֣�
//����: ��
//----------------------------------------------------------------------------
void __write_address_nand(uint32_t addr)
{
    pg_nand_reg->NFADDR = (uint8_t)((addr) & 0xff);
    pg_nand_reg->NFADDR = (uint8_t)((addr>>8) & 0x7);
    pg_nand_reg->NFADDR = (uint8_t)((addr>>11) & 0xff);
    pg_nand_reg->NFADDR = (uint8_t)((addr>>19) & 0xff);
    pg_nand_reg->NFADDR = (uint8_t)((addr>>27) & 0xff);

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
    __write_command_nand(cn_nand_page_read);    //д���ģʽ����
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_command_nand(cn_nand_startup_read);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    for(i=0; i < (2048+64); i++)
    {
        data[i] = pg_nand_reg->NFDATA;     //��ȡ����
    }
    ce_inactive();                      //�ر�Ƭѡ
    return ;
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
    address =tg_samsung_nand.block_size*PCRB_block + 60;
    ce_active();                        //����Ƭѡ

    __write_command_nand(cn_nand_page_program); //�����������
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    //�ܱ�����Ŀ����д��flash��ע��:û����ECCУ�飬�Ժ���---db
    pg_nand_reg->NFDATA = (u8)protected_block;
    pg_nand_reg->NFDATA = (u8)(protected_block>>8);
    pg_nand_reg->NFDATA = (u8)(protected_block>>16);
    pg_nand_reg->NFDATA = (u8)(protected_block>>24);
    __write_command_nand(cn_nand_startup_write);  //����оƬ�ڲ�д�����
    __wait_ready_nand_slow(cn_wait_page_write);        //�ȴ�оƬ�ڲ��������

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
    address =tg_samsung_nand.block_size*PCRB_block + 60;
    ce_active();                        //����Ƭѡ
    __write_command_nand(cn_nand_page_read);    //����oobҳ
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_command_nand(cn_nand_startup_read);
    //wjz code revision 256M�ģ���64M�ģ����˸�0x30�������������ǵȴ�ʱ����
    //data[i],ǰ��һЩ���ݣ������������.
    __wait_ready_nand( );
    //�ܱ�����Ŀ����д��flash��ע��:û����ECCУ�飬�Ժ���---db
    protected_block = pg_nand_reg->NFDATA;
    protected_block += (u32)(pg_nand_reg->NFDATA)<<8;
    protected_block += (u32)(pg_nand_reg->NFDATA)<<16;
    protected_block += (u32)(pg_nand_reg->NFDATA)<<24;
    *restored = protected_block;

    ce_inactive();
    if(protected_block > tg_samsung_nand.block_sum)
    {
        erase_block_nand(PCRB_block);
        return true;
    }
    sector_buf = M_MallocLc(u32g_sector_size,0);
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
    free(sector_buf);
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
    while((pg_nand_reg->NFSTAT & 1) ==0 );
    return true;
}

//----�ȴ�оƬ�ڲ����ٲ������-------------------------------------------------
//����: ��оƬִ��д������Ҫ���ñ������ȴ�������ɲ��ܽ�һ������������һ������
//      ͬ���������˷Ѻܶ�ʱ�䣬����һЩ�ܿ�����Ĳ�������ʹ��__wait_ready_nand
//      ������
//����: wait_time�����Ƶȴ�ʱ�䣬΢����
//����: true = ��ȷ��ɲ�����false = ��������
//-----------------------------------------------------------------------------
bool_t __wait_ready_nand_slow(uint16_t wait_time)
{
    Timer_SetCounter(1,wait_time);     //����ֵ��Ϊwait_time
    Timer_Reload(1);                    //���ض�ʱֵ
    Timer_Start(1);                     //������ʱ��
    Int_AsynSignalSync(cn_int_line_timer1);
    Timer_Stop(1);
    while((pg_nand_reg->NFSTAT & 1) ==0 );

    return true;
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
    __write_command_nand(cn_nand_page_read);    //д���ģʽ����
    __write_address_nand(address);

    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_command_nand(cn_nand_startup_read);
    //wjz code revision 256M�ģ���64M�ģ����˸�0x30�������������ǵȴ�ʱ����
    //data[i],ǰ��һЩ���ݣ������������.
    __wait_ready_nand( );

    for(i=0; i < size; i++)
    {
        data[i] = pg_nand_reg->NFDATA;     //��ȡ����
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;
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
        j = Ecc_Corect256(data + i*256, ecc + i*3);
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
    __write_command_nand(cn_nand_page_read);    //д���ģʽ����
    __write_address_nand(address);

    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_command_nand(cn_nand_startup_read);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    for(i=0; i < u32g_sector_size; i++)
    {
        pg_sector_buf[i] = pg_nand_reg->NFDATA;     //��ȡ����
    }

   if(u32g_sector_size > 256)
    {
        ecc[0] = pg_nand_reg->NFDATA;     //�����־����ȡ������
        for(i=0; i < u32g_sector_size/256*3; i++)//��ȡУ���룬�����־��Ĳ���
            ecc[i] = pg_nand_reg->NFDATA;
    }
    else
    {
        for(i = 0; i < 3; i++)              //��ȡУ���룬ֻ��3�ֽ�
            ecc[i] = pg_nand_reg->NFDATA;
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;
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
    chip_status = pg_nand_reg->NFDATA;

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
    __write_command_nand(cn_nand_page_program); //�����������
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i < size; i++)
    {//����Ѵ�д�������д�뵽����������������
        pg_nand_reg->NFDATA = data[i];
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;

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
    memcpy(pg_sector_buf+offset,data,size);
    __make_sector_ecc(pg_sector_buf, ecc);               //����ecc����
    ce_active();
    __write_command_nand(0x00);

    __write_command_nand(cn_nand_page_program); //�����������
    address=u32g_sector_size*sector;  //д����ʼ��ַ
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

    for(i=0; i<(u32g_sector_size); i++)
    {//����Ѵ�д�������д�뵽����������������
        pg_nand_reg->NFDATA = pg_sector_buf[i];
       // printf("WriteFlash: data = 0x%x\n", data);
    }

    if(u32g_sector_size > 256)
    {
        pg_nand_reg->NFDATA = 0xFF;            //�����־��д0xff�൱�ڱ���ԭֵ
        for(i = 0; i < u32g_sector_size/256*3; i++)              //д��У���룬�����־ǰ��5�ֽ�
            pg_nand_reg->NFDATA = ecc[i];
        for(;i<64-1;i++)//-1,�ǵ�һ��д�뻵���־
             pg_nand_reg->NFDATA= 0xff;
    }else
    {
        for(i = 0; i < 3; i++)              //д��У���룬ֻ��3�ֽ�
            pg_nand_reg->NFDATA = ecc[i];
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
//����: sector�����
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
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_samsung_nand.block_size)
        return CN_LIMIT_UINT32;

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

    pg_nand_reg->NFADDR = (u8)page_no;         // A9 ~ A16
    pg_nand_reg->NFADDR = (u8)(page_no>>8);    // A17 ~ A24
    pg_nand_reg->NFADDR = (u8)(page_no>>16);   // A25

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
bool_t __erase_all_nand(void)
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
        __write_command_nand(cn_nand_page_read);    //д���ģʽ����
        __write_address_nand(address);
        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
        __write_command_nand(cn_nand_startup_read);
        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
        for(loop=0; loop < u32g_sector_size+cn_oob_size; loop++)
        {
            data = pg_nand_reg->NFDATA;    //��ȡ����
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
        return false;

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

        __write_command_nand(cn_nand_page_read);    //д���ģʽ����
        __write_address_nand(address);
        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
        __write_command_nand(cn_nand_startup_read);//wjz 256M�����̣���datasheet
        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������

        for(loop=0; loop < read_size; loop++)
        {
            data = pg_nand_reg->NFDATA;    //��ȡ����
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
    u32 address;
    u8 data;
    ce_active();

    address = tg_samsung_nand.block_size * block_no + u32g_sector_size+1;

    __write_command_nand(0x00);

    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_command_nand(cn_nand_startup_read);//wjz ʹ�õ��ڶ�����
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    data = pg_nand_reg->NFDATA;

    if(data != 0xFF)
    {
        ce_inactive();
        return  false;
    }

    ce_inactive();
    return true;
}
//----���һ������-----------------------------------------------------------
//���ܣ��ڶ���д�������Ⱥ����е��ã�������������з���ĳ���ǻ��飬�������ñ�����
//      �������Ϊ���飬����ÿ��������oob�ĵ�1�ֽ�д0.
//����: block_no�����
//����: true=�ɹ���ǣ�false=���ʧ�ܡ�
//----------------------------------------------------------------------------
bool_t __mark_invalid_block(u32 block_no)
{
    u32 address;


    address = tg_samsung_nand.block_size * block_no + u32g_sector_size+1;
    ce_active();

    __write_command_nand(cn_nand_page_program);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    __write_address_nand(address);
    __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
    pg_nand_reg->NFDATA = 0;
    __write_command_nand(cn_nand_startup_write);
    __wait_ready_nand_slow(cn_wait_page_write);  //�ȴ�оƬ�ڲ��������
    if(__read_status_nand() & cn_nand_failure)
    {
        ce_inactive();
        return false;
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
    u32 i,sum = 0;
    u32 address;
    u8 data;

    for(i=0; i<tg_samsung_nand.block_sum; i++)
    {
        ce_active();
        address = tg_samsung_nand.block_size * i + u32g_sector_size+1;

        __write_command_nand(0x00);

        __write_address_nand(address);
        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
        __write_command_nand(cn_nand_startup_read);//wjz ʹ�õ��ڶ�����
        __wait_ready_nand( );  //�ȴ�оƬ�ڲ��������
        data = pg_nand_reg->NFDATA;
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
        Ecc_Make256(data + i*256, ecc + i*3);
    }
}

//----��flash оƬid-----------------------------------------------------------
//����: ��ȡflashоƬ��id
//����: ��
//����: оƬid
//-----------------------------------------------------------------------------
uint16_t __read_chip_id (void)
{
    uint16_t id;
    ce_active();

    __write_command_nand(cn_nand_reset);
    __wait_ready_nand();  //�ȴ�оƬ�ڲ��������
    __write_command_nand(cn_nand_read_id);

    pg_nand_reg->NFADDR = 0;  // Address. 1cycle

    id = pg_nand_reg->NFDATA<<8;
    id |= pg_nand_reg->NFDATA;

    ce_inactive();
    return id ;
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
            tg_samsung_nand.PTT_read_buf_size = tg_nand_table[no].block_size;
            tg_samsung_nand.PTT_write_buf_size = tg_nand_table[no].block_size;
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
  ce_active();

  __write_command_nand(cn_nand_reset);
  __wait_ready_nand_slow(cn_wait_reset);  //�ȴ�оƬ�ڲ��������
  ce_inactive();

}

//----��ʼ��nandоƬ-----------------------------------------------------------
//���ܣ���ʼ��nandоƬ�����ļ�ϵͳ
//����: ��
//����: 1= �ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_FsNandFlash(ptu32_t para)
{
    uint16_t chip_id;
    char *name;

    //��ʼ��timer4��Ϊ�Ⱥ�flash�ڲ�������ɵ��жϣ�����ISR
    Timer_SetClkSource(1,0);              //��Ƶ��1/2��Ƶ
    //Ԥ��Ƶ��:���ö�ʱ������ʱ��1Mhz
    Timer_SetPrecale(0,(u32)CN_CFG_TIMER_CLK/1000000/2 -1);
    Timer_SetType(1,1);                //���ö�ʱ����������
    Int_SettoAsynSignal(cn_int_line_timer1);     //��Ϊ�첽�ź�
//    Int_RestoreAsynLine(cn_int_line_timer1);//�����ж�,

    //nand config register
    // TACLS        [14:12] CLE&ALE duration = HCLK*TACLS.
    // TWRPH0       [10:8]  TWRPH0 duration = HCLK*(TWRPH0+1)
    // TWRPH1       [6:4]   TWRPH1 duration = HCLK*(TWRPH1+1)
    // AdvFlash(R)  [3]     Advanced NAND, 0:256/512, 1:1024/2048
    // PageSize(R)  [2]     NAND memory page size
    //                      when [3]==0, 0:256, 1:512 bytes/page.
    //                      when [3]==1, 0:1024, 1:2048 bytes/page.
    // AddrCycle(R) [1]     NAND flash addr size
    //                      when [3]==0, 0:3-addr, 1:4-addr.
    //                      when [3]==1, 0:4-addr, 1:5-addr.
    // BusWidth(R/W) [0]    NAND bus width. 0:8-bit, 1:16-bit.
    pg_nand_reg->NFCONF =(cn_talcs<<12)|(cn_twrph0<<8)|(cn_twrph1<<4)|(0<<0);

    //nand control register
    // Lock-tight   [13]    0:Disable lock, 1:Enable lock.
    // Soft Lock    [12]    0:Disable lock, 1:Enable lock.
    // EnablillegalAcINT[10] Illegal access interupt control.0:Disable,1:Enable
    // EnbRnBINT    [9] RnB interrupt. 0:Disable, 1:Enable
    // RnB_TrandMode[8] RnB transition detection config.0:Low->High,1:High->Low
    // SpareECCLock [6]     0:Unlock, 1:Lock
    // MainECCLock  [5]     0:Unlock, 1:Lock
    // InitECC(W)   [4]     1:Init ECC decoder/encoder.
    // Reg_nCE      [1]     0:nFCE=0, 1:nFCE=1.
    // NANDC Enable [0]     operating mode. 0:Disable, 1:Enable.
    pg_nand_reg->NFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)
                            |(1<<5)|(1<<4)|(1<<1)|(1<<0);
    __reset_nand();
    chip_id = __read_chip_id();
    if( __parse_chip(chip_id,&name) == false)
        return 0;
    pg_sector_buf = (u8*)M_Malloc(u32g_sector_size+u16g_oob_size,0);
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
    if(DFFSD_InstallChip(&tg_samsung_nand,name,cn_reserve_blocks))
        return 1;
    else
    {
        free(pg_sector_buf);
        return 0;
    }
}
#endif

