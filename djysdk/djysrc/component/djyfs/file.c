//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

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
#include "config-prj.h"
#include "ctype.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "rsc.h"
#include "pool.h"
#include "lock.h"
#include "systime.h"
#include "djyos.h"
#include "file.h"
#include "shell.h"
#include "shell_fs.h"
//ansi c standard ֧�ֵ��ļ������������

//�� The wide-character input functions �� those functions described in 7.24 that perform
//input into wide characters and wide strings: fgetwc, fgetws, getwc, getwchar,
//fwscanf, wscanf, vfwscanf, and vwscanf.

//�� The wide-character output functions �� those functions described in 7.24 that
//perform output from wide characters and wide strings: fputwc, fputws, putwc,
//putwchar, fwprintf, wprintf, vfwprintf, and vwprintf.

//�� The wide-character input/output functions �� the union of the ungetwc function,
//the wide-character input functions, and the wide-character output functions.

//�� The byte input/output functions �� those functions described in this subclause that
//perform input/output: fgetc, fgets, fprintf, fputc, fputs, fread,
//fscanf, fwrite, getc, getchar, gets, printf, putc, putchar, puts,
//scanf, ungetc, vfprintf, vfscanf, vprintf, and vscanf.

//Operations on files���ļ���������
//int remove(const char *filename);
//int rename(const char *old, const char *new);
//FILE *tmpfile(void);
//char *tmpnam(char *s);
//
//File access functions���ļ����ʺ���
//int fclose(FILE *stream);
//int fflush(FILE *stream);
//FILE *fopen(const char * filename,const char * mode);
//FILE *freopen(const char * filename,const char * mode,FILE * restrict stream);
//void setbuf(FILE * restrict stream,char * restrict buf);
//int setvbuf(FILE * restrict stream,char * restrict buf,int mode, size_t size);
//
//Formatted input/output functions����ʽ�������������
//int fprintf(FILE * restrict stream,const char * restrict format, ...);
//int fscanf(FILE * restrict stream,const char * restrict format, ...);
//int printf(const char * restrict format, ...);
//int scanf(const char * restrict format, ...);
//int snprintf(char * restrict s, size_t n,const char * restrict format, ...);
//int sprintf(char * restrict s,const char * restrict format, ...);
//int sscanf(const char * restrict s,const char * restrict format, ...);
//int vfprintf(FILE * restrict stream,const char * restrict format,va_list arg);#include <stdarg.h>
//int vfscanf(FILE * restrict stream,const char * restrict format,va_list arg);
//int vprintf(const char * restrict format,va_list arg);
//int vscanf(const char * restrict format,va_list arg);
//int vsnprintf(char * restrict s, size_t n,const char * restrict format,va_list arg);
//int vsprintf(char * restrict s,const char * restrict format,va_list arg);
//int vsscanf(const char * restrict s,const char * restrict format,va_list arg);
//
//Character input/output functions���ַ������������
//int fgetc(FILE *stream);
//char *fgets(char * restrict s, int n,FILE * restrict stream);
//int fputc(int c, FILE *stream);
//int fputs(const char * restrict s,FILE * restrict stream);
//int getc(FILE *stream);
//int getchar(void);
//char *gets(char *s);
//int putc(int c, FILE *stream);
//int putchar(int c);
//int puts(const char *s);
//int ungetc(int c, FILE *stream);
//
//Direct input/output functions��ֱ�������������
//size_t fread(void * restrict ptr,size_t size, size_t nmemb,FILE * restrict stream);
//size_t fwrite(const void * restrict ptr,size_t size, size_t nmemb,FILE * restrict stream);
//
//File positioning functions���ļ���дλ�ú���
//int fgetpos(FILE * restrict stream,fpos_t * restrict pos);
//int fseek(FILE *stream, long int offset, int whence);
//int fsetpos(FILE *stream, const fpos_t *pos);
//long int ftell(FILE *stream);
//void rewind(FILE *stream);
//
//Error-handling functions����������
//void clearerr(FILE *stream);
//int feof(FILE *stream);
//int ferror(FILE *stream);
//void perror(const char *s);

//�ļ�(Ŀ¼)���������淶:
//��"a:\aaa\bbb\ccc"�У�
// 1."a"������ΪPTT_name
// 2."aaa"��"bbb"Ϊspathname
// 3."ccc"������Ϊfilename
//4."aaa"��"bbb"��"ccc"ͳ��Ϊword
//5.���������������ʱ������Ϊfullname
//6."a:\aaa\bbb\"������Ϊfullpath
//7."\aaa\bbb\"������Ϊabspath
//8."aaa\bbb\"��Ϊrelativepath
//9."\aaa\bbb\ccc"����Ϊsynname
//10."aaa\bbb\ccc"����Ϊrelativename(����ڵ�ǰ·��)

static struct tagFileRsc *s_ptContentBuf;//����Ŀ¼���ڴ��
static struct tagMemCellPool *s_ptContentPool;              //�ڴ��ͷָ�롣
//�趨һ������·��,�൱��windows/dos�ĵ�ǰ·��
struct tagFileRsc *g_ptWorkPath = NULL;    //����·��ָ��
static struct tagRscNode s_tFileSystemRoot; //�򿪵��ļ���Դ�ĸ��ڵ㡣
struct tagSemaphoreLCB *Fs_Semaphore; //����fs�����õ��ź���

bool_t __Djyfs_PickWord(const char *synname,u32 curent_offset,char *name);
bool_t __Djyfs_PickPathWord(const char *synname,u32 curent_offset,char *name);
bool_t __Djyfs_PickFileNameWord(const char *synname,char *name);
bool_t __Djyfs_IsAbsPath(const char *fullname);

struct tagShellCmdTab const ShellFsCmdTab[] =
{
    {
        "rmdir",
        ShFs_RD,
        "ɾ��һ��Ŀ¼",
        NULL
    },
    {
        "rd",
        ShFs_RD,
        "ɾ��һ��Ŀ¼",
        NULL
    },
    {
        "dir",
        ShFs_Dir,
        "��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�",
        "dir [������]·��  [������]·��... ...\r\n\
            ��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�\r\n\
dir [������][����ͨ���·��][������][����ͨ���·��]... ...\r\n\
            ��ʾָ��Ŀ¼��ƥ����ļ�����Ŀ¼�б�\r\n\
dir [\\][\\]... ... ��ʾ��Ŀ¼��һ�����ļ�����Ŀ¼�б�\r\n\
dir [..][..]... ... ��ʾ��ǰĿ¼��һ�����ļ�����Ŀ¼�б�\r\n\
dir [](���ַ�)      ��ʾ��ǰĿ¼�µ��ļ�����Ŀ¼�б�"
    },
    {
        "rename",
        ShFs_Ren,
        "������һ���ļ�",
        "Ren/rename [����:][·��]�ļ���1 �ļ���2"
    },
    {
        "ren",
        ShFs_Ren,
        "������һ���ļ�",
        "Ren/rename [����:][·��]�ļ���1 �ļ���2"
    },
    {
        "del",
        ShFs_Del,
        "ɾ������һ���ļ�",
        NULL
    },
    {
        "copy",
        ShFs_Copy,
        "����һ���ļ�",
        NULL
    },
    {
        "move",
        ShFs_Move,
        "��һ�������ļ��ƶ���ָ��Ŀ¼��",
        NULL
    },
    {
        "chdir",
        ShFs_CD,
        "��ʾ��ǰĿ¼�����ƻ������",
        NULL
    },
    {
        "cd",
        ShFs_CD,
        "��ʾ��ǰĿ¼�����ƻ������",
        "PTT_name[:]         ����ָ����������Ŀ¼\r\n\
                   ���磺D�� ����D������Ŀ¼\r\n\
cd/chdir[������]·�� ��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼\r\n\
                   ����: 1)cd C:\\folder ����folderĿ¼\r\n\
                          2)cd folder ����folderĿ¼\r\n\
cd/chdir[](���ַ�) ��ʾ��ǰĿ¼����\r\n\
cd/chdir[\\]       ��������Ŀ¼\r\n\
cd/chdir[..]       ��������һ��Ŀ¼\r\n\
                   ����: 1)cd ..\\.. ������������Ŀ¼ \r\n\
                         2)cd ..\\folder ������һ��Ŀ¼�к���folder��Ŀ¼"
    },
    {
        "mkdir",
        ShFs_MD,
        "����һ��Ŀ¼",
        NULL
    },
    {
        "md",
        ShFs_MD,
        "����һ��Ŀ¼",
        NULL
    },
    {
        "create-ptt",
        ShFs_CreatePTT,
        "��������",
        "�����ʽ: create-ptt ������С ������"
    },
    {
        "test-w",
        ShFs_TestWrite,
        "����д�ļ�",
        NULL
    },
    {
        "test-r",
        ShFs_TestRead,
        "���Զ��ļ�",
        NULL
    },
   {
        "format",
        ShFs_Format,
        "��ʽ���ļ�ϵͳ",
        NULL
    },
    {
        "chkptt",
        ShFs_Chkptt,
        "��ʾ�����ռ�Ĳ���",
        NULL
    },
};

static struct tagShellCmdRsc tg_FileCmdRsc
                        [sizeof(ShellFsCmdTab)/sizeof(struct tagShellCmdTab)];
void Sh_PrintWorkPathSet(void);

ptu32_t ModuleInstall_Djyfs(ptu32_t para)
{
    //����Դ�����н���һ�������fs�����з���������Ķ�Լ��򿪵��ļ���Ŀ¼�������ڴ˽���¡�
    //��ʼ������ʱ��ÿ��������ӵķ���Ϊ�ýڵ���ӽڵ㣬�÷����ĸ�Ŀ¼���Ϊ���������ӽ�㡣
    Fs_Semaphore = Lock_SempCreate(1,1,CN_SEMP_BLOCK_FIFO,"fs tree");
    if(Fs_Semaphore == NULL)
        return 0;  //fs���ź�������ʧ��
    s_ptContentBuf = M_Malloc(gc_u32CfgOpenedfileLimit * sizeof(struct tagFileRsc),0);
    if(s_ptContentBuf == NULL)
    {
        Lock_SempDelete(Fs_Semaphore);
        return 0;
    }
    Rsc_AddTree(&s_tFileSystemRoot,sizeof(struct  tagRscNode),RSC_RSCNODE,"fs");
    //����Ŀ¼���ڴ�أ�ÿ���򿪵��ļ���һ�ÿ��Ŀ¼Ҳ��һ�����Ƕ༶Ŀ¼��
    //��ÿ���������㣬����򿪵Ķ���ļ���·�����غϣ��غϲ��ֲ�����ռ��Ŀ¼�
    s_ptContentPool = Mb_CreatePool(s_ptContentBuf,
                                gc_u32CfgOpenedfileLimit,
                                sizeof(struct tagFileRsc),0,0,
                                "�򿪵�Ŀ¼���ڴ��");
    if(s_ptContentPool == NULL)
    {
        Lock_SempDelete(Fs_Semaphore);
        free(s_ptContentBuf);
        return 0;
    }
    if(Sh_InstallCmd(ShellFsCmdTab,tg_FileCmdRsc,
            sizeof(ShellFsCmdTab)/sizeof(struct tagShellCmdTab)))
    {
        Sh_PrintWorkPathSet();
        return 1;
    }
    return 0;
}

//----���õ�ǰ����·��---------------------------------------------------
//����: ��ǰ����·����Ĭ������£�����Ϊ��һ��
//      ������һ����Ŀ¼��cfg_work_path����cofig-prj.h������
//����: path���µĹ���·��
//����: 1�����óɹ�
//      0, ����ʧ��
//-------------------------------------------------------------------------
u32 Djyfs_SetWorkPath(char *path)
{
    struct tagPTTDevice *PTT_device_tag;

    if(path != NULL)
    {
        if(strcmp(path,"")!=0)
        {
            if( Djyfs_AppCtrl(enum_fs_work_path,
                                    (ptu32_t)path,0)
                                    == 0)
            {
                printf("�û����ù���·��ʧ��\r\n");
                return 0;
            }
            else
                return 1;
        }
    }
    //ָ���һ�������ĸ�Ŀ¼
    PTT_device_tag = (struct tagPTTDevice*)Rsc_GetSon(&s_tFileSystemRoot);
    if(PTT_device_tag->opened_root == NULL)
    {
        return 0;
    }
    else
    {
        g_ptWorkPath = PTT_device_tag->opened_root;
        return 1;
    }
}

//----��ȡ��ǰ������-----------------------------------------------------------
//����: ��ȡ��ǰ����Ŀ¼���ڵķ�����(��Ӧwindows��dos���̷�)
//����: PTT_name�������ַ����Ļ�����
//      namebuf_len,���ֻ������ĳ���
//����: true����ȡ�ɹ�
//      false,��ȡʧ��
//------------
//���¼�¼:
// 1.����: 2015/3/25
//   ˵��: �޸Ĵ�ӡ��Ϣ
//   ����: ������
//-------------------------------------------------------------------------
bool_t Djyfs_GetPTT_Name(char *PTT_name,u32 namebuf_len)
{
    struct tagRscNode *parent,*fs_tree;

    if(g_ptWorkPath == NULL)// ˵��·��δ��ʼ�����߷���δ����
    {
        printf("δָ����ǰ����Ŀ¼\r\n");
        return false;
    }
    if(PTT_name == NULL)
        return false;
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    fs_tree = Rsc_GetTree((struct tagRscNode*)g_ptWorkPath);
    parent = (struct tagRscNode*)g_ptWorkPath;
    while(Rsc_GetParent(parent) != fs_tree)//��parentָ��ָ�������Դ�ڵ�
    {
        parent = Rsc_GetParent(parent);
    }
    if(strlen(parent->name) >= namebuf_len)
    {
        Lock_SempPost(Fs_Semaphore);
        return false;
    }
    else
    {
        strcpy(PTT_name,parent->name);
        Lock_SempPost(Fs_Semaphore);
        return true;
    }

}

//----�����ļ�----------------------------------------------------------------
//����: ��file.c�ڲ����õ�һ���������������ļ�ϵͳ�в���һ���ļ���Ŀ¼��
//      �Ƿ���ڡ��������÷�����lookfor_item�����������ֻ���ز���
//      ������������ر�����Ŀ�����Ϣ��
//����: PTT_device_tag���������ķ������ݽṹָ��
//      synname,�ļ�������Ŀ¼���ַ�����
//����: �ҵ��򷵻�true���Ҳ�������false
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ����"\"�����жϵ��߼��ϲ�����__Djyfs_PickWord��
//   ����: ������
//-----------------------------------------------------------------------------
bool_t __Djyfs_LookforItem(struct tagPTTDevice *PTT_device_tag,char *synname,
                                          struct stat *result)
{
    struct tagFileRsc *parent,*son=NULL,fp_buf;
    uint16_t next_char_off;
    char name[CN_FILE_NAME_LIMIT + 1];
    if(__Djyfs_IsAbsPath(synname)) //synname���Ѿ�ȥ���˷��������������ж�
    {   //·������"\"��ʼ,��Ӹ�Ŀ¼��ʼ����
        parent = PTT_device_tag->opened_root;
        next_char_off = 1;
    }
    else
    {   //�ӵ�ǰ·����ʼ����
        parent = g_ptWorkPath;
        next_char_off = 0;
    }

    Lock_SempPend(Fs_Semaphore, CN_TIMEOUT_FOREVER);
    //��·��������Ŀ¼���ļ�
    while(__Djyfs_PickWord(synname, next_char_off, name))
    {
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
        next_char_off += strlen(name) + 1; //+1��Ϊ�����ַ�'\'
        son = (struct tagFileRsc *)Rsc_SearchSon(&parent->file_node, name);
        if(NULL != son)
        {   //Ŀ���Ѿ���
            parent = son; //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
        }
        else
        {   //Ŀ���ļ�(Ŀ¼)��δ��
            Rsc_CleanNode(&fp_buf.file_node);
            //����Ŀ�겢��ʼ���ļ����ݽṹ
            if(PTT_device_tag->lookfor_item(name, parent, &fp_buf))
            {   //Ŀ����ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                fp_buf.home_PTT = parent->home_PTT;
                son = &fp_buf;   //�Ե�ǰ�򿪵�Ŀ��Ϊ�´�ʹ�õĸ�Ŀ¼
                Rsc_CleanNode(&son->file_node);
                parent = son;
            }
            else
            {   //Ŀ¼�����ڣ�����false
                Lock_SempPost(Fs_Semaphore);
                return false;
            }
        }
        if(next_char_off >= strlen(synname))
            break;
    }
    Lock_SempPost(Fs_Semaphore);

     Djyfs_Fstat(parent, result);
    return true;
}

//----�򿪷����е��ļ�-------------------------------------------------
//����: ��file.c�ڲ����õ�һ���������򿪸÷����е�һ���ļ�����������
//      ���÷�����lookfor_itemȷ��Ŀ���ļ��Ƿ���ڣ�Ȼ����·�������Ŀ¼��
//      Ϊÿһ��Ŀ¼���� struct tagFileRsc �ṹ�ڴ棬�������ҵ���Դ�����ϡ�����
//      ���ļ���Ҳ�ҵ���Դ�����ϡ�
//����: PTT_device_tag���������ķ������ݽṹָ��
//      synname,�ļ�������Ŀ¼���ַ������ļ���Ŀ¼ͳ��item
//      mode���ļ���ģʽ
//����: �򿪵��ļ����򿪵�Ŀ¼��Դ�������򷵻�NULL
//-----------------------------------------------------------------------------
FILE *__Djyfs_OpenFile(struct tagPTTDevice *PTT_device_tag,
                                const char *synname,char *mode)
{
    struct tagFileRsc *parent,*son;
    struct tagFileRsc *opened = NULL;   //�������һ���Ѿ��򿪵�Ŀ¼��
    u32 open_result;
    uint16_t next_char_off;
    char    name[256];
    union file_attrs file_attr;
    enum _FILE_OPEN_MODE_ my_mode;
    bool_t need_to_creat = true;    //��ʾ���ļ�������ʱ���Ƿ���Ҫ����
    file_attr.all = 0;
    if((strcmp(mode,"r")==0) || (strcmp(mode,"rb")==0))
    {
        need_to_creat = false;
        my_mode = EN_R_RB;
    }else if((strcmp(mode,"w")==0) || (strcmp(mode,"wb")==0))
        my_mode = EN_W_WB;
    else if((strcmp(mode,"a")==0) || (strcmp(mode,"ab")==0))
        my_mode = EN_A_AB;
    else if((strcmp(mode,"r+")==0)||(strcmp(mode,"rb+")==0)
                            ||(strcmp(mode,"r+b")==0))
    {
        need_to_creat = false;
        my_mode = EN_R_RB_PLUS;
    }else if((strcmp(mode,"w+")==0) || (strcmp(mode,"wb+")==0)
                         || (strcmp(mode,"w+b")==0))
        my_mode = EN_W_WB_PLUS;
    else if((strcmp(mode,"a+")==0) || (strcmp(mode,"ab+")==0)
                            || (strcmp(mode,"a+b")==0))
        my_mode = EN_A_AB_PLUS;
    else if(strcmp(mode,"cd")==0)
    {
        my_mode = EN_A_AB_PLUS;
        file_attr.bits.close_delete = 1;
    }
    else
        return NULL;
    if(__Djyfs_IsAbsPath(synname))  //synname���Ѿ�ȥ���˷��������������ж�
    {
        parent = PTT_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        next_char_off = 1;
    }else
    {
        parent = g_ptWorkPath;  //�ӵ�ǰ·����ʼ����
        next_char_off = 0;
    }
    //��djyfs_fclose�Ҳ����Ū��Ը�Ŀ¼�Ĳ���
    if(strcmp(synname,"\\")==0)
        return PTT_device_tag->opened_root;//���ص��Ǹ�Ŀ¼

    //��������澯(sonʹ��ǰδ��ʼ��),�������while��if������ٻ���һ������������
    //���Բ������sonδ��ʼ���������
    son = parent;

    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    while(__Djyfs_PickPathWord(synname,next_char_off,name))
    {   //��·���𼶴�Ŀ¼����������ļ�
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
        next_char_off += strlen(name) +1;    //+1��Ϊ�����ַ�'\'
        if((son = (struct tagFileRsc *)Rsc_SearchSon(&parent->file_node,name))
                    != NULL)
        {//Ŀ���Ѿ���
            opened = son;
            if(son->open_counter != CN_LIMIT_UINT32)
                son->open_counter ++;
        }
        else
        {//Ŀ���ļ�(Ŀ¼)��δ��
            son = (struct tagFileRsc *)Mb_Malloc(s_ptContentPool,0);
            if(son == NULL)
                goto exit_open_err;    //�����ڴ�ʧ��
            son->home_PTT = parent->home_PTT;
            //����Ŀ¼����ʼ���ļ����ݽṹ
            open_result = PTT_device_tag->open_item(name,parent,son,my_mode);
            if(open_result == CN_FS_OPEN_SUCCESS)
            {//Ŀ¼���ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                //��Ŀ¼��ʵ���Ͼ��ǰ�Ŀ¼���ҵ��򿪵��ļ���Դ���ϡ�
                Rsc_AddHeadSon(&parent->file_node,&son->file_node,
                                sizeof(struct tagFileRsc),RSC_FILE,son->name);
                if(PTT_device_tag->opened_sum != CN_LIMIT_UINT32)
                    PTT_device_tag->opened_sum ++;
                son->open_counter = 1;
                son->eno = EN_FS_NO_ERROR;
                son->open_mode = my_mode;
            }
            else if(open_result == CN_FS_ITEM_EXIST)   //Ŀ¼���ڣ���ģʽ��ƥ��
            {
                goto exit_open_err;
            }
            else
            {//Ŀ¼�����ڣ����Ƿ���Ҫ������
                if(need_to_creat)
                {//��Ҫ����
                    file_attr.bits.folder = 1;  //��������Ŀ¼
                    if(PTT_device_tag->create_item(name,parent,son,
                                                file_attr,my_mode) == false)
                    {//����Ŀ¼ʧ��
                        Mb_Free(s_ptContentPool,son);
                        goto exit_open_err;
                    }
                    else
                    {
                        //��Ŀ¼ʵ���Ͼ��ǰ�Ŀ¼���ҵ��򿪵��ļ���Դ����
                        Rsc_AddHeadSon(&parent->file_node,
                                           &son->file_node,
                                           sizeof(struct tagFileRsc),
                                           RSC_FILE,son->name);
                        if(PTT_device_tag->opened_sum != CN_LIMIT_UINT32)
                            PTT_device_tag->opened_sum ++;
                        son->open_counter = 1;
                        son->open_mode = my_mode;
                        son->eno = EN_FS_NO_ERROR;
                    }
                }
                else
                {//����Ҫ����������r��r+��ʽ���ļ�ʱ��������Ҳ��������
                    Mb_Free(s_ptContentPool,son);
                    goto exit_open_err;
                }
            }
            //���򿪵���Ŀ(Ŀ¼)��������
            son->home_PTT = PTT_device_tag;
        }
        parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
    }
    //���ˣ�Ŀ¼�Ѿ�ȫ����(�򴴽�)��������ļ�
    if(__Djyfs_PickFileNameWord(synname,name))  //synname���а����ļ�����?
    {//���ļ�
        son = (struct tagFileRsc *)Rsc_SearchSon(&parent->file_node,name);
        if(son == NULL)
        {//�ļ���δ��
            son = (struct tagFileRsc *)Mb_Malloc(s_ptContentPool,0);
            if(son == NULL)
                goto exit_open_err;    //�����ڴ�ʧ��
            son->home_PTT = parent->home_PTT;
            //���Ҳ����ļ�
            open_result = PTT_device_tag->open_item(name,parent,son,my_mode);
            if(open_result == CN_FS_OPEN_SUCCESS)
            {//�ļ����ڲ��ҿ��԰�my_modeģʽ�򿪣��Ѿ���ʼ���ļ���Ϣ
                Rsc_AddHeadSon(&parent->file_node,&son->file_node,
                                sizeof(struct tagFileRsc),RSC_FILE,son->name);
                if(PTT_device_tag->opened_sum != CN_LIMIT_UINT32)
                    PTT_device_tag->opened_sum ++;
                son->open_counter = 1;
                son->eno = EN_FS_NO_ERROR;
                son->open_mode = my_mode;
            }
            else if(open_result == CN_FS_ITEM_EXIST)   //�ļ����ڣ���ģʽ��ƥ��
            {
                goto exit_open_err;
            }
            else
            {//�ļ������ڣ����Ƿ���Ҫ������
                if(need_to_creat)
                {//��Ҫ����
                    file_attr.bits.folder = 0;      //���������ļ�
                    file_attr.bits.archive = 1;     //���ļ�
                    if(PTT_device_tag->create_item(name,parent,son,
                                                    file_attr,my_mode)==false)
                    {//�����ļ�ʧ��
                        Mb_Free(s_ptContentPool,son);
                        goto exit_open_err;
                    }
                    else
                    {
                        Rsc_AddHeadSon(&parent->file_node,
                                           &son->file_node,
                                           sizeof(struct tagFileRsc),
                                           RSC_FILE,son->name);
                        son->open_counter = 1;
                        son->eno = EN_FS_NO_ERROR;
                        son->open_mode = my_mode;
                        if(PTT_device_tag->opened_sum != CN_LIMIT_UINT32)
                            PTT_device_tag->opened_sum ++;
                    }
                }
                else
                {//r��r+��ʽ������Ҫ������
                    Mb_Free(s_ptContentPool,son);
                    goto exit_open_err;
                }
            }
        }
        else
        {   //�ļ��Ѿ���
            //�Ѿ��򿪵ģ����������������·��������û�з�б�ܵȵȣ�������
            //���ж�һ�£��кô���
            if(son->file_attr.bits.folder)
            {
                if(son->open_counter != CN_LIMIT_UINT32)
                son->open_counter ++;
            }
            else
            {
                if(my_mode == son->open_mode)
                {//�Ѿ��򿪵��ļ���ֻ������ͬ�ķ�ʽ�ٴδ򿪡����Ҷ�дָ�벻�ı�
                    if(son->open_counter != CN_LIMIT_UINT32)
                        son->open_counter ++;
                    goto exit_open_son;
                }else
                {
                    Lock_SempPost(Fs_Semaphore);
                    return NULL;
                }
            }
        }
    }
    else
    {
        //��Ŀ¼��Ŀ¼��if(__Djyfs_PickFileNameWord(synname,name))ǰ�Ѿ��򿪣�
        //������ƴ򿪴���
        goto exit_open_son;//���ش򿪵�Ŀ¼
    }
    son->home_PTT = PTT_device_tag;
exit_open_son:
    Lock_SempPost(Fs_Semaphore);
    return son;
exit_open_err:
    //ɾ���Ѿ���ӵ���Դ�ڵ���ͷŷ�����ڴ�,opened������Ǳ������ӵ�
    //��һ����Դ�ڵ����һ���ڵ㡣
    if(opened == NULL)
    {
        Lock_SempPost(Fs_Semaphore);
        return NULL;
    }
    son = (struct tagFileRsc *)Rsc_GetTwig(&opened->file_node);
    while(son != NULL)
    {
        Rsc_DelNode(&son->file_node);
        Mb_Free(s_ptContentPool,son);
        son = (struct tagFileRsc *)Rsc_GetTwig(&opened->file_node);
    }
    Rsc_DelNode(&opened->file_node);
    Mb_Free(s_ptContentPool,opened);
    Lock_SempPost(Fs_Semaphore);
    return NULL;
}
//----�ļ�ϵͳ�豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ���ϲ㷢�������ļ�ϵͳ��������
//������AppCmd�����������֣�ʹ��ʱת����enum fs_left_cmd����
//      data1��data2������������ص�����
//���أ������������
//-----------------------------------------------------------------------------
ptu32_t Djyfs_AppCtrl(u32 AppCmd,ptu32_t data1,ptu32_t data2)
{
    switch(AppCmd)
    {
        case enum_fs_work_path:
        {
            //data1,�ַ���ָ��
            //����: 1�����óɹ�
            //      0, ����ʧ��
            if((char*)data1 == NULL)
                return 0;
            g_ptWorkPath = fopen((char*)data1,"w+");
            if(g_ptWorkPath == NULL)
            {
                printf("�û����ù���·��ʧ��\r\n");
                return 0;
            }
            else
                return 1;

        }break; //for enum_fs_work_path
        default:break;
    }
    return 0;
}

//----�ļ�ϵͳ�豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ�д洢�豸driver�������ļ�ϵͳ��������
//������right_cmd�����������֣�ʹ��ʱת����enum fs_right_cmd����
//      data1��data2������������ص�����
//���أ�����������أ��μ�Դ����ע��
//------------
//���¼�¼:
// 1.����: 2015/3/20
//   ˵��: 1.��ӷ���ָ�������Ӷ�Rsc_AddSonʧ�ܵ��жϡ�
//   ����: ������
//-----------------------------------------------------------------------------
ptu32_t Djyfs_FsDriverCtrl(u32 right_cmd,
                                ptu32_t data1,ptu32_t data2)
{
    struct tagFileRsc *root_folder;
    struct tagPTTDevice *PTT_device_tag;
    switch(right_cmd)
    {
        case enum_fs_add_PTT:
        {//�ɹ���ӷ���enum_fs_no_error�����򷵻�enum_fs_creat_PTT_error
         //data1Ϊ����ר�����ݽṹָ��
         //data1�� struct tagPTTDevice* ���ͣ�ָ�����ר�����ݽṹ��
            struct tagRscNode *NewNode;
            //��ȡ����ר�����ݽṹ
            PTT_device_tag = (struct tagPTTDevice*)data1;
            Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
            //���·����ķ�����Դ�ڵ���뵽�ļ�����Դ����£����ڶ���β
            NewNode = Rsc_AddSon(&s_tFileSystemRoot, &PTT_device_tag->PTT_node,
                                 sizeof(struct tagPTTDevice), RSC_PTT,
                                 PTT_device_tag->name);
            if(NULL == NewNode)
                return (EN_FS_CREAT_PTT_ERROR);

            if(PTT_device_tag->formatted)   //�����Ѿ���ʽ���õķ���
            {
                //�����Ŀ¼��Դ����ڴ�
                root_folder = Mb_Malloc(s_ptContentPool,0);
                if(root_folder == NULL)
                {   //���벻���ڴ棬�ͷ��ź���������EN_FS_CREAT_PTT_ERROR
                    Lock_SempPost(Fs_Semaphore);
                    return EN_FS_CREAT_PTT_ERROR;
                }
                memset(root_folder,0,sizeof(struct tagFileRsc));
                //���·����ĸ�Ŀ¼��Դ�ڵ���뵽��Ӧ�ķ�����Դ�ڵ��£����ڶ���β
                Rsc_AddSon(&PTT_device_tag->PTT_node,&root_folder->file_node,
                                  sizeof(struct tagFileRsc),RSC_FILE,PTT_device_tag->name);
                root_folder->home_PTT = PTT_device_tag;
                root_folder->file_attr.bits.folder = 1;
                root_folder->file_attr.bits.read_only = 1;
                //�����豸�Ĵ��ļ��ĸ����ָ��ø���Դ��㡣
                PTT_device_tag->opened_root = root_folder;
                PTT_device_tag->opened_sum = 0;
                strcpy(root_folder->name,PTT_device_tag->name);
//                strcat(PTT_device_tag->opened_root->name,":");
            }else                           //����δ��ʽ���ķ���
                PTT_device_tag->opened_root = NULL;
            Lock_SempPost(Fs_Semaphore);
            return EN_FS_NO_ERROR;
        }break; //for enum_fs_add_PTT
        case enum_fs_del_PTT:
        {
            //��ȡ����ר�����ݽṹ
            PTT_device_tag = (struct tagPTTDevice*)data1;
            Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
            if(Rsc_DelNode(&PTT_device_tag->opened_root->file_node))
            {
                Lock_SempPost(Fs_Semaphore);
                    return EN_FS_NO_ERROR;
            }else
                Lock_SempPost(Fs_Semaphore);
                return EN_FS_REMOVE_ERROR;
        }break; //for enum_fs_del_PTT
        default:break;
    }
    return EN_FS_NO_ERROR;
}

//----APPд����------------------------------------------------------------
//����: �ӷ���APP�ӿڰ�����д���ļ�,��������file.c�ڲ����ã�
//����: PTT_device_tag���������ķ������ݽṹָ��
//      buf��д�����ݻ�����ָ��
//      write_para��д�����
//      file����д���ļ�ָ�롣
//----------------------------------------------------------------------------
u32 __Djyfs_PTT_AppWrite(struct tagPTTDevice *PTT_device_tag,ptu32_t buf,
                        ptu32_t write_para,u32 res)
{
    struct tagRwPara *pl_write_para = (struct tagRwPara *)write_para;
    u32 write_len;  //��Ҫд������ݳ���
    sint64_t rest_len;
    u32 result;
    struct tagFileRsc *fp = (struct tagFileRsc *)pl_write_para->fp;  //ȡ���ļ�ָ��
    res = res;        //�����������澯
    write_len = pl_write_para->nmemb * pl_write_para->size;  //������д��ĳ���
    //��ѯ�����豸���пռ�
    rest_len = PTT_device_tag->query_file_cubage(fp);
    if(rest_len >= write_len)
    {//�����豸�����㹻�Ŀռ�
        result=PTT_device_tag->write(fp,(uint8_t*)buf,write_len);
        result = result / pl_write_para->nmemb;
    }else
    {//�����豸��û���㹻�Ŀռ䣬д��������������¼
        write_len = (u32)((rest_len / pl_write_para->nmemb) * pl_write_para->nmemb);
        result=PTT_device_tag->write(fp,(uint8_t*)buf,write_len);
        result = result / pl_write_para->nmemb;
    }
    return result;
}
//----APP������------------------------------------------------------------
//����: �ӷ���APP�ӿڶ����ļ�,��������file.c�ڲ�����
//����: PTT_device_tag���������ķ������ݽṹָ��
//      buf���������ݵĻ�����ָ��
//      read_para����������
//      file���������ļ�ָ�롣
//----------------------------------------------------------------------------
ptu32_t __Djyfs_PTT_AppRead(struct tagPTTDevice *PTT_device_tag,ptu32_t buf,
                        ptu32_t read_para,u32 res)
{
    struct tagRwPara *pl_read_para = (struct tagRwPara *)read_para;
    u32 read_len;  //��Ҫ��ȡ�����ݳ���
    sint64_t stock_size;
    u32 result;

    struct tagFileRsc *fp = (struct tagFileRsc *)pl_read_para->fp;  //ȡ���ļ�ָ��
    res = res;        //�����������澯
    read_len = pl_read_para->nmemb * pl_read_para->size;
    //��ѯ�����豸�ж������ݿɶ����ļ��ߴ粢���ɿ���������
    stock_size = PTT_device_tag->query_file_stocks(fp);
    if(stock_size >= read_len)
    {//�����豸�����㹻������
        //��ȡ�����豸
        result = PTT_device_tag->read(fp,(uint8_t*)buf,read_len);
        result = result / pl_read_para->nmemb;
    }else
    {//�����豸��û���㹻�����ݣ���Ҫ������ȡ��������ȷ��������������Ԫ
        read_len = (u32)((stock_size / pl_read_para->nmemb) * pl_read_para->nmemb);
        //��Ҫ�������豸��ȡ�������ֽ���
        read_len = read_len * pl_read_para->size;
        //��ȡ�����豸
        result = PTT_device_tag->read(fp,(uint8_t*)buf,read_len);
        result = result / pl_read_para->nmemb;
    }

    return result;
}
//----�����ַ����Ƿ����������-----------------------------------------------
//����: ���Ը������ַ����Ƿ����������
//����: fullname���������ַ���
//����: �����������򷵻ط������ĳ��ȣ����򷵻�0
//��ע: �ڲ�����������ǰ��ȷ��fullname�ǺϷ����ַ�����
//----------------------------------------------------------------------------
uint16_t __Djyfs_IsContainPTT_Name(const char *fullname)
{
    uint16_t offset;
    for(offset = 0; offset < CN_PTT_NAME_LIMIT+1; offset++)
    {
        if(fullname[offset] == ':')
            return offset;
        if(fullname[offset] == '\0')
            return 0;
    }
        return 0;
}

//----����Ŀ¼���Ƿ����·��--------------------------------------------------
//����: ����Ŀ¼���Ƿ����·��
//����: fullname���������ַ���
//����: true = fullname�Ǿ���·����false = ���Ǿ���·��
//��ע: �ڲ�����������ǰ��ȷ��fullname�ǺϷ����ַ�����
//----------------------------------------------------------------------------
bool_t __Djyfs_IsAbsPath(const char *fullname)
{
    if(fullname[0] == '\\')
    {
        return true;
    }else
    {
        if(__Djyfs_IsContainPTT_Name(fullname) != 0)
            return true;
        else
            return false;
    }
}

//----����Ŀ¼���Ƿ�����ļ���--------------------------------------------------
//����: ����Ŀ¼���Ƿ�����ļ���
//����: fullname���������ַ���
//����: true = fullname�����ļ�����false = �������ļ���
//��ע: �ڲ�����������ǰ��ȷ��fullname�ǺϷ����ַ�����
//----------------------------------------------------------------------------
bool_t __Djyfs_IsContainFileName(const char *fullname)
{
    //fullname���Ѿ������ַ������ȺϷ��Լ���ָ��
    if(fullname[strlen(fullname)-1] != '\\')
        return true;
    else
        return false;
}

//----�鿴�ļ�(Ŀ¼)�Ƿ��Ѿ���----------------------------------------------
//����: �鿴һ���ļ�(Ŀ¼)�Ƿ��Ѿ��򿪣��������Ƿ��ڱ��򿪵��ļ���Դ��
//����: PTT_device_tag���������ķ������ݽṹָ��
//      synname��������ļ�(Ŀ¼)��
//����: true = �Ѿ��򿪣�false = δ��(����������)
//��ע: ����ǰ��֤fullname�ǺϷ����ַ���
//----------------------------------------------------------------------------
bool_t __Djyfs_ItemIsOpened(struct tagPTTDevice *PTT_device_tag,
                              char *synname)
{
    struct tagFileRsc *parent,*son;
    u32 offset,name_len,index=0;
    char    name[256];

    if(__Djyfs_IsAbsPath(synname))
    {
        parent = PTT_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        offset = 1;
    }else
    {
        parent = g_ptWorkPath;  //�ӵ�ǰ·����ʼ����
        offset = 0;
    }
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    name_len = strlen(synname);
    for( ; offset< name_len; offset++)
    {//��·���𼶲鿴Ŀ¼�Ƿ��Ѿ���
        if(synname[offset] == '\\')    //�����ַ�'\'
        {
            name[index] = '\0';
            index = 0;
            Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
            if((son=(struct tagFileRsc*)Rsc_SearchSon(&parent->file_node,name))
                                            != NULL)
            {//Ŀ���Ѿ���
                parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
                Lock_SempPost(Fs_Semaphore);
            }else
            {
                Lock_SempPost(Fs_Semaphore);
                return false;
            }
        }else   //δ�����ַ�'\'��
        {
            name[index] = synname[offset];     //���Ƶ�ǰ�ַ����ļ�����
            index++;
        }
        if(index >= CN_FILE_NAME_LIMIT)    //Ŀ¼�����ȳ��ޡ�
            return false;
    }
    //���ˣ����filename��������ļ���(ĩ�ַ�����'\')��name��������ļ�����
    //index�Ǵ����ȣ���������'\0'δ��ֵ������·�����Ѿ��򿪡�
    //���filename������Ŀ¼������name�������һ��Ŀ¼�����Ѿ��򿪣�index=0��
    if(index != 0)
    {
        name[index] = '\0';
        Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
        if(Rsc_SearchSon(&parent->file_node,name) != NULL)
        {
            Lock_SempPost(Fs_Semaphore);
            return true;        //�ļ��Ѿ���
        }else
        {
            Lock_SempPost(Fs_Semaphore);
            return false;       //�ļ���δ��
        }
    }else
        return true;
}

//----ɾ���ļ�(Ŀ¼)----------------------------------------------------------
//���ܣ�ɾ��һ���ļ���Ŀ¼��������������ɾ���ǿ�Ŀ¼���Լ�ֻ���ļ���Ŀ¼����Ҫ
//      ɾ��ֻ���ļ���ֻ���Ȱ�����Ϊ��ֻ���ļ����ܽ��С���������file.c�ڲ�����
//����: PTT_device_tag���������ķ������ݽṹָ��
//      synname,�ļ�������Ŀ¼���ַ������ļ���Ŀ¼ͳ��item
//���أ��ο��ļ�ϵͳ��������б�
//----------------------------------------------------------------------------
u32 __Djyfs_RemoveItem(struct tagPTTDevice *PTT_device_tag,char *synname)
{
    struct tagFileRsc item;
    struct tagFileRsc *parent,*son=NULL;
    uint16_t next_char_off;
    char    name[256];

    if(__Djyfs_ItemIsOpened(PTT_device_tag,synname) == true)
    {
        return (u32)EN_FS_OBJECT_OPENED;       //����ɾ���Ѿ��򿪵Ķ���
    }
    if(__Djyfs_IsAbsPath(synname))  //synname���Ѿ�ȥ���˷��������������ж�
    {
        parent = PTT_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        next_char_off = 1;
    }
    else
    {
        parent = g_ptWorkPath;  //�ӵ�ǰ·����ʼ����
        next_char_off = 0;
    }
    //�Ǹ�Ŀ¼������ɾ��
       if(strcmp(synname,"\\")==0)
        return EN_FS_PTT_ERROR;

    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    while(__Djyfs_PickWord(synname,next_char_off,name))
    {//��·���𼶴�Ŀ¼����������ļ�
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
        next_char_off += strlen(name) +1;    //+1��Ϊ�����ַ�'\'
        if((son = (struct tagFileRsc *)Rsc_SearchSon(&parent->file_node,name))
                    != NULL)
        {//Ŀ���Ѿ���
            parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
        }
        else
        {//Ŀ���ļ�(Ŀ¼)��δ��
            //����Ŀ�겢��ʼ���ļ����ݽṹ
            if(PTT_device_tag->lookfor_item(name,parent,&item))
            {//Ŀ����ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                item.home_PTT = parent->home_PTT;
                son = &item;   //�Ե�ǰ�򿪵�Ŀ��Ϊ�´�ʹ�õĸ�Ŀ¼
                Rsc_CleanNode(&son->file_node);
                parent = son;
            }else       //Ŀ¼�����ڣ�����false
            {
                Lock_SempPost(Fs_Semaphore);
                return (u32)EN_FS_OBJECT_NONENTITY;   //��ɾ���Ķ��󲻴���
            }
        }
        if(next_char_off >= strlen(synname))
            break;
    }
    Lock_SempPost(Fs_Semaphore);
    item.open_counter = 0;     //�򿪴�������
    item.home_PTT = PTT_device_tag;
    if(item.file_attr.bits.read_only == true)
        return (u32)EN_FS_OBJECT_READONLY;       //����ɾ��ֻ������
    if(item.file_attr.bits.folder)
    {//�Ǹ�Ŀ¼����Ҫ�ж��Ƿ��
        if(PTT_device_tag->check_folder(&item) != 0)
            return (u32)EN_FS_FOLDER_UNBLANK;
    }
    if(PTT_device_tag->remove_item(&item))
        return EN_FS_NO_ERROR;
    else
        return EN_FS_REMOVE_ERROR;
}

//----Ŀ¼(�ļ�)����-----------------------------------------------------------
//���ܣ���һ��Ŀ¼���ļ����������������޸�ֻ��Ŀ¼���ļ��������֣�Ҫ�޸�ֻ����
//      �����������޸�ֻ���ļ������ԣ���������file.c�ڲ����ã�
//����: PTT_device_tag���������ķ������ݽṹָ��
//      old_synname,�ļ���Ŀ¼ԭ���ַ�����
//      newname���ļ���Ŀ¼���������ַ���
//���أ��ο��ļ�ϵͳ��������б�
//----------------------------------------------------------------------------
u32 __Djyfs_PTT_RenameItem(struct tagPTTDevice *PTT_device_tag,
                            char *old_synname,char *newname)
{
    struct tagFileRsc item;
    struct tagFileRsc *parent,*son=NULL;
    uint16_t next_char_off;
    char    name[256];

    if(__Djyfs_ItemIsOpened(PTT_device_tag,old_synname) == true)
    {
        return (u32)EN_FS_OBJECT_OPENED;       //���ܸ����Ѿ��򿪵Ķ���
    }
    if(__Djyfs_IsAbsPath(old_synname)) //synname���Ѿ�ȥ���˷��������������ж�
    {
        parent = PTT_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        next_char_off = 1;
    }else
    {
        parent = g_ptWorkPath;  //�ӵ�ǰ·����ʼ����
        next_char_off = 0;
    }
    //�Ǹ�Ŀ¼�������޸�
    if(strcmp(old_synname,"\\")==0)
        return EN_FS_RENAME_ERROR;
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    while(__Djyfs_PickWord(old_synname,next_char_off,name))
    {//��·���𼶴�Ŀ¼����������ļ�
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
    //    printf(name);
        next_char_off += strlen(name) +1;    //+1��Ϊ�����ַ�'\'
        if((son = (struct tagFileRsc *)Rsc_SearchSon(&parent->file_node,name))
                   != NULL)
        {//Ŀ���Ѿ���
           parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
        }else
        {//Ŀ���ļ�(Ŀ¼)��δ��
            //����Ŀ�겢��ʼ���ļ����ݽṹ
            if(PTT_device_tag->lookfor_item(name,parent,&item))
            {//Ŀ����ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                item.home_PTT = parent->home_PTT;
                son = &item;   //�Ե�ǰ�򿪵�Ŀ��Ϊ�´�ʹ�õĸ�Ŀ¼
                Rsc_CleanNode(&son->file_node);
                parent = son;
            }else       //Ŀ¼�����ڣ�����false
            {
                Lock_SempPost(Fs_Semaphore);
                return (u32)EN_FS_OBJECT_NONENTITY;   //�������Ķ��󲻴���
            }
        }
        if(next_char_off >= strlen(old_synname))
            break;
    }
    Lock_SempPost(Fs_Semaphore);
    if(item.file_attr.bits.read_only == true)
    {
           return EN_FS_OBJECT_READONLY;   //ֻ���ļ����������
    }

    if( PTT_device_tag->rename_item(son,newname))
        return EN_FS_NO_ERROR;
    else
        return EN_FS_RENAME_ERROR;

}


//----�ر��ļ�(Ŀ¼)----------------------------------------------------------
//���ܣ��ر�һ���ļ���Ŀ¼������������file.c�ڲ�����
//������PTT_device_tag�����ر��ļ���Ŀ¼���������������ݽṹָ��
//      file�����رյ��ļ���Ŀ¼��ָ��
//���أ�true=�ɹ��ر���false=ʧ��
//----------------------------------------------------------------------------
bool_t __Djyfs_PTT_CloseItem(struct tagPTTDevice *PTT_device_tag,struct tagFileRsc *fp)
{
    struct tagFileRsc *parent,*son=NULL;
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    if(fp->file_attr.bits.folder)
    {
        if(Rsc_GetSon(&fp->file_node) != NULL)
        {
            Lock_SempPost(Fs_Semaphore);
            return false;       //���رյ�Ŀ¼�����Ѿ����򿪵�������ܹر�
        }
    }

    //������ļ�����Դ������ɾ���������Ѿ��յ�Ŀ¼
    son = fp;
    parent = (struct tagFileRsc *)Rsc_GetParent(&son->file_node);
    do
    {//�ӱ�ɾ����㿪ʼ��������ɾ����Ŀ¼����Ŀ¼��ָ��Դ�����ϵĿ�Ŀ¼��㣬
     //������洢���ϸ�Ŀ¼���ܲ��ǿյ�
        if(son == g_ptWorkPath)  //��ǰ����·�����ܱ�ɾ��
            break;

        //����Ǹ�Ŀ¼�Ļ����ǲ����ͷŵġ�ֱ�ӷ��أ���������
        if(parent == PTT_device_tag->opened_root)
        {
            PTT_device_tag->close_item(son);
            if(Rsc_DelNode(&son->file_node) != NULL) //�Ӵ��ļ���Դ������ɾ�����
                Mb_Free(s_ptContentPool,son);               //�ͷŽ���ڴ�
            if(PTT_device_tag->opened_sum != 0)
                PTT_device_tag->opened_sum --;
            break;
        }
        //����ɾ�����ļ��������ֵܱ�����ô�����ͷŸ��ڵ�
        if(Rsc_GetNext(&son->file_node) != &son->file_node)
        {
            PTT_device_tag->close_item(son);
            if(Rsc_DelNode(&son->file_node) != NULL) //�Ӵ��ļ���Դ������ɾ�����
                Mb_Free(s_ptContentPool,son);
            if(PTT_device_tag->opened_sum != 0)
                PTT_device_tag->opened_sum --;
            break;
        }
        if(son->open_counter>1)     //lst 20120722,�����������һ��if��˳��
        {//��Ŀ¼��α��򿪣���ֻ��Ҫ�򿪴�����1���ʹﵽ�ر�һ�ε�Ŀ��
            son->open_counter--;
            break;
        }
        else
        {
            PTT_device_tag->close_item(son);
            if(son->file_attr.bits.close_delete)
                PTT_device_tag->remove_item(son);
        }
        if(Rsc_DelNode(&son->file_node) != NULL) //�Ӵ��ļ���Դ������ɾ�����
            Mb_Free(s_ptContentPool,son);               //�ͷŽ���ڴ�

        son = parent;               //��һѭ���ͷŸ����
        parent=(struct tagFileRsc*)Rsc_GetParent(&parent->file_node);//���������
        if(PTT_device_tag->opened_sum != 0)
            PTT_device_tag->opened_sum --;
    }while(son != PTT_device_tag->opened_root);    //ֱ����Ŀ¼
    Lock_SempPost(Fs_Semaphore);
    return true;
}

//----���ļ�д�뵽�洢��------------------------------------------------------
//����: ���ļ�д�����������ݺ�buf�е�����һ��д�뵽�洢���У������д��������
//      �ú�����file.c�ڲ�����
//������PTT_device_tag���������ļ��������������ݽṹָ��
//      buf�����ݻ�����ָ��
//      fp�����رյ��ļ���Ŀ¼��ָ��
//���أ�д��洢���ʵ�������
//----------------------------------------------------------------------------
u32 __Djyfs_PTT_FlushFile(struct tagPTTDevice *PTT_device_tag,struct tagFileRsc *fp)
{
    u32 result;
    result = PTT_device_tag->flush(fp);
    return result;
}

//----flush�����������ļ�----------------------------------------------------
//����: �ѷ����������ļ���д������������д�뵽�洢���У������д��������
//      ��������file.c�ڲ�����
//������PTT_device_tag���������ļ��������������ݽṹָ��
//���أ�0
//----------------------------------------------------------------------------
u32 __Djyfs_PTT_FlushAll(struct tagPTTDevice *PTT_device_tag)
{
    struct  tagRscNode  *current,*start,*temp;
    struct  tagFileRsc *item;
    start = &PTT_device_tag->opened_root->file_node;
    current = &PTT_device_tag->opened_root->file_node;
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    while((temp = Rsc_TraveScion(start,current)) != NULL)
    {//��ѭ�����������������д򿪵��ļ���Ŀ¼
        item = (struct tagFileRsc *)temp;
        if( ! item->file_attr.bits.folder)    //������ļ�����ˢ���洢��
            __Djyfs_PTT_FlushFile(PTT_device_tag,item);
        current = temp; //rsc_search_tree�����´ε��õ����
    }
    Lock_SempPost(Fs_Semaphore);
    return 0;
}
//----�����豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ���ϲ㷢�������ļ�ϵͳ��������
//������PTT_device_tag���������ķ������ݽṹָ��
//      left_cmd�����������֣�ʹ��ʱת����enum PTT_left_cmd����
//      data1��data2������������ص�����
//���أ�����������أ��μ�Դ����ע��
//-----------------------------------------------------------------------------
ptu32_t __Djyfs_PTT_AppCtrl(struct tagPTTDevice *PTT_device_tag,u32 left_cmd,
                                ptu32_t data1,ptu32_t data2)
{
    struct tagFileRsc *root_folder;
    switch((enum _PTT_LEFT_CMD_)left_cmd)
    {
        case EN_PTT_FORMAT:
        {//��ʽ��������data1��data2�Ǹ�ʽ������
            if(PTT_device_tag->opened_root == NULL)
            {
                //�����Ŀ¼��Դ����ڴ�
                root_folder = Mb_Malloc(s_ptContentPool,0);
                if(root_folder == NULL)
                {
                    PTT_device_tag->formatted = false;
                    return EN_FS_CREAT_ROOT_FOLDER_ERROR;
                }
                memset(root_folder,0,sizeof(struct tagFileRsc));
                //���·����ĸ�Ŀ¼��Դ�ڵ���뵽�ļ�����Դ����£�
                Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
                Rsc_AddSon(&PTT_device_tag->PTT_node,&root_folder->file_node,
                                  sizeof(struct tagFileRsc),RSC_FILE,PTT_device_tag->name);
                Lock_SempPost(Fs_Semaphore);
                root_folder->home_PTT = PTT_device_tag;
                //�����豸�Ĵ��ļ��ĸ����ָ��ø���Դ��㡣
                PTT_device_tag->opened_root = root_folder;

            }
            if(PTT_device_tag->format(data1,data2,PTT_device_tag))
            {
                PTT_device_tag->opened_root->file_attr.bits.folder = 1;
                PTT_device_tag->opened_root->file_attr.bits.read_only = 1;
                strcpy(PTT_device_tag->opened_root->name,PTT_device_tag->name);
 //               strcat(PTT_device_tag->opened_root->name,":");
                PTT_device_tag->opened_root->home_PTT = PTT_device_tag;
                PTT_device_tag->formatted = true;
                return EN_FS_NO_ERROR;
            }else
            {
                Mb_Free(s_ptContentPool, PTT_device_tag->opened_root);
                PTT_device_tag->formatted = false;
                PTT_device_tag->opened_root = NULL;
                return EN_FS_FORMAT_PTT_ERROR;
            }
        }break; //for EN_PTT_FORMAT
        case EN_CHK_PTT_FORMAT:
        {//�����ļ���data1���ļ���(Ŀ¼��)
            return PTT_device_tag->formatted;
        }break; //for EN_CHK_PTT_FORMAT
        case EN_PTT_LOOKFOR:
        {//�����ļ���data1���ļ���(Ŀ¼��)
            return (ptu32_t)__Djyfs_LookforItem(PTT_device_tag,
                                                    (char*)data1,
                                                    (struct stat*)data2);
        }break; //for EN_PTT_LOOKFOR
        case EN_PTT_OPEN:
        {//���ļ���data1���ļ�����data2��ģʽ�ַ����������ļ�ָ��
            return (ptu32_t)__Djyfs_OpenFile(PTT_device_tag,(char*)data1,(char*)data2);
        }break; //for EN_PTT_OPEN
        case EN_PTT_REMOVE:
        {//data1���ļ���Ŀ¼������data2����
            return (ptu32_t)__Djyfs_RemoveItem(PTT_device_tag,(char*)data1);
        }break;     //for EN_PTT_REMOVE
        case EN_PTT_RENAME:
        {//data1��ԭ�ļ���Ŀ¼������data2�����ļ���
            return (ptu32_t)__Djyfs_PTT_RenameItem(PTT_device_tag,(char*)data1,(char*)data2);
        }break; //for EN_PTT_RENAME
        case EN_PTT_CLOSE:
        {
            return (ptu32_t)__Djyfs_PTT_CloseItem(PTT_device_tag,(struct tagFileRsc*)data1);
        }break; //for EN_PTT_CLOSE
        case EN_PTT_FLUSH:
        {
            if(__Djyfs_PTT_FlushFile(PTT_device_tag,(struct tagFileRsc *)data1)==0)
                return (ptu32_t)EOF;
            else
                return 0;
        } break; //for EN_PTT_FLUSH
        case EN_PTT_SEEK:
        {
            return (ptu32_t)PTT_device_tag->seek_file((struct tagFileRsc *)data2,
                                                     (struct tagSeekPara *)data1);
        } break; //for EN_PTT_SEEK
        case EN_PTT_TRUNCATE:
        {
            return (ptu32_t)PTT_device_tag->set_file_size((struct tagFileRsc *)data2,
                                                     *(s64*)data1);
        } break; //for EN_PTT_TRUNCATE
        case EN_PTT_TRAVERSAL_SON:
        {
            return (ptu32_t)PTT_device_tag->item_traversal_son(
                            (struct tagFileRsc *)data1,(struct tagFileRsc *)data2);
        } break; //for EN_PTT_TRAVERSAL_SON
        case EN_QUERY_FILE_CUBAGE:
        {
            return PTT_device_tag->query_file_cubage((struct tagFileRsc *)data2);
        }
        default : break;
    }
    return 0;
}

//----�����豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ�д洢�豸driver�������ļ�ϵͳ��������
//������PTT_device_tag���������ķ������ݽṹָ��
//      right_cmd�����������֣�ʹ��ʱת����enum PTT_right_cmd����
//      data1��data2������������ص�����
//���أ�����������أ��μ�Դ����ע��
//-----------------------------------------------------------------------------
ptu32_t __Djyfs_PTT_MemoryCtrl(struct tagPTTDevice *PTT_device_tag,u32 right_cmd,
                                ptu32_t data1,ptu32_t data2)
{
    data2 = data2;        //�����������澯
    switch((enum _PTT_RIGHT_CMD_)right_cmd)
    {
        case CN_PTT_CLR_READBUF:
        {//����ļ��Ķ���������data1���ļ�ָ��
            if(((struct tagFileRsc*)data1)->p_read_buf != NULL)
               Ring_Flush(((struct tagFileRsc*)data1)->p_read_buf);
            return 0;
        }break; //for CN_PTT_CLR_READBUF
        case CN_PTT_CLR_WRITEBUF:
        {//����ļ���д��������data1���ļ�ָ��
            if(((struct tagFileRsc*)data1)->p_write_buf != NULL)
               Ring_Flush(((struct tagFileRsc*)data1)->p_write_buf);
            return 0;
        }break; //for CN_PTT_CLR_WRITEBUF
        case CN_PTT_FLUSH_ONE:
        {//
            return (ptu32_t)__Djyfs_PTT_FlushFile(PTT_device_tag,(struct tagFileRsc *)data1);
        }break; //for CN_PTT_FLUSH_ONE
        case CN_PTT_FLUSH_ALL:
        {//
            return (ptu32_t)__Djyfs_PTT_FlushAll(PTT_device_tag);
        }break; //for CN_PTT_FLUSH_ALL
        case CN_PTT_UPDATE_MEDIUM_TAG:
        {//�޸�(����)һ���ļ���ý���ǣ�data1Ϊ�ļ�ָ�룬data2Ϊ�±��
            ;
        }break; //for CN_PTT_UPDATE_MEDIUM_TAG
        case CN_PTT_SEARCH_MEDIUM_TAG:
        {//����һ���ļ���ý���ǣ�data1Ϊ���ļ���ý���ǣ������ļ�ָ��
            ;
        }break; //for CN_PTT_SEARCH_MEDIUM_TAG
        default:break;
    }
    return 0;
}

//----�򿪷����ӿ�------------------------------------------------------
//����: �����ļ�(Ŀ¼)��������֮��Ӧ�ķ����ӿڡ�
//����: fullname���ļ�(Ŀ¼)��ָ��
//���أ��豸��������������������NULL:
//      1.����������  2.fullname��pg_work_path_name��û��ָ��������
//----------------------------------------------------------------------------
struct tagPTTDevice *__Djyfs_OpenPTT_Left(const char *fullname)
{
    struct tagPTTDevice *PTT_device_tag;
    char PTT_name[256];
    uint16_t name_len;

    name_len = __Djyfs_IsContainPTT_Name(fullname);
    if(name_len != 0)
    {//fullname�ǰ������������ڵ�ȫ·��(Ҳ�����ǵ�ǰ����)
        memcpy(PTT_name,fullname,name_len); //�Ӳ�����copy������
        PTT_name[name_len] = '\0';          //д�봮������
    }else
    {
        //fullname��û��ָ����������Ӧ�ӵ�ǰ����·����ȡ
        if(!Djyfs_GetPTT_Name(PTT_name,CN_FILE_NAME_LIMIT))
            return NULL;
    }
    //��ȡ�������ݽṹָ��
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    PTT_device_tag = (struct tagPTTDevice *)Rsc_SearchSon(&s_tFileSystemRoot,PTT_name);
    Lock_SempPost(Fs_Semaphore);
    return PTT_device_tag;
}

//----��Ŀ¼��ȡһ��-----------------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ���curent_offsetλ�ÿ�ʼȡ��һ��
//      ��'\'�ָ���word
//����: synname�������ַ�������ʽ��"\aaa\bbb\ccc.txt"
//      start_offset��������ʼλ��ָ��
//      name������ȡ�õ��ַ����Ļ�����ָ��
//����: true=�ɹ�
//      false=ʧ�ܣ�1���Ѿ�������һ��Ŀ¼��
//                  2��synname�и����Ͳ�����Ŀ¼��
//                  3���ֳ�����255�ַ�
//      ��*name�з���ȡ�õ��ַ���
//��ע: synnameӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: 1.·������(��current_offset����)"\",����false��
//         2.·������(��current_offset����)Ϊ���ַ�,����false;
//   ����: ������
//-----------------------------------------------------------------------------
bool_t __Djyfs_PickWord(const char *synname,u32 curent_offset,char *name)
{
    u32 index = 0;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    u32 name_len = strlen(synname);
    u32 offset = curent_offset;

    if (('\\' == synname[curent_offset]) && ('\0' == synname[curent_offset+1]))
        return (false);//·������Ϊ"\"
    if ('\0' == synname[curent_offset])
        return (false);//·������Ϊ���ַ�

    for( ; offset< name_len; offset++)
    {
        if(synname[offset] == '\\')
        {   //�����ַ�'\\'
            name[index] = '\0';
            return true;
        }
        else
        {   //δ�����ַ�'\\'
            if(index == CN_FILE_NAME_LIMIT)
                return false;
            name[index] = synname[offset]; //���Ƶ�ǰ�ַ����ļ�����
            index++;
        }
    }

    name[index] = '\0';//�ļ�(��Ŀ¼)�Ľ�ֹ���š�
    return true; //����ȷ��ȡĿ¼�֣����򲻻���������

}

//----ȡһĿ¼����-------------------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�����curent_offsetλ�ÿ�ʼȡ��һ��
//      Ŀ¼��
//����: synname�������ַ�������ʽ��"\aaa\bbb\ccc.txt"
//      start_offset��������ʼλ��ָ��
//      name������ȡ�õ��ַ����Ļ�����ָ��
//����: true=�ɹ�
//      false=ʧ�ܣ�1���Ѿ�������һ��Ŀ¼��
//                  2��synname�и����Ͳ�����Ŀ¼��
//                  3���ֳ�����255�ַ�
//      ��*name�з���ȡ�õ��ַ���
//��ע: synnameӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __Djyfs_PickPathWord(const char *synname,u32 curent_offset,char *name)
{
    u32 index = 0;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    u32 name_len = strlen(synname);
    u32 offset = curent_offset;
    for( ; offset< name_len; offset++)
    {
        if(synname[offset] == '\\')    //�����ַ�'\'
        {
            name[index] = '\0';
            return true;
        }else   //δ�����ַ�'\'��
        {
            if(index == CN_FILE_NAME_LIMIT)
                return false;
            name[index] = synname[offset];     //���Ƶ�ǰ�ַ����ļ�����
            index++;
        }
    }
    return false;   //����ȷ��ȡĿ¼�֣����򲻻���������
}

//----ȡ�ļ�����---------------------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�����ȡ���ļ���
//����: synname�������ַ�������ʽ��"\aaa\bbb\ccc.txt"
//      start_offset��������ʼλ��ָ��
//      name������ȡ�õ��ַ����Ļ�����ָ��
//����: true=�ɹ�
//      false=ʧ�ܣ�synname�в������ļ���
//      ��*name�з���ȡ�õ��ַ���
//��ע: synnameӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __Djyfs_PickFileNameWord(const char *synname,char *name)
{
    u32 index = 0;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    u32 name_len = strlen(synname);
    u32 offset;
    for(offset = name_len; offset >0; offset--)
    {//�����һ���ַ������ַ�'\'
        if(synname[offset-1] == '\\')    //�����ַ�'\'
            break;
    }
    if(offset == name_len)      //���һ���ַ�����'\'������û���ļ���
        return false;
    else if((name_len -offset) > CN_FILE_NAME_LIMIT)    //�ļ�������
        return false;
    for( ; offset < name_len; offset++)
    {
        name[index] = synname[offset];
        index++;
    }
    name[index] = '\0';
    return true;
}

//----����·���еķ�������---------------------------------------------------
//����: �����ļ�(Ŀ¼)���ַ����й��ڷ����Ĳ��֣���ȡsynname�����磬
//      ����"c:\aaa\bbb\ccc.txt"��������"\aaa\bbb\ccc.txt"
//����: fullname,�����ַ���
//����: ȡ�õ�·����
//----------------------------------------------------------------------------
char *__Djyfs_PickSynName(const char *fullname)
{
    uint16_t name_len;

    name_len = __Djyfs_IsContainPTT_Name(fullname);
    if(name_len != 0)//����·��
           return (char *)fullname + name_len +1;
    else
        return (char *)fullname;//���·��������ԭ�����ַ���
}


//----���·�����Ƿ�Ϸ�------------------------------------------------------
//����: ���·�����Ƿ�Ϸ�����windows���ݣ��ݲ���������·��������������:
//      1.���ַ�':'�ָ��ķ��������ȱ���Ϸ���':'����������ַ�'\'
//      2.�����������������'\'��
//      3.���������'* ? / | < > "'��7���ַ�,��Щ�ַ���������������;
//      4.��������ֳ��ȳ���255�ַ���object
//����: fullname,�����ַ���
//����: true=�Ϸ���false=�Ƿ�
//----------------------------------------------------------------------------
bool_t __Djyfs_CheckFullName(const char *fullname)
{
    u32 colon_offset = CN_LIMIT_UINT32,syn_offset;
    u32 loop,temp;
    ufast_t depth = 0;
    if(fullname == NULL)
        return false;
    //����ʹ��strlen���fullname���ȣ���Ϊ�����������м����Ŀ�����޶�������
    //������
    for(loop = 0; loop <= CN_PTT_NAME_LIMIT; loop++)
    {//��鴮�е�һ��':'������ʲôλ��
        if((fullname[loop]=='*')||(fullname[loop]=='/')||(fullname[loop]=='?')
           ||(fullname[loop]=='<')||(fullname[loop]=='>')||(fullname[loop]=='|')
           ||(fullname[loop]=='"'))
            return false;   //���ƴ��в��ܳ����⼸���ַ���
        if(fullname[loop] == ':')
        {
            colon_offset = loop; //��һ��':'���ֵ�λ�ã����ڼ�����������
            break;
        }

        if(fullname[loop] == '\\')   //����word�ָ�����
            break;
        if(fullname[loop] == '\0')    //����ֹͣ����
            break;
    }
    if(colon_offset == 0)
        return false;       //���ַ�������':'
    if(colon_offset != CN_LIMIT_UINT32) //':'λ�úϷ�(���������ҳ��ȺϷ�)
    {
        if(fullname[colon_offset+1] != '\\')
            return false;       //ð�ź���������ַ�'\'
        syn_offset = colon_offset + 2;
    }else if(fullname[0] =='\\')
        syn_offset = 1;
    else
        syn_offset = 0;
    temp = 0;
    //�����û�г���cn_file_name_limit�ַ���object���������û�зǷ��ַ�
    //�����������ȫ·�����������ַ��������ᳬ��
    //CN_PATH_DEPTH_LIMIT * (CN_FILE_NAME_LIMIT+1),+1�ǰ���word�ָ���'\'
    for(loop = syn_offset;
               (loop < CN_PATH_DEPTH_LIMIT*(CN_FILE_NAME_LIMIT+1)+syn_offset);
               loop++)
    {
        //�ӷ��������濪ʼ�������Ƿ��ַ�����ʱ��ð��':'Ҳ�ǷǷ��ַ�
        if((fullname[loop]=='*')||(fullname[loop]=='/')||(fullname[loop]=='?')
           ||(fullname[loop]=='<')||(fullname[loop]=='>')||(fullname[loop]=='|')
           ||(fullname[loop]=='"')||(fullname[loop]==':'))
            return false;   //�����⼸���Ƿ��ַ���
        if(fullname[loop] == '\0')
            return true;
        if(fullname[loop] == '\\')
        {
            if(temp == 0)
                return false;       //˵������������word�ָ���'\'
            temp = 0;//�ļ��ĳ������㣬���¿�ʼ����
            depth++;
        }else
        {
            if(depth >= CN_PATH_DEPTH_LIMIT)    //·������
                return false;
            temp++;
            if(temp > CN_FILE_NAME_LIMIT)       //word���ȳ���
                return false;
        }
    }
    return false;       //���ܵ�����϶�û���ҵ���������
}

//----��鵥�����Ƿ�Ϸ�------------------------------------------------------
//����: ���һ������������Ƿ�Ϸ��������ܺ�'\/?:*'�⼸���ַ�
//����: word,�����ַ����������ǵ�һ·������Ҳ�������ļ���
//����: true=�Ϸ���false=�Ƿ�
//----------------------------------------------------------------------------
bool_t __check_word(const char *word)
{
    u32 loop;
    //word���Ѿ������ַ������ȺϷ��Լ���ָ��
    for(loop = 0; loop < strlen(word); loop++)
    {
        if((word[loop]=='*')||(word[loop]=='/')
                        ||(word[loop]=='?')||(word[loop] == ':'))
            return false;   //�����⼸���Ƿ��ַ���
    }
    return true;
}

//----��ȡ�ļ���Ϣ-------------------------------------------------------------
//����: ����һ���ļ�ָ�룬ȡ�ø��ļ���һЩ��Ϣ
//����: fp���ļ�ָ��
//      fp_info�����ڷ�����Ϣ��ָ�롣
//����: ��Ŀ¼ָ��
//------------
//���¼�¼:
// 1.����: 2015/3/25
//   ˵��: ����fp_info�ṹ��
//   ����: ������
//-----------------------------------------------------------------------------
bool_t Djyfs_Fstat(struct tagFileRsc *fp,struct stat *fp_info)
{

    if((fp == NULL) || (fp_info == NULL))
        return false;
    fp_info->st_mode = fp->file_attr;
    fp_info->open_mode = fp->open_mode;

    fp_info->CreateTime = fp->CreateTime;
    fp_info->ModTime = fp->ModTime;
    fp_info->file_size = fp->file_size;

    return true;
}
//----��ȡ��Ŀ¼---------------------------------------------------------------
//����: ����һ���ļ�ָ�룬ȡ�ø��ļ�ָ�����ڵķ����ĸ�Ŀ¼ָ��
//����: fp���ļ�ָ��
//����: ��Ŀ¼ָ��
//-----------------------------------------------------------------------------
struct tagFileRsc *Djyfs_GetRootFolder(struct tagFileRsc *fp)
{
    struct tagPTTDevice *PTT_device_tag;
    //fullname����һ���Ϸ����ַ���
    if(fp == NULL)
        return false;
    PTT_device_tag = (struct tagPTTDevice*)(fp->home_PTT);
    if(PTT_device_tag->formatted)
    {
        return PTT_device_tag->opened_root;
    }else
        return NULL;
}
//----ȡ������Ϣ-------------------------------------------------------------
//����: ȡһ����������Ϣ���ܿռ䣬���пռ��
//����: fullname��·��������fullname�а����˷���������ȡ�÷�������Ϣ�����û������
//          ��ȡ��ǰ·�����ڷ�����Ϣ������ǰ·����û�����ã�����false����ֻ���
//          ȡ��ǰ·�����ڷ�����Ϣ���ɸ�NULL
//      sum,valid,free,���ڷ������ݣ�������NULL
//����: true = ��ȷ��ȡ��false = ���󣬿��ܲ�������Ҳ����û��ʽ��
//------------
//���¼�¼:
// 1.����: 2015/3/25
//   ˵��: ���º���ʵ��
//   ����: ������
//-----------------------------------------------------------------------------
bool_t Djyfs_PTT_Stat(const char *fullname,s64 *sum,s64 *valid,s64 *free)
{

    struct tagRscNode *FsRoot, *Fs;

    //fullname����һ���Ϸ����ַ���
    if(__Djyfs_CheckFullName(fullname) == false)
    {
        printf("�������������Ϸ�.\r\n");
        return (false);
    }

    FsRoot = Rsc_SearchTree("fs");
    if( NULL == FsRoot )
    {
        printf("ϵͳ��δ��װ�ļ�ϵͳģ��\r\n");
        return (false);
    }

    /* �����ļ�ϵͳ�¸������������ļ�ϵͳ�� */
    Fs = FsRoot;
    do
    {
        Fs = Rsc_TraveSon(FsRoot, Fs);
        if(NULL != Fs) {
            if(0 == strcmp(Fs->name, fullname))
            {
            	((struct tagPTTDevice *)Fs)->check_PTT(
            			(struct tagPTTDevice *)Fs, sum, valid, free);

                return (true);
            }
        }
    }while(NULL != Fs);

    printf("����������\r\n");
    return (false);

}

//----ȡ������Ϣ-------------------------------------------------------------
//����: ȡfp���ڵķ�������Ϣ���ܿռ䣬��Ч�ռ䡢���пռ�
//����: fp���ļ�ָ��
//      sum,valid,free,���ڷ������ݣ�������NULL
//����: true = ��ȷ��ȡ��false = ���󣬿��ܲ�������Ҳ����û��ʽ��
//-----------------------------------------------------------------------------
bool_t Djyfs_PTT_StatFp(struct tagFileRsc * fp,s64 *sum,s64 *valid,s64 *free)
{
    bool_t result;
    struct tagPTTDevice *PTT_device_tag;
    //fullname����һ���Ϸ����ַ���
    if(fp == NULL)
        return false;
    PTT_device_tag = (struct tagPTTDevice*)(fp->home_PTT);
    if(PTT_device_tag->formatted)
    {
        PTT_device_tag->check_PTT(PTT_device_tag,sum,valid,free);
        result = true;
    }else
        result = false;
    return result;
}

//----ȡ����·���ַ�������-----------------------------------------------------
//����: ������
//����: fp��Ŀ���ļ�
//����: �ַ������ȣ�����������'\0'
//-----------------------------------------------------------------------------
u32  Djyfs_GetPathLen(struct tagFileRsc * fp)
{
    struct tagFileRsc *current_fp;
    struct tagPTTDevice *PTT_device_tag;
    u32 len = 0,index;

    if(fp == NULL)
        return 0;

    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);

    current_fp = fp;
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    for(index = 0; index<CN_PATH_DEPTH_LIMIT; index++)
    {
        len += strlen(current_fp->name);
        len+=2;      //�ַ�'\'���Լ����һ����'\0'��λ��
        if(current_fp == PTT_device_tag->opened_root)
            break;
        current_fp = (struct tagFileRsc *)Rsc_GetParent(
                                    (struct tagRscNode *)current_fp);
    }
    Lock_SempPost(Fs_Semaphore);
    return len+1;    //+1�Ƿ�������ġ�:��
}

//----ȡ����·��-----------------------------------------------------------
//����: ȡָ���ļ��ľ���·����buf�У���󳤶�Ϊmaxlen��
//      bufΪNULL��̬����buf�����ڴ棬���÷�����ʹ����Ϻ��ͷ��ڴ档djyosǿ��
//      ���Ƽ����ַ�ʽ����Ϊ���봫ͳ�������ּ��ݲű������ַ�ʽ.djyfs�ṩ�������
//      ��:�ȵ���djyfs_getpath_len��ȡĿ¼���ȣ��û��Լ����ݳ��ȷ����ڴ棬�ٵ���
//      djyfs_getpath���Ϳ�����"malloc--free"��Ժ���������ͬһ�������ڡ�
//����: fp��Ŀ���ļ�
//      buf������·�������ַ�����������
//      maxlen���������ƣ�����������'\0'��
//����: true = �ɹ���ȡ��false = ʧ��
//-----------------------------------------------------------------------------
bool_t Djyfs_GetPath(struct tagFileRsc * fp,char *buf,u32 maxlen)
{
    struct tagFileRsc *current_fp;
    struct tagPTTDevice *PTT_device_tag;
    u32 index=0,len = 0;
    struct tagRscNode *point_num[CN_PATH_DEPTH_LIMIT];

    if((fp == NULL) || (maxlen == 0))
        return false;
    if(buf == NULL)
    {
        maxlen = Djyfs_GetPathLen(fp);
        if((buf = M_MallocLc(maxlen,0)) == NULL)
            return false;
        buf[0] = '\0';
    }
    else
        buf[0] = '\0';
    PTT_device_tag = (struct tagPTTDevice *)(g_ptWorkPath->home_PTT);
    current_fp = fp;

    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    for(index = 0; index<CN_PATH_DEPTH_LIMIT; index++)
    {
        point_num[index] = (struct tagRscNode *)current_fp;
        if(current_fp == PTT_device_tag->opened_root)
            break;
        current_fp = (struct tagFileRsc *)Rsc_GetParent(
                                    (struct tagRscNode *)current_fp);
    }
    Lock_SempPost(Fs_Semaphore);
    len = strlen(point_num[0]->name) + 2;
    if(len < maxlen)
    {
        strcat(buf,point_num[0]->name);
        strcat(buf,":\\");
    }

    for( ; index>0; index--)
    {
        len += strlen(point_num[index]->name) +1;
        if(len < maxlen)
        {
            strcat(buf,point_num[index]->name);
            strcat(buf,"\\");
        }
        else
            break;
    }
    return true;
}

//----ȡ��ǰ����·���ַ�������-------------------------------------------------
//����: ������
//����: ��
//����: �ַ������ȣ�����������'\0'
//-----------------------------------------------------------------------------
u32  Djyfs_GetCwdLen(void)
{
    return Djyfs_GetPathLen(g_ptWorkPath);
}

//----ȡ��ǰ����·��-----------------------------------------------------------
//����: ȡ��ǰ�ľ���·����buf�У���󳤶�Ϊmaxlen��
//      bufΪNULL��̬����buf�����ڴ棬���÷�����ʹ����Ϻ��ͷ��ڴ档djyosǿ��
//      ���Ƽ����ַ�ʽ����Ϊ���봫ͳ�������ּ��ݲű������ַ�ʽ.djyfs�ṩ�������
//      ��:�ȵ���djyfs_getpath_len��ȡĿ¼���ȣ��û��Լ����ݳ��ȷ����ڴ棬�ٵ���
//      djyfs_getpath���Ϳ�����"malloc--free"��Ժ���������ͬһ�������ڡ�
//����: buf������·�������ַ�����������
//      maxlen���������ƣ�����������'\0'��
//����: true = �ɹ���ȡ��false = ʧ��
//-----------------------------------------------------------------------------
bool_t Djyfs_GetCwd(char *buf,u32 maxlen)
{
    if(maxlen == 0)
        return false;
    return Djyfs_GetPath(g_ptWorkPath,buf,maxlen);
}

//----����һ��Ŀ¼�µ���Ŀ¼(�ļ�)---------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڻ�ȡ�ļ���Ŀ¼������Ϣ,����ȡ��Ϣ��
//      �ļ���û�б��򿪡�
//����: parent,��Ŀ¼����Ϣ
//      current����ǰ����λ�ã������ʾ��һ�β��ҡ�
//����: current����һ���ļ�
//��ע: ��������Ƿ�Ҫ���windows���Ƶ�FindFirstFile������API����ʱ������
//      api��ʱ����˵��
//-----------------------------------------------------------------------------
FILE *Djyfs_FolderTraversalSon(struct tagFileRsc *parent,
                                       struct tagFileRsc *current)
{
    struct tagFileRsc * result;
    struct tagPTTDevice *PTT_device_tag;

    if(parent == NULL)
        return NULL;
    PTT_device_tag = (struct tagPTTDevice *)parent->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(! parent->file_attr.bits.folder)
    {
        result = NULL;         //�ļ����ܱ���
        goto goto_end;
    }
    result = (struct tagFileRsc *)__Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_TRAVERSAL_SON,
                                    (ptu32_t)parent,(ptu32_t)current);

goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;

}

//----�����ļ�----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڼ���ļ���Ŀ¼���Ƿ����,������buf��
//      �գ���ͨ��buf�����ļ���Ϣ
//����: fullname,�ļ���Ŀ¼����
//      buf,�ǿ��򷵻��ļ���Ϣ
//����: -1��ʾʧ��
//------------
//���¼�¼:
// 1.����: 2015/3/25
//   ˵��: ���º���ʵ��
//   ����: ������
//-----------------------------------------------------------------------------

s32 stat(const char *filename, struct stat *buf)
{
    bool_t result;
    struct tagPTTDevice *PTT_device_tag;
    char *synname;

    if(filename == NULL)
        return -1; //�˴���������errno = ENOENT,�ļ��������ڡ�
    if(!__Djyfs_CheckFullName(filename))
        return -1; //�˴���������errno = ENOENT,�ļ��������ڡ�

    PTT_device_tag = __Djyfs_OpenPTT_Left(filename);
    if(PTT_device_tag == NULL)
        return -1; //���������ڣ���fullname�͵�ǰ·����û��ָ��������
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(PTT_device_tag->formatted)
    {
        synname = __Djyfs_PickSynName(filename);
       //���÷������ƺ���
       result = (bool_t)__Djyfs_PTT_AppCtrl(PTT_device_tag, EN_PTT_LOOKFOR,
                                        (ptu32_t)synname,(ptu32_t)buf);
    }
    else
        result = false;
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    if(result)
        return 0;
    else
        return -1;
}
//-----------------------------------------------------------------------------
//����: �����ļ��Ƿ���ڣ��Լ�����Ȩ�ޡ���������ҵ���Ŀ��Ŀ¼����ֱ�ӷ����ļ����ڡ�
//����: fullname���ļ���
//     flags�������Եķ���Ȩ�ޣ�0������ļ��Ƿ���ڡ�
//         F_OK        0   // does file exist
//         X_OK        1   // is it executable by caller
//         W_OK        2   // is it writable by caller
//         R_OK        4   // is it readable by caller
//���: ��
//����: 0=�ļ�������flags��ʾ�ķ���Ȩ��Ϊ�档-1=�ļ������ڣ����޷���Ȩ�ޡ�
//------------
//���¼�¼:
// 1.����: 2014/12/08
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
s32 access(const char *fullname, int flags)
{
  struct stat s;
  if (stat(fullname, &s))
    return -1;
  if (s.st_mode.bits.folder == 1)
    return 0;
  if (flags & W_OK)
  {
    if (s.st_mode.bits.read_only)
      return -1;
    return 0;
  }
  return 0;
}

//-----------------------------------------------------------------------------
//����: ���һ���ļ��ĳ���״̬��
//����: fp,Ŀ���ļ�
//���: ��
//����: ��
//------------
//���¼�¼:
// 1.����: 2014/12/09
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
void clearerr(FILE *fp)
{
    if(fp == NULL)
        return;
    fp->eno = EN_FS_NO_ERROR;
}

//----��ѯ�ļ���д��С---------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڲ�ѯ�ļ���д���ʵ�ʴ�С
//����: fp����д���ļ�
//����: ��д��Ĵ�С
//-----------------------------------------------------------------------------
size_t Djyfs_Fcubage(FILE *fp)
{
    size_t result;
    struct tagPTTDevice *PTT_device_tag;
    if(fp == NULL)
        return 0;
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(fp->file_attr.bits.folder)
    { //Ŀ¼����д
        result = 0;
        goto goto_end;
    }
    if(fp->open_mode == EN_R_RB)
    {//ֻ���ļ�������д
        result = 0;
        goto goto_end;
    }
    result = __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_QUERY_FILE_CUBAGE,
                                (ptu32_t)0,(ptu32_t)fp);
goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----�ı��ļ�����-------------------------------------------------------------
//����: �ı��ļ����ȣ�����ļ���д��ʽ�򿪣����³��ȴ����ļ��ߴ磬
//      �������ļ��������ӡ�
//������fp�����������ļ�ָ��
//      offset���ƶ���λ��
//      whence���Ӻδ���ʼ�����ƶ����ȣ��μ�SEEK_CUR�ȵĶ���
//���أ�0=�ɹ���-1=ʧ��
//-----------------------------------------------------------------------------
u32 Djyfs_Ftruncate (FILE *fp, sint64_t offset)
{
    struct tagPTTDevice *PTT_device_tag;
    u32 result;

    if(fp == NULL)
        return CN_LIMIT_UINT32;
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if( (fp->file_attr.bits.folder)
        ||(fp->open_mode == EN_R_RB)
        ||(fp->open_mode == EN_A_AB)
        ||(fp->open_mode == EN_A_AB_PLUS)
        ||(fp->file_size == offset) )
    {//Ŀ¼û�в������ó���
        result = CN_LIMIT_UINT32;
        goto goto_end;
    }

    result = __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_TRUNCATE ,
                                (ptu32_t)&offset,(ptu32_t)fp);
goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----��ʽ������-------------------------------------------------------------
//����: ��ʽ������
//����: format_para1����ʽ������
//      format_para1����ʽ������
//      ptt_name������ʽ���ķ�������
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t Djyfs_Format(u32 format_para1,u32 format_para2,char *PTT_name)
{
    u32 name_len,loop;
    bool_t result;
    struct tagPTTDevice *PTT_device_tag;
    if(PTT_name == NULL)
        return false;
    name_len = strnlen(PTT_name,CN_PTT_NAME_LIMIT+1);
    if((name_len == 0) || (name_len > CN_PTT_NAME_LIMIT))
        return false;
    for(loop = 0;loop < name_len; loop++)
    {
        if((PTT_name[loop]=='*')||(PTT_name[loop]=='/')||(PTT_name[loop]=='?')
           ||(PTT_name[loop]=='<')||(PTT_name[loop]=='>')||(PTT_name[loop]=='|')
           ||(PTT_name[loop]=='"')||(PTT_name[loop]==':'))
            return false;   //���ƴ��в��ܳ����⼸���ַ���
    }
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    PTT_device_tag = (struct tagPTTDevice *)Rsc_SearchSon(&s_tFileSystemRoot,PTT_name);
    Lock_SempPost(Fs_Semaphore);
    if(PTT_device_tag == NULL)
        return false;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(PTT_device_tag->opened_sum != 0)
        result = false;
    else
    {
        if(EN_FS_NO_ERROR == __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_FORMAT,
                                       format_para1,format_para2))
        {
            result = true;
        }
        else
            result = false;
    }
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}
//----�ر��ļ�-----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڹرմ������ļ�
//����: fp�����رյ��ļ�
//����: 0=�ɹ���cn_limit_uint32=ʧ�ܡ�
//-----------------------------------------------------------------------------
u32 fclose(FILE *fp)
{
    bool_t result;
    struct tagPTTDevice *PTT_device_tag;
    if(fp == NULL)
        return CN_LIMIT_UINT32;
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
     //��Ŀ¼�͵�ǰ����·��������ر�
    if(fp == PTT_device_tag->opened_root || fp == g_ptWorkPath)
    {
        Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
        return true;
    }

    if(fp->open_counter>1)
    {//���ļ���α��򿪣���ֻ��Ҫ�򿪴�����1���ʹﵽ�ر�һ�ε�Ŀ��
        fp->open_counter--;
        Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
        return 0;
    }
    result = (bool_t)__Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_CLOSE,(ptu32_t)fp,0);
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    if(result)
    {
        //��ֹ��ιر�
        fp = NULL;
        return 0;
    }
    else
        return CN_LIMIT_UINT32;
}

//-----------------------------------------------------------------------------
//����: ����ļ�ָ���Ƿ�ﵽ��β��
//����: fp,Ŀ���ļ�
//���: ��
//����: 0 = �ļ�δ������-1 = ����������1 = �ļ�δ����
//------------
//���¼�¼:
// 1.����: 2014/12/09
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
s32 feof(FILE *fp)
{
    if(fp == NULL)
        return -1;
    if(fp->file_size == fp->read_ptr)
        return 1;
    else
        return 0;
}

//-----------------------------------------------------------------------------
//����: ��ȡ�ļ������һ�η��ʴ�����롣
//����: fp,Ŀ���ļ�
//���: ��
//����: ���ļ����һ�η��ʴ�����롣
//------------
//���¼�¼:
// 1.����: 2014/12/09
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
s32 ferror(FILE *fp)
{
    if(fp == NULL)
        return -1;
    return fp->eno;
}

//----ˢ���ļ�-----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴӴ������ļ�������
//����: fp����ˢ�µ��ļ�
//����: 0=�ɹ���ָ������û�л���������ֻ����ʱҲ����0ֵ������EOFָ��һ������
//-----------------------------------------------------------------------------
u32 fflush(FILE *fp)
{
    struct tagPTTDevice *PTT_device_tag;
    size_t result;
    if(fp == NULL)
        return EOF;
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(fp->file_attr.bits.folder)
    {   //Ŀ¼����д
        result = EOF;
        goto goto_end;
    }
    if(fp->open_mode == EN_R_RB)
    {//ֻ���ļ�������ˢ��
        result = EOF;
        goto goto_end;
    }
    result = __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_FLUSH,
                                (ptu32_t)fp,0);
goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----���ļ�----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴ��ļ�ϵͳ�е�һ���ļ�
//����: fullname,�ļ���
//      mode���ļ���ģʽ
//����: �ɹ����򷵻��ļ�ָ�룬���򷵻�NULL��
//-----------------------------------------------------------------------------
FILE *fopen(const char *fullname, char *mode)
{
    struct tagFileRsc *result;
    struct tagPTTDevice *PTT_device_tag;
    char *synname;
    //fullname����һ���Ϸ����ַ���
    if((__Djyfs_CheckFullName(fullname) == false)||(mode ==  NULL))
        return NULL;
    PTT_device_tag = __Djyfs_OpenPTT_Left(fullname);
    if(PTT_device_tag == NULL)
        return NULL;    //���������ڣ���fullname�͵�ǰ·����û��ָ��������
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(PTT_device_tag->formatted)
    {
        synname = __Djyfs_PickSynName(fullname);
        //���ô��ļ�����
        result = (struct tagFileRsc *)__Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_OPEN,
                          (ptu32_t)synname,(ptu32_t)mode);
    }else
        result = NULL;
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----�̰߳�ȫ���ļ�-----------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴӴ������ļ������ݡ�C��׼��
//      djyfs_fread����Ĭ�ϴӵ�ǰ�ļ�ָ�봦���������ָ�뱻����̸߳ı䣬������
//      ���ݽ��Ǵ���λ�õģ��ʲ����̰߳�ȫ������
//      ʵ���ϣ�����������һ�����ź��������µ�fseek��fread��ϲ�����
//����: fp���������ļ�
//      buf��������������ݵĻ�����
//      size�������ĳߴ磬nmemb����
//      offset���ļ�ƫ����(�ֽ���)
//����: ʵ�ʶ�����������
//ע��: ����˳���fread��һ�������Ǹ�win32api��ReadFile����
//-----------------------------------------------------------------------------
size_t fread_r(FILE *fp,const void *buf,size_t size, size_t offset)
{
    size_t result;
    struct tagPTTDevice *PTT_device_tag;
    struct tagRwPara  param;
    struct tagSeekPara new_pos;
    if( (fp == NULL) || (buf == NULL))
        return false;
    if(size == 0)
        return 0;   //��ȡ����������0
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(fp->file_attr.bits.folder)
    { //Ŀ¼���ܶ�
        result = 0;
        goto goto_end;
    }
    if((fp->open_mode == EN_W_WB)||(fp->open_mode == EN_A_AB))
    { //ֻдģʽ�����ܶ�
        result = 0;
        goto goto_end;
    }
    new_pos.offset = offset;
    new_pos.whence = SEEK_SET;
    result = __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_SEEK,
                                (ptu32_t)&new_pos,(ptu32_t)fp);
    param.nmemb = 1;
    param.size = size;
    param.fp = fp;
    result = __Djyfs_PTT_AppRead(PTT_device_tag,(ptu32_t)buf,(ptu32_t)&param,0);

goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----���ļ�-----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴӴ������ļ������ݣ�����C��׼
//����: buf��������������ݵĻ�����
//      size�������ĳߴ磬nmemb����
//      nmemb��ÿ�����ݳ���(�ֽ���)
//      fp���������ļ�
//����: ʵ�ʶ�������������nmemb����
//-----------------------------------------------------------------------------
size_t fread(const void *buf,size_t size, size_t nmemb,FILE *fp)
{
    size_t result;
    struct tagPTTDevice *PTT_device_tag;
    struct tagRwPara  param;
    if( (fp == NULL) || (buf == NULL))
        return false;
    if((size == 0) || (nmemb == 0))
        return 0;   //��ȡ����������0
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(fp->file_attr.bits.folder)
    { //Ŀ¼���ܶ�
        result = 0;
        goto goto_end;
    }
    if((fp->open_mode == EN_W_WB)||(fp->open_mode == EN_A_AB))
    { //ֻдģʽ�����ܶ�
        result = 0;
        goto goto_end;
    }
    param.nmemb = nmemb;
    param.size = size;
    param.fp = fp;
    result = __Djyfs_PTT_AppRead(PTT_device_tag,(ptu32_t)buf,(ptu32_t)&param,0);

goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----�����ļ�ָ��-------------------------------------------------------------
//����: ���ļ�ָ�����õ�ָ����λ�ã�����ļ���д��ʽ�򿪣�����λ�ô����ļ��ߴ磬
//      �������ļ��������ӡ�
//������fp�����������ļ�ָ��
//      offset���ƶ���λ��
//      whence���Ӻδ���ʼ�����ƶ����ȣ��μ�SEEK_CUR�ȵĶ���
//���أ�0=�ɹ���-1=ʧ��
//-----------------------------------------------------------------------------
u32 fseekL(FILE *fp, s64 offset, s32 whence)
{
    struct tagPTTDevice *PTT_device_tag;
    u32 result;
    struct tagSeekPara new_pos;
    if(fp == NULL)
        return CN_LIMIT_UINT32;
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if( (fp->file_attr.bits.folder)
        ||(fp->open_mode == EN_A_AB))
    {//Ŀ¼û��λ��ָ��,׷��ֻд��ʽ���ܸı�λ��ָ��
        result = CN_LIMIT_UINT32;
        goto goto_end;
    }
    new_pos.offset = offset;
    new_pos.whence = whence;
    result = __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_SEEK,
                                (ptu32_t)&new_pos,(ptu32_t)fp);
goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----�����ļ���ָ��----------------------------------------------------------
//����: ���ļ�ָ�����õ�ָ����λ�ã�����ļ���д��ʽ�򿪣�����λ�ô����ļ��ߴ磬
//      �������ļ��������ӡ�
//������fp�����������ļ�ָ��
//      offset���ƶ���λ��
//      whence���Ӻδ���ʼ�����ƶ����ȣ��μ�SEEK_CUR�ȵĶ���
//���أ�0=�ɹ���-1=ʧ��
//-----------------------------------------------------------------------------
u32 fseek(FILE *fp, long offset, s32 whence)
{
    return fseekL(fp,(s64)offset,whence);
}

//----�̰߳�ȫд�ļ�-----------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڰ�����д��Ӵ������ļ���C��׼��
//      djyfs_fwrite����Ĭ��д�뵱ǰ�ļ�ָ�봦�������ָ�뱻����̸߳ı�,
//      ���ݽ�д�����λ�õģ��ʲ����̰߳�ȫ������
//      ʵ���ϣ�����������һ�����ź��������µ�fseek��fwrite��ϲ�����
//����: fp���������ļ�
//      buf��������������ݵĻ�����
//      size�������ĳߴ磬nmemb����
//      offset���ļ�ƫ����(�ֽ���)
//����: ʵ��д���������
//ע��: ����˳���fwrite��һ�������Ǹ�win32api��WriteFile����
//-----------------------------------------------------------------------------
size_t fwrite_r(FILE *fp,const void *buf,size_t size, size_t offset)
{
    size_t result;
    struct tagPTTDevice *PTT_device_tag;
    struct tagRwPara  param;
    struct tagSeekPara new_pos;
    if( (fp == NULL) || (buf == NULL))
        return 0;
    if(size == 0)
        return 0;   //д�����������0
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(fp->file_attr.bits.folder)
    { //Ŀ¼����д
        result = 0;
        goto goto_end;
    }
    if(fp->open_mode == EN_R_RB)
    {//ֻ���ļ�������д
        result = 0;
        goto goto_end;
    }
    new_pos.offset = offset;
    new_pos.whence = SEEK_SET;
    result = __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_SEEK,
                                (ptu32_t)&new_pos,(ptu32_t)fp);
    param.nmemb = 1;
    param.size = size;
    param.fp = fp;
    result = __Djyfs_PTT_AppWrite(PTT_device_tag,(ptu32_t)buf,
                                (ptu32_t)&param,0);
goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----д�ļ�-------------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڰ�����д��������ļ�
//����: buf���ṩд������ݵĻ�����
//      size��д��ĳߴ磬nmemb����
//      nmemb��ÿ�����ݳ���(�ֽ���)
//      fp����д���ļ�
//����: ʵ��д�����������nmemb����
//-----------------------------------------------------------------------------
size_t fwrite(const void *buf,size_t size, size_t nmemb,FILE *fp)
{
    size_t result;
    struct tagPTTDevice *PTT_device_tag;
    struct tagRwPara  param;
    if( (fp == NULL) || (buf == NULL))
        return 0;
    if((size == 0) || (nmemb == 0))
        return 0;   //д�����������0
    PTT_device_tag = (struct tagPTTDevice *)fp->home_PTT;
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(fp->file_attr.bits.folder)
    { //Ŀ¼����д
        result = 0;
        goto goto_end;
    }
    if(fp->open_mode == EN_R_RB)
    {//ֻ���ļ�������д
        result = 0;
        goto goto_end;
    }
    param.nmemb = nmemb;
    param.size = size;
    param.fp = fp;
    result = __Djyfs_PTT_AppWrite(PTT_device_tag,(ptu32_t)buf,
                                (ptu32_t)&param,0);
goto_end:
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----ɾ���ļ�----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã�����ɾ��һ���ļ���Ŀ¼��
//����: fullname,�ļ���Ŀ¼����
//����: 0=�ɹ�����0=ɾ��ʧ�ܣ����鿴�ļ�ϵͳ��������
//-----------------------------------------------------------------------------
u32 remove(const char *fullname)
{
    u32 result;
    struct tagPTTDevice *PTT_device_tag;
    char *synname;

    if(fullname == NULL)
        return 1;
    if(!__Djyfs_CheckFullName(fullname))
        return 1;
    PTT_device_tag = __Djyfs_OpenPTT_Left(fullname);
    if(PTT_device_tag == NULL)
        return 1;    //���������ڣ���fullname�͵�ǰ·����û��ָ��������
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(PTT_device_tag->formatted)
    {
        synname = __Djyfs_PickSynName(fullname);
        result = __Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_REMOVE,(ptu32_t)synname,0);
        //���÷��������ֿ��ƺ���
        if(result == EN_FS_NO_ERROR)
            result = 0;
    }else
        result = EN_FS_PTT_NO_FORMATED;
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

//----�޸��ļ�(Ŀ¼)����-------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã������޸�һ���ļ���Ŀ¼��������
//����: old_fullname,�ļ���Ŀ¼������
//      new_filename��������
//����: 0=�ɹ�����0 = ʧ��
//-----------------------------------------------------------------------------
u32 rename(const char *old_fullname,const char *new_filename)
{
    u32 result;
    struct tagPTTDevice *PTT_device_tag;
    char *synname;

    if(old_fullname == NULL)
        return 1;
    if(__check_word(new_filename) == false)
        return 1;
    if(!__Djyfs_CheckFullName(old_fullname))
        return 1;
    PTT_device_tag = __Djyfs_OpenPTT_Left(old_fullname);
    if(PTT_device_tag == NULL)
        return false;    //���������ڣ���fullname�͵�ǰ·����û��ָ��������
    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(PTT_device_tag->formatted)
    {
        synname = __Djyfs_PickSynName(old_fullname);
        //���÷��������ֿ��ƺ���
        if(__Djyfs_PTT_AppCtrl(PTT_device_tag,EN_PTT_RENAME,(ptu32_t)synname,
                             (ptu32_t)new_filename) == EN_FS_NO_ERROR)
            result = 0;
        else
            result = 1;
    }else
        result = 1;
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);
    return result;
}

s32 fputs(const char *__restrict buf, FILE *__restrict fp)
{
    return 0;
    #warning ����ɡ�
}

//-----------------------------------------------------------------------------
//����: ���ļ��ж�ȡһ���ַ����ֽڣ������ļ����ɶ����ߴﵽ�ļ�Ϊ������EOF
//����: fp���������ļ�
//���: ��
//����: ��ȡ���ַ�������EOF��
//------------
//���¼�¼:
// 1.����: 2014/12/09
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
s32 fgetc(FILE *fp)
{
    if(fp == NULL)
        return EOF;
    #warning "fgetc��û��ʵ�֡�"
    return 0;

}

//-----------------------------------------------------------------------------
//����: ������ַ�,���ǰ�ԭʼ�ַ����,����ת���ɱ��ض��ֽ��ַ����
//����: wc,������Ŀ��ַ�
//    fp,�����ַ����ļ�.
//���: ��
//����: WEOF����������ַ�
//------------
//���¼�¼:
// 1.����: 2014/12/31
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
wint_t putwc(wchar_t wc, FILE *fp)
{
    char buf[8];    //���mb�ַ���utf8��ֻ��7���ֽڡ�
    wctomb(buf,wc);
    fputs (buf, fp);
    return wc;
#warning "δ�������,�ر���WEOF"
}





