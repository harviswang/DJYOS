//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:flash�ļ�ϵͳ��ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _RAMFILE_H_
#define _RAMFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "os_inc.h"



#define cn_MDR_DDR_main_ramfs     0xfe
#define cn_MDR_DDR_bak_ramfs      0xfd
//#define cn_MDR_PCRB         0xfc
#define cn_DBX_sum_max_ramfs      3

#define cn_MDR_CDR_size_ramfs     512     //MDR����CDR�ĳߴ磬����8�ı���
#define cn_MDR_DBX_size_ramfs     56      //MDR����ÿ��DBX��ĳߴ磬����8�ı���
//MDR��CDR��DBX����ܳߴ�
#define cn_CDR_and_DBX_size_ramfs (cn_MDR_CDR_size_ramfs + cn_DBX_sum_max_ramfs*cn_MDR_DBX_size_ramfs)
//������¼��ƫ�Ƶ�ַ
#define cn_roll_offset_ramfs       (((cn_CDR_and_DBX_size_ramfs + 7) /8)*8)
//MDR��ĳߴ磬������С��856��������djyramfs_install_chip�ᱨ���������ע��
#define cn_MDR_size         4096
//�ڴ��ļ�ϵͳռ���ڴ������С��ߴ��׺0�ĸ���
#define cn_block_size_suffix_zero 12


struct st_MDR_flag_ramfs
{
    bool_t DBX_created[cn_DBX_sum_max_ramfs];   //true=�ļ����Ѿ�������false=��δ����
    uint32_t MDR_main_block;              //�洢MDR_main����ʼ���
    uint32_t MDR_bak_block;               //�洢MDR_bak����ʼ���
    uint32_t roll_next_main;              //�����洢����һ���洢λ��
    uint32_t roll_next_bak;               //�����洢����һ���洢λ��
};
 struct st_ram_chip
{
    struct rsc_node chip_node;
    u32 block_sum;         //�ܿ���
    u32 block_size;        //��ߴ�
    u32 valid_sum;         //���ÿ���
    u32 rev_blocks;        //����������
    u32 rev_start;         //�������׿��ţ�������ǰ��оƬ�����顣
    u32 DBX_start;         //�����ļ�����׿���
    u32 DBX_read_buf_size; //���ļ��������ߴ�
    u32 DBX_write_buf_size;//д�ļ��������ߴ�
    struct st_MDR_flag_ramfs MDR;     //MDR����ز���
    uint8_t  *block_buf;        //�黺����
    struct semaphore_LCB *left_semaphore;
    struct semaphore_LCB *right_semaphore;

    //��һ��֮�ڵ����ⳤ�����ݣ�����У�������ȷ����
    //���أ�cn_limit_uint32=������������ֵ��ʵ��д������
    u32 (*read_block)(uint32_t block,uint32_t offset,
                          uint8_t *buf,uint32_t size);
    //д��һ��֮�ڵ����ⳤ�����ݣ�У�鷽��ȡ���ھ���������������Ҳ������У�顣
    //���أ�cn_limit_uint32=������������ֵ��ʵ��д������
    u32 (*write_block)(uint32_t block,uint32_t offset,
                           uint8_t *buf,uint32_t size);
};
//MAT��ǰָ��ͺ�ָ��������λ���������ʾ�ÿ�����ԣ����16�����ԣ�Ŀǰ�õ�7��
//��ǰָ��ĸ���λΪ�ͣ���ָ��Ϊ�ߡ����ԣ�MAT��ʵ������30λ�ġ�
#define cn_file_block_start_ramfs     0   //�ļ�MAT�����׿�(�п���ͬʱҲ��ĩ��)
#define cn_file_block_continue_ramfs  1   //�ļ�MAT���ĺ�����
#define cn_FDT_block_ramfs            2   //�����Ŀ¼��Ŀ�
#define cn_invalid_block_ramfs        3   //����
#define cn_free_block_ramfs           4   //���п�
#define cn_DDR_main_block_ramfs       5   //���ڱ���DDR_main�Ŀ�
#define cn_DDR_bak_block_ramfs        6   //���ڱ���DDR_bak�Ŀ�
//#define cn_PCRB_block           7   //���ڱ������ָ����ݵĿ�

struct MAT_table_ramfs
{
    uint32_t previous;      //��ǰָ��(��ţ���C���Ե�ָ�벻ͬ)����30λ��Ч
    uint32_t next;          //���ָ�룬��30λ��Ч��
    uint8_t  attr;          //�����ԣ���cn_file_block�峣������
};
//���ṹ��st_DBX_device_tag�ṹ�е�medium_tag��Աָ��оƬdriver���ɼ�
struct st_DBX_ram_tag
{
    struct st_ram_chip *chip;    //�ļ�������оƬ
    bool_t   nand_ecc;          //�Ƿ���eccУ�飬һ����˵��nand����flash����ý��
                                //�洢��ʱ���ɲ���eccУ�顣
    uint32_t DBX_no;            //������оƬ�е��ļ�����
    uint32_t start_block;       //��ʼ��ţ������оƬ�ľ��Կ��
    uint32_t block_sum;         //�ܿ���
    uint32_t valid_sum;         //��Ч����
    uint32_t free_sum;          //���п���
  //  uint32_t PCRB_no;           //����ָ���Ŀ�ţ��ļ����ڵ���Կ��
    uint32_t DDR_main;          //�׸�DDR���ţ��ļ����ڵ���Կ��
    uint32_t DDR_bak;           //�ڶ���DDR���ţ��ļ����ڵ���Կ��
    uint32_t ero;               //�ļ���״̬
    uint32_t FDT_capacity;      //�Ѿ������FDT���FDT_item����wjz:�ļ����ܹ��ı�����
    uint32_t FDT_free_num;      //�Ѿ�ʹ�õ�FDT������ wjz: ������ʹ�õ�FDT����
    uint32_t DDR_size;          //DDR��ߴ�
    uint32_t *DDR_DBL;          //DBL����main��bak
  //  uint32_t DDR_ARB;         //ĥ�������������ĥ�������(16bit)��ʵ��ĥ�����
    uint32_t DDR_FDSB;          //Ŀ¼���׿���
    struct MAT_table_ramfs *DDR_MAT;  //MAT��
    //��DDR��д���־���飬true=��д��flash��false=δд�������С��DDR��ռ�õ�
    //������ͬ��������һ����
    bool_t  *writed_DDR_main;
    bool_t  *writed_DDR_bak;
    //�����������ڷ����ڴ�ʱ�밴������Ҫ��֤�߽����(ʹ��djyos��̬�ڴ����
    //���Ա�֤)
   // uint32_t *ART_position;       //ĥ��λ�ñ��ÿ���ĥ��˳����е�λ�ã�
   // uint32_t *ART_block_no; //ĥ���ű��������ͬ�У������������ж�Ӧ��Ŀ��
   // uint16_t *ART_times;    //ĥ�������æ����п�ֱ�ĥ���������
   // uint32_t balance_region[19];//ĥ�������æ���п��9������������ǻ������
};

struct file_access_tag_ramfs
{
    sint64_t true_size;         //�ļ�ʵ�ʳ��ȣ���ֻ��ӳ�Ѿ�д��flash�����ݣ�
                                //��fp�е�file_size�����д�������е����ݣ�
    uint32_t last_block_no;     //���һ��Ŀ��
    uint32_t write_block_no;    //дָ�����ڿ��
    uint32_t read_block_no;     //��ָ�����ڿ��
};

//�����ļ�����,��fat��ʽ���е����Է���fat��ͬ��λ��,����fdt���е�attr��Ա��
//�漰����������Ϊ���죬����ʹ��ö��������(enum)��
#define cn_FS_ATTR_READ_ONLY_ramfs   (1<<0)
#define cn_FS_ATTR_HIDDEN_ramfs      (1<<1)
#define cn_FS_ATTR_USER_ramfs        (1<<2)   //�û�����
#define cn_FS_ATTR_RESERVE_ramfs     (1<<3)   //����
#define cn_FS_ATTR_DIRECTORY_ramfs   (1<<4)   //Ŀ¼
#define cn_FS_ATTR_ARCHIVE_ramfs     (1<<5)    //�ļ�
#define cn_FS_ATTR_DELETED_ramfs     (1<<7)   //����(ɾ��)��־
//�ر����һ��cn_FS_ATTR_USER����fat�У����λ��λ��������ʾ�Ƿ�ϵͳ�ļ���Ȼ����
//ĳ�ļ��Ƿ���Ϊϵͳ�ļ������ļ�ϵͳʹ���ߵ����⣬�ļ�ϵͳdriver��Ӧ�ù��ʵģ�
//���ԣ�������ط���djyramfsģ�����һ�����û����͵�λ��
//ͬ����cn_FS_ATTR_RESERVEλ������fat�ľ��λ���಻�á�

#define cn_ram_file_name_limit    215     //δ����fat��֤�����256�ֽڣ�fat�����׼���
#define cn_ram_DBX_name_limit     31      //flash�ļ������ֳ���

//���ṹ��flashd river��˽�����ݽṹ��оƬdriver(����nand.c)���ɼ�
struct ramfs_fdt
{
    uint8_t  mattr;              //�̶���ʽ�����ڴ洢�����е��ļ�����,
    uint8_t  name_buf_len;       //�ļ���buf���ȣ�Ӧ=cn_ram_file_name_limit+1
                                 //�ṩ��Ӧ�ó�����Ŀ¼����
    uint8_t  second_create;      //�ļ�����ʱ��:second,2λ���BCD��
    uint8_t  minute_create;      //�ļ�����ʱ��:minute,2λ���BCD��
    uint8_t  hour_create;        //�ļ�����ʱ��:hour,2λ���BCD��
    uint8_t  date_create;        //�ļ���������,2λ���BCD��
    uint8_t  month_create;       //�ļ������·�,2λ���BCD��
    uint8_t  year_low_create;    //�ļ��������,4λ���BCD��
    uint8_t  year_high_create;   //�ļ��������,4λ���BCD��
    uint8_t  second_modify;      //�ļ��޸�ʱ��:second,2λ���BCD��
    uint8_t  minute_modify;      //�ļ��޸�ʱ��:minute,2λ���BCD��
    uint8_t  hour_modify;        //�ļ��޸�ʱ��:hour,2λ���BCD��
    uint8_t  date_modify;        //�ļ��޸�����,2λ���BCD��
    uint8_t  month_modify;       //�ļ��޸��·�,2λ���BCD��
    uint8_t  year_low_modify;    //�ļ��޸����,4λ���BCD��
    uint8_t  year_high_modify;   //�ļ��޸����,4λ���BCD��
    uint8_t  file_size[8];       //�ļ��ߴ磬��Ŀ¼��file_size�����;�Ƚ����⣬
                                 //ǰ4�ֽ��ǿ���fdt����������4�ֽ����ܱ�����
    //����4��ָ�빹����rsc���Ƶ������������
    uint8_t  parent[4];          //��Ŀ¼��struct fdt_info��Ŀ��
    uint8_t  fstart_dson[4];     //�ļ���ʼ���/�ļ��е�����Ŀ��
    uint8_t  previous[4];        //ͬĿ¼�ڵ�ǰһ���ļ���Ŀ��
    uint8_t  next[4];            //ͬĿ¼�ڵĺ�һ���ļ���Ŀ��
    char     name[cn_ram_file_name_limit+1];  //�ļ��������215���ַ�
};

struct MDR_temp_record_ramfs
{
    char name[cn_DBX_sum_max_ramfs][cn_ram_DBX_name_limit+1];  //�ļ�����
    bool_t   formatted[cn_DBX_sum_max_ramfs];         //�ļ����ʽ����
    uint32_t DDR_main_block_no[cn_DBX_sum_max_ramfs]; //DDR_main���
    uint32_t DDR_bak_block_no[cn_DBX_sum_max_ramfs];  //DDR_bak���
    uint32_t PCRB_block_no[cn_DBX_sum_max_ramfs];     //PCRB���
    uint32_t start_blocks[cn_DBX_sum_max_ramfs];      //�ļ�����ʼ���
    uint32_t blocks_sum[cn_DBX_sum_max_ramfs];        //�ļ������
};
//MDR����оƬ��������,ռ��512�ֽڣ����ṹֻ�Ƕ�����ʹ���˵Ĳ��֣�
//�ʲ���ʹ��sizeof�������ڴ档
struct MDR_CDR_mirror_ramfs
{
    char chip_init_flag[8];         //оƬ��ʼ����־(8�ֽ�=djyosfs)
    uint8_t chip_CDR_size[4];       //CDR�ߴ磬���ڳ���cn_MDR_CDR_size
    uint8_t chip_block_sum[4];      //flashоƬ�ܿ������������飩,
    uint8_t chip_block_size[4];     //��ߴ磬
    uint8_t chip_MDR_bak[4];        //MDR_bak��ţ�
    uint8_t chip_reserve[4];        //оƬ������������
    uint8_t chip_MDR_CDR_ECC[4];    //CDR��У����
};
//MDR�����ļ�����������,ռ��56�ֽڣ����ṹֻ�Ƕ�����ʹ���˵Ĳ��֣�
//�ʲ���ʹ��sizeof�������ڴ档
struct MDR_DBX_mirror_ramfs
{
    char  DBX_format_flag[8];       //�ļ����ʽ����־
    char  DBX_name[cn_ram_DBX_name_limit+1]; //�ļ��������൱��windows�ľ��
    uint8_t DBX_start_block[4];     //��ʼ��ţ���оƬ�еľ������,
    uint8_t DBX_blocks_sum[4];      //�ļ���ռ�ÿ���
    uint8_t chip_MDR_DBX_ECC[4];    //DBX��У����
};

bool_t djyramfs_uninstall_chip(struct st_ram_chip *chip);
bool_t djyramfs_format_DBX(uint32_t cmd,struct st_DBX_device_tag *DBX_device_tag);
uint32_t __djyramfs_locate_block(struct st_DBX_ram_tag *DBX_ram_tag,
                              struct file_rsc *fp,sint64_t offset);
uint32_t __djyramfs_last_block(struct st_DBX_ram_tag *DBX_ram_tag,
                            struct file_rsc *fp);
void __djyramfs_get_access_info(struct st_DBX_ram_tag *DBX_ram_tag,
                            struct file_rsc *fp,
                            struct file_access_tag_ramfs *access_tag);
bool_t __djyramfs_sync_buf_after_write(struct st_DBX_ram_tag *DBX_ram_tag,
                               djyfs_file *fp,uint32_t write_len);
uint32_t __djyramfs_write_update_block(struct st_DBX_ram_tag *DBX_ram_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
uint32_t __djyramfs_write_append_block(struct st_DBX_ram_tag *DBX_ram_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
uint32_t __djyramfs_write_new_block(struct st_DBX_ram_tag *DBX_ram_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
bool_t __djyramfs_move_file_read_ptr(struct st_DBX_ram_tag *DBX_ram_tag,
                            djyfs_file *fp,sint64_t new_ptr);
bool_t __djyramfs_move_file_write_ptr(struct st_DBX_ram_tag *DBX_ram_tag,
                             djyfs_file *fp,sint64_t new_ptr);
uint32_t __djyramfs_write_buf_to_flash(struct st_DBX_ram_tag *DBX_ram_tag,
                                    struct file_rsc *fp);
bool_t __djyramfs_sync_buf_before_read(struct st_DBX_ram_tag *DBX_ram_tag,
                              djyfs_file *fp,uint32_t read_len);
uint32_t djyramfs_write_file(struct file_rsc *fp,uint8_t *buf,uint32_t len);
uint32_t djyramfs_read_file(struct file_rsc *fp,uint8_t *buf,uint32_t len);
uint32_t djyramfs_flush_file(struct file_rsc *fp);
sint64_t djyramfs_query_file_stocks(struct file_rsc *fp);
sint64_t djyramfs_query_file_cubage(struct file_rsc *fp);
sint64_t __djyramfs_set_file_size(struct file_rsc *fp,sint64_t new_size);
uint32_t djyramfs_seek_file(struct file_rsc *fp,struct seek_para *position);
bool_t djyramfs_create_file(char *name,struct file_rsc *parent,
                         union file_attrs attr);
uint32_t djyramfs_check_file(struct file_rsc *parent);
bool_t djyramfs_remove_file(struct file_rsc *fp);
bool_t __djyramfs_cmp_file_name(char *fs_name,char *FDT_name);
uint32_t djyramfs_open_item(char *name,struct file_rsc *parent,
                       struct file_rsc *result,enum file_open_mode mode);
bool_t djyramfs_lookfor_item(char *name,struct file_rsc *parent,
                         struct file_rsc *result);
ptu32_t module_init_djyramfs(ptu32_t para);
bool_t djyramfs_close_item(struct file_rsc *fp);
bool_t djyramfs_rename_item(struct file_rsc *fp,char *newname);
bool_t __djyramfs_repair_FDT(struct st_DBX_ram_tag *DBX_ram_tag);
uint32_t djyramfs_check_FDT_size(struct st_DBX_device_tag *DBX_device_tag);
void djyramfs_read_FDT(struct st_DBX_device_tag *DBX_device_tag,uint8_t *buf);
void __djyramfs_fill_DBX_device_tag(struct st_DBX_device_tag *DBX_device_tag,
                                 char *name);
void __djyramfs_scan_chip(struct st_ram_chip *chip,uint32_t start_block_no);
bool_t djyramfs_install_chip(struct st_ram_chip *chip,char *name,uint32_t rev_blocks);
bool_t __djyramfs_query_restored_block(void);
bool_t ramfs_creat_DBX_test(char *DBX_name);
#ifdef __cplusplus
}
#endif

#endif // _RAMFILE_H_

