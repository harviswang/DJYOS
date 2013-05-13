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

ufast_t __djyramfs_get_block_purpose(struct st_DBX_ram_tag *DBX_ram_tag,
                            uint32_t u32_pre_ptr,uint32_t u32_next_ptr,
                            uint32_t block_no);
void __djyramfs_rebuild_DDR_bak(struct st_DBX_ram_tag *DBX_ram_tag);
void __djyramfs_rebuild_DDR_main(struct st_DBX_ram_tag *DBX_ram_tag);
uint32_t __djyramfs_MAT_start(struct st_DBX_ram_tag *DBX_ram_tag);
bool_t __djyramfs_read_DDR(struct st_DBX_ram_tag *DBX_ram_tag,
                            ufast_t which);
bool_t __djyramfs_verify_DDR_bak(struct st_DBX_ram_tag *DBX_ram_tag);
bool_t __read_DDR_main(struct st_DBX_ram_tag *DBX_ram_tag);
bool_t __read_DDR_bak(struct st_DBX_ram_tag *DBX_ram_tag);
bool_t __djyramfs_scan_DDR(struct st_DBX_ram_tag *DBX_ram_tag);
void __djyramfs_ART_flip(struct st_DBX_ram_tag *DBX_ram_tag,
                            ufast_t from,ufast_t to,uint32_t position);
bool_t __djyramfs_exchange_busy0_free8(struct st_DBX_ram_tag *DBX_ram_tag);
uint32_t __djyramfs_allocate_block(struct st_DBX_ram_tag *DBX_ram_tag);
bool_t __djyramfs_write_DDR_exe(struct st_DBX_ram_tag *DBX_ram_tag,ufast_t which);
bool_t __djyramfs_write_DDR(struct st_DBX_ram_tag *DBX_ram_tag);
void __djyramfs_add_MAT_item(struct st_DBX_ram_tag *DBX_ram_tag,
                           uint32_t last_block_no,uint32_t new_block_no);
void __djyramfs_update_MAT_item(struct st_DBX_ram_tag *DBX_ram_tag,
                       uint32_t old_block_no,uint32_t new_block_no);
void __djyramfs_whirl_ART_table(struct st_DBX_ram_tag *DBX_ram_tag);
void __djyramfs_inc_abrasion_times(struct st_DBX_ram_tag *DBX_ram_tag,
                                uint32_t block_no);
void __djyramfs_register_invalid(struct st_DBX_ram_tag *DBX_ram_tag,
                            uint32_t block_no);
uint32_t __djyramfs_allocate_block_simple(struct st_DBX_ram_tag *DBX_ram_tag);
void __djyramfs_free_block(struct st_DBX_ram_tag *DBX_ram_tag,
                    uint32_t block_no);
void __djyramfs_exchange_ART_item(struct st_DBX_ram_tag *DBX_ram_tag,
                                uint32_t pos1,uint32_t pos2);
void __djyramfs_sort_ART(struct st_DBX_ram_tag *DBX_ram_tag);
uint32_t __djyramfs_calculate_DDR_size(struct st_DBX_ram_tag *DBX_ram_tag);


