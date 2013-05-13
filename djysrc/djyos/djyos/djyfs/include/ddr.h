//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:flash文件系统
//作者：lst
//版本：V1.0.0
//文件描述:flash文件系统中DDR表的部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _DDR_H_
#define _DDR_H_

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

ufast_t __DFFSD_get_block_purpose(struct st_DBX_flash_tag *DBX_flash_tag,
                            uint32_t u32_pre_ptr,uint32_t u32_next_ptr,
                            uint32_t block_no);
void __DFFSD_rebuild_DDR_bak(struct st_DBX_flash_tag *DBX_flash_tag);
void __DFFSD_rebuild_DDR_main(struct st_DBX_flash_tag *DBX_flash_tag);
uint32_t __DFFSD_MAT_start(struct st_DBX_flash_tag *DBX_flash_tag);
bool_t __DFFSD_read_DDR(struct st_DBX_flash_tag *DBX_flash_tag,
                            ufast_t which);
bool_t __DFFSD_verify_DDR_bak(struct st_DBX_flash_tag *DBX_flash_tag);
bool_t __read_DDR_main(struct st_DBX_flash_tag *DBX_flash_tag);
bool_t __read_DDR_bak(struct st_DBX_flash_tag *DBX_flash_tag);
bool_t __DFFSD_scan_DDR(struct st_DBX_flash_tag *DBX_flash_tag);
void __DFFSD_ART_flip(struct st_DBX_flash_tag *DBX_flash_tag,
                            ufast_t from,ufast_t to,uint32_t position);
bool_t __DFFSD_exchange_busy0_free8(struct st_DBX_flash_tag *DBX_flash_tag);
uint32_t __DFFSD_allocate_block(struct st_DBX_flash_tag *DBX_flash_tag);
bool_t __DFFSD_write_DDR_exe(struct st_DBX_flash_tag *DBX_flash_tag,ufast_t which);
bool_t __DFFSD_write_DDR(struct st_DBX_flash_tag *DBX_flash_tag);
void __DFFSD_add_MAT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t last_block_no,uint32_t new_block_no);
void __DFFSD_update_MAT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                       uint32_t old_block_no,uint32_t new_block_no);
void __DFFSD_whirl_ART_table(struct st_DBX_flash_tag *DBX_flash_tag);
void __DFFSD_inc_abrasion_times(struct st_DBX_flash_tag *DBX_flash_tag,
                                uint32_t block_no);
void __DFFSD_register_invalid(struct st_DBX_flash_tag *DBX_flash_tag,
                            uint32_t block_no);
uint32_t __DFFSD_allocate_block_simple(struct st_DBX_flash_tag *DBX_flash_tag);
void __DFFSD_free_block(struct st_DBX_flash_tag *DBX_flash_tag,
                    uint32_t block_no);
void __DFFSD_exchange_ART_item(struct st_DBX_flash_tag *DBX_flash_tag,
                                uint32_t pos1,uint32_t pos2);
void __DFFSD_sort_ART(struct st_DBX_flash_tag *DBX_flash_tag);
uint32_t __DFFSD_calculate_DDR_size(struct st_DBX_flash_tag *DBX_flash_tag);

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

#ifdef __cplusplus
}
#endif

#endif // _DDR_H_
