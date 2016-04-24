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
//�ļ�����:flash�ļ�ϵͳ��FDR����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "endian.h"
#include "time.h"
#include "file.h"
#include "flashfile.h"
#include "fdt.h"
#include "ddr.h"

#include <string.h>

//----��ʼ��FDT��--------------------------------------------------------------
//���ܣ���ʼ��FDT���Ѵ洢�黮��Ϊһ����FDT_item������˫����������������FDT
//      ��ĵ�һ��FDT_itemʼ���Ǹ�Ŀ¼�������ֻ���ڷ�����ʽ��ʱ����
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//���أ�true=�ɹ���false=ʧ��
//------------
//���¼�¼:
// 1.����: 2014/3/25
//   ˵��: 1. ������FDT�ṹ���С�ļ��㷽ʽ
//         2. ʱ�����Խṹ
//   ����: ������
//-----------------------------------------------------------------------------
bool_t __DFFSD_InitFDT(struct tagStPTT_Flash *PTT_flash_tag)
{
    uint8_t *FDT_buf;
    struct tagFdtInfo *pl_FDT_item;
    struct tagFlashChip *chip;
    uint32_t true_block,FDT_block_no;
    uint32_t block_capacity,loop;

    chip = PTT_flash_tag->chip;
    FDT_block_no = PTT_flash_tag->DDR_FDSB;
    block_capacity = chip->block_size / sizeof(struct tagFdtInfo);//���������ֻ������һ��ռ��
    PTT_flash_tag->FDT_capacity = block_capacity;
    PTT_flash_tag->FDT_free_num= block_capacity - 1;
    FDT_buf = chip->block_buf;

    for(loop=0; loop < block_capacity; loop++)
    {
        pl_FDT_item = (struct tagFdtInfo*)
                      (FDT_buf + (loop * sizeof(struct tagFdtInfo)));
        pl_FDT_item->name_buf_len = CN_FLASH_FILE_NAME_LIMIT +1;
        pl_FDT_item->mattr = 0xff;  //��ʾ����FDT_item
        //���°�����FDT_item���޸ĺʹ�������ʱ�����Ϊ2009-01-04-12-00-00��
        //����djyos�����ա�
        /* �����ʼʱ���趨û�п������� */
        pl_FDT_item->CreateTime = 0x12345678;
        pl_FDT_item->ModTime = 0x87654321;
        strcpy(pl_FDT_item->name,"free item");
        fill_little_32bit(pl_FDT_item->file_size,0,0);
        fill_little_32bit(pl_FDT_item->file_size,1,0);

        if(loop == 0)   //���Ǹ�Ŀ¼��
        {
            //��Ŀ¼���next��previousָ�����ڰѿ��п鴮������
            //��Ŀ¼���previousָ��ָ��FDTĩ��
            fill_little_32bit(pl_FDT_item->previous,0,block_capacity-1);
            //��Ŀ¼���nextָ��ָ���һ��������
            fill_little_32bit(pl_FDT_item->next,0,1);
            //��Ŀ¼���file_sizeָ����;���⣬ǰ4�ֽڼ�¼�ſ���FDT������
            fill_little_32bit(pl_FDT_item->file_size,0,block_capacity-1);
            //��Ŀ¼���file_sizeָ����;���⣬��4�ֽڼ�¼����FDT������
            fill_little_32bit(pl_FDT_item->file_size,1,block_capacity);
            //��Ŀ¼��ĸ�Ŀ¼ָ��parent����
            fill_little_32bit(pl_FDT_item->parent,0,CN_LIMIT_UINT32);
            //��Ŀ¼�����Ŀ¼ָ��fstart_dson��ʱ����
            fill_little_32bit(pl_FDT_item->fstart_dson,0,CN_LIMIT_UINT32);
            //��Ŀ¼�Ǹ�ֻ��Ŀ¼
            pl_FDT_item->mattr = CN_FS_ATTR_DIRECTORY + CN_FS_ATTR_READ_ONLY;
            strcpy(pl_FDT_item->name,"root folder");
        }else if(loop == block_capacity-1)  //���е����һ��
        {
            //previousָ��ǰһ��
            fill_little_32bit(pl_FDT_item->previous,0,block_capacity-2);
            //���һ���nextָ��ָ���Ŀ¼��
            fill_little_32bit(pl_FDT_item->next,0,0);
            fill_little_32bit(pl_FDT_item->parent,0,CN_LIMIT_UINT32);
            fill_little_32bit(pl_FDT_item->fstart_dson,0,CN_LIMIT_UINT32);
        }else
        {
            //previousָ��ָ��ǰһ��
            fill_little_32bit(pl_FDT_item->previous,0,loop - 1);
            //nextָ��ָ���һ��
            fill_little_32bit(pl_FDT_item->next,0,loop + 1);
            fill_little_32bit(pl_FDT_item->parent,0,CN_LIMIT_UINT32);
            fill_little_32bit(pl_FDT_item->fstart_dson,0,CN_LIMIT_UINT32);
        }
    }
    //�ѻ�����д��flash����
    true_block = __DFFSD_WriteNewBlock(PTT_flash_tag,
                                            FDT_block_no,0,chip->block_size);
    if(true_block == CN_LIMIT_UINT32)   //���д�����
        return false;
    if(true_block != FDT_block_no)      //��������˿齻��
        __DFFSD_UpdateMAT_Item(PTT_flash_tag,FDT_block_no,true_block);
    return true;
}
//----FDT������1��-------------------------------------------------------------
//���ܣ���FDT��������󣬵��ñ���������1��洢�������������ӵĴ洢������Ϊһ��
//      ��FDT_item�飬Ȼ����˫���������������������ӵ����б���ȥ��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//���أ�true=�ɹ���false=ʧ�ܣ�һ������Ϊû�п��п�
//-----------------------------------------------------------------------------
bool_t __DFFSD_AddFDTBlock(struct tagStPTT_Flash *PTT_flash_tag)
{
    uint8_t *FDT_buf;
    struct tagFdtInfo *pl_FDT_item,FDT_item;
    struct tagFlashChip *chip;
    uint32_t FDT_item_new_start,free_item,new_block_no,true_block;
    uint32_t block_capacity,last_block_no;
    uint32_t loop;

    if(PTT_flash_tag->valid_sum <= 2)
        return false;   //ʣ����ÿ���С�ڵ���2ʱ������һ���FDT���Ǻ��������
    chip = PTT_flash_tag->chip;
    FDT_item_new_start = PTT_flash_tag->FDT_capacity;
    block_capacity = chip->block_size / 256;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,0,&FDT_item);
    new_block_no = __DFFSD_AllocateBlock(PTT_flash_tag); //����һ��flash��
    if(new_block_no == CN_LIMIT_UINT32)
        return false;
    FDT_buf = chip->block_buf;
    pl_FDT_item = (struct tagFdtInfo*)FDT_buf;
    //�������ĵ�һ��previousָ��ָ���Ŀ¼��
    fill_little_32bit(pl_FDT_item->previous,0,0);
    //�������ĵ�һ��nextָ��ָ���������ĵڶ���
    fill_little_32bit(pl_FDT_item->next,0,(FDT_item_new_start + 1));

    pl_FDT_item = (struct tagFdtInfo*)(FDT_buf + (block_capacity-1) * 256);
    //�����������һ���previousָ��ָ�����ڶ��-2����Ϊ��Ŀ�Ŵ�0��ơ�
    fill_little_32bit(pl_FDT_item->previous,0,
                        (FDT_item_new_start + block_capacity -2));
    if(0 == PTT_flash_tag->FDT_free_num)    //�Ѿ�û�п���FDT_item
    {
        //��Ŀ¼���previousָ��ָ����������ĩ��
        fill_little_32bit(FDT_item.previous,0,
                            FDT_item_new_start + block_capacity-1);
        //��Ŀ¼���nextָ��ָ��������������
        fill_little_32bit(FDT_item.next,0,FDT_item_new_start);
        //��Ŀ¼��д��flash��

        //�����������һ���nextָ��ָ���Ŀ¼��
        fill_little_32bit(pl_FDT_item->next,0,0);
    }else       //��ʣ���������FDT_item
    {
/* todo jzl ��δ����߼������⣬����֤*/
        free_item = pick_little_32bit(FDT_item.next, 0);//����һ������FDT_item����Ŀ��
        fill_little_32bit(FDT_item.next, 0, FDT_item_new_start);//��Ŀ¼���nextָ��ָ��������������
        //debug FDT_item �������뻻������Ϊ����Ļ�ûд�롣ע�Ⱑ��
        //����һ������FDT���������
        __DFFSD_ReadFDT_Item(PTT_flash_tag,free_item,&FDT_item);
        //ԭ��һ�����������ǰָ��ָ�������������һ��
        fill_little_32bit(FDT_item.previous,0,
                            FDT_item_new_start + block_capacity-1);
        //debug
        //���д�룬ҲҪ��������д��ɣ���ȻӰ��0x40��0x7f��ĵ���
        //ԭ��һ��������д��flash
        if(! __DFFSD_WriteFDT_Item(PTT_flash_tag,free_item,&FDT_item))
            return false;
        //�����������һ���nextָ��ָ��ԭ��һ��������
        fill_little_32bit(pl_FDT_item->next,0,free_item);
    }
    for(loop=1; loop < block_capacity-1; loop++)
    {//������������ĩFDT��Ŀ���������FDT��Ŀ��������
        pl_FDT_item = (struct tagFdtInfo*)(FDT_buf + loop * 256);
        fill_little_32bit(pl_FDT_item->previous,0,
                            FDT_item_new_start + loop - 1);
        fill_little_32bit(pl_FDT_item->next,0,
                            FDT_item_new_start + loop + 1);
    }
    true_block = __DFFSD_WriteNewBlock(PTT_flash_tag,
                                            new_block_no,0,chip->block_size);
    if(true_block == CN_LIMIT_UINT32)
        return false;  //��ʱ��FDT��Ŀ¼���ָ���Ѿ��ı䲢д��flash�����¿黺��
                       //��ȴû��д��flash�������FDT��free�������ѣ����������
                       //����ʱ�������ʱ���ῴ����Ŀ¼���е�һ��������ĺ����
                       //��MAT��FDT���������
    PTT_flash_tag->FDT_capacity += block_capacity;
    PTT_flash_tag->FDT_free_num += block_capacity;
    //��Ŀ¼���file_sizeָ����;���⣬ǰ4�ֽڼ�¼�ſ���FDT������
    fill_little_32bit(FDT_item.file_size,0,PTT_flash_tag->FDT_free_num);
    //��Ŀ¼���file_sizeָ����;���⣬��4�ֽڼ�¼����FDT������
    fill_little_32bit(FDT_item.file_size,1,PTT_flash_tag->FDT_capacity);
    if(! __DFFSD_WriteFDT_Item(PTT_flash_tag,0,&FDT_item))
         return false;
    //loop����FDT�׿�Ŀ�ţ����ڶ�λMAT���е�FDT��
    loop = PTT_flash_tag->DDR_FDSB;
    do  //��ѭ�������ҵ�MAT����FDT�������һ��
    {
        last_block_no = loop;
        loop = PTT_flash_tag->DDR_MAT[last_block_no].next;
    }while(loop != last_block_no);
    //����������ʵ��д��Ŀ�ż��뵽MAT���е�FDT�������
    __DFFSD_AddMAT_Item(PTT_flash_tag,last_block_no,true_block);
    if(!__DFFSD_WriteDDR(PTT_flash_tag))
        return false;
    chip->erase_block(PTT_flash_tag->PCRB_no + PTT_flash_tag->start_block);
    return true;
}
//----��һ��FDT_item��ӵ�����-------------------------------------------------
//����: ��һ���Ѿ���������FDT_item���뵽�����������У���Ϊ��һ����㡣
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      FDT_item_no�����ͷŵ���Ŀ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_AddFDTFreeItem(struct tagStPTT_Flash *PTT_flash_tag,
                         uint32_t FDT_item_no)
{
    uint32_t free_first;
    struct tagFdtInfo FDT_item;

    __DFFSD_ReadFDT_Item(PTT_flash_tag,0,&FDT_item);   //������Ŀ¼��
    PTT_flash_tag->FDT_free_num++;
    //FDT���Ŀ¼���file_sizeָ��������;:ǰ4�ֽڼ�¼����FDT������
    fill_little_32bit(FDT_item.file_size,0,PTT_flash_tag->FDT_free_num);
    free_first = pick_little_32bit(FDT_item.next,0);
    if(PTT_flash_tag->FDT_free_num == 1)      //ԭ���������ǿյ�
    {
        //��Ŀ¼�����ǰ���ָ���ָ��Ŀ����
        fill_little_32bit(FDT_item.next,0,FDT_item_no);
        fill_little_32bit(FDT_item.previous,0,FDT_item_no);
        __DFFSD_WriteFDT_Item(PTT_flash_tag,0,&FDT_item);  //д���Ŀ¼��
        //�������ͷ���
        __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
        //Ŀ������ǰ���ָ���ָ���Ŀ¼��
        fill_little_32bit(FDT_item.next,0,0);
        fill_little_32bit(FDT_item.previous,0,0);
        FDT_item.mattr |= CN_FS_ATTR_DELETED;

        fill_little_32bit(FDT_item.parent,0,CN_LIMIT_UINT32);
        fill_little_32bit(FDT_item.fstart_dson,0,CN_LIMIT_UINT32);
        //���޸���д��flash��
        __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
        //Ϊ����޶ȱ������ݻָ���Ϣ�����޸ĳ������������������
    }else               //��������ǿ�
    {
        //��Ŀ¼�����ǰָ���ָ��Ŀ����
        fill_little_32bit(FDT_item.next,0,FDT_item_no);
        __DFFSD_WriteFDT_Item(PTT_flash_tag,0,&FDT_item);  //д���Ŀ¼��
        //�������ͷ���
        __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);

        fill_little_32bit(FDT_item.parent,0,CN_LIMIT_UINT32);
        fill_little_32bit(FDT_item.fstart_dson,0,CN_LIMIT_UINT32);
        //���ͷ���ǰָ��ָ��ԭ���һ��������
        fill_little_32bit(FDT_item.next,0,free_first);

        fill_little_32bit(FDT_item.previous,0,0);

        FDT_item.mattr |= CN_FS_ATTR_DELETED;

        strcpy(FDT_item.name,"free item");
        //���ͷ���д��flash��
        __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
        //����ԭ��һ��������
        __DFFSD_ReadFDT_Item(PTT_flash_tag,free_first,&FDT_item);
        //ԭ��һ��������ĺ�ָ��ָ���ͷ���
        fill_little_32bit(FDT_item.previous,0,FDT_item_no);
        //ԭ���һ��������д��flash
        __DFFSD_WriteFDT_Item(PTT_flash_tag,free_first,&FDT_item);
    }
}

//----����һ��æFDT_item-------------------------------------------------------
//����: ��һ��FDT_item����ԭ����FDT���й�������������ɾ���ļ�/Ŀ¼�ĵ�һ����
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      FDT_item_no������������Ŀ��
//����: true = �ɹ���false = ʧ�ܣ�һ����ӦΪFDT_item_no����Ŀ¼/�ļ�
//-----------------------------------------------------------------------------
bool_t __DFFSD_AcnodeFDTItem(struct tagStPTT_Flash *PTT_flash_tag,
                         uint32_t FDT_item_no)
{
    uint32_t parent,elder,less,eldest;     //���ͷ���ĸ����֡��ܡ�������Ŀ��
    struct tagFdtInfo FDT_item;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item); //����Ŀ����
    if((FDT_item.mattr & CN_FS_ATTR_DELETED) != 0)
        return false;   //Ŀ��FDT_item��һ��������/��ɾ����
    if((((FDT_item.mattr & CN_FS_ATTR_DIRECTORY) == 1) &&
          (pick_little_32bit(FDT_item.fstart_dson,0)) != CN_LIMIT_UINT32))
        return false;   //Ŀ��FDT_item�����ļ�/Ŀ¼��Ŀ¼
    parent = pick_little_32bit(FDT_item.parent,0);    //ȡ��FDT��Ŀ��
    elder = pick_little_32bit(FDT_item.previous,0);   //ȡ��FDT��Ŀ��
    less = pick_little_32bit(FDT_item.next,0);        //ȡ��FDT��Ŀ��
    if(elder == less)       //�֡���FDT��Ŀ�����
    {
        if(elder == FDT_item_no)    //FDT_item_no�Ǹ�Ŀ¼��Ψһ���ļ�/Ŀ¼
        {
            __DFFSD_ReadFDT_Item(PTT_flash_tag,parent,&FDT_item);//������
            //�����������Ϊû������
            fill_little_32bit(FDT_item.fstart_dson,0,CN_LIMIT_UINT32);
            __DFFSD_WriteFDT_Item(PTT_flash_tag,parent,&FDT_item); //д����
        }else                   //��Ŀ¼���������ļ�/Ŀ¼
        {
            //������(��)FDT��
            __DFFSD_ReadFDT_Item(PTT_flash_tag,elder,&FDT_item);
            fill_little_32bit(FDT_item.previous,0,elder); //ǰָ��ָ���Լ�
            fill_little_32bit(FDT_item.next,0,elder);     //��ָ��ָ���Լ�
            __DFFSD_WriteFDT_Item(PTT_flash_tag,elder,&FDT_item);  //д��flash
            __DFFSD_ReadFDT_Item(PTT_flash_tag,parent,&FDT_item);  //������
            eldest = pick_little_32bit(FDT_item.fstart_dson,0); //ȡ������Ŀ��
            if(eldest != elder)     //��������ǡ�þ��ǳ���
            {
                //���ø���������
                fill_little_32bit(FDT_item.fstart_dson,0,elder);
                __DFFSD_WriteFDT_Item(PTT_flash_tag,parent,&FDT_item);//д����
            }
        }
    }else       //�֡�����Ŀ�Ų���ȣ�˵�����������ͬ��Ŀ¼��������3��
    {
        __DFFSD_ReadFDT_Item(PTT_flash_tag,parent,&FDT_item);  //������
        eldest = pick_little_32bit(FDT_item.fstart_dson,0);   //ȡ�������
        if(eldest == FDT_item_no)   //��������ǡ�þ��ǳ���
        {
            //���ø���������
            fill_little_32bit(FDT_item.fstart_dson,0,less);
            __DFFSD_WriteFDT_Item(PTT_flash_tag,parent,&FDT_item); //д����
        }
        __DFFSD_ReadFDT_Item(PTT_flash_tag,elder,&FDT_item);   //���ֽ��
        fill_little_32bit(FDT_item.next,0,less); //�ֽ������ָ��ָ��ܽ��
        __DFFSD_WriteFDT_Item(PTT_flash_tag,elder,&FDT_item);  //д�ֽ��
        __DFFSD_ReadFDT_Item(PTT_flash_tag,less,&FDT_item);    //���ܽ��
        //�ܽ�����ǰָ��ָ��ܽ��
        fill_little_32bit(FDT_item.previous,0,elder);
        __DFFSD_WriteFDT_Item(PTT_flash_tag,less,&FDT_item);   //д�ܽ��
    }
    //ע�⣬Ϊ����޶ȱ������ݻָ���Ϣ�����Ա����������κ��޸�
    return true;
}

//----�ͷ�FDT_item-------------------------------------------------------------
//����: �ͷ�һ��FDT_item��FDT���У������������������
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      FDT_item_no�����ͷŵ���Ŀ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_FreeFDTItem(struct tagStPTT_Flash *PTT_flash_tag,
                         uint32_t FDT_item_no)
{
    struct tagFlashChip *chip = PTT_flash_tag->chip;
    //��Ŀ¼������ȡ����������FDT��
    if(__DFFSD_AcnodeFDTItem(PTT_flash_tag,FDT_item_no))
    {
        //�Ѹղ�ȡ����FDT����뵽����FDT������ȥ
        __DFFSD_AddFDTFreeItem(PTT_flash_tag,FDT_item_no);
    }
    chip->erase_block(PTT_flash_tag->PCRB_no + PTT_flash_tag->start_block);
}


//----����FDT_item-------------------------------------------------------------
//���ܣ���FDT���з���һ��FDT_item�������ӿ���������ȡ�������û�п���FDT_item��
//      �����DFFSD_add_FDT_block��������һ�顣
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//���أ��·����FDT_item��ţ������򷵻�cn_limit_uint32
//-----------------------------------------------------------------------------
uint32_t __DFFSD_AllocateFDTItem(struct tagStPTT_Flash *PTT_flash_tag)
{
    uint32_t next_free,next_next_free;
    struct tagFdtInfo FDT_root,FDT_free_item;
    struct tagFlashChip *chip = PTT_flash_tag->chip;

    __DFFSD_ReadFDT_Item(PTT_flash_tag,0,&FDT_root);
    //���п���FDT_item���������Ը�Ŀ¼Ϊ���ĵ�˫��������
    next_free = pick_little_32bit(FDT_root.next,0);

    if(next_free >= PTT_flash_tag->FDT_capacity)
        return CN_LIMIT_UINT32;   //��Ŀ�Ŵ���FDT�������϶�����
    if(PTT_flash_tag->FDT_free_num == 0)
    {
        if(!__DFFSD_AddFDTBlock(PTT_flash_tag))     //�ӳ�FDT��ʧ��
            return CN_LIMIT_UINT32;
        __DFFSD_ReadFDT_Item(PTT_flash_tag,0,&FDT_root);   //���¶���Ŀ¼��
        next_free = pick_little_32bit(FDT_root.next,0); //�ض���Ŀ¼��nextָ��
    }
    //��ȡ��һ�������Ҳ���Ǽ������ظ������ߵ���
    __DFFSD_ReadFDT_Item(PTT_flash_tag,next_free,&FDT_free_item);
    //��ȡ����һ��������ı��
    next_next_free = pick_little_32bit(FDT_free_item.next,0);
    PTT_flash_tag->FDT_free_num--;
    //FDT���Ŀ¼���file_sizeָ��������;:ǰ4�ֽڼ�¼����FDT������
    fill_little_32bit(FDT_root.file_size,0,PTT_flash_tag->FDT_free_num);
    if(next_next_free == 0) //next_next_freeָ��ָ���0��������һ��������
    {
        fill_little_32bit(FDT_root.next,0,0);
        fill_little_32bit(FDT_root.previous,0,0);
        if(! __DFFSD_WriteFDT_Item(PTT_flash_tag,0,&FDT_root))
            return CN_LIMIT_UINT32;
    }else
    {
        //��ȡ����һ�������������
        __DFFSD_ReadFDT_Item(PTT_flash_tag,next_next_free,&FDT_free_item);
        //��Ŀ¼���nextָ��ָ������һ������������
        fill_little_32bit(FDT_root.next,0,next_next_free);
        //����һ���������previousָ��ָ���Ŀ¼��
        fill_little_32bit(FDT_free_item.previous,0,0);
        //��Ŀ¼��д��flash
        if(! __DFFSD_WriteFDT_Item(PTT_flash_tag,0,&FDT_root))
            return CN_LIMIT_UINT32;
        //����һ��������д��flash
        if(!__DFFSD_WriteFDT_Item(PTT_flash_tag,next_next_free,&FDT_free_item))
            return CN_LIMIT_UINT32;
    }
    //���д��FDT��ʱ���������滻������Ҫˢ��DDR��
    if(!__DFFSD_WriteDDR(PTT_flash_tag))
        return false;
    chip->erase_block(PTT_flash_tag->PCRB_no + PTT_flash_tag->start_block);
    return next_free;
}
//----ѹ��FDT��----------------------------------------------------------------
//���ܣ���FDT���п��б�����ռ�Ĵ洢����������1��blockʱ�����Ե��ñ�����ѹ��֮��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//���أ���
// db ������
//-----------------------------------------------------------------------------
void __DFFSD_CompressFDT(struct tagStPTT_Flash *PTT_flash_tag)
{
}

struct FDT_test
{
    u8 res[16];
    u32 free,sum;
    u32 parent,son,pre,next;
    u8 nane[216];
};

//----дһ��FDT����------------------------------------------------------------
//����: ��һ��FDT����д��flash��FDT����
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      FDT_item_no����Ŀ��
//      FDT_item��FDT����ָ��
//����: true=�ɹ�д�룬false=ʧ�ܣ�һ��������������û�п��п���滻��
//------------
//���¼�¼:
// 1.����: 2015/3/23
//   ˵��: ������FDT��С�ļ��㷽ʽ
//   ����: ������
//-----------------------------------------------------------------------------
bool_t __DFFSD_WriteFDT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                           uint32_t FDT_item_no,struct tagFdtInfo *FDT_item)
{
    uint32_t true_block;
    struct tagFlashChip *chip;
    uint32_t block_offset,FDT_block_no,loop,FDT_block_serial;
    uint32_t FDT_Size = sizeof(struct tagFdtInfo);

    chip = PTT_flash_tag->chip;
    //����FDT_item_no��fdt���еĿ����

    FDT_block_serial = FDT_item_no * FDT_Size / chip->block_size;
    block_offset = FDT_item_no * FDT_Size % chip->block_size;

    //��ȡFDT�׿�Ŀ��
    FDT_block_no = PTT_flash_tag->DDR_FDSB;
    //���׿鿪ʼ����MAT���������ҵ���FDT_block_serial��FDT��Ŀ��
    for(loop = 0; loop < FDT_block_serial; loop++)
        FDT_block_no = PTT_flash_tag->DDR_MAT[FDT_block_no].next;

    memcpy((chip->block_buf + block_offset), FDT_item, FDT_Size);
    //д��Ŀ���
    true_block = __DFFSD_WriteUpdateBlock(PTT_flash_tag, FDT_block_no,
                                          block_offset, FDT_Size);

    if(true_block != CN_LIMIT_UINT32)
    {//��ȷд��
        if(true_block != FDT_block_no)   //д��ʱ�����˿��滻
        {
            __DFFSD_UpdateMAT_Item(PTT_flash_tag,FDT_block_no,true_block);
            if(! __DFFSD_WriteDDR(PTT_flash_tag))
                return false;
        }
    }else       //д�����
        return false;
    chip->erase_block(PTT_flash_tag->PCRB_no + PTT_flash_tag->start_block);
    return true;
}

//----��һ��FDT����------------------------------------------------------------
//����: ��FDT���ж�ȡһ������
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      FDT_item_no����Ŀ��
//      FDT_item���������ݵ�ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_ReadFDT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                           uint32_t FDT_item_no,struct tagFdtInfo *FDT_item)
{
    struct tagFlashChip *chip;
    uint32_t block_offset,FDT_block_abs,FDT_block_no,loop,FDT_block_serial;
    chip = PTT_flash_tag->chip;
    //����FDT_item_no��fdt���еĿ����
    FDT_block_serial = FDT_item_no * sizeof(struct tagFdtInfo) / chip->block_size;
    block_offset = FDT_item_no * sizeof(struct tagFdtInfo) % chip->block_size;
    //��ȡFDT�׿�Ŀ��
    FDT_block_no = PTT_flash_tag->DDR_FDSB;
    //���׿鿪ʼ����MAT���������ҵ���FDT_block_serial��FDT��Ŀ��
    for(loop = 0; loop < FDT_block_serial; loop++)
        FDT_block_no = PTT_flash_tag->DDR_MAT[FDT_block_no].next;
    //����FDT_block��оƬ�еľ��Կ��
    FDT_block_abs = FDT_block_no + PTT_flash_tag->start_block;
    chip->read_data_with_ecc(FDT_block_abs,block_offset,
                            (uint8_t*)FDT_item,sizeof(struct tagFdtInfo));
}

//----����һ��FDT_item---------------------------------------------------------
//���ܣ�����һ��FDT_item���ڴ�Ӱ�񣬵���û�д����������4��ָ�롣
//����: FDT_item���������FDT��Ŀָ��
//      attr����Ŀ����
//      name����Ŀ����
//����: ��
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ���Ķ��ļ�ʱ�����Ը�ʽ
//   ����: ������
//-----------------------------------------------------------------------------
void __DFFSD_AsmFDT_Item(struct tagFdtInfo *FDT_item,union file_attrs file_attr,
                          char *name)
{
    uint8_t attr_mirror;
    s64 nowtime;
    struct tm *file_time;
    ufast_t i;
    //����ڿ��ܵ���ֲ���⣬����ֱ��ʹ��λ���Ͻṹ��attr����
    attr_mirror = 0;
    if(file_attr.bits.read_only)
        attr_mirror +=CN_FS_ATTR_READ_ONLY;
    if(file_attr.bits.hidden)
        attr_mirror +=CN_FS_ATTR_HIDDEN;
    if(file_attr.bits.folder)
        attr_mirror +=CN_FS_ATTR_DIRECTORY;
    if(file_attr.bits.archive)
        attr_mirror +=CN_FS_ATTR_ARCHIVE;
    if(file_attr.bits.close_delete)
        attr_mirror +=CN_FS_ATTR_CLOSEDELETE;
    FDT_item->mattr = attr_mirror;
    FDT_item->name_buf_len = CN_FLASH_FILE_NAME_LIMIT +1;
    for(i = 0; i < 8; i++)
        FDT_item->file_size[i] = 0;    //�ļ��ߴ�=0
    //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
    if(strlen(name) <= CN_FLASH_FILE_NAME_LIMIT)//��ȡ��������
        strcpy(FDT_item->name,name);
    else
    {
        memcpy(FDT_item->name,name,CN_FLASH_FILE_NAME_LIMIT);
        FDT_item->name[CN_FLASH_FILE_NAME_LIMIT] = '\0';
    }

    __DFFSD_SetItemTime(FDT_item);
    __DFFSD_UpdateItemTime(FDT_item);

    return;
}

//----��ȡ�ļ�(Ŀ¼)����-------------------------------------------------------
//���ܣ���FDT������ڴ澵���е��ж�ȡ�ļ�(Ŀ¼)��Ϣ������ʼ���ļ��ṹָ��fp��
//������PTT_flash_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//      fp�������ļ�(Ŀ¼)���Ե��ļ�ָ��
//      FDT_item����flash�ж�����FDT����
//���أ���
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: �޸��ļ�ʱ���ʽ
//   ����: ������
//-----------------------------------------------------------------------------
void __DFFSD_UasmFDT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                              struct tagFileRsc *fp,
                              struct tagFdtInfo *FDT_item)
{
    uint8_t attr_mirror;
    uint32_t name_len;

    fp->file_attr.all = 0;
    attr_mirror = FDT_item->mattr;
    if(attr_mirror & CN_FS_ATTR_READ_ONLY)
        fp->file_attr.bits.read_only =1;
    if(attr_mirror & CN_FS_ATTR_DIRECTORY)
        fp->file_attr.bits.folder =1;
    if(attr_mirror & CN_FS_ATTR_HIDDEN)
        fp->file_attr.bits.hidden =1;
    if(attr_mirror & CN_FS_ATTR_ARCHIVE)
        fp->file_attr.bits.archive =1;
    if(attr_mirror & CN_FS_ATTR_CLOSEDELETE)
        fp->file_attr.bits.close_delete =1;

    fp->CreateTime = FDT_item->CreateTime;
    fp->ModTime = FDT_item->ModTime;
    //���ļ��ߴ磬Ϊ��ֹ����Ϊ��չ���²���Ҫ���鷳�����޷�������λ��
    fp->file_size = pick_little_32bit(FDT_item->file_size,0)
            +((uint64_t)pick_little_32bit(FDT_item->file_size,1)<<32);
    name_len = strnlen(FDT_item->name,CN_FLASH_FILE_NAME_LIMIT+1);
    if(name_len <= CN_FLASH_FILE_NAME_LIMIT)//�������ֲ�����cn_flash_file_name_limit
        strcpy(fp->name, FDT_item->name);
    else
    {
        memcpy(fp->name, FDT_item->name,CN_FLASH_FILE_NAME_LIMIT);
        fp->name[CN_FLASH_FILE_NAME_LIMIT] = '\0';
    }
    //fp->open_counter = 0;
    return ;
}

//----������Ŀ¼��Ŀ-------------------------------------------------------
//���ܣ��Ӹ�Ŀ¼��FDT������ڴ澵���ж�ȡ��Ŀ¼��Ϣ��д��PTT_flash_tag��
//������PTT_flash_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//      FDT_item����flash�ж�����FDT����
//���أ���
//-----------------------------------------------------------------------------
void __DFFSD_UasmRootFolder(struct tagStPTT_Flash *PTT_flash_tag,
                                struct tagFdtInfo *FDT_item)
{
    //��Ŀ¼��FDT��Ŀ��file_size��������;��ǰ4�ֽ�ΪFDT���п���FDT��Ŀ������
    //4���ֽ���FDT������FDT��Ŀ��
    PTT_flash_tag->FDT_free_num = pick_little_32bit(FDT_item->file_size,0);
    PTT_flash_tag->FDT_capacity = pick_little_32bit(FDT_item->file_size,1);
    return ;
}

//----�޸�fdt��Ŀ�е���ʼ���--------------------------------------------------
//����: ��һ���ļ����׿鱻�޸�ʱ�����ļ���fdt��Ŀ�б������ʼ���Ҳ�����޸ġ�ֻ
//      ���ڷ���ĥ��ƽ��齻��ʱ�����п��ܵ��ñ����������ڵ��ñ�����ʱ������д
//      ���������޸�ĥ���������ʹ�������齻����Ҳ�����ñ�׼�Ŀ���亯��
//      __allocate_block������п飬����ֱ�Ӵ�free����ȡһ�飬���޸�ĥ�������
//      �������Ա��ⷢ�������ס�
//����: PTT_flash_tag,��������flash�����Ĵ洢ý���ǩ
//      FDT_item_no,fdt��Ŀ��š�
//      new_block_no���滻���
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_UpdateFDT_FstartDson(struct tagStPTT_Flash *PTT_flash_tag,
                       uint32_t FDT_item_no,uint32_t new_block_no)
{
    struct tagFdtInfo FDT_item;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    fill_little_32bit(FDT_item.fstart_dson,0,new_block_no);
    __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----��FDT���޸��ļ��ߴ�------------------------------------------------------
//����: �޸�FDT�����е��ļ��ߴ���
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ�룬�ɵ��÷���֤����Ŀ¼�����ļ���
//      new_size���µ��ļ��ߴ�
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_UpdateFDT_FileSize(struct tagStPTT_Flash *PTT_flash_tag,
                                  struct tagFileRsc *fp,sint64_t new_size)
{
    uint32_t FDT_item_no;
    struct tagFdtInfo FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    fill_little_32bit(FDT_item.file_size,0,(u32)new_size);
    //Ϊ��ֹ����Ϊ��չ���²���Ҫ���鷳�����޷�������λ��
    fill_little_32bit(FDT_item.file_size,1,(uint64_t)new_size>>32);
    __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----��FDT���޸��ļ���------------------------------------------------------
//����: �޸�FDT�����е��ļ�(Ŀ¼)����
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ��
//      name���µ��ļ�(Ŀ¼)��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_UpdateFDT_ItemName(struct tagStPTT_Flash *PTT_flash_tag,
                                  struct tagFileRsc *fp,char *name)
{
    uint32_t FDT_item_no;
    struct tagFdtInfo FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
    if(strlen(name) <= CN_FLASH_FILE_NAME_LIMIT)//��ȡ��������
        strcpy(FDT_item.name,name);
    else
    {
        memcpy(FDT_item.name,name,CN_FLASH_FILE_NAME_LIMIT);
        FDT_item.name[CN_FLASH_FILE_NAME_LIMIT] = '\0';
    }
    __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----��FDT���޸��ļ�����------------------------------------------------------
//����: �޸�FDT�����е��ļ�(Ŀ¼)������
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ��
//      attr���µ��ļ�(Ŀ¼)����
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_UpdateFDT_ItemAttr(struct tagStPTT_Flash *PTT_flash_tag,
                                  struct tagFileRsc *fp,union file_attrs attr)
{
    uint32_t FDT_item_no;
    struct tagFdtInfo FDT_item;
    uint8_t attr_mirror;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    //����ڿ��ܵ���ֲ���⣬����ֱ��ʹ��λ���Ͻṹ��attr����
    attr_mirror = FDT_item.mattr;
    if(attr.bits.read_only)
        attr_mirror +=CN_FS_ATTR_READ_ONLY;
    if(attr.bits.hidden)
        attr_mirror +=CN_FS_ATTR_HIDDEN;
    if(attr.bits.archive)
        attr_mirror +=CN_FS_ATTR_ARCHIVE;   //����ֻ��ʼ��3�����ԣ��������ݲ���
                                            //�����޸�Ŀ¼/�ļ�����
    FDT_item.mattr = attr_mirror;
    __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    return ;
}
//----------------------------------------------------------------------------
//����:
//����:
//
//����:
//����: �ȴ�C��local�������,�ٽ��32λʱ������
//----------------------------------------------------------------------------
inline void __DFFSD_UpdateItemTime(struct tagFdtInfo *item)
{
    s64 Time;
    Time = TM_Time(NULL);
    fill_little_32bit((u8*)&(item->ModTime), 0, (u32)Time);
}
//----------------------------------------------------------------------------
//����: ����FDT�Ĵ���ʱ������
//����:
//
//����:
//����: �ȴ�C��local�������,�ٽ��32λʱ������
//----------------------------------------------------------------------------
inline void __DFFSD_SetItemTime(struct tagFdtInfo *item)
{
    s64 Time;
    Time = TM_Time(NULL);
    fill_little_32bit((u8*)&(item->CreateTime), 0, (u32)Time);
}

