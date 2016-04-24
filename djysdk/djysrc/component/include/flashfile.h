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
#ifndef __NANDFILE_H__
#define __NANDFILE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "rsc.h"
#include "file.h"

#define CN_MDR_DDR_MAIN     0xfe
#define CN_MDR_DDR_BAK      0xfd
#define CN_MDR_PCRB         0xfc
#define CN_PTT_SUM_MAX      3

#define CN_MDR_CDR_SIZE     512     //MDR����CDR�ĳߴ磬����8�ı���
#define CN_MDR_PTT_SIZE     56      //MDR����ÿ��PTT��ĳߴ磬����8�ı���
//MDR��CDR��PTT����ܳߴ�
#define CN_MDR_AND_PTT_SIZE (CN_MDR_CDR_SIZE + CN_PTT_SUM_MAX*CN_MDR_PTT_SIZE)
//������¼��ƫ�Ƶ�ַ
#define CN_ROLL_OFFSET       (((CN_MDR_AND_PTT_SIZE + 7) /8)*8)
//MDR��ĳߴ磬������С��856��������DFFSD_install_chip�ᱨ���������ע��
#define CN_MDR_SIZE         4096

struct tagStMDR_Flag
{
    bool_t PTT_created[CN_PTT_SUM_MAX];   //true=�����Ѿ�������false=��δ����.���ж�
    uint32_t MDR_main_block;              //�洢MDR_main����ʼ���
    uint32_t MDR_bak_block;               //�洢MDR_bak����ʼ���
    uint32_t roll_next_main;              //�����洢����һ���洢λ��
    uint32_t roll_next_bak;               //�����洢����һ���洢λ��
};

struct tagFlashChip
{
    struct tagRscNode chip_node;
    uint32_t block_sum;         //�ܿ���
    uint32_t block_size;        //��ߴ�
    uint32_t valid_sum;         //���ÿ���,��������������
    uint32_t rev_blocks;        //����������,�û��趨
    uint32_t rev_start;         //�������׿��ţ�������ǰ��оƬ�����顣�������Ϊ��ʼ
    uint32_t PTT_start;         //���ڷ������׿���,�������Ϊ��ʼ
    uint32_t PTT_read_buf_size; //���ļ��������ߴ�
    uint32_t PTT_write_buf_size;//д�ļ��������ߴ�
    struct tagStMDR_Flag MDR;     //MDR����ز���
    uint8_t  *block_buf;        //�黺����
    struct tagMutexLCB *chip_mutex;
    pHeap_t ChipHeap;

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

struct tagMAT_Table
{
    uint32_t previous;      //��ǰָ��(��ţ���C���Ե�ָ�벻ͬ)����30λ��Ч
    uint32_t next;          //���ָ�룬��30λ��Ч��
    uint8_t  attr;          //�����ԣ���cn_file_block�峣������
};
//���ṹ��st_PTT_device_tag�ṹ�е�medium_tag��Աָ��оƬdriver���ɼ�
struct tagStPTT_Flash
{
    struct tagFlashChip *chip;    //��������оƬ
    bool_t   nand_ecc;          //�Ƿ���eccУ�飬һ����˵��nand����flash����ý��
                                //�洢��ʱ���ɲ���eccУ�顣
    uint32_t PTT_no;            //������оƬ�еķ������
    uint32_t start_block;       //��ʼ��ţ������оƬ�ľ��Կ��,�������
    uint32_t block_sum;         //�ܿ���
    uint32_t valid_sum;         //��Ч����
    uint32_t free_sum;          //���п���
    uint32_t PCRB_no;           //����ָ���Ŀ�ţ������ڵ���Կ��
    uint32_t DDR_main;          //�׸�DDR���ţ������ڵ���Կ��
    uint32_t DDR_bak;           //�ڶ���DDR���ţ������ڵ���Կ��
    uint32_t ero;               //����״̬
    uint32_t FDT_capacity;      //�Ѿ������FDT���FDT_item�����������ܹ��ı�����
    uint32_t FDT_free_num;      //�Ѿ�ʹ�õ�FDT��������������ʹ�õ�FDT����
    uint32_t DDR_size;          //DDR��ߴ�
    uint32_t *DDR_DBL;          //DBL����main��bak
    uint32_t DDR_ARB;         //ĥ�������������ĥ�������(16bit)��ʵ��ĥ�����
    uint32_t DDR_FDSB;          //Ŀ¼���׿���,��������Կ��
    struct tagMAT_Table *DDR_MAT;  //MAT��
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

struct tagFileAccess
{
    sint64_t true_size;         //�ļ�ʵ�ʳ��ȣ���ֻ��ӳ�Ѿ�д��flash�����ݣ�
                                //��fp�е�file_size�����д�������е����ݣ�
    uint32_t last_block_no;     //���һ��Ŀ��
    uint32_t write_block_no;    //дָ�����ڿ��
    uint32_t read_block_no;     //��ָ�����ڿ��
};

//�����ļ�����,��fat��ʽ���е����Է���fat��ͬ��λ��,����fdt���е�attr��Ա��
//�漰����������Ϊ���죬����ʹ��ö��������(enum)��
#define CN_FS_ATTR_READ_ONLY   (1<<0)
#define CN_FS_ATTR_HIDDEN      (1<<1)
#define CN_FS_ATTR_USER        (1<<2)   //�û�����
#define CN_FS_ATTR_CLOSEDELETE (1<<3)   //����
#define CN_FS_ATTR_DIRECTORY   (1<<4)   //Ŀ¼
#define CN_FS_ATTR_ARCHIVE     (1<<5)    //�ļ�
#define CN_FS_ATTR_DELETED     (1<<7)   //����(ɾ��)��־
//�ر����һ��cn_FS_ATTR_USER����fat�У����λ��λ��������ʾ�Ƿ�ϵͳ�ļ���Ȼ����
//ĳ�ļ��Ƿ���Ϊϵͳ�ļ������ļ�ϵͳʹ���ߵ����⣬�ļ�ϵͳdriver��Ӧ�ù��ʵģ�
//���ԣ�������ط���DFFSDģ�����һ�����û����͵�λ��
//ͬ����cn_FS_ATTR_RESERVEλ������fat�ľ��λ���಻�á�

#define CN_FLASH_FILE_NAME_LIMIT    215     //δ����fat��֤�����256�ֽڣ�fat�����׼���
#define CN_FLASH_PTT_NAME_LIMIT     31      //flash�������ֳ���

//���ṹ��flashd river��˽�����ݽṹ��оƬdriver(����nand.c)���ɼ�
struct tagFdtInfo
{
    uint8_t  mattr;              //�̶���ʽ�����ڴ洢�����е��ļ�����,
    uint8_t  name_buf_len;       //�ļ���buf���ȣ�Ӧ=CN_FLASH_FILE_NAME_LIMIT+1
                                 //�ṩ��Ӧ�ó�����Ŀ¼����

    sint32_t CreateTime; //����ʱ��
    sint32_t ModTime; //������޸�ʱ��

    uint8_t  file_size[8];       //�ļ��ߴ磬��Ŀ¼��file_size�����;�Ƚ����⣬
                                 //ǰ4�ֽ��ǿ���fdt����������4�ֽ����ܱ�����
    //����4��ָ�빹����rsc���Ƶ������������
    uint8_t  parent[4];          //��Ŀ¼��struct fdt_info��Ŀ��
    uint8_t  fstart_dson[4];     //�ļ���ʼ���/�ļ��е�����Ŀ��
    uint8_t  previous[4];        //ͬĿ¼�ڵ�ǰһ���ļ���Ŀ��
    uint8_t  next[4];            //ͬĿ¼�ڵĺ�һ���ļ���Ŀ��
    char     name[CN_FLASH_FILE_NAME_LIMIT+1];  //�ļ��������215���ַ�
};

struct tagMDR_TempRecord
{
    char name[CN_PTT_SUM_MAX][CN_FLASH_PTT_NAME_LIMIT+1];  //������
    bool_t   formatted[CN_PTT_SUM_MAX];         //������ʽ����
    uint32_t DDR_main_block_no[CN_PTT_SUM_MAX]; //DDR_main���
    uint32_t DDR_bak_block_no[CN_PTT_SUM_MAX];  //DDR_bak���
    uint32_t PCRB_block_no[CN_PTT_SUM_MAX];     //PCRB���
    uint32_t start_blocks[CN_PTT_SUM_MAX];      //������ʼ���
    uint32_t blocks_sum[CN_PTT_SUM_MAX];        //��������
};
//MDR����оƬ��������,ռ��512�ֽڣ����ṹֻ�Ƕ�����ʹ���˵Ĳ��֣�
//�ʲ���ʹ��sizeof�������ڴ档
struct tagMDR_CDR_Mirror
{
    char chip_init_flag[8];         //оƬ��ʼ����־(8�ֽ�=djyosfs)
    uint8_t chip_CDR_size[4];       //CDR�ߴ磬���ڳ���cn_MDR_CDR_size
    uint8_t chip_block_sum[4];      //flashоƬ�ܿ������������飩,
    uint8_t chip_block_size[4];     //��ߴ磬
    uint8_t chip_MDR_bak[4];        //MDR_bak��ţ�
    uint8_t chip_reserve[4];        //оƬ������������
    uint8_t chip_MDR_CDR_ECC[4];    //CDR��У����
};
//MDR���з�����������,ռ��56�ֽڣ����ṹֻ�Ƕ�����ʹ���˵Ĳ��֣�
//�ʲ���ʹ��sizeof�������ڴ档
struct tagMDR_PTT_Mirror
{
    char  PTT_format_flag[8];       //������ʽ����־
    char  PTT_name[CN_FLASH_PTT_NAME_LIMIT+1]; //���������൱��windows�ľ��
    uint8_t PTT_start_block[4];     //��ʼ��ţ���оƬ�еľ������,
    uint8_t PTT_blocks_sum[4];      //����ռ�ÿ���
    uint8_t chip_MDR_PTT_ECC[4];    //PTT��У����
};

bool_t DFFSD_UninstallChip(struct tagFlashChip *chip);
bool_t DFFSD_FormatPTT(u32 fmt_para1,u32 fmt_para2,
                        struct tagPTTDevice *PTT_device_tag);
uint32_t __DFFSD_LocateBlock(struct tagStPTT_Flash *PTT_flash_tag,
                              struct tagFileRsc *fp,sint64_t offset);
uint32_t __DFFSD_LastBlock(struct tagStPTT_Flash *PTT_flash_tag,
                            struct tagFileRsc *fp);
void __DFFSD_GetAccessInfo(struct tagStPTT_Flash *PTT_flash_tag,
                            struct tagFileRsc *fp,
                            struct tagFileAccess *access_tag);
bool_t __DFFSD_SyncBufAfterWrite(struct tagStPTT_Flash *PTT_flash_tag,
                               FILE *fp,uint32_t write_len);
uint32_t __DFFSD_WriteUpdateBlock(struct tagStPTT_Flash *PTT_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
uint32_t __DFFSD_WriteAppendBlock(struct tagStPTT_Flash *PTT_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
uint32_t __DFFSD_WriteNewBlock(struct tagStPTT_Flash *PTT_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size);
bool_t __DFFSD_MoveFileReadPtr(struct tagStPTT_Flash *PTT_flash_tag,
                            FILE *fp,sint64_t new_ptr);
bool_t __DFFSD_MoveFileWritePtr(struct tagStPTT_Flash *PTT_flash_tag,
                             FILE *fp,sint64_t new_ptr);
uint32_t __DFFSD_WriteBufToFlash(struct tagStPTT_Flash *PTT_flash_tag,
                                    struct tagFileRsc *fp);
bool_t __DFFSD_SyncBufBeforeRead(struct tagStPTT_Flash *PTT_flash_tag,
                              FILE *fp,uint32_t read_len);
uint32_t DFFSD_WriteFile(struct tagFileRsc *fp,uint8_t *buf,uint32_t len);
uint32_t DFFSD_ReadFile(struct tagFileRsc *fp,uint8_t *buf,uint32_t len);
uint32_t DFFSD_FlushFile(struct tagFileRsc *fp);
sint64_t DFFSD_QueryFileStocks(struct tagFileRsc *fp);
sint64_t DFFSD_QueryFileCubage(struct tagFileRsc *fp);
sint64_t DFFSD_SetFileSize(struct tagFileRsc *fp,sint64_t size);
uint32_t DFFSD_SeekFile(struct tagFileRsc *fp,struct tagSeekPara *position);
bool_t DFFSD_CreateFile(char *name,struct tagFileRsc *parent,
                         union file_attrs attr);
uint32_t DFFSD_CheckFile(struct tagFileRsc *parent);
bool_t DFFSD_RemoveFile(struct tagFileRsc *fp);
bool_t __DFFSD_CmpFileName(char *fs_name,char *FDT_name);
uint32_t DFFSD_OpenItem(char *name,struct tagFileRsc *parent,
                       struct tagFileRsc *result,enum _FILE_OPEN_MODE_ mode);
bool_t DFFSD_LookForItem(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result);
ptu32_t ModuleInstall_DFFSD(ptu32_t para);
bool_t DFFSD_CloseItem(struct tagFileRsc *fp);
bool_t DFFSD_RenameItem(struct tagFileRsc *fp,char *newname);
bool_t __DFFSD_RepairFDT(struct tagStPTT_Flash *PTT_flash_tag);
uint32_t DFFSD_CheckFDT_Size(struct tagPTTDevice *PTT_device_tag);
void DFFSD_ReadFDT(struct tagPTTDevice *PTT_device_tag,uint8_t *buf);
void __DFFSD_Fill_FDT_Device(struct tagPTTDevice *PTT_device_tag,
                                 char *name);
void __DFFSD_ScanChip(struct tagFlashChip *chip,uint32_t start_block_no);
bool_t DFFSD_InstallChip(struct tagFlashChip *chip,char *name,uint32_t rev_blocks);
bool_t DFFSD_CreatPTT(char *PTT_name);//���ﺯ������δ�ҵ�,�ǲ���д����?

#ifdef __cplusplus
}
#endif

#endif // __NANDFILE_H__

