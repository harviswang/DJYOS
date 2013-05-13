//------------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:flash�ļ�ϵͳ��FDR����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _FDT_H_
#define _FDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

bool_t __DFFSD_init_FDT(struct st_DBX_flash_tag *DBX_flash_tag);
bool_t __DFFSD_add_FDT_block(struct st_DBX_flash_tag *DBX_flash_tag);
void __DFFSD_add_FDT_free_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no);
bool_t __DFFSD_acnode_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no);
void __DFFSD_free_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no);
uint32_t __DFFSD_allocate_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag);
void __DFFSD_compress_FDT(struct st_DBX_flash_tag *DBX_flash_tag);
bool_t __DFFSD_write_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t FDT_item_no,struct fdt_info *FDT_item);
void __DFFSD_read_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t FDT_item_no,struct fdt_info *FDT_item);
void __DFFSD_asm_FDT_item(struct fdt_info *FDT_item,union file_attrs attr,
                          char *name);
void __DFFSD_uasm_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                                struct file_rsc *fp,
                                struct fdt_info *FDT_item);
void __DFFSD_uasm_root_folder(struct st_DBX_flash_tag *DBX_flash_tag,
                                struct fdt_info *FDT_item);
void __DFFSD_update_FDT_fstart_dson(struct st_DBX_flash_tag *DBX_flash_tag,
                       uint32_t FDT_item_no,uint32_t new_block_no);
void __DFFSD_update_FDT_file_size(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,sint64_t new_size);
void __DFFSD_update_FDT_item_name(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,char *name);
void __DFFSD_update_FDT_item_attr(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,union file_attrs attr);
//void __DFFSD_update_FDT_item_ctime(struct st_DBX_flash_tag *DBX_flash_tag,
//                                  struct file_rsc *fp,struct time new_time);
//void __DFFSD_update_FDT_item_utime(struct st_DBX_flash_tag *DBX_flash_tag,
//                                  struct file_rsc *fp,struct time new_time);

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

#ifdef __cplusplus
}
#endif

#endif // _FDT_H_

