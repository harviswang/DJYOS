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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
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
//����ģ��: djyshģ��
//���ߣ�wjz
//�汾��djysh1.0.0
//�ļ�����: djysh���ļ�ϵͳshell���
//�޶���ʷ:
// 1. ����:2010-6-15
//   ����:wjz
//   �°汾�ţ�djysh1.0.0
//------------------------------------------------------

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stat.h"
#include "char_term.h"
#include "shell.h"
#include "shell_fs.h"
#include "file.h"
#include "flashfile.h"

extern struct tagFileRsc *g_ptWorkPath;
extern uint16_t __Djyfs_IsContainPTT_Name(const char *fullname);
extern bool_t __Djyfs_CheckFullName(const char *fullname);
extern bool_t __Djyfs_PickFileNameWord(const char *synname,char *name);
extern bool_t Sh_PrintWorkPath(void);
//----���ͨ���-----------------------------------------------
//����: ����ַ�����·�������Ƿ����ǺŻ����ʺ�,���ҳ�������������󳤶�Ϊ�ޡ�
//����: str,�����ַ���
//����: true=·��������'*'����'?'��false=û'*'����'?'
//----------------------------------------------------------------------------
bool_t __Shfs_CheckWildCardPath(char *str)
{
    u32 loop,name_len;

    if(str == NULL)
        return false;
    name_len = strlen(str);
    for(loop = 0; loop <= name_len; loop++)
    {
        if((str[loop]=='*')||(str[loop]=='?'))
        {
            break;
        }
    }
    for( ; loop <= name_len; loop++)
    {
        if(str[loop]=='\\')
        {
            return true;
        }
    }
    return false;
}

//----���ͨ���-----------------------------------------------
//����: ����ַ������ļ��������Ƿ����ǺŻ����ʺ�,���ҳ�������������󳤶�Ϊ�ޡ�
//����: str,�����ַ���
//����: true=�ļ���������'*'����'?'��false=û'*'����'?'
//----------------------------------------------------------------------------
bool_t __Shfs_CheckWildCardName(char *str)
{
    u32 name_len;
    s32 loop;
    if(str == NULL)
        return false;
    name_len = (u32)strlen(str);
    if((str[name_len-1] == '\\') || (str[name_len-1] == ':'))
        return false;
    for(loop = name_len-1 ; loop >= 0; loop--)
    {
        if((str[loop]=='*')||(str[loop]=='?'))
        {
            return true;
        }
        if(str[loop]=='\\')
        {
            return false;
        }
    }
    return false;
}

//----���ͨ���-----------------------------------------------
//����: ����ַ����Ƿ����ǺŻ����ʺ�,���ҳ�������������󳤶�Ϊ�ޡ�
//����: str,�����ַ���
//����: true=�ļ���������'*'����'?'��false=û'*'����'?'
//----------------------------------------------------------------------------
bool_t __Shfs_CheckWildCard(char *str)
{
    u32 loop,name_len;

    if(str == NULL)
        return false;
    name_len = strlen(str);
    for( loop = 0; loop <= name_len; loop++)
    {
        if((str[loop]=='*')||(str[loop]=='?'))
        {
            return true;
        }
    }
    return false;
}

//-------��ͨ������ַ�ƥ��-----------------------------------------------------
//����: ��ͨ������ַ�ƥ�������ͨ�����'?'��'*'������,'?'���Դ���һ���ַ�
//        (����û��),'*'���Դ����������ַ�(����Ϊ��)��������'?'�� ����'?'
//        �Ĺ���,'?'��ʾ�����ַ�,Ҳ����˵��ƥ�������,'?'��Զƥ��ɹ� ��
//        ������,'?'��û���޸��㷨���������޸���ƥ����� -- ����'?'��һ��ƥ�� ��
//        Ȼ��'*'��˲�ͬ,'*'��������ƥ���������ַ�����Ȼ���ǲ��ܼ򵥵��޸�ƥ��
//        ���̶�����Ҫ��
//����: src - ��ƥ����ַ���,���ܺ���'?'��'*'
//        wildcard - ͨ����ַ���
//����: true=ƥ��ɹ�,
//      false=ƥ��ʧ��,
//-----------------------------------------------------------------------------
bool_t __Shfs_WildCardMatch(char *src, char *wildcard)
{
    u32 i = 0;
    u32 j = 0;

    if(wildcard == NULL)
        return true;

    while (wildcard[j] || src[i])
    {
        if (wildcard[j] == '*')
        {
            while (wildcard[j] == '*')    // �ҵ�һ����*�ŵ����һ��*��
            {
                j++;
                if (wildcard[j] == '\0')
                {
                    return true;    // ƥ��
                }
            }
            while (src[i] != wildcard[j])    // ƥ��src����*���һ���ַ�
            {
                if (src[i])
                {
                    i++;
                }
                else
                {
                    return false;    // ��ƥ��
                }
            }
        }
        else
        {
            if ((src[i] == wildcard[j]) || (wildcard[j] == '?'))
            {
                i++;
                j++;
            }
            else
            {
                return false;
            }
        }
    }

    if ((src[i] == '\0') && (wildcard[j] == '\0'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//----�ж�'\'�����Ƿ�������-----------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ������жϵ�һ������'\'֮���Ƿ���
//      ����
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�ɹ�, ˵��'\'֮��������,����û��'\'
//      false=ʧ�ܣ�˵��'\'֮��û������
//��ע: pathӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __Shfs_JudgeBackSlashIsData(char *path)
{
    uint32_t index;
    uint32_t name_len;

    name_len = strlen(path);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == '\\')    //�����ַ�'\'
        {
            if(path[index+1] != '\0')
                return true;
            else
                return false;
        }
    }
    //�ַ�����û�з�б��
    if(index == name_len)
        return true;
    else
        return false;
}

//----ɾ��·������ļ�����Ŀ¼-------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�����ɾ�����һ����path��һ����
//      Ŀ¼�����򲻲���
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�ɹ�ɾ����������ɾ��
//      false=ʧ��
//-----------------------------------------------------------------------------
bool_t __Shfs_CutPathEndItem(char *path)
{
    u32 name_len,loop;

    name_len = (u32)strlen(path);
    loop=name_len;
    if(path[name_len-1]=='\\')//�з�б��˵����ȥ���ļ���
    {
        if(path[name_len-2]==':')
            return true;//˵���Ǹ�Ŀ¼�����ò���
        else
        {
            for(loop=name_len-2;loop>0;loop--)
            {
                if(path[loop] == '\\')
                {
                    path[loop+1] = '\0';
                    break;
                }
            }
        }
        return true;
    }
    else//û�з�б�ܣ�˵��ȥ�������ļ�
    {
        for(loop=name_len-1;loop>0;loop--)
        {
            if(path[loop] == '\\')
            {
                path[loop+1] = '\0';
                break;
            }
        }
    }
    return true;
}

//----��ȡ·������ļ�����Ŀ¼--------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�������ȡ���һ����path�Ǹ�Ŀ¼
//      �򲻲�����path�����ֲ��䡣
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//        name,������ȡ���ļ�����Ŀ¼
//����: true=�ɹ�, ��ȡ�ɹ�
//      false=ʧ�ܣ���ȡʧ��
//-----------------------------------------------------------------------------
bool_t __Shfs_PickPathEndItem(char *path,char *name)
{
    u32 name_len,loop;
    char *str;

    name_len = strlen(path);
    loop=name_len;
    if(path[name_len-1]=='\\')//�з�б�ܣ�˵����ȡ�����ļ�����
    {
        if(path[name_len-2]==':')
            return false;   //˵���Ǹ�Ŀ¼�����ò���
        else
        {
            for(loop=name_len-2;loop>0;loop--)
            {
                if(path[loop] == '\\')
                {
                    str=&path[loop+1];
                    strcpy(name,str);
                    return true;
                }
            }
        }
    }
    else//û�з�б�ܣ�˵����ȡ�����ļ���
    {
        for(loop=name_len-1;loop>0;loop--)
        {
            if(path[loop] == '\\')
            {
                str=&path[loop+1];
                strcpy(name,str);
                return true;
            }
        }

    }
    //ִ�е��ⲽ��˵��ֻ��һ���ļ�����Ŀ¼��ֱ�ӿ�����ok
    strcpy(name,path);
    return true;
}

//----�����ļ���---------------------------------------------------------------
//����: ����ַ��������ļ�����������һ��'\\'��ĵ�һ���ַ��ĳ�'\0'���������ļ�
//      ��ָ��
//����: path�������ԵĴ�
//      filename�������ļ�����ַ,�����ַ��Ѿ�����Ϊ����ֵ���أ����б��滻Ϊ'\0'
//      pathname����path���д���Ŀ¼�����򷵻�·������ַ�����򷵻�NULL;
//����: ��'\0'�滻���ַ���'\0'=�����ļ���
//-----------------------------------------------------------------------------
char __Shfs_SeparateFileName(char *path,char **filename,char **pathname)
{
    u32 name_len,loop;
    char ch;
    name_len = strlen(path);
    if(name_len == 0)
        return 0;
    for(loop = 0; loop < name_len; loop++)
    {
        if((path[loop-1] == ':') || (path[loop-1] == '\\'))
            break;
    }
    if(loop == name_len)        //��������һ���ļ���
    {
        *pathname = NULL;
        *filename = path;
        return path[0];
    }
    else if(loop == name_len -1)    //�����ļ���
    {
        *filename = NULL;
        *pathname = path;
        return 0;
    }
    else
    {
        *filename = path + loop;
        *pathname = path;
        ch = path[loop];
        path[loop] = '\0';
        return ch;
    }
}

//----�ж��ַ����Ǿ���·���������·��-----------------------------------------
//����: ���һ�����ܰ����༶·�������ļ������ַ����Ƿ�����������ľ���·��
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�Ǿ���·��
//      false=���Ǿ���·��
//��ע: pathӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __Shfs_PathIsAbsolutePath(char *path)
{
    uint32_t index;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    uint32_t name_len;

    name_len = strlen(path);
    if(path[0] == '\\')
        return true;
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')
        {
            return true;
        }
    }
        return false;
}

//----�ж�·���Ƿ��������---------------------------------------------------
//����: �ж�һ�����ܰ����༶·�������ļ������ַ����Ƿ����������
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=��������
//      false=����������
//-----------------------------------------------------------------------------
bool_t __Shfs_PathIncludeDbxName(char *path)
{
    uint32_t index;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    uint32_t name_len;

    name_len = strlen(path);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')
        {
            return true;
        }
    }
        return false;
}

//----Ŀ¼��ƥ����ļ���ȡ��һ���ַ���--------------------------------------
//����: ��һ��Ŀ¼��ɨ����ڵ��ļ���Ȼ��ÿ���ļ�ͨ��ƥ�亯����wildcard_param����
//        һһƥ�䣬��ƥ����ļ����ᵽ��һ���ַ����
//����: wildcard_param������ͨ����Ĳ���
//        get_merge_str ��������Ҫ����ļ����ϲ����ɵ��ַ���
//����: true=�ɹ�, ��ȡ�ɹ�
//      false=ʧ�ܣ���ȡʧ��
//-----------------------------------------------------------------------------
char *ShFs_GetFolderWildcardStr(char *wildcard_param,char *get_merge_str)
{
    struct tagFileRsc *fp = NULL,*current_fp = NULL;
    char work_path[256];

    if(__Djyfs_IsContainPTT_Name(wildcard_param) ==0)
        fp = g_ptWorkPath;//���·����ֱ��ʹ�ù���·��
    else
    {
        strcpy(work_path,wildcard_param);
        __Djyfs_PickFileNameWord(work_path,wildcard_param);
        __Shfs_CutPathEndItem(work_path);
        fp = fopen(work_path, "r");
    }
    //���ļ���û���壨ͬʱfp2�ǷǸ�Ŀ¼,��Ŀ¼�£��ļ����Բ�ȷ����
    if(fp->file_attr.bits.archive&&(fp->file_medium_tag!=0))
        return NULL;
    current_fp =(struct tagFileRsc *)Djyfs_FolderTraversalSon(fp,current_fp);
    if(current_fp == NULL)
    {
        fclose(fp);
        printf("��Ŀ¼Ϊ��!\r\n");
        return NULL;
    }

        //ȫ����0�������Ϳ���׼ȷ֪������merge_str�Ѿ�û��������
    memset(get_merge_str,0,sizeof(get_merge_str));
    do//ѭ����Ŀ¼��ֱ������
    {
        if(__Shfs_WildCardMatch(current_fp->name,wildcard_param))
        {//��ƥ��ɹ���Ŀ¼����һ�����ַ���
            strcat(get_merge_str,current_fp->name);
            strcat(get_merge_str," ");
        }
        //��fp�����е��ļ�����ɾ���ˣ���û�ļ�������false
        current_fp = (struct tagFileRsc *)Djyfs_FolderTraversalSon(fp,current_fp);
        if(current_fp == NULL)
            break;
    }while(current_fp != NULL);
    fclose(fp);

    return get_merge_str;
}

//----��ͨ���ɾ���ļ�---------------------------------------------------------
//����: ��ͨ���ɾ���ļ���·�����ֲ��ܴ�ͨ�������Ŀ¼Ҳһ��ɾ����
//����:
//����: true=�ɹ�, ��ȡ�ɹ�
//      false=ʧ�ܣ���ȡʧ��
//-----------------------------------------------------------------------------
bool_t ShFs_DelWildcardFiles(char *wildcard_param)
{
    char *filename,*pathname;
    char ch;
    struct tagFileRsc *current=NULL,*parent;

    if(wildcard_param == NULL)
    {
        printf("�ļ�������ȷ!\r\n");
        return false;
    }
    ch = __Shfs_SeparateFileName(wildcard_param,&filename,&pathname);
    if(filename == NULL)     //�����ļ���
    {
        printf("�ļ�������ȷ!\r\n");
        return false;
    }

    if(pathname != NULL)
    {
        if(stat(pathname,NULL) != -1)
        {
            parent = fopen(pathname,"r");
        }
        else
        {
            printf("·��������\n\r");
            return false;
        }
    }
    else
    {
        parent = g_ptWorkPath;
    }
    if(filename != NULL)
        filename[0] = ch;       //�ָ��ļ����ĵ�һ���ַ�
    while(1)                //ѭ����Ŀ¼��ֱ������
    {
        current =(struct tagFileRsc *)Djyfs_FolderTraversalSon(parent,current);
        if(current == NULL)
            break;
        if(__Shfs_WildCardMatch(current->name,filename))
        {
            remove(current->name);
        }
    }

    fclose(parent);
    return true;
}

//----���ַ���ĩβ�޷�б�ܼ��Ϸ�б��--------------------------------------------------------------
//���ܣ����ַ���ĩβ�޷�б�ܼ��Ϸ�б��,�з�б�ܾͲ�Ҫ����
//������str �ַ������κ���ʽ
//���أ�true �ɹ�
//        false ����".."��
//------------------------------------------------------------------------------
bool_t ShFs_StrEndIncreaseBackSlash(char *str)
{
    s32 name_len;

    if(strcmp(str,"..") ==0)//�����"..",��ô��Ҫ����
        return false;
    name_len = strnlen(str,CN_SHELL_CMD_LIMIT +1);
    if(name_len >CN_SHELL_CMD_LIMIT)
        return false;
    //����ַ������һλ����'\\'����ô�ӷ�б��
    if(str[name_len-1] != '\\')
    {
        str[name_len] = '\\';
        //���str���ǹ涨�õĿռ䣬������й¶�������ֹ
           str[name_len+1] = '\0';
    }
    return true;
}

//----���ַ���ĩβ�з�б��ȥ����б��--------------------------------------------
//���ܣ����ַ���ĩβ�޷�б�ܼ��Ϸ�б�ܣ�û�з�б�ܾͲ�����
//������str �ַ������κ���ʽ
//���أ�true �ɹ�
//        false �ַ���û�з�б��
//------------------------------------------------------------------------------
bool_t ShFs_StrEndWipeBackSlash(char *str)
{
    s32 name_len;

    name_len = strnlen(str,CN_SHELL_CMD_LIMIT +1);
    if(name_len >CN_SHELL_CMD_LIMIT)
        return false;
    if(str[name_len-1] == '\\')
    {
        str[name_len-1] = '\0';
        return true;
    }
    else
    return false;

}

//----��ʾĿ¼�µ��ļ����ļ���------------------------------------------------
//����: ɨ���Ŀ¼�µ��ļ����ļ��У�Ȼ����һ������ʽ��ȫ����ӡ����
//����: ancestor_fp������ʾ���ļ���
//����: true=�ɹ�, �г��ɹ�
//      false=ʧ�ܣ��г�ʧ��
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: δ���
//   ����: ������
//-----------------------------------------------------------------------------
bool_t ShFs_DirItemSon(struct tagFileRsc *parent_fp)
{
    struct tagFileRsc *current_fp;
    s64 files_size=0;
    u32 index=0;
    uint32_t files_items=0,folder_items=0;
    s64 available_PTT_size=0;
    char *buf;

    if(parent_fp == NULL)
        return false;//�����Ƿ���δ����

    index = Djyfs_GetPathLen(parent_fp);
    buf = M_MallocLc(index,0);
    if(buf == NULL)
    {
        printf("�ڴ治�㣬����ִ��dir");
        return false;
    }
    buf[0] = '\0';
    Djyfs_GetPath(parent_fp,buf,index+1);
    printf("%s ��Ŀ¼\r\n\r\n",buf);
    free(buf);
    Djyfs_PTT_StatFp(parent_fp,NULL,NULL,&available_PTT_size);

    current_fp = NULL;
    current_fp = (struct tagFileRsc*)Djyfs_FolderTraversalSon(parent_fp,current_fp);
    if(current_fp == NULL)
    {
        printf("��Ŀ¼Ϊ��!\r\n");
        return false;
    }

    do //ѭ����Ŀ¼��ֱ������
    {
#if 0
        printf("%d/%02d/%02d %02d:%02d",
                (current_fp->year_high_modify<<8) + current_fp->year_low_modify,
                current_fp->month_modify,
                current_fp->date_modify,
                current_fp->hour_modify,
                current_fp->minute_modify);
#endif
        if(current_fp->file_attr.bits.folder)
        {
            printf(" <DIR> ");//������Ŀ¼�����Լ���dir
            folder_items++;
        }
        else
        {
            printf("       ");
            files_items++;
        }
        printf("%9d %s\r\n",
                (s32)current_fp->file_size, current_fp->name);
        files_size+=current_fp->file_size;
        current_fp = (struct tagFileRsc*)Djyfs_FolderTraversalSon(parent_fp,current_fp);
    }while(current_fp != NULL);

    printf("%d���ļ� %d�ֽ�\r\n", files_items, (s32)files_size);
    printf("%d��Ŀ¼ %d�����ֽ�\r\n", folder_items, (s32)available_PTT_size);
    return true;
}

//----��ʾ����ͨ������ļ������ļ���------------------------------------------------
//����: dir�����£���ʾ����ͨ������ļ������ļ��С�ɨ�赱ǰ����·������ָ��·����
//        ���ļ�����Ŀ¼��Ȼ����ƥ�亯��һһƥ�䡣����Ҫ��ģ���ʾ������
//����: wildcard_param������ͨ������ַ���
//����: true=�ɹ�, �г��ɹ�
//      false=ʧ�ܣ��г�ʧ��
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: δ���
//   ����: ������
//-----------------------------------------------------------------------------
bool_t ShFs_DirWildcardFiles(char *wildcard_param)
{
    char *filename,*pathname;
    uint32_t files_items=0,folder_items=0;
    s64 files_size=0,available_PTT_size=0;
    char ch;
    struct tagFileRsc *current=NULL,*parent;
//    struct tagFileRsc *bak_work_path = g_ptWorkPath;

    if(wildcard_param == NULL)
    {
        printf("�ļ�������ȷ!\r\n");
        return false;
    }
    ch = __Shfs_SeparateFileName(wildcard_param,&filename,&pathname);

    if(pathname != NULL)
    {
        if(stat(pathname,NULL) != -1)
        {
            parent = fopen(pathname,"r");
        }
        else
        {
            printf("·��������\n\r");
            return false;
        }
    }
    else
    {
        parent = g_ptWorkPath;
    }
    if(filename != NULL)
        filename[0] = ch;       //�ָ��ļ����ĵ�һ���ַ�

    while(1)                //ѭ����Ŀ¼��ֱ������
    {
        current =(struct tagFileRsc *)Djyfs_FolderTraversalSon(parent,current);
        if(current == NULL)
            break;
        if(__Shfs_WildCardMatch(current->name,filename))
        {
#if 0  //�ļ�ʱ���ʽ�ı�
            printf("%d/%02d/%02d %02d:%02d",
                    (current->year_high_modify<<8) + current->year_low_modify,
                    current->month_modify,
                    current->date_modify,
                    current->hour_modify,
                    current->minute_modify);
#endif
            if(current->file_attr.bits.folder)
            {
                printf(" <DIR> ");
                folder_items++;
            }
            else
            {
                printf("       ");
                files_items++;
            }
            printf("%9dB %s\r\n", (s32)current->file_size, current->name);
            files_size += current->file_size;
        }
    }

    Djyfs_PTT_StatFp(parent,NULL,NULL,&available_PTT_size);
    fclose(parent);
    printf("%d���ļ� %d�ֽ�\r\n", files_items, (s32)files_size);
    printf("%d��Ŀ¼ %d�����ֽ�\r\n", folder_items, (s32)available_PTT_size);
    return true;
}



//----�ж�һ��Ŀ¼�Ƿ����ļ���------------------------------------------------
//����: ͨ������fp�����ԣ��ж��Ƿ����ļ���
//����: fullname,·�� ��ʽ:"C:\aa\bb\cc"
//����: CN_ITEM_IS_FOLDER=���ļ���,
//      CN_ITEM_IS_FILE=���ļ���
//      CN_ITEM_NO_EXIST=�������������ļ����ļ���
//ע:   ����ʱ��ȷ��fullname�ǺϷ����ַ���
//-----------------------------------------------------------------------------
u32 __Shfs_JudgeItemAttr(char *fullname)
{
    struct stat FileStat;
    bool_t result;
    result = stat(fullname,&FileStat);
    if(result != -1)
    {
           if(FileStat.st_mode.bits.folder)
        {
            return CN_ITEM_IS_FOLDER;
        }
        else
        {
            return CN_ITEM_IS_FILE;
        }
    }
    else
        return CN_ITEM_NO_EXIST;
}

//----����һ���ļ���------------------------------------------------
//����: ָ������һ���ļ���
//����: md_param�������ַ�
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t mkdir(char *md_param)
{
    struct tagFileRsc *fp;

    if(md_param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }
    //�ڴ�����ĩβ����'\'��������Σ�����Ŀ¼
    //���ﲻ���ڴ�й©����Ϊmd������涨�˳���
    ShFs_StrEndIncreaseBackSlash(md_param);
    if(!__Djyfs_CheckFullName(md_param))
         return false;
    //����д����ļ�����Ŀ¼����ô�Ͳ���Ҫ������
    if(stat(md_param,NULL) == -1)
    {
        fp = fopen(md_param,"w+");
        if(fp == NULL)
            return false;
        fclose(fp);
    }
    else
    {
        printf("\r\n��Ŀ¼�����ļ�%d�Ѿ�����\r\n", md_param);

        return false;
    }
    return true;
}
//----ɾ���ļ���--------------------------------------------------------------
//����: ��ָ�����ļ���ɾ����
//����: rd_param�������ַ�
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t ShFs_RemoveFolder(char *rd_param)
{
    struct tagFileRsc *fp;

    if(rd_param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }
    if(!__Djyfs_CheckFullName(rd_param))
         return false;
    fp = fopen(rd_param,"r");
    if(fp == NULL)
    {
        printf("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        return false;
    }
    if(fp->file_attr.bits.folder)
    {
        fclose(fp);
        if(remove(rd_param) == EN_FS_FOLDER_UNBLANK)
        {
            printf("Ŀ¼%s���ǿգ�\r\n", rd_param);
        }
        return true;
    }
    else
    {
        fclose(fp);
        printf("Ŀ¼������Ч!\r\n");
        return false;
    }

}

//----��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�-------------------------------------------------------
//���ܣ���ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�
//������dir_param��dir����Ĳ������������ļ���·�����߲�����
//���أ�true����ʾ�ɹ�
//        false,��ʾʧ��
//-----------------------------------------------------------------------------
bool_t Shfs_DirParent(char *next_param)
{
    char *foldername=NULL;
    struct tagFileRsc *fp;
    uint32_t lenth=0;
    char work_path[CN_SHELL_CMD_LIMIT+1];

    //�������ʹ��get_work_path�ˡ����棬dir ..\folder����Ҫ                                                        ->private_tag);
    Djyfs_GetCwd(work_path,CN_SHELL_CMD_LIMIT);
    do
    {
        foldername = (char *)Sh_GetItem(next_param,&next_param);
        if(foldername == NULL&&lenth==0)
            return false;
        //ִ�� ��: cd ..\foder
        if(strcmp(foldername,"..") != 0)
        {
            ShFs_StrEndIncreaseBackSlash(work_path);
            strcat(work_path,foldername);
            fp = fopen(work_path,"r");
            if(fp == NULL)
            {
                printf("ϵͳ�Ҳ���ָ����·��!\r\n");
                return false;
            }
            ShFs_DirItemSon(fp);
            fclose(fp);
            break;
        }
        __Shfs_CutPathEndItem(work_path);
        if(next_param == NULL)
        {
            fp = fopen(work_path,"r");
            ShFs_DirItemSon(fp);
            fclose(fp);
            break;
        }
        //·����ȴ���cn_path_depth_limit��������ѭ���������
        //��Ϊ�˷�ֹ��ѭ������һ��������
        lenth++;
     }while((lenth!=CN_PATH_DEPTH_LIMIT)&&(foldername != NULL));
    return true;
}

//----���ĵ�ǰ����·������Ŀ¼-------------------------------------------------------
//���ܣ����ĵ�ǰ����·����ԭ����ǰ����·���ĸ�Ŀ¼
//������next_param������Ĳ���
//���أ�true����ʾ�ɹ�
//        false,��ʾʧ��
//-----------------------------------------------------------------------------

bool_t ShFs_CD_Parent(char *next_param)
{
    char *foldername=NULL;
    struct tagFileRsc *fp;
    struct tagFileRsc *root,*temp;
    uint32_t lenth=0;

    root = (struct tagFileRsc*)Rsc_GetTree(&g_ptWorkPath->file_node);
    do
    {
        foldername = Sh_GetItem(next_param,&next_param);
        //ִ�� ��: cd ..\foder
        if(strcmp(foldername,"..") != 0)
        {
            //��Ŀ¼���ܴ򿪣����ÿ��Ǹ�Ŀ¼�������
            //����"cd ..\foder",�ڸ�Ŀ¼��ִ�У���û���塣
            //��ֻ����ʽ�򿪣���������ڣ��ͷ���NULL��
            //�Ͳ���ȥר��������Ŀ¼�����Ŀ¼�Ƿ���ڡ�
            if(__Shfs_JudgeItemAttr(foldername) == CN_ITEM_IS_FOLDER)
            {
                fp = g_ptWorkPath;
                g_ptWorkPath = fopen(foldername,"r");
                fclose(fp);
                return true;
            }
            else
            {
                printf("ϵͳ�Ҳ���ָ��·��\r\n");
                return false;
            }
            lenth++;
        }
        temp = (struct tagFileRsc*)Rsc_GetParent(&g_ptWorkPath->file_node);
        if(temp != root)
        {
            fp=g_ptWorkPath;        //��ǰ����·�����ܹر�,��ת��
            g_ptWorkPath = temp;
            fclose(fp);
        }
        else
            break;
        //·����ȴ���cn_path_depth_limit��������ѭ���������
        //��Ϊ�˷�ֹ��ѭ������һ��������
     }while((lenth <= CN_PATH_DEPTH_LIMIT)&&(next_param != NULL));
    return true;
}


//----��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�-------------------------------------------------------
//���ܣ���ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�
//������dir_param��dir����Ĳ������������ļ���·�����߲�����
//���أ�true����ʾ�ɹ�
//        false,��ʾʧ��
//-----------------------------------------------------------------------------

bool_t ShFs_CD_PTT(char *PTT_Name)
{
    char work_path[CN_SHELL_CMD_LIMIT+1];
    struct tagFileRsc *fp;

    strcpy(work_path,PTT_Name);
    strcat(work_path,"\\");
    if(stat(work_path,NULL) != -1)
    {
        fp = fopen(work_path,"r");
        if(fp != NULL)
        {
            g_ptWorkPath = fp;
            return true;
        }
        else
            return false;
    }
    else
    {
        printf("�÷���������\r\n");
        return false;
    }
}

//���ƴ���̫�࣬���������!������ĥ��ƽ���!��֮��������һ����bug��

//----����һ���ļ���Ŀ��------------------------------------------------
//����: ��һ���ļ����Ƶ���Ҫ�����Ƶĵط�
//����:  src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//        �������޸ġ�
//        dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t Shfs_CopyFileToDest(char *src,char *dest)
{
    uint32_t file_size,loop,loop_times,buf_size;
    uint32_t buf_size_offset,size;
    struct tagFileRsc *src_fp;
    struct tagFileRsc *dest_fp;
    char* buf;

    src_fp = fopen(src,"r");
    if(src_fp == NULL)
        return false;
    file_size = src_fp->file_size;
    fclose(src_fp);
    ////����16k�ڴ�,��ֹ�ڴ治��
    //������ڴ治��̫С�������ļ��󣬲��������࣬Ӱ��copy���ٶ�
    //̫�����벻�˾��군�ˣ���ʱ��Ϊ16k�����һ��flash��Ĵ�С
    //��Ƶ������֮����ȷ��
    buf_size = 16384;
    loop_times =(file_size+buf_size-1)/buf_size;
    if(loop_times==0)
    {//loop_times==0ʱ��˵���ļ���СΪ0.ֻ��Ҫ��������ok
        dest_fp = fopen(dest,"a+");
        fclose(dest_fp);
        return true;
    }
    buf = (char *)M_MallocLc(file_size,0);
    if(buf == NULL)
        return false;
    buf_size_offset = file_size%buf_size;
    if(buf_size_offset == 0)
        buf_size_offset = buf_size;
    src_fp = fopen(src,"r");
    dest_fp = fopen(dest,"a+");
    for(loop=0;loop<loop_times;loop++)
    {
        if(loop!=loop_times-1)
            size = buf_size;
        else//д�������С��������ڴ棬��Ӧ��д��buf_size�Ĵ�С
            size = buf_size_offset;
        fseek(src_fp, (sint64_t)(size*loop), SEEK_SET);
           fread(buf, (size_t)size, 1,src_fp);
        fwrite((void *)buf,(size_t)size,1,dest_fp);
    }
    fclose(src_fp);
    fclose(dest_fp);
    free(buf);
    return true;
}
//----�ƶ�һ��Ŀ¼�µ��ļ���Ŀ��------------------------------------------------
//����: ��һ��Ŀ¼���ļ��ƶ�����Ҫ���ƶ��ĵط�
//����: src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//        �������޸ġ�
//        dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t Shfs_CopyFileOfFolderToDest(char *src,char *dest)
{

    uint32_t file_nunmber=0;
    struct tagFileRsc *dest_fp,*src_fp,*current_fp=NULL;
    if(stat(dest,NULL) == -1)
    {
        printf("djyos��֧�ִ˲���,�����뿴help!\r\n");
        return false;
    }
    dest_fp = fopen(dest,"r");
    if(dest_fp == NULL)
        return false;
    //djyfs��֧�ֶ���ļ��ϲ�д�뵽һ���ļ���
    if(dest_fp->file_attr.bits.archive)
    {
        printf("djyos��֧�ִ˲���,�����뿴help!\r\n");
        fclose(dest_fp);
        return false;
    }
    fclose(dest_fp);

    src_fp = fopen(src,"r");
    if(src_fp == NULL)
    {
        fclose(dest_fp);
        return false;
    }
    current_fp = (struct tagFileRsc *)Djyfs_FolderTraversalSon(src_fp,current_fp);
    //����ѭ��һ�ξ͹رգ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
       fclose(src_fp);
    if(current_fp == NULL)
    {
        printf("%s*\r\n", src);
        printf("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        printf("�Ѿ�������            0���ļ�\r\n");//��ʱ����
        return false;  //parent��һ����Ŀ¼
    }

    do
    {

        if(current_fp->file_attr.bits.archive)
        {
            strcat(src,current_fp->name);
            strcat(dest,current_fp->name);
            Shfs_CopyFileToDest(src,dest);
            printf("%s\r\n", src);
            __Shfs_CutPathEndItem(src);
            __Shfs_CutPathEndItem(dest);
            file_nunmber++;
        }
        src_fp = fopen(src,"r");
        if(src_fp == NULL)
        {
            fclose(dest_fp);
            return false;
        }
        //����ѭ��һ�ξ͹ر�һ�Σ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
           fclose(src_fp);
        current_fp = (struct tagFileRsc *)Djyfs_FolderTraversalSon(src_fp,current_fp);
    } while(current_fp != NULL);
        printf("�Ѿ�������            %d���ļ�\r\n", file_nunmber);
        return true;
}

//���ƴ���̫�࣬���������!������ĥ��ƽ���!��֮��������һ����bug��

//----�ƶ�һ���ļ���Ŀ��------------------------------------------------
//����: ��һ���ļ��ƶ�����Ҫ���ƶ��ĵط�
//����:  src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//        �������޸ġ�
//        dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t Shfs_MoveFileToDest(char *src,char *dest)
{
    uint32_t file_size,loop,loop_times,buf_size;
    uint32_t buf_size_offset,size;
    struct tagFileRsc *src_fp;
    struct tagFileRsc *dest_fp;
    char* buf;

    src_fp = fopen(src,"r");
    if(src_fp == NULL)
        return false;
    file_size = src_fp->file_size;
    fclose(src_fp);
    ////����16k�ڴ�,��ֹ�ڴ治��
    //������ڴ治��̫С�������ļ��󣬲��������࣬Ӱ��copy���ٶ�
    //̫�����벻�˾��군�ˣ���ʱ��Ϊ16k�����һ��flash��Ĵ�С
    //��Ƶ������֮����ȷ��
    buf_size = 16384;
    loop_times =(file_size+buf_size-1)/buf_size;
    if(loop_times == 0)
    {
        dest_fp = fopen(dest,"a+");
        fclose(dest_fp);
        remove(src);
    }
    buf = (char *)M_MallocLc(file_size,0);
    if(buf == NULL)
        return false;
    buf_size_offset = file_size%buf_size;
    if(buf_size_offset == 0)
        buf_size_offset = buf_size;
    src_fp = fopen(src,"r");
    dest_fp = fopen(dest,"a+");
    for(loop=0;loop<loop_times;loop++)
    {
        if(loop!=loop_times-1)
            size = buf_size;
        else//д�������С��������ڴ棬��Ӧ��д��buf_size�Ĵ�С
            size = buf_size_offset;
        fseek(src_fp, (sint64_t)(size*loop), SEEK_SET);
           fread(buf, (size_t)size, 1,src_fp);
        fwrite((void *)buf,(size_t)size,1,dest_fp);
    }
    fclose(src_fp);
    fclose(dest_fp);
    remove(src);
    free(buf);

    return true;
}

//----�ƶ�һ��Ŀ¼�µ��ļ���Ŀ��------------------------------------------------
//����: ��һ��Ŀ¼���ļ��ƶ�����Ҫ���ƶ��ĵط�
//����: src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//        �������޸ġ�
//        dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t Shfs_MoveFileOfFolderToDest(char *src,char *dest)
{

    uint32_t file_nunmber=0,fat_block_no=0xffffffff;
    struct tagFileRsc *dest_fp,*src_fp,*current_fp=NULL;
    if(stat(dest,NULL) == -1)
    {
        printf("djyos��֧�ִ˲���,�����뿴help!\r\n");
        return false;
    }
    dest_fp = fopen(dest,"r");
    if(dest_fp == NULL)
        return false;
    //djyfs��֧�ֶ���ļ��ϲ�д�뵽һ���ļ���
    if(dest_fp->file_attr.bits.archive)
    {
        printf("djyos��֧�ִ˲���,�����뿴help!\r\n");
        fclose(dest_fp);
        return false;
    }
    fclose(dest_fp);

    src_fp = fopen(src,"r");
    if(src_fp == NULL)
    {
        fclose(dest_fp);
        return false;
    }
    current_fp =(struct tagFileRsc *) Djyfs_FolderTraversalSon(src_fp,current_fp);
    //����ѭ��һ�ξ͹رգ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
       fclose(src_fp);
    if(current_fp == NULL)
    {
        printf("%s*\r\n", src);
        printf("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        printf("�Ѿ��ƶ���            0���ļ�\r\n");//��ʱ����
        return false;  //parent��һ����Ŀ¼
    }
    //strcpy(name,current_fp->name);
    do
    {

        if(current_fp->file_attr.bits.archive)
        {
            strcat(src,current_fp->name);
            strcat(dest,current_fp->name);
            Shfs_MoveFileToDest(src,dest);
            printf("%s\r\n", src);
            __Shfs_CutPathEndItem(src);
            __Shfs_CutPathEndItem(dest);
            file_nunmber++;
        }
        else if(!current_fp->file_attr.bits.archive)
        {
            if(fat_block_no==0xffffffff)//��һ���Ļ��������child
                fat_block_no = current_fp->file_medium_tag;
        }
        src_fp = fopen(src,"r");
        if(src_fp == NULL)
        {
            fclose(dest_fp);
            return false;
        }
        //����ѭ��һ�ξ͹ر�һ�Σ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
           fclose(src_fp);
        current_fp = (struct tagFileRsc *)Djyfs_FolderTraversalSon(src_fp,current_fp);
        if(current_fp ==NULL)
            break;
        if(current_fp->file_attr.bits.archive)
        {
           continue;
        }
        if(fat_block_no==current_fp->file_medium_tag)
            break;
    } while(current_fp !=NULL);
        printf("�Ѿ��ƶ���            %d���ļ�\r\n", file_nunmber);
        return true;
}

//----��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�-----------------------------------------
//���ܣ���ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�
//������dir_param��dir����Ĳ������������ļ�
//���أ�true����ʾ�ɹ�
//        false,��ʾʧ��
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: δ���
//   ����: ������
//-----------------------------------------------------------------------------

bool_t Shfs_Dir(char *dir_param)
{
    struct tagFileRsc *fp;
    s64 size;
    if(dir_param == NULL)
        return false;
    if(__Shfs_CheckWildCardPath(dir_param))     //·�����ֲ������ͨ���
    {
        printf("%s �ļ�������ȷ!\r\n",dir_param);
        return true;
    }
    else if(__Shfs_CheckWildCardName(dir_param))   //�ļ�����ͨ���
    {
        ShFs_DirWildcardFiles(dir_param);
        return true;
    }

    if(strcmp(dir_param,"\\")==0)//ִ�� dir [\]
    {
        fp = Djyfs_GetRootFolder(g_ptWorkPath);
        ShFs_DirItemSon(fp);
        return true;
    }
    if(dir_param[0]=='.' && dir_param[1]=='.')//ִ��dir    [..]
    {
        if(Shfs_DirParent(dir_param))
            return true;
        else
            return false;
    }
    if((dir_param[0] == '.') && (dir_param[1] == '\0'))
    {
        ShFs_DirItemSon(g_ptWorkPath);
        return true;
    }

    if(!__Djyfs_CheckFullName(dir_param))
    {
        printf("·����ʽ����ȷ!\r\n");
            return false;
    }
    if(__Shfs_JudgeItemAttr(dir_param) != CN_ITEM_IS_FOLDER)
    {
        printf("ϵͳ�Ҳ���ָ����·��\r\n");
        return false;
    }
    if(stat(dir_param,NULL) != -1)
    {
        fp = fopen(dir_param,"r");
        if(fp->file_attr.bits.folder)
            ShFs_DirItemSon(fp);
        else
        {
            Djyfs_PTT_StatFp(fp,NULL,NULL,&size);
            printf("%s ��Ŀ¼\r\n\r\n",dir_param);
#if 0  //�ļ�ʱ���ʽ�ı�
            printf("%d/%02d/%02d %02d:%02d",
                    (fp->year_high_modify<<8) + fp->year_low_modify,
                    fp->month_modify,
                    fp->date_modify,
                    fp->hour_modify,
                    fp->minute_modify);
#endif
            printf("       ");
            printf("%9dB %s\r\n", (s32)fp->file_size, fp->name);
            printf("1���ļ� %d�ֽ�\r\n", (s32)fp->file_size);
            printf("0��Ŀ¼ %d�����ֽ�\r\n", (s32)size);
        }
        fclose(fp);
        return true;
    }
    else
    {
        printf("�Ҳ����ļ�\r\n");
        return false;
    }
}
//----�г������ڵĿռ����--------------------------------------------------
//���ܣ��ѷ����Ĵ�С����Ч�ռ䣬���д�С���г���
//������PTT_name ������
//���أ�true �ɹ�
//------------
//���¼�¼:
// 1.����: 2015/3/25
//   ˵��: ���ĺ���ʵ��
//   ����: ������
//-----------------------------------------------------------------------------
bool_t ShFs_Chkptt(char *PTT_name)
{
    s64 sum_size;
    s64 valid_size;
    s64 free_size;

    if(PTT_name == NULL)
    {
        printf("����: ��������Ϊ��!\r\n");
        return false;
    }
    if(Djyfs_PTT_Stat(PTT_name,&sum_size,&valid_size,&free_size))
    {
        printf("����%sռ���ܿռ䣺%d �ֽ�\r\n", PTT_name, (s32)sum_size);
        printf("����%s��Ч�ռ䣺%d �ֽ�\r\n", PTT_name, (s32)valid_size);
        printf("����%s���пռ䣺%d �ֽ�\r\n", PTT_name, (s32)free_size);
    }
    else
    {
        printf("����������ȷ��δ��ʽ��!\r\n");
    }
     return true;
}


//----������һ���ļ����Ƶ�����һ��λ��--------------------------------------------------
//���ܣ�������һ���ļ����Ƶ�����һ��λ��
//������PTT_name ������
//���أ�true �ɹ�
//-----------------------------------------------------------------------------
bool_t ShFs_Copy(char *param)
{
    char *next_param,source[255],destination[255];
    struct tagFileRsc *fp,*second_fp;
    char s[255];
    if(param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }
    strcpy(source,Sh_GetWord(param,&next_param));

    if(NULL == next_param)
    {
        printf("djyfs��֧�ִ˲����������뿴help\r\n");
        return false;
    }
    strcpy(destination,Sh_GetWord(next_param,&next_param));
    if((!__Djyfs_CheckFullName(source))||(!__Djyfs_CheckFullName(destination)))
        return false;
    if(stat(source,NULL) == -1)
    {
        printf("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        return false;
    }
    //����������ļ�һ�������أ�����������ΪЧ��һ��
    if(strcmp(source,destination)==0)
        return true;
    if(stat(destination,NULL) != -1)
    {
        second_fp = fopen(destination,"r");
        if(second_fp == NULL)
        {
            return false;
        }
        if(second_fp->file_attr.bits.archive)
        {
            printf("�Ƿ񸲸�%s�� <yes/no>", destination);
            fclose(second_fp);
            while(1)
            {
                EasyScanf(NULL,s);
                if(s[0] == 'y'||s[0] == 'Y')
                {
                    Djy_PutChar(s[0]);
                    printf("\r\n");
                    remove(destination);//Ϊ������׼��
                    break;
                }
                else if(s[0] == 'n'||s[0] == 'N')
                {
                    Djy_PutChar(s[0]);
                    printf("\r\n");
                    return false;
                }
            }
        }
        else
        {
            //��Ŀ¼����Ҫ���ӷ�б�ܣ��Ա����濽��·��ʹ��
            ShFs_StrEndIncreaseBackSlash(destination);
            fp = fopen(source,"r");
            if(fp == NULL)
            {
                fclose(second_fp);
                return false;
            }
            if(fp->file_attr.bits.archive)
            {//���source_file���ļ������ļ������뵽second_work_pathĩβ
            //ʹ����������������ṩ׼ȷ��·����
                ShFs_StrEndWipeBackSlash(source);
                __Djyfs_PickFileNameWord(source,s);
                strcat(destination,s);
             }
                fclose(second_fp);
                fclose(fp);
        }
    }
    else//���destination�����ڣ���ôȫ�������ļ�����
        ShFs_StrEndWipeBackSlash(source);//���Բ��üӷ�б�ܰ�?
    fp = fopen(source,"r");
    if(fp == NULL)
        return 0;
    if(!fp->file_attr.bits.folder)//source��һ���ļ�
    {
        fclose(fp);
        Shfs_CopyFileToDest(source,destination);
        printf("�Ѿ�������      1���ļ�\r\n");
    }
    else//source��һ��Ŀ¼
    {
        fclose(fp);
        ShFs_StrEndIncreaseBackSlash(source);
        Shfs_CopyFileOfFolderToDest(source,destination);
    }
    return true;
}


//----������һ���ļ��ƶ�������һ��λ��--------------------------------------------------
//���ܣ�������һ���ļ��ƶ�������һ��λ��
//������PTT_name ������
//���أ�true �ɹ�
//-----------------------------------------------------------------------------
bool_t ShFs_Move(char *param)
{
    char *next_param,source[255],destination[255];
    struct tagFileRsc *fp,*second_fp;
    char s[255];
    if(param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }
    strcpy(source,Sh_GetWord(param,&next_param));

    if(NULL == next_param)
    {
        printf("djyfs��֧�ִ˲����������뿴help\r\n");
        return false;
    }
    strcpy(destination,Sh_GetWord(next_param,&next_param));
    if((!__Djyfs_CheckFullName(source))||(!__Djyfs_CheckFullName(destination)))
        return false;
    if(stat(source,NULL) == -1)
    {
        printf("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        return false;
    }
    //����������ļ�һ�������أ�����������ΪЧ��һ��
    if(strcmp(source,destination)==0)
        return true;
    if(stat(destination,NULL) != -1)
    {
        second_fp = fopen(destination,"r");
        if(second_fp == NULL)
        {
            return false;
        }
        if(second_fp->file_attr.bits.archive)
        {
            printf("�Ƿ񸲸�%s�� <yes/no>", destination);
            fclose(second_fp);
            while(1)
            {
                EasyScanf(NULL,s);
                if(s[0] == 'y')
                {
                    Djy_PutChar(s[0]);
                    printf("\r\n");
                    remove(destination);//Ϊ������׼��
                    break;
                }
                else if(s[0] == 'n')
                {
                    Djy_PutChar(s[0]);
                    printf("\r\n");
                    return false;
                }
            }
        }
        else
        {
            //��Ŀ¼����Ҫ���ӷ�б�ܣ��Ա����濽��·��ʹ��
            ShFs_StrEndIncreaseBackSlash(destination);
            fp = fopen(source,"r");
            if(fp == NULL)
            {
                fclose(second_fp);
                return false;
            }
            if(fp->file_attr.bits.archive)
            {//���source_file���ļ������ļ������뵽second_work_pathĩβ
            //ʹ����������������ṩ׼ȷ��·����
                ShFs_StrEndWipeBackSlash(source);
                __Djyfs_PickFileNameWord(source,s);
                strcat(destination,s);
             }
                fclose(second_fp);
                fclose(fp);
        }
    }
    else//���destination�����ڣ���ôȫ�������ļ�����
        ShFs_StrEndWipeBackSlash(source);
    fp = fopen(source,"r");
    if(fp == NULL)
        return 0;
    if(!fp->file_attr.bits.folder)//source��һ���ļ�
    {
        fclose(fp);
        Shfs_MoveFileToDest(source,destination);
        printf("�Ѿ��ƶ���      1���ļ�\r\n");
    }
    else//source��һ��Ŀ¼
    {
        fclose(fp);
        ShFs_StrEndIncreaseBackSlash(source);
        Shfs_MoveFileOfFolderToDest(source,destination);
    }
    return true;
}

//----�޸�ָ���ļ������ļ��е�����------------------------------------------------
//����: ��������ren���޸�ָ�����ļ������ļ���
//����: param���������
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t ShFs_Ren(char *param)
{
    char *next_param;
    char *fullname;//ָ�����޸ĵ��ļ�
    char *filename;//�޸ĺ������

    if(param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }
    fullname = Sh_GetWord(param,&next_param);
    filename = Sh_GetWord(next_param,&next_param);
    if(!__Djyfs_CheckFullName(fullname))
        return false;
    if(stat(fullname,NULL) == -1)
    {
        printf("ϵͳ�Ҳ���ָ�����ļ�\r\n");
         return false;
    }
    rename(fullname,filename);
    return true;

}
//----����һ�����߶��Ŀ¼------------------------------------------------
//����: ��������md������һ�����߶��Ŀ¼
//����: param���������
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t ShFs_MD(char *param)
{
    char *next_param;
    char *del_param;
    //����255�ַ�����Ϊfs_mkdir�Ĳ�����Ҫ���������ַ���
    //��������ƣ����ڴ�й©����ɲ��ɿ�
    char str[CN_SHELL_CMD_LIMIT+1];

    del_param = Sh_GetWord(param,&next_param);
    if(del_param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }
    strcpy(str,del_param);
    do
    {
        mkdir(str);
        del_param = Sh_GetWord(next_param,&next_param);
        if(del_param != NULL)
            strcpy(str,del_param);
    }while(del_param!=NULL);
    return true;
}


//----ɾ��һ�����߶���ļ�������ļ�-------------------------------------------
//����: ��ָ����һ�����߶���ļ���ɾ������ֻ��ɾ��Ŀ¼
//����: rd_param�������ַ�
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t ShFs_RD(char *param)
{
    char *next_param = param;
    char *del_param;

    if(param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }


    while(1)
    {
        del_param = Sh_GetWord(next_param,&next_param);
        if(del_param == NULL)
            break;
        if(__Shfs_CheckWildCard(del_param))     //�������ͨ���ɾ���ļ���
        {
            printf("�����﷨����ȷ!\r\n");
        }
        else
            ShFs_RemoveFolder(del_param);
    }

    return true;
}

//----ɾ��һ�����߶���ļ�-----------------------------------------------------
//����: ɾ��һ�����߶���ļ�
//����: param�������ַ�
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t ShFs_Del(char *param)
{
    char *next_param;
    char *del_param;

    //��������Ϊ���ַ�
    if(param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        return false;
    }
    next_param = param;
    while(1)
    {
        del_param = strsep(&next_param," ");
        if(del_param == NULL)
            break;
        if(del_param[0] == ' ')     //���������ո�
            break;
        if( ! __Shfs_JudgeBackSlashIsData(del_param))     //·���������ļ���
        {
            printf("%s �ļ�������ȷ!\r\n",del_param);
            break;
        }
        if(__Shfs_CheckWildCardPath(del_param))     //·�����ֲ������ͨ���
        {
            printf("%s �ļ�������ȷ!\r\n",del_param);
        }
        else if(__Shfs_CheckWildCardName(del_param))   //�ļ�����ͨ���
        {
            ShFs_DelWildcardFiles(del_param);
        }
        else
        {
            if(__Shfs_JudgeItemAttr(del_param) == CN_ITEM_IS_FILE) //��ɾ��Ŀ¼
            {
                if(remove(del_param) == 0)
                    printf("��ɾ�� %s \r\n",del_param);
                else
                    printf("ɾ�� %s ����\r\n",del_param);
            }
        }
    }

    return true;
}

//----��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼-----------------------------------------
//����: ��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼
//����: chdir_param�������ַ�
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t ShFs_CD(char *chdir_param)
{
    bool_t result;
    struct tagFileRsc *fp;
    char work_path[CN_SHELL_CMD_LIMIT+1];

    //ִ��cd/chdir[](���ַ�) ��ʾ��ǰĿ¼����
    if(chdir_param == NULL)
    {
        Sh_PrintWorkPath();
        printf("\r\n");
        return true;
    }
    //�����û�����Ŀ¼��ʱ�������뷴б��
    //��"cd folder\"���"cd folder"
    //ִ��cd/chdir[\] ��������Ŀ¼
    if(strcmp(chdir_param,"\\") ==0)
    {
        if(!Djyfs_GetPTT_Name(work_path,CN_SHELL_CMD_LIMIT))
            return false;
        strcat(work_path,":\\");
        fp=g_ptWorkPath;
        g_ptWorkPath = fopen(work_path,"r");
        fclose(fp);
        return true;
    }
    //ִ��cd/chdir[..] ��������һ��Ŀ¼
    else if(chdir_param[0]=='.' && chdir_param[1]=='.')//ִ�� "cd .."
    {
        if(ShFs_CD_Parent(chdir_param))
            return true;
        else
            return false;
    }
    else
    {
        if(!__Djyfs_CheckFullName(chdir_param))
             return false;
        result = stat(chdir_param,NULL);
        //ִ�� cd/chdir[������]·�� ��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼
        if(result != -1)
        {
            //��Ŀ¼���ܴ�
            if(__Shfs_JudgeItemAttr(chdir_param) == CN_ITEM_IS_FOLDER)
            {
                fp = g_ptWorkPath;
                g_ptWorkPath = fopen(chdir_param,"r");
                fclose(fp);
                return true;
            }
            else if(!__Shfs_JudgeBackSlashIsData(chdir_param))
            {//��C:\,��judge_is_folder���޷��ɹ��ж�
                fp = g_ptWorkPath;
                g_ptWorkPath = fopen(chdir_param,"r");
                fclose(fp);
                return true;
            }
            else
            {
                printf("ϵͳ�Ҳ���ָ����·��\r\n");
                return false;
            }
        }
        else
                printf("ϵͳ�Ҳ���ָ����·��\r\n");
    }
     return true;
}

//-------��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�--------------------------------------
//����: ��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�
//����: param�������ַ�
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t ShFs_Dir(char *param)
{
    char *dir_param;
    char *next_param = param;
    if(param == NULL)
    {
        ShFs_DirItemSon(g_ptWorkPath);
        return true;
    }

    do
    {
        dir_param = strsep(&next_param," ");
        if(dir_param == NULL)
            break;
        if(dir_param[0] == ' ')     //���������ո�
            break;
        Shfs_Dir(dir_param);
    }while(dir_param != NULL);

    return true;
}

//-----------------------------------------------------------------------------
//����: ��������
//����: param: �����ߴ�(�ֽ�Ϊ��λ) ������
//����: true=�ɹ�,
//      false=ʧ�ܣ�
//------------
//���¼�¼:
// 1.����: 2015/3/11
//   ˵��: �޸����ܹ�����
//   ����: ������
// 2.����: 2015/3/19
//   ˵��: ����chipδ��ȷʹ�õ�����
//   ����: ������
// todo: ����ר��Ϊflashfileд��shellô?
//-----------------------------------------------------------------------------
bool_t ShFs_CreatePTT(char *param)
{

    struct tagFlashChip *chip;
    struct tagRscNode *chip_tree;
    u64 dbx_size;//�����ߴ�, �ֽ�Ϊ��λ
    u32 blocks;//�����ߴ�ת
    char *word, *next_param;
    bool_t DeviceFound = false;



    chip_tree = Rsc_SearchTree("flash chip"); //��ȡFlash��Դ���ڵ�,���ڲ�ѯ�豸�ṹ��
    if(NULL != chip_tree){
        chip = (struct tagFlashChip *)Rsc_GetSon(chip_tree);
        if(NULL != chip)
            DeviceFound = true;
    }

    if(!DeviceFound)
    {
        printf("�����豸������.\r\n");
        return (false);
    }

    word = Sh_GetWord(param, &next_param);
    dbx_size = strtol(word, (char **)NULL, 0);
    blocks = dbx_size / chip->block_size + 1; //������С�Կ��
    if(blocks > chip->valid_sum)
    {
        printf("�ߴ�̫��\r\n");
        return false;
    }
    word = Sh_GetWord(next_param, &next_param);
    if(word == NULL)
    {
        printf("����������Ϊ��\r\n");
        return false;
    }


    if(0 != DFFSD_Create_PTT(chip, blocks, word))
    {
        printf("���������ɹ�\r\n");
        return true;
    }
    else
    {
        printf("��������ʧ��\r\n");
        return false;
    }
}

bool_t ShFs_Format(char *param)
{
    if(param == NULL)
    {
        printf("����������Ϊ��");
        return true;
    }

    if(true == Djyfs_Format(1, 0, param))
    {
        printf("��ʽ���ɹ�");
    }
    else
    {
        printf("��ʽ��ʧ��");
    }
    printf("\r\n");
    return true;
}

//----��ӡ��ǰĿ¼-------------------------------------------------
//����: ��ӡ��ǰĿ¼��
//����: node���õ�����Դ�ڵ�
//����: true����ӡ�ɹ�
//      false,��ӡʧ��
//-----------------------------------------------------------------------------
bool_t Sh_PrintWorkPath(void)
{
    u32 len;
    char *buf;
    len = Djyfs_GetCwdLen( );
    buf = M_MallocLc(len,0);
    if(buf == NULL)
        return false;
    Djyfs_GetCwd(buf,len);
    printf("\r\n%s>", buf);
    free(buf);
    return true;
}


bool_t ShFs_TestWrite(char *param)
{
    FILE *fp;
    u16 i;
    u8 write_buf[100];

    for(i=0;i<100;i++)
        write_buf[i]=i;

    fp = fopen("sys:\\Test_W_R.dbx","w+");
    if(fp == NULL)
    {
        printf("�ļ�������");
    }
    else
    {
        if(fwrite(write_buf,100,1,fp) != 100)
        {
            printf("�ļ�д������");
        }
        else
        {
            printf("�ļ�д��ɹ�");
        }

    }
    printf("\r\n");
    fclose(fp);

    return true;
}

bool_t ShFs_TestRead(char *param)
{
    FILE *fp;
    u16 i;
    u8 read_buf[100];

    fp = fopen("sys:\\Test_W_R.dbx","r");
    if(fp == NULL)
    {
        printf("�ļ�������");
    }
    else
    {
        if(fread(read_buf,100,1,fp) != 100)
        {
            printf("�ļ���������");
        }
        else
        {
            for(i=0;i<100;i++)
            {
                if(read_buf[i]!=i)
                {
                    printf("�ļ�У������");
                    i = 200;
                }
            }
            if(i<200)
                printf("�ļ�У��ɹ�");
        }
    }
    printf("\r\n");
    fclose(fp);

    return true;
}

