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
#ifndef _MDR_H_
#define _MDR_H_

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

void DFFSD_init_MDR(struct flash_chip *chip,uint32_t reserves);
bool_t __DFFSD_read_MDR_bak(struct flash_chip *chip,uint8_t *buf);
void __DFFSD_read_MDR_main(struct flash_chip *chip,uint8_t *buf);
bool_t __DFFSD_allocate_DBX(struct flash_chip *chip,
                            uint32_t size,uint32_t *result);
bool_t __DFFSD_write_MDR_DBX(struct flash_chip *chip,
                           uint32_t DBX_no,uint8_t *MDR_DBX_buf);
bool_t DFFSD_create_DBX(struct flash_chip *chip, uint32_t size,char *name);
uint32_t __DFFSD_probe_MDR_bak(struct flash_chip *chip);
bool_t __DFFSD_verify_MDR_CDR(uint8_t *buf);
bool_t __DFFSD_verify_MDR_DBX(uint8_t *buf);
bool_t __DFFSD_verify_end_flag(uint8_t *buf);
uint32_t __DFFSD_verify_MDR_record(uint8_t *buf);
void __DFFSD_uasm_roll(struct flash_chip *chip,uint8_t *MDR_buf,
                       struct MDR_temp_record *MDR_record,uint32_t end_offset);
bool_t __DFFSD_check_MDR_bak(struct flash_chip *chip);
uint32_t __DFFSD_find_roll_next(struct flash_chip *chip,uint8_t *MDR_buf);
ufast_t __DFFSD_uasm_DBX(struct flash_chip *chip,
                         struct MDR_temp_record *MDR_record,uint8_t *MDR_buf);
bool_t __DFFSD_read_MDR(struct flash_chip *chip,struct MDR_temp_record *MDR_record);
void __DFFSD_fill_ECC_MDR_record(uint8_t *buf);
void __DFFSD_fill_ECC_MDR_CDR(uint8_t *buf);
void __DFFSD_fill_ECC_MDR_DBX(uint8_t *buf);
void __DFFSD_write_end_flag_main(struct flash_chip *chip);
void __DFFSD_write_end_flag_bak(struct flash_chip *chip);
void __DFFSD_write_MDR_main(struct flash_chip *chip,uint8_t *MDR_buf,uint32_t len);
void __DFFSD_write_MDR_bak(struct flash_chip *chip,uint8_t *MDR_buf,uint32_t len);
void __DFFSD_write_MDR_DBX_main(struct flash_chip *chip,uint8_t *MDR_DBX_buf,
                                                      uint32_t DBX_no);
void __DFFSD_write_MDR_DBX_bak(struct flash_chip *chip,uint8_t *MDR_DBX_buf,
                                                 uint32_t DBX_no);
void __DFFSD_write_MDR_roll_record(struct flash_chip *chip,uint32_t DBX_no,
                                 uint32_t type,uint32_t value);
uint32_t __DFFSD_asm_roll_buf(struct flash_chip *chip,
                          struct MDR_temp_record *MDR_record,uint8_t *roll_buf);
void __DFFSD_write_roll_main(struct flash_chip *chip,struct MDR_temp_record *MDR_record);
void __DFFSD_write_roll_bak(struct flash_chip *chip,struct MDR_temp_record *MDR_record);
void __DFFSD_write_MDR_item(struct st_DBX_flash_tag *DBX_flash_tag,
                    uint32_t type,uint32_t value);
void __DFFSD_set_MDR_DBX_formatted(struct flash_chip *chip,uint32_t DBX_no);
void __DFFSD_erase_MDR_main(struct flash_chip *chip);
void __DFFSD_erase_MDR_bak(struct flash_chip *chip);


#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

#ifdef __cplusplus
}
#endif

#endif // _MDR_H_

