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
//�汾��V1.0.0
//�ļ�����: ��������nand flash���ļ�ϵͳ����ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ��ֲ��yf44b0
//------------------------------------------------------
#ifndef __k9fxxx_h__
#define __k9fxxx_h__

#ifdef __cplusplus
extern "C" {
#endif

//#include "os_inc.h"
//nand flashоƬ��С����
#define cn_nand_little_block    0
#define cn_nand_big_block       1
#define cn_nand_block           cn_nand_big_block

#if (cn_nand_little_block == cn_nand_block)

#define cn_oob_size     16
#define cn_reserve_blocks   0                   //����һ��������������MDR��

//���涨��������ȴ�ʱ�䣬��λ΢��
#define cn_wait_address_ready   10
#define cn_wait_page_write      200
#define cn_wait_block_erase     2000
#define cn_wait_reset           500

//eccУ��������
#define cn_all_right_verify     0               //��ȫ��ȷ
#define cn_ecc_right_verify     1               //��ecc������ȷ
#define cn_ecc_error_verify     2               //���󣬲��ܾ���
#define cn_other_error_verify   CN_LIMIT_UINT32 //��������һ���ǷǷ�����


#define nand_ce_bit     (0x40)
#define nand_busy_bit   (0x20)
#define nand_cle_bit    (0x80)
#define nand_ale_bit    (0x100)

// HCLK=100Mhz
#define cn_talcs        1  //1clk(10ns)
#define cn_twrph0       4  //3clk(30ns)
#define cn_twrph1       0  //1clk(10ns)  //cn_talcs+cn_twrph0+cn_twrph1>=50ns


#define ce_active()     (pg_nand_reg->NFCONT &= ~(1<<1))
#define ce_inactive()   (pg_nand_reg->NFCONT |= (1<<1))

#define CN_FS_BUF_TICKS (1000)      //�ļ��Զ�д��ʱ��,���͵�����ʧ
#define CN_FS_BUF_BLOCKS (16)       //�ļ��������,

// Flash commands:
#define cn_nand_select_page0        0x00
#define cn_nand_select_page1        0x01
#define cn_nand_select_oob          0x50
#define cn_nand_reset               0xff
#define cn_nand_page_program        0x80
#define cn_nand_startup_write       0x10
#define cn_nand_block_erase         0x60
#define cn_nand_startup_erase       0xd0
#define cn_nand_read_status         0x70
#define cn_nand_read_id             0x90

#define cn_nand_failure             0x01
#define RB                          0x40
#endif

#if (cn_nand_big_block==1)

#define cn_oob_size     64
#define cn_reserve_blocks   0                   //����һ��������������MDR��

//���涨��������ȴ�ʱ�䣬��λ΢��
#define cn_wait_address_ready   10
#define cn_wait_page_write      200
#define cn_wait_block_erase     2000
#define cn_wait_reset           500

//eccУ��������
#define cn_all_right_verify     0               //��ȫ��ȷ
#define cn_ecc_right_verify     1               //��ecc������ȷ
#define cn_ecc_error_verify     2               //���󣬲��ܾ���
#define cn_other_error_verify   CN_LIMIT_UINT32 //��������һ���ǷǷ�����

#define nand_ce_bit     (0x40)
#define nand_busy_bit   (0x20)
#define nand_cle_bit    (0x80)
#define nand_ale_bit    (0x100)

// HCLK=100Mhz
#define cn_talcs        1  //1clk(10ns)
#define cn_twrph0       4  //3clk(30ns)
#define cn_twrph1       0  //1clk(10ns)  //cn_talcs+cn_twrph0+cn_twrph1>=50ns
// HCLK=50Mhz
//#define cn_talcs      0  //1clk(20ns)
//#define cn_twrph0     1  //2clk(40ns)
//#define cn_twrph1     0  //1clk(20ns)

#define ce_active()     (pg_nand_reg->NFCONT &= ~(1<<1))
#define ce_inactive()   (pg_nand_reg->NFCONT |= (1<<1))

#define CN_FS_BUF_TICKS (1000)      //�ļ��Զ�д��ʱ��,���͵�����ʧ
#define CN_FS_BUF_BLOCKS (16)       //�ļ��������,

// Flash commands:
#define cn_nand_page_read        0x00
#define cn_nand_startup_read         0x30
#define cn_nand_reset               0xff
#define cn_nand_page_program        0x80
#define cn_nand_startup_write       0x10
#define cn_nand_block_erase         0x60
#define cn_nand_startup_erase       0xd0
#define cn_nand_read_status         0x70
#define cn_nand_read_id             0x90

#define cn_nand_failure             0x01
#define RB                          0x40


#endif

struct nand_table
{
    uint16_t vendor_chip_id;
    u16 oob_size;
    uint16_t pages_per_block;
    u32 blocks_sum;
    u32 block_size;
    char *chip_name;
};
void __write_address_nand(uint32_t addr);
void __read_sector_and_oob(u32 sector,u8 *data);
bool_t write_PCRB_nand(u32 PCRB_block,
                       u32 protected_block,u8 *buf);
bool_t restore_PCRB_nand(u32 PCRB_block,u32 *restored);
bool_t __wait_ready_nand(void);
bool_t __wait_ready_nand_slow(uint16_t wait_time);
void __write_command_nand(u8 val);
u32 __read_sector_nand_no_ecc(u32 sector,u32 offset,
                                 u8 *data,u32 size);
u32 read_block_ss_no_ecc(u32 block,u32 offset,
                         u8 *buf,u32 size);
u32 __correct_sector(u8 *data,const u8 *old_ecc);
u32 __read_sector_nand_with_ecc(u32 sector,u32 offset,
                                 u8 *data,u32 size);
u32 read_block_ss_with_ecc(u32 block,u32 offset,
                         u8 *buf,u32 size);
u8 __read_status_nand(void);
u32 __write_sector_nand_no_ecc(u32 sector,u32 offset,
                                 u8 *data,u32 size);
u32 write_block_ss_no_ecc(u32 block,u32 offset,
                          u8 *buf,u32 size);
u32 __write_sector_nand_with_ecc(u32 sector,u32 offset,
                                 u8 *data,u32 size);
u32 write_block_ss_with_ecc(u32 block,u32 offset,
                          u8 *buf,u32 size);
bool_t erase_block_nand(u32 block_no);
bool_t __erase_all_nand(void);
bool_t query_block_ready_ss_with_ecc(u32 block,u32 offset,
                              u8 *buf,u32 size);
bool_t query_block_ready_nand_no_ecc(u32 block,u32 offset,
                              u8 *buf,u32 size);
bool_t query_ready_with_data_nand(u8 *new_data,u8 *org_data,
                                        u32 size);
bool_t check_block_nand(u32 block_no);
bool_t __mark_invalid_block(u32 block);
u32 __check_all_ss(void);
void __make_sector_ecc(const u8 *data,u8 *ecc);
uint16_t __read_chip_id (void);
bool_t __parse_chip(uint16_t id,char **name);
void __reset_nand(void);
ptu32_t ModuleInstall_FsNandFlash(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // __k9fxxx_h__

