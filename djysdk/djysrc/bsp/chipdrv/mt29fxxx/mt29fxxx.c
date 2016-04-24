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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��MT29Fxxx.c
// ģ������: ����DJYOS�ļ�ϵͳ��Ƶ�NANDFALSH����������NFCģ��Ľӿں���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 20/10.2014
// =============================================================================

#include "stdint.h"
#include "verify.h"
#include "file.h"
#include "flashfile.h"
#include "cpu_peri.h"
#include <string.h>
#include "MT29Fxxx.h"


//����������ָ�룬������д�����ڲ�ר�ã�djyffs�У�������������flash�Ĺ��ԣ��ʲ�
//������struct flash_chip �ṹ�С���̬�����ڴ��������ɾ�̬���飬����Ϊchip��
//Ϊһ����Դ���ǿ���ɾ���ģ�ɾ��ʱ���Ի�����Դ��
static u8 *pg_sector_buf;      //����������ָ��
static u32 u32g_sectors_per_block; //ÿ�������������
static u32 u32g_sector_size;      //�������ߴ�
static u16 u16g_oob_size;

static  struct tagFlashChip tg_micron_nand;    //оƬ

struct nand_table tg_nand_table[] =
{
    {0x9876,16,32,4096,16384,"Toshiba TH58512FT,1.8v,64Mbytes"},
    {0xec36,16,32,4096,16384,"samsung k9f1208,1.8v,64Mbytes"},
    {0xec76,16,32,4096,16384,"samsung k9f1208,3.3v,64Mbytes"},
    {0xec73,16,32,1024,16384,"samsung k9f2808,3.3v,16Mbytes"},
    {0xec33,16,32,1024,16384,"samsung k9f2808,1.8v,16Mbytes"},
    {0xecda,64,64,2048,131072,"samsung K9F2G08,3.3v,256Mbytes"},
    {0x2CCA,64,64,2048,131072,"Micron MT29F2G16,1.8V,256Mbytes"}
};
//----д������-----------------------------------------------------------------
//����: д��оƬд������
//����: val��д�����֣�
//����: ��
//-----------------------------------------------------------------------------
#define __write_command_nand(cmd)   //pg_nand_reg->NFCMD = cmd

//----д���ַ-----------------------------------------------------------------
//����: д��оƬд������,5�зֱ�д��A0 ~ A7��A9 ~ A11��A12 ~ A19,A20~A27
//����: val��д�����֣�
//����: ��
//----------------------------------------------------------------------------
void __AddrToRowCol(u32 Addr,u32* RowAddr,u32 *ColAddr)
{
    *ColAddr = Addr & 0x7FF;
    *RowAddr = (Addr >> 11) & 0x1FFFF;
}

//----������(��oob)------------------------------------------------------------
//����: ��һ����������oob�е�����һ���
//����: sector��������������
//      data���������ݵĻ�����
//����: ��
//-----------------------------------------------------------------------------
void __read_sector_and_oob(u32 sector,u8 *data)
{
//    u32 i;
    u32 address,RowAddr,ColAddr;
    address =u32g_sector_size*sector;     //����ʵ�ʵ�ַ

    __AddrToRowCol(address,&RowAddr,&ColAddr);
    NFC_CsActive(NFC_CE0);
    NFC_PageRead(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand( );

    NFC_ReadBuffer(data,0,2048+64);
    NFC_CsInactive(NFC_CE0);
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
    u32 address,RowAddr,ColAddr;
    u32 loop;

    if(PCRB_block >= tg_micron_nand.block_sum)
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
    address =tg_micron_nand.block_size*PCRB_block +
            (u32g_sector_size + u16g_oob_size-4)/2;//��ַ����16����//todo

    //�ܱ�����Ŀ����д��flash��ע��:û����ECCУ�飬�Ժ���---db
    __AddrToRowCol(address,&RowAddr,&ColAddr);
    NFC_WriteBuffer((u8*)&protected_block,0,4);

    NFC_CsActive(NFC_CE0);
    NFC_PageProgram(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand_slow(CN_WAIT_PAGE_WRITE);

    if(__read_status_nand() & cn_nand_failure)
    {
        NFC_CsInactive(NFC_CE0);
        return false;
    }
    NFC_CsInactive(NFC_CE0);

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
    u32 address,RowAddr,ColAddr;
    u32 loop;
    u32 protected_block;
    u32 sector_from,sector_to;
    u8 *sector_buf;   //��������������̬���䣬����ջ�з���
    bool_t result = true;

    if(PCRB_block >= tg_micron_nand.block_sum)
        return false;
    //��һҳ�����4�ֽڱ����ܱ�����Ŀ���š�
    address =tg_micron_nand.block_size*PCRB_block +
            (u32g_sector_size + u16g_oob_size-4)/2;//��ַ����16λ��

    __AddrToRowCol(address,&RowAddr,&ColAddr);
    NFC_CsActive(NFC_CE0);
    NFC_PageRead(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand( );

    NFC_ReadBuffer((u8*)&protected_block,0,4);

    *restored = protected_block;

    if(protected_block > tg_micron_nand.block_sum)
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
    NFC_CsInactive(NFC_CE0);
    return result;
}

//----�ȴ�оƬ�ڲ���ɲ���----------------------------------------------------
//����: ��оƬִ��д������ҪЧ�ñ������ȴ�������ɲ��ܽ�һ��������
//����: ��
//����: true = ��ȷ��ɲ�����false = ��������
//-----------------------------------------------------------------------------
bool_t __wait_ready_nand(void)
{
    while(!NFC_OptionCompleted());
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
//    timer_set_counter(1,wait_time);     //����ֵ��Ϊwait_time
//    timer_reload(1);                    //���ض�ʱֵ
//    timer_start(1);                     //������ʱ��
//    int_asyn_signal_sync(cn_int_line_timer1);
//    timer_stop(1);
//    while((pg_nand_reg->NFSTAT & 1) ==0 );

    u32 timestart,timeend;

    timestart = (u32)DjyGetTime( );
    timeend = timestart;

    while ( (timeend - timestart) < wait_time)
    {
      if(NFC_OptionCompleted())
       {
          break;
       }
      timeend = (u32)DjyGetTime( );
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
//    u32 i;
    u32 address,RowAddr,ColAddr;
    address =u32g_sector_size*sector;     //����ʵ�ʵ�ַ
    //wjz code revision 256M�ģ���64M�ģ����˸�0x30�������������ǵȴ�ʱ����
    //data[i],ǰ��һЩ���ݣ������������.
    NFC_CsActive(NFC_CE0);
    __AddrToRowCol(address,&RowAddr,&ColAddr);
    NFC_PageRead(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand( );

    NFC_ReadBuffer(data,offset,size);
    NFC_CsInactive(NFC_CE0);
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

    if(block >= tg_micron_nand.block_sum)
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_micron_nand.block_size)
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
    u32 address,result,RowAddr,ColAddr;
    u8 *ecc;
    address =u32g_sector_size*sector;     //����ʵ�ʵ�ַ
    ecc = pg_sector_buf+u32g_sector_size;

    NFC_CsActive(NFC_CE0);
    __AddrToRowCol(address,&RowAddr,&ColAddr);
    NFC_PageRead(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand( );

    NFC_ReadBuffer(pg_sector_buf,0,u32g_sector_size);

   if(u32g_sector_size > 256)
    {
       //OOB�ĵ�һ���ֽ�Ϊ�����ǣ���˲���
       NFC_ReadBuffer(ecc,u32g_sector_size+1,u32g_sector_size/256*3);
    }
    else
    {
        //��ȡУ���룬ֻ��3�ֽ�
        NFC_ReadBuffer(ecc,u32g_sector_size,3);
    }

   NFC_CsInactive(NFC_CE0);
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

    if(block >= tg_micron_nand.block_sum)
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_micron_nand.block_size)
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

    chip_status = NFC_FlashStatusRead();
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
    u32 address,RowAddr,ColAddr;
    address =u32g_sector_size*sector;     //����ʵ�ʵ�ַ

    if(offset != 0)
    {
        __read_sector_nand_with_ecc(sector,0,pg_sector_buf,u32g_sector_size);
        memcpy(pg_sector_buf+offset,data,size);
        NFC_WriteBuffer(pg_sector_buf,0,u32g_sector_size);
    }
    else
    {
        memset(pg_sector_buf,0xFF,u32g_sector_size);
        memcpy(pg_sector_buf,data,size);
        NFC_WriteBuffer(pg_sector_buf,0,u32g_sector_size);
    }

    __AddrToRowCol(address,&RowAddr,&ColAddr);

    NFC_CsActive(NFC_CE0);
    NFC_PageProgram(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand_slow(CN_WAIT_PAGE_WRITE);

    if(__read_status_nand() & cn_nand_failure)
    {
        NFC_CsInactive(NFC_CE0);
        return cn_ecc_error_verify;
    }
    NFC_CsInactive(NFC_CE0);
    return cn_all_right_verify;
}

//----д��(����eccУ��)----------------------------------------------------
//����: ����eccУ��ѻ�����д����ڣ���ַ���ܿ��߽�
//����: sector�����
//      offset������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//���أ�ʵ��д�����������CN_LIMIT_UINT32��ʾ����
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

    if(block >= tg_micron_nand.block_sum)
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_micron_nand.block_size)
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
    u32 address,RowAddr,ColAddr;
    u8 *ecc,temp;
    if((offset != 0) || (size != u32g_sector_size))
    {
        __read_sector_nand_with_ecc(sector,0,pg_sector_buf,u32g_sector_size);
        //ִ��ECCУ�飬���ǲ��ж�У��������Ϊ����д��ǰ������������ݣ�У���
        //�󲢲���˵�������˴���
    }
    else
        memset(pg_sector_buf,0xFF,u32g_sector_size+u16g_oob_size);

    ecc = pg_sector_buf+u32g_sector_size;
    memcpy(pg_sector_buf+offset,data,size);
    __make_sector_ecc(pg_sector_buf, ecc);               //����ecc����

    address=u32g_sector_size*sector;  //д����ʼ��ַ

    NFC_WriteBuffer(pg_sector_buf,0,u32g_sector_size);//�ӻ������ж�������
    if(u32g_sector_size > 256)
    {
        temp = 0xFF;                                //д0xFF�������flash�޸�
        NFC_WriteBuffer(&temp,u32g_sector_size,1);
        NFC_WriteBuffer(ecc,u32g_sector_size+1,u32g_sector_size/256*3);//дECC
        i = u32g_sector_size/256*3 + 1;
        for(; i < 64; i ++)                         //64�ֽ���ʣ�µ�д0xFF
            NFC_WriteBuffer(&temp,u32g_sector_size+i,1);
    }
    else
    {
        NFC_WriteBuffer(ecc,u32g_sector_size,3);
    }

    NFC_CsActive(NFC_CE0);
    __AddrToRowCol(address,&RowAddr,&ColAddr);
    NFC_PageProgram(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand_slow(CN_WAIT_PAGE_WRITE);

    if(__read_status_nand() & cn_nand_failure)      //�ж��Ƿ���ɹ�
    {
        NFC_CsInactive(NFC_CE0);
        return cn_ecc_error_verify;
    }
    NFC_CsInactive(NFC_CE0);
    return cn_all_right_verify;
}

//----д��(��eccУ��)----------------------------------------------------
//����: ��eccУ��ѻ�����д����ڣ���ַ���ܿ��߽�
//����: sector�����
//      offset������ƫ�Ƶ�ַ
//      data�������ȡ���ݵĻ�����
//      size����ȡ�ĳߴ�
//���أ�ʵ��д�����������CN_LIMIT_UINT32��ʾ����
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

    if(block >= tg_micron_nand.block_sum)
        return CN_LIMIT_UINT32;
    if((size + offset) > tg_micron_nand.block_size)
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

    NFC_CsActive(NFC_CE0);
    // �е�ַ����Ϊ�е�ַ���������block�ź�page�ţ�оƬ��ʡ��page��
    NFC_BlockErase(page_no);

    __wait_ready_nand_slow(CN_WAIT_BLOCK_ERASE);
    NFC_CsInactive(NFC_CE0);
    return true;
}

//----��������оƬ-------------------------------------------------------------
//����: ��������оƬ��
//����: ��
//����: true = �ɹ�������false = ����
//-----------------------------------------------------------------------------
bool_t __erase_all_nand(void)
{
    u32 i;
    for(i=0; i<tg_micron_nand.block_sum; i++)
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
    u32 start_offset;
    u32 end_sector;    //������ַ��������
    u32 cur_sector;    //��ǰ���ڶ�������
    u32 address,ColAddr,RowAddr;
    u32 loop;
    u8 data;

    if(block >= tg_micron_nand.block_sum)
        return false;
    if((size + offset) > tg_micron_nand.block_size)
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
    start_offset = offset;
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        NFC_CsActive(NFC_CE0);                          //����Ƭѡ
        __AddrToRowCol(address,&RowAddr,&ColAddr);      //��ַת��
        NFC_PageRead(BufferNo,RowAddr,ColAddr);         //д����
        __wait_ready_nand( );                           //�ȴ����
        for(loop=0; loop < u32g_sector_size+cn_oob_size; loop++)
        {
            NFC_ReadBuffer(&data,start_offset + loop,1);//��һsector��Ҫƫ��
            if(data != 0xff)            //ֻҪ��1�ֽڷ�oxff,�����ش���
            {
                NFC_CsInactive(NFC_CE0);
                return false;
            }
        }
        NFC_CsInactive(NFC_CE0);                        //�ر�Ƭѡ
        address += u32g_sector_size;
        start_offset = 0;                               //�ӵڶ���sector��ƫ��Ϊ0
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
    u32 address,ColAddr,RowAddr;
    u32 completed = 0;
    u32 loop;
    u8 data;

    if(block >= tg_micron_nand.block_sum)
        return false;
    if((size + offset) > tg_micron_nand.block_size)
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
    address = block * tg_micron_nand.block_size;
    for(cur_sector = start_sector; cur_sector <= end_sector; cur_sector++)
    {
        if(cur_sector != end_sector)    //��ǰ�����������һ������
            read_size = u32g_sector_size - start_offset;
        else    //��ǰ���������һ������
            //+1����Ϊend_offset��������Ҫд���
            read_size = end_offset - start_offset +1;

        NFC_CsActive(NFC_CE0);
        __AddrToRowCol(address,&RowAddr,&ColAddr);
        NFC_PageRead(BufferNo,RowAddr,ColAddr);
        __wait_ready_nand( );

        for(loop=0; loop < read_size; loop++)
        {
            NFC_ReadBuffer(&data,start_offset + loop,1);//��ȡ����
            if((data | buf[completed]) != data)         //�鿴�з�0->1��λ
            {
                NFC_CsInactive(NFC_CE0);                //�ر�Ƭѡ
                return false;                           //��0->1��λ��������Ҫ����
            }
            completed++;
        }

        NFC_CsInactive(NFC_CE0);                        //�ر�Ƭѡ
        address += read_size;
        start_offset = 0;                       //�ӵڶ���������ʼ���϶���0��ʼ��
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
    u32 address,RowAddr,ColAddr;
    u8 data;

    NFC_CsActive(NFC_CE0);

    //OBB�ĵ�һ���ֽ�Ϊ0xFF��Ϊ�ÿ飬16λ������ʱ�������е�ַ����2
    address = tg_micron_nand.block_size * block_no + u32g_sector_size/2;

    __AddrToRowCol(address,&RowAddr,&ColAddr);
    NFC_PageRead(BufferNo,RowAddr,ColAddr);
    __wait_ready_nand( );

    NFC_ReadBuffer(&data,0,1);                  //todo

    if(data != 0xFF)
    {
        NFC_CsInactive(NFC_CE0);
        return  false;
    }

    NFC_CsInactive(NFC_CE0);
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
    u32 address,RowAddr,ColAddr;
    u8 temp = 0,i;

    address = tg_micron_nand.block_size * block_no + u32g_sector_size/2;
    __AddrToRowCol(address,&RowAddr,&ColAddr);

    NFC_WriteBuffer(&temp,0,1);                     //д0ʱ���Ϊ����
    temp = 0xFF;
    for(i = 0; i < u16g_oob_size-1;i++)
    {
        NFC_WriteBuffer(&temp,i+1,1);
    }
    NFC_CsActive(NFC_CE0);
    NFC_PageProgram(BufferNo,RowAddr,ColAddr);      //���
    __wait_ready_nand_slow(CN_WAIT_PAGE_WRITE);     //�ȴ�оƬ�ڲ��������

    if(__read_status_nand() & cn_nand_failure)
    {
        NFC_CsInactive(NFC_CE0);
        return false;
     }
    NFC_CsInactive(NFC_CE0);                        //����Ƭѡ
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
    u32 address,RowAddr,ColAddr;

    for(i=0; i<tg_micron_nand.block_sum; i++)
    {
        NFC_CsActive(NFC_CE0);
        address = tg_micron_nand.block_size * i + u32g_sector_size/2;

        __AddrToRowCol(address,&RowAddr,&ColAddr);

        NFC_PageRead(BufferNo,RowAddr,ColAddr);
        __wait_ready_nand( );
    }

    NFC_CsInactive(NFC_CE0);
    return sum;
}

//----��������ecc��-----------------------------------------------------------
//����: data�а���256�ֽ����ݣ���������ecc��
//����: data�����ݻ�����
//      ecc: 3�ֽڵĻ�����������ecc��
//����: ��
//-----------------------------------------------------------------------------
void __make_sector_ecc(u8 *data,u8 *ecc)
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
    u32 id0;
    NFC_CsActive(NFC_CE0);

    NFC_FlashIDRead(BufferNo,&id0,NULL);
    id = (uint16_t)(id0>>16);

    NFC_CsInactive(NFC_CE0);
    return id ;
}

//----����оƬid---------------------------------------------------------------
//����: ����оƬidʶ��оƬ,ʶ������д��tg_micron_nand��
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
            tg_micron_nand.block_sum = tg_nand_table[no].blocks_sum;
            tg_micron_nand.block_size = tg_nand_table[no].block_size;
            tg_micron_nand.PTT_read_buf_size = tg_nand_table[no].block_size;
            tg_micron_nand.PTT_write_buf_size = tg_nand_table[no].block_size;
            *name = tg_nand_table[no].chip_name;
            u16g_oob_size = tg_nand_table[no].oob_size;
            u32g_sector_size =tg_micron_nand.block_size/u32g_sectors_per_block;
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
    NFC_CsActive(NFC_CE0);

    // ����Reset ����
    NFC_Reset();

    __wait_ready_nand_slow(CN_WAIT_RESET);  //�ȴ�оƬ�ڲ��������
    NFC_CsInactive(NFC_CE0);
}

//----��ʼ��nandоƬ-----------------------------------------------------------
//���ܣ���ʼ��nandоƬ�����ļ�ϵͳ
//����: ��
//����: 1= �ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_FsNandFlash(const char *FlashHeapName)
{
    uint16_t chip_id;
    char *name;
    pHeap_t MyHeap;

    // ��ʱ����ʼ�������ڵȴ�������ɶ�ʱ---todo
    NFC_ModuleInit();

    __reset_nand();
    chip_id = __read_chip_id();
    if( __parse_chip(chip_id,&name) == false)
        return 0;
    MyHeap = M_FindHeap(FlashHeapName);
    if(MyHeap == NULL)
        return 0;
    tg_micron_nand.ChipHeap = MyHeap;

    pg_sector_buf = (u8*)M_MallocHeap(u32g_sector_size+u16g_oob_size,MyHeap,0);
    if(pg_sector_buf == NULL)
    {
        return 0;
    }
    tg_micron_nand.query_block_ready_with_ecc= query_block_ready_ss_with_ecc;
    tg_micron_nand.query_block_ready_no_ecc = query_block_ready_nand_no_ecc;
    tg_micron_nand.query_ready_with_data = query_ready_with_data_nand;
    tg_micron_nand.erase_block = erase_block_nand;
    tg_micron_nand.check_block = check_block_nand;
    tg_micron_nand.read_data_with_ecc = read_block_ss_with_ecc;
    tg_micron_nand.write_data_with_ecc = write_block_ss_with_ecc;
    tg_micron_nand.read_data_no_ecc = read_block_ss_no_ecc;
    tg_micron_nand.write_data_no_ecc = write_block_ss_no_ecc;
    tg_micron_nand.write_PCRB = write_PCRB_nand;
    tg_micron_nand.restore_PCRB = restore_PCRB_nand;
    if(DFFSD_InstallChip(&tg_micron_nand,name,cn_reserve_blocks))
        return 1;
    else
    {
        free(pg_sector_buf);
        return 0;
    }
}
bool_t nand_test(void)
{
    ModuleInstall_FsNandFlash(0);
    u8 buf[1024];
    u32 i;

    __reset_nand();

    erase_block_nand(10);

    __read_sector_nand_with_ecc(10*0x40,0,buf,1024);

    for(i=0;i<1024;i++)
        buf[i] = i;

    __write_sector_nand_with_ecc(10*0x40,0,buf,1024);

    __read_sector_nand_with_ecc(10*0x40,0,buf,1024);

    return true;
}

bool_t nand_reset(void)
{
    NFC_ModuleInit();
    return true;
}
