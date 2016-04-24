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
//�ļ�����:�ļ�ϵͳ��洢�����޹صĲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����:
//   ����: ����
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "config-prj.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"
#include "lock.h"
#include "systime.h"
#include "file.h"
#include "easynorflash.h"
#include "verify.h"
#include "endian.h"

static struct tagEasyFlashFileInfo *s_ptEasyNorInfo;    //todo:������������ʡ�
u16 Crc_16(u8 *buf, u32 len);

bool_t Easynor_Format_PTT(u32 fmt_para1,u32 fmt_para2,
                        struct tagPTTDevice *PTT_device_tag);
u32 Easynor_WriteFile(struct tagFileRsc *fp,uint8_t *buf,u32 len);
u32 Easynor_ReadFile(struct tagFileRsc *fp,uint8_t *buf,u32 len);
u32 Easynor_FlushFile(struct tagFileRsc *fp);
sint64_t Easynor_QueryFileStocks(struct tagFileRsc *fp);
sint64_t Easynor_QueryFileCubage(struct tagFileRsc *fp);
u32 Easynor_SeekFile(struct tagFileRsc *fp,struct tagSeekPara *position);
bool_t Easynor_CreateItem(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result,union file_attrs attr,
                         enum _FILE_OPEN_MODE_ mode);
bool_t Easynor_MoveFile(struct tagFileRsc *src_fp,    struct tagFileRsc *dest_fp);
bool_t Easynor_RemoveFile(struct tagFileRsc *fp);
struct tagFileRsc * Easynor_ItemTraversalSon(struct tagFileRsc *parent,
                                             struct tagFileRsc *current);
u32 Easynor_OpenItem(char *name,struct tagFileRsc *parent,
                       struct tagFileRsc *result,enum _FILE_OPEN_MODE_ mode);
bool_t Easynor_LookforItem(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result);
bool_t Easynor_CloseItem(struct tagFileRsc *fp);
bool_t Easynor_RenameItem(struct tagFileRsc *fp,char *newname);
void Easynor_CheckPTT(struct tagPTTDevice *PTT_device_tag,
                     sint64_t *sum_size,sint64_t *valid_size,sint64_t *free_size);
sint64_t Easynor_SetFileSize(struct tagFileRsc *fp,s64 new_size);
u32 Easynor_CheckFDT_Size(struct tagPTTDevice *PTT_device_tag);
u32 Easynor_CheckFolder(struct tagFileRsc *parent);

//flashоƬ����Դ��㣬����flashоƬ����װ�ڴ˽����
static struct tagRscNode tg_flash_chip_root_rsc;
struct tagSemaphoreLCB *g_ptEasyNorSemp; //����flash_chip�����õ��ź���

//----��дһ��:��������--------------------------------------------------------
//����: ��оƬ�黺����������д��flash��ĳ�飬���Ǳ�д���λ�õ�ԭ�����ݣ����
//      ��д��ʱ�������飬����һ���¿��滻֮�����ѻ���Ǽǵ�MAT����(������MATд
//      �뵽flash��)�������滻��Ŀ�š�д��ǰ���ȿ����Ƿ���Ҫ�����������Ҫ����
//      ��Ҫ���ÿ��Ƿ��Ѿ���busy[8]�������ǣ���Ҫ��"�ͷ�-��������-��д"�Ĺ��̽�
//      �п��滻����д��������ǿ黺������offset��ʼ��size���ֽڡ�
//����: PTT_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      block_no��Ŀ����
//      offset����ʼ��ַ�ڿ��ڵ�ƫ����
//      size��д�뻺��������
//����: д��ɹ��򷵻�ʵ��д��Ŀ�ţ�
//      д��ʧ�ܷ���cn_limit_uint32���ռ䲻�������������10������ᵼ��ʧ��
//-----------------------------------------------------------------------------
u32 __Easynor_WriteUpdateBlock(struct tagEasynorFlashChip *chip, u32 block_no,
        u32 offset, u8 *buf, u32 size)
{
    bool_t block_ready;
    block_ready = chip->query_block_ready(block_no, offset, buf, size);
    if (!block_ready)
    {
        if (offset != 0)
            //��Ŀ�������ݶ����黺�����У��ȶ�offset��ǰ�Ĳ���
            chip->read_data(block_no, 0, chip->block_buf, offset);
        if (offset + size != chip->block_size)
            //�ٶ���Ч�����Ժ�Ĳ���
            chip->read_data(block_no, offset + size,
                    chip->block_buf + offset + size,
                    chip->block_size - offset - size);
        memcpy(chip->block_buf + offset, buf, size);
        chip->erase_block(block_no); //������
        //д��Ŀ��飬����д��
        return chip->write_data(block_no, 0, chip->block_buf, chip->block_size);
    }
    else
    {
        return chip->write_data(block_no, offset, buf, size);
    }
}

//----��дһ��:�������--------------------------------------------------------
//����: ��оƬ�黺����������д��flash��ĳ�飬��д���λ��ԭ��û����Ч���ݣ����
//      ��д��ʱ�������飬����һ���¿��滻֮�����ѻ���Ǽǵ�MAT����(������MATд
//      �뵽flash��)�������滻��Ŀ�š�д��ǰ���ȿ����Ƿ���Ҫ�����������Ҫ����
//      ��Ҫ���ÿ��Ƿ��Ѿ���busy[8]�������ǣ���Ҫ��"�ͷ�-��������-��д"�Ĺ��̽�
//      �п��滻����д��������ǿ黺������offset��ʼ��size���ֽڡ�
//����: PTT_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      block_no��Ŀ����
//      offset����ʼ��ַ�ڿ��ڵ�ƫ����
//      size��д�뻺��������
//����: д��ɹ��򷵻�ʵ��д��Ŀ�ţ�
//      д��ʧ�ܷ���cn_limit_uint32���ռ䲻�������������10������ᵼ��ʧ��
//-----------------------------------------------------------------------------
u32 __Easynor_WriteAppendBlock(struct tagEasynorFlashChip *chip, u32 block_no,
                                 u32 offset, u8 *buf, u32 size)
{
    bool_t block_ready;
    block_ready = chip->query_block_ready(block_no, offset, buf, size);
    if (!block_ready)
    {
        if (offset != 0)
            //��Ŀ�������ݶ����黺�����У��ȶ�offset��ǰ�Ĳ���
            chip->read_data(block_no, 0, chip->block_buf, offset);
        memcpy(chip->block_buf + offset, buf, size);
        chip->erase_block(block_no); //������
        //д��Ŀ��飬����д��
        return chip->write_data(block_no, 0, chip->block_buf, chip->block_size);
    }
    else
    {
        return chip->write_data(block_no, offset, buf, size);
    }
}

//----������д���¿�-----------------------------------------------------------
//����: ��оƬ�Ŀ黺����������д�뵽һ��������Ч���ݵ��¿��У���д��ǰ���ڲ���
//      ��Ч���ݣ������趯�õ���ָ��飬�����д��ʱ�������飬����һ���¿��滻
//      ֮�����ѻ���Ǽǵ�MAT����(������MATд�뵽flash��)�������滻��Ŀ�š�
//����: PTT_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      block_no��Ŀ����
//      offset����ʼ��ַ�ڿ��ڵ�ƫ����
//      size��д�뻺��������
//����: д��ɹ��򷵻�ʵ��д��Ŀ�ţ�
//      д��ʧ�ܷ���cn_limit_uint32���ռ䲻�������������10������ᵼ��ʧ��
//-----------------------------------------------------------------------------
u32 __Easynor_WriteNewBlock(struct tagEasynorFlashChip *chip, u32 block_no,
                              u32 offset, u8 *buf, u32 size)
{
    bool_t block_ready;
    block_ready = chip->query_block_ready(block_no, offset, buf, size);
    if (!block_ready)
    {
        chip->erase_block(block_no); //������
        //д��Ŀ��飬����д��
        return chip->write_data(block_no, offset, buf, size);
    }
    else
    {
        return chip->write_data(block_no, offset, buf, size);
    }
}
//----����ļ������filesize-----------------------------------------------------------
//����: ���һ���ļ�������filesize��ֻ��飬�����κθ�д
//����: buf��������
//      location��filesize���ݵ�λ�ã����filesize�������ǳ����λ��
//����: 0 = �޴�1=�д�û�и���
//-----------------------------------------------------------------------------
u32 __Easynor_CheckFileSize(u8 *buf, u32 *location)
{
    u32 filesize_no, offset;
    u32 ret = 0;

    for (filesize_no = 0; filesize_no < CN_EASYNOR_FILESIZE_NUM;
            filesize_no++)
    {
        offset = filesize_no * 8;
        if(pick_little_64bit(buf + offset,0) == CN_LIMIT_UINT64)//Ϊȫ1��������޴�
            break;
        else if(pick_little_64bit(buf + offset,0) == 0)//��ʾ����д���Ĵ�С��Ϊȫ0,ֱ������
            continue;
        else if(pick_little_32bit(buf + offset,0) !=
                ~(pick_little_32bit(buf + offset + 4,0)))//�������д�
        {
            ret = 1;
            break;
        }
    }
    *location = filesize_no;
    return ret;
}
//----�������������-----------------------------------------------------------
//����: ���һ���ļ��������������������д��������֮,����д��оƬ�����޴����
//      ���ܸ�����ͬʱ�Ѹû������������ļ������ӵ�chip->files_sum��
//����: buf��������
//      index���ڼ�������
//      chip������оƬ
//����: 0 = �޴�1=�д�������2=�д��ܸ���
//-----------------------------------------------------------------------------
u32 __Easynor_CheckSingleBuf(u8 *buf, u32 index, struct tagEasynorFlashChip *chip)
{
    u32 offset, location, item_no,item_num;
    u32 temp_files_sum = 0;
    u32 ret = 0;

    item_num = (CN_EASYNOR_CFG_LIMIT > chip->block_size)?
            chip->block_size:CN_EASYNOR_CFG_LIMIT;
    item_num = item_num / CN_EASYNOR_CFG_ITEM_LIMIT;
    if(index == chip->start_block)
        item_no = 1;
    else
        item_no = 0;
    //һ�������ж����Ϣ�����ӵڶ� ����ʼ���,
    for (;item_no < item_num;item_no++)
    {
        offset = item_no * CN_EASYNOR_CFG_ITEM_LIMIT;
        if (buf[offset] != 0xFF) //nameΪ�ǿգ���ʾ��������
        {
            temp_files_sum++; //�����õ������ļ�����1
            offset += CN_EASYNOR_FILESIZE_OFF;
            if(__Easynor_CheckFileSize(buf + offset,&location)) //�д���filesize��д��0
            {
            	ret = 1;
            	memset(buf+offset+8*location,0,8);
            }
        }
    }
    //�ѱ����������ļ����ŵ�chip->files_sum
    chip->files_sum += temp_files_sum;
    return ret;
}

//----�������-----------------------------------------------------------
//����: ���һ��(��+����)�ļ��������������������д��������֮,�������Ѿ���
//      flash�У�������д��ȥ��ͬʱ�Ѹû������������ļ������ӵ�chip->files_sum��
//����: buf��������
//      index���ڼ�������
//      chip������оƬ
//����: ��
//-----------------------------------------------------------------------------
void __Easynor_CheckBlock(u8 *buf_main,u32 index_main, u8 *buf_bak, u32 index_bak,
                   struct tagEasynorFlashChip *chip)
{
    u32 item_no,offset,item_num;
    u32 reflash = 0, location;
    u32 temp_files_sum = 0;
    u8 * correct_filesize;

    item_num = (CN_EASYNOR_CFG_LIMIT > chip->block_size)?
            chip->block_size:CN_EASYNOR_CFG_LIMIT;
    item_num = item_num / CN_EASYNOR_CFG_ITEM_LIMIT;
    if(index_main == chip->start_block)
        item_no = 1;
    else
        item_no = 0;
    //�������ļ������ĵڶ�����ʼ���
    for (;item_no < item_num;item_no++)
    {
        offset = item_no * CN_EASYNOR_CFG_ITEM_LIMIT;
        if ((buf_main[offset] != 0xFF) || (buf_bak[offset] != 0xFF)) //�������ж��Ƿ����õ����ļ�����
        {
            temp_files_sum++;
            //������������name,startblock,maxsize,crc
            if(memcmp(buf_main+offset,buf_bak+offset,CN_EASYNOR_FILESIZE_OFF))//������ͬ
            {
                //����������,������������д�������ҪҪ����itemȫ������
                if(memcmp(buf_main + CN_EASYNOR_VERIFITY_OFF, "easyfile", 8))
                {
                    memcpy(buf_main+offset,buf_bak+offset,CN_EASYNOR_CFG_ITEM_LIMIT);
                }
                else    //���÷�������
                {
                    memcpy(buf_bak+offset,buf_main+offset,CN_EASYNOR_CFG_ITEM_LIMIT);
                }
                reflash = 1;
                continue;
            }

            //�˶������ļ�������filesize
            offset += CN_EASYNOR_FILESIZE_OFF;
            if(memcmp(buf_main+offset,buf_bak+offset,
                    CN_EASYNOR_VERIFITY_OFF - CN_EASYNOR_FILESIZE_OFF))//���������filesize��С��ͬ
            {
                if (__Easynor_CheckFileSize(buf_main + offset, &location)) //��������д�
                {
                    correct_filesize = buf_bak+offset+8*location;
                    if(pick_little_64bit(correct_filesize,0) == CN_LIMIT_UINT64)//�������filesize���󣬵���bak��ûд
                    {
                        memset(correct_filesize, 0, 8);//�����������һ��
                        memset(buf_main + offset + 8 * location, 0, 8);
                        chip->write_data(index_main,offset+8 * location,buf_main+offset+8*location,8);
                        chip->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,8);
                    }
                    else if(location == CN_EASYNOR_FILESIZE_NUM)//����������һ��filesize
                    {
                        reflash = 1;
                        memset(buf_main+offset,0xff,CN_EASYNOR_VERIFITY_OFF-CN_EASYNOR_FILESIZE_OFF);
                        memcpy(buf_main+offset,correct_filesize,8);//������ȷfilesize����һ��λ��
                        memset(buf_bak+offset,0xff,CN_EASYNOR_VERIFITY_OFF-CN_EASYNOR_FILESIZE_OFF);
                        memcpy(buf_bak+offset,buf_main+offset,8);//������ȷfilesize����һ��λ��
                    }
                    else////�������filesize���󣬵���bak��д����bak�ļ���С��ȷ
                    {
                        //��дΪ0����filesizeд�����������Ӧ��λ��
                        memset(buf_main + offset + 8 * location, 0, 8);//�����������filesize������д��0
                        memcpy(buf_main + offset + 8 * location + 8,correct_filesize,8);//�ӱ��ݱ��п�����ȷ��filesize����һ��
                        memcpy(correct_filesize + 8,correct_filesize,8);
                        memset(correct_filesize, 0, 8);//�����������һ��

                        //�����µ�filesizeд��оƬ��
                        chip->write_data(index_main,offset+8 * location,buf_main+offset+8*location,16);
                        chip->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,16);
                    }
                }
                else//���������ȷ
                {
                    correct_filesize = buf_main+offset+8*location;
                    if(pick_little_64bit(correct_filesize,0) == CN_LIMIT_UINT64)//�������filesize���󣬵���bak��ûд
                    {
                        memset(correct_filesize, 0, 8);//�����������һ��
                        memset(buf_bak + offset + 8 * location, 0, 8);
                        chip->write_data(index_main,offset+8 * location,buf_main+offset+8*location,8);
                        chip->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,8);
                    }
                    else if(location == CN_EASYNOR_FILESIZE_NUM)//����������һ��filesize
                    {
                        reflash = 1;
                        memset(buf_bak+offset,0xff,CN_EASYNOR_VERIFITY_OFF-CN_EASYNOR_FILESIZE_OFF);
                        memcpy(buf_bak+offset,correct_filesize,8);//������ȷfilesize����һ��λ��
                        memset(buf_main+offset,0xff,CN_EASYNOR_VERIFITY_OFF-CN_EASYNOR_FILESIZE_OFF);
                        memcpy(buf_main+offset,buf_bak+offset,8);//������ȷfilesize����һ��λ��
                    }
                    else
                    {
                        //��bak��filesizeд��0
                        memset(buf_bak + offset + 8 * location, 0, 8);
                        memcpy(correct_filesize + 8,correct_filesize, 8); //���Ʋ�����
                        memcpy(buf_bak + offset + 8 * location + 8,correct_filesize, 8); //�������ļ�������filesize
                        memset(correct_filesize, 0, 8); //�뱸�÷�������һ��

                        //�����µ�filesizeд��оƬ��
                        chip->write_data(index_main,offset+8 * location,buf_main+offset+8*location,16);
                        chip->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,16);
                    }
                }
            }
        }
    }

    chip->files_sum += temp_files_sum;
    if (reflash) //�������Ϣ���filesize������д��flash
    {
        chip->erase_block(index_main);
        chip->erase_block(index_bak);
        chip->write_data(index_main, 0, buf_main, chip->block_size);
        chip->write_data(index_bak, 0, buf_bak, chip->block_size);
    }
}

//----���norflashоƬ---------------------------------------------------------
//����: ���chip��ʽ�Ƿ���ȷ���������ȷ���ָ�֮��
//      ����ȷ��ԭ��: 1��δ�����ļ�ϵͳ��ͬʱ�����ļ�ϵͳ��
//      2�������е����ˣ��ñ������ݻָ�֮��
//      �е�����㷨:
//          1��ÿ��sector����CRCУ�������������У��ֵ��ȣ��������sector��
//          �ļ��ߴ����������������ȷ�� ��������ȷ����ӱ�����copy�������õĲ���
//          ȷ���򱸷�֮��
//          2��filesizeʹ��������ѭ���洢���ҵ����һ��У����ȷ�ļ��ɡ������У
//          �鲻��ȷ�Ĵ��ڣ��Ѹ�����Ϣ���㼴�ɡ�
//          3��"easyfile"���ڵ������ֻ����һ��������Ϣ���ܳ���
//����: chip��������flashоƬ��
//����: true = �ɹ���false = ʧ��
//-----------------------------------------------------------------------------
bool_t __Easynor_CheckNorChip(struct tagEasynorFlashChip *chip)
{
    u32 loop, blocks, end_block, blocksize;
    u8 *bakbuf, *block_buf;
    u8 mainblockerr = 0,bakblockerr = 0;
    u16 crc16_check,temp;
    bakbuf = M_MallocLc(chip->block_size, 0);
    if (bakbuf == NULL)
        return false;
    blocks = (CN_EASYNOR_CFG_LIMIT + chip->block_size-1) / chip->block_size;
    end_block = chip->start_block + blocks;
    blocksize = chip->block_size;
    block_buf = chip->block_buf;

    //����ļ�ϵͳ�Ƿ��Ѿ�����
    chip->read_data(chip->start_block, 0, block_buf, CN_EASYNOR_CFG_ITEM_LIMIT);
    chip->read_data(end_block, 0, bakbuf, CN_EASYNOR_CFG_ITEM_LIMIT);

    //�����������ƻ�������Ҫ���½����ļ�ϵͳ
    if ((memcmp(bakbuf, "easyfile", 8)||
          memcmp(bakbuf + CN_EASYNOR_VERIFITY_OFF, "easyfile", 8)) &&
          (memcmp(block_buf, "easyfile", 8)||
                    memcmp(block_buf + CN_EASYNOR_VERIFITY_OFF, "easyfile", 8)) )
    {
        printf("�ļ�ϵͳδ�����������ļ�ϵͳ---\r\n");
        //�ļ�ϵͳδ����������֮
        memset(bakbuf, 0xff, blocksize);
        memcpy(bakbuf, "easyfile", 8);
        memcpy(bakbuf + CN_EASYNOR_VERIFITY_OFF, "easyfile", 8);
        fill_little_32bit(bakbuf, 2, chip->block_sum);
        fill_little_32bit(bakbuf, 3, chip->block_size);

        crc16_check = Crc_16(bakbuf,16);
//        fill_little_16bit(bakbuf,(blocksize/2)-1,crc16_check);//��CRC16����ڸ�block������ֽ���
        fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
        for (loop = chip->start_block; loop < end_block; loop++)
        {
            chip->erase_block(loop);
            chip->erase_block(loop + blocks);
        }
        chip->write_data(chip->start_block, 0, bakbuf,blocksize);
        chip->write_data(end_block, 0, bakbuf, blocksize);
        //added,������CRCд���ļ�ϵͳ��Ϣ����������ֽ�
        chip->write_data(chip->start_block + blocks - 1,blocksize-2, (u8*)&temp,2);
        chip->write_data(end_block + blocks - 1,blocksize-2, (u8*)&temp,2);
        free(bakbuf);
        return true;
    }

    //����ļ�ϵͳ��Ϣ��������ֽ�CRC�Ƿ���ȷ
    chip->read_data(chip->start_block + blocks - 1, blocksize-2, block_buf+blocksize-2, 2);
    chip->read_data(end_block + blocks - 1, blocksize-2, bakbuf+blocksize-2, 2);

    crc16_check = pick_little_16bit(block_buf,(blocksize/2)-1);
    if(crc16_check != Crc_16(block_buf,16))
        mainblockerr = 1;

    crc16_check = pick_little_16bit(bakbuf,(blocksize/2)-1);
    if(crc16_check != Crc_16(bakbuf,16)) //bakУ���
        bakblockerr = 1;

    //���¼��������ȷ��
    for (loop = chip->start_block; loop < end_block; loop++)
    {
        chip->read_data(loop, 0, block_buf, blocksize);
        chip->read_data(loop + blocks, 0, bakbuf, blocksize);

        if ((block_buf[0] != 0xFF) || (bakbuf[0] != 0xFF)) //��ʾ��block����
        {
//            crc16_check = pick_little_16bit(block_buf,(blocksize/2)-1);
//            if(crc16_check != Crc_16(block_buf,16)) //��У���,����CRC������Ҫ�ǲ�������
            if(mainblockerr == 1)
            {
//                if(crc16_check != Crc_16(bakbuf,16)) //bakУ���
                if(bakblockerr == 1)
                {
                    //����������У������������ļ�ϵͳ�Ѿ��𻵣�����
                    free(bakbuf);
                    return false;
                }
                else //bakУ����ȷ
                {
                    temp = __Easynor_CheckSingleBuf(bakbuf, loop, chip);
                    if (temp == 0)
                    {
                        chip->erase_block(loop);
                        chip->write_data(loop, 0, bakbuf, blocksize);
                    }
                    else if (temp == 1)
                    {
                        chip->erase_block(loop);
                        chip->write_data(loop, 0, bakbuf, blocksize);
                        chip->erase_block(loop + blocks);
                        chip->write_data(loop + blocks, 0, bakbuf, blocksize);
                    }
                    else
                    {
                        free(bakbuf);
                        return false;
                    }
                }
            }
            else //��У����ȷ
            {
//                crc16_check = pick_little_16bit(bakbuf,(blocksize/2)-1);
//                if(crc16_check != Crc_16(bakbuf,16)) //bakУ���
                if(bakblockerr == 1)
                {
                    temp = __Easynor_CheckSingleBuf(block_buf, loop,chip);
                    if (temp == 0)
                    {
                        chip->erase_block(loop + blocks);
                        chip->write_data(loop + blocks, 0, block_buf,blocksize);
                    }
                    else if (temp == 1)
                    {
                        chip->erase_block(loop);
                        chip->write_data(loop, 0, block_buf, blocksize);
                        chip->erase_block(loop + blocks);
                        chip->write_data(loop + blocks, 0, block_buf,blocksize);
                    }
                    else
                    {
                        free(bakbuf);
                        return false;
                    }
                }
                else //bakУ��Ҳ��ȷ
                {
                    __Easynor_CheckBlock(block_buf,loop, bakbuf, loop+blocks, chip);
                }
            }
        }
        else
        {
            break; //�ÿ�δʹ��
        }
    }
    free(bakbuf);
    return true;
}

//----DFFSDģ���ʼ��----------------------------------------------------------
//����: ������ϵͳ��Դ���������flashоƬ�����
//����: ��
//����: 1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Easynor(ptu32_t para)
{
    g_ptEasyNorSemp = Lock_SempCreate(1,1,CN_SEMP_BLOCK_FIFO,"flash_chip tree");
    if(g_ptEasyNorSemp == NULL)
        return 0;  //flash_chip���ź�������ʧ��
    s_ptEasyNorInfo = M_Malloc(gc_u32CfgOpenedfileLimit * sizeof(struct tagEasyFlashFileInfo),0);
    if (s_ptEasyNorInfo == NULL)
        Lock_SempDelete(g_ptEasyNorSemp);
    //����Դ�����н���һ������㣬ϵͳ������оƬ��������������
    Rsc_AddTree(&tg_flash_chip_root_rsc,sizeof(struct  tagRscNode),RSC_RSCNODE,
                        "flash chip");
    return 1;
}

//----���PTT_device_tag-------------------------------------------------------
//����: ���PTT_device_tag
//������PTT_device_tag�����������ļ���ָ�룬ͨ�ýṹ����flashר�á�
//      name���ļ�����
//����: 0=�ɹ�����0=ʧ��
//-----------------------------------------------------------------------------
u32 Easynor_InstallChip(struct tagEasynorFlashChip *chip)
{
//    tagDevHandle fs_handle_right = NULL;
    static struct tagPTTDevice PTT_device_tag;

    if(chip == NULL)
        return -1;
//    if ((fs_handle_right = Driver_DevOpenRight("fs", 0)) == NULL)
//        return -1;

    PTT_device_tag.chip_mutex_tag = Lock_MutexCreate(chip->PTT_name);
    if(PTT_device_tag.chip_mutex_tag == NULL)
        return false;

    PTT_device_tag.format = Easynor_Format_PTT;
    PTT_device_tag.write = Easynor_WriteFile;
    PTT_device_tag.read = Easynor_ReadFile;
    PTT_device_tag.flush = Easynor_FlushFile;
    PTT_device_tag.query_file_stocks = Easynor_QueryFileStocks;
    PTT_device_tag.query_file_cubage = Easynor_QueryFileCubage;
    PTT_device_tag.check_PTT = Easynor_CheckPTT;
    PTT_device_tag.set_file_size = Easynor_SetFileSize;
    PTT_device_tag.seek_file = Easynor_SeekFile;
    PTT_device_tag.create_item = Easynor_CreateItem;
//    PTT_device_tag->move_file= Easynor_MoveFile;
    PTT_device_tag.remove_item = Easynor_RemoveFile;
    PTT_device_tag.open_item = Easynor_OpenItem;
    PTT_device_tag.close_item = Easynor_CloseItem;
    PTT_device_tag.lookfor_item = Easynor_LookforItem;
    PTT_device_tag.rename_item = Easynor_RenameItem;
    PTT_device_tag.item_traversal_son = Easynor_ItemTraversalSon;
    PTT_device_tag.check_fdt_size = Easynor_CheckFDT_Size;
    PTT_device_tag.read_fdt = NULL;
    PTT_device_tag.check_folder = Easynor_CheckFolder;
    PTT_device_tag.PTT_medium_tag = (ptu32_t) chip;
    PTT_device_tag.opened_sum = 0;
    strncpy(PTT_device_tag.name, chip->PTT_name, CN_EASYNOR_PTT_NAME_LIMIT);

    if(__Easynor_CheckNorChip(chip) == false)
    {
        printf("����װ��ʧ�ܣ�\r\n");
        return -1;
    }
    PTT_device_tag.formatted = true;

    Lock_SempPend(g_ptEasyNorSemp,CN_TIMEOUT_FOREVER);
    //����flashоƬ���뵽flashоƬ����Դ����£����ڶ���β
    Rsc_AddSon(&tg_flash_chip_root_rsc,&chip->chip_node,
                        sizeof(struct tagEasynorFlashChip),RSC_EFS_NOR_CHIP,chip->PTT_name);
    Lock_SempPost(g_ptEasyNorSemp);

    Djyfs_FsDriverCtrl(enum_fs_add_PTT,(ptu32_t)&PTT_device_tag,0);
    return 0;
}

//----��ʽ���ļ���------------------------------------------------------------
//���ܣ���ʽ���ļ��񣬸��ļ���Ӧ���Ѿ����������Ҿ���������ʼ�����뵽�豸�����С�
//      A��δ��ʽ�����ļ����ʽ������:
//      1������洢DDR������Ŀ���(2��)
//      2��ɨ��һ�黵�飬��MAT���б�ǻ��飬���Ӻÿ�������2��DDR��
//      3����дDDR�е�DBL��MAT��
//      4��ART�����򣬲���DDR��д��flash�С�
//      5������һ����FDT��
//      6����ʼ��FDT��Ȼ��д��flash��
//      7������MDR����д��flash��
//      B���Ѿ���ʽ�����ļ����ʽ������
//      ��˵��
//������fmt_para1����ʽ��������1=ʹ��eccУ�飬0��ʾ���ã�һ��ר�Ŵ洢��ý���ļ�
//              ���ļ�����Բ���
//      fmt_para2����ʽ���������ݲ���
//      PTT_device_tag������ʽ�����ļ��񣬸��ļ����Ѿ���װ���豸ϵͳ��
//���أ�true=�ɹ���ʽ������ԭ�����Ѿ���ʽ���ö������ٴθ�ʽ���ġ�
//      false=ʧ��
//-----------------------------------------------------------------------------
bool_t Easynor_Format_PTT(u32 fmt_para1, u32 fmt_para2,
        struct tagPTTDevice *PTT_device_tag)
{
    struct tagEasynorFlashChip *chip;
    u32 loop, blocks, end_block;
    u16 crc16_check,little_crc16;

    if (PTT_device_tag == NULL)
        return false;
    if (PTT_device_tag->opened_sum != 0)
        return false;
    //�ļ�����flashоƬָ��
    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;
    blocks = (CN_EASYNOR_CFG_LIMIT + chip->block_size - 1) / chip->block_size;
    end_block = chip->start_block + blocks;

    memset(chip->block_buf, 0xff, chip->block_size);
    memcpy(chip->block_buf, "easyfile", 8);
    memcpy(chip->block_buf + CN_EASYNOR_VERIFITY_OFF, "easyfile", 8);
    fill_little_32bit(chip->block_buf, 2, chip->block_sum);
    fill_little_32bit(chip->block_buf, 3, chip->block_size);

    //��CRC16����ڸ�block������ֽ���
    crc16_check = Crc_16(chip->block_buf,16);
//    fill_little_16bit(chip->block_buf,(chip->block_size/2)-1,crc16_check);
    fill_little_16bit((u8*)&little_crc16,0,crc16_check);    //ת��ΪС��
    chip->files_sum = 0;
    PTT_device_tag->formatted = true;
    for (loop = chip->start_block; loop < end_block; loop++)
    {
        chip->erase_block(loop);
        chip->erase_block(loop + blocks);
    }
    chip->write_data(chip->start_block, 0, chip->block_buf,chip->block_size);
    chip->write_data(end_block, 0, chip->block_buf, chip->block_size);
    chip->write_data(chip->start_block + blocks - 1,chip->block_size-2, (u8*)&little_crc16,2);
    chip->write_data(end_block + blocks - 1,chip->block_size-2, (u8*)&little_crc16,2);
    return true;
}

//----�޸��ļ��ߴ�-------------------------------------------------------------
//����: �޸��ļ���������ļ��ߴ粿�֡�1���������������һ������λ��д�롣2�����
//      �Ҳ�������������飬������filesize������ff��Ȼ����ļ��ߴ���µ���һ��
//      ��λ�ϣ������鲢д��֮��3����bak������ͬ�Ĳ�����
//����: fp�����������ļ�ָ��
//      newsize���µ��ļ��ߴ�
//����: ��
//-----------------------------------------------------------------------------
void __Easynor_ChangeFileSize(struct tagFileRsc *fp, u32 newsize)
{
    struct tagEasyFlashFileInfo *fileinfo;
    struct tagPTTDevice *PTT_device_tag;
    struct tagEasynorFlashChip *chip;
    u8 *block_buf;
    u32 offset, blocksize, cfg_blocks, block_no, item_no,temp;
    u32 filesize_no,file_max_size;

    PTT_device_tag = (struct tagPTTDevice *) (fp->home_PTT);
    fileinfo = (struct tagEasyFlashFileInfo *) fp->file_medium_tag;
    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;

    block_buf = chip->block_buf;
    blocksize = chip->block_size;
    cfg_blocks = (CN_EASYNOR_CFG_LIMIT+ blocksize-1) / blocksize;

    //������ļ�����������ڵ�block
    block_no = (fileinfo->start_item * CN_EASYNOR_CFG_ITEM_LIMIT) / chip->block_size;
    block_no = block_no + chip->start_block;
    //���ļ�����������block�е�ƫ��
    offset = (fileinfo->start_item * CN_EASYNOR_CFG_ITEM_LIMIT) % chip->block_size;
    //������ļ���������block���ǵڼ����ļ�item
    item_no = offset / CN_EASYNOR_CFG_ITEM_LIMIT;

    //�������ļ������
    chip->read_data(block_no, offset,block_buf,CN_EASYNOR_CFG_ITEM_LIMIT);

    //�ļ��ߴ粻�ܴ����ļ����ߴ�
    file_max_size = pick_little_32bit(block_buf,CN_EASYNOR_MAXSIZE_OFF >> 2);

    if(newsize > file_max_size)
        return;

    offset = CN_EASYNOR_FILESIZE_OFF;
    for (filesize_no = 0; filesize_no < CN_EASYNOR_FILESIZE_NUM;filesize_no++)
    {
        if (pick_little_64bit(block_buf + offset,0) == CN_LIMIT_UINT64) //����������ҵ�����λ��
        {
            fill_little_32bit(block_buf, offset >> 2, newsize);
            fill_little_32bit(block_buf, (offset >> 2) + 1, ~newsize);
            temp = offset + item_no * CN_EASYNOR_CFG_ITEM_LIMIT;
            chip->write_data(block_no, temp, block_buf + offset, 8);
            chip->write_data(block_no + cfg_blocks, temp, block_buf + offset, 8);
            break;
        }
        offset += 8;
    }

    if (filesize_no == CN_EASYNOR_FILESIZE_NUM) //�������û�п���λ��
    {
        //�ȶ����������ݵ�buf,������
        chip->read_data(block_no, 0, block_buf,blocksize);

        chip->erase_block(block_no);
        chip->erase_block(block_no + cfg_blocks);

        offset = item_no * CN_EASYNOR_CFG_ITEM_LIMIT + CN_EASYNOR_FILESIZE_OFF;
        memset(block_buf + offset,0xFF,CN_EASYNOR_VERIFITY_OFF - CN_EASYNOR_FILESIZE_OFF);
        fill_little_32bit(block_buf, offset >> 2, newsize);
        fill_little_32bit(block_buf, (offset >> 2) + 1, ~newsize);

        chip->write_data(block_no, 0, block_buf, blocksize);
        chip->write_data(block_no + cfg_blocks, 0, block_buf, blocksize);
    }
}

//----д��flash�ļ�------------------------------------------------------------
//���ܣ��ѻ������е�����д���ļ���ִ�����в�����
//      1�����д���������㹻�Ŀռ䣬������д��д���������ɡ�
//      2�����д������û���㹻�Ŀռ䣬��ͬд������ԭ�����ݺ����뻺�����е�����
//         һ��д��flash
//      3��д����ɺ�����������Ƿ���Ҫͬ����ִ��֮��
//      4�����FDT���DDR���Ƿ���Ҫ�޸Ĳ�ִ��֮
//������buf�����뻺����
//      len������д���������
//      fp�����������ļ�ָ��
//���أ�����д�����������������д��������ԭ�е�����
//ע:   ��������û���ȵ���__easynor_write_buf_to_flash��������дbuf�е����ݣ�����
//      ������Ҫ��һ�ε���flashд������Ч�ʵͣ����ǰ�д��������������buf�е�����
//      �ϲ�һ��д��flash.
//-----------------------------------------------------------------------------
u32 Easynor_WriteFile(struct tagFileRsc *fp, uint8_t *buf, u32 len)
{
    struct tagEasyFlashFileInfo *fileinfo;
    struct tagPTTDevice *PTT_device_tag;
    struct tagEasynorFlashChip *chip;
    u32 start_block, offset_start, end_block, loop;
    u32 write_sum, wr_point, write_len, completed = 0;

    if ((fp == NULL) || (len == 0) || (buf == NULL))
        return 0;

    PTT_device_tag = (struct tagPTTDevice *) (fp->home_PTT);
    fileinfo = (struct tagEasyFlashFileInfo *) fp->file_medium_tag;
    //�ļ�����flashоƬָ��
    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;

    //������������ߴ磬�򳬳����ֻᱻ����
    if (len <= (fileinfo->max_size - fp->file_size))
        write_sum = len;
    else
        write_sum = fileinfo->max_size - fp->file_size;
    wr_point = fp->write_ptr;
    //������ʼдλ�ô����ļ��ڵڼ����ƫ����
    start_block = (u32) (wr_point / chip->block_size)
                + fileinfo->start_block;
    offset_start = (u32) (wr_point % chip->block_size);
    //������ֹдλ�ô����ļ��ڵڼ���
    end_block = (u32) ((fp->write_ptr + len - 1) / chip->block_size)
                + start_block;
    for (loop = start_block; loop <= end_block; loop++)
    {
        //���㱾��д������ݳ���,����ﵽblockβ���������β
        if (write_sum - completed > chip->block_size - offset_start)
            write_len = chip->block_size - offset_start;
        else
            write_len = write_sum - completed;
        if (wr_point >= fp->file_size) //׷������
        {
            if (offset_start == 0) //��д��Ŀ���������Ч����
            {
                //д��Ŀ���
                __Easynor_WriteNewBlock(chip, loop, offset_start,
                            &buf[completed], write_len);
            }
            else //����׷�ӣ�����д��Ŀ��Ѿ��в�����Ч����
            {
                //д��Ŀ���
                __Easynor_WriteAppendBlock(chip, loop, offset_start,
                        &buf[completed], write_len);
            }
        }
        else //���ļ����м�д�룬�����������Ҫʹ��PCRB����б���
        {
            //д��Ŀ���
            __Easynor_WriteUpdateBlock(chip, loop, offset_start,
                        &buf[completed], write_len);
        }
        completed += write_len;
        offset_start = 0; //�ڶ��鿪ʼ����ʼƫ�Ƶ�ַ�϶���0
    }

    fp->write_ptr += completed;
    fp->read_ptr += completed;
    if (fp->file_size < fp->write_ptr)
    {
        __Easynor_ChangeFileSize(fp, fp->write_ptr);
        fp->file_size = fp->write_ptr;
    }

    return completed;
}
//--------��flash�ļ�------------------------------------------------------------
//���ܣ����ļ��е����ݶ��뵽�������У�ִ�����²���
//      1����������������û�����ݣ�������뵽���ջ�������
//      2�������������е������������û�����ݣ����flash�в����ȡ�������ղ�Խ��
//         �߽�Ϊ������޶ȵ���д��������
//      3��������ɺ��Ƿ���Ҫͬ��д��������ִ��֮
//������buf�����뻺����
//      len��������������
//      fp�����������ļ�ָ��
//���أ�ʵ�ʶ�ȡ���ֽ���
//-----------------------------------------------------------------------------
u32 Easynor_ReadFile(struct tagFileRsc *fp, uint8_t *buf, u32 len)
{
    struct tagEasyFlashFileInfo *fileinfo;
    struct tagPTTDevice *PTT_device_tag;
    struct tagEasynorFlashChip *chip;
    u32 completed = 0, read_len, result_len;
    u32 offset_start;
    u32 start_block, end_block, loop;

    if ((fp == NULL) || (len == 0) || (buf == NULL))
        return 0;
    PTT_device_tag = (struct tagPTTDevice *) (fp->home_PTT);
    fileinfo = (struct tagEasyFlashFileInfo *) fp->file_medium_tag;
    if (fp->read_ptr + len > fp->file_size)
        result_len = (u32) (fp->file_size - fp->read_ptr);
    else
        result_len = len;
    //�ļ�����flashоƬָ��
    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;
    //������ʼ��дλ�ô����ļ��ڵڼ���
    start_block = (u32) (fp->read_ptr / chip->block_size)
            + fileinfo->start_block;
    offset_start = fp->read_ptr % chip->block_size;
    //������ֹ��дλ�ô����ļ��ڵڼ���
    end_block = (u32) ((fp->read_ptr + result_len - 1) / chip->block_size)
            + fileinfo->start_block;
    for (loop = start_block; loop <= end_block; loop++)
    {
        if (result_len - completed > chip->block_size - offset_start)
            read_len = chip->block_size - offset_start;
        else
            read_len = result_len - completed;
        //����Ŀ���
        chip->read_data(loop, offset_start, buf + completed, read_len);
        completed += read_len;
        offset_start = 0; //�ڶ��鿪ʼ����ʼƫ�Ƶ�ַ�϶���0
    }
    fp->read_ptr += completed;
    return completed;
}

//----ˢд������---------------------------------------------------------------
//����: ���ļ�д������������д��flash,easynorÿ���ļ�������ֱ��д�룬����flush��
//������fp�����������ļ�ָ��
//���أ�ʵ��д��flash���ֽ���
//-----------------------------------------------------------------------------
u32 Easynor_FlushFile(struct tagFileRsc *fp)
{
    return 0;
}

//----��ѯ�ļ��ɶ�������-------------------------------------------------------
//���ܣ���ѯһ���ļ����ж��ٿɶ����ݣ����ڹ�̬�ļ�����flash�ļ�����˵�������ļ�
//      ����-��ǰָ�룬�����ı�׼���岢��һ����ˣ�����ͨ�Ŷ��ӡ����ң��ļ���д
//      ָ����α�ʾ������ļ�ϵͳ������ʵ���йأ��ļ�ϵͳ�ĺ��ĳ���Ӧ�����κ�
//      ���衣
//������fp�����������ļ�ָ��
//���أ��ļ��ɶ����ݵ��ֽ���
//-----------------------------------------------------------------------------
sint64_t Easynor_QueryFileStocks(struct tagFileRsc *fp)
{
    if (fp == NULL)
        return 0;
    return fp->file_size - fp->read_ptr;
}

//----��ѯ�ļ���д������-------------------------------------------------------
//���ܣ���ѯһ���ļ�����д��������ݣ����ڹ�̬�ļ�����flash�ļ��������ܾ����ļ�
//      ���ʣ��ռ䣬�����ı�׼���岢��һ����ˣ�����ͨ�Ŷ��ӡ����ң��ļ���ʣ��
//      �ռ���α�ʾ������ļ�ϵͳ������ʵ���йأ��ļ�ϵͳ�ĺ��ĳ���Ӧ�����κ�
//      ���衣������ΪPTT_flash_tag->free_sum�����ļ��Կ�д�����������
//      flash�ļ�ϵͳ�ļ��㷽ʽ��:
//      1���ļ��ѷ���Ŀ������-�ļ��ߴ硣
//      2�������ļ���ʣ��ռ�PTT_flash_tag->free_sum��
//      3����ȥ�ļ������������е�������
//������fp�����������ļ�ָ��
//���أ��ļ�����д����ֽ���
//-----------------------------------------------------------------------------
sint64_t Easynor_QueryFileCubage(struct tagFileRsc *fp)
{
    struct tagEasyFlashFileInfo *fileinfo;
//    struct tagPTTDevice *PTT_device_tag;
    if (fp == NULL)
        return 0;
//    PTT_device_tag = (struct tagPTTDevice *) (fp->home_PTT);
    fileinfo = (struct tagEasyFlashFileInfo *) fp->file_medium_tag;
    return fileinfo->max_size - fp->write_ptr;
}

//----�����ļ�ָ��-------------------------------------------------------------
//���ܣ����ļ�ָ�����õ�ָ����λ�ã�����Ӧ��ͬ����д�������������λ�ó����ļ���
//      �ȣ������ӳ��ļ�����ֻ��max_size=0���ļ��ܱ��ӳ���ֻ���ӳ�һ��
//������fp�����������ļ�ָ��
//      offset���ƶ���λ��
//      whence���Ӻδ���ʼ�����ƶ����ȣ��μ�SEEK_CUR�ȵĶ���
//���أ�0=�ɹ���1=ʧ��
//-----------------------------------------------------------------------------
u32 Easynor_SeekFile(struct tagFileRsc *fp, struct tagSeekPara *position)
{
    sint64_t new_position;
    uint8_t whence;
    if ((position == NULL) || (fp == NULL))
        return 1;
    whence = position->whence;
    if (whence == SEEK_SET)
        new_position = position->offset;
    else if (whence == SEEK_CUR)
        new_position = position->offset + fp->read_ptr;
    else if (whence == SEEK_END)
        new_position = fp->file_size + position->offset;
    else
        return 1;
    //�����ļ�ϵͳ���������ļ��ӳ��Ĺ���
    if (new_position > fp->file_size)
//        Easynor_SetFileSize(fp, new_position);
        return 1;
    fp->read_ptr = new_position;
    fp->write_ptr = new_position;
    return 0;
}

//----�����ļ�-----------------------------------------------------------------
//���ܣ��򴴽��ļ�ʱ��Ҫ�ƶ�max_size��������׼��djyfs_fopen�����޷�ʵ�֣����ṩ
//      ������api:easynor_create_file���ڴ����ļ�����������Ŀ¼��
//������name��Ŀ¼���ļ�����
//      parent����Ŀ¼ָ�롣
//      attr�����ļ���Ŀ¼��������
//���أ�true=�ɹ���false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t Easynor_CreateItem(char *name, struct tagFileRsc *parent,
                           struct tagFileRsc *result, union file_attrs attr,
                           enum _FILE_OPEN_MODE_ mode)
{
    struct tagEasyFlashFileInfo *fileinfo;
    struct tagEasynorFlashChip *chip;
    struct tagPTTDevice *PTT_device_tag;

    u32 block_no,start_block;
    u32 size,offset;
    u8 cfg_blocks;

    if ((parent == NULL) || (result == NULL))
        return 0;
    PTT_device_tag = (struct tagPTTDevice *) (parent->home_PTT);
    chip = (struct tagEasynorFlashChip *)PTT_device_tag->PTT_medium_tag;

    if (chip->files_sum >= CN_EASYNOR_CFG_LIMIT / CN_EASYNOR_CFG_ITEM_LIMIT - 1)
        return false; //�Ѵ�ɴ����ļ�������

    offset = (chip->files_sum * CN_EASYNOR_CFG_ITEM_LIMIT) % chip->block_size;
    block_no = (chip->files_sum * CN_EASYNOR_CFG_ITEM_LIMIT) / chip->block_size;
    block_no = chip->start_block + block_no;
    //�����ļ�ϵͳ�ļ��������������ʹ�õ�blocks
    cfg_blocks = (CN_EASYNOR_CFG_LIMIT + chip->block_size-1) / chip->block_size;

    fileinfo = &s_ptEasyNorInfo[PTT_device_tag->opened_sum];
    //���ļ�ϵͳ�ոս������������д����ļ�ʱ�������´���
    if(chip->files_sum < 1)
    {
        //��һ���ļ���ŵĵ�ַ���ڸ÷��������ĵ�һ��block��ƫ����Ϊ0
        start_block = chip->start_block + 2*cfg_blocks;
        if(start_block >= (chip->start_block + chip->block_sum))
            return false;
        offset = CN_EASYNOR_CFG_ITEM_LIMIT;
    }
    else
    {
        //���¼������е����һ���ļ����ļ�������е�λ��(��ź�ƫ��)��ע�⣬�ļ�����
        //���0����ϵͳռ��
        //�����ļ����ã�ȡ���ļ�����󳤶Ⱥͳ�ʼ��ţ����ڼ������ļ��ĳ�ʼ��š�
        chip->read_data(block_no, offset, chip->block_buf,CN_EASYNOR_CFG_ITEM_LIMIT);
        start_block = pick_little_32bit(chip->block_buf,
                                        CN_EASYNOR_STARTBLOCK_OFF >> 2);
        size = pick_little_32bit(chip->block_buf, CN_EASYNOR_MAXSIZE_OFF >> 2);

        //��һ���ļ��ߴ����ߴ�û������
        if(size == 0xFFFFFFFF)
            return false;
        //���ļ��ĳ�ʼ���,easyfile��ÿ���ļ�ռ��һ��block
        start_block += (size + chip->block_size - 1) / chip->block_size;

        //���¼������ļ����ļ�������е�λ��(��ź�ƫ��)
        offset += CN_EASYNOR_CFG_ITEM_LIMIT;
        if (offset >= chip->block_size)
        {
            offset = 0;
            block_no++;
        }
    }
    //���³�ʼ���ļ������д��flash
    memset((char *)chip->block_buf, 0xff, CN_EASYNOR_CFG_ITEM_LIMIT);
    strncpy((char *)chip->block_buf, name, CN_EASYNOR_PTT_NAME_LIMIT);
    fill_little_32bit(chip->block_buf, CN_EASYNOR_STARTBLOCK_OFF >> 2,
            start_block);
    fill_little_32bit(chip->block_buf, CN_EASYNOR_MAXSIZE_OFF >> 2, 0xFFFFFFFF);
    fill_little_32bit(chip->block_buf, CN_EASYNOR_FILESIZE_OFF >> 2, 0);
    fill_little_32bit(chip->block_buf, (CN_EASYNOR_FILESIZE_OFF >> 2) + 1, ~0);
    memcpy(chip->block_buf + CN_EASYNOR_VERIFITY_OFF, "easyfile", 8);
    chip->write_data(block_no, offset, chip->block_buf,CN_EASYNOR_CFG_ITEM_LIMIT);
    chip->write_data(block_no+cfg_blocks,offset,chip->block_buf,CN_EASYNOR_CFG_ITEM_LIMIT);

    chip->files_sum++;
    //�����ļ���Ϣ����������
    result->file_medium_tag = (ptu32_t)fileinfo;
    result->file_size = 0;
    strncpy(result->name, name, CN_EASYNOR_FILENAME_LIMIT);
    result->read_ptr = 0;
    result->write_ptr = 0;
    result->file_attr = attr;

    //�����ļ���Ϣ����������
    fileinfo->filesize = 0;
    fileinfo->max_size = 0;
    fileinfo->start_block = start_block;
    fileinfo->start_item = chip->files_sum;

    return true;
}

//----�ƶ���Ŀ���ļ�/Ŀ¼��----------------------------------------------------
//���ܣ��̶�ֻ�������ļ�����֧���ƶ�
//������name��Ŀ¼���ļ�����
//      parent����Ŀ¼ָ�롣
//      attr�����ļ���Ŀ¼��������
//���أ�true=�ɹ���false=ʧ�ܣ�һ������Ϊ�ļ���û���㹻����������
//-----------------------------------------------------------------------------
bool_t Easynor_MoveFile(struct tagFileRsc *src_fp, struct tagFileRsc *dest_fp)
{
    return false;
}

//----ɾ����Ŀ���ļ�/Ŀ¼��---------------------------------------------------
//���ܣ�ɾ�������ļ�ϵͳ���ļ��б�����һ���ļ���ɾ������ļ������״̬��δ����
//      ʱһ�£���flash�иõ�ַ��Ϊȫ0xFF
//������fp����ɾ����Ŀ¼���ļ���ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t Easynor_RemoveFile(struct tagFileRsc *fp)
{
    struct tagEasyFlashFileInfo *fileinfo;
    struct tagEasynorFlashChip *chip;
    struct tagPTTDevice *PTT_device_tag;

    u32 block_no,offset;
    u8 cfg_blocks;

    if (fp == NULL)
        return 0;
    PTT_device_tag = (struct tagPTTDevice *) (fp->home_PTT);
    chip = (struct tagEasynorFlashChip *)PTT_device_tag->PTT_medium_tag;
    fileinfo = (struct tagEasyFlashFileInfo *) fp->file_medium_tag;

    //�ж��Ƿ�Ϊ���һ���ļ�,easynor file sysֻ����ɾ�����һ���ļ�
    if(fileinfo->start_item != chip->files_sum)
    {
        printf("�ļ������ļ��б�����һ���ļ�������������\r\n");
        return false;
    }

    //��Ϊ���һ���ļ������ɾ��
    chip->files_sum --;

    cfg_blocks = (CN_EASYNOR_CFG_LIMIT+ chip->block_size-1) / chip->block_size;
    //������ļ�����������ڵ�block
    block_no = (fileinfo->start_item * CN_EASYNOR_CFG_ITEM_LIMIT) / chip->block_size;
    block_no = block_no + chip->start_block;

    //���ļ�����������block�е�ƫ��
    offset = (fileinfo->start_item * CN_EASYNOR_CFG_ITEM_LIMIT) % chip->block_size;

    //�Ƚ���block�е����ݶ��������������ļ����������д0xFF,
    chip->read_data(block_no, 0, chip->block_buf, chip->block_size);
    memset(chip->block_buf + offset,0xFF,CN_EASYNOR_CFG_ITEM_LIMIT);

    //���������鲢д��
    chip->erase_block(block_no);
    chip->write_data(block_no, 0, chip->block_buf, chip->block_size);
    chip->erase_block(block_no + cfg_blocks);
    chip->write_data(block_no + cfg_blocks, 0, chip->block_buf, chip->block_size);

    return true;
}

//----��ȡ�ļ�(Ŀ¼)����-------------------------------------------------------
//���ܣ����ļ����������ڴ澵���е��ж�ȡ�ļ�(Ŀ¼)��Ϣ������ʼ���ļ��ṹָ��fp��
//������PTT_flash_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//      file_num���ڸ��ļ�ϵͳ�и��ļ���
//      current�������ļ�(Ŀ¼)���Ե��ļ�ָ��
//���أ�true=�ɹ���false=ʧ��
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: �޸��ļ�ʱ�����Ը�ʽ
//   ����: ������
//-----------------------------------------------------------------------------
bool_t __Easynor_ReadItem(struct tagPTTDevice *PTT_device_tag,
                           uint32_t file_num,
                           struct tagFileRsc *current)
{
    u32 block_no,offset,loop;
    u8 *hsize;
    struct tagEasynorFlashChip *chip;

    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;

    //������ļ�����������ڵ�block
    block_no = (file_num * CN_EASYNOR_CFG_ITEM_LIMIT)/chip->block_size;
    block_no = block_no + chip->start_block;

    //���ļ�����������block�е�ƫ��
    offset = (file_num * CN_EASYNOR_CFG_ITEM_LIMIT) % chip->block_size;
    //�����ļ���Ϣ
    chip->read_data(block_no, offset,chip->block_buf,CN_EASYNOR_CFG_ITEM_LIMIT);

    //���������ļ���Դ����������struct file_rsc������Ҫ������ģ��ά���Ĳ��֡�
    current->file_attr.bits.folder = 0;
    current->file_attr.bits.archive = 1;
    current->file_semp = NULL;
    current->p_read_buf = NULL;
    current->p_write_buf = NULL;
    //�ļ��ĸò�����Ϣeasyfile��δʵ�֣���˸�ֵΪ�Ϸ�ֵ
#if 0/* �ļ�ʱ���ʽ�ı�,��ʱ���� */
    current->year_high_modify = 0x07;
    current->year_low_modify  = 0xdd;
    current->month_modify     = 1;
    current->date_modify      = 1;
    current->hour_modify      = 9;
    current->minute_modify    = 0;
#endif

    //�ļ���С
    for (loop = 0; loop < CN_EASYNOR_FILESIZE_NUM; loop++)
    {
        hsize = (u8*)(chip->block_buf + CN_EASYNOR_FILESIZE_END - loop * 8);
        if ((pick_little_64bit(hsize,0) == CN_LIMIT_UINT64) ||
            (pick_little_64bit(hsize,0) == 0))
            continue;
        if (pick_little_32bit(hsize,0) == ~(pick_little_32bit(hsize + 4,0)))
        {
            current->file_size = pick_little_32bit(chip->block_buf,
                    (CN_EASYNOR_FILESIZE_END - loop * 8) >> 2);
            break;
        }
    }
    if(loop == CN_EASYNOR_FILESIZE_NUM)//û�еõ���ȷ���ļ���С
        return false;

    current->read_ptr = 0;
    current->write_ptr = 0;

    loop = 0;
    while((chip->block_buf[loop]) && (loop++ < CN_EASYNOR_FILENAME_LIMIT));
    if(loop >= CN_EASYNOR_FILENAME_LIMIT)//�ļ�������
        return false;
    strncpy((char *)current->name, (char *)chip->block_buf, CN_EASYNOR_FILENAME_LIMIT);
    return true;
}

//----����һ��Ŀ¼������-------------------------------------------------------
//����: ��֧��
//����: parent,��Ҫ��������֦�����Ƚ��
//      current_file,��ǰ����λ��,�����ʾ��һ�β���
//����: ������������ָ��(�������ļ���Ŀ¼)
//��ע: ���������������Ϊwindows�� FindFirstFile��������������ϱ�����
//      ��Ŀ¼�µ��ļ�
//      ��һ�α���ʱ(��current==NULL)Ϊ��־��ϵͳ��Ϊstruct file_rsc�ṹ�����ڴ棬
//      ���һ�Σ����ͷ��ڴ档����û���ȷ�ص���easynor_item_traversal_son������
//      ����Ŀ¼���ڴ������ȫ����������Ϊstruct file_rsc�ṹ������ڴ潫��
//      Ҫ�û��ֶ��ͷ�
//---------------------------------------------------------------------
struct tagFileRsc * Easynor_ItemTraversalSon(struct tagFileRsc *parent,
                                             struct tagFileRsc *current)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagEasynorFlashChip *chip;

    if ((parent == NULL))
        return NULL;
    if(parent->file_medium_tag == CN_LIMIT_UINT32)
        return NULL;
    PTT_device_tag = (struct tagPTTDevice *)(parent->home_PTT);
    //�ļ�����flashоƬָ��
    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;

    if(chip->files_sum == 0)    //��û�н����ļ�
        return NULL;

    if(current == NULL)
    {
        current = M_MallocLc(sizeof(struct tagFileRsc),CN_TIMEOUT_FOREVER);
        if(current == NULL)
            return NULL;
        memset(current,0,sizeof(struct tagFileRsc));
        if(__Easynor_ReadItem(PTT_device_tag,1,current))//����һ���ļ���������Ϣ
            current->file_medium_tag = 1;//��һ���ļ�
        else
        {
            free(current);
            return NULL;
        }
    }
    else if(current->file_medium_tag < chip->files_sum)//medium_tag��ʱ����ļ���
    {
        current->file_medium_tag ++;
        if(false == __Easynor_ReadItem(PTT_device_tag,
                    current->file_medium_tag,current))
        {
            free(current);
            return NULL;
        }

    }
    else if(current->file_medium_tag >= chip->files_sum)//���������ֵ
    {
        free(current);
        return NULL;
    }

    return current;
}

//----�򿪣��ļ�/Ŀ¼��--------------------------------------------------------
//���ܣ����ļ����в��Ҳ����ļ�(Ŀ¼)��������ļ�������ҪΪ�������д��������ֻ
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
u32 Easynor_OpenItem(char *name, struct tagFileRsc *parent,
                      struct tagFileRsc *result, enum _FILE_OPEN_MODE_ mode)
{
    u32 loop, block_no, block_offset;
    u8 *hsize;
    struct tagPTTDevice *PTT_device_tag;
    struct tagEasynorFlashChip *chip;
    struct tagEasyFlashFileInfo *fileinfo;
    bool_t found = false;

    if ((parent == NULL) || (result == NULL))
        return CN_LIMIT_UINT32;
    PTT_device_tag = (struct tagPTTDevice *)(parent->home_PTT);
    //�ļ�����flashоƬָ��
    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;
    fileinfo = &s_ptEasyNorInfo[PTT_device_tag->opened_sum];
    for (loop = 1; loop <= chip->files_sum; loop++)
    {
        block_no = loop * CN_EASYNOR_CFG_ITEM_LIMIT;
        block_offset = block_no % chip->block_size;
        block_no = chip->start_block + block_no / chip->block_size;
        chip->read_data(block_no, block_offset, chip->block_buf,
                CN_EASYNOR_PTT_NAME_LIMIT);
        chip->block_buf[CN_EASYNOR_PTT_NAME_LIMIT] = '\0';
        if (strncmp((const char*)chip->block_buf,(const char*)name,
                CN_EASYNOR_PTT_NAME_LIMIT) == 0)
        {
            chip->read_data(block_no, block_offset, chip->block_buf,
                    CN_EASYNOR_CFG_ITEM_LIMIT);
            found = true;
            fileinfo->start_item = loop;
            break;
        }
    }
    if (!found)
    {
        return CN_FS_ITEM_INEXIST;
    }
    else
    {
        //���������ļ���Դ����������struct file_rsc������Ҫ������ģ��ά���Ĳ��֡�
        result->file_attr.bits.archive = 1;
        result->file_semp = NULL;
        result->file_medium_tag = (ptu32_t) fileinfo;
        result->p_read_buf = NULL;
        result->p_write_buf = NULL;
        for (loop = 0; loop < CN_EASYNOR_FILESIZE_NUM; loop++)
        {
            hsize = (chip->block_buf + CN_EASYNOR_FILESIZE_END - loop * 8);
            if ((pick_little_64bit(hsize,0) == CN_LIMIT_UINT64) ||
                (pick_little_64bit(hsize,0) == 0))
                continue;
            if (pick_little_32bit(hsize,0) == ~(pick_little_32bit(hsize + 4,0)))
            {
                result->file_size = pick_little_32bit(chip->block_buf,
                        (CN_EASYNOR_FILESIZE_END - loop * 8) >> 2);
                break;
            }
        }

        result->read_ptr = 0;
        result->write_ptr = 0;
        fileinfo->start_block = pick_little_32bit(chip->block_buf,
                CN_EASYNOR_STARTBLOCK_OFF >> 2);
        fileinfo->max_size = pick_little_32bit(chip->block_buf,
                CN_EASYNOR_MAXSIZE_OFF >> 2);
        strncpy(result->name, name, CN_EASYNOR_FILENAME_LIMIT);

        return CN_FS_OPEN_SUCCESS;
    }
}

//----���ң��ļ�/Ŀ¼��--------------------------------------------------------
//���ܣ����ļ����в��Ҳ����ļ�(Ŀ¼)��������ļ�������ҪΪ�������д��������
//������name���ļ���Ŀ¼����
//      parent���ļ�/Ŀ¼�ĸ�Ŀ¼��
//      result�������ҵ����ļ�����Ϣ
//���أ�true=�ļ�/Ŀ¼���ڣ�����result�з����ļ���Ϣ(�������ļ�������)��
//      false = �ļ�/Ŀ¼�����ڣ����޸�result��
//ע��easynorflash �ļ�ϵͳ��ʹ�û������Լ������ļ�ģʽ�����ļ��Ͳ����ļ��Ĳ���
//      ����ͬ�ġ�
//----------------------------------------------------------------------------
bool_t Easynor_LookforItem(char *name, struct tagFileRsc *parent,
                            struct tagFileRsc *result)
{
    if(Easynor_OpenItem(name,parent,result, 0)== CN_FS_OPEN_SUCCESS)
        return true;
    else
        return false;

}

//----�ر��ļ�(Ŀ¼)-----------------------------------------------------------
//����: �ر�һ���򿪵��ļ�(Ŀ¼)��������ļ������ͷŶ�д����������ʹ�ļ�����
//      opened_sum�����������Ŀ¼����ֻ��opened_sum������������ֻ�Ǵ���رչ���
//      ��洢������صĲ�����ϵͳ��Դ������file.cģ�鴦��
//������fp�����������ļ�ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t Easynor_CloseItem(struct tagFileRsc *fp)
{
    return true;
}

//----�ļ���Ŀ¼������---------------------------------------------------------
//���ܣ��޸�һ���ļ���Ŀ¼�������֣������ֱ�����fp�С�fp�е����ֿ��Դﵽ255��
//      ������ֻ��ǰcn_flash_file_name_limit�ַ���Ч��
//������fp�����������ļ�ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t Easynor_RenameItem(struct tagFileRsc *fp, char *newname)
{
    return false;
}

//----��ѯ�ļ������-----------------------------------------------------------
//����: �����ļ����ܿռ䣬��Ч�ռ䣬���пռ���ֽ���
//����:
//����: ��
//-----------------------------------------------------------------------------
void Easynor_CheckPTT(struct tagPTTDevice *PTT_device_tag,
                       sint64_t *sum_size, sint64_t *valid_size, sint64_t *free_size)
{
    if (sum_size != NULL)
        *sum_size = 0;
    if (valid_size != NULL)
        *valid_size = 0;
    if (free_size != NULL)
        *free_size = 0;
}

//----�����ļ�����-------------------------------------------------------------
//���ܣ������ļ����ȣ�easyfileģ��ֻʵ����filersc��Ҫ��Ĳ��ֹ��ܣ�ֻ��Ϊ���һ
//      ���������ļ�����ռ䣬��ֻ�ܷ���һ�Ρ�
//������fp�����������ļ�ָ��
//      new_size���µ��ļ����ȡ�
//���أ��µ��ļ�����,�ļ�������ȷ�򷵻�0
//-----------------------------------------------------------------------------
sint64_t Easynor_SetFileSize(struct tagFileRsc *fp, s64 new_size)
{
     struct tagEasynorFlashChip *chip;
     struct tagPTTDevice *PTT_device_tag;
     struct tagEasyFlashFileInfo *fileinfo;
     u32 block_no,offset,cfg_blocks;
     u8 buf[4];

    if (fp == NULL)
        return CN_LIMIT_UINT32;

    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    //�ļ�����flashоƬָ��
    chip = (struct tagEasynorFlashChip *) PTT_device_tag->PTT_medium_tag;
    fileinfo = (struct tagEasyFlashFileInfo *)fp->file_medium_tag;

    //item������Ϊ0
    if(fileinfo->start_item == 0)
        return -1;

    //�������һ���ļ�����ֻ�޸����ļ���С
    if(fileinfo->start_item != chip->files_sum)
    {
        __Easynor_ChangeFileSize(fp,new_size);
        return 0;
    }

    //������ļ�����������ڵ�block
    block_no = (fileinfo->start_item * CN_EASYNOR_CFG_ITEM_LIMIT) / chip->block_size;
    block_no = block_no + chip->start_block;
    //�����ļ�ϵͳ�ļ��������������ʹ�õ�blocks
    cfg_blocks = (CN_EASYNOR_CFG_LIMIT + chip->block_size-1) / chip->block_size;

    //���ļ�����������block�е�ƫ��
    offset = (fileinfo->start_item * CN_EASYNOR_CFG_ITEM_LIMIT)% chip->block_size;

    fill_little_32bit(buf, 0, (u32)new_size);

    //�մ������ļ������ߴ类��ʼ��Ϊ0xFFFFFFFF����ˣ�������Ҫ��������˵��
    //�˴κ���������Ϊ���޸��ļ������ߴ�
    if(chip->query_block_ready(block_no,offset+CN_EASYNOR_MAXSIZE_OFF,buf,sizeof(buf)))
    {
        fileinfo->max_size = new_size;
        chip->write_data(block_no,offset+CN_EASYNOR_MAXSIZE_OFF,buf,sizeof(buf));
        chip->write_data(block_no+cfg_blocks,offset+CN_EASYNOR_MAXSIZE_OFF,buf,sizeof(buf));
    }
    else//����Ҫ��������˵���˴κ����������޸����һ���ļ��ĳߴ�
    {
        __Easynor_ChangeFileSize(fp,new_size);
    }
    return 0;
}

//----��ѯĿ¼��ߴ�-----------------------------------------------------------
//���ܣ���ѯһ���ļ����Ŀ¼��ߴ磬���ֽڼ��㡣��Ӧ�ó�����Ҫ��ȡ����Ŀ¼��ʱ��
//      �������ñ���������Ŀ¼��ߴ磬Ȼ����ܷ����ڴ档
//������PTT_device_tag�����������ļ���ָ�룬ͨ�ýṹ����flashר�á�
//���أ�Ŀ¼��ߴ�
//-----------------------------------------------------------------------------
u32 Easynor_CheckFDT_Size(struct tagPTTDevice *PTT_device_tag)
{
    return 0;
}
//----����ļ��е����ļ���Ŀ¼����---------------------------------------------
//���ܣ�����ļ��е����ļ���Ŀ¼��������������Ŀ¼������Ŀ
//������PTT_device_tag�����������ļ���ָ�룬ͨ�ýṹ����flashר�á�
//      fp��Ŀ���ļ��е��ļ�ָ��
//���أ����ļ���Ŀ¼����
//-----------------------------------------------------------------------------
u32 Easynor_CheckFolder(struct tagFileRsc *parent)
{
    return 0;
}

