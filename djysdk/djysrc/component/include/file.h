//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
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
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "errno.h"
#include "rsc.h"
#include "ring.h"
#ifdef __cplusplus
extern "C" {
#endif


#define F_OK        0   // does file exist
#define X_OK        1   // is it executable by caller
#define W_OK        2   // is it writable by caller
#define R_OK        4   // is it readable by caller
#define S_IREAD     0x0000400 // read permission, owner
#define S_IWRITE    0x0000200 // write permission, owner
#define S_IEXEC     0x0000100 // execute/search permission, owner

//��ô���ĵ����ļ�����Ƕ��ʽϵͳ���޶�����壬Ϊ���ݴ�������ļ���ϵͳ��
#define     CN_FILE_NAME_LIMIT    255
#define     CN_PTT_NAME_LIMIT     255      //�������൱���̷������ֳ���
//��Ҳ�����޶��ļ�ϵͳ��·����ȣ�ÿ�취���������·���ᵼ�����޳���ִ��ʱ�䣬
//��Ƕ��ʽϵͳ��Ӧ����Ҫ����ģ�20��ҲӦ�ù����˰�!
#define     CN_PATH_DEPTH_LIMIT   20

//���豸���������Ĳ����������ޣ��������¼����ṹ�����ڴ��ݲ���
 struct tagRwPara    //���ݶ�д����
{
    size_t  size;       //��д��ÿ��Ԫ�ߴ�
    size_t nmemb;       //��Ԫ��
    struct tagFileRsc  *fp;       //��д���Ŀ���ļ���
};
 struct tagSeekPara         //����seekƫ�����Ĳ���
{
    sint64_t offset;    //seek��ƫ����
    u8  whence;    //seek�Ĳο�λ�� SEEK_CUR��SEEK_END��SEEK_SET
};

enum _FILE_OPEN_MODE_                         //seek
{                   //���� ���� ��� seek     ��β дλ��   ��λ��  ��ʼ seek���
    EN_R_RB,      //ֻ�� ���� ���� ��λ��   ��Ч ��Ч     seekλ��   0   ��Ч
    EN_W_WB,      //ֻд ���� ��� дλ��   ���� seekλ�� ��Ч       0   ����
    EN_A_AB,      //׷�� ���� ���� ��Ч     ��Ч β��     ��Ч     β��  ��Ч
    EN_R_RB_PLUS, //��д ���� ���� ��дλ�� ���� seekλ�� seekλ��   0   ����
    EN_W_WB_PLUS, //��д ���� ��� ��дλ�� ���� seekλ�� seekλ��   0   ����
    EN_A_AB_PLUS, //��д ���� ���� ��λ��   ���� β��     seekλ��   0   ��Ч
} ;

//�ļ�ϵͳ������룬��enum������cn_fs_error��ʼ������1.
enum _FS_ERROR_CODE_
{
    EN_FS_NO_ERROR = CN_FS_NO_ERROR,    //û�д���
    EN_FS_NAME_INVALID_CHAR,            //���ֺ��зǷ��ַ�
    EN_FS_NAME_LEN_ERROR,               //���ֳ��ȴ���
    EN_FS_OBJECT_OPENED,                //�����Ѿ���
    EN_FS_OBJECT_NONENTITY,             //���󲻴���
    EN_FS_OBJECT_READONLY,              //ֻ������
    EN_FS_FOLDER_UNBLANK,               //Ŀ¼�ǿ�
    EN_FS_CREAT_PTT_ERROR,              //������������
    EN_FS_CREAT_ROOT_FOLDER_ERROR,      //������Ŀ¼����
    EN_FS_FORMAT_PTT_ERROR,             //��ʽ������
    EN_FS_REMOVE_ERROR,                 //ɾ���������
    EN_FS_PTT_ERROR,                    //ɾ����������
    EN_FS_RENAME_ERROR,                 //�����������
    EN_FS_PTT_NAME_REPEAT,              //������������
    EN_FS_PTT_NO_FORMATED               //����δ��ʽ��
};

#define CN_FS_OPEN_SUCCESS  0       //�ɹ����ļ�(Ŀ¼)
#define CN_FS_ITEM_EXIST    1       //�ļ�(Ŀ¼)���ڵ����ܴ�(һ����ģʽ������)
#define CN_FS_ITEM_INEXIST  2       //�ļ�(Ŀ¼)������

struct tagFileRsc;
union file_attrs;
//�����豸��˽�����ݽṹ
//ɾ����file_sum��folder_sum��Ա����Ϊ�ó�Ա��Ҫɨ������FDT����ܻ�ã�ɨ���
//ʱ����ܻ�ܳ��Ҳ�ȷ����������Ƕ��ʽӦ�á�
struct tagPTTDevice
{
    struct tagRscNode PTT_node;      //������Դ��㣬��ϵͳά��
    u32    opened_sum;         //�򿪵��ļ�(Ŀ¼)����
    bool_t      formatted;          //������ʽ����־
    char        name[CN_PTT_NAME_LIMIT+1];    //���������൱��dos�ľ��
    struct tagFileRsc *opened_root;   //�򿪵ĸ�Ŀ¼�����=������
    ptu32_t PTT_medium_tag;     //�ض��������������ݽṹָ�룬�ɴ洢ģ��ʹ��
    struct tagMutexLCB *chip_mutex_tag;   //���������Ļ�����ָ��

    //�������ܺ���
    //��ʽ����������PTT_device_tag�����з��ظ�ʽ��������Ĳ������ɹ���true��
    //����false
    bool_t (*format)(u32 fmt_para1,u32 fmt_para2,
                            struct tagPTTDevice *PTT_device_tag);

    //�����ļ���д��������Щ������left_read left_write����ʹ��
    //д�ļ�����buf�г���Ϊlen������д�뵽file�ĵ�ǰָ�봦
    u32 (*write)(struct tagFileRsc *fp,u8 *buf,u32 len);
    //���ļ�����file��ǰָ�봦��ȡlen���ȵ����ݵ�buf��
    u32 (*read)(struct tagFileRsc *fp,u8 *buf,u32 len);
    //��д������������д��flash
    u32 (*flush)(struct tagFileRsc *fp);
    //��ѯ�ж��ٿɶ����ݣ�����ʵ���ļ���˵������(�ļ�����-��ǰָ��)�������ı�׼
    //���岢��һ����ˣ�����ͨ�Ŷ��ӡ�
    sint64_t (*query_file_stocks)(struct tagFileRsc *fp);
    //��ѯĿ���ļ�������д��������ݣ�һ����ڷ�����ʣ��ռ䣬�����ı�׼
    //���岢��һ����ˡ�sum_size,valid_size,free_size����ָ�붼�����ǿ�ָ�룬
    //���ǿ�ָ�룬���Ӧ�Ĳ��������ء�
    sint64_t (*query_file_cubage)(struct tagFileRsc *fp);
    void (*check_PTT)(struct tagPTTDevice *PTT_device_tag,
                     sint64_t *sum_size,sint64_t *valid_size,sint64_t *free_size);
    //������Ŀ¼(�ļ�)��������,��Щ������left_ctrl��������
    //�����ļ����ȣ�����ʵ�ʳ��Ƚ���β�����ڵ�ǰ���Ƚ�׷�ӿռ䡣
    sint64_t (*set_file_size)(struct tagFileRsc *fp,sint64_t new_len);
    //�����ļ�ָ��,0=�ɹ�����0=ʧ�ܣ������λ�ó����ļ��������ļ���д����������
    //���ļ��ߴ粢�����µĴ洢�ռ�
    u32 (*seek_file)(struct tagFileRsc *fp,struct tagSeekPara *pos);
    //�����ļ�(Ŀ¼)��attr��������������Ŀ¼�����ļ���
    bool_t (*create_item)(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result,union file_attrs attr,
                         enum _FILE_OPEN_MODE_ mode);
    //debug bool_t (*move_file)(struct tagFileRsc *src_fp,struct tagFileRsc *dest_fp);
    //ɾ��һ���ļ�(Ŀ¼)��ֻ��ɾ����Ŀ¼
    bool_t (*remove_item)(struct tagFileRsc *fp);
    //���ļ�(Ŀ¼)��result����item��Ϣ���������ؽ��:
    //CN_FS_OPEN_SUCCESS,�ɹ����ļ�(Ŀ¼)
    //CN_FS_ITEM_EXIST,�ļ�(Ŀ¼)���ڵ����ܴ�(һ����ģʽ������)
    //CN_FS_ITEM_INEXIST,�ļ�(Ŀ¼)������
    u32 (*open_item)(char *name,struct tagFileRsc *parent,
                        struct tagFileRsc *result,enum _FILE_OPEN_MODE_ mode);
    //�ر�Ŀ¼(�ļ�)��true = �ɹ���false = ʧ��(�����������Դ��һ�㲻��ʧ��)
    bool_t (*close_item)(struct tagFileRsc *fp);
    //�����ļ�(Ŀ¼)��result�ǿ��򷵻�item��Ϣ�����������д����֧��ͨ���
    //���������ɹ��ҵ�item����true�����򷵻�false
    bool_t (*lookfor_item)(char *name,struct tagFileRsc *parent,
                           struct tagFileRsc *result);
    bool_t (*rename_item)(struct tagFileRsc *fp,char *newname);
    struct tagFileRsc *(*item_traversal_son)(struct tagFileRsc * parent,
                                    struct tagFileRsc * current_file);
    //Ŀ¼���������
    //��ѯĿ¼��ߴ�
    u32 (*check_fdt_size)(struct tagPTTDevice *PTT_device_tag);
    //��Ŀ¼��,��������ΪһЩר�ù���׼���ģ�һ�㲻Ҫ��
    void (*read_fdt)(struct tagPTTDevice *PTT_device_tag,u8 *buf);
    //����ļ�������Ŀ¼���ļ�����������������Ŀ¼�µ��ļ���
    u32 (*check_folder)(struct tagFileRsc *folder);
};
enum _FS_LEFT_CMD_
{
    enum_fs_work_path,    //���ò��򿪵�ǰ����·��
};
enum _FS_RIGHT_CMD_
{
    enum_fs_add_PTT,  //����һ������
    enum_fs_del_PTT,  //ɾ��һ������
};
enum _PTT_LEFT_CMD_
{
    EN_PTT_FORMAT,        //��ʽ������
    EN_CHK_PTT_FORMAT,    //��ѯ�����Ƿ��ʽ��
    EN_PTT_LOOKFOR,       //����һ����Ŀ���ļ���Ŀ¼���Ƿ���ڣ�ֻ����һ��������
    EN_PTT_OPEN,          //��һ���ļ���ֻ�����ļ�
    EN_PTT_REMOVE,        //ɾ��һ���ļ����Ŀ¼�����ṩɾ��һ�����Ĺ���
    EN_PTT_RENAME,        //�ļ���Ŀ¼������
    EN_PTT_SEEK,          //����һ���򿪵��ļ��Ķ�дλ��
    EN_PTT_TRUNCATE,      //�����ļ����ȣ����ܵ��½ض̻��ӳ��ļ�
    EN_PTT_CLOSE,         //�ر�һ���ļ�
    EN_PTT_CREAT,        //����һ���ļ�,ansi c�ﲢû�ж�Ӧ��creat������
    EN_PTT_FLUSH,         //�ѻ�����ˢ�������豸��ȥ
    EN_PTT_TRAVERSAL_SON, //������Ŀ¼
    EN_QUERY_FILE_CUBAGE, //��ѯһ���ļ�����д���������
};
enum _PTT_RIGHT_CMD_
{
    CN_PTT_CLR_READBUF,     //���ĳ�ļ���������
    CN_PTT_CLR_WRITEBUF,    //���ĳ�ļ�д������
    CN_PTT_FLUSH_ONE,       //��ĳ�ļ�������д��洢�豸
    CN_PTT_FLUSH_ALL,       //�Ѹ÷��������д��ļ�������д��洢�豸
    CN_PTT_SEARCH_MEDIUM_TAG,  //�޸��ļ���ý����
    CN_PTT_UPDATE_MEDIUM_TAG,  //�޸��ļ���ý����
};

struct tagFileAttrBits
{
    u8 read_only:1;    //ֻ���ļ�
    u8 hidden:1;       //�����ļ�
    u8 reserve:2;
    u8 folder:1;       //Ŀ¼
    u8 archive:1;      //�����ļ�
    u8 close_delete:1; //1=�ر�ʱɾ����������ʱ�ļ�
    u8 deleted:1;      //�ļ���ɾ��
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
//
//  ����Ϊdjyfs��չ��־
//dc �����ļ������ڹر�ʱɾ�����ر��ʺ���ʱ�ļ���


union file_attrs
{
    u8 all;
    struct tagFileAttrBits bits;
};

struct tagDjyfsStat
{
    s64 st_atime;           //������ʱ�䣬��ʱ����(2012-08-23)
    s64 st_ctime;           //�ļ�����ʱ��
    s64 st_mtime;           //����޸�ʱ��
    union file_attrs  st_mode;    //�ļ�ģʽ��bit mask
    s64 st_size;            //�ļ��ߴ�
};

struct tagFileRsc
{
    struct tagRscNode file_node;      //�򿪵��ļ���Դ��㣬��ϵͳά��
    u32 open_counter;               //�ļ��򿪴�������,��������file.cģ��ά��
    struct tagPTTDevice *home_PTT;          //���ļ������ķ�������file.cά��
    enum _FILE_OPEN_MODE_ open_mode;  //�ļ���ģʽ����file.cά��
    union file_attrs file_attr;     //�ļ����ԣ�file.c������ģ�鹲ͬά��
    struct tagSemaphoreLCB *file_semp;//�������ã�������ά��
    sint32_t CreateTime; //����ʱ��
    sint32_t ModTime; //������޸�ʱ��
    ptu32_t file_medium_tag;        //��ý����صı�ǣ��京�����ض��ļ�ϵͳ
                                    //driver���ͣ���flash file driver)�У�����
                                    //���ļ�(Ŀ¼)��FDT��Ŀ�š�
    //�������ֻ���������������ģ��ʹ�ã�����ģ��Ҳ��ѡ��ʹ�û���������ʹ�õ�
    //������ָ����NULL����
    struct tagRingBuf read_rw_buf;    //������д�ļ��Ķ�����������ϲ���д�ļ��Ķ�
                                    //д���������ϲ���д�ģ���д������write_buf
    struct tagRingBuf write_buf;      //�ļ�д������
    struct tagRingBuf *p_read_buf;    //��������ָ��
    struct  tagRingBuf *p_write_buf;   //д������ָ��
    sint64_t file_size;             //�ļ��ߴ�,�ֽ����������ڻ����������ݣ�������
                                    //ģ��ά��
    sint64_t read_ptr;              //��ǰ��ָ�룬������ά����ʹ�á�
    sint64_t write_ptr;             //��ǰдָ�룬������ά����ʹ�á�
    u32 eno;           //���һ�γ������ͣ�EN_FS_NO_ERROR��ʾ����û�г���
    char     name[CN_FILE_NAME_LIMIT+1]; //�ļ�����Ŀ¼����,������֧�ֵ��ļ�����
                                    //��������ģ������ģ���������ģ��ά����
};

struct stat
{
    union file_attrs st_mode;       //�ļ�����
    enum _FILE_OPEN_MODE_ open_mode;  //�ļ���ģʽ
    sint32_t CreateTime; //����ʱ��
    sint32_t ModTime; //������޸�ʱ��
    sint64_t file_size;             //�ļ��ߴ�,�ֽ������������ڻ�����������
};

ptu32_t ModuleInstall_Djyfs(ptu32_t para);
u32 Djyfs_SetWorkPath(char *path);
bool_t Djyfs_GetCwd(char *user_work_path,u32 namebuf_len);
bool_t Djyfs_GetPTT_Name(char *PTT_name,u32 namebuf_len);
ptu32_t Djyfs_AppCtrl(u32 left_cmd,
                                ptu32_t data1,ptu32_t data2);
ptu32_t Djyfs_FsDriverCtrl(u32 right_cmd,
                                ptu32_t data1,ptu32_t data2);
bool_t Djyfs_Fstat(struct tagFileRsc *fp,struct stat *fp_info);
struct tagFileRsc *Djyfs_GetRootFolder(struct tagFileRsc *fp);
bool_t Djyfs_PTT_Stat(const char *fullname,s64 *sum,s64 *valid,s64 *free);
bool_t Djyfs_PTT_StatFp(struct tagFileRsc * fp,s64 *sum,s64 *valid,s64 *free);
u32  Djyfs_GetPathLen(struct tagFileRsc * fp);
bool_t Djyfs_GetPath(struct tagFileRsc * fp,char *buf,u32 maxlen);
u32  Djyfs_GetCwdLen(void);
bool_t Djyfs_GetCwd(char *buf,u32 maxlen);
FILE *Djyfs_FolderTraversalSon(struct tagFileRsc *ancestor_file,
                                            struct tagFileRsc *current_file);
u32 Djyfs_Ftruncate ( FILE *fp, sint64_t offset);
u32 Djyfs_Format(u32 format_para1,u32 format_para2,char *dbx_name);
//bool_t djyfs_move(char *src,char *dest);
size_t Djyfs_Fcubage(FILE *fp);
u32 fseekL(FILE *fp, s64 offset, s32 whence);


#ifdef __cplusplus
}
#endif

#endif // _FILE_H_

