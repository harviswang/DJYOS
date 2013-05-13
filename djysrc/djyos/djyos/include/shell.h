#ifndef __shell_h__
#define __shell_h__

#if (cfg_shell == 1)

#ifdef __cplusplus
extern "C" {
#endif

#define cn_shell_cmd_limit 255 //sh_service读取串口,限制255字符。故路径限制使用255字符

struct cmd_table
{
    char *cmd;
    bool_t (*shell_cmd_func)(char *param);
};

ptu32_t module_init_shell(ptu32_t para);
void sh_install_sh_ext(bool_t (*sh_user_ext)(char *buf));
bool_t sh_show_for_cycle(char *param);
bool_t sh_list_rsc(char *param);
void sh_list_rsc_all(void);
void __sh_show_branche(struct rsc_node *branche);
bool_t sh_show_memory(char *param);
bool_t sh_fill_memory(char *param);
bool_t sh_keypad_test(void);
bool_t sh_cmd_parser(const char *buf,const char *keyword,char **param);
char *sh_get_word(char *buf,char **next);
char *sh_get_item(char *buf,char **next);
void print_logo_ver(void);
void sh_list_cd_help(void);
void sh_list_dir_help(void);
void sh_list_rd_help(void);
void sh_list_ren_help(void);
void sh_list_md_help(void);
void sh_list_deltree_help(void);
void sh_list_del_help(void);
void sh_list_copy_help(void);
void sh_list_move_help(void);
void sh_list_chkdbx_help(void);
void sh_list_ver_help(void);
void sh_list_chcp_help(void);
void sh_list_format_help(void);
void sh_list_time_help(void);
void sh_list_date_help(void);
void sh_list_cls_help(void);
void shell_cmd_help(void);
bool_t sh_list_cmd_help(char *param);
bool_t sh_ver(char *param);
void capital_convert_lowercase(char *cmd);
bool_t sh_show_event(char *param);
bool_t sh_show_evtt(char *param);
bool_t sh_exec_command(char *cmd);
void sh_service(void);

#ifdef __cplusplus
}
#endif

#endif //__shell_h__

#endif  //#if (cfg_shell == 1)

