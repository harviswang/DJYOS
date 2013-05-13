#ifndef __shell_fs_h__
#define __shell_fs_h__

#ifdef __cplusplus
extern "C" {
#endif

bool_t __check_wildcard(char *str);
bool_t __wildcard_match( char* src, char* wildcard);
bool_t __judge_backslash_if_data(char *path);
bool_t __judge_colon_if_data(char *path);
bool_t __cut_path_end_item(char *path);
bool_t __pick_path_end_item(char *path,char *name);
bool_t __judge_if_absolute_path(char *path);
char *get_folder_wildcard_str(char *wildcard_param,char *get_merge_str);
bool_t del_wildcard_files(char *wildcard_param);
bool_t deltree_wildcard_files(char *wildcard_param);
bool_t str_end_increase_backslash(char *str);
bool_t str_end_wipe_backslash(char *str);
bool_t dir_item_son(struct file_rsc *parent_fp);
bool_t dir_wildcard(char *wildcard_param);
bool_t judge_if_folder(char *fullname);
bool_t del_items_of_folder_or_loop(char *work_path);
bool_t del_files_of_folder(char *work_path);
bool_t fs_mkdir(char *md_param);
bool_t fs_remove_tree(char *tree_param);
bool_t fs_remove_folder(char *rd_param);
bool_t cd_parent(char *next_param);
bool_t cd_dbx(char *dbx);
bool_t sh_chkdbx(char *DBX_name);
bool_t sh_copy(char *param);
bool_t sh_move(char *param);
bool_t sh_ren(char *param);
bool_t sh_md(char *param);
bool_t sh_deltree(char *param);
bool_t sh_rd(char *param);
bool_t sh_del(char *param);
bool_t sh_cd(char *chdir_param);
bool_t sh_dir(char *param);
bool_t fs_search(char *fullname);
bool_t djyfs_write_read_test(char *param);
bool_t read_test(char *param);
bool_t write_a(char *param);
bool_t fs_fopen(char *fullname);
bool_t write(char *param);
void fopen_time_test(void);
bool_t sh_create_dbx(char *param);
bool_t sh_format(char *param);



#ifdef __cplusplus
}
#endif

#endif //__shell_fs_h__
