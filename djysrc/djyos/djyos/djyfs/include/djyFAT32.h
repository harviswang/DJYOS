#ifndef __ZNFAT_H__
#define __ZNFAT_H__

//#include "mytype.h" //类型重定义
//#include "stm32f10x_lib.h"
#include "os_inc.h"

// 用于计算簇的开始扇区
#define SOC(c) (((c-pArg->FirstDirClust)*(pArg->SectorsPerClust))+pArg->FirstDirSector) 

#define DEVS 1 //这里表明存储设备的个数，这里只有1个存储设备，就是SD卡
struct FAT32_init_arg *pArg;

 //设备表
#define SDCARD 0 //SD卡
#define UDISK  1 //U盘
#define CFCARD 2 //CF卡
#define OTHER  3 //其它
                                         //这里的存储设备表，可以灵活扩充，以实现对更多存储设备的支持
                        //-------------------------------------------
                        
#define MAKE_FILE_TIME(h,m,s)    ((((unsigned int)h)<<11)+(((unsigned int)m)<<5)+(((unsigned int)s)>>1))    /* 生成指定时分秒的文件时间数据 */
#define MAKE_FILE_DATE(y,m,d)    (((((unsigned int)y)+20)<<9)+(((unsigned int)m)<<5)+((unsigned int)d))    /* 生成指定年月日的文件日期数据 */
                        
//DPT:分区记录结构如下
struct part_record
{
uint8_t active;         //0x80表示此分区有效
uint8_t start_head;      //分区的开始磁头
uint8_t start_cyl_sect[2];//开始柱面与扇区
uint8_t part_type;       //分区类型
uint8_t end_head;        //分区的结束头
uint8_t end_cyl_sect[2];  //结束柱面与扇区
uint8_t start_LBA[4];    //分区的第一个扇区
uint8_t size[4];        //分区的大小 
};
                        
//MBR:分区扇区（绝对0扇区）定义如下
struct part_sector
{
uint8_t PartCode[446]; //MBR的引导程序
struct part_record Part[4];   // 4个分区记录
uint8_t BootSectSig0;  //55
uint8_t BootSectSig1;  //AA
};

/* FDT文件属性 */
#define cn_FAT_ATTR_READ_ONLY              0x01
#define cn_FAT_ATTR_HIDDEN                 0x02
#define cn_FAT_ATTR_SYSTEM                 0x04
#define cn_FAT_ATTR_VOLUME_ID              0x08
#define cn_FAT_ATTR_DIRECTORY              0x10
#define cn_FAT_ATTR_ARCHIVE                0x20

//FAT32中对BPB的定义如下  一共占用90个字节
struct FAT32_BPB
{
uint8_t BS_jmpBoot[3];     //跳转指令            offset: 0
uint8_t BS_OEMName[8];     //                    offset: 3
uint8_t BPB_BytesPerSec[2];//每扇区字节数        offset:11
uint8_t BPB_SecPerClus[1]; //每簇扇区数          offset:13
uint8_t BPB_RsvdSecCnt[2]; //保留扇区数目        offset:14
uint8_t BPB_NumFATs[1];    //此卷中FAT表数       offset:16
uint8_t BPB_RootEntCnt[2]; //FAT32为0            offset:17
uint8_t BPB_TotSec16[2];   //FAT32为0            offset:19
uint8_t BPB_Media[1];      //存储介质            offset:21
uint8_t BPB_FATSz16[2];    //FAT32为0            offset:22
uint8_t BPB_SecPerTrk[2];  //磁道扇区数          offset:24
uint8_t BPB_NumHeads[2];   //磁头数              offset:26
uint8_t BPB_HiddSec[4];    //FAT区前隐扇区数     offset:28
uint8_t BPB_TotSec32[4];   //该卷总扇区数        offset:32
uint8_t BPB_FATSz32[4];    //一个FAT表扇区数     offset:36
uint8_t BPB_ExtFlags[2];   //FAT32特有           offset:40
uint8_t BPB_FSVer[2];      //FAT32特有           offset:42
uint8_t BPB_RootClus[4];   //根目录簇号          offset:44
uint8_t fs_info[2];         //保留扇区FSINFO扇区数offset:48
uint8_t BPB_BkBootSec[2];  //通常为6             offset:50
uint8_t BPB_Reserved[12];  //扩展用              offset:52
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
//存储卡上面的目录结构体,即目录项                       
struct hardware_item_info 
{
uint8_t deName[8];       // 文件名，不足部分以空格补充
uint8_t deExtension[3];  // 扩展名，不足部分以空格补充
uint8_t deAttributes;    // 文件属性
uint8_t deLowerCase;     // 0
uint8_t deCHundredth;    // 世纪
uint8_t deCTime[2];      // 创建时间
uint8_t deCDate[2];      // 创建日期
uint8_t deADate[2];      // 访问日期
uint8_t deHighClust[2];  // 开始簇的高字
uint8_t deMTime[2];      // 最近的修改时间
uint8_t deMDate[2];      // 最近的修改日期
uint8_t deLowCluster[2];     // 开始簇的低字
uint8_t deFileSize[4];      // 文件大小 
};
                        
//FAT32初始化时初始参数装入如下结构体中
struct FAT32_init_arg
{
uint8_t DEV_No;
                        
uint8_t BPB_Sector_No;   //BPB所在扇区号
uint32_t Total_Size;      //磁盘的总容量
uint32_t FirstDirClust;   //根目录的开始簇
uint32_t BytesPerSector;    //每个扇区的字节数
uint32_t FATsectors;      //FAT表所占扇区数
uint32_t SectorsPerClust;    //每簇的扇区数
uint32_t FirstFATSector;  //第一个FAT表所在扇区
uint32_t FirstDirSector;  //第一个目录所在扇区
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
//FAT32,文件资源结构体
struct FAT32_file_rsc
{
uint8_t  FileName[12];       //文件名
uint32_t  FileStartCluster;   //文件的开始簇
uint32_t  FileCurCluster;//文件的当前簇
uint32_t  FileSize;      
uint32_t  FileCurSector;     //文件的当前扇区
uint16_t  FileCurPos;         //文件在当前扇区中的位置
uint32_t  FileCurOffset;     //文件的当前偏移量
uint32_t  Rec_Sec;            //文件的文件/目录项所在的扇区
uint16_t  nRec;               //文件的文件/目录项所在扇区中的位置
                        
uint8_t  FileAttr;       //文件属性              
struct Time    FileCreateTime;     //文件的创建时间
struct Date    FileCreateDate;     //文件的创建日期
struct Time    FileMTime;          //文件的修改时间
struct Date    FileMDate;          //文件的修改日期
struct Date    FileADate;          //文件的访问日期
};
//FAT32的文件系统信息结构
//就是FSINFO信息扇区上的内容
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
uint8_t __read_sector(uint32_t addr,uint8_t *buf); //znFAT的与底层存储设备的驱动接口，读取扇区
uint8_t __write_sector(uint32_t addr,uint8_t *buf);//znFAT的与底层存储设备的驱动接口，写入扇区
uint32_t __little_to_big(uint8_t *dat,uint8_t len); //小端转大端
sint8_t __lowercase_convert_capital(sint8_t c);
uint8_t __compare_dir_name(sint8_t *a,sint8_t *b);                       
uint16_t __find_DBR(void); //定位DBR
uint32_t __get_total_size(void); //获取总容量                        
uint32_t __search_last_usable_cluster(void); //从获取FSInfo中的下一个可用空闲簇
sint64_t FAT32_query_file_cubage(void); //获取剩余容量                      
void __update_fsinfo_free_clu(uint32_t PlusOrMinus); //更新FSInfo的空闲簇的数量
void __update_fsinfo_last_clu(uint32_t Last_Clu); //更新FSInfo的下一个可用空闲簇                        
uint32_t __get_next_cluster(uint32_t LastCluster); //获取下一个簇
void __to_file_name(char *dName,char *eName,char *pName);
void __str_to_up(char *str);
uint8_t __seek_file(struct FAT32_file_rsc *FAT32_fp,uint32_t offset); //文件定位 
uint32_t __find_free_clust(unsigned char flag);   //寻找下一个可用的空闲簇
void __fill_rec_inf(struct hardware_item_info *prec,sint8_t *name,uint8_t is_dir,uint8_t *ptd);
void __modify_FAT32(uint32_t cluster,uint32_t dat);
void __empty_cluster(uint32_t Cluster);
sint8_t __create_rec(struct FAT32_file_rsc *FAT32_fp,uint32_t cluster,sint8_t *name,uint8_t is_dir,uint8_t *ptd); //构建文件/目录项
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
ptu32_t module_init_FAT32(ptu32_t para); //文件系统初始化                       
#endif

