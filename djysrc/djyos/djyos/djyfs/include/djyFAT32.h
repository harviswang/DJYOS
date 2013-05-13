#ifndef __ZNFAT_H__
#define __ZNFAT_H__

//#include "mytype.h" //�����ض���
//#include "stm32f10x_lib.h"
#include "os_inc.h"

// ���ڼ���صĿ�ʼ����
#define SOC(c) (((c-pArg->FirstDirClust)*(pArg->SectorsPerClust))+pArg->FirstDirSector) 

#define DEVS 1 //��������洢�豸�ĸ���������ֻ��1���洢�豸������SD��
struct FAT32_init_arg *pArg;

 //�豸��
#define SDCARD 0 //SD��
#define UDISK  1 //U��
#define CFCARD 2 //CF��
#define OTHER  3 //����
                                         //����Ĵ洢�豸������������䣬��ʵ�ֶԸ���洢�豸��֧��
                        //-------------------------------------------
                        
#define MAKE_FILE_TIME(h,m,s)    ((((unsigned int)h)<<11)+(((unsigned int)m)<<5)+(((unsigned int)s)>>1))    /* ����ָ��ʱ������ļ�ʱ������ */
#define MAKE_FILE_DATE(y,m,d)    (((((unsigned int)y)+20)<<9)+(((unsigned int)m)<<5)+((unsigned int)d))    /* ����ָ�������յ��ļ��������� */
                        
//DPT:������¼�ṹ����
struct part_record
{
uint8_t active;         //0x80��ʾ�˷�����Ч
uint8_t start_head;      //�����Ŀ�ʼ��ͷ
uint8_t start_cyl_sect[2];//��ʼ����������
uint8_t part_type;       //��������
uint8_t end_head;        //�����Ľ���ͷ
uint8_t end_cyl_sect[2];  //��������������
uint8_t start_LBA[4];    //�����ĵ�һ������
uint8_t size[4];        //�����Ĵ�С 
};
                        
//MBR:��������������0��������������
struct part_sector
{
uint8_t PartCode[446]; //MBR����������
struct part_record Part[4];   // 4��������¼
uint8_t BootSectSig0;  //55
uint8_t BootSectSig1;  //AA
};

/* FDT�ļ����� */
#define cn_FAT_ATTR_READ_ONLY              0x01
#define cn_FAT_ATTR_HIDDEN                 0x02
#define cn_FAT_ATTR_SYSTEM                 0x04
#define cn_FAT_ATTR_VOLUME_ID              0x08
#define cn_FAT_ATTR_DIRECTORY              0x10
#define cn_FAT_ATTR_ARCHIVE                0x20

//FAT32�ж�BPB�Ķ�������  һ��ռ��90���ֽ�
struct FAT32_BPB
{
uint8_t BS_jmpBoot[3];     //��תָ��            offset: 0
uint8_t BS_OEMName[8];     //                    offset: 3
uint8_t BPB_BytesPerSec[2];//ÿ�����ֽ���        offset:11
uint8_t BPB_SecPerClus[1]; //ÿ��������          offset:13
uint8_t BPB_RsvdSecCnt[2]; //����������Ŀ        offset:14
uint8_t BPB_NumFATs[1];    //�˾���FAT����       offset:16
uint8_t BPB_RootEntCnt[2]; //FAT32Ϊ0            offset:17
uint8_t BPB_TotSec16[2];   //FAT32Ϊ0            offset:19
uint8_t BPB_Media[1];      //�洢����            offset:21
uint8_t BPB_FATSz16[2];    //FAT32Ϊ0            offset:22
uint8_t BPB_SecPerTrk[2];  //�ŵ�������          offset:24
uint8_t BPB_NumHeads[2];   //��ͷ��              offset:26
uint8_t BPB_HiddSec[4];    //FAT��ǰ��������     offset:28
uint8_t BPB_TotSec32[4];   //�þ���������        offset:32
uint8_t BPB_FATSz32[4];    //һ��FAT��������     offset:36
uint8_t BPB_ExtFlags[2];   //FAT32����           offset:40
uint8_t BPB_FSVer[2];      //FAT32����           offset:42
uint8_t BPB_RootClus[4];   //��Ŀ¼�غ�          offset:44
uint8_t fs_info[2];         //��������FSINFO������offset:48
uint8_t BPB_BkBootSec[2];  //ͨ��Ϊ6             offset:50
uint8_t BPB_Reserved[12];  //��չ��              offset:52
uint8_t BS_DrvNum[1];      //                    offset:64
uint8_t BS_Reserved1[1];   //                    offset:65
uint8_t BS_BootSig[1];     //                    offset:66
uint8_t BS_VolID[4];       //                    offset:67
uint8_t BS_FilSysType[11]; //                    offset:71
uint8_t BS_FilSysType1[8]; //"FAT32    "         offset:82
};
                        
struct FAT32_FAT_Item
{
uint8_t Item[4];
};
struct FAT32_FAT
{
struct FAT32_FAT_Item Items[128];
};
//�洢�������Ŀ¼�ṹ��,��Ŀ¼��                       
struct hardware_item_info 
{
uint8_t deName[8];       // �ļ��������㲿���Կո񲹳�
uint8_t deExtension[3];  // ��չ�������㲿���Կո񲹳�
uint8_t deAttributes;    // �ļ�����
uint8_t deLowerCase;     // 0
uint8_t deCHundredth;    // ����
uint8_t deCTime[2];      // ����ʱ��
uint8_t deCDate[2];      // ��������
uint8_t deADate[2];      // ��������
uint8_t deHighClust[2];  // ��ʼ�صĸ���
uint8_t deMTime[2];      // ������޸�ʱ��
uint8_t deMDate[2];      // ������޸�����
uint8_t deLowCluster[2];     // ��ʼ�صĵ���
uint8_t deFileSize[4];      // �ļ���С 
};
                        
//FAT32��ʼ��ʱ��ʼ����װ�����½ṹ����
struct FAT32_init_arg
{
uint8_t DEV_No;
                        
uint8_t BPB_Sector_No;   //BPB����������
uint32_t Total_Size;      //���̵�������
uint32_t FirstDirClust;   //��Ŀ¼�Ŀ�ʼ��
uint32_t BytesPerSector;    //ÿ���������ֽ���
uint32_t FATsectors;      //FAT����ռ������
uint32_t SectorsPerClust;    //ÿ�ص�������
uint32_t FirstFATSector;  //��һ��FAT����������
uint32_t FirstDirSector;  //��һ��Ŀ¼��������
};
                        
struct Date
{
uint16_t year;
uint8_t month;
uint8_t day;
};
                        
struct Time
{
uint8_t hour;
uint8_t min;
uint8_t sec;
};                       
//FAT32,�ļ���Դ�ṹ��
struct FAT32_file_rsc
{
uint8_t  FileName[12];       //�ļ���
uint32_t  FileStartCluster;   //�ļ��Ŀ�ʼ��
uint32_t  FileCurCluster;//�ļ��ĵ�ǰ��
uint32_t  FileSize;      
uint32_t  FileCurSector;     //�ļ��ĵ�ǰ����
uint16_t  FileCurPos;         //�ļ��ڵ�ǰ�����е�λ��
uint32_t  FileCurOffset;     //�ļ��ĵ�ǰƫ����
uint32_t  Rec_Sec;            //�ļ����ļ�/Ŀ¼�����ڵ�����
uint16_t  nRec;               //�ļ����ļ�/Ŀ¼�����������е�λ��
                        
uint8_t  FileAttr;       //�ļ�����              
struct Time    FileCreateTime;     //�ļ��Ĵ���ʱ��
struct Date    FileCreateDate;     //�ļ��Ĵ�������
struct Time    FileMTime;          //�ļ����޸�ʱ��
struct Date    FileMDate;          //�ļ����޸�����
struct Date    FileADate;          //�ļ��ķ�������
};
//FAT32���ļ�ϵͳ��Ϣ�ṹ
//����FSINFO��Ϣ�����ϵ�����
struct fs_info 
{
uint8_t Head[4];
uint8_t Resv1[480];
uint8_t Sign[4];
uint8_t Free_Cluster[4];
uint8_t Last_Cluster[4];
uint8_t Resv2[14];
uint8_t Tail[2];
};                       
uint8_t __read_sector(uint32_t addr,uint8_t *buf); //znFAT����ײ�洢�豸�������ӿڣ���ȡ����
uint8_t __write_sector(uint32_t addr,uint8_t *buf);//znFAT����ײ�洢�豸�������ӿڣ�д������
uint32_t __little_to_big(uint8_t *dat,uint8_t len); //С��ת���
sint8_t __lowercase_convert_capital(sint8_t c);
uint8_t __compare_dir_name(sint8_t *a,sint8_t *b);                       
uint16_t __find_DBR(void); //��λDBR
uint32_t __get_total_size(void); //��ȡ������                        
uint32_t __search_last_usable_cluster(void); //�ӻ�ȡFSInfo�е���һ�����ÿ��д�
sint64_t FAT32_query_file_cubage(void); //��ȡʣ������                      
void __update_fsinfo_free_clu(uint32_t PlusOrMinus); //����FSInfo�Ŀ��дص�����
void __update_fsinfo_last_clu(uint32_t Last_Clu); //����FSInfo����һ�����ÿ��д�                        
uint32_t __get_next_cluster(uint32_t LastCluster); //��ȡ��һ����
void __to_file_name(char *dName,char *eName,char *pName);
void __str_to_up(char *str);
uint8_t __seek_file(struct FAT32_file_rsc *FAT32_fp,uint32_t offset); //�ļ���λ 
uint32_t __find_free_clust(unsigned char flag);   //Ѱ����һ�����õĿ��д�
void __fill_rec_inf(struct hardware_item_info *prec,sint8_t *name,uint8_t is_dir,uint8_t *ptd);
void __modify_FAT32(uint32_t cluster,uint32_t dat);
void __empty_cluster(uint32_t Cluster);
sint8_t __create_rec(struct FAT32_file_rsc *FAT32_fp,uint32_t cluster,sint8_t *name,uint8_t is_dir,uint8_t *ptd); //�����ļ�/Ŀ¼��
void __direntry_to_file_rsc(struct hardware_item_info *pdir,struct FAT32_file_rsc *FAT32_fp,struct file_rsc *fp);
uint32_t FAT32_read_file(struct file_rsc *fp,uint8_t *pbuf,uint32_t len);
uint32_t FAT32_write_file(struct file_rsc *fp,uint8_t *pbuf,uint32_t len);
uint32_t FAT32_check_folder(struct file_rsc *parent);
bool_t FAT32_remove_item(struct file_rsc *fp);
bool_t FAT32_rename_item(struct file_rsc *fp,char *newname);
bool_t FAT32_format_DBX(uint32_t cmd,struct st_DBX_device_tag *DBX_device_tag);
uint32_t FAT32_lookfor_item(char *name,struct file_rsc *parent,struct file_rsc *result);
uint32_t FAT32_seek_file(struct file_rsc *fp,struct seek_para *pos);
uint32_t FAT32_open_item(char *name,struct file_rsc *parent,
                                    struct file_rsc *result,enum file_open_mode mode);
bool_t FAT32_creat_item(sint8_t *name,struct file_rsc *parent,union file_attrs attr);
bool_t FAT32_close_item(struct file_rsc *fp);
struct file_rsc *FAT32_item_traversal_son(struct file_rsc *ancestor_file,
                                        	struct file_rsc *current_file);
sint64_t FAT32_query_file_stocks(struct file_rsc *fp);
uint32_t FAT32_flush_file(struct file_rsc *fp);
void FAT32_check_DBX(struct st_DBX_device_tag *DBX_device_tag,
                     sint64_t *sum_size,sint64_t *valid_size,sint64_t *free_size);
sint64_t FAT32_set_file_size(struct file_rsc *fp,sint64_t new_size);
void __FAT32_fill_DBX_device_tag(struct st_DBX_device_tag *DBX_device_tag,
                                 char *name);
void __djyos_load_FAT32(void);            
ptu32_t module_init_FAT32(ptu32_t para); //�ļ�ϵͳ��ʼ��                       
#endif

