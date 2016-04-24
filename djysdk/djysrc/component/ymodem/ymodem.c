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
//����ģ��:shellģ��
//����:  ����.
//�汾��V1.0.0
//�ļ�����:Ϊdjyos��shell���������������ļ��Ĺ��ܣ����Խ��ļ���
//         �ص�ָ���ļ���ָ����ַ
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-10-06
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
//todo: ��ָ��λ�ö�д�Ĺ��ܲ��ȥ,���ļ�������׼�ļ�ϵͳ����Ĺ���.
//todo: ���ڳ�ʼ��������ָ��ͨ���豸,�������ڱ�׼��������豸
#include "config-prj.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "file.h"
#include "char_term.h"
#include "string.h"
#include "djyos.h"
#include "systime.h"
#include "shell.h"
#include "driver.h"
#include "verify.h"
#include "stddev.h"
#include "ymodem.h"

extern const char *gc_pCfgStddevName;  //��׼�ն�����
                                    //
#define CN_YMODEM_SOH     1         //128�ֽ���ʼ��
#define CN_YMODEM_STX     2         //1028�ֽ���ʼ��
#define CN_YMODEM_EOT     4
#define CN_YMODEM_ACK     6
#define CN_YMODEM_NAK     0x15
#define CN_YMODEM_C       0x43          //��д��ĸC
#define CN_YMODEM_CAN     0x18
#define CN_YMODEM_ERR     0xff

#define CN_YMODEM_PACK_SIZE  1024       //ymodem�����ݰ���С
#define CN_YMODEM_HEAD_SIZE  128        //ymodem���ش�СΪ128
#define CN_YMODEM_PACK_ZERO  00         //��һ��

#define CN_FILEBUF_SIZE   (33*1024) //���建������С���ɺ�����̬����
#define CN_MAX_PACKNUM    32            //����������������������������Ҫд���ļ�
#define CN_DATABUF_SIZE    (32*1024)   //���建������С

bool_t Sh_DownloadFile(char *param);
bool_t Sh_DownloadFileAt(char *param);
bool_t Sh_UploadFile(char *param);
bool_t Sh_UploadFileFrom(char *param);
bool_t Sh_ModifyFlash(char *param);

u32 (*fn_pWriteTo)(uint32_t addr, uint8_t *buf, uint32_t len) = NULL;
u32 (*fn_pReadFrom)(uint32_t addr, uint8_t *buf, uint32_t len) = NULL;

struct tagShellCmdTab const ymodem_cmd_table[] =
{
    {
        "download",
        Sh_DownloadFile,
        "�����ļ�",
        "�����ʽ: download"
    },
    {
        "download_at",
        Sh_DownloadFileAt,
        "�����ļ���ָ����ַ",
        "�����ʽ: download_at ��ַ"
    },
    {
        "upload",
        Sh_UploadFile,
        "�ϴ��ļ�",
        "�����ʽ: upload �ļ���"
    },
    {
        "upload_from",
        Sh_UploadFileFrom,
        "��ָ����ַ�����ݣ����ϴ�Ϊ�ļ�",
        "�����ʽ: upload_from ��ַ ���� �ļ���"
    },
    {
        "flash",
        Sh_ModifyFlash,
        "�޸�flash��ָ����ַ����,��󲻳�16byte",
        "�����ʽ��flash ��ַ  ���� ����"
    }
};

static struct tagShellCmdRsc tg_ymodem_cmd_rsc
                        [sizeof(ymodem_cmd_table)/sizeof(struct tagShellCmdTab)];

//----ymodemģ�ͳ�ʼ��---------------------------------------
//����: ymodem�����ļ�ģ��ӿں�������module_init���ã�
//      �ú�����ʼ��xmodem��װ��shell�����ļ�����
//����: ��
//����: ��
//-----------------------------------------------------------
ptu32_t Ymodem_ModuleInit(ptu32_t para)
{
    if(Sh_InstallCmd(ymodem_cmd_table,tg_ymodem_cmd_rsc,
            sizeof(ymodem_cmd_table)/sizeof(struct tagShellCmdTab)))
        return true;
    else
        return false;
}

//----��ȡ��������-------------------------------------------
//���ܣ�ymodem����ʱ����ȡ�������ݺ�������djy_getchar���ƣ���
//      �����޸İ汾������Ӧ��ģ��Ӧ������
//������buf��������ָ��
//���أ�ʵ�ʶ����Ĵ�С,cn_limit_uint32��ʱ
//-----------------------------------------------------------
u32 __Ymodem_Gets(u8 *buf)
{
    struct tagInputDeviceMsg input_msg;
    struct tagCharTermineralMsg *char_msg;

    if(! Stddev_ReadDefaultMsg(&input_msg,5000*mS))
    {
        return CN_LIMIT_UINT32;
    }
//  input_msg = (struct tagInputDeviceMsg *)Djy_GetEventPara(NULL,NULL);
//  if(input_msg == NULL)
//  {
//      evttid = Djy_MyEvttId();
//      if(Djy_WaitEvttPop(evttid,NULL,5000*mS) == CN_SYNC_TIMEOUT)
//          return CN_LIMIT_UINT32;
//      input_msg = (struct tagInputDeviceMsg *)Djy_GetEventPara(NULL,NULL);
//  }
    char_msg = &(input_msg.input_data.char_termineral);
    memcpy(buf,char_msg->input_char,char_msg->num);
//  Djy_ParaUsed(0);

    return char_msg->num;
}

//----��ȡ����----------------------------------------------
//����: ��buf����ȡһ���ɿո���н����������ĵ��ʣ�next����
//      ������һ�������׵�ַ�����û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
//-----------------------------------------------------------
char *__Ymodem_GetWord(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    if(buf == NULL)
        return NULL;
    while(1)
    {
        if((buf[i] == ' ') || (buf[i] == 0))
        {
            buf[i] = '\0';
            break;
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *next = NULL;
            else
                *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}
//------ymodemȡ������---------------------------------------
//���ܣ�ymodemģ��֪ͨ�Զ�ȡ�����δ���
//��������
//���أ���
//-----------------------------------------------------------
void __Ymodem_CancelTrans(void)
{
    Djy_PutChar(CN_YMODEM_CAN);                 //ȡ������
    Djy_PutChar(CN_YMODEM_CAN);
    Djy_PutChar(CN_YMODEM_CAN);
}

//------У��ymodem���ݰ�-------------------------------------
//���ܣ�ymodemģ��У����յ������ݰ�
//������buf�����ݰ�����
//      pack_len: ymodem���ݰ�����
//���أ�true:  ���ݰ�У����ȷ
//      false: ���ݰ�У�����
//----------------------------------------------------------
bool_t __Ymodem_PackCheck(u8* buf, u32 pack_len)
{
    u16 checksum;
    if((buf[1] + buf[2]) != 0xff)               //У�����������
    {
        Djy_PutChar(CN_YMODEM_NAK);                 //Ӧ��nak�������ط�
        return false;
    }
    checksum = Crc_16(buf+3, pack_len);
    if(checksum != *(u16 *)&buf[pack_len+3])    //CRCУ�����
    {
        Djy_PutChar(CN_YMODEM_NAK);                 //Ӧ��nak�������ط�
        return false;
    }

    return true;
}

//------ymodem�����ļ�---------------------------------------
//���ܣ������ļ���shell����
//������param: �����ַ���
//���أ�true:  �����ɹ�
//      false: ����ʧ��
//----------------------------------------------------------
bool_t Sh_DownloadFile(char *param)
{
    u8 package[1029];
    u32 over=0,readed=0,reads=0;
    u32 write_len=0,file_size=0,completed=0;
    u32 offset=0,pack_no=0,pack_len=0; //���Ŵ�0��ʼ����
    u32 i=0,sum=0;

    char* filebuf;
    FILE *file=NULL;

    char *filename,*nextparam,*str_filesize;

    //��������Ϊ32K
    if((filebuf=(char*)M_MallocLc(CN_FILEBUF_SIZE,0))==NULL)
    {
        printf("�����ڴ�ʧ��!\r\n");
        return false;
    }

    Djy_PutChar(CN_YMODEM_C);

    //�ȴ������������ݣ���ʱ����
    printf("���ص���ʱ��     ");
    while (Djy_WaitEvttPop(Djy_MyEvttId(), NULL,
            1000 * mS) == CN_SYNC_TIMEOUT)
    {
        if (over++ < 30)
        {
            printf("\b\b\b\b\b%2dS ",30-over);
            Djy_PutChar(CN_YMODEM_C); //��ʱ�����·���C
            continue;
        }
        else
        {
            printf("��ʱδ����\r\n");
            free(filebuf);
            return false;
        }
    }
    //�Ѿ���ʼ�����ļ�
    while(1)
    {
        reads = __Ymodem_Gets(package + readed);            //��ȡ��
        readed += reads;
        if(reads == CN_LIMIT_UINT32)                        //���ճ�ʱ
        {
            __Ymodem_CancelTrans();
            printf("\r\n���䳬ʱ��ѡ��Э�����\r\n");
            printf("��ѡ��Ymodem��\r\n");
            fclose(file);
            free(filebuf);
            return false;
        }

        switch(package[0])
        {
        case CN_YMODEM_SOH:                                 //128�ֽ�֡��������ʼ֡������֡����������֡��
            pack_len = CN_YMODEM_HEAD_SIZE;
            if(readed >= pack_len+5)
            {
                readed = 0;
                if(__Ymodem_PackCheck(package,pack_len) == false)
                    break;
                if(package[1] == CN_YMODEM_PACK_ZERO)               //����Ϊ0�����ʾΪ��ʼ���������
                {
                    if(pack_no == 0)    //pack_noΪ0ʱ����ʾ�ð�Ϊ��ʼ��,����Ϊ������
                    {
                        //��ȡ���ƺʹ�С
                        filename = __Ymodem_GetWord((char*)&package[3],&nextparam);         //��ȡ�ļ���
                        str_filesize = __Ymodem_GetWord(nextparam,&nextparam);
                        file_size = strtol(str_filesize, (char **)NULL, 0);

                        //���ļ�
                        file=fopen(filename,"r+b");
                        if(file == NULL)
                        {
                            __Ymodem_CancelTrans();
                            printf("\r\n���ļ�ʧ�ܻ��ļ�������!\r\n");
                            free(filebuf);
                            return false;
                        }
                        if(file_size > Djyfs_Fcubage(file))
                        {
                            __Ymodem_CancelTrans();
                            printf("\r\n���ص��ļ��������ļ������ߴ�!\r\n");
                            free(filebuf);
                            return false;
                        }
                        Djy_PutChar(CN_YMODEM_ACK);                                             //�װ��ظ�ACK
                        Djy_PutChar(CN_YMODEM_C);
                    }
                    else
                    {
                        for(i=3; i < 130; i++)
                            sum += package[i];
                        if(sum == 0)                                //�������
                        {
                            Djy_PutChar(CN_YMODEM_ACK);
                            printf("\r\n�������\r\n");
                            Djyfs_Ftruncate(file, completed);
                            fclose(file);//-----todo
                            free(filebuf);
                            return true;
                        }
                        else                            //�ȷ���ʼ��Ҳ�ǽ����������
                        {
                            __Ymodem_CancelTrans();
                            printf("\r\n��������ѡ��Э�����\r\n");
                            printf("��ѡ��Ymodem��\r\n");
                            free(filebuf);
                            return false;
                        }
                    }
                }
                else                                        //�ð�Ϊ��ͨ128�ֽ����ݰ�
                {
                    if(package[1] != (pack_no & 0xff))
                    {
                        __Ymodem_CancelTrans();
                        printf("\r\n��������ѡ��Э�����\r\n");
                        printf("��ѡ��Ymodem��\r\n");
                        fclose(file);
                        free(filebuf);
                        return false;
                    }

                    memcpy(&filebuf[offset],(const char*)package+3,pack_len);
                    offset += pack_len;
                    if((offset >= file_size-completed) || (offset >= CN_FILEBUF_SIZE-1024)) //����㹻��д��flash
                    {
                        if(offset >= file_size-completed)                                   //�жϱ���д���С
                            write_len = file_size - completed;
                        else
                            write_len = CN_FILEBUF_SIZE - 1024;
                        offset = 0;
                        fwrite_r(file, filebuf,write_len, completed);
                        completed += write_len;
                    }
                    Djy_PutChar(CN_YMODEM_ACK);
                }
                pack_no++;
            }
            break;
        case CN_YMODEM_STX:                                 //����֡��1024�ֽ�
            pack_len = CN_YMODEM_PACK_SIZE;
            if(readed >= pack_len+5)
            {
                readed = 0;
                if(__Ymodem_PackCheck(package,pack_len) == false)
                    break;
                if(package[1] != (pack_no & 0xff))
                {
                    __Ymodem_CancelTrans();
                    printf("\r\n�������\r\n");
                    fclose(file);
                    free(filebuf);
                    return false;
                }
                memcpy(&filebuf[offset],(const char*)package+3,pack_len);
                offset += pack_len;
                if((offset >= file_size-completed) || (offset >= CN_FILEBUF_SIZE-1024)) //����㹻��д��flash
                {
                    if(offset >= file_size-completed)                                   //�жϱ���д���С
                        write_len = file_size - completed;
                    else
                        write_len = CN_FILEBUF_SIZE-1024;
                    offset = 0;
                    fwrite_r(file, filebuf,write_len, completed);
                    completed += write_len;
                }
                Djy_PutChar(CN_YMODEM_ACK);
                pack_no++;
            }
            break;
        case CN_YMODEM_EOT:                                 //������,���滹��ȫ0����֡
            readed = 0;
            Djy_PutChar(CN_YMODEM_ACK);                         //���յ����������ظ�ACK
            Djy_PutChar(CN_YMODEM_C);                           //���յ����������ظ�C
            break;
        case CN_YMODEM_CAN:                                 //ȡ������
            printf("\r\nȡ������ɹ�\r\n");
            fclose(file);
            free(filebuf);
            return true;
        default:
            __Ymodem_CancelTrans();
            printf("\r\n��������ѡ��Э�����\r\n");
            printf("��ѡ��Ymodem��\r\n");
            fclose(file);
            free(filebuf);
            return false;
        }
    }
    return true;
}

//-------ymodem�����ļ���ָ����ַ----------------------------
//���ܣ������ļ���ָ���ĵ�ַ����ַͨ������ָ��
//������param: �����ַ���
//���أ�true:  �����ɹ�
//      false: ����ʧ��
//----------------------------------------------------------
bool_t Sh_DownloadFileAt(char *param)
{
    u8 package[1029];
    u32 over=0,readed=0,reads=0;
    u32 write_len=0,file_size=0,completed=0;
    u32 offset=0,pack_no=0,pack_len=0; //���Ŵ�0��ʼ����
    u32 i=0,sum=0;
    u32 result= 0;

    char* filebuf;
    char *str_addr,*next_param;
    u32 addr;

    char *nextparam,*str_filesize;

    if(fn_pWriteTo == NULL)
        return false;
    str_addr = Sh_GetWord(param,&next_param);
    addr = strtol(str_addr, (char **)NULL, 0);

    if(addr == 0)
    {
        printf("�������ַ\r\n");
        printf("��ȷ������:download_at ��ַ");
        return false;
    }

    //��������Ϊ32K
    if((filebuf=(char*)M_MallocLc(CN_FILEBUF_SIZE,0))==NULL)
    {
        printf("�����ڴ�ʧ��!\r\n");
        return false;
    }

    Djy_PutChar(CN_YMODEM_C);

    //�ȴ������������ݣ���ʱ����
    printf("���ص���ʱ��     ");
    while (Djy_WaitEvttPop(Djy_MyEvttId(), NULL,
            1000 * mS) == CN_SYNC_TIMEOUT)
    {
        if (over++ < 30)
        {
            printf("\b\b\b\b\b%2ds ",30-over);
            Djy_PutChar(CN_YMODEM_C); //��ʱ�����·���C
            continue;
        }
        else
        {
            printf("��ʱδ����\r\n");
            free(filebuf);
            return false;
        }
    }
    //�Ѿ���ʼ�����ļ�
    while(1)
    {
        reads = __Ymodem_Gets(package + readed);            //��ȡ��
        readed += reads;
        if(reads == CN_LIMIT_UINT32)                        //���ճ�ʱ
        {
            __Ymodem_CancelTrans();
            printf("\r\n���䳬ʱ��ѡ��Э�����\r\n");
            printf("��ѡ��Ymodem��\r\n");
            free(filebuf);
            return false;
        }

        switch(package[0])
        {
        case CN_YMODEM_SOH:                                 //128�ֽ�֡��������ʼ֡������֡����������֡��
            pack_len = CN_YMODEM_HEAD_SIZE;
            if(readed >= pack_len+5)
            {
                readed = 0;
                if(__Ymodem_PackCheck(package,pack_len) == false)
                    break;
                if(package[1] == CN_YMODEM_PACK_ZERO)               //����Ϊ0�����ʾΪ��ʼ���������
                {
                    if(pack_no == 0)    //pack_noΪ0ʱ����ʾ�ð�Ϊ��ʼ��,����Ϊ������
                    {
                        //��ȡ���ƺʹ�С
                        __Ymodem_GetWord((char*)&package[3],&nextparam);            //��ȡ�ļ���
                        str_filesize = __Ymodem_GetWord(nextparam,&nextparam);
                        file_size = strtol(str_filesize, (char **)NULL, 0);

                        Djy_PutChar(CN_YMODEM_ACK);                                             //�װ��ظ�ACK
                        Djy_PutChar(CN_YMODEM_C);
                    }
                    else
                    {
                        for(i=3; i < 130; i++)
                            sum += package[i];
                        if(sum == 0)                                //�������
                        {
                            Djy_PutChar(CN_YMODEM_ACK);
                            printf("\r\n�������\r\n");
                            free(filebuf);
                            return true;
                        }
                        else                            //�ȷ���ʼ��Ҳ�ǽ����������
                        {
                            __Ymodem_CancelTrans();
                            printf("\r\n��������ѡ��Э�����\r\n");
                            printf("��ѡ��Ymodem��\r\n");
                            free(filebuf);
                            return false;
                        }
                    }
                }
                else                                        //�ð�Ϊ��ͨ128�ֽ����ݰ�
                {
                    if(package[1] != (pack_no & 0xff))
                    {
                        __Ymodem_CancelTrans();
                        printf("\r\n��������ѡ��Э�����\r\n");
                        printf("��ѡ��Ymodem��\r\n");
                        free(filebuf);
                        return false;
                    }

                    memcpy(&filebuf[offset],(const char*)package+3,pack_len);
                    offset += pack_len;
                    if((offset >= file_size-completed) || (offset >= CN_FILEBUF_SIZE-1024)) //����㹻��д��flash
                    {
                        if(offset >= file_size-completed)                                   //�жϱ���д���С
                            write_len = file_size - completed;
                        else
                            write_len = CN_FILEBUF_SIZE - 1024;
                        offset = 0;
                        result = fn_pWriteTo(addr+completed,(u8 *)filebuf,write_len);
                        if(result != write_len)
                        {
                            __Ymodem_CancelTrans();
                            if(result == 0)
                                printf("\r\nд�벻�ɹ�\r\n");
                            else if(result == CN_LIMIT_UINT32)
                                printf("\r\n����ĵ�ַ����ȷ\r\n");
                            free(filebuf);
                            return false;
                        }
                        completed += write_len;
                    }
                    Djy_PutChar(CN_YMODEM_ACK);
                }
                pack_no++;
            }
            break;
        case CN_YMODEM_STX:                                 //����֡��1024�ֽ�
            pack_len = CN_YMODEM_PACK_SIZE;
            if(readed >= pack_len+5)
            {
                readed = 0;
                if(__Ymodem_PackCheck(package,pack_len) == false)
                    break;
                if(package[1] != (pack_no & 0xff))
                {
                    __Ymodem_CancelTrans();
                    printf("\r\n�������\r\n");
                    free(filebuf);
                    return false;
                }
                memcpy(&filebuf[offset],(const char*)package+3,pack_len);
                offset += pack_len;
                if((offset >= file_size-completed) || (offset >= CN_FILEBUF_SIZE-1024)) //����㹻��д��flash
                {
                    if(offset >= file_size-completed)                                   //�жϱ���д���С
                        write_len = file_size - completed;
                    else
                        write_len = CN_FILEBUF_SIZE - 1024;
                    offset = 0;
                    result = fn_pWriteTo(addr+completed,(u8 *)filebuf,write_len);
                    if(result != write_len)
                    {
                        __Ymodem_CancelTrans();
                        if(result == 0)
                            printf("\r\nд�벻�ɹ�\r\n");
                        else if(result == CN_LIMIT_UINT32)
                            printf("\r\n����ĵ�ַ����ȷ\r\n");
                        free(filebuf);
                        return false;
                    }
                    completed += write_len;
                }
                Djy_PutChar(CN_YMODEM_ACK);
                pack_no++;
            }
            break;
        case CN_YMODEM_EOT:                                 //������,���滹��ȫ0����֡
            readed = 0;
            Djy_PutChar(CN_YMODEM_ACK);                         //���յ����������ظ�ACK
            Djy_PutChar(CN_YMODEM_C);                           //���յ����������ظ�C
            break;
        case CN_YMODEM_CAN:                                 //ȡ������
            printf("\r\nȡ������ɹ�\r\n");
            free(filebuf);
            return true;
        default:
            __Ymodem_CancelTrans();
            printf("\r\n��������ѡ��Э�����\r\n");
            printf("��ѡ��Ymodem��\r\n");
            free(filebuf);
            return false;
        }
    }
    return true;
}

bool_t __sh_WriteFile(u8 *pData,u32 length,u32 timeout)
{
    tagDevHandle char_term_hdl;
    u32 stdinout;
    bool_t result = false;

    stdinout = Driver_FindDevice((char*)gc_pCfgStddevName);
    char_term_hdl = Driver_OpenDeviceAlias(stdinout,O_RDWR,0);
    if(NULL != char_term_hdl)
    {
        Driver_WriteDevice(char_term_hdl,pData,length,0,CN_BLOCK_BUFFER,timeout);
        Driver_CloseDevice(char_term_hdl);
        result = true;
    }

    return result;
}
//----------------------------------------------------------
//���ܣ��ϴ��ļ���shell����
//������param: �����ַ���
//���أ�true:  �����ɹ�
//      false: ����ʧ��
//----------------------------------------------------------
bool_t Sh_UploadFile(char *param)
{
    char *filebuf = NULL;
    FILE *file=NULL;
    char *filename,*nextparam;

    u32 over = 0,readed = 0,reads = 0,pack_no = 0;
    u32 sendsum = 0,sendlen=0,completed = 0,checklen = 0;
    u32 filenamelen = 0;
    u8 command[32],package[1029];
    u16 checksum = 0;

    filename = Sh_GetWord(param, &nextparam);
    if(filename == NULL)
    {
        printf("�����ʽ����!\r\n");
        printf("��ȷ�������ʽ:upload �ļ���\r\n");
        return false;
    }
    file = fopen(filename,"r+b");//�򿪷�ʽ-------todo
    if(file == NULL)
    {
        printf("�ļ������ڻ��ʧ��\r\n");
        return false;
    }

    filenamelen = strlen(filename);
    sendsum = (u32)(file->file_size);
    //�ȴ����շ����ļ�����
    while (Djy_WaitEvttPop(Djy_MyEvttId(), NULL,
            1000 * mS) == CN_SYNC_TIMEOUT)
    {
        if (over++ < 30)
        {
            printf("\b\b\b\b%2d s",30-over);
            continue;
        }
        else
        {
            printf("��ʱδ����\r\n");
            free(filebuf);
            return false;
        }
    }

    filebuf = (char *)M_MallocLc(sendsum,0);
    if(filebuf == NULL)
    {
        printf("�����ڴ�ʧ��!\r\n");
        return false;
    }
    fread_r(file, filebuf, sendsum, 0);

    while(1)
    {
        reads = __Ymodem_Gets(command + readed);            //��ȡ��
        readed += reads;
        if(reads == CN_LIMIT_UINT32)                        //���ճ�ʱ
        {
            __Ymodem_CancelTrans();
            printf("\r\n���䳬ʱ��ѡ��Э�����\r\n");
            printf("��ѡ��Ymodem��\r\n");
            fclose(file);
            free(filebuf);
            return false;
        }
        switch(command[0])
        {
        case CN_YMODEM_C:
            readed = 0;
            if(pack_no == 0)                                //�װ�
            {
                memset(package,0x00,CN_YMODEM_HEAD_SIZE+5);
                package[0] = CN_YMODEM_SOH;
                package[1] = 0x00;
                package[2] = 0xFF;
                memcpy(package+3,filename,filenamelen);
                package[filenamelen+3] = 0x00;
                itoa(sendsum,(char*)&package[filenamelen+4],10);
                checksum = Crc_16(package+3,CN_YMODEM_HEAD_SIZE);
                memcpy(package+CN_YMODEM_HEAD_SIZE+3,&checksum, 2);

                //send the start package
//                Driver_DevWriteLeft(pg_char_term_lhdl,(ptu32_t)package,
//                              CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);
                __sh_WriteFile(package,CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);
            }
            pack_no++;
            break;
        case CN_YMODEM_NAK:
            __Ymodem_CancelTrans();
            printf("��ѡ��ymodem\r\n");
            fclose(file);
            free(filebuf);
            return false;
            break;
        case CN_YMODEM_CAN:
            fclose(file);
            free(filebuf);
            return false;
            break;
        case CN_YMODEM_ACK:
            readed = 0;
            if(completed < sendsum)//�����������ݰ�
            {
                if(sendsum - completed <= CN_YMODEM_HEAD_SIZE)//����128����ȡ128bytes
                {
                    memset(package,0x00,CN_YMODEM_HEAD_SIZE+5);
                    sendlen = sendsum-completed;
                    checklen = CN_YMODEM_HEAD_SIZE;
                    package[0] = CN_YMODEM_SOH;
                }
                else if(sendsum - completed <= CN_YMODEM_PACK_SIZE)//����128������1024
                {
                    memset(package,0x00,CN_YMODEM_PACK_SIZE+5);
                    sendlen = sendsum-completed;
                    checklen = CN_YMODEM_PACK_SIZE;
                    package[0] = CN_YMODEM_STX;
                }
                else    //�㹻1024�������СΪ1024
                {
                    memset(package,0x00,CN_YMODEM_PACK_SIZE+5);
                    sendlen = CN_YMODEM_PACK_SIZE;
                    checklen = CN_YMODEM_PACK_SIZE;
                    package[0] = CN_YMODEM_STX;
                }
                package[1] = pack_no;
                package[2] = 0xFF - pack_no;
                memcpy(package+3,filebuf+completed,sendlen);
                checksum = Crc_16(package+3,checklen);
                memcpy(package+checklen+3,&checksum,2);
                //send the start package
//                Driver_DevWriteLeft(pg_char_term_lhdl,(ptu32_t)package,
//                              checklen+5,CN_TIMEOUT_FOREVER);
                __sh_WriteFile(package,checklen+5,CN_TIMEOUT_FOREVER);
                completed += sendlen;
            }
            else    //���ݰ�������ϣ��ɽ���
            {
                Djy_PutChar(CN_YMODEM_EOT);
                memset(package,0x00,CN_YMODEM_HEAD_SIZE+5);
                package[0] = CN_YMODEM_SOH;
                package[1] = 0x00;
                package[2] = 0xFF;
                checksum = Crc_16(package+3,CN_YMODEM_HEAD_SIZE);
                memcpy(package+CN_YMODEM_HEAD_SIZE+3,&checksum, 2);
                //send the start package
//                Driver_DevWriteLeft(pg_char_term_lhdl,(ptu32_t)package,
//                              CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);
                __sh_WriteFile(package,CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);

                printf("�������\r\n");
                fclose(file);
                free(filebuf);
                return true;
            }
            pack_no++;
            break;
        default:
            fclose(file);
            free(filebuf);
            return false;
            break;
        }
    }
}

//-------ymodem��ָ����ַ��ȡ���ݲ����浽һ���ļ�-----------
//���ܣ���ָ����λ�ö�ȡָ�����ȵ����ݣ������ļ�����ʽ����
//������param: �����ַ�����param�а������²���
//      1.addr,��ȡ���ݵ���ʼ��ַ����0xffc00000
//      2.len, ��ȡ���ݵĳ��ȣ��ֽ�Ϊ��λ����128
//      3.filename,�ļ���������ȡ�����ݴ�ŵ��ļ���������׺
//���أ�true:  �����ɹ�
//      false: ����ʧ��
//----------------------------------------------------------
bool_t Sh_UploadFileFrom(char *param)
{
    u8 *databuf = NULL;
    char *addr_str,*len_str,*filename,*nextparam;

    u32 read_addr,read_len,filename_len;
    u32 loop,read_flash_times = 0,temp_read_len;
    u32 read_cp = 0;

    u32 over = 0,readed = 0,reads = 0,pack_no = 0;
    u32 sendlen=0,completed = 0,checklen = 0;
    u8 command[32],package[1029];
    u16 checksum = 0;

    if(fn_pReadFrom == NULL)
        return false;
    addr_str = Sh_GetWord(param, &nextparam);
    len_str  = Sh_GetWord(nextparam, &nextparam);
    filename = Sh_GetWord(nextparam, &nextparam);
    if((addr_str == NULL) || (len_str == NULL) || (filename == NULL))
    {
        printf("\r\n��ȷ�������ʽ: upload_from ��ַ ���� �ļ���\r\n");
        return false;
    }

    read_addr    = strtol(addr_str, (char **)NULL, 0);
    read_len     = strtol(len_str, (char **)NULL, 0);
    filename_len = strlen(filename);
    read_flash_times = read_len/CN_DATABUF_SIZE + (read_len%CN_DATABUF_SIZE?1:0);

    databuf = (u8 *)M_MallocLc(CN_DATABUF_SIZE,0);
    if(databuf == NULL)
    {
        printf("�����ڴ�ʧ��!\r\n");
        return false;
    }
    //�ȴ����շ����ļ�����
    while (Djy_WaitEvttPop(Djy_MyEvttId(), NULL,
            1000 * mS) == CN_SYNC_TIMEOUT)
    {
        if (over++ < 30)
        {
            printf("\b\b\b\b%2d s",30-over);
            continue;
        }
        else
        {
            printf("��ʱδ����\r\n");
            free(databuf);
            return false;
        }
    }

    for(loop = 0; loop < read_flash_times;loop++ )
    {
        completed = 0;
        if(loop == read_flash_times - 1)
            temp_read_len = read_len - loop * CN_DATABUF_SIZE;
        else
            temp_read_len = CN_DATABUF_SIZE;
        read_cp += fn_pReadFrom(read_addr + read_cp,
                                        databuf, temp_read_len);
        if(loop >0)
            goto continue_send;
        while(1)
        {
            reads = __Ymodem_Gets(command + readed);            //��ȡ��
            readed += reads;
            if(reads == CN_LIMIT_UINT32)                        //���ճ�ʱ
            {
                __Ymodem_CancelTrans();
                printf("\r\n���䳬ʱ��ѡ��Э�����\r\n");
                printf("��ѡ��Ymodem��\r\n");
                free(databuf);
                return false;
            }
            switch(command[0])
            {
            case CN_YMODEM_C:
                readed = 0;
                if(pack_no == 0)                                //�װ�
                {
                    memset(package,0x00,CN_YMODEM_HEAD_SIZE+5);
                    package[0] = CN_YMODEM_SOH;
                    package[1] = 0x00;
                    package[2] = 0xFF;
                    memcpy(package+3,filename,filename_len);
                    package[filename_len+3] = 0x00;              //�ļ���
                    itoa(read_len,(char*)&package[filename_len+4],10);
                    checksum = Crc_16(package+3,CN_YMODEM_HEAD_SIZE);
                    memcpy(package+CN_YMODEM_HEAD_SIZE+3,&checksum, 2);

                    //send the start package
//                    Driver_DevWriteLeft(pg_char_term_lhdl,(ptu32_t)package,
//                                  CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);
                    __sh_WriteFile(package,CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);
                }
                pack_no++;
                break;
            case CN_YMODEM_NAK:       //���ط�����û��-----todo
                __Ymodem_CancelTrans();
                printf("��ѡ��ymodem\r\n");
                free(databuf);
                return false;
                break;
            case CN_YMODEM_CAN:        //ȡ������
                free(databuf);
                return false;
                break;
            case CN_YMODEM_ACK:        //Ӧ�𣬼������Ͱ�
continue_send:
                readed = 0;
                if(completed < temp_read_len) //�����������ݰ�
                {
                    if(temp_read_len - completed <= CN_YMODEM_HEAD_SIZE)//����128
                    {
                        memset(package,0x00,CN_YMODEM_HEAD_SIZE+5);
                        sendlen = temp_read_len - completed;
                        checklen = CN_YMODEM_HEAD_SIZE;
                        package[0] = CN_YMODEM_SOH;
                    }
                    else if(temp_read_len - completed <= CN_YMODEM_PACK_SIZE)//����1024
                    {
                        memset(package,0x00,CN_YMODEM_PACK_SIZE+5);
                        sendlen = temp_read_len-completed;
                        checklen = CN_YMODEM_PACK_SIZE;
                        package[0] = CN_YMODEM_STX;
                    }
                    else        //1024/pack
                    {
                        memset(package,0x00,CN_YMODEM_PACK_SIZE+5);
                        sendlen = CN_YMODEM_PACK_SIZE;
                        checklen = CN_YMODEM_PACK_SIZE;
                        package[0] = CN_YMODEM_STX;
                    }
                    package[1] = pack_no;
                    package[2] = 0xFF - pack_no;
                    memcpy(package+3,databuf+completed,sendlen);
                    checksum = Crc_16(package+3,checklen);
                    memcpy(package+checklen+3,&checksum,2);
                    //send the start package
//                    Driver_DevWriteLeft(pg_char_term_lhdl,(ptu32_t)package,
//                                  checklen+5,CN_TIMEOUT_FOREVER);
                    __sh_WriteFile(package,checklen+5,CN_TIMEOUT_FOREVER);
                    completed += sendlen;
                }
                else if(loop == read_flash_times - 1)//�����Ѿ�ȫ����ɣ��ɽ���
                {
                    Djy_PutChar(CN_YMODEM_EOT);
                    memset(package,0x00,CN_YMODEM_HEAD_SIZE+5);
                    package[0] = CN_YMODEM_SOH;
                    package[1] = 0x00;
                    package[2] = 0xFF;
                    checksum = Crc_16(package+3,CN_YMODEM_HEAD_SIZE);
                    memcpy(package+CN_YMODEM_HEAD_SIZE+3,&checksum, 2);
                    //send the start package
//                    Driver_DevWriteLeft(pg_char_term_lhdl,(ptu32_t)package,
//                                  CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);
                    __sh_WriteFile(package,CN_YMODEM_HEAD_SIZE+5,CN_TIMEOUT_FOREVER);

                    printf("�������\r\n");
                    free(databuf);
                    return true;
                }
                else                //databuf�е����ݴ������
                    goto read_loop;
                pack_no++;
                break;
            default:
                free(databuf);
                return false;
                break;
            }
        }
        //for loop
read_loop:
        completed = 0;
    }
    return true;
}

//-------ymodem�޸�flash����--------------------------------
//���ܣ��޸�FLASH��ָ����ַ�����ݣ�ֻ֧���޸�ָ����ַ1�ֽڡ�2�ֽ�
//      ��4�ֽڳ���
//������param: �����ַ���
//���أ�true:  �����ɹ�
//      false: ����ʧ��
//----------------------------------------------------------
bool_t Sh_ModifyFlash(char *param)
{
    ptu32_t addr;
    uint32_t data,length;
    uint8_t buf[16],i;
    char *word_addr,*len,*word_byte,*next_param;

    if(fn_pWriteTo == NULL)
        return false;
    //��ȡ3������
    word_addr = Sh_GetWord(param,&next_param);
    len = Sh_GetWord(next_param,&next_param);
    if((word_addr == NULL)||(len == NULL))
    {
        printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n>flash ��ַ ���� ����\r\n");
        return false;
    }

    addr = strtol(word_addr, (char **)NULL, 0);
    length = strtol(len, (char **)NULL, 0);

    if(length > 16)
    {
        printf("\r\n����������볤�ȴ���16�ֽ���󳤶ȣ�\r\n");
        return false;
    }

    for(i = 0; i < length; i++)
    {
        word_byte = Sh_GetWord(next_param,&next_param);
        if(word_byte == NULL)
        {
            printf("\r\n������������볤�Ȳ�һ�£�\r\n");
            return false;
        }
        data = strtol(word_byte, (char **)NULL, 0);
        if(data > 0xFF)
        {
            printf("\r\n��������ֽ����ݴ���255��\r\n");
            return 0;
        }
        buf[i] = (u8)data;
    }

    if(fn_pWriteTo(addr,(u8*)buf,length) != length)
    {
        printf("\r\nд��flash����\r\n");
    }
    return 0;
}

//----ע���д����-------------------------------------------------------------
//����: ��ָ���ĵ�ַ��/д�Ĺ���,���ļ����䲻һ��,�ļ������б�׼��������.����оƬ
//      �ͺŸ��졢�������ò�ͬ,�޷�������׼�Ĳ�������.����ymodemģ�鶨��������
//      ����ָ��,���û�ʵ����غ�����,���ñ�����ע�ᵽ������ָ����.
//����: pWriteTo,д���ݵ�ָ����ַ�ĺ���ָ��
//      pReadFrom, ���ƶ���ַ�����ݵĺ���ָ�롣
//����: ��
//-----------------------------------------------------------------------------
void ymodem_RegisterFn  (
                u32 (*pWriteTo)(uint32_t addr, uint8_t *buf, uint32_t len),
                u32 (*pReadFrom)(uint32_t addr, uint8_t *buf, uint32_t len)
                        )
{
    if(pWriteTo != NULL)
        fn_pWriteTo = pWriteTo;
    if(pReadFrom != NULL)
        fn_pReadFrom = pReadFrom;
    return;
}
