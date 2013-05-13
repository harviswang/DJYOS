//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:文件系统
//作者：lst
//版本：V1.0.0
//文件描述:文件系统与存储介质无关的部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _FILE_H_
#define _FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))
typedef struct file_rsc djyfs_file;
//_IOFBF
//_IOLBF
//_IONBF
//BUFSIZ
#define     EOF         (cn_limit_uint32)
#define     SEEK_SET    0       //从0开始计算
#define     SEEK_CUR    1       //可读文件从读指针为准，只写文件以写指针为准
#define     SEEK_END    2       //从文件结束计算

//这么长的单个文件名对嵌入式系统并无多大意义，为兼容大多数长文件名系统。
#define     cn_file_name_limit    255
#define     cn_DBX_name_limit     255      //文件柜（相当于磁盘）名字长度
//我也不想限定文件系统的路径深度，每办法，无限深的路径会导致无限长的执行时间，
//在嵌入式系统中应该是要避免的，20级也应该够用了吧!
#define     cn_path_depth_limit   20

//因设备驱动函数的参数个数有限，定义以下几个结构是用于传递参数
struct rw_para  //传递读写参数
{
    size_t  size;       //读写的每单元尺寸
    size_t nmemb;       //单元数
};
struct seek_para        //传递seek偏移量的参数
{
    sint64_t offset;    //seek的偏移量
    uint8_t  whence;    //seek的参考位置 SEEK_CUR、SEEK_END、SEEK_SET
};
struct DBX_semp_para    //保护文件柜的信号量
{
    struct semaphore_LCB *left;     //左手信号量
    struct semaphore_LCB *right;    //右手信号量
};

enum file_open_mode                     //  seek
{                 //操作 创建 清空 seek     截尾 写位置   读位置  初始 seek填充
    enum_r_rb,      //只读 不创 不清 读位置   无效 无效     seek位置   0   无效
    enum_w_wb,      //只写 创建 清空 写位置   不截 seek位置 无效       0   不填
    enum_a_ab,      //追加 创建 不清 无效     无效 尾部     无效     尾部  无效
    enum_r_rb_plus, //读写 不创 不清 读写位置 不截 seek位置 seek位置   0   不填
    enum_w_wb_plus, //读写 创建 清空 读写位置 不截 seek位置 seek位置   0   不填
    enum_a_ab_plus, //读写 创建 不清 读位置   不截 尾部     seek位置   0   无效
} ;

//文件系统出错代码，本enum常量从cn_fs_error开始依序增1.
enum fs_error_code
{
    enum_fs_no_error = cn_fs_no_error,    //没有错误
    enum_fs_name_invalid_char,            //名字含有非法字符
    enum_fs_name_len_error,               //名字长度错误
    enum_fs_object_opened,                //对象已经打开
    enum_fs_object_nonentity,             //对象不存在
    enum_fs_object_readonly,              //只读对象
    enum_fs_folder_unblank,               //目录非空
    enum_fs_creat_dbx_error,              //建立文件柜错误
    enum_fs_create_root_folder_error,     //建立根目录错误
    enum_fs_format_DBX_error,             //建立根目录错误
    enum_fs_remove_error,                 //删除对象错误
    enum_fs_dbx_error,                 //删除文件柜错误
    enum_fs_rename_error,                 //对象改名错误
    enum_fs_DBX_name_repeat,              //文件柜重名错误
};

#define cn_fs_open_success  0       //成功打开文件(目录)
#define cn_fs_item_exist    1       //文件(目录)存在但不能打开(一般是模式不兼容)
#define cn_fs_item_inexist  2       //文件(目录)不存在


struct file_rsc;
union file_attrs;
//文件柜设备的私有数据结构
//删掉了file_sum和folder_sum成员，因为该成员需要扫描整个FDT表才能获得，扫描的
//时间可能会很长且不确定，不符合嵌入式应用。
struct st_DBX_device_tag
{
    uint32_t    opened_sum;         //打开的文件(目录)总数
    bool_t      formatted;          //文件柜格式化标志
    char        name[cn_DBX_name_limit+1];    //文件柜名，相当于dos的卷标
    struct file_rsc *opened_root;
    ptu32_t DBX_medium_tag;     //特定文件柜的特征数据结构指针，由存储模块使用

    //文件柜功能函数
    //格式化文件柜，在DBX_device_tag参数中返回格式化后文件柜的参数，成功则true，
    //否则false
    bool_t (*format)(u32 fmt_para1,u32 fmt_para2,
                            struct st_DBX_device_tag *DBX_device_tag);

    //文件柜文件读写函数，这些函数由left_read left_write函数使用
    //写文件，把buf中长度为len的数据写入到file的当前指针处
    uint32_t (*write)(struct file_rsc *fp,uint8_t *buf,uint32_t len);
    //读文件，从file当前指针处读取len长度的数据到buf中
    uint32_t (*read)(struct file_rsc *fp,uint8_t *buf,uint32_t len);
    //把写缓冲区的数据写入flash
    uint32_t (*flush)(struct file_rsc *fp);
    //查询有多少可读数据，对于实质文件来说，就是(文件长度-当前指针)，但流的标准
    //定义并非一定如此，比如通信端子。
    sint64_t (*query_file_stocks)(struct file_rsc *fp);
    //查询目标文件还可以写入多少数据，一般等于文件柜的剩余空间，但流的标准
    //定义并非一定如此。
    sint64_t (*query_file_cubage)(struct file_rsc *fp);
    void (*check_DBX)(struct st_DBX_device_tag *DBX_device_tag,
                     sint64_t *sum_size,sint64_t *valid_size,sint64_t *free_size);
    //以下是目录(文件)操作函数,这些函数由left_ctrl函数调用
    //设置文件长度，短于实际长度将截尾，长于实际长度将追加空间。
    sint64_t (*set_file_size)(struct file_rsc *fp,sint64_t new_len);
    //设置文件指针,0=成功，非0=失败
    uint32_t (*seek_file)(struct file_rsc *fp,struct seek_para *pos);
    //建立文件(目录)，attr参数将表明建立目录还是文件。
    bool_t (*create_item)(char *name,struct file_rsc *parent,
                          union file_attrs attr);
    //debug bool_t (*move_file)(struct file_rsc *src_fp,struct file_rsc *dest_fp);                           
    //删除一个文件(目录)，只能删除空目录
    bool_t (*remove_item)(struct file_rsc *fp);
    //打开文件(目录)，result返回item信息，函数返回结果:
    //cn_fs_open_success,成功打开文件(目录)
    //cn_fs_item_exist,文件(目录)存在但不能打开(一般是模式不兼容)
    //cn_fs_item_inexist,文件(目录)不存在
    uint32_t (*open_item)(char *name,struct file_rsc *parent,
                        struct file_rsc *result,enum file_open_mode mode);
    //关闭目录(文件)，true = 成功，false = 失败(因无需分配资源，一般不会失败)
    bool_t (*close_item)(struct file_rsc *fp);
    //查找文件(目录)，result返回item信息，但不分配
    //缓冲区，成功找到item返回true，否则返回false
    bool_t (*lookfor_item)(char *name,struct file_rsc *parent,
                           struct file_rsc *result);
    bool_t (*rename_item)(struct file_rsc *fp,char *newname);
	struct file_rsc *(*item_traversal_son)(struct file_rsc * ancestor_file, 
                                    struct file_rsc * current_file);
    //目录表操作函数
    //查询目录表尺寸
    uint32_t (*check_fdt_size)(struct st_DBX_device_tag *DBX_device_tag);
    //读目录表,本函数是为一些专用工具准备的，一般不要用
    void (*read_fdt)(struct st_DBX_device_tag *DBX_device_tag,uint8_t *buf);
    //检查文件夹下子目录和文件的数量，不包含子目录下的文件。
    uint32_t (*check_folder)(struct file_rsc *folder);
};
enum fs_left_cmd
{
    enum_fs_work_path,    //设置并打开当前工作路径
};
enum fs_right_cmd
{
    enum_fs_add_DBX,  //增加一个文件柜
    enum_fs_del_DBX,  //删除一个文件柜
};
enum DBX_left_cmd
{
    enum_DBX_format,        //格式化文件柜
    enum_chk_DBX_format,    //查询文件柜是否格式化
    enum_DBX_lookfor,       //查找一个项目（文件或目录）是否存在，只返回一个布尔量
    enum_DBX_open,          //打开一个文件，只能是文件
    enum_DBX_remove,        //删除一个文件或空目录，不提供删除一棵树的功能
    enum_DBX_rename,        //文件（目录）改名
    enum_DBX_seek,          //设置一个打开的文件的读写位置
    enum_DBX_close,         //关闭一个文件
    enum_DBX_create,        //创建一个文件,ansi c里并没有对应的creat函数。
    enum_DBX_flush,         //把缓冲区刷到物理设备上去
};
enum DBX_right_cmd
{
    enum_DBX_clr_readbuf,     //清空某文件读缓冲区
    enum_DBX_clr_writebuf,    //清空某文件写缓冲区
    enum_DBX_flush_one,       //把某文件缓冲区写入存储设备
    enum_DBX_flush_all,       //把该文件柜下所有打开文件缓冲区写入存储设备
    enum_DBX_search_medium_tag,  //修改文件的媒体标记
    enum_DBX_update_medium_tag,  //修改文件的媒体标记
};

struct file_attr_bits
{
    uint8_t read_only:1;    //只读文件
    uint8_t hidden:1;       //隐藏文件
    uint8_t reserve:2;
    uint8_t folder:1;       //目录
    uint8_t archive:1;      //档案文件
    uint8_t reserve1:1;
    uint8_t deleted:1;      //文件被删除
};

//r open text file for reading
//    打开文本文件用于读
//w truncate to zero length or create text file for writing
//    重置或建立一个文本文件用于写
//a append; open or create text file for writing at end-of-file
//    打开一个文本文件用于追加
//rb open binary file for reading
//    打开一个二进制文件用于读
//wb truncate to zero length or create binary file for writing
//    重置或建立一个二进制文件用于写
//ab append; open or create binary file for writing at end-of-file
//    打开或建立一个二进制文件用于追加
//r+ open text file for update (reading and writing)
//    打开一个文本文件用于读写
//w+ truncate to zero length or create text file for update
//    重置或建立一个文本文件用于读写
//a+ append; open or create text file for update, writing at end-of-file
//    打开或建立一个文本文件用于读和追加写
//r+b or rb+ open binary file for update (reading and writing)
//    打开一个二进制文件用于读写
//w+b or wb+ truncate to zero length or create binary file for update
//    重置或建立一个二进制文件用于读写
//a+b or ab+ append; open or create binary file for update, writing at end-of-file
//    打开或建立一个二进制文件用于读和追加写
//      与unix一样，djyos的文件系统只支持二进制模式，理由：文件系统只是用来帮助
//      访问文件的助手，不应该插手文件格式，只支持二进制，既简单又可以获得更好
//      的模块化。因此，djyos中mode的含义如下：
//      r，rb   只读方式打开一个文件
//      w，wb   只写方式创建一个文件，若原文件存在则覆盖之
//      a，ab   追加方式打开一个文件，若原文件不存在则创建之
//      r+，r+b，rb+ 可读可写方式打开一个文件
//      w+，w+b，wb+ 可读可写方式创建一个文件，若原文件存在则覆盖之
//      a+，a+b，ab+ 可读写追加方式打开一个文件，若原文件不存在则创建之

union file_attrs
{
    uint8_t all;
    struct file_attr_bits bits;
};

struct file_rsc
{
    struct rsc_node file_node;
    struct semaphore_LCB *file_semp;//保留备用
    union file_attrs attr;          //文件属性
    enum file_open_mode open_mode;  //文件打开模式
    uint8_t  second_create;         //文件建立时间:second
    uint8_t  minute_create;         //文件建立时间:minute
    uint8_t  hour_create;           //文件建立时间:hour
    uint8_t  date_create;           //文件建立日期
    uint8_t  month_create;          //文件建立月份
    uint8_t  year_low_create;       //文件建立年度
    uint8_t  year_high_create;      //文件建立年度
    uint8_t  second_modify;         //文件修改时间:second
    uint8_t  minute_modify;         //文件修改时间:minute
    uint8_t  hour_modify;           //文件修改时间:hour
    uint8_t  date_modify;           //文件修改日期
    uint8_t  month_modify;          //文件修改月份
    uint8_t  year_low_modify;       //文件修改年度
    uint8_t  year_high_modify;      //文件修改年度
    struct pan_device *home_DBX;    //本文件所属的文件柜
    ptu32_t file_medium_tag;        //与媒体相关的标记，其含义由特定文件系统
                                    //driver解释，如flash file driver)中，保存
                                    //该文件(目录)的FDT项目号。
    struct ring_buf read_rw_buf;    //独立读写文件的读缓冲区，或合并读写文件的
                                    //读写缓冲区
    struct ring_buf write_buf;      //文件写缓冲区
    struct ring_buf *p_read_buf;    //读缓冲区指针
    struct ring_buf *p_write_buf;   //写缓冲区指针
    uint32_t open_counter;          //文件打开次数计数
    sint64_t file_size;             //文件尺寸,字节数，不含仍在缓冲区的数据
    sint64_t read_ptr;              //当前读指针
    sint64_t write_ptr;             //当前写指针----保留，将来也许有用
    uint32_t eno;           //最近一次出错类型，enum_fs_no_error表示从来没有出错
    char     name[cn_file_name_limit+1]; //文件名（目录名）
};

ptu32_t module_init_djyfs(ptu32_t para);
uint32_t djyfs_user_set_work_path(void);
bool_t get_work_path_name(char *user_work_path,u32 namebuf_len);
bool_t get_DBX_name(char *DBX_name,u32 namebuf_len);
bool_t print_work_path(void);
bool_t __djyfs_DBX_lookfor_item(struct pan_device *DBX_lhdl,char *synname);
djyfs_file *__djyfs_DBX_open_file(struct pan_device * DBX_lhdl,
                                char *synname,char *mode);
ptu32_t djyfs_fs_app_ctrl(struct pan_device * fs_lhdl,uint32_t left_cmd,
                                ptu32_t data1,ptu32_t data2);
ptu32_t djyfs_fs_driver_ctrl(struct pan_device * fs_rhdl,uint32_t right_cmd,
                                ptu32_t data1,ptu32_t data2);
ptu32_t djyfs_DBX_app_write(struct pan_device * DBX_lhdl,ptu32_t buf,
                        ptu32_t write_para,ptu32_t file);
ptu32_t djyfs_DBX_app_read(struct pan_device * DBX_lhdl,ptu32_t buf,
                        ptu32_t param,ptu32_t file);
uint16_t __djyfs_if_contain_DBX_name(char *fullname);
bool_t __djyfs_if_abs_path(char *fullname);
bool_t __djyfs_if_contain_filename(char *fullname);
bool_t __djyfs_item_if_opened(struct st_DBX_device_tag *DBX_device_tag,char *synname);
uint32_t __djyfs_DBX_remove_item(struct pan_device * DBX_lhdl,char *synname);
uint32_t __DBX_rename_item(struct pan_device * DBX_lhdl,
                            char *old_synname,char *newname);
bool_t __DBX_close_item(struct pan_device * DBX_lhdl,struct file_rsc *fp);
uint32_t __DBX_flush_file(struct pan_device * DBX_lhdl,struct file_rsc *fp);
uint32_t __DBX_flush_all(struct pan_device * DBX_lhdl);
ptu32_t DBX_left_ctrl(struct pan_device * DBX_lhdl,uint32_t left_cmd,
                                ptu32_t data1,ptu32_t data2);
ptu32_t DBX_right_ctrl(struct pan_device * DBX_rhdl,uint32_t right_cmd,
                                ptu32_t data1,ptu32_t data2);
djy_handle_t __open_DBX_left(char *fullname);
bool_t __pick_word(char *synname,uint32_t curent_offset,char *name);
bool_t __pick_path_word(char *synname,uint32_t curent_offset,char *name);
bool_t __pick_filename_word(char *synname,char *name);
char *__pick_synname(char *fullname);
bool_t __check_fullname(char *fullname);
bool_t __check_word(char *word);
djyfs_file *djyfs_folder_traversal_son(struct file_rsc *ancestor_file,
                                        	struct file_rsc *current_file);
void djyfs_find_close(struct file_rsc *current_file);
djyfs_file *djyfs_fopen(char *fullname, char *mode);
bool_t djyfs_fsearch(char *fullname);
uint32_t djyfs_remove(char *fullname);
uint32_t djyfs_rename(char *old_fullname,char *new_filename);
uint32_t djyfs_fclose(djyfs_file *fp);
size_t djyfs_fread(void *buf,size_t size, size_t nmemb,djyfs_file *fp);
size_t djyfs_fwrite(void *buf,size_t size, size_t nmemb,djyfs_file *fp);
uint32_t djyfs_fflush(djyfs_file *fp);
uint32_t djyfs_fseek(djyfs_file *fp, sint64_t offset, int whence);
bool_t djyfs_format(uint32_t format_para1,uint32_t format_para2,char *dbx_name);
//bool_t djyfs_move(char *src,char *dest);

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))

#ifdef __cplusplus
}
#endif

#endif // _FILE_H_

