//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 文件系统芯片驱动
//作者：lst
//版本：V1.0.0
//文件描述: 用于三星nand flash的文件系统驱动模块
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 移植自yf44b0
//------------------------------------------------------
#ifndef _samsung_nand_H_
#define _samsung_nand_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NandFlash_WP_GPIOD3        	GPIO_Pin_3
#define NandFlash_RB_GPIOD6        		GPIO_Pin_6
#define NandFlash_CE_GPIOG9        		GPIO_Pin_9

#define Bank2_NAND_ADDR    ((uint32_t)0x70000000)
#define Bank3_NAND_ADDR    ((uint32_t)0x80000000)

#define Bank_NAND_ADDR     Bank3_NAND_ADDR

#define CMD_AREA                   (uint32_t)(1<<16)  			/* A16 = CLE  high */
#define ADDR_AREA                  (uint32_t)(1<<17)  			/* A17 = ALE high */

#define cn_oob_size     16
#define cn_reserve_blocks   0                   //定义一个保留区，紧跟MDR表

//下面定义各操作等待时间，单位微秒
#define cn_wait_address_ready   10
#define cn_wait_page_write      200
#define cn_wait_block_erase     2000
#define cn_wait_reset           500

//ecc校验操作结果
#define cn_all_right_verify     0               //完全正确
#define cn_ecc_right_verify     1               //经ecc纠正正确
#define cn_ecc_error_verify     2               //错误，不能纠正
#define cn_other_error_verify   cn_limit_uint32 //其他错误，一般是非法参数

#define CN_FS_BUF_TICKS (1000)      //文件自动写入时间,降低掉电损失
#define CN_FS_BUF_BLOCKS (16)       //文件缓冲块数,

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

struct nand_chip_id
{
  uint8_t Maker_ID;
  uint8_t Device_ID;
  uint8_t Third_ID;
  uint8_t Fourth_ID;
};

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
bool_t __erase_all_nand(char *param);
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
bool_t __parse_chip(uint16_t id,char **name);
void __reset_nand(void);
ptu32_t module_init_fs_nandflash(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // _samsung_nand_H_

