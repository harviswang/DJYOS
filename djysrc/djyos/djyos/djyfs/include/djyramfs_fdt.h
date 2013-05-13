//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:flash文件系统
//作者：lst
//版本：V1.0.0
//文件描述:flash文件系统中FDR表部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------


bool_t __djyramfs_init_FDT(struct st_DBX_ram_tag *DBX_ram_tag);
bool_t __djyramfs_add_FDT_block(struct st_DBX_ram_tag *DBX_ram_tag);
void __djyramfs_add_FDT_free_item(struct st_DBX_ram_tag *DBX_ram_tag,
                         uint32_t FDT_item_no);
bool_t __djyramfs_acnode_FDT_item(struct st_DBX_ram_tag *DBX_ram_tag,
                         uint32_t FDT_item_no);
void __djyramfs_free_FDT_item(struct st_DBX_ram_tag *DBX_ram_tag,
                         uint32_t FDT_item_no);
uint32_t __djyramfs_allocate_FDT_item(struct st_DBX_ram_tag *DBX_ram_tag);
void __djyramfs_compress_FDT(struct st_DBX_ram_tag *DBX_ram_tag);
bool_t __djyramfs_write_FDT_item(struct st_DBX_ram_tag *DBX_ram_tag,
                           uint32_t FDT_item_no,struct ramfs_fdt *FDT_item);
void __djyramfs_read_FDT_item(struct st_DBX_ram_tag *DBX_ram_tag,
                           uint32_t FDT_item_no,struct ramfs_fdt *FDT_item);
void __djyramfs_asm_FDT_item(struct ramfs_fdt *FDT_item,union file_attrs attr,
                          char *name);
void __djyramfs_uasm_FDT_item(struct st_DBX_ram_tag *DBX_ram_tag,
                                struct file_rsc *fp,
                                struct ramfs_fdt *FDT_item);
void __djyramfs_uasm_root_folder(struct st_DBX_ram_tag *DBX_ram_tag,
                                struct ramfs_fdt *FDT_item);
void __djyramfs_update_FDT_fstart_dson(struct st_DBX_ram_tag *DBX_ram_tag,
                       uint32_t FDT_item_no,uint32_t new_block_no);
void __djyramfs_update_FDT_file_size(struct st_DBX_ram_tag *DBX_ram_tag,
                                  struct file_rsc *fp,sint64_t new_size);
void __djyramfs_update_FDT_item_name(struct st_DBX_ram_tag *DBX_ram_tag,
                                  struct file_rsc *fp,char *name);
void __djyramfs_update_FDT_item_attr(struct st_DBX_ram_tag *DBX_ram_tag,
                                  struct file_rsc *fp,union file_attrs attr);
//void __djyramfs_update_FDT_item_ctime(struct st_DBX_ram_tag *DBX_ram_tag,
//                                  struct file_rsc *fp,struct time new_time);
//void __djyramfs_update_FDT_item_utime(struct st_DBX_ram_tag *DBX_ram_tag,
//                                  struct file_rsc *fp,struct time new_time);


