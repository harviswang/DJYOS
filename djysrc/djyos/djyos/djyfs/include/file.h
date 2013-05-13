//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:�ļ�ϵͳ��洢�����޹صĲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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
#define     SEEK_SET    0       //��0��ʼ����
#define     SEEK_CUR    1       //�ɶ��ļ��Ӷ�ָ��Ϊ׼��ֻд�ļ���дָ��Ϊ׼
#define     SEEK_END    2       //���ļ���������

//��ô���ĵ����ļ�����Ƕ��ʽϵͳ���޶�����壬Ϊ���ݴ�������ļ���ϵͳ��
#define     cn_file_name_limit    255
#define     cn_DBX_name_limit     255      //�ļ����൱�ڴ��̣����ֳ���
//��Ҳ�����޶��ļ�ϵͳ��·����ȣ�ÿ�취���������·���ᵼ�����޳���ִ��ʱ�䣬
//��Ƕ��ʽϵͳ��Ӧ����Ҫ����ģ�20��ҲӦ�ù����˰�!
#define     cn_path_depth_limit   20

//���豸���������Ĳ����������ޣ��������¼����ṹ�����ڴ��ݲ���
struct rw_para  //���ݶ�д����
{
    size_t  size;       //��д��ÿ��Ԫ�ߴ�
    size_t nmemb;       //��Ԫ��
};
struct seek_para        //����seekƫ�����Ĳ���
{
    sint64_t offset;    //seek��ƫ����
    uint8_t  whence;    //seek�Ĳο�λ�� SEEK_CUR��SEEK_END��SEEK_SET
};
struct DBX_semp_para    //�����ļ�����ź���
{
    struct semaphore_LCB *left;     //�����ź���
    struct semaphore_LCB *right;    //�����ź���
};

enum file_open_mode                     //  seek
{                 //���� ���� ��� seek     ��β дλ��   ��λ��  ��ʼ seek���
    enum_r_rb,      //ֻ�� ���� ���� ��λ��   ��Ч ��Ч     seekλ��   0   ��Ч
    enum_w_wb,      //ֻд ���� ��� дλ��   ���� seekλ�� ��Ч       0   ����
    enum_a_ab,      //׷�� ���� ���� ��Ч     ��Ч β��     ��Ч     β��  ��Ч
    enum_r_rb_plus, //��д ���� ���� ��дλ�� ���� seekλ�� seekλ��   0   ����
    enum_w_wb_plus, //��д ���� ��� ��дλ�� ���� seekλ�� seekλ��   0   ����
    enum_a_ab_plus, //��д ���� ���� ��λ��   ���� β��     seekλ��   0   ��Ч
} ;

//�ļ�ϵͳ������룬��enum������cn_fs_error��ʼ������1.
enum fs_error_code
{
    enum_fs_no_error = cn_fs_no_error,    //û�д���
    enum_fs_name_invalid_char,            //���ֺ��зǷ��ַ�
    enum_fs_name_len_error,               //���ֳ��ȴ���
    enum_fs_object_opened,                //�����Ѿ���
    enum_fs_object_nonentity,             //���󲻴���
    enum_fs_object_readonly,              //ֻ������
    enum_fs_folder_unblank,               //Ŀ¼�ǿ�
    enum_fs_creat_dbx_error,              //�����ļ������
    enum_fs_create_root_folder_error,     //������Ŀ¼����
    enum_fs_format_DBX_error,             //������Ŀ¼����
    enum_fs_remove_error,                 //ɾ���������
    enum_fs_dbx_error,                 //ɾ���ļ������
    enum_fs_rename_error,                 //�����������
    enum_fs_DBX_name_repeat,              //�ļ�����������
};

#define cn_fs_open_success  0       //�ɹ����ļ�(Ŀ¼)
#define cn_fs_item_exist    1       //�ļ�(Ŀ¼)���ڵ����ܴ�(һ����ģʽ������)
#define cn_fs_item_inexist  2       //�ļ�(Ŀ¼)������


struct file_rsc;
union file_attrs;
//�ļ����豸��˽�����ݽṹ
//ɾ����file_sum��folder_sum��Ա����Ϊ�ó�Ա��Ҫɨ������FDT����ܻ�ã�ɨ���
//ʱ����ܻ�ܳ��Ҳ�ȷ����������Ƕ��ʽӦ�á�
struct st_DBX_device_tag
{
    uint32_t    opened_sum;         //�򿪵��ļ�(Ŀ¼)����
    bool_t      formatted;          //�ļ����ʽ����־
    char        name[cn_DBX_name_limit+1];    //�ļ��������൱��dos�ľ��
    struct file_rsc *opened_root;
    ptu32_t DBX_medium_tag;     //�ض��ļ�����������ݽṹָ�룬�ɴ洢ģ��ʹ��

    //�ļ����ܺ���
    //��ʽ���ļ�����DBX_device_tag�����з��ظ�ʽ�����ļ���Ĳ������ɹ���true��
    //����false
    bool_t (*format)(u32 fmt_para1,u32 fmt_para2,
                            struct st_DBX_device_tag *DBX_device_tag);

    //�ļ����ļ���д��������Щ������left_read left_write����ʹ��
    //д�ļ�����buf�г���Ϊlen������д�뵽file�ĵ�ǰָ�봦
    uint32_t (*write)(struct file_rsc *fp,uint8_t *buf,uint32_t len);
    //���ļ�����file��ǰָ�봦��ȡlen���ȵ����ݵ�buf��
    uint32_t (*read)(struct file_rsc *fp,uint8_t *buf,uint32_t len);
    //��д������������д��flash
    uint32_t (*flush)(struct file_rsc *fp);
    //��ѯ�ж��ٿɶ����ݣ�����ʵ���ļ���˵������(�ļ�����-��ǰָ��)�������ı�׼
    //���岢��һ����ˣ�����ͨ�Ŷ��ӡ�
    sint64_t (*query_file_stocks)(struct file_rsc *fp);
    //��ѯĿ���ļ�������д��������ݣ�һ������ļ����ʣ��ռ䣬�����ı�׼
    //���岢��һ����ˡ�
    sint64_t (*query_file_cubage)(struct file_rsc *fp);
    void (*check_DBX)(struct st_DBX_device_tag *DBX_device_tag,
                     sint64_t *sum_size,sint64_t *valid_size,sint64_t *free_size);
    //������Ŀ¼(�ļ�)��������,��Щ������left_ctrl��������
    //�����ļ����ȣ�����ʵ�ʳ��Ƚ���β������ʵ�ʳ��Ƚ�׷�ӿռ䡣
    sint64_t (*set_file_size)(struct file_rsc *fp,sint64_t new_len);
    //�����ļ�ָ��,0=�ɹ�����0=ʧ��
    uint32_t (*seek_file)(struct file_rsc *fp,struct seek_para *pos);
    //�����ļ�(Ŀ¼)��attr��������������Ŀ¼�����ļ���
    bool_t (*create_item)(char *name,struct file_rsc *parent,
                          union file_attrs attr);
    //debug bool_t (*move_file)(struct file_rsc *src_fp,struct file_rsc *dest_fp);                           
    //ɾ��һ���ļ�(Ŀ¼)��ֻ��ɾ����Ŀ¼
    bool_t (*remove_item)(struct file_rsc *fp);
    //���ļ�(Ŀ¼)��result����item��Ϣ���������ؽ��:
    //cn_fs_open_success,�ɹ����ļ�(Ŀ¼)
    //cn_fs_item_exist,�ļ�(Ŀ¼)���ڵ����ܴ�(һ����ģʽ������)
    //cn_fs_item_inexist,�ļ�(Ŀ¼)������
    uint32_t (*open_item)(char *name,struct file_rsc *parent,
                        struct file_rsc *result,enum file_open_mode mode);
    //�ر�Ŀ¼(�ļ�)��true = �ɹ���false = ʧ��(�����������Դ��һ�㲻��ʧ��)
    bool_t (*close_item)(struct file_rsc *fp);
    //�����ļ�(Ŀ¼)��result����item��Ϣ����������
    //���������ɹ��ҵ�item����true�����򷵻�false
    bool_t (*lookfor_item)(char *name,struct file_rsc *parent,
                           struct file_rsc *result);
    bool_t (*rename_item)(struct file_rsc *fp,char *newname);
	struct file_rsc *(*item_traversal_son)(struct file_rsc * ancestor_file, 
                                    struct file_rsc * current_file);
    //Ŀ¼���������
    //��ѯĿ¼��ߴ�
    uint32_t (*check_fdt_size)(struct st_DBX_device_tag *DBX_device_tag);
    //��Ŀ¼��,��������ΪһЩר�ù���׼���ģ�һ�㲻Ҫ��
    void (*read_fdt)(struct st_DBX_device_tag *DBX_device_tag,uint8_t *buf);
    //����ļ�������Ŀ¼���ļ�����������������Ŀ¼�µ��ļ���
    uint32_t (*check_folder)(struct file_rsc *folder);
};
enum fs_left_cmd
{
    enum_fs_work_path,    //���ò��򿪵�ǰ����·��
};
enum fs_right_cmd
{
    enum_fs_add_DBX,  //����һ���ļ���
    enum_fs_del_DBX,  //ɾ��һ���ļ���
};
enum DBX_left_cmd
{
    enum_DBX_format,        //��ʽ���ļ���
    enum_chk_DBX_format,    //��ѯ�ļ����Ƿ��ʽ��
    enum_DBX_lookfor,       //����һ����Ŀ���ļ���Ŀ¼���Ƿ���ڣ�ֻ����һ��������
    enum_DBX_open,          //��һ���ļ���ֻ�����ļ�
    enum_DBX_remove,        //ɾ��һ���ļ����Ŀ¼�����ṩɾ��һ�����Ĺ���
    enum_DBX_rename,        //�ļ���Ŀ¼������
    enum_DBX_seek,          //����һ���򿪵��ļ��Ķ�дλ��
    enum_DBX_close,         //�ر�һ���ļ�
    enum_DBX_create,        //����һ���ļ�,ansi c�ﲢû�ж�Ӧ��creat������
    enum_DBX_flush,         //�ѻ�����ˢ�������豸��ȥ
};
enum DBX_right_cmd
{
    enum_DBX_clr_readbuf,     //���ĳ�ļ���������
    enum_DBX_clr_writebuf,    //���ĳ�ļ�д������
    enum_DBX_flush_one,       //��ĳ�ļ�������д��洢�豸
    enum_DBX_flush_all,       //�Ѹ��ļ��������д��ļ�������д��洢�豸
    enum_DBX_search_medium_tag,  //�޸��ļ���ý����
    enum_DBX_update_medium_tag,  //�޸��ļ���ý����
};

struct file_attr_bits
{
    uint8_t read_only:1;    //ֻ���ļ�
    uint8_t hidden:1;       //�����ļ�
    uint8_t reserve:2;
    uint8_t folder:1;       //Ŀ¼
    uint8_t archive:1;      //�����ļ�
    uint8_t reserve1:1;
    uint8_t deleted:1;      //�ļ���ɾ��
};

//r open text file for reading
//    ���ı��ļ����ڶ�
//w truncate to zero length or create text file for writing
//    ���û���һ���ı��ļ�����д
//a append; open or create text file for writing at end-of-file
//    ��һ���ı��ļ�����׷��
//rb open binary file for reading
//    ��һ���������ļ����ڶ�
//wb truncate to zero length or create binary file for writing
//    ���û���һ���������ļ�����д
//ab append; open or create binary file for writing at end-of-file
//    �򿪻���һ���������ļ�����׷��
//r+ open text file for update (reading and writing)
//    ��һ���ı��ļ����ڶ�д
//w+ truncate to zero length or create text file for update
//    ���û���һ���ı��ļ����ڶ�д
//a+ append; open or create text file for update, writing at end-of-file
//    �򿪻���һ���ı��ļ����ڶ���׷��д
//r+b or rb+ open binary file for update (reading and writing)
//    ��һ���������ļ����ڶ�д
//w+b or wb+ truncate to zero length or create binary file for update
//    ���û���һ���������ļ����ڶ�д
//a+b or ab+ append; open or create binary file for update, writing at end-of-file
//    �򿪻���һ���������ļ����ڶ���׷��д
//      ��unixһ����djyos���ļ�ϵͳֻ֧�ֶ�����ģʽ�����ɣ��ļ�ϵͳֻ����������
//      �����ļ������֣���Ӧ�ò����ļ���ʽ��ֻ֧�ֶ����ƣ��ȼ��ֿ��Ի�ø���
//      ��ģ�黯����ˣ�djyos��mode�ĺ������£�
//      r��rb   ֻ����ʽ��һ���ļ�
//      w��wb   ֻд��ʽ����һ���ļ�����ԭ�ļ������򸲸�֮
//      a��ab   ׷�ӷ�ʽ��һ���ļ�����ԭ�ļ��������򴴽�֮
//      r+��r+b��rb+ �ɶ���д��ʽ��һ���ļ�
//      w+��w+b��wb+ �ɶ���д��ʽ����һ���ļ�����ԭ�ļ������򸲸�֮
//      a+��a+b��ab+ �ɶ�д׷�ӷ�ʽ��һ���ļ�����ԭ�ļ��������򴴽�֮

union file_attrs
{
    uint8_t all;
    struct file_attr_bits bits;
};

struct file_rsc
{
    struct rsc_node file_node;
    struct semaphore_LCB *file_semp;//��������
    union file_attrs attr;          //�ļ�����
    enum file_open_mode open_mode;  //�ļ���ģʽ
    uint8_t  second_create;         //�ļ�����ʱ��:second
    uint8_t  minute_create;         //�ļ�����ʱ��:minute
    uint8_t  hour_create;           //�ļ�����ʱ��:hour
    uint8_t  date_create;           //�ļ���������
    uint8_t  month_create;          //�ļ������·�
    uint8_t  year_low_create;       //�ļ��������
    uint8_t  year_high_create;      //�ļ��������
    uint8_t  second_modify;         //�ļ��޸�ʱ��:second
    uint8_t  minute_modify;         //�ļ��޸�ʱ��:minute
    uint8_t  hour_modify;           //�ļ��޸�ʱ��:hour
    uint8_t  date_modify;           //�ļ��޸�����
    uint8_t  month_modify;          //�ļ��޸��·�
    uint8_t  year_low_modify;       //�ļ��޸����
    uint8_t  year_high_modify;      //�ļ��޸����
    struct pan_device *home_DBX;    //���ļ��������ļ���
    ptu32_t file_medium_tag;        //��ý����صı�ǣ��京�����ض��ļ�ϵͳ
                                    //driver���ͣ���flash file driver)�У�����
                                    //���ļ�(Ŀ¼)��FDT��Ŀ�š�
    struct ring_buf read_rw_buf;    //������д�ļ��Ķ�����������ϲ���д�ļ���
                                    //��д������
    struct ring_buf write_buf;      //�ļ�д������
    struct ring_buf *p_read_buf;    //��������ָ��
    struct ring_buf *p_write_buf;   //д������ָ��
    uint32_t open_counter;          //�ļ��򿪴�������
    sint64_t file_size;             //�ļ��ߴ�,�ֽ������������ڻ�����������
    sint64_t read_ptr;              //��ǰ��ָ��
    sint64_t write_ptr;             //��ǰдָ��----����������Ҳ������
    uint32_t eno;           //���һ�γ������ͣ�enum_fs_no_error��ʾ����û�г���
    char     name[cn_file_name_limit+1]; //�ļ�����Ŀ¼����
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

