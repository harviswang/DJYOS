//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:djyfs��windows�ļ�ϵͳ�����ӿڣ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "stdint.h"
#include "stdlib.h"
#include "time.h"
#include "file.h"
#include "lock.h"
#include <windows.h>
#include <tchar.h>
#include <ntsecapi.h>

#define From1601To1970US    10000000000
//#pragma comment (lib,"ntdll_32.lib")  // Copy From DDK  64λϵͳ��Ϊntdll_64.lib
//#pragma comment(linker, "/subsystem:windows")
//#pragma comment(linker, "/entry:start")
//#pragma comment(linker, "/filealign:0x200")


#define cn_winpath_limit    260
static char gc_windows_workpath[cn_winpath_limit + 2];
//----�ƶ��ļ�дָ��-----------------------------------------------------------
//����: ��һ���ļ���дָ���ƶ���һ����λ��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp��Ŀ���ļ�ָ��
//      new_ptr���µ�дָ��
//����: true=�ɹ���false=�д���
//-----------------------------------------------------------------------------
bool_t __winfs_move_file_write_ptr(FILE *fp,sint64_t new_ptr)
{
    switch (fp->open_mode)
    {
        case EN_R_RB:
        {
            return false;                       //ֻ���ļ���дλ����Ч
        }break;
        case EN_W_WB:
        {
            fp->write_ptr = new_ptr;            //дλ��
        }break;
        case EN_A_AB:
        {
            fp->write_ptr = new_ptr;            //дλ��
        }break;
        case EN_R_RB_PLUS:
        {
            fp->read_ptr = new_ptr;             //��λ��
            fp->write_ptr = new_ptr;            //дλ��
        }break;
        case EN_W_WB_PLUS:
        {
            fp->read_ptr = new_ptr;             //��λ��
            fp->write_ptr = new_ptr;            //дλ��
        }break;
        case EN_A_AB_PLUS:
        {
            fp->write_ptr = new_ptr;            //дλ��
        }break;
        default: return false;
    }
    if(new_ptr > fp->file_size)       //дָ�볬���ļ�����
    {
        fp->file_size = fp->write_ptr;  //��дָ�븲���ļ�����
    }
    return true;
}


//���������ܾ���GetFinalPathNameByHandle�ķ���ֵ��ȥ��ǰĿ¼����ϧ
//GetFinalPathNameByHandle������xp�����°汾��windows�в�֧�֡�
//���hnd��ͨ��look_for_item�õ��ģ���ô��?��hnd������Դ������!!    lst
void __winfs_GetFileNameFromRsc(char *name,struct tagFileRsc *parent,char *buf)
{
//    char name_temp[cn_winpath_limit+1];
    char *pwn;
    pwn = &parent->name[strlen(parent->name)+1];
    strcpy(buf,pwn);
    if(name != NULL)
    {
//        mbstowcs(name_temp,name,cn_winpath_limit+1);
        strcat(buf,_T("\\"));
        strcat(buf,name);
    }
    return ;
}

//----д�����ݵ��ļ�------------------------------------------------------------
//���ܣ�������д���ļ���ʵ���ϵ���windows��WriteFile����ִ��д��
//������buf�����뻺����
//      len������д���������
//      fp�����������ļ�ָ��
//���أ�����д���������
//-----------------------------------------------------------------------------
uint32_t winfs_write_file(struct tagFileRsc *fp,uint8_t *buf,uint32_t len)
{
    uint32_t completed = 0;
    if((fp==NULL)||(len==0)||(buf==NULL))
        return 0;
    if(! WriteFile((HANDLE)(fp->file_medium_tag),buf,len,(PDWORD)&completed,NULL))
        completed = 0;
    __winfs_move_file_write_ptr(fp,fp->write_ptr + completed);
    printf("%s    %d \n\r",__FUNCTION__ ,completed);
    return completed;
}

//----��������-----------------------------------------------------------------
//���ܣ����ļ��е����ݶ��뵽�������У�ʵ���ϵ���windows��ReadFile������ȡ
//������buf�����뻺����
//      len��������������
//      fp�����������ļ�ָ��
//���أ�ʵ�ʶ�ȡ���ֽ���
//-----------------------------------------------------------------------------
uint32_t winfs_read_file(struct tagFileRsc *fp,uint8_t *buf,uint32_t len)
{
    uint32_t completed = 0;
    if((fp==NULL)||(len==0)||(buf==NULL))
        return 0;
    if(! ReadFile((HANDLE)(fp->file_medium_tag),buf,len,(PDWORD)&completed,NULL))
        completed = 0;
    fp->read_ptr += completed;
    printf("%s    %d \n\r",__FUNCTION__ ,completed);
    return completed;
}

//----ˢд������---------------------------------------------------------------
//����: ���ļ�д������������д��flash
//������fp�����������ļ�ָ��
//���أ�ʵ��д��flash���ֽ���
//-----------------------------------------------------------------------------
uint32_t winfs_flush_file(struct tagFileRsc *fp)
{
    printf("%s    %s \n\r",__FUNCTION__ ,fp->name);
    return FlushFileBuffers((HANDLE)(fp->file_medium_tag));
}

//----��ѯ�ļ��ɶ�������-------------------------------------------------------
//���ܣ���ѯһ���ļ����ж��ٿɶ����ݣ����ڹ�̬�ļ�����flash�ļ�����˵�������ļ�
//      ����-��ǰָ�룬�����ı�׼���岢��һ����ˣ�����ͨ�Ŷ��ӡ����ң��ļ���д
//      ָ����α�ʾ������ļ�ϵͳ������ʵ���йأ��ļ�ϵͳ�ĺ��ĳ���Ӧ�����κ�
//      ���衣
//������fp�����������ļ�ָ��
//���أ��ļ��ɶ����ݵ��ֽ���
//-----------------------------------------------------------------------------
sint64_t winfs_query_file_stocks(struct tagFileRsc *fp)
{
    printf("%s    %d \n\r",__FUNCTION__ ,fp->file_size);
    return fp->file_size - fp->read_ptr;
}

//----��ѯ�ļ���д������-------------------------------------------------------
//���ܣ���ѯһ���ļ�����д��������ݣ����ڹ�̬�ļ�����flash�ļ��������ܾ����ļ�
//      ���ʣ��ռ䣬����windowsģ������˵������ģ�������ڵĴ���ʣ��ռ�
//������fp�����������ļ�ָ��
//���أ��ļ�����д����ֽ���
//-----------------------------------------------------------------------------
s64 winfs_query_file_cubage(struct tagFileRsc *fp)
{
    ULARGE_INTEGER size;
    if(GetDiskFreeSpaceEx(NULL,&size,NULL,NULL))
        return (s64)size.LowPart + ((s64)size.HighPart<<32);
    else
        return 0;
}

//----�����ļ�����-------------------------------------------------------------
//���ܣ������ļ����ȣ�����ʵ�ʳ��Ƚ���β������ʵ�ʳ��Ƚ�׷�ӿռ䡣
//������fp�����������ļ�ָ��
//      new_size���µ��ļ����ȡ�
//���أ��µ��ļ����ȣ�����new_size�������ȣ�һ������Ϊ����û���㹻����������
//-----------------------------------------------------------------------------
sint64_t winfs_set_file_size(struct tagFileRsc *fp,s64 new_size)
{
    LARGE_INTEGER liDistanceToMove;
    liDistanceToMove.QuadPart = new_size;
    SetFilePointerEx((HANDLE)(fp->file_medium_tag),liDistanceToMove,NULL,FILE_BEGIN);
    SetEndOfFile((HANDLE)(fp->file_medium_tag));
    //�����޸��ļ��ߴ������ģʽ����дָ�붼����ͬ�ģ�ֻ���ж϶�ָ�뼴��
    if(fp->read_ptr < new_size)
    {
        liDistanceToMove.QuadPart = fp->read_ptr;
        //djyfs�Ķ��壬�޸��ļ��ߴ粢���ı��ļ�ָ�롣
        SetFilePointerEx((HANDLE)(fp->file_medium_tag),liDistanceToMove,NULL,FILE_BEGIN);
    }
    else    //���ԭָ�볬���ļ����³��ȣ����Ϊ�³���
    {
        fp->read_ptr = new_size;
        fp->write_ptr = new_size;
    }
    fp->file_size = new_size;
    printf("%s    %d \n\r",__FUNCTION__ ,new_size);
    return new_size;
}

//----�����ļ�ָ��-------------------------------------------------------------
//���ܣ����ļ�ָ�����õ�ָ����λ�ã�����Ӧ��ͬ����д��������
//������fp�����������ļ�ָ��
//      position�ĳ�Ա:
//          offset���ƶ���λ��
//          whence���Ӻδ���ʼ�����ƶ����ȣ��μ�SEEK_CUR�ȵĶ���
//���أ�0=�ɹ���1=ʧ��
//-----------------------------------------------------------------------------
uint32_t winfs_seek_file(struct tagFileRsc *fp,struct tagSeekPara *position)
{
    sint64_t new_position;
//    uint32_t buffed_size;
    uint8_t  whence;
    LARGE_INTEGER liDistanceToMove;
    printf("%s    %s \n\r",__FUNCTION__ ,fp->name);
    if((position == NULL) ||(fp == NULL))
        return 1;
    whence = position->whence;
    switch (fp->open_mode)
    {
        case EN_R_RB:           //ֻ����ʽ�����ƶ���ָ��Ͷ�������
        case EN_A_AB_PLUS:      //д׷�ӷ�ʽ��дָ�벻�ܱ䣬��ָ��ķ�ʽ��
                                  //ֻ���ļ�һ��
        {
            if(whence == SEEK_SET)
                new_position = position->offset;
            else if(whence == SEEK_CUR)
                new_position = position->offset + fp->read_ptr;
            else if(whence == SEEK_END)
                new_position = fp->file_size + position->offset;
            else
                return 1;
            if(fp->read_ptr == new_position)
                return 0;
            if(new_position > fp->file_size)
                fp->read_ptr = fp->file_size;
            else if(new_position < 0)
                fp->read_ptr = 0;
            else
                fp->read_ptr = new_position;
            liDistanceToMove.QuadPart = fp->read_ptr;
            SetFilePointerEx((HANDLE)(fp->file_medium_tag),
                                liDistanceToMove,NULL,FILE_BEGIN);
        }break;
        case EN_W_WB:   //ֻд�ļ������ƶ�дָ��
        {
            if(whence == SEEK_SET)
                new_position = position->offset;
            else if(whence == SEEK_CUR)
                new_position = position->offset + fp->write_ptr;
            else if(whence == SEEK_END)
                new_position = fp->file_size - position->offset;
            else
                return 1;
            if(fp->write_ptr == new_position)
                return 0;
            if(new_position < 0)
                new_position = 0;
            liDistanceToMove.QuadPart = new_position;
            SetFilePointerEx((HANDLE)(fp->file_medium_tag),
                                liDistanceToMove,NULL,FILE_BEGIN);
            fp->write_ptr = new_position;   //����дָ��
            if(new_position > fp->file_size)
            {
                SetEndOfFile((HANDLE)(fp->file_medium_tag));
            }
        }break;
        case EN_A_AB:   //׷��д��ʽ��seek������Ч
        {
        }break;
        case EN_R_RB_PLUS:  //�����߶��ǿɶ�д��ʽ����дָ���غϣ�����ʽ��ͬ
        case EN_W_WB_PLUS:
        {
            if(whence == SEEK_SET)
                new_position = position->offset;
            else if(whence == SEEK_CUR)
                new_position = position->offset + fp->read_ptr;
            else if(whence == SEEK_END)
                new_position = fp->file_size + position->offset;
            else
                return 1;

            if(fp->read_ptr == new_position)
                return 0;

            if(new_position < 0)
                new_position = 0;

            if(new_position > fp->file_size)    //��λ�ó����ļ�����
            {
                liDistanceToMove.QuadPart = new_position;
                if(SetFilePointerEx((HANDLE)(fp->file_medium_tag),
                                liDistanceToMove,NULL,FILE_BEGIN))
                {
                    if(SetEndOfFile((HANDLE)(fp->file_medium_tag)))
                    {
                        fp->write_ptr = new_position;   //����дָ��
                        fp->read_ptr = new_position;    //���ö�ָ��
                    }
                    else
                    {
                        liDistanceToMove.QuadPart = fp->write_ptr;
                        SetFilePointerEx((HANDLE)(fp->file_medium_tag),
                                            liDistanceToMove,NULL,FILE_BEGIN);
                    }
                }
            }
            else
            {
                liDistanceToMove.QuadPart = new_position;
                if(SetFilePointerEx((HANDLE)(fp->file_medium_tag),
                                liDistanceToMove,NULL,FILE_BEGIN))
                {
                    fp->write_ptr = new_position;   //����дָ��
                    fp->read_ptr = new_position;    //���ö�ָ��
                }
            }
        }break;
        default: return 1;
    }
    return 0;
}

//----������Ŀ���ļ�/Ŀ¼��----------------------------------------------------
//���ܣ������ļ�(Ŀ¼)��attr��������������Ŀ¼�����ļ�����������ļ�����ͬʱ����
//      1��洢����ɾ���ļ�����ʹ�ļ�����Ϊ0ʱ��Ҳ��������һ���ͷš���֮���κ�
//      ����£��ļ�����ռ��1��洢��
//������name��Ŀ¼���ļ�����
//      parent����Ŀ¼ָ�롣
//      attr�����ļ���Ŀ¼��������
//���أ�true=�ɹ���false=ʧ�ܣ�һ������Ϊ����û���㹻����������
//��ע��������������������ɵ����߱�֤����Ϊ�洢���ʣ�ר�ش洢���ݵ�ְ���Ƿ�
//      ������������һ������Ρ�
//-----------------------------------------------------------------------------
bool_t winfs_create_item(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result,union file_attrs attr,
                         enum _FILE_OPEN_MODE_ mode)
{
    s64 s64Time;
    char name_buf[cn_winpath_limit+1];
    u32 FlagsAndAttributes = 0;
    HANDLE fhandle;
//    struct tm *file_time;
    char *pchar,*pwn;

    printf("%s    %s \n\r",__FUNCTION__ ,name);
    if(parent == NULL)
        return false;
    if(parent->file_attr.bits.folder == 0)
    {
        //��Ŀ¼�������ļ�
        return false;
    }
    __winfs_GetFileNameFromRsc(name,parent,name_buf);
    if( attr.bits.folder) //����Ŀ¼
    {
        if(CreateDirectoryA(name_buf,NULL))
        {
            fhandle = CreateFileA( name_buf,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_BACKUP_SEMANTICS,
                        NULL
                        );
        }
        else
        {
            return false;
        }
    }else
    {
        if(attr.bits.archive)
            FlagsAndAttributes |= FILE_ATTRIBUTE_ARCHIVE;
        if(attr.bits.read_only)
            FlagsAndAttributes |= FILE_ATTRIBUTE_READONLY;
        if(attr.bits.hidden)
            FlagsAndAttributes |= FILE_ATTRIBUTE_HIDDEN ;
        //�ر�ʱɾ����ʱ�ļ��Ĳ�������file.c��djyfs_fclose��������ɡ�
//        if(attr.bits.close_delete)
//            FlagsAndAttributes |= FILE_FLAG_DELETE_ON_CLOSE  ;
        fhandle = CreateFileA( name_buf,
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    CREATE_NEW,
                    FlagsAndAttributes,
                    NULL
                    );
    }
    s64Time = TM_Time(NULL);
#if 0
    file_time = Tm_LocalTime(&s64Time);      //ȡϵͳʱ��
    result->second_create = (u8)file_time->tm_sec;
    result->second_modify = (u8)file_time->tm_sec;
    result->minute_create = (u8)file_time->tm_min;
    result->minute_modify = (u8)file_time->tm_min;
    result->hour_create = (u8)file_time->tm_hour;
    result->hour_modify = (u8)file_time->tm_hour;
    result->date_create = (u8)file_time->tm_mday;
    result->date_modify = (u8)file_time->tm_mday;
    result->month_create = (u8)file_time->tm_mon;
    result->minute_modify = (u8)file_time->tm_mon;
    result->year_low_create = (u8)file_time->tm_year;
    result->year_high_create = (u8)file_time->tm_year>>8;
    result->year_low_modify = (u8)file_time->tm_year;
    result->year_high_modify = (u8)file_time->tm_year>>8;
#endif
    result->CreateTime = s64Time;
    result->ModTime = s64Time;
    result->file_attr = attr;

    strcpy(result->name,name);
    pchar = &result->name[strlen(result->name)+1];
    pwn = &parent->name[strlen(parent->name)+1];
    strcpy(pchar,pwn);
    strcat(pchar,"\\");
    strcat(pchar,name);
    result->file_size = 0;
    result->read_ptr = 0;
    result->write_ptr = 0;
    result->p_read_buf = NULL;
    result->p_write_buf = NULL;
    result->file_medium_tag = (ptu32_t)fhandle;
    result->file_semp = NULL;
    return true;
}

//----�ƶ��ļ�-----------------------------------------------------------------
//���ܣ���һ���ļ���һ��Ŀ¼�ƶ�����һ��Ŀ¼
//������name��Ŀ¼���ļ�����
//      parent����Ŀ¼ָ�롣
//      attr�����ļ���Ŀ¼��������
//���أ�true=�ɹ���false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t winfs_move_file(struct tagFileRsc *src_fp,   struct tagFileRsc *dest_fp)
{
    return false;
}

//----����ļ��е����ļ���Ŀ¼����---------------------------------------------
//���ܣ�����ļ��е����ļ���Ŀ¼��������������Ŀ¼������Ŀ
//������PTT_device_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//      fp��Ŀ���ļ��е��ļ�ָ��
//���أ����ļ���Ŀ¼����
//-----------------------------------------------------------------------------
uint32_t winfs_check_folder(struct tagFileRsc *parent)
{
    char name_buf[cn_winpath_limit+1];
    u32 items_num = 0;
    HANDLE hWnd = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;

    __winfs_GetFileNameFromRsc("*",parent,name_buf);
    hWnd = FindFirstFileA(name_buf,&ffd);
    if (INVALID_HANDLE_VALUE != hWnd)
    {
        do
        {
#ifdef _MSC_VER     //VC��ffd.cFileName�ǿ��ַ�
                if( (wcscmp(ffd.cFileName,_T(".")) == 0)
                    || (wcscmp(ffd.cFileName,_T("..")) == 0) )
                    continue;
#else               //minGW��ffd.cFileName�Ǳ������ַ�
                if( (strcmp(ffd.cFileName,(".")) == 0)
                    || (strcmp(ffd.cFileName,("..")) == 0) )
                    continue;
#endif
            items_num++;
        }while (FindNextFile(hWnd, &ffd) != 0);

        FindClose(hWnd);
    }

    printf("%s    %d \n\r",__FUNCTION__ ,items_num);
    return items_num;
}

//----ɾ����Ŀ���ļ�/Ŀ¼��---------------------------------------------------
//���ܣ�ɾ��һ���ļ�(Ŀ¼)��ֻ��ɾ����Ŀ¼���Ѿ��رյ��ļ���
//������fp����ɾ����Ŀ¼���ļ���ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t winfs_remove_file(struct tagFileRsc *fp)
{
    char name_buf[cn_winpath_limit+1];

    if(fp == NULL)
        return false;
    if(( ! fp->file_attr.bits.folder) && (fp->open_counter != 0))
        return false;   //fp��һ���Ѿ��򿪵��ļ�
    if(fp->file_attr.bits.folder)
    {
        if(winfs_check_folder(fp) != 0)
            return false;   //��һ���ǿ�Ŀ¼
    }
    __winfs_GetFileNameFromRsc(NULL,fp,name_buf);
    if( ! fp->file_attr.bits.folder) //�Ǹ��ļ�����Ҫ�ͷ��ļ�ռ�õĴ洢��
    {
        DeleteFileA(name_buf);
    }
    else
    {
        RemoveDirectoryA(name_buf);
    }
    return true;
}

//----����һ��Ŀ¼������-------------------------------------------------------
//����: ��current_file��ʼ,��ȡ��parent����һ���ļ���ָ��,ֱ��parent
//      �������������.
//����: parent,��Ҫ��������֦�����Ƚ��
//      current_file,��ǰ����λ��,�����ʾ��һ�β���
//����: ������������ָ��(�������ļ���Ŀ¼)
//��ע: ���������������Ϊwindows�� FindFirstFile��������������ϱ�����
//      ��Ŀ¼�µ��ļ�
//      ��һ�α���ʱ(��current==NULL)Ϊ��־��ϵͳ��Ϊstruct file_rsc�ṹ�����ڴ棬
//      ���һ�Σ����ͷ��ڴ档����û���ȷ�ص���winfs_item_traversal_son������
//      ����Ŀ¼���ڴ������ȫ����������Ϊstruct file_rsc�ṹ������ڴ潫��
//      Ҫ�û��ֶ��ͷ�
//---------------------------------------------------------------------
struct tagFileRsc * winfs_item_traversal_son(struct tagFileRsc *parent,
                                            struct tagFileRsc *current)
{
    char name_buf[cn_winpath_limit+1];
    HANDLE *hWnd;
//    SYSTEMTIME syst;
    WIN32_FIND_DATA ffd;
    bool_t found = false;
    char *pchar,*pwn;

    if(parent == NULL)
        return NULL;
    if(current == NULL)
    {
        current = M_MallocLc(sizeof(struct tagFileRsc),CN_TIMEOUT_FOREVER + sizeof(HANDLE*));
        if(current == NULL)
            return NULL;
        hWnd = (HANDLE*)(current+1);
        __winfs_GetFileNameFromRsc("*",parent,name_buf);
        *hWnd = FindFirstFileA(name_buf,&ffd);
        if (INVALID_HANDLE_VALUE != *hWnd)
        {
            do
            {
#ifdef _MSC_VER     //VC��ffd.cFileName�ǿ��ַ�
                if( (wcscmp(ffd.cFileName,_T(".")) == 0)
                    || (wcscmp(ffd.cFileName,_T("..")) == 0) )
                    continue;
#else               //minGW��ffd.cFileName�Ǳ������ַ�
                if( (strcmp(ffd.cFileName,(".")) == 0)
                    || (strcmp(ffd.cFileName,("..")) == 0) )
                    continue;
#endif
                else
                {
                    found = true;
                    break;
                }
            }while (FindNextFile(*hWnd, &ffd) != 0);
            if(!found)
            {
                FindClose(*hWnd);
                free(current);
                return NULL;
            }

        }
    }
    else
    {
        hWnd = (HANDLE*)(current+1);    //+1���״θ�current�����ڴ�ʱ������
        if(FindNextFile(*hWnd, &ffd) != 0)
            found = true;
        else
            FindClose(*hWnd);
    }

    if(found)
    {

        current->file_attr.all = 0;
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
        {
            current->file_attr.bits.read_only = 1;
        }
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
        {
            current->file_attr.bits.hidden = 1;
        }

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            current->file_attr.bits.folder =1;
        }
        else
        {
            current->file_attr.bits.archive =1;
        }
        current->file_size = (s64)(ffd.nFileSizeLow + (((u64)ffd.nFileSizeHigh)<<32));
        current->read_ptr = 0;
        current->write_ptr = 0;
        current->p_read_buf = NULL;
        current->p_write_buf = NULL;
        current->file_medium_tag = (ptu32_t)(*hWnd);
        current->file_semp = NULL;
        current->open_mode = 0;

#ifdef _MSC_VER     //VC��ffd.cFileName�ǿ��ַ�
        wcstombs(current->name,ffd.cFileName,cn_winpath_limit);
#else               //minGW��ffd.cFileName�ǿ��ַ�
        strcpy(current->name,ffd.cFileName);
#endif
        pchar = &current->name[strlen(current->name)+1];
        pwn = &parent->name[strlen(parent->name)+1];
        strcpy(pchar,pwn);
        strcat(pchar,"\\");
        strcat(pchar,current->name);

        current->CreateTime = (s64)ffd.ftCreationTime.dwLowDateTime
                              +((s64)ffd.ftCreationTime.dwLowDateTime<<32)
                              - (s64)From1601To1970US;
        current->ModTime    = (s64)ffd.ftLastWriteTime.dwLowDateTime
                              +((s64)ffd.ftLastWriteTime.dwLowDateTime<<32)
                              - (s64)From1601To1970US;
#if 0
        FileTimeToSystemTime(&ffd.ftCreationTime,&syst);

        current->second_create = syst.wSecond;
        current->minute_create = syst.wMinute;
        current->hour_create = syst.wHour;
        current->date_create = syst.wDay;
        current->month_create = syst.wMonth ;
        current->year_low_create = syst.wYear/100;
        current->year_high_create = syst.wYear%100;

        FileTimeToSystemTime(&ffd.ftLastWriteTime,&syst);
        current->second_modify = syst.wSecond;
        current->minute_modify = syst.wMinute;
        current->hour_modify = syst.wHour;
        current->date_modify = syst.wDay;
        current->month_modify = syst.wMonth ;
        current->year_low_modify = syst.wYear/100;
        current->year_high_modify = syst.wYear%100;
#endif
        return current;
    }
    else
    {
        free(current);
        return NULL;
    }
}


//----�򿪣��ļ�/Ŀ¼��--------------------------------------------------------
//���ܣ��ڷ����в��Ҳ����ļ�(Ŀ¼)��������ļ�������ҪΪ�������д��������ֻ
//      �鸸Ŀ¼����������������
//������name���ļ���Ŀ¼����
//      parent���ļ�/Ŀ¼�ĸ�Ŀ¼��
//      result�������ҵ����ļ�����Ϣ
//      mode���ļ���Ŀ¼����ģʽ
//���أ�cn_fs_open_success,�ɹ����ļ�(Ŀ¼)
//      CN_FS_ITEM_EXIST,�ļ�(Ŀ¼)���ڵ����ܴ�(һ����ģʽ������)
//      CN_FS_ITEM_INEXIST,�ļ�(Ŀ¼)������
//      cn_limit_uint32����������
//----------------------------------------------------------------------------
uint32_t winfs_open_item(char *name,struct tagFileRsc *parent,
                       struct tagFileRsc *result,enum _FILE_OPEN_MODE_ access_mode)
{
    char name_buf[cn_winpath_limit+1];
    u32 dwCreationDisposition = 0;
    u32 dwDesiredAccess = 0;
    LARGE_INTEGER liDistanceToMove;
    HANDLE fhandle;
    WIN32_FIND_DATA ffd;
//    SYSTEMTIME syst;
    char *pchar,*pwn;

    printf("%s    %s \n\r",__FUNCTION__ ,name);
    if(parent == NULL)
        return CN_LIMIT_UINT32;
    if(parent->file_attr.bits.folder == 0)
    {
        //��Ŀ¼�������ļ�
        return CN_LIMIT_UINT32;
    }
    __winfs_GetFileNameFromRsc(name,parent,name_buf);
    fhandle = FindFirstFileA(name_buf,&ffd);
    FindClose(fhandle);
    if (INVALID_HANDLE_VALUE != fhandle)
    {

        result->file_attr.all = 0;
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
        {
            result->file_attr.bits.read_only = 1;
        }
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
        {
            result->file_attr.bits.hidden = 1;
        }

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            fhandle = CreateFileA( name_buf,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_BACKUP_SEMANTICS,
                        NULL
                        );
            if (INVALID_HANDLE_VALUE == fhandle)
                return CN_FS_ITEM_EXIST;
            result->file_attr.bits.folder =1;
        }
        else
        {
            if(access_mode== EN_R_RB)
                dwDesiredAccess = GENERIC_READ;
            else
            {
                if(ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                {
                    return CN_FS_ITEM_EXIST;
                }
                else
                    dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
            }

            if( (access_mode == EN_W_WB) || (access_mode == EN_W_WB_PLUS))
                dwCreationDisposition = TRUNCATE_EXISTING;
            else
                dwCreationDisposition = OPEN_EXISTING;
            fhandle = CreateFileA( name_buf,
                        dwDesiredAccess,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        dwCreationDisposition,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL
                        );
            if (INVALID_HANDLE_VALUE == fhandle)
                return CN_FS_ITEM_EXIST;

            result->file_attr.bits.archive =1;
        }
        result->file_size = (s64)(ffd.nFileSizeLow + (((u64)ffd.nFileSizeHigh)<<32));
        if(access_mode == EN_A_AB)
        {
            result->read_ptr = result->file_size;
            result->write_ptr = result->file_size;
            liDistanceToMove.QuadPart = result->file_size;
            SetFilePointerEx((HANDLE)(result->file_medium_tag),
                                            liDistanceToMove,NULL,FILE_BEGIN);
        }
        else
        {
            result->p_read_buf = NULL;
            result->p_write_buf = NULL;
        }
        result->file_medium_tag = (ptu32_t)fhandle;
        result->file_semp = NULL;

        strcpy(result->name,name);
        pchar = &result->name[strlen(result->name)+1];
        pwn = &parent->name[strlen(parent->name)+1];
        strcpy(pchar,pwn);
        strcat(pchar,"\\");
        strcat(pchar,name);

        result->CreateTime = (s64)ffd.ftCreationTime.dwLowDateTime
                              +((s64)ffd.ftCreationTime.dwLowDateTime<<32)
                              - (s64)From1601To1970US;
        result->ModTime    = (s64)ffd.ftLastWriteTime.dwLowDateTime
                              +((s64)ffd.ftLastWriteTime.dwLowDateTime<<32)
                              - (s64)From1601To1970US;
#if 0
        FileTimeToSystemTime(&ffd.ftCreationTime,&syst);

        result->second_create = syst.wSecond;
        result->minute_create = syst.wMinute;
        result->hour_create = syst.wHour;
        result->date_create = syst.wDay;
        result->month_create = syst.wMonth ;
        result->year_low_create = syst.wYear/100;
        result->year_high_create = syst.wYear%100;

        FileTimeToSystemTime(&ffd.ftLastWriteTime,&syst);
        result->second_modify = syst.wSecond;
        result->minute_modify = syst.wMinute;
        result->hour_modify = syst.wHour;
        result->date_modify = syst.wDay;
        result->month_modify = syst.wMonth ;
        result->year_low_modify = syst.wYear/100;
        result->year_high_modify = syst.wYear%100;
#endif
    }
    else
        return CN_FS_ITEM_INEXIST;
    return CN_FS_OPEN_SUCCESS;
}



//----���ң��ļ�/Ŀ¼��--------------------------------------------------------
//���ܣ��ڷ����в����ļ�(Ŀ¼)
//������name���ļ���Ŀ¼����
//      parent���ļ�/Ŀ¼�ĸ�Ŀ¼��
//      result�������ҵ����ļ�����Ϣ
//���أ�true=�ļ�/Ŀ¼���ڣ�����result�з����ļ���Ϣ(�������ļ�������)��
//      false = �ļ�/Ŀ¼�����ڣ����޸�result��
//----------------------------------------------------------------------------
bool_t winfs_lookfor_item(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result)
{
    char name_buf[cn_winpath_limit+1];
    HANDLE fhandle;
    WIN32_FIND_DATA ffd;
//    SYSTEMTIME syst;
    char *pchar,*pwn;

    printf("%s    %s \n\r",__FUNCTION__ ,name);
    if(parent == NULL)
        return false;
    if(parent->file_attr.bits.folder == 0)
    {
        //��Ŀ¼�������ļ�
        return false;
    }
    __winfs_GetFileNameFromRsc(name,parent,name_buf);

    fhandle = FindFirstFileA(name_buf,&ffd);
    FindClose(fhandle);
    if (INVALID_HANDLE_VALUE != fhandle)
    {
        result->file_attr.all = 0;
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
        {
            result->file_attr.bits.read_only = 1;
        }
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
        {
            result->file_attr.bits.hidden = 1;
        }

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            result->file_attr.bits.folder =1;
        }
        else
        {
            result->file_attr.bits.archive =1;
        }
        result->file_size = (s64)(ffd.nFileSizeLow + (((u64)ffd.nFileSizeHigh)<<32));
        result->read_ptr = 0;
        result->write_ptr = 0;
        result->p_read_buf = NULL;
        result->p_write_buf = NULL;
        result->file_medium_tag = (ptu32_t)fhandle;
        result->file_semp = NULL;
        result->open_mode = 0;

        strcpy(result->name,name);
        pchar = &result->name[strlen(result->name)+1];
        pwn = &parent->name[strlen(parent->name)+1];
        strcpy(pchar,pwn);
        strcat(pchar,"\\");
        strcat(pchar,name);

        result->CreateTime = (s64)ffd.ftCreationTime.dwLowDateTime
                              +((s64)ffd.ftCreationTime.dwLowDateTime<<32)
                              - (s64)From1601To1970US;
        result->ModTime    = (s64)ffd.ftLastWriteTime.dwLowDateTime
                              +((s64)ffd.ftLastWriteTime.dwLowDateTime<<32)
                              - (s64)From1601To1970US;
#if 0
        FileTimeToSystemTime(&ffd.ftCreationTime,&syst);

        result->second_create = syst.wSecond;
        result->minute_create = syst.wMinute;
        result->hour_create = syst.wHour;
        result->date_create = syst.wDay;
        result->month_create = syst.wMonth ;
        result->year_low_create = syst.wYear/100;
        result->year_high_create = syst.wYear%100;

        FileTimeToSystemTime(&ffd.ftLastWriteTime,&syst);
        result->second_modify = syst.wSecond;
        result->minute_modify = syst.wMinute;
        result->hour_modify = syst.wHour;
        result->date_modify = syst.wDay;
        result->month_modify = syst.wMonth ;
        result->year_low_modify = syst.wYear/100;
        result->year_high_modify = syst.wYear%100;
#endif
        return true;
    }
    else
        return false;
}



//----�ر��ļ�(Ŀ¼)-----------------------------------------------------------
//����: �ر�һ���򿪵��ļ�(Ŀ¼)��������ļ������ͷŶ�д����������ʹ������
//      opened_sum�����������Ŀ¼����ֻ��opened_sum������������ֻ�Ǵ���رչ���
//      ��洢������صĲ�����ϵͳ��Դ������file.cģ�鴦��
//������fp�����������ļ�ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t winfs_close_item(struct tagFileRsc *fp)
{
    printf("%s    %s \n\r",__FUNCTION__ ,fp->name);
    if(fp == NULL)
        return false;
    if(CloseHandle((HANDLE)fp->file_medium_tag))
        return true;
    else
        return false;
}

//----�ļ���Ŀ¼������---------------------------------------------------------
//���ܣ��޸�һ���ļ���Ŀ¼�������֣������ֱ�����fp�С�fp�е����ֿ��Դﵽ255��
//      ������ֻ��ǰcn_flash_file_name_limit�ַ���Ч��
//������fp�����������ļ�ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t winfs_rename_item(struct tagFileRsc *fp,char *newname)
{
    return true;
}

//----��ѯ��������-----------------------------------------------------------
//����: ���ط����ܿռ䣬��Ч�ռ䣬���пռ���ֽ���
//����:
//����: ��
//-----------------------------------------------------------------------------
void winfs_check_PTT(struct tagPTTDevice *PTT_device_tag,
                     sint64_t *sum_size,sint64_t *valid_size,sint64_t *free_size)
{
    u32 SectorsPerCluster;
    u32 BytesPerSector;
    u32 NumberOfFreeClusters;
    u32 TotalNumberOfClusters;
    printf("%s     \n\r",__FUNCTION__);
    if( GetDiskFreeSpace(NULL,(PDWORD)&SectorsPerCluster,(PDWORD)&BytesPerSector,
                        (PDWORD)&NumberOfFreeClusters,(PDWORD)&TotalNumberOfClusters))
    {
        if(sum_size != NULL)
            *sum_size =     (s64)SectorsPerCluster
                          * (s64)BytesPerSector
                          * (s64)TotalNumberOfClusters;
        if(valid_size != NULL)
            *valid_size = *sum_size;
        if(free_size != NULL)
            *free_size =     (s64)SectorsPerCluster
                          * (s64)BytesPerSector
                          * (s64)NumberOfFreeClusters;
    }
    else
    {
        if(sum_size != NULL)
            *sum_size = 0;
        if(valid_size != NULL)
            *valid_size = 0;
        if(free_size != NULL)
            *free_size = 0;
    }
}

//----��ѯĿ¼��ߴ�-----------------------------------------------------------
//���ܣ���ѯһ��������Ŀ¼��ߴ磬���ֽڼ��㡣��Ӧ�ó�����Ҫ��ȡ����Ŀ¼��ʱ��
//      �������ñ���������Ŀ¼��ߴ磬Ȼ����ܷ����ڴ档
//������PTT_device_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//���أ�Ŀ¼��ߴ�
//-----------------------------------------------------------------------------
uint32_t winfs_check_FDT_size(struct tagPTTDevice *PTT_device_tag)
{
    printf("%s    \n\r",__FUNCTION__);
    return 0;
}
bool_t winfs_Format(u32 fmt_para1,u32 fmt_para2,struct tagPTTDevice *PTT_device_tag)
{
    return true;
}

//----winfsģ���ʼ��----------------------------------------------------------
//����: ������ϵͳ��Դ���������flashоƬ�����
//����: ��
//����: 1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_WinFs(ptu32_t para)
{
    static struct tagPTTDevice PTT_device_tag[3];
    struct tagFileRsc *root_file;
    char *pchar;
    char buf[256];
    u32 dbx_num = 0;
    HANDLE hWnd = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
    SYSTEMTIME syst;
    syst.wSecond = 0;
    syst.wMinute = 0;
    syst.wHour = 0;
    syst.wDay = 1;
    syst.wMonth = 1;
    syst.wYear = 1970;
    SystemTimeToFileTime(&syst,&ffd.ftCreationTime);

    if( ! SetCurrentDirectory( _T("djyfs") ))
    {
        printf("��ʼ���ļ�ϵͳʧ�ܣ��������Ŀ¼���Ƿ���\"djyfs\"Ŀ¼");
        return 0;
    }

    GetCurrentDirectoryA(cn_winpath_limit+2,gc_windows_workpath);
    hWnd = FindFirstFileA("*",&ffd);
    if (INVALID_HANDLE_VALUE != hWnd)
    {
        do
        {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
#ifdef _MSC_VER     //VC��ffd.cFileName�ǿ��ַ�
                if( (wcscmp(ffd.cFileName,_T(".")) == 0)
                    || (wcscmp(ffd.cFileName,_T("..")) == 0) )
                    continue;
#else               //minGW��ffd.cFileName�Ǳ������ַ�
                if( (strcmp(ffd.cFileName,(".")) == 0)
                    || (strcmp(ffd.cFileName,("..")) == 0) )
                    continue;
#endif
                PTT_device_tag[dbx_num].chip_mutex_tag = Lock_MutexCreate("winfile");
                if(PTT_device_tag->chip_mutex_tag == NULL)
                {
                    break;
                }
                PTT_device_tag[dbx_num].format = winfs_Format;
                PTT_device_tag[dbx_num].write = winfs_write_file;
                PTT_device_tag[dbx_num].read = winfs_read_file;
                PTT_device_tag[dbx_num].flush = winfs_flush_file;
                PTT_device_tag[dbx_num].query_file_stocks = winfs_query_file_stocks;
                PTT_device_tag[dbx_num].query_file_cubage = winfs_query_file_cubage;
                PTT_device_tag[dbx_num].check_PTT = winfs_check_PTT;
                PTT_device_tag[dbx_num].set_file_size = winfs_set_file_size;
                PTT_device_tag[dbx_num].seek_file = winfs_seek_file;
                PTT_device_tag[dbx_num].create_item = winfs_create_item;
            //    PTT_device_tag[dbx_num].move_file= winfs_move_file;
                PTT_device_tag[dbx_num].remove_item = winfs_remove_file;
                PTT_device_tag[dbx_num].open_item = winfs_open_item;
                PTT_device_tag[dbx_num].close_item = winfs_close_item;
                PTT_device_tag[dbx_num].lookfor_item = winfs_lookfor_item;
                PTT_device_tag[dbx_num].rename_item = winfs_rename_item;
                PTT_device_tag[dbx_num].item_traversal_son = winfs_item_traversal_son;
                PTT_device_tag[dbx_num].check_fdt_size = winfs_check_FDT_size;
                PTT_device_tag[dbx_num].read_fdt = (void (*)(struct tagPTTDevice *,u8 *))NULL_func;
                PTT_device_tag[dbx_num].check_folder = winfs_check_folder;
                PTT_device_tag[dbx_num].opened_sum = 0;

#ifdef _MSC_VER     //VC��ffd.cFileName�ǿ��ַ�
                wcstombs(PTT_device_tag[dbx_num].name,
                                ffd.cFileName,cn_winpath_limit);
#else               //minGW��ffd.cFileName�Ǳ������ַ�
                strcpy(PTT_device_tag[dbx_num].name,ffd.cFileName);
#endif
                PTT_device_tag[dbx_num].PTT_medium_tag = (ptu32_t)0;
                PTT_device_tag[dbx_num].formatted = true;
                Djyfs_FsDriverCtrl(enum_fs_add_PTT,
                                    (ptu32_t)&(PTT_device_tag[dbx_num]),0);

                buf[0] = '\0';
                strcat(buf,PTT_device_tag[dbx_num].name);
                strcat(buf,":\\");
                root_file = fopen(buf,"r");
                pchar = &root_file->name[strlen(root_file->name)+1];
                *pchar = '\0';
                strcat(pchar,PTT_device_tag[dbx_num].name);
                dbx_num++;
            }
            if(dbx_num >=3)
                break;
        }while (FindNextFile(hWnd, &ffd) != 0);

        FindClose(hWnd);
    }

    return 1;
}

