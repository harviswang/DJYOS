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
//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:flash�ļ�ϵͳ��MDR����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "endian.h"
#include "lock.h"
#include "file.h"
#include "djyos.h"
#include "systime.h"
#include "flashfile.h"
#include "ddr.h"
#include "mdr.h"

bool_t __check_word(const char *word);

//----��ʼ��flashоƬ---------------------------------------------------------
//���ܣ���оƬ���ΪdjyosfsоƬ������������������
//����: chip����������оƬ
//      reserves������������
//����: true=��ʼ���ɹ���false=ʧ��
//---------------------------------------------------------------------------
void DFFSD_InitMDR(struct tagFlashChip *chip,uint32_t reserves)
{
    uint8_t MDR_CDR_buffer[CN_MDR_CDR_SIZE];
    struct tagMDR_CDR_Mirror *MDR_CDR = (struct tagMDR_CDR_Mirror *)MDR_CDR_buffer;
    uint32_t valid_blocks = 1;  //�׿�϶��ǿ��õģ��ʳ�ֵ=1
    uint32_t i,MDR_bak_block = 0;
    //ע�⣬nandflashֻ���׿��Ǳ�֤��Ч�ģ���MDR��ߴ粻�ܳ����׿飬�ⲻ�����⣬
    //ĿǰΪֹnandflash��block_sizeû��С��4K�ġ�
    //��norflash��û��������ƣ�norflash��֤��Ƭ���Ǻõġ�
    for(i = 1; i < chip->block_sum; i++)
    {    //��һ���ÿ���ΪMDR���ÿ�
        if(chip->check_block(i))
        {
            valid_blocks++;
        }
    }
    MDR_bak_block = __DFFSD_Probe_MDR_Bak(chip);
    chip->valid_sum = valid_blocks;
    chip->rev_blocks = reserves;    //����������
    //��������ʼ���
    chip->rev_start = MDR_bak_block
                      +(CN_MDR_SIZE + chip->block_size -1) / chip->block_size;
    chip->PTT_start = chip->rev_start + reserves;   //������ʼ���
    if(valid_blocks <= chip->PTT_start)
        return;     //��Ч����̫��
    chip->MDR.MDR_bak_block = MDR_bak_block;
    chip->MDR.roll_next_main = CN_ROLL_OFFSET;
    chip->MDR.roll_next_bak = CN_ROLL_OFFSET;
    for(i = 0; i < CN_PTT_SUM_MAX; i++)
        chip->MDR.PTT_created[i] = false;
    strcpy((char*)MDR_CDR_buffer,"djyosfs");
    fill_little_32bit(MDR_CDR->chip_CDR_size,0,CN_MDR_CDR_SIZE);
    fill_little_32bit(MDR_CDR->chip_block_sum,0,chip->block_sum);
    fill_little_32bit(MDR_CDR->chip_block_size,0,chip->block_size);
    fill_little_32bit(MDR_CDR->chip_MDR_bak,0,MDR_bak_block);
    fill_little_32bit(MDR_CDR->chip_reserve,0,reserves);
    __DFFSD_Fill_ECC_MDR_CDR(MDR_CDR_buffer);
    __DFFSD_EraseMDR_Main(chip);
    __DFFSD_WriteMDR_Main(chip,MDR_CDR_buffer,
                (uint32_t)MDR_CDR->chip_MDR_CDR_ECC-(uint32_t)MDR_CDR +4);

    __DFFSD_WriteEndFlagMain(chip);

    __DFFSD_EraseMDR_Bak(chip);
    __DFFSD_WriteMDR_Bak(chip,MDR_CDR_buffer,
                (uint32_t)MDR_CDR->chip_MDR_CDR_ECC-(uint32_t)MDR_CDR +4);
    __DFFSD_WriteEndFlagBak(chip);

}

//----��MDR_bak��-------------------------------------------------------------
//����: ��MDR�������������
//����: chip��Ŀ��оƬ��chip->bak_start�����Ѿ���ʼ��
//      buf�������������ݵĻ�����
//����: true=��ȷ��ȡ��false=����
//-----------------------------------------------------------------------------
bool_t __DFFSD_ReadMDR_Bak(struct tagFlashChip *chip,uint8_t *buf)
{
    uint32_t complete_len=0,read_len;
    uint32_t block_no = chip->MDR.MDR_bak_block;
    do  //��ѭ�����ڶ�ȡ����MDR_main����������MDR_size���ܴ���1�飬��ѭ����
    {
        if((CN_MDR_SIZE-complete_len) > chip->block_size)
            read_len = chip->block_size;
        else
            read_len = CN_MDR_SIZE-complete_len;
        //MDR�������¼����У�飬������У�飬��ÿ�����Ӽ�¼��Ҫ��У���룬������
        chip->read_data_no_ecc(block_no,0,buf+complete_len,read_len);
        block_no++;
        complete_len += read_len;
    }while(complete_len < CN_MDR_SIZE);
    return true;
}

//----��MDR_main��-------------------------------------------------------------
//����: ��MDR�������������
//����: chip��Ŀ��оƬ
//      buf�������������ݵĻ�����
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_ReadMDR_Main(struct tagFlashChip *chip,uint8_t *buf)
{
    uint32_t complete_len=0,read_len;
    uint32_t block_no = 0;      //MDR_main��0��ʼ�洢
    do  //��ѭ�����ڶ�ȡ����MDR_main����������MDR_size���ܴ���1�飬��ѭ����
    {
        if((CN_MDR_SIZE-complete_len) > chip->block_size)
            read_len = chip->block_size;
        else
            read_len = CN_MDR_SIZE-complete_len;
        //MDR�������¼����У�飬������У�飬��ÿ�����Ӽ�¼��Ҫ��У���룬������
        chip->read_data_no_ecc(block_no,0,buf+complete_len,read_len);
        block_no++;
        complete_len += read_len;
    }while(complete_len < CN_MDR_SIZE);
    return;
}

//----����һ���洢����---------------------------------------------------------
//����: �����·���ʱ�����ô˺����Ӵ洢�����з���洢����
//����: chip��Ŀ��оƬ
//      size��������Ŀ�����
//      result�����ط�����
//����: true=����ɹ������������result�У�false = �����ʧ��
////---------------------------------------------------------------------------
bool_t __DFFSD_Allocate_PTT(struct tagFlashChip *chip,
                            uint32_t size,uint32_t *result)
{
    //uint8_t MDR_buf[CN_MDR_SIZE];
    //������ʼ�����飬���ڲ�ѯ���пռ䣬+2��Ϊ�������ܴ��ڵ���β������
    uint32_t block_start[CN_PTT_SUM_MAX+2];
    //�������������飬���ڲ�ѯ���пռ䣬+2��Ϊ�������ܴ��ڵ���β������
    uint32_t block_end[CN_PTT_SUM_MAX+2];
    struct tagMDR_PTT_Mirror *MDR_PTT;
    uint32_t loop,loop1=0,temp;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
        return false;
    __DFFSD_ReadMDR_Main(chip,MDR_buf); //����MDR��������(ԭ��������������)
    block_start[0] = chip->PTT_start;
    block_end[0] = chip->PTT_start;
    //MDR_PTTָ��ָ��MDR�������е�һ��PTT�������ڼ���PTT���Ƿ����
    MDR_PTT = (struct tagMDR_PTT_Mirror *)(MDR_buf+CN_MDR_CDR_SIZE);

    for(loop = 1; loop < CN_PTT_SUM_MAX+1; loop++)
    {//�������Ѿ������ķ�������ʼ��źͽ�����ţ����浽block_start��block_end
    //�������������Ԫ�رȷ������������������ڱ��������β���������
        if(chip->MDR.PTT_created[loop-1])
        {//���Ѿ������ķ�����������MDR_PTT�����ж�����ʼ�ͽ������

            block_start[loop] = pick_little_32bit(MDR_PTT->PTT_start_block,0);
            //block_start[loop] = block_end[loop-1]
            //                + pick_little_32bit(MDR_PTT->PTT_blocks_sum,0);
            //������ţ��ÿ��Ѿ������ڱ������ˡ�
            block_end[loop] = block_start[loop]
                            + pick_little_32bit(MDR_PTT->PTT_blocks_sum,0);
        }else
        {//Ϊ�����ķ�������ʼ�ͽ�����ž���ʼ��Ϊ������ֵ��ѽ����ķ�����
        //��ʼ�ͽ�����ţ���Ч�ڱ������ǿյġ�
            block_start[loop] = block_end[loop-1];
            block_end[loop] = block_end[loop-1];
        }
        MDR_PTT++;  //������ָ����һ��������MDR_PTT
    }

    block_start[CN_PTT_SUM_MAX+1] = chip->block_sum;//оƬβ�������������ʼ���
    block_end[CN_PTT_SUM_MAX+1] = chip->block_sum;  //оƬβ����������Ľ������
    for(loop = 0; loop <CN_PTT_SUM_MAX+1; loop++)
    {//��ð�ݷ��Ը���������ʼ����Ŷ�(���ǿ��п�ߴ�)��оƬ����ʹ�ú���ܷ���
    //������������ռ�õĴ洢��λ�ò�������PTT��MDR�е�˳����ͬ��ֻ�а����ַ
    //����󣬲�����Чȷ�������ÿռ�ĳߴ硣
        for(loop1 = loop+1; loop1 < CN_PTT_SUM_MAX+2; loop1++)
        {
            if(block_start[loop] > block_start[loop1])
            {
                temp = block_start[loop];
                block_start[loop] = block_start[loop1];
                block_start[loop1] = temp;
                temp = block_end[loop];
                block_end[loop] = block_end[loop1];
                block_end[loop1] = temp;
            }
        }
    }
    //��һ������Ҫ����������пռ�
    for(loop = 0; loop < CN_PTT_SUM_MAX+1; loop++)
    {

        if(!chip->MDR.PTT_created[loop])
        {
            if(block_start[loop+1] - block_end[loop] >= size)
            {
                *result = block_start[loop];  //�ҵ�����Ҫ��Ŀռ�
                    break;
               }
        }
    }
    free(MDR_buf);
    if(loop == CN_PTT_SUM_MAX +1)
        return false;   //û���ҵ�����Ҫ���������������
    else
        return true;
}

//----д��һ��MDR_PTT��--------------------------------------------------------
//����: ��һ��MDR_PTT��д��flash�У���ͬʱд��MDR_main��MDR_bak��
//����: chip��Ŀ��оƬ
//      PTT_no��������
//      MDR_PTT_buf���Ѿ����õĻ�����
//����: true=�ɹ���false=ʧ��
//ע������д���Ƿ���ȷ��У�鹦��----db
//-----------------------------------------------------------------------------
bool_t __DFFSD_write_MDR_PTT(struct tagFlashChip *chip,
                           uint32_t PTT_no,uint8_t *MDR_PTT_buf)
{
    struct tagMDR_TempRecord MDR_record;
    uint32_t block_offset,block_no;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
        return false;
    //��MDR_PTT��MDR���е�ƫ����(�ֽ�)
    block_offset = CN_MDR_CDR_SIZE + PTT_no * CN_MDR_PTT_SIZE;
    block_no = block_offset / chip->block_size;//��MDR_PTT��MDR���еĿ��
    block_offset = block_offset % chip->block_size; //����ƫ����
    //���MDR_main�Ƿ���Ҫ����
    if(chip->query_block_ready_no_ecc(
                            block_no,block_offset,MDR_PTT_buf,CN_MDR_PTT_SIZE))
    {//MDR_main�и�PTT���������
        __DFFSD_WriteMDR_PTT_Main(chip,MDR_PTT_buf,PTT_no);  //ֱ��д��MDR_main
        //�������MDR_PTTд�뵽MDR_bak��
        block_no += chip->MDR.MDR_bak_block;    //ȡMDR_bak���
        //���MDR_bak�Ƿ���Ҫ����
        if(chip->query_block_ready_no_ecc(
                            block_no,block_offset,MDR_PTT_buf,CN_MDR_PTT_SIZE))
        {//MDR_bak�и�PTT���������
            __DFFSD_WriteMDR_PTT_Bak(chip,MDR_PTT_buf,PTT_no);
        }else
        {//MDR_bak�и�PTT����Ҫ����
            __DFFSD_ReadMDR_Main(chip,MDR_buf); //������MDR��
            __DFFSD_UasmPTT(chip,&MDR_record,MDR_buf);
            //�ӻ�������ȡ������������Ч���ݣ����浽MDR_record�У����·���
            //��δ��ɳ�ʼ������������û�и÷��������ݡ�
            __DFFSD_UasmRoll(chip,MDR_buf,
                              &MDR_record,chip->MDR.roll_next_main);
            memcpy(MDR_buf+CN_MDR_CDR_SIZE + PTT_no * CN_MDR_PTT_SIZE,
                        MDR_PTT_buf,CN_MDR_PTT_SIZE);
            __DFFSD_EraseMDR_Bak(chip);
            __DFFSD_WriteMDR_Bak(chip,MDR_buf,CN_MDR_AND_PTT_SIZE);
            __DFFSD_WriteRollBak(chip,&MDR_record);
            __DFFSD_WriteEndFlagBak(chip);
        }
    }
    else
    {//MDR_main�и�PTT����Ҫ����
        __DFFSD_ReadMDR_Main(chip,MDR_buf);
        __DFFSD_UasmPTT(chip,&MDR_record,MDR_buf);
        __DFFSD_UasmRoll(chip,MDR_buf,&MDR_record,chip->MDR.roll_next_main);
        memcpy(MDR_buf+CN_MDR_CDR_SIZE + PTT_no * CN_MDR_PTT_SIZE,
                            MDR_PTT_buf,CN_MDR_PTT_SIZE);
        __DFFSD_EraseMDR_Bak(chip);
        __DFFSD_WriteMDR_Bak(chip,MDR_buf,CN_MDR_AND_PTT_SIZE);
        __DFFSD_WriteRollBak(chip,&MDR_record);
        __DFFSD_WriteEndFlagBak(chip);

        __DFFSD_EraseMDR_Main(chip);
        __DFFSD_WriteMDR_Main(chip,MDR_buf,CN_MDR_AND_PTT_SIZE);
        __DFFSD_WriteRollMain(chip,&MDR_record);
        __DFFSD_WriteEndFlagMain(chip);
    }
    free(MDR_buf);
    return true;
}
//----����һ������----------------------------------------------------------
//����: ��ָ��оƬ������һ������,�·���ֻ�ǵǼ���оƬ�ϣ���δ����������ϵͳ��
//      ��Ҫ���أ�������������ϵͳ��
//����: chip����������оƬ
//      size�������ӵķ����ߴ�(�ÿ�����ʾ)
//      name�������ӵķ�������
//      nand_ecc���������Ƿ���ҪeccУ��
//����: true = �ɹ���false = ʧ�ܣ�ʧ��ԭ��һ���Ǵ洢�ռ䲻����
//------------
//���¼�¼:
// 1.����: 2015/3/10
//   ˵��: ���ӷ������Ϸ��Լ�顣
//   ����: ������
// 2.����: 2015/3/17
//   ˵��: 1.������MDR_PTT->PTT_name�����ж���
//         2.����PTT_device_tag�������ͷŵ�bug��
//         3.����__DFFSD_write_MDR_PTTд�����
//         4.ɾ���ظ���ֵ������
//         5.���Ӷ�MDR_PTT_buf[]�ĳ�ʼ��
//   ����: ������
//-----------------------------------------------------------------------------
bool_t DFFSD_Create_PTT(struct tagFlashChip *chip, uint32_t size,char *name)
{
    struct tagStPTT_Flash *PTT_flash_tag = NULL;
    struct tagPTTDevice *PTT_device_tag = NULL;
    //djy_handle_t fs_handle_right = NULL;
    struct tagMDR_PTT_Mirror *MDR_PTT;
    struct tagRscNode *fs_rsc_node;
    uint32_t PTT_start,PTT_no,name_len,loop,table_size,DDR_blocks;
    uint8_t MDR_PTT_buf[CN_MDR_PTT_SIZE]={0};

    if((chip == NULL) || (size == 0) || (name == NULL))
        return false;
    name_len = strnlen(name,CN_FLASH_PTT_NAME_LIMIT+1);  //����ַ������ȣ�
    if(name_len == 0)               //����������Ϊ0���Ƿ�
        return false;
    if(!__check_word(name)) //�������Ϸ��Լ��
        return false;
    for(loop = 0; loop < CN_PTT_SUM_MAX; loop++)//����Ƿ񻹿��Խ����µķ���
    {
        if(!chip->MDR.PTT_created[loop])
        {
            PTT_no = loop;      //��loop������δ��������MDR_PTT�����ǿյ�
            break;              //Ϊ�·����ҵ�MDR_PTT����˳�ѭ��
        }
    }
    if(loop == CN_PTT_SUM_MAX)
        return false;   //MDR���Ѿ�û�п��е�PTT��

    //���³�ʼ���·�����MDR_PTT����
    MDR_PTT = (struct tagMDR_PTT_Mirror*)MDR_PTT_buf;

    if(name_len <= CN_FLASH_PTT_NAME_LIMIT)     //copy������
    {
        strcpy(MDR_PTT->PTT_name,name);
    }
    else
    {
        memcpy(MDR_PTT->PTT_name,name,CN_FLASH_PTT_NAME_LIMIT);
        MDR_PTT->PTT_name[CN_FLASH_PTT_NAME_LIMIT] = '\0';
    }
    Lock_SempPend(Fs_Semaphore,CN_TIMEOUT_FOREVER);
    fs_rsc_node = Rsc_SearchTree("fs"); //ȡ�ļ�ϵͳ�豸����Դ�ڵ�ָ��
    if(fs_rsc_node == NULL)
    {
        Lock_SempPost(Fs_Semaphore);
        return false;
    }
    //�ڷ����豸�в���ΪMDR_PTT->PTT_name�Ľڵ�
    if(Rsc_SearchSon(fs_rsc_node,MDR_PTT->PTT_name))
    {
        Lock_SempPost(Fs_Semaphore);
        Djy_SaveLastError(EN_FS_PTT_NAME_REPEAT);
        printf("������[%s]����.\n\r", MDR_PTT->PTT_name);
        return false;
    }
    Lock_SempPost(Fs_Semaphore);
    if( ! __DFFSD_Allocate_PTT(chip, size, &PTT_start)) //����洢�ռ�
        return false;
    strcpy(MDR_PTT->PTT_format_flag, "unready"); //��ʽ����־
    fill_little_32bit(MDR_PTT->PTT_start_block, 0, PTT_start); //��ʼ���
    fill_little_32bit(MDR_PTT->PTT_blocks_sum, 0, size); //�����ߴ�(����)
    __DFFSD_Fill_ECC_MDR_PTT((uint8_t *)MDR_PTT); //���ECC��

    //��������豸ר�����ݽṹ�ڴ棬�ýṹ��Ƕ�뵽���豸�ṹ��
    PTT_device_tag = (struct tagPTTDevice*)
                M_Malloc(sizeof(struct tagPTTDevice),0);
    if(PTT_device_tag == NULL)
        goto goto_exit_install_chip;
    //����flash�ļ�ϵͳר�����ݽṹ�ڴ棬�ýṹ��Ƕ�뵽�����豸��
    PTT_flash_tag = (struct tagStPTT_Flash*)
                M_Malloc(sizeof(struct tagStPTT_Flash),0);
    if(PTT_flash_tag == NULL)
        goto goto_exit_install_chip;
    PTT_flash_tag->ART_position = NULL;
    PTT_flash_tag->ART_block_no = NULL;
    PTT_flash_tag->ART_times = NULL;
    PTT_flash_tag->writed_DDR_main = NULL;
    PTT_flash_tag->writed_DDR_bak = NULL;
    PTT_flash_tag->chip = chip;         //flash�����洢������оƬ
    PTT_flash_tag->PTT_no = PTT_no;     //��������оƬ�е����
    PTT_flash_tag->PCRB_no = CN_LIMIT_UINT32;   //δ��ʽ��
    PTT_flash_tag->DDR_main = CN_LIMIT_UINT32;  //δ��ʽ��
    PTT_flash_tag->DDR_bak = CN_LIMIT_UINT32;   //δ��ʽ��
    PTT_flash_tag->start_block = PTT_start;
    PTT_flash_tag->block_sum = size;

    table_size = PTT_flash_tag->block_sum * (4+4+2);
    //Ϊĥ��λ�ñ�����ڴ�
    PTT_flash_tag->ART_position = (uint32_t*)M_Malloc(table_size,0);
    if(PTT_flash_tag->ART_position == NULL)
        goto goto_exit_install_chip;
    //Ϊĥ���ű�����ڴ�
    PTT_flash_tag->ART_block_no = PTT_flash_tag->ART_position
                                  + PTT_flash_tag->block_sum;
    //Ϊĥ�����������ڴ�
    PTT_flash_tag->ART_times = (uint16_t*)(PTT_flash_tag->ART_block_no
                                  + PTT_flash_tag->block_sum);

    //����DDR��ߴ�
    PTT_flash_tag->DDR_size = __DFFSD_CalculateDDR_Size(PTT_flash_tag);
    //1��DDR��ռ�õĿ���
    DDR_blocks = (PTT_flash_tag->DDR_size
                        +chip->block_size-1)/chip->block_size;
    //��������DBL�������ڴ�
    PTT_flash_tag->DDR_DBL =(uint32_t*)M_Malloc(DDR_blocks*8,0);
    if(PTT_flash_tag->DDR_DBL == NULL)
        goto goto_exit_install_chip;
    //����MAT�������ڴ�
    PTT_flash_tag->DDR_MAT = (struct tagMAT_Table *)M_Malloc(
                        PTT_flash_tag->block_sum*sizeof(struct tagMAT_Table),0);
    if(PTT_flash_tag->DDR_MAT == NULL)
        goto goto_exit_install_chip;
    //����д���־
    PTT_flash_tag->writed_DDR_main = (bool_t*)M_Malloc(
                                        DDR_blocks*sizeof(bool_t)*2,0);
    if(PTT_flash_tag->writed_DDR_main == NULL)
        goto goto_exit_install_chip;
    PTT_flash_tag->writed_DDR_bak=PTT_flash_tag->writed_DDR_main+DDR_blocks;
//    for(loop = 0; loop < DDR_blocks; loop++)
//    {
//        PTT_flash_tag->writed_DDR_main[loop] = false;
//        PTT_flash_tag->writed_DDR_bak[loop] = false;
//    }

//    __DFFSD_WriteDDR(PTT_flash_tag);
    //��亯��ָ��ͷ�����
    __DFFSD_Fill_FDT_Device(PTT_device_tag,name);
    //�����豸�Ĵ洢ý��ָ��flash�����洢��
    PTT_device_tag->PTT_medium_tag = (ptu32_t)PTT_flash_tag;
    PTT_device_tag->formatted =false;
    PTT_device_tag->chip_mutex_tag = chip->chip_mutex;

    //���ǵ��ڴ��������״���,���Ž�������Ϣд�뵽FLASH;(�߼�δ���ƣ��������ͷŽڵ��߼�)
    if( ! __DFFSD_write_MDR_PTT(chip, PTT_no, MDR_PTT_buf))
        goto goto_exit_install_chip;

    Lock_MutexPend(PTT_device_tag->chip_mutex_tag,CN_TIMEOUT_FOREVER);
    if(EN_FS_NO_ERROR !=  Djyfs_FsDriverCtrl(enum_fs_add_PTT,
                                             (ptu32_t)PTT_device_tag,0))
    {
        goto goto_exit_install_chip;
    }
    Lock_MutexPost(PTT_device_tag->chip_mutex_tag);

    //������������
    chip->MDR.PTT_created[PTT_no] = true; //���������ɹ�
    return true;

goto_exit_install_chip:
    if(PTT_flash_tag !=NULL)
    {
        if(PTT_flash_tag->writed_DDR_main != NULL)
            free(PTT_flash_tag->writed_DDR_main);
        if(PTT_flash_tag->DDR_MAT != NULL)
            free(PTT_flash_tag->DDR_MAT);
        if(PTT_flash_tag->DDR_DBL != NULL)
            free(PTT_flash_tag->DDR_DBL);
        if(PTT_flash_tag->ART_position != NULL)
            free(PTT_flash_tag->ART_position);
        free(PTT_flash_tag);
    }
    if(PTT_device_tag !=NULL)
        free(PTT_device_tag);
    return false; //�������뵽�豸��ʧ��
}

//----����MDR_bak�Ŀ��--------------------------------------------------------
//����: �ڶ�MDR_mainʧ�ܣ��޷����л�ȡMDR_bak�Ŀ��ʱ������оƬ�Ŀ�ߴ����
//      MDR_bak���ܵĴ��λ�á����ñ�����ȷ�����ʱ��Ӧ�ôӸÿ�ſ�ʼ��������
//      5�飬ֱ���ҵ��Ϸ���MDR_bak��Ϊֹ�����5�鶼δ���ҵ��Ϸ���MDR_bak������Ϊ
//      ��flashû�а���djyosfs��ʼ������
//����: chip��Ŀ��оƬ
//����: ��ţ�0��ʾû���ҵ�
//-----------------------------------------------------------------------------
uint32_t __DFFSD_Probe_MDR_Bak(struct tagFlashChip *chip)
{
    return (CN_MDR_SIZE+chip->block_size-1) / chip->block_size;
}
//----У��MDR_CDR--------------------------------------------------------------
//����: У��MDR_CDR�������������ݣ�������ܸ��������ش���
//����: buf��������δУ���MDR_CDR���ݵĻ�����
//����: true=�޴���߱�������false=�д��Ҳ��ܸ�����
//-----------------------------------------------------------------------------
bool_t __DFFSD_Verify_MDR_CDR(uint8_t *buf)
{
    return true;
}
//----У��MDR_PTT--------------------------------------------------------------
//����: У��MDR_PTT�������������ݣ�������ܸ��������ش���
//����: buf��������δУ���MDR_PTT���ݵĻ�����
//����: true=�޴���߱�������false=�д��Ҳ��ܸ�����
//-----------------------------------------------------------------------------
bool_t __DFFSD_verify_MDR_PTT(uint8_t *buf)
{
    return true;
}

//----У��MDR������־----------------------------------------------------------
//����: 1.У��MDR������4���ֽ��Ƿ�AA5555AA,���ֻ����һλ������
//      2.У��CDR��PTT���ECC
//����: buf���ֽڻ�����
//����: true=У����ȷ��false=����
//-----------------------------------------------------------------------------
//�ر�ע�⣬��������/��С������/��СѰַ��Ԫ����8λ���⣬��Щ����ʹ�ò��ܰ�
//AA5555AA���32λ�������Ƚ�
bool_t __DFFSD_Verify_EndFlag(uint8_t *buf)
{
    uint8_t temp[4] = {0xAA,0x55,0x55,0xAA};
    uint8_t xor;
    ufast_t loop4,loop8,error_bit = 0;
    for(loop4 = 0; loop4 < 4; loop4++)
    {
        if(buf[loop4] != temp[loop4])
        {
            xor = buf[loop4] ^ temp[loop4];
            for(loop8 = 0; loop8 < 8; loop8++)
            {
                if(xor & (1<<loop8))
                    error_bit ++;
                if(error_bit > 1)
                    return false;
            }
        }else
            continue;
    }
    if(! __DFFSD_Verify_MDR_CDR(buf))
        return false;
    if(! __DFFSD_verify_MDR_PTT(buf))
        return false;
    return true;
}

//----MDR������¼ECCУ��-------------------------------------------------------
//����: MDR�й����洢�ļ�¼����ECCУ�飬���������ݡ�
//����: buf����¼ָ��
//����: 0=�޴�1=�д���������2=�޷�����
//-----------------------------------------------------------------------------
uint32_t __DFFSD_Verify_MDR_Record(uint8_t *buf)
{
    return 0;
}

//----ɨ������洢��-----------------------------------------------------------
//����: ɨ������洢����������ȡ�Ѿ���ʽ���ķ�����DDR_main��š�DDR_bak��š�
//      PCRB��š�
//����: chip����������оƬ
//      MDR_buf��������
//      MDR_record������ɨ�赽�����ݵ�ָ��
//      end_offset�����һ����Ч��¼λ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_UasmRoll(struct tagFlashChip *chip,uint8_t *MDR_buf,
                       struct tagMDR_TempRecord *MDR_record,uint32_t end_offset)
{
    uint8_t *record;
    uint32_t loop,loop3,item_sum=0;
    bool_t got_DDR_main[CN_PTT_SUM_MAX];//������DDR_main����Ƿ��ҵ���־
    bool_t got_DDR_bak[CN_PTT_SUM_MAX]; //������DDR_bak����Ƿ��ҵ���־
    bool_t got_PCRB[CN_PTT_SUM_MAX];    //������PCRB����Ƿ��ҵ���־

    for(loop3 = 0; loop3 < CN_PTT_SUM_MAX; loop3++)
    {
        got_DDR_main[loop3] = false;
        got_DDR_bak[loop3] = false;
        got_PCRB[loop3] = false;
        if(MDR_record->formatted[loop3])
        {
            item_sum +=3;   //ÿ���Ѿ���ʽ���ķ�������3��������¼
        }
    }
    //ѭ��ɨ�����й����洢������Ч��Ŀ��
    for(loop = end_offset-8;loop >= CN_ROLL_OFFSET; loop-=8)
    {
        record = MDR_buf + loop;
        for(loop3 = 0; loop3 < CN_PTT_SUM_MAX; loop3++)
        {
            if(! MDR_record->formatted)     //����δ��ʽ��
                continue;
            __DFFSD_Verify_MDR_Record(record); //У��ü�¼
            if(!got_DDR_main[loop3])        //DDR_main������¼��δ�ҵ�
            {
                //��鱾��Ŀ�ǲ��ǵ�loop��������DDR_main���
                if((record[0] == CN_MDR_DDR_MAIN) && (record[1] == loop3))
                {
                    item_sum--;                 //�ҵ���ʣ����Ŀ������
                    got_DDR_main[loop3] = true;  //��Ǹ�DDR_main��������ҵ�
                    //�����д�뷵�صĽṹָ����
                    MDR_record->DDR_main_block_no[loop3]
                            = pick_little_32bit(&record[2],0);
                    break;
                }
            }
            if(!got_DDR_bak[loop3])         //DDR_bak������¼��δ�ҵ�
            {
                //��鱾��Ŀ�ǲ��ǵ�loop��������DDR_bak���
                if((record[0] == CN_MDR_DDR_BAK) && (record[1] == loop3))
                {
                    item_sum--;                 //�ҵ���ʣ����Ŀ������
                    got_DDR_bak[loop3] = true;  //��Ǹ�DDR_bak��������ҵ�
                    //�����д�뷵�صĽṹָ����
                    MDR_record->DDR_bak_block_no[loop3]
                                    = pick_little_32bit(&record[2],0);
                    break;
                }
            }
            if(!got_PCRB[loop3])                //PCRB������¼��δ�ҵ�
            {
                //��鱾��Ŀ�ǲ��ǵ�loop��������PCRB���
                if((record[0] == CN_MDR_PCRB) && (record[1] == loop3))
                {
                    item_sum--;                 //�ҵ���ʣ����Ŀ������
                    got_PCRB[loop3] = true;  //��Ǹ�PCRB��������ҵ�
                    //�����д�뷵�صĽṹָ����
                    MDR_record->PCRB_block_no[loop3] =
                                pick_little_32bit(&record[2],0);
                    break;
                }
            }
        }
        if(item_sum == 0)   //ʣ����ĿΪ0��������Ҫ�ҵ���Ŀ�Ѿ�ȫ���ҵ�
            break;
    }
}

//----���MDR_bak��������------------------------------------------------------
//����: ���MDR_bak�Ƿ��������Ƿ����ϴε���ʱ����д������²������������Ǽ��
//      β����AA5555AA���Լ����һ��������¼��У����
//����: chip��Ŀ��оƬ
//����: true=������false=������
//-----------------------------------------------------------------------------
bool_t __DFFSD_CheckMDR_Bak(struct tagFlashChip *chip)
{
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
        return false;
    __DFFSD_ReadMDR_Bak(chip,MDR_buf);
    if(!__DFFSD_Verify_EndFlag(MDR_buf))
    {
        free(MDR_buf);
        return false;
    }
    free(MDR_buf);
    return true ;
}

//----������������λ��-------------------------------------------------------
//����: �������洢����һ�����м�¼��ƫ���������۰���ҷ���
//����: chip��Ŀ��оƬ
//      MDR_buf��MDR������
//����: ƫ����
//-----------------------------------------------------------------------------
uint32_t __DFFSD_FindRollNext(struct tagFlashChip *chip,uint8_t *MDR_buf)
{
    uint32_t busy_endian,free_endian,middle,result;
    uint32_t block_offset;
    busy_endian = CN_ROLL_OFFSET/8;
    free_endian = CN_MDR_SIZE /8 -1;   //���һ�����������Ա��
    while(1)
    {
        middle = (busy_endian+free_endian)/2; //�����м���
        block_offset = 8*middle;
        //���Ե�block_no��ƫ��block_offset��ʼ��8�ֽ��Ƿ������δʹ�ù���
        //��Ȼ������֪��flash���������Ƿ����0xff���ж��Ƿ��������datasheet
        //�ϲ�û������˵���ѱ����첻�����һ�����������ģ��ʰ��жϽ���оƬ
        //driver�ſ�ȷ������ֲ�ԡ�
        if(chip->query_ready_with_data(NULL,MDR_buf+block_offset,8))
        {//��middle��Ԫ�ǿ��е�
            if(middle == busy_endian)
            {
                result = middle*8;
                break;
            }else
                free_endian = middle;
        }else
        {//��middle��Ԫ���Ѿ�ʹ���˵�
            if(free_endian == middle+1)
            {
                result = free_endian*8;
                break;
            }else
                busy_endian = middle;
        }
    }//���ҹ���λ�ý���
    return result;
}

//----����PTT��----------------------------------------------------------------
//����: ɨ��MDR��������3��PTT���ҳ��Ѿ������ķ�����������������
//����: chip��Ŀ��оƬ��Ҳ���ڱ���ɨ����
//      MDR_record�����ڱ�������ָ��
//      MDR_buf��MDR������
//����: �Ѿ������ķ�������
//-----------------------------------------------------------------------------
ufast_t __DFFSD_UasmPTT(struct tagFlashChip *chip,
                         struct tagMDR_TempRecord *MDR_record,uint8_t *MDR_buf)
{
    struct tagMDR_PTT_Mirror *MDR_PTT;
    uint32_t loop,name_len;
    ufast_t PTT_sum=0;    //�Ѿ������ķ�������
    bool_t formatted,unformatted;
    //ȡMDR_PTT��¼����ʼ��ַ
    MDR_PTT = (struct tagMDR_PTT_Mirror *)(MDR_buf + CN_MDR_CDR_SIZE);
    for(loop = 0; loop < CN_PTT_SUM_MAX; loop++)
    {//��ѭ���� 1���жϷ���������
     //         2������������
     //         3��������������ʼ��źͿ��������ܳ�ʼ����
        formatted = (strcmp(MDR_PTT->PTT_format_flag,"ready")==0);
        unformatted = (strcmp(MDR_PTT->PTT_format_flag,"unready")==0);
        if(formatted || unformatted)    //���ܸ�ʽ���񣬶���ʾ�����Ѿ�������
        {
            PTT_sum++;                              //������������
            chip->MDR.PTT_created[loop] = true;     //����������־��Ϊtrue
            MDR_record->formatted[loop] = formatted;    //���ʽ����־
            name_len = strnlen(MDR_PTT->PTT_name,CN_FLASH_PTT_NAME_LIMIT+1);
            if(name_len == 0)
                strcpy(MDR_record->name[loop],"no name deedbox");
            else if(name_len <= CN_FLASH_PTT_NAME_LIMIT)
                strcpy(MDR_record->name[loop],MDR_PTT->PTT_name);
            else
            {
                memcpy(MDR_record->name[loop],MDR_PTT->PTT_name,
                                        CN_FLASH_PTT_NAME_LIMIT);
                MDR_record->name[loop][CN_FLASH_PTT_NAME_LIMIT] = '\0';
            }
            //������ʼ���
            MDR_record->start_blocks[loop] =
                            pick_little_32bit(MDR_PTT->PTT_start_block,0);
            //�����ߴ�(�Կ�����ʾ)
            MDR_record->blocks_sum[loop] =
                            pick_little_32bit(MDR_PTT->PTT_blocks_sum,0);

        }else   //MDR_PTT�Ȳ���"ready"Ҳ����"unready"��ͷ����������δ����
        {
            chip->MDR.PTT_created[loop] = false;    //����������־��Ϊfalse
            MDR_record->formatted[loop] = false;    //���ʽ����־
        }
        //ȡ��һ������MDR_PTT��ƫ�Ƶ�ַ
        MDR_PTT = (struct tagMDR_PTT_Mirror *)((uint32_t)MDR_PTT +CN_MDR_PTT_SIZE);
    }
    return PTT_sum;
}

//----��ʼ����MDR��------------------------------------------------------------
//����: оƬ��ʼ��ʱ������MDR����ȡ��Ч���ݡ��������MDR_main��MDR_bak������
//      һ���д���ˢ��֮
//����: chip����������оƬ��ͬʱ���ڷ��ؽ��
//      MDR_record��������Ч���ݵ�ָ��
//����: ɨ��MDR�Ľ��������chip��MDR_record�����ṹָ����
//      true=оƬ�Ѿ���djyosfs��ʼ����false=оƬδ��djyosfs��ʼ��
//-----------------------------------------------------------------------------
//��ע: ���Ա�����ʱ�������rvd���������ʷǷ���ַ����windows�رգ���ARM��������
//      ���������´�rvd���������Դ��ϴ��˳��ĵط��������С�
bool_t __DFFSD_ReadMDR(struct tagFlashChip *chip,struct tagMDR_TempRecord *MDR_record)
{
    uint8_t *MDR_buf;
    uint32_t roll_size;
    uint8_t roll_buf[CN_PTT_SUM_MAX*3*8];
    bool_t MDR_main_valid,MDR_bak_valid;
    struct tagMDR_CDR_Mirror *MDR_CDR;
    ufast_t PTT_sum = 0;    //�Ѿ������ķ�������


    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
        return false;
    __DFFSD_ReadMDR_Main(chip,MDR_buf);
    if(!__DFFSD_Verify_EndFlag(&MDR_buf[CN_MDR_SIZE-4]))  //���MDR_main�Ƿ�����
    {//MDR_main�д��󣬶�ȡMDR_bak
        MDR_main_valid = false;
        chip->MDR.MDR_bak_block = __DFFSD_Probe_MDR_Bak(chip);   //̽��MDR_bak�Ĵ洢λ��
    }else
    {
        MDR_main_valid = true;
        MDR_CDR = (struct tagMDR_CDR_Mirror *)MDR_buf;
        if(strcmp(MDR_CDR->chip_init_flag,"djyosfs") != 0)
        {
            free(MDR_buf);
            return false;      //�洢����δ��ʼ��
        }
        //�����CDR������ݶ�ȡ��chip��Դ��
        chip->MDR.MDR_bak_block = pick_little_32bit(MDR_CDR->chip_MDR_bak,0);
        //��ȡ���������������ʼ��ţ����������������������п����л���
        chip->rev_start = chip->MDR.MDR_bak_block
                          + (CN_MDR_SIZE+chip->block_size -1)/chip->block_size;
        chip->rev_blocks = pick_little_32bit(MDR_CDR->chip_reserve,0);
        //�������ڷ�������ʼ��ţ�
        chip->PTT_start = chip->rev_start + chip->rev_blocks;
        PTT_sum = __DFFSD_UasmPTT(chip,MDR_record,MDR_buf);
        if(PTT_sum != 0)
        {
            //�ҵ������洢������һ���洢λ��
            chip->MDR.roll_next_main = __DFFSD_FindRollNext(chip,MDR_buf);
            //ɨ�貢��ȡ������¼�е���Ч��¼(��ÿ���¼�����һ����¼)
            __DFFSD_UasmRoll(chip,MDR_buf,MDR_record,
                              chip->MDR.roll_next_main);
        }else
            chip->MDR.roll_next_main = CN_ROLL_OFFSET;
    }

    __DFFSD_ReadMDR_Bak(chip,MDR_buf);     //��MDR_bak����������
    if(!__DFFSD_Verify_EndFlag(&MDR_buf[CN_MDR_SIZE-4]))  //���MDR_bak�Ƿ�����
    {
        MDR_bak_valid = false;  //MDR_bak�д���
    }else
    {
        MDR_bak_valid = true;
        if(MDR_main_valid)      //���MDR_main��Ч����ֻ��ȡ��һ�������洢λ��
        {
            if(PTT_sum != 0)
                chip->MDR.roll_next_bak = __DFFSD_FindRollNext(chip,MDR_buf);
            else
                chip->MDR.roll_next_bak = CN_ROLL_OFFSET;
        }else   //���MDR_main��Ч�������з�����Ϣ��MDR_bak��ȡ
        {
            MDR_CDR = (struct tagMDR_CDR_Mirror *)MDR_buf;
            if(strcmp(MDR_CDR->chip_init_flag,"djyosfs")!=0)
            {
                free(MDR_buf);
                return false;      //�洢����δ��ʼ��
            }
            //�����CDR������ݶ�ȡ��chip��Դ��
            //��ȡ���������������ʼ��ţ����������������������п����л���
            chip->rev_start = chip->MDR.MDR_bak_block
                          +(CN_MDR_SIZE+chip->block_size -1)/chip->block_size;
            chip->rev_blocks = pick_little_32bit(MDR_CDR->chip_reserve,0);
            //�������ڷ�������ʼ��ţ�
            chip->PTT_start = chip->rev_start + chip->rev_blocks;
            PTT_sum = __DFFSD_UasmPTT(chip,MDR_record,MDR_buf);
            if(PTT_sum != 0)
            {
                chip->MDR.roll_next_bak = __DFFSD_FindRollNext(chip,MDR_buf);
                //ɨ�貢��ȡ������¼�е���Ч��¼(��ÿ���¼�����һ����¼)
                __DFFSD_UasmRoll(chip,MDR_buf,MDR_record,
                                  chip->MDR.roll_next_bak);
            }
        }
    }
    if((!MDR_main_valid) && (!MDR_bak_valid))
    {
        free(MDR_buf);    //���MDR_main��MDR_bak������˵��δ��ʼ���洢��
        return false;
    }
    else
    {
        roll_size = __DFFSD_AsmRollBuf(chip,MDR_record,roll_buf);
        if(!MDR_main_valid) //MDR_main������MDR_bak�ָ�֮
        {
            __DFFSD_EraseMDR_Main(chip);
            memcpy(MDR_buf+CN_ROLL_OFFSET,roll_buf,roll_size);
            __DFFSD_WriteMDR_Main(chip,MDR_buf,CN_ROLL_OFFSET + roll_size);
            __DFFSD_WriteEndFlagMain(chip);
        }else if(!MDR_bak_valid) //MDR_bak������MDR_main�ָ�֮
        {
            __DFFSD_EraseMDR_Bak(chip);
            memcpy(MDR_buf+CN_ROLL_OFFSET,roll_buf,roll_size);
            __DFFSD_WriteMDR_Bak(chip,MDR_buf,CN_ROLL_OFFSET + roll_size);
            __DFFSD_WriteEndFlagBak(chip);
        }
    }
    free(MDR_buf);
    return true;
}

//----��������ĿУ����-------------------------------------------------------
//����: ��һ��MDR�Ĺ����洢��¼����У���롣
//����: buf����¼ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_FillECC_MDR_Record(uint8_t *buf)
{
}

//----���MDR_CDRУ����--------------------------------------------------------
//����: ��һ��MDR_CDR����У���롣
//����: buf��CDR��¼ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_Fill_ECC_MDR_CDR(uint8_t *buf)
{
}

//----���MDR_PTTУ����--------------------------------------------------------
//����: ��һ��MDR_PTT����У���롣
//����: buf��PTT��¼ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_Fill_ECC_MDR_PTT(uint8_t *buf)
{
}

//----дMDR_main�Ľ������-----------------------------------------------------
//����: д��MDR_main�Ľ�����־����ʾMDR_main��������
//����: chip����������оƬ
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteEndFlagMain(struct tagFlashChip *chip)
{
    uint32_t block_no,block_offset;
    uint8_t buf[4];
    block_no = (CN_MDR_SIZE -4)/chip->block_size;
    block_offset = (CN_MDR_SIZE -4)%chip->block_size;
    buf[0] = 0xaa;
    buf[1] = 0x55;
    buf[2] = 0x55;
    buf[3] = 0xaa;
    chip->write_data_no_ecc(block_no,block_offset,buf,4);
}

//----дMDR_bak�Ľ������-----------------------------------------------------
//����: д��MDR_bak�Ľ�����־����ʾMDR_bak��������
//����: chip����������оƬ
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteEndFlagBak(struct tagFlashChip *chip)
{
    uint32_t block_no,block_offset;
    uint8_t buf[4];
    block_no = (CN_MDR_SIZE -4)/chip->block_size + chip->MDR.MDR_bak_block;
    block_offset = (CN_MDR_SIZE -4)%chip->block_size;
    buf[0] = 0xaa;
    buf[1] = 0x55;
    buf[2] = 0x55;
    buf[3] = 0xaa;
    chip->write_data_no_ecc(block_no,block_offset,buf,4);
}

//----дMDR_main--------------------------------------------------------------
//����: ��DDR_main��ͷ���ֿ�ʼ��д��ָ�����ȵ����ݡ�
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      len,д�볤��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteMDR_Main(struct tagFlashChip *chip,uint8_t *MDR_buf,uint32_t len)
{
    uint32_t complete_len=0,write_len;
    uint32_t block_no;
    block_no = 0;   //MDR_mainĬ�ϴ�0�鿪ʼ

    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((len - complete_len) > chip->block_size)
            write_len = chip->block_size;
        else
            write_len = len-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,0,MDR_buf+complete_len,write_len);
        block_no++;
        complete_len += write_len;
    }while(complete_len < len);
}

//----дMDR_bak--------------------------------------------------------------
//����: ��DDR_bak��ͷ���ֿ�ʼ��д��ָ�����ȵ����ݡ�
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      len,д�볤��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteMDR_Bak(struct tagFlashChip *chip,uint8_t *MDR_buf,uint32_t len)
{
    uint32_t complete_len=0,write_len;
    uint32_t block_no;
    block_no = chip->MDR.MDR_bak_block;   //MDR_mainĬ�ϴ�0�鿪ʼ
    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((len - complete_len) > chip->block_size)
            write_len = chip->block_size;
        else
            write_len = len-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,0,MDR_buf+complete_len,write_len);
        block_no++;
        complete_len += write_len;
    }while(complete_len < len);
}

//----дMDR_main��PTT����------------------------------------------------
//����: ��PTTд�뵽MDR_main�У����ô˺���ǰ��ȷ������PTT��¼�Ĳ����Ѿ�������.
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      PTT_no���������
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteMDR_PTT_Main(struct tagFlashChip *chip,uint8_t *MDR_PTT_buf,
                                                      uint32_t PTT_no)
{
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    write_size = CN_MDR_PTT_SIZE;
    block_no = (CN_MDR_CDR_SIZE + PTT_no*CN_MDR_PTT_SIZE) / chip->block_size ;
    block_offset = (CN_MDR_CDR_SIZE + PTT_no*CN_MDR_PTT_SIZE)% chip->block_size;
    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((write_size-complete_len) > chip->block_size-block_offset)
            write_len = chip->block_size-block_offset;
        else
            write_len = write_size-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,block_offset,
                                MDR_PTT_buf+complete_len,write_len);
        block_no++;
        block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
        complete_len += write_len;
    }while(complete_len < write_size);
}

//----дMDR_bak��PTT����------------------------------------------------
//����: ��PTTд�뵽MDR_bak�У����ô˺���ǰ��ȷ������PTT��¼�Ĳ����Ѿ�������.
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      PTT_no���������
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteMDR_PTT_Bak(struct tagFlashChip *chip,uint8_t *MDR_PTT_buf,
                                                 uint32_t PTT_no)
{
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    write_size = CN_MDR_PTT_SIZE;
    block_no = (CN_MDR_CDR_SIZE + PTT_no*CN_MDR_PTT_SIZE) / chip->block_size
                                    + chip->MDR.MDR_bak_block;
    block_offset = (CN_MDR_CDR_SIZE + PTT_no*CN_MDR_PTT_SIZE) % chip->block_size ;
    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((write_size-complete_len) > chip->block_size-block_offset)
            write_len = chip->block_size-block_offset;
        else
            write_len = write_size-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,block_offset,
                                MDR_PTT_buf+complete_len,write_len);
        block_no++;
        block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
        complete_len += write_len;
    }while(complete_len < write_size);
}

//----дһ��MDR������¼--------------------------------------------------------
//����: MDR����д��һ��������¼��ֻ��MDR_main�вŻ��д˲�����MDR_bak�������й���
//      ��¼ͬʱд�룬�����е���д��Ĳ�����
//����: chip����������оƬ
//      PTT_no���������
//      type��д�����ͣ�����ֵcn_MDR_DDR_main��cn_MDR_DDR_bak��cn_MDR_PCRB
//      value����д�����ֵ
//����: ��
//ע��: ���ܶ�δ��ʽ���ķ������ñ�����������һ���ڲ������������Ŀ�����
//      �Ƿ��Ѿ���ʽ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteMDR_RollRecord(struct tagFlashChip *chip,uint32_t PTT_no,
                                   uint32_t type,uint32_t value)
{
    uint8_t item_buf[8];
    uint32_t block_no,block_offset;
    uint8_t *MDR_buf;
    struct tagMDR_TempRecord MDR_record;

    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
         return ;
    item_buf[0] = type;             //����¼����
    item_buf[1] = PTT_no;           //��������
    //���÷�����DDR_main���
    fill_little_32bit(&item_buf[2],0,value);
    __DFFSD_FillECC_MDR_Record(item_buf);//���У����
    if((chip->MDR.roll_next_main + 8) > CN_MDR_SIZE-8)
    {//�ر�ע������cn_MDR_size<784,�˴�������ѭ��
        __DFFSD_ReadMDR_Main(chip,MDR_buf);
        __DFFSD_UasmPTT(chip,&MDR_record,MDR_buf);
        __DFFSD_UasmRoll(chip,MDR_buf,&MDR_record,
                          chip->MDR.roll_next_main);
        if(type == CN_MDR_DDR_MAIN)
            MDR_record.DDR_main_block_no[PTT_no] = value;
        else if(type == CN_MDR_DDR_BAK)
            MDR_record.DDR_bak_block_no[PTT_no] = value;
        else if(type == CN_MDR_PCRB)
            MDR_record.PCRB_block_no[PTT_no] = value;
        __DFFSD_WriteRollBak(chip,&MDR_record);
        __DFFSD_EraseMDR_Main(chip);
        __DFFSD_WriteMDR_Main(chip,MDR_buf,CN_MDR_AND_PTT_SIZE);
        __DFFSD_WriteRollMain(chip,&MDR_record);
        __DFFSD_WriteEndFlagMain(chip);
    }else
    {
        block_no = chip->MDR.roll_next_main / chip->block_size ;
        block_offset = chip->MDR.roll_next_main % chip->block_size ;
        chip->write_data_no_ecc(block_no,block_offset,item_buf,8);
        chip->MDR.roll_next_main += 8;
    }
    free(MDR_buf);
}

//----��������洢��-----------------------------------------------------------
//����: �ӹ����洢��¼�����ܹ�ֱ�ӱ�����flash�е����ݣ��Ѹ�ʽ���ķ����Ż���
//      ������¼��
//����: chip����������оƬ
//      MDR_record�������洢��¼
//      roll_buf�������洢��������
//����: roll_buf����Ч���ݳ���
//-----------------------------------------------------------------------------
uint32_t __DFFSD_AsmRollBuf(struct tagFlashChip *chip,
                          struct tagMDR_TempRecord *MDR_record,uint8_t *roll_buf)
{
    uint32_t loop,PTT_sum=0;
    uint8_t *buf = roll_buf;
    for(loop = 0; loop < CN_PTT_SUM_MAX; loop++)
    {
        if(MDR_record->formatted[loop])   //�����Ѿ���ʽ��
        {
            buf[0] = (uint8_t)CN_MDR_DDR_MAIN; //����¼����
            buf[1] = (uint8_t)loop;            //��������
            //���÷�����DDR_main���
            fill_little_32bit(buf+2,0,MDR_record->DDR_main_block_no[loop]);
            __DFFSD_FillECC_MDR_Record(buf);//���У����
            buf += 8;
            buf[0] = CN_MDR_DDR_BAK; //����¼����
            buf[1] = loop;            //��������

            //���÷�����DDR_bak���
            fill_little_32bit(buf+2,0,MDR_record->DDR_bak_block_no[loop]);
            __DFFSD_FillECC_MDR_Record(buf);//���У����
            buf += 8;
            buf[0] = CN_MDR_PCRB; //����¼����
            buf[1] = loop;            //��������

            //���÷�����PCRB���
            fill_little_32bit(buf+2,0,MDR_record->PCRB_block_no[loop]);
            __DFFSD_FillECC_MDR_Record(buf);//���У����
            buf += 8;
            PTT_sum +=8*3;
        }
    }
    return PTT_sum;
}
//----дMDR_main��ȫ��������¼-------------------------------------------------
//����: д��MDR_main���е�ȫ�������洢��
//����: chip����������оƬ
//      MDR_record������ȫ�������洢���ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteRollMain(struct tagFlashChip *chip,
                             struct tagMDR_TempRecord *MDR_record)
{
    struct tagStMDR_Flag *MDR_flag = &chip->MDR;
    uint8_t roll_buf[CN_PTT_SUM_MAX*3*8];
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    bool_t write_end_flag=false;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
        return ;
    write_size = __DFFSD_AsmRollBuf(chip,MDR_record,roll_buf);
    if(write_size != 0)
    {
        //���д�볤��+��ǰƫ�ƴ��ڹ���������
        if((MDR_flag->roll_next_main + write_size) > CN_MDR_SIZE-8)
        {//�ر�ע������cn_MDR_size<784,�˴�������ѭ��
            __DFFSD_WriteRollBak(chip,MDR_record);
            __DFFSD_ReadMDR_Main(chip,MDR_buf);
            __DFFSD_EraseMDR_Main(chip);
            __DFFSD_WriteMDR_Main(chip,MDR_buf,CN_MDR_AND_PTT_SIZE);
            MDR_flag->roll_next_main = CN_ROLL_OFFSET;
            write_end_flag = true;
        }
        block_no = MDR_flag->roll_next_main / chip->block_size ;
        block_offset = MDR_flag->roll_next_main % chip->block_size;
        MDR_flag->roll_next_main += write_size;
        do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
        {
            if((write_size-complete_len) > chip->block_size-block_offset)
                write_len = chip->block_size-block_offset;
            else
                write_len = write_size-complete_len;
            //д�����ݵ�flash�У���У��
            chip->write_data_no_ecc(block_no,block_offset,
                                    roll_buf+complete_len,write_len);
            block_no++;
            block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
            complete_len += write_len;
        }while(complete_len < write_size);
        if(write_end_flag)
            __DFFSD_WriteEndFlagMain(chip);
    }
    free(MDR_buf);
}

//----дMDR_bak��ȫ��������¼-------------------------------------------------
//����: д��MDR_bak���е�ȫ�������洢��
//����: chip����������оƬ
//      MDR_record������ȫ�������洢���ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteRollBak(struct tagFlashChip *chip,
                            struct tagMDR_TempRecord *MDR_record)
{
    struct tagStMDR_Flag *MDR_flag = &chip->MDR;
    uint8_t roll_buf[CN_PTT_SUM_MAX*3*8];
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    bool_t write_end_flag=false;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
        return ;
    write_size = __DFFSD_AsmRollBuf(chip,MDR_record,roll_buf);
    if(write_size != 0)
    {
        if((MDR_flag->roll_next_bak + write_size) > CN_MDR_SIZE-8)
        {//�ر�ע������cn_MDR_size<784,�˴�������ѭ��
            __DFFSD_ReadMDR_Bak(chip,MDR_buf);
            __DFFSD_EraseMDR_Bak(chip);
            __DFFSD_WriteMDR_Bak(chip,MDR_buf,CN_MDR_AND_PTT_SIZE);
            MDR_flag->roll_next_bak = CN_ROLL_OFFSET;
            write_end_flag = true;
        }
        block_no = MDR_flag->roll_next_main / chip->block_size ;
        block_offset = MDR_flag->roll_next_main % chip->block_size ;
        MDR_flag->roll_next_bak += write_size;
        do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
        {
            if((write_size-complete_len) > chip->block_size-block_offset)
                write_len = chip->block_size-block_offset;
            else
                write_len = write_size-complete_len;
            //д�����ݵ�flash�У���У��
            chip->write_data_no_ecc(block_no,block_offset,
                                    roll_buf+complete_len,write_len);
            block_no++;
            block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
            complete_len += write_len;
        }while(complete_len < write_size);
        if(write_end_flag)
            __DFFSD_WriteEndFlagBak(chip);
    }
    free(MDR_buf);
}

//----д��һ��MDR��¼----------------------------------------------------------
//����: ����һ��MDR��¼����������flash driverʹ�ã�flash driver����֪��ʹ�ù���
//      ��ʽ��¼��������ֻ��__DFFSD_write_MDR_roll_record�����İ�װ���ѡ�
//����: PTT_flash_tag���������ķ���
//      type��д�����ͣ�����ֵcn_MDR_DDR_main��cn_MDR_DDR_bak��cn_MDR_PCRB
//      value����д�����ֵ
//����: ��
//ע��: ���ܶ�δ��ʽ���ķ������ñ�����������һ���ڲ������������Ŀ�����
//      �Ƿ��Ѿ���ʽ��
//-----------------------------------------------------------------------------
void __DFFSD_WriteMDR_Item(struct tagStPTT_Flash *PTT_flash_tag,
                            uint32_t type,uint32_t value)
{
    uint32_t PTT_no;
    struct tagFlashChip *chip;
    if(PTT_flash_tag == NULL)
        return;
    if((type==CN_MDR_DDR_MAIN)||(type==CN_MDR_DDR_BAK)||(type==CN_MDR_PCRB))
    {
        PTT_no = PTT_flash_tag->PTT_no;
        chip = PTT_flash_tag->chip;
        __DFFSD_WriteMDR_RollRecord(chip,PTT_no,type,value);
    }
}

//----��Ƿ���Ϊ�Ѹ�ʽ��-----------------------------------------------------
//����: ��MDR_PTT���б��һ������Ϊ�Ѿ���ʽ������MDR_main��MDR_bak�ж�Ҫ�޸ġ�
//������chip����������оƬ
//      PTT_no��������
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_SetMDR_PTT_Formatted(struct tagFlashChip *chip,uint32_t PTT_no)
{
    uint8_t *MDR_buf;
    struct tagMDR_PTT_Mirror *MDR_PTT;

    MDR_buf = (uint8_t*)M_MallocLc(CN_MDR_SIZE, 0);
    if(MDR_buf == NULL)
        return ;
    //���㱻д�ķ�����MDR_PTT��¼�ĵ�ַ
    MDR_PTT = (struct tagMDR_PTT_Mirror *)(MDR_buf +
                        CN_MDR_CDR_SIZE + PTT_no * CN_MDR_PTT_SIZE);
    __DFFSD_ReadMDR_Bak(chip,MDR_buf);
    strcpy(MDR_PTT->PTT_format_flag,"ready");
    __DFFSD_Fill_ECC_MDR_PTT((uint8_t *)MDR_PTT);
    __DFFSD_EraseMDR_Bak(chip);
    //���4�ֽ��ǽ�����־AA5555AA���ʼ�4
    __DFFSD_WriteMDR_Bak(chip,MDR_buf,CN_MDR_SIZE-4);
    __DFFSD_WriteEndFlagBak(chip);

    __DFFSD_ReadMDR_Main(chip,MDR_buf);
    strcpy(MDR_PTT->PTT_format_flag,"ready");
    __DFFSD_Fill_ECC_MDR_PTT((uint8_t *)MDR_PTT);
    __DFFSD_EraseMDR_Main(chip);
    //���4�ֽ��ǽ�����־AA5555AA���ʼ�4
    __DFFSD_WriteMDR_Main(chip,MDR_buf,CN_MDR_SIZE-4);
    __DFFSD_WriteEndFlagMain(chip);
    free(MDR_buf);
    return;
}
//----����MDR_main-------------------------------------------------------------
//����: ����MDR_main��MDR_main����ʼ��Ĭ��Ϊ0
//������chip����������оƬ
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_EraseMDR_Main(struct tagFlashChip *chip)
{
    uint32_t blocks,loop;
    blocks = (CN_MDR_SIZE + chip->block_size -1) / chip->block_size;
    for(loop = 0; loop < blocks; loop++)
        chip->erase_block(loop);
}

//----����MDR_bak-------------------------------------------------------------
//����: ����MDR_bak��MDR_bak����ʼ��Ĭ��Ϊ0
//������chip����������оƬ
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_EraseMDR_Bak(struct tagFlashChip *chip)
{
    uint32_t blocks,loop;
    blocks = (CN_MDR_SIZE + chip->block_size -1) / chip->block_size;
    for(loop = 0; loop < blocks; loop++)
        chip->erase_block(loop + chip->MDR.MDR_bak_block);
}

