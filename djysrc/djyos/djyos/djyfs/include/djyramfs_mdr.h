//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:flash文件系统
//作者：lst
//版本：V1.0.0
//文件描述:flash文件系统中MDR表部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------


void djyramfs_init_MDR(struct st_ram_chip *chip,uint32_t reserves);
bool_t __djyramfs_read_MDR_bak(struct st_ram_chip *chip,uint8_t *buf);
void __djyramfs_read_MDR_main(struct st_ram_chip *chip,uint8_t *buf);
bool_t __djyramfs_allocate_DBX(struct st_ram_chip *chip,
                            uint32_t size,uint32_t *result);
bool_t __djyramfs_write_MDR_DBX(struct st_ram_chip *chip,
                           uint32_t DBX_no,uint8_t *MDR_DBX_buf);
bool_t djyramfs_create_DBX(struct st_ram_chip *chip, uint32_t size,char *name,
                                                bool_t nand_ecc);
uint32_t __djyramfs_probe_MDR_bak(struct st_ram_chip *chip);
bool_t __djyramfs_verify_MDR_CDR(uint8_t *buf);
bool_t __djyramfs_verify_MDR_DBX(uint8_t *buf);
bool_t __djyramfs_verify_end_flag(uint8_t *buf);
uint32_t __djyramfs_verify_MDR_record(uint8_t *buf);
void __djyramfs_uasm_roll(struct st_ram_chip *chip,uint8_t *MDR_buf,
                       struct MDR_temp_record_ramfs *MDR_record,uint32_t end_offset);
bool_t __djyramfs_check_MDR_bak(struct st_ram_chip *chip);
uint32_t __djyramfs_find_roll_next(struct st_ram_chip *chip,uint8_t *MDR_buf);
ufast_t __djyramfs_uasm_DBX(struct st_ram_chip *chip,
                         struct MDR_temp_record_ramfs *MDR_record,uint8_t *MDR_buf);
bool_t __djyramfs_read_MDR(struct st_ram_chip *chip,struct MDR_temp_record_ramfs *MDR_record);
void __djyramfs_fill_ECC_MDR_record(uint8_t *buf);
void __djyramfs_fill_ECC_MDR_CDR(uint8_t *buf);
void __djyramfs_fill_ECC_MDR_DBX(uint8_t *buf);
void __djyramfs_write_end_flag_main(struct st_ram_chip *chip);
void __djyramfs_write_end_flag_bak(struct st_ram_chip *chip);
void __djyramfs_write_MDR_main(struct st_ram_chip *chip,uint8_t *MDR_buf,uint32_t len);
void __djyramfs_write_MDR_bak(struct st_ram_chip *chip,uint8_t *MDR_buf,uint32_t len);
void __djyramfs_write_MDR_DBX_main(struct st_ram_chip *chip,uint8_t *MDR_DBX_buf,
                                                      uint32_t DBX_no);
void __djyramfs_write_MDR_DBX_bak(struct st_ram_chip *chip,uint8_t *MDR_DBX_buf,
                                                 uint32_t DBX_no);
void __djyramfs_write_MDR_roll_record(struct st_ram_chip *chip,uint32_t DBX_no,
                                 uint32_t type,uint32_t value);
uint32_t __djyramfs_asm_roll_buf(struct st_ram_chip *chip,
                          struct MDR_temp_record_ramfs *MDR_record,uint8_t *roll_buf);
void __djyramfs_write_roll_main(struct st_ram_chip *chip,struct MDR_temp_record_ramfs *MDR_record);
void __djyramfs_write_roll_bak(struct st_ram_chip *chip,struct MDR_temp_record_ramfs *MDR_record);
void __djyramfs_write_MDR_item(struct st_DBX_ram_tag *DBX_ram_tag,
                    uint32_t type,uint32_t value);
void __djyramfs_set_MDR_DBX_formatted(struct st_ram_chip *chip,uint32_t DBX_no);
void __djyramfs_erase_MDR_main(struct st_ram_chip *chip);
void __djyramfs_erase_MDR_bak(struct st_ram_chip *chip);

