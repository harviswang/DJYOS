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
#ifndef _NANDFILE_H_
#define _NANDFILE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "os_inc.h"

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

#include "ecc256.h"


#define cn_MDR_DDR_main     0xfe
#define cn_MDR_DDR_bak      0xfd
#define cn_MDR_PCRB         0xfc
#define cn_DBX_sum_max      3

#define cn_MDR_CDR_size     512     //MDR����CDR�ĳߴ磬����8�ı���
#define cn_MDR_DBX_size     56      //MDR����ÿ��DBX��ĳߴ磬����8�ı���
//MDR��CDR��DBX����ܳߴ�
#define cn_CDR_and_DBX_size (cn_MDR_CDR_size + cn_DBX_sum_max*cn_MDR_DBX_size)
//������¼��ƫ�Ƶ�ַ
#define cn_roll_offset       (((cn_CDR_and_DBX_size + 7) /8)*8)
//MDR��ĳߴ磬������С��856��������DFFSD_install_chip�ᱨ���������ע��
#define cn_MDR_size         4096


struct st_MDR_flag
{
    bool_t DBX_created[cn_DBX_sum_max];   //true=�ļ����Ѿ�������false=��δ����
    uint32_t MDR_main_block;              //�洢MDR_main����ʼ���
    uint32_t MDR_bak_block;               //�洢MDR_bak����ʼ���
    uint32_t roll_next_main;              //�����洢����һ���洢λ��
    uint32_t roll_next_bak;               //�����洢����һ���洢λ��
};

struct flash_chip
{
    struct rsc_node chip_node;
    uint32_t block_sum;         //�ܿ���
    uint32_t block_size;        //��ߴ�
    uint32_t valid_sum;         //���ÿ���
    uint32_t rev_blocks;        //����������
    uint32_t rev_start;         //�������׿��ţ�������ǰ��оƬ�����顣
    uint32_t DBX_start;         //�����ļ�����׿���
    uint32_t DBX_read_buf_size; //���ļ��������ߴ�
    uint32_t DBX_write_buf_size;//д�ļ��������ߴ�
    struct st_MDR_flag MDR;     //MDR����ز���
    uint8_t  *block_buf;        //�黺����
    struct semaphore_LCB *left_semaphore;
    struct semaphore_LCB *right_semaphore;

    //�����ǵ��͵�flash��������ָ�룬��дһ������оƬ������ʱ����Ϊ����оƬ��
    //��������,��Щ����������ÿһ��оƬ����Ҫ������Ҫ����NULL��

    //���Ѳ����涨������д��flashǰ���Ƿ���Ҫ���������buf=NULL������offset
    //��size�趨�������Ƿ񱻲���
    //true = ��׼���ã�����Ҫ����(��տ�),false = ��Ҫ������
    bool_t   (*query_block_ready_with_ecc)(uint32_t block,uint32_t offset,
                                  uint8_t *buf,uint32_t size);
    bool_t   (*query_block_ready_no_ecc)(uint32_t block,uint32_t offset,
                                  uint8_t *buf,uint32_t size);
    //org_data�л������flash��ԭ�������ݣ����޸�Ϊnew_data�����ݣ�����Ƿ���Ҫ
    //�Ȳ�����new_data==NULL�����org_data��Ϊ�κ�����ʱ�Ƿ���Ҫ����,
    //û�д�ecc�汾��
    bool_t   (*query_ready_with_data)(uint8_t *new_data,uint8_t *org_data,
                                            uint32_t size);
    //����1�飬����: true = �ɹ�������false = ����
    bool_t (*erase_block)(uint32_t block);
    bool_t   (*check_block)(uint32_t Block);  //����: true=�ÿ飬false=���顣

    //��һ��֮�ڵ����ⳤ�����ݣ�����У�������ȷ����
    //���أ�cn_limit_uint32=������������ֵ��ʵ��д������
    uint32_t (*read_data_with_ecc)(uint32_t block,uint32_t offset,
                          uint8_t *buf,uint32_t size);
    //д��һ��֮�ڵ����ⳤ�����ݣ�У�鷽��ȡ���ھ���������������Ҳ������У�顣
    //���أ�cn_limit_uint32=������������ֵ��ʵ��д������
    uint32_t (*write_data_with_ecc)(uint32_t block,uint32_t offset,
                           uint8_t *buf,uint32_t size);
    //����У��Ķ����������� ʵ�ʶ�ȡ�������������������򷵻�cn_limit_uint32
    uint32_t (*read_data_no_ecc)(uint32_t block,uint32_t offset,
                          uint8_t *buf,uint32_t size);
    //����У��Ķ����������أ�cn_limit_uint32=������������ֵ��ʵ��д������
    uint32_t (*write_data_no_ecc)(uint32_t block,uint32_t offset,
                           uint8_t *buf,uint32_t size);
    //д���籣����,PCR_blockΪ������籣����Ϣ�Ŀ�ţ�protected_block�Ǳ�������
    //��ţ�����оƬ�ľ��Կ�š�
    //����: true = �ɹ���false = д�����PCRB_block�ǻ���
    bool_t (*write_PCRB)(uint32_t PCRB_block,
                         uint32_t protected_block,uint8_t *buf);
    //�ѵ���ָ�������ݻָ���Ŀ��飬��������ʹ����ָ��㷨��ȫ��оƬ�����ṩ
    //PCRB_block�ǵ���ָ���ľ��Կ�š�
    //����: true=�ɹ��ָ�������ָ���false=�ָ�ʧ��
    bool_t (*restore_PCRB)(uint32_t PCRB_block,uint32_t *restored);
};

//MAT��ǰָ��ͺ�ָ��������λ���������ʾ�ÿ�����ԣ����16�����ԣ�Ŀǰ�õ�7��
//��ǰָ��ĸ���λΪ�ͣ���ָ��Ϊ�ߡ����ԣ�MAT��ʵ������30λ�ġ�
#define cn_file_block_start     0   //�ļ�MAT�����׿�(�п���ͬʱҲ��ĩ��)
#define cn_file_block_continue  1   //�ļ�MAT���ĺ�����
#define cn_FDT_block            2   //�����Ŀ¼��Ŀ�
#define cn_invalid_block        3   //����
#define cn_free_block           4   //���п�
#define cn_DDR_main_block       5   //���ڱ���DDR_main�Ŀ�
#define cn_DDR_bak_block        6   //���ڱ���DDR_bak�Ŀ�
#define cn_PCRB_block           7   //���ڱ������ָ����ݵĿ�

struct MAT_table
{
    uint32_t previous;      //��ǰָ��(��ţ���C���Ե�ָ�벻ͬ)����30λ��Ч
    uint32_t next;          //���ָ�룬��30λ��Ч��
    uint8_t  attr;          //�����ԣ���cn_file_block�峣������
};
//���ṹ��st_DBX_device_tag�ṹ�е�medium_tag��Աָ��оƬdriver���ɼ�
struct st_DBX_flash_tag
{
    struct flash_chip *chip;    //�ļ�������оƬ
    bool_t   nand_ecc;          //�Ƿ���eccУ�飬һ����˵��nand����flash����ý��
                                //�洢��ʱ���ɲ���eccУ�顣
    uint32_t DBX_no;            //������оƬ�е��ļ�����
    uint32_t start_block;       //��ʼ��ţ������оƬ�ľ��Կ��
    uint32_t block_sum;         //�ܿ���
    uint32_t valid_sum;         //��Ч����
    uint32_t free_sum;          //���п���
    uint32_t PCRB_no;           //����ָ���Ŀ�ţ��ļ����ڵ���Կ��
    uint32_t DDR_main;          //�׸�DDR���ţ��ļ����ڵ���Կ��
    uint32_t DDR_bak;           //�ڶ���DDR���ţ��ļ����ڵ���Կ��
    uint32_t ero;               //�ļ���״̬
    uint32_t FDT_capacity;      //�Ѿ������FDT���FDT_item�������ļ����ܹ��ı�����
    uint32_t FDT_free_num;      //�Ѿ�ʹ�õ�FDT��������������ʹ�õ�FDT����
    uint32_t DDR_size;          //DDR��ߴ�
    uint32_t *DDR_DBL;          //DBL����main��bak
    uint32_t DDR_ARB;         //ĥ�������������ĥ�������(16bit)��ʵ��ĥ�����
    uint32_t DDR_FDSB;          //Ŀ¼���׿���
    struct MAT_table *DDR_MAT;  //MAT��
    //��DDR��д���־���飬true=��д��flash��false=δд�������С��DDR��ռ�õ�
    //������ͬ��������һ����
    bool_t  *writed_DDR_main;
    bool_t  *writed_DDR_bak;
    //�����������ڷ����ڴ�ʱ�밴������Ҫ��֤�߽����(ʹ��djyos��̬�ڴ����
    //���Ա�֤)
    uint32_t *ART_position; //ĥ��λ�ñ��ÿ���ĥ��˳����е�λ�ã�
    uint32_t *ART_block_no; //ĥ���ű��������ͬ�У������������ж�Ӧ��Ŀ��
    uint16_t *ART_times;    //ĥ�������æ����п�ֱ�ĥ���������
    uint32_t balance_region[19];//ĥ�������æ���п��9������������ǻ������
};

struct file_access_tag
{
    sint64_t true_size;         //�ļ�ʵ�ʳ��ȣ���ֻ��ӳ�Ѿ�д��flash�����ݣ�
                                //��fp�е�file_size�����д�������е����ݣ�
    uint32_t last_block_no;     //���һ��Ŀ��
    uint32_t write_block_no;    //дָ�����ڿ��
    uint32_t read_block_no;     //��ָ�����ڿ��
};

//�����ļ�����,��fat��ʽ���е����Է���fat��ͬ��λ��,����fdt���е�attr��Ա��
//�漰����������Ϊ���죬����ʹ��ö��������(enum)��
#define cn_FS_ATTR_READ_ONLY   (1<<0)
#define cn_FS_ATTR_HIDDEN      (1<<1)
#define cn_FS_ATTR_USER        (1<<2)   //�û�����
#define cn_FS_ATTR_RESERVE     (1<<3)   //����
#define cn_FS_ATTR_DIRECTORY   (1<<4)   //Ŀ¼
#define cn_FS_ATTR_ARCHIVE     (1<<5)    //�ļ�
#define cn_FS_ATTR_DELETED     (1<<7)   //����(ɾ��)��־
//�ر����һ��cn_FS_ATTR_USER����fat�У����λ��λ��������ʾ�Ƿ�ϵͳ�ļ���Ȼ����
//ĳ�ļ��Ƿ���Ϊϵͳ�ļ������ļ�ϵͳʹ���ߵ����⣬�ļ�ϵͳdriver��Ӧ�ù��ʵģ�
//���ԣ�������ط���DFFSDģ�����һ�����û����͵�λ��
//ͬ����cn_FS_ATTR_RESERVEλ������fat�ľ��λ���಻�á�

#define cn_flash_file_name_limit    215     //δ����fat��֤�����256�ֽڣ�fat�����׼���
#define cn_flash_DBX_name_limit     31      //flash�ļ������ֳ���

//���ṹ��flashd river��˽�����ݽṹ��оƬdriver(����nand.c)���ɼ�
struct fdt_info
{
    uint8_t  mattr;              //�̶���ʽ�����ڴ洢�����е��ļ�����,
    uint8_t  name_buf_len;       //�ļ���buf���ȣ�Ӧ=cn_flash_file_name_limit+1
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
    char     name[cn_flash_file_name_limit+1];  //�ļ��������215���ַ�
};

struct MDR_temp_record
{
    char name[cn_DBX_sum_max][cn_flash_DBX_name_limit+1];  //�ļ�����
    bool_t   formatted[cn_DBX_sum_max];         //�ļ����ʽ����
    uint32_t DDR_main_block_no[cn_DBX_sum_max]; //DDR_main���
    uint32_t DDR_bak_block_no[cn_DBX_sum_max];  //DDR_bak���
    uint32_t PCRB_block_no[cn_DBX_sum_max];     //PCRB���
    uint32_t start_blocks[cn_DBX_sum_max];      //�ļ�����ʼ���
    uint32_t blocks_sum[cn_DBX_sum_max];        //�ļ������
};
//MDR����оƬ��������,ռ��512�ֽڣ����ṹֻ�Ƕ�����ʹ���˵Ĳ��֣�
//�ʲ���ʹ��sizeof�������ڴ档
struct MDR_CDR_mirror
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
struct MDR_DBX_mirror
{
    char  DBX_format_flag[8];       //�ļ����ʽ����־
    char  DBX_name[cn_flash_DBX_name_limit+1]; //�ļ��������൱��windows�ľ��
    uint8_t DBX_start_block[4];     //��ʼ��ţ���оƬ�еľ������,
    uint8_t DBX_blocks_sum[4];      //�ļ���ռ�ÿ���
    uint8_t chip_MDR_DBX_ECC[4];    //DBX��У����
};

bool_t DFFSD_uninstall_chip(struct flash_chip *chip);
bool_t DFFSD_format_DBX(u32 fmt_para1,u32 fmt_para2,
                        struct st_DBX_device_tag *DBX_device_tag);
uint32_t __DFFSD_locate_block(struct st_DBX_flash_tag *DBX_flash_tag,
                              struct file_rsc *fp,sint64_t offset);
uint32_t __DFFSD_last_block(struct st_DBX_flash_tag *DBX_flash_tag,
                            struct file_rsc *fp);
void __DFFSD_get_access_info(struct st_DBX_flash_tag *DBX_flash_tag,
                            struct file_rsc *fp,
                            struct file_access_tag *access_tag);
bool_t __DFFSD_sync_buf_after_write(struct st_DBX_flash_tag *DBX_flash_tag,
                               djyfs_file *fp,uint32_t write_len);
uint32_t __DFFSD_write_update_block(struct st_DBX_flash_tag *DBX_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
uint32_t __DFFSD_write_append_block(struct st_DBX_flash_tag *DBX_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
uint32_t __DFFSD_write_new_block(struct st_DBX_flash_tag *DBX_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
bool_t __DFFSD_move_file_read_ptr(struct st_DBX_flash_tag *DBX_flash_tag,
                            djyfs_file *fp,sint64_t new_ptr);
bool_t __DFFSD_move_file_write_ptr(struct st_DBX_flash_tag *DBX_flash_tag,
                             djyfs_file *fp,sint64_t new_ptr);
uint32_t __DFFSD_write_buf_to_flash(struct st_DBX_flash_tag *DBX_flash_tag,
                                    struct file_rsc *fp);
bool_t __DFFSD_sync_buf_before_read(struct st_DBX_flash_tag *DBX_flash_tag,
                              djyfs_file *fp,uint32_t read_len);
uint32_t DFFSD_write_file(struct file_rsc *fp,uint8_t *buf,uint32_t len);
uint32_t DFFSD_read_file(struct file_rsc *fp,uint8_t *buf,uint32_t len);
uint32_t DFFSD_flush_file(struct file_rsc *fp);
sint64_t DFFSD_query_file_stocks(struct file_rsc *fp);
sint64_t DFFSD_query_file_cubage(struct file_rsc *fp);
sint64_t __DFFSD_set_file_size(struct file_rsc *fp,sint64_t new_size);
uint32_t DFFSD_seek_file(struct file_rsc *fp,struct seek_para *position);
bool_t DFFSD_create_file(char *name,struct file_rsc *parent,
                         union file_attrs attr);
uint32_t DFFSD_check_file(struct file_rsc *parent);
bool_t DFFSD_remove_file(struct file_rsc *fp);
bool_t __DFFSD_cmp_file_name(char *fs_name,char *FDT_name);
uint32_t DFFSD_open_item(char *name,struct file_rsc *parent,
                       struct file_rsc *result,enum file_open_mode mode);
bool_t DFFSD_lookfor_item(char *name,struct file_rsc *parent,
                         struct file_rsc *result);
ptu32_t module_init_DFFSD(ptu32_t para);
bool_t DFFSD_close_item(struct file_rsc *fp);
bool_t DFFSD_rename_item(struct file_rsc *fp,char *newname);
bool_t __DFFSD_repair_FDT(struct st_DBX_flash_tag *DBX_flash_tag);
uint32_t DFFSD_check_FDT_size(struct st_DBX_device_tag *DBX_device_tag);
void DFFSD_read_FDT(struct st_DBX_device_tag *DBX_device_tag,uint8_t *buf);
void __DFFSD_fill_DBX_device_tag(struct st_DBX_device_tag *DBX_device_tag,
                                 char *name);
void __DFFSD_scan_chip(struct flash_chip *chip,uint32_t start_block_no);
bool_t DFFSD_install_chip(struct flash_chip *chip,char *name,uint32_t rev_blocks);
bool_t creat_DBX(char *DBX_name);

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))


#ifdef __cplusplus
}
#endif

#endif // _NANDFILE_H_

