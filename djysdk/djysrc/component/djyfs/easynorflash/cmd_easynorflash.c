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
//����ģ��:easyfile system
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:easy nor flash�ļ�ϵͳ���е�shell����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����:
//   ����: ����
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include <ctype.h>
#include "stdlib.h"
#include "string.h"
#include "lock.h"
#include "systime.h"
#include "file.h"
#include "easynorflash.h"
#include "stdio.h"
#include "shell.h"
#include "endian.h"

extern struct tagSemaphoreLCB *Fs_Semaphore; //����fs�����õ��ź���

bool_t Sh_CreatFile(char *param);
bool_t Sh_ShowFile(char* param);
bool_t Sh_DirEf(char *param);
struct tagShellCmdTab const filesize_cmd_table[] =
{
    {
        "createfile",
        Sh_CreatFile,
        "�����ļ�",
        "�����ʽ: createfile �ļ��� �ļ�����ֽ�"
    },
    {
        "file",
        Sh_ShowFile,
        "��ʾָ���ļ����ļ���С������ļ��ߴ�",
        "�����ʽ: file �ļ���"
    },
    {
        "dir_ef",
        Sh_DirEf,
        "��ʾ�����ļ����ļ���С������ļ��ߴ�",
        "�����ʽ: dir_ef"
    },
};

struct tagGetFileInfo
{
    u32 file_size;
    u32 max_size;
    char name[CN_FILE_NAME_LIMIT + 1];
};


static struct tagShellCmdRsc tg_filesize_cmd_rsc
              [sizeof(filesize_cmd_table)/sizeof(struct tagShellCmdTab)];

//------��Ӽ����ļ�ϵͳshell����----------------------------
//���ܣ���Ӽ����ļ�ϵͳ��shell���ϵͳ���
//������param: �ַ���
//���أ�true:  �����ɹ�
//      false: ����ʧ��
//----------------------------------------------------------
ptu32_t Cmd_InstallEasynorFs(ptu32_t para)
{
    if(Sh_InstallCmd(filesize_cmd_table,tg_filesize_cmd_rsc,
            sizeof(filesize_cmd_table)/sizeof(struct tagShellCmdTab)))
        return true;
    else
        return false;
}

//------ymodem������ļ�----------------------------------
//���ܣ������ļ���shell����
//������param: �ַ���
//���أ�true:  �����ɹ�
//      false: ����ʧ��
//----------------------------------------------------------
bool_t Sh_CreatFile(char *param)
{
    ptu32_t max_size;
    char *next_param;
    char *filename,*str_maxsize;
    FILE *new_file=NULL;

    if(param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        printf("��ȷ������:createfile �ļ��� �ļ�����ֽ�\r\n");
        return false;
    }

    filename = Sh_GetWord(param,&next_param);
    str_maxsize  = Sh_GetWord(next_param,&next_param);

    if((filename == NULL) || (str_maxsize == NULL))
    {
        printf("�����﷨����ȷ!\r\n");
        printf("��ȷ������:createfile �ļ��� �ļ�����ֽ�\r\n");
        return false;
    }

    max_size = strtol(str_maxsize, (char **)NULL, 0);
    //���ļ����ڣ���򿪣��������ڣ��������
    new_file = fopen(filename,"w+b");
    if(new_file != NULL)
    {
        //�����������޸��ļ����ߴ�
        if(0 == Djyfs_Ftruncate(new_file,max_size))
        {
            printf("�ļ������ɹ�!\r\n");
            return true;
        }
    }
    printf("�����ļ�ʧ��!\r\n");
    return false;
}

//----shell��ʾ�ļ���С---------------------------------------------------
//���ܣ������ļ�ϵͳ�ڴ����ļ�ʱ����ָ���ļ����ߴ磬��shell����ķ�����
//      ��ʾָ�����ļ����ļ���С������ļ��ߴ�
//������param���ַ�������
//���أ�true=�ɹ���false=ʧ��
//-------------------------------------------------------------------------
bool_t Sh_ShowFile(char* param)
{
    struct tagRscNode *fs_rsc_node;
    struct tagPTTDevice *tagPTTDevice;
    struct tagEasynorFlashChip *chip;
    struct tagEasyFlashFileInfo fileinfo;
    bool_t found = false;

    char *disk;
    bool_t result = false;
    u32 block_no,offset,loop;
    u32 *hsize,len;

    char *filename,*next_param;
    if(param == NULL)
    {
        printf("�����﷨����ȷ!\r\n");
        printf("��ȷ������:file �ļ���\r\n");
        return false;
    }

    filename = Sh_GetWord(param,&next_param);
    
    //��ȡ��ǰ�̷�
    len = Djyfs_GetCwdLen( );
    disk = M_MallocLc(len,0);
    if(disk == NULL)
        return false;
    Djyfs_GetCwd(disk,len);
    
    for(loop = 0; loop <len || loop < 256; loop++)
    {
    	if(disk[loop] == ':')
    	{
    		disk[loop] = '\0';//��Ӵ��̷��ַ���������
    		break;
    	}
    }
    if((loop == len) || (loop == 256))
    {
    	printf("���̷�����\r\n");
    	return false;
    }

    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    fs_rsc_node = Rsc_SearchTree("fs");    //ȡ�ļ�ϵͳ�豸����Դ���ָ��
    if(fs_rsc_node == NULL)
    {
        Lock_SempPost(Fs_Semaphore);
        printf("�ļ�ϵͳδ����!\r\n");
        return false;
    }
    
    //�ڷ����豸�в���ΪPTT_name�Ľ��
    tagPTTDevice = (struct tagPTTDevice *)
            Rsc_SearchSon(fs_rsc_node,disk);
    if(NULL != tagPTTDevice)
    {	
		chip = (struct tagEasynorFlashChip *)tagPTTDevice->PTT_medium_tag;
	
		for (loop = 1; loop <= chip->files_sum; loop++)
		{
			block_no = (loop * CN_EASYNOR_CFG_ITEM_LIMIT)/chip->block_size;
			block_no = chip->start_block + block_no;
			offset = (loop * CN_EASYNOR_CFG_ITEM_LIMIT) % chip->block_size;
	
			chip->read_data(block_no, offset, chip->block_buf,
					CN_EASYNOR_PTT_NAME_LIMIT);
			chip->block_buf[CN_EASYNOR_PTT_NAME_LIMIT] = '\0';
			if (strncmp((const char*)chip->block_buf,(const char*)filename,
					CN_EASYNOR_PTT_NAME_LIMIT) == 0)
			{
				chip->read_data(block_no, offset, chip->block_buf,
						CN_EASYNOR_CFG_ITEM_LIMIT);
				found = true;
				fileinfo.start_item = loop;
				fileinfo.max_size = pick_little_32bit(chip->block_buf,
					CN_EASYNOR_MAXSIZE_OFF >> 2);
				break;
			}
		}
	
		if(!found)
		{
			printf("ָ�����ļ�������򲻴���\r\n");
			return false;
		}
	
		//�������ļ���ʵʱ�ߴ�����ߴ�
		for (loop = 0; loop < CN_EASYNOR_FILESIZE_NUM; loop++)
		{
			hsize =(u32*) (chip->block_buf + CN_EASYNOR_FILESIZE_END - loop * 8);
			if ((*(u64*) hsize == CN_LIMIT_UINT64) || (*(u64*) hsize == 0))
				continue;
			if (*hsize == ~(*(hsize + 1)))
			{
				fileinfo.filesize = pick_little_32bit(chip->block_buf,
						(CN_EASYNOR_FILESIZE_END - loop * 8) >> 2);
				break;
			}
		}
	
		//��ӡ��Ϣ
		printf("\r\n�ļ���                          �ļ���С(byte)      �ļ����ߴ�(byte)\r\n");
		if(loop != CN_EASYNOR_FILESIZE_NUM)
		{
			printf("%-32s0x%08x          0x%08x\r\n",
						  filename,fileinfo.filesize,fileinfo.max_size);
		}
		else
			printf("�����ļ���Сʧ��\r\n");
		
		result = true;
    }
    else
    {
    	printf("δ�ҵ�����!\r\n");
    	result = false;
    }
    
	Lock_SempPost(Fs_Semaphore);
	free(disk);
    return result;
}

//----shell��ʾ�ļ���С---------------------------------------------------
//���ܣ����ļ�ϵͳ�ж���ÿһ���ļ����ļ������ļ���С�Լ��ļ����ߴ�
//������param���ַ�������
//���أ�true=�ɹ���false=ʧ��
//-------------------------------------------------------------------------
u32 __Easynor_ReadFileSize(struct tagPTTDevice *tagPTTDevice,
                           uint32_t file_num,
                           struct tagGetFileInfo *file)
{
    u32 block_no,offset,loop;
    u8 *hsize;
    struct tagEasynorFlashChip *chip;

    chip = (struct tagEasynorFlashChip *) tagPTTDevice->PTT_medium_tag;

    //������ļ�����������ڵ�block��offset
    block_no = (file_num * CN_EASYNOR_CFG_ITEM_LIMIT)/chip->block_size;
    block_no = block_no + chip->start_block;
    offset = (file_num * CN_EASYNOR_CFG_ITEM_LIMIT)%chip->block_size;
    //�����ļ���Ϣ
    chip->read_data(block_no, offset,chip->block_buf,CN_EASYNOR_CFG_ITEM_LIMIT);

    //�ļ���С
    for (loop = 0; loop < CN_EASYNOR_FILESIZE_NUM; loop++)
    {
        hsize = (u8*)(chip->block_buf + CN_EASYNOR_FILESIZE_END - loop * 8);
        if ((pick_little_64bit(hsize,0) == CN_LIMIT_UINT64) ||
            (pick_little_64bit(hsize,0) == 0))
            continue;
        if (pick_little_32bit(hsize,0) == ~(pick_little_32bit(hsize + 4,0)))
        {
            file->file_size = pick_little_32bit(chip->block_buf,
                    (CN_EASYNOR_FILESIZE_END - loop * 8) >> 2);
            break;
        }
    }
    if(loop == CN_EASYNOR_FILESIZE_NUM)//û�еõ���ȷ���ļ���С
        return false;

    file->max_size = pick_little_32bit(chip->block_buf,
                  CN_EASYNOR_MAXSIZE_OFF >> 2);
    loop = 0;
    while((chip->block_buf[loop]) && (loop++ < CN_EASYNOR_FILENAME_LIMIT));
    if(loop >= CN_EASYNOR_FILENAME_LIMIT)//�ļ�������
        return false;
    strncpy((char *)file->name, (char *)chip->block_buf, CN_EASYNOR_FILENAME_LIMIT);
    return true;
}

//----shell��ʾ�ļ���С---------------------------------------------------
//���ܣ������ļ�ϵͳ�ڴ����ļ�ʱ����ָ���ļ����ߴ磬��shell����ķ�����
//      ��ʾ�����ļ����ļ���С������ļ��ߴ�
//������param���ַ�������
//���أ�true=�ɹ���false=ʧ��
//-------------------------------------------------------------------------
bool_t Sh_DirEf(char *param)
{
    struct tagPTTDevice *tagPTTDevice;
    struct tagEasynorFlashChip *chip;
    struct tagRscNode *fs_rsc_node;
    u32 loop,len;
    char *disk;
    bool_t result = false;
    struct tagGetFileInfo fileinfo;

    //��ȡ��ǰ�̷�
    len = Djyfs_GetCwdLen( );
    disk = M_MallocLc(len,0);
    if(disk == NULL)
        return false;
    Djyfs_GetCwd(disk,len);
    
    for(loop = 0; loop <len || loop < 256; loop++)
    {
    	if(disk[loop] == ':')
    	{
    		disk[loop] = '\0';//��Ӵ��̷��ַ���������
    		break;
    	}
    }
    if((loop == len) || (loop == 256))
    {
    	printf("���̷�����\r\n");
    	return false;
    }
    
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    fs_rsc_node = Rsc_SearchTree("fs");    //ȡ�ļ�ϵͳ�豸����Դ���ָ��
    if(fs_rsc_node == NULL)
    {
        Lock_SempPost(Fs_Semaphore);
        printf("�ļ�ϵͳδ����!\r\n");
        return false;
    }

    //�ڷ����豸�в���ΪMDR_PTT->PTT_name�Ľ��
    tagPTTDevice = (struct tagPTTDevice *)Rsc_SearchSon(fs_rsc_node,disk);
    if(NULL != tagPTTDevice)
    {
		chip = (struct tagEasynorFlashChip *)tagPTTDevice->PTT_medium_tag;
	
		printf("\r\n�ļ���                          �ļ���С(byte)      �ļ����ߴ�(byte)\r\n");
		for(loop = 1; loop <= chip->files_sum; loop++)
		{
			if(__Easynor_ReadFileSize(tagPTTDevice, loop, &fileinfo))
			{
				printf("%-32s0x%08x          0x%08x\r\n",
						  fileinfo.name,fileinfo.file_size,fileinfo.max_size);
			}
			else
				printf("�����ļ���Ϣ����!\r\n");
		}
		printf("���ļ�%d���ļ�\r\n",chip->files_sum);
		result = true;
    }
    else
    {
    	printf("δ�ҵ�����\r\n");
    	result = false;
    }

	Lock_SempPost(Fs_Semaphore);
    free(disk);
    return result;
}

