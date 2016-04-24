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
//�ļ�����:flash�ļ�ϵͳ������ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
/*
����дȫ�ƣ�
MDR  memory medium description record   �洢����(оƬ)������¼
CDR  chip description record            MDR��оƬ��������
PTT  deedbox                            ������ҲָMDR�з�����������
DDR  deedbox description record         ����������¼
DBL  DDR_mirror blocks list             �洢DDR��Ϣ�Ŀ���б�
ARB  abrasion record base               ĥ���¼����
ART  abrasion record table              ĥ���¼��
FDSB file description starting block    �ļ��������׿��ţ����ڱ�����
FDT  file description table             �ļ�������
MAT  memory allocation table            �洢�������
PCRB power cut restore block            ����ָ���
ECCF ecc flag                           eccʹ�ܱ�־��1=ʹ�ܣ�0=��ʹ�ܡ�

�ڴ�������ʱӦ���ƶ��÷����Ƿ�ʹ��eccУ�飬��Щר�������洢ͼ�λ���������
������������ʹ��ecc�ģ��������Դ����ߴ�ȡ�ٶȡ�

��ע:MDR��������ECCУ�飬����ÿ�ι����޸Ķ���Ҫ��ECC�롣����MDR��Ҫ��ECCУ�顣

A��MDR���ļ�ϵͳ����ʹ�ã���ģ���ʼ����module_init_��������ȡ�������ݰ���
    1��CDR,оƬ��ʼ����־(8�ֽ�=djyosfs)��flashоƬ�ܿ������������飩����ߴ磬
       MDR_bak��ţ�����������ռ��512�ֽڣ����з�������ѭ��д�룬�������Ҫ
       ѭ����¼���ÿ����ģ�����Ϊ���ÿ����Ƕ�̬��ǿ����¼Ҳû�ж��ʵ�����壬��
       �������MDR��ĥ�𣬹��ö�̬�����ķ�����
    2��1�ŷ���PTT,��ʽ����־(8�ֽ�=ready/unready)��������(32�ֽ�)����ʼ��ţ�
       ռ�ÿ�����������ָ���Ŀ�ţ�ÿ����1024�θı�һ�Ρ�2��DDR�����׿���
       ��ÿʹ��1024�θ�һ�Σ���������ռ��1024�ֽڣ�ѭ��д�����ӳ�������
    3��2�ŷ���PTT,........
    4��һ��оƬ�Ͽɽ����ķ�������Ĭ��������3��������оƬ�������޸ġ�

    flash�洢����������nand����nor�ͣ�����ʹ��ͷ�������ÿ�洢����һ��洢��MDR��
    �ڶ���洢����MDR������ĳЩ���Ʊ�֤��MDR��д������٣������������٣�ֻ�е�
    ������MDRʱ����ʹ�ñ���MDR������ʱ���ȶ���MDR�����д����ٶ�����MDR��������
    ���Ա�֤��
    1����֤��MDR�������ĥ��
    2�����ҽ���д��MDRʱǡ��������Ҫ�ӱ�����
    3������MDR��Ĳ�д�������١�

B��DDR���Դ洢��˵����DDR���׿��Ϊ��׼��
    1��DBL,DDR����ռ�ݵĿ���б�ÿ���=2��4,����ͱ��ݱ��һ
    2��FDSB�ļ��������׿��ţ�����=sizeof(mat_size_t)��ÿ�޸�1000�θ�һ�Ρ�
    3��ARBĥ�����������4�ֽڣ���¼ĥ������ĵ�����4�ֽڶ���
    4��ARTĥ�������ÿ��ռ��2�ֽڣ����ϻ������øÿ�ʵ��ĥ��������Զ�����
    5��MAT�洢�������ÿ��8�ֽڣ�����ǰһ��ͺ�һ��Ŀ�ţ�
       �׿��ǰһ�鱣��Ŀ¼���ţ�ĩ��ĺ�һ��ָ���Լ�����ÿ���ļ�����һ����
       ���飺ǰ��ָ����ȵ������ڱ����ţ����Ǳ����ż�1
       ���п飺ǰ��ָ����ȣ����ڱ����ż�2
       DDR_main�飺ǰ��ָ����ȣ����ڱ����ż�3
       DDR_bak�飺ǰ��ָ����ȣ����ڱ����ż�4
       ����ָ��飺ǰ��ָ����ȣ����ڱ����ż�5
��ע: ����DDR���DBL���ֲ���ͬ������������ȫ��ͬ

����˵��(DDR)ռ�õĿռ��������:
������ռ��x�ֽڣ�y�飬����:y=(x+block_size-1)/block_size
DBL = 4*y(����ͱ��ݱ��һ)
ECCF= 4
ARB = 4
ART = 2*block_sum
FDSB = 4
MAT = 4*2*block_sum,Ϊ��ֹһ��MAT������߽磬MAT�����ʼ��ַӦ����4/8�ֽڶ���ġ�
x = DBL + ARB + ART + FDSB + MAT+�������
��y�ı��ʽ������ʽ���ⷽ�̵õ�:
x= (ECCF+ARB+ART+FDSB+MAT)*block_size + (block_size -1)*4/(block_size-4)
xӦ����һ�����(n~n+4)֮��������ټ��϶�����ļ��ɡ�

E��Ŀ¼��Ϣ��ÿ���ļ�/Ŀ¼��40�ֽڱ�ʾ��Ŀ¼�����Ƕ�̬��

    ��һ���ļ���һ��������漰���޸ģ�
    1���޸ĸÿ鱾��
    2���������������׿飬��Ҫ�޸�Ŀ¼��
    3����MAT��
    4�������Ŀ¼���MAT��ʱ����������棬��A���֡�
*/
#include "stdint.h"
#include "stdlib.h"
#include "endian.h"
#include "rsc.h"
#include "lock.h"
#include "time.h"
#include "systime.h"
#include "file.h"
#include "flashfile.h"
#include "mdr.h"
#include "ddr.h"
#include "fdt.h"

#include <string.h>

//flashоƬ����Դ��㣬����flashоƬ����װ�ڴ˽����
static struct tagRscNode tg_flash_chip_root_rsc;
struct tagSemaphoreLCB *g_ptFlashfileSemp; //����flash_chip�����õ��ź���

//----ж��һ��оƬ------------------------------------------------------------
//����: ���ļ�ϵͳ��ɾ����оƬ��ɾ�����ڸ�оƬ�����з�����ֻ���ڸ�оƬ������
//      ������û�д򿪵��ļ���ʱ��ſ���ɾ��
//����: chip����ɾ����оƬ
//����: �ɹ�ɾ������true�����򷵻�false
//----------------------------------------------------------------------------
bool_t DFFSD_UninstallChip(struct tagFlashChip *chip)
{
    return true;
}

//----��ʽ������------------------------------------------------------------
//���ܣ���ʽ���������÷���Ӧ���Ѿ����������Ҿ���������ʼ�����뵽�豸�����С�
//      A��δ��ʽ���ķ�����ʽ������:
//      1������洢DDR������Ŀ���(2��)
//      2��ɨ��һ�黵�飬��MAT���б�ǻ��飬���Ӻÿ�������2��DDR��
//      3����дDDR�е�DBL��MAT��
//      4��ART�����򣬲���DDR��д��flash�С�
//      5������һ����FDT��
//      6����ʼ��FDT��Ȼ��д��flash��
//      7������MDR����д��flash��
//      B���Ѿ���ʽ���ķ�����ʽ������
//      ��˵��
//������fmt_para1����ʽ��������1=ʹ��eccУ�飬0��ʾ���ã�һ��ר�Ŵ洢��ý���ļ�
//              �ķ������Բ���
//      fmt_para2����ʽ���������ݲ���
//      PTT_device_tag������ʽ���ķ������÷����Ѿ���װ���豸ϵͳ��
//���أ�true=�ɹ���ʽ������ԭ�����Ѿ���ʽ���ö������ٴθ�ʽ���ġ�
//      false=ʧ��
//------------
//���¼�¼:
// 1.����: 2015/3/10
//   ˵��: ɾ��valid_sum���ظ���ֵ
//   ����: ������
//-----------------------------------------------------------------------------
bool_t DFFSD_FormatPTT(u32 fmt_para1,u32 fmt_para2,
                        struct tagPTTDevice *PTT_device_tag)
{
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFlashChip *chip;
    struct tagMAT_Table *MAT_item;
    uint32_t DDR_blocks,valid_blocks;
    uint32_t loop;

    if(PTT_device_tag == NULL)
        return false;
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;
    //DDR����ռ�Ŀ���
    DDR_blocks=(chip->block_size+PTT_flash_tag->DDR_size-1)/chip->block_size;

    PTT_flash_tag->DDR_ARB = 0; //ĥ�������������
    PTT_flash_tag->nand_ecc = (bool_t)fmt_para1;//�Ƿ���ECC
    MAT_item = PTT_flash_tag->DDR_MAT;//ָ���ڴ��н�����MAT��
    valid_blocks = 0;

    for(loop = 0; loop < PTT_flash_tag->block_sum; loop++)//�Է������ܹ滮
    {
        MAT_item[loop].previous = loop;
        MAT_item[loop].next = loop;
        PTT_flash_tag->ART_times[loop] = 0;
        PTT_flash_tag->ART_position[loop] = loop;
        PTT_flash_tag->ART_block_no[loop] = loop;
        if(chip->check_block(loop + PTT_flash_tag->start_block))
        {
            if(valid_blocks < DDR_blocks)
            {//��Щ������DDR_main��
                if(valid_blocks == 0)
                    PTT_flash_tag->DDR_main=loop;
                //��дDBL��,��DDR_DBL����Ч��ָ������ʵ�ʿ��
                PTT_flash_tag->DDR_DBL[valid_blocks] = loop;
                //���õ�loop����DDR_main��
                MAT_item[loop].attr = cn_DDR_main_block;
            }else if(valid_blocks < 2*DDR_blocks)
            {//��Щ������DDR_bak��
                if(valid_blocks == DDR_blocks)
                    PTT_flash_tag->DDR_bak=loop;
                //��дDBL��
                PTT_flash_tag->DDR_DBL[valid_blocks] = loop;
                //���õ�loop����DDR_bak��
                MAT_item[loop].attr = cn_DDR_bak_block;
            }
            else if(valid_blocks == 2*DDR_blocks)
            {//��������Ŀ¼���ո�ʽ���ķ�����Ŀ¼��ֻ��1��
                //��дFDS(Ŀ¼���׿���)
                PTT_flash_tag->DDR_FDSB = loop;
                MAT_item[loop].attr = cn_FDT_block;
            }
            else if(valid_blocks == 2*DDR_blocks+1)
            {//��������ָ���
                //���õ�loop���ǵ���ָ���
                MAT_item[loop].attr = cn_PCRB_block;
                chip->erase_block(loop + PTT_flash_tag->start_block);
                PTT_flash_tag->PCRB_no = loop;
            }
            else
            {//���п�
                //���õ�loop���ǿ��п�
                MAT_item[loop].attr = cn_free_block;
            }
            valid_blocks++; //�ҵ��Ŀ��ÿ���������
        }
        else
        {//����
            //���õ�i���ǻ���
            MAT_item[loop].attr = cn_invalid_block;
        }
    }
    //����Ч������������__DFFSD_write_DDR��������������飬���޸�valid_sum��
    __DFFSD_SortART(PTT_flash_tag); //����ART��
    for(loop = 0; loop<DDR_blocks; loop++)  //�ո�ʽ����DDR��Ȼ��û��д��
    {
        PTT_flash_tag->writed_DDR_main[loop] = false;
        PTT_flash_tag->writed_DDR_bak[loop] = false;
    }
    if( ! __DFFSD_WriteDDR(PTT_flash_tag))         //дDDR��flash
    {
        return false;   //DDR��д��flash��������
    }
    //����4���޸�flash�е�MDR��MDR����ͨ���ϸ�����ĥ�������ǿ��У������֤
    //��ȷ�ģ���ʹ�������Ŀ飬Ҳ�ܱ�֤��ȷ�����ᱨ��
    //���÷�����MDR��DBX����Ϊ"ready"
    __DFFSD_SetMDR_PTT_Formatted(chip,PTT_flash_tag->PTT_no);
    //��DDR_main��DDR_bak��PCRBд��flash�Ĺ�����¼��
    __DFFSD_WriteMDR_Item(PTT_flash_tag,CN_MDR_DDR_MAIN,
                        PTT_flash_tag->DDR_main+PTT_flash_tag->start_block);
    __DFFSD_WriteMDR_Item(PTT_flash_tag,CN_MDR_DDR_BAK,
                        PTT_flash_tag->DDR_bak+PTT_flash_tag->start_block);
    __DFFSD_WriteMDR_Item(PTT_flash_tag,CN_MDR_PCRB,
                        PTT_flash_tag->PCRB_no+PTT_flash_tag->start_block);
    if( ! __DFFSD_InitFDT(PTT_flash_tag))    //дĿ¼��
    {
        return false;
    }


    //���ø�Ŀ¼�����ڵ�FDT����
    if(PTT_device_tag->opened_root!=NULL)
    {
        PTT_device_tag->opened_root->file_medium_tag = 0;
    }

    return true;
}

//----��λ��(д)��------------------------------------------------------------
//����: ȷ����дλ�õĿ�ţ�����ǻ��ڷ�������Կ�ţ������дλ���Ѿ������ļ�
//      �ѷ���Ŀ������򷵻�cn_limit_uint32��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp,�ļ�ָ�룬���÷���ȷ�����ļ�������Ŀ¼
//      offset,��дλ��ƫ����
//����: ���
//----------------------------------------------------------------------------
uint32_t __DFFSD_LocateBlock(struct tagStPTT_Flash *PTT_flash_tag,
                              struct tagFileRsc *fp,sint64_t offset)
{
    uint32_t block_no,loop,next_block,end_block;
    struct tagFlashChip *chip;
    struct tagFdtInfo FDT_item;

    //��ȡ�ļ�����оƬ
    chip = PTT_flash_tag->chip;
    //�����ļ���Ŀ¼��
    __DFFSD_ReadFDT_Item(PTT_flash_tag,(uint32_t)fp->file_medium_tag,&FDT_item);
    //��Ŀ¼����ȡ�ļ��׿���
    next_block = pick_little_32bit(FDT_item.fstart_dson,0);
    block_no = (u32)(offset/chip->block_size); //�����дλ�ô����ļ��ڵڼ���
    //�����ļ�����λ�����ļ��ڵĿ�ţ�
    if(fp->file_size != 0)
        end_block = (u32)((fp->file_size - 1) / chip->block_size);
    else
        end_block = 0;
    if(block_no > end_block) //��дλ�ó����ļ��ѷ���Ŀ�
    {
        return CN_LIMIT_UINT32;
    }
    for(loop = 0; loop < block_no; loop++)  //��MAT���ҵ�Ŀ��λ�����ڿ�
    {
        next_block = PTT_flash_tag->DDR_MAT[next_block].next;
    }
    return next_block;
}

//----��λ���һ��-------------------------------------------------------------
//����: ȷ���ļ����һ��Ŀ��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp,�ļ�ָ�룬���÷���ȷ�����ļ�������Ŀ¼
//����: ���
//----------------------------------------------------------------------------
uint32_t __DFFSD_LastBlock(struct tagStPTT_Flash *PTT_flash_tag,
                            struct tagFileRsc *fp)
{
    uint32_t loop,next_block;
    struct tagFdtInfo FDT_item;

    //�����ļ���Ŀ¼��
    __DFFSD_ReadFDT_Item(PTT_flash_tag,(uint32_t)fp->file_medium_tag,&FDT_item);
    //��Ŀ¼����ȡ�ļ��׿���
    loop = pick_little_32bit(FDT_item.fstart_dson,0);
    do
    {
        next_block = loop;
        loop = PTT_flash_tag->DDR_MAT[next_block].next;
    }while(loop != next_block);
    return next_block;
}

//----��ȡ�ļ�������Ϣ---------------------------------------------------------
//����: ��ȡ���ڷ����ļ������λ����Ϣ��������λ�ÿ�ţ�дλ�ÿ�ţ��ļ�ռ�ÿ�
//      �����Լ��ļ����һ��Ŀ�š�
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp,�ļ�ָ�룬���÷���ȷ�����ļ�������Ŀ¼
//      access_tag�����ڷ��ؽ����ָ��
//����: ��access_tag�з��ظ��ֲ���
//todo: �������û��棬����ÿ�ζ�д����flashоƬ�л�ȡaccess_tag��Ϣ
//----------------------------------------------------------------------------
void __DFFSD_GetAccessInfo(struct tagStPTT_Flash *PTT_flash_tag,
                            struct tagFileRsc *fp,
                            struct tagFileAccess *access_tag)
{
    uint32_t block,next_block,r_block,w_block,blocks=0;
    struct tagFlashChip *chip;
    struct tagFdtInfo FDT_item;
    uint32_t buffed_data; //�ļ�д�������е���������ʣ��������

    buffed_data = Ring_Check(fp->p_write_buf); //���д�������е�������
    //��ȡ�ļ�����оƬ
    chip = PTT_flash_tag->chip;
    r_block = (u32)(fp->read_ptr / chip->block_size);

    w_block = (u32)((fp->write_ptr-buffed_data)/ chip->block_size);

    if(w_block>0)
    {
        if(((fp->write_ptr-buffed_data)% chip->block_size) == 0)
            access_tag->write_block_no = CN_LIMIT_UINT32;
    }
    //�����ļ���Ŀ¼��
    __DFFSD_ReadFDT_Item(PTT_flash_tag,(uint32_t)fp->file_medium_tag,&FDT_item);
    //��Ŀ¼����ȡ�ļ��׿���
    next_block = pick_little_32bit(FDT_item.fstart_dson,0);
    //���ļ���ʵ�ߴ磬Ϊ��ֹ������չ���²���Ҫ���鷳�����޷�������λ��
    //true_sizeֻ��ӳflash�е����ݣ�������д�������е�����
    access_tag->true_size = pick_little_32bit(FDT_item.file_size,0)
            +((uint64_t)pick_little_32bit(FDT_item.file_size,1)<<32);
    do
    {
        block = next_block;
        if(blocks == r_block)
            access_tag->read_block_no = block;
        if(blocks == w_block)
            access_tag->write_block_no = block;
        blocks++;
        next_block = PTT_flash_tag->DDR_MAT[next_block].next;
    }while(block != next_block);
    access_tag->last_block_no = block;
    return ;
}

//----д������ͬ���ļ�������---------------------------------------------------
//����: ִ���ļ�д����֮�󣬵��ñ�����ͬ����������
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp��Ŀ���ļ�ָ��
//      write_len��ʵ��д�볤��
//����: true=�ɹ���false=�д���
//-----------------------------------------------------------------------------
bool_t __DFFSD_SyncBufAfterWrite(struct tagStPTT_Flash *PTT_flash_tag,
                               FILE *fp,uint32_t write_len)
{
    switch(fp->open_mode)
    {
        case EN_R_RB:   //ֻ���ļ����������
        {
        }break;
        case EN_W_WB:   //ֻд�ļ����������
        {
        }break;
        case EN_A_AB:   //ֻд�ļ����������
        {
        }break;
        case EN_R_RB_PLUS:  //��дλ����ͬ�����������������ݣ�ɾ��write_len����
        case EN_W_WB_PLUS:  //����������
        {
            Ring_DumbRead(fp->p_read_buf,write_len);
        }break;
        case EN_A_AB_PLUS:  //��дλ�ÿ��Բ�ͬ����ǰ����__sync_buf_before_read
        {                   //�����Ѿ���֤д���������������������ͻ����д���
                            //���ݿ϶����Ḳ�Ƕ���������ԭ�����ݣ������账��
        }break;
        default:break;
    }
    return true;
}

//----��дһ��:��������--------------------------------------------------------
//����: ��оƬ�黺����������д��flash��ĳ�飬���Ǳ�д���λ�õ�ԭ�����ݣ����
//      ��д��ʱ�������飬����һ���¿��滻֮�����ѻ���Ǽǵ�MAT����(������MATд
//      �뵽flash��)�������滻��Ŀ�š�д��ǰ���ȿ����Ƿ���Ҫ�����������Ҫ����
//      ��Ҫ���ÿ��Ƿ��Ѿ���busy[8]�������ǣ���Ҫ��"�ͷ�-��������-��д"�Ĺ��̽�
//      �п��滻����д��������ǿ黺������offset��ʼ��size���ֽڡ�
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      block_no��Ŀ����
//      offset����ʼ��ַ�ڿ��ڵ�ƫ����
//      size��д�뻺��������
//����: д��ɹ��򷵻�ʵ��д��Ŀ�ţ�
//      д��ʧ�ܷ���cn_limit_uint32���ռ䲻�������������10������ᵼ��ʧ��
//-----------------------------------------------------------------------------
uint32_t __DFFSD_WriteUpdateBlock(struct tagStPTT_Flash *PTT_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size)
{
    uint32_t fail_counter;  //�����������10���������ȷд�룬�򷵻�
    uint32_t write_result,result_block,PTT_start_block;
    struct tagFlashChip *chip = PTT_flash_tag->chip;
    uint8_t *buf;
    bool_t block_ready,write_success;

    buf = chip->block_buf;
    PTT_start_block = PTT_flash_tag->start_block;
    fail_counter = 0;
    result_block = block_no;
    if(PTT_flash_tag->nand_ecc)
        block_ready = chip->query_block_ready_with_ecc(
                                    result_block+PTT_start_block,
                                    offset,buf+offset,size);
    else
        block_ready = chip->query_block_ready_no_ecc(
                                    result_block+PTT_start_block,
                                    offset,buf+offset,size);
    if(offset != 0)
        //��Ŀ�������ݶ����黺�����У��ȶ�offset��ǰ�Ĳ���
        chip->read_data_with_ecc(block_no+PTT_start_block,
                                 0,buf,offset);
    if(offset+size != chip->block_size)
        //�ٶ���Ч�����Ժ�Ĳ���
        chip->read_data_with_ecc(block_no+PTT_start_block,
                                 offset+size,buf+offset+size,
                                 chip->block_size-offset-size);

    //������д�뵽����ָ��飬
    chip->write_PCRB(PTT_flash_tag->PCRB_no+PTT_start_block,
                     block_no + PTT_start_block,buf);
    do  //��д����̿��ܲ������飬����Ҫ��ѭ����ֱ����ȷд����˳�
    {
        if((fail_counter == 0) && (block_ready)) //������������
        {
            write_result = chip->write_data_with_ecc(
                                result_block + PTT_start_block,
                                offset,buf+offset,size);
            if(write_result == size)
                write_success = true;
            else
                write_success = false;
        }else           //��Ҫ���������
        {
            chip->erase_block(result_block+PTT_start_block);//������
            //д��Ŀ��飬����д��
            write_result = chip->write_data_with_ecc(
                                result_block + PTT_start_block,
                                0,buf,chip->block_size);
            if(write_result == chip->block_size)
            {
                //ĥ���������
                __DFFSD_IncAbrasionTimes(PTT_flash_tag,result_block);
                write_success = true;
            }else
                write_success = false;
        }
        if(!write_success)   //д��ʧ�ܣ�����
        {
            fail_counter++;     //д��ʧ�ܴ�������
            //�Ǽǻ��鵽ĥ���
            __DFFSD_RegisterInvalid(PTT_flash_tag,result_block);
            //�����¿飬����ʹ�ü��׷��䷨�������������������
            result_block = __DFFSD_AllocateBlockSimple(PTT_flash_tag);
            //ע�⣬����ָ����Ŀ���������޸ģ���Ϊ��ʱMAT���Լ�������Ҫ�޸�
            //��FDT��û�иı䡣
            if(result_block == CN_LIMIT_UINT32) //����ʧ��(�޿��п�)
            {
                return CN_LIMIT_UINT32;         //���ش���
            }
        }else       //�ɹ�д��
        {
            break;
        }
    }while(fail_counter <= 10); //����������10������Ϊ�쳣�˳�����
    if(fail_counter > 10)           //��10�λ��������do-whileѭ��
        return CN_LIMIT_UINT32;
    else                            //�����Ѿ���ȷд��
        return result_block;
}

//----��дһ��:�������--------------------------------------------------------
//����: ��оƬ�黺����������д��flash��ĳ�飬��д���λ��ԭ��û����Ч���ݣ����
//      ��д��ʱ�������飬����һ���¿��滻֮�����ѻ���Ǽǵ�MAT����(������MATд
//      �뵽flash��)�������滻��Ŀ�š�д��ǰ���ȿ����Ƿ���Ҫ�����������Ҫ����
//      ��Ҫ���ÿ��Ƿ��Ѿ���busy[8]�������ǣ���Ҫ��"�ͷ�-��������-��д"�Ĺ��̽�
//      �п��滻����д��������ǿ黺������offset��ʼ��size���ֽڡ�
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      block_no��Ŀ����
//      offset����ʼ��ַ�ڿ��ڵ�ƫ����
//      size��д�뻺��������
//����: д��ɹ��򷵻�ʵ��д��Ŀ�ţ�
//      д��ʧ�ܷ���cn_limit_uint32���ռ䲻�������������10������ᵼ��ʧ��
//-----------------------------------------------------------------------------
uint32_t __DFFSD_WriteAppendBlock(struct tagStPTT_Flash *PTT_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size)
{
    uint32_t fail_counter;  //�����������10���������ȷд�룬�򷵻�
    uint32_t write_result,result_block,PTT_start_block;
    struct tagFlashChip *chip = PTT_flash_tag->chip;
    uint8_t *buf;
    bool_t block_ready,PCRB_writed=false,write_success;

    buf = chip->block_buf;
    PTT_start_block = PTT_flash_tag->start_block;
    fail_counter = 0;
    result_block = block_no;
    if(PTT_flash_tag->nand_ecc)
        block_ready = chip->query_block_ready_with_ecc(
                                    result_block+PTT_start_block,
                                    offset,buf+offset,size);
    else
        block_ready = chip->query_block_ready_no_ecc(
                                    result_block+PTT_start_block,
                                    offset,buf+offset,size);

    do  //��д����̿��ܲ������飬����Ҫ��ѭ����ֱ����ȷд����˳�
    {
        if((fail_counter == 0) && (block_ready))//�������ϺͲ���Ҫ����������д��
        {
            write_result = chip->write_data_with_ecc(
                                    result_block + PTT_start_block,
                                    offset,buf+offset,size);
            if(write_result == size)
                write_success = true;
            else
                write_success = false;
        }else
        {
            if(!PCRB_writed)
            {
                if(offset != 0)
                    //��Ŀ�������ݶ����黺�����У��ȶ�offset��ǰ�Ĳ���
                    chip->read_data_with_ecc(block_no+PTT_start_block,
                                             0,buf,offset);
                if(offset+size != chip->block_size)
                    //�ٶ���Ч�����Ժ�Ĳ���
                    chip->read_data_with_ecc(block_no+PTT_start_block,
                                             offset+size,buf+offset+size,
                                             chip->block_size-offset-size);
                //������д�뵽����ָ��飬
                chip->write_PCRB(PTT_flash_tag->PCRB_no+PTT_start_block,
                                 block_no + PTT_start_block,buf);
                PCRB_writed = true;
            }
            chip->erase_block(result_block+PTT_start_block);
            //д��Ŀ��飬����д��
            write_result = chip->write_data_with_ecc(
                                result_block + PTT_start_block,
                                0,buf,chip->block_size);
            if(write_result == chip->block_size)
            {
                //ĥ���������
                __DFFSD_IncAbrasionTimes(PTT_flash_tag,result_block);
                write_success = true;
            }else
                write_success = false;
        }
        if(!write_success)   //д��ʧ�ܣ�����
        {
            fail_counter++;     //д��ʧ�ܴ�������
            //�Ǽǻ��鵽ĥ���
            __DFFSD_RegisterInvalid(PTT_flash_tag,result_block);
            //�����¿飬����ʹ�ü��׷��䷨�������������������
            result_block = __DFFSD_AllocateBlockSimple(PTT_flash_tag);
            //ע�⣬����ָ����Ŀ���������޸ģ���Ϊ��ʱMAT���Լ�������Ҫ�޸�
            //��FDT��û�иı䡣
            if(result_block == CN_LIMIT_UINT32) //����ʧ��(�޿��п�)
            {
                return CN_LIMIT_UINT32;         //���ش���
            }
        }else       //�ɹ�д��
        {
            break;
        }
    }while(fail_counter <= 10); //����������10������Ϊ�쳣�˳�����
    if(fail_counter > 10)           //��10�λ��������do-whileѭ��
        return CN_LIMIT_UINT32;
    else                            //�����Ѿ���ȷд��
        return result_block;
}


//----������д���¿�-----------------------------------------------------------
//����: ��оƬ�Ŀ黺����������д�뵽һ��������Ч���ݵ��¿��У���д��ǰ���ڲ���
//      ��Ч���ݣ������趯�õ���ָ��飬�����д��ʱ�������飬����һ���¿��滻
//      ֮�����ѻ���Ǽǵ�MAT����(������MATд�뵽flash��)�������滻��Ŀ�š�
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      block_no��Ŀ����
//      offset����ʼ��ַ�ڿ��ڵ�ƫ����
//      size��д�뻺��������
//����: д��ɹ��򷵻�ʵ��д��Ŀ�ţ�
//      д��ʧ�ܷ���cn_limit_uint32���ռ䲻�������������10������ᵼ��ʧ��
//-----------------------------------------------------------------------------
uint32_t __DFFSD_WriteNewBlock(struct tagStPTT_Flash *PTT_flash_tag,
                             uint32_t block_no,uint32_t offset,uint32_t size)

{
    uint32_t fail_counter;  //�����������10���������ȷд�룬�򷵻�
    uint32_t write_result,result_block,PTT_start_block;
    struct tagFlashChip *chip = PTT_flash_tag->chip;

    fail_counter = 0;
    PTT_start_block = PTT_flash_tag->start_block;
    result_block = block_no;
    do  //��д����̿��ܲ������飬����Ҫ��ѭ����ֱ����ȷд����˳�
    {
        //�鿴�Ƿ���Ҫ����
        if(!chip->query_block_ready_with_ecc(
                                    result_block+PTT_start_block,
                                    offset,chip->block_buf+offset,size))
        {//��Ҫ����
            //����Ŀ���
            chip->erase_block(result_block + PTT_start_block);
            //������д��Ŀ�����
            write_result = chip->write_data_with_ecc(
                                result_block+PTT_start_block,
                                offset,chip->block_buf+offset,size);
        }else   //����Ҫ������������Ŀ�պ���һ���Ѿ��������Ŀ顣
        {
            //ֱ�Ӱ�����д�뵽Ŀ�����
            write_result = chip->write_data_with_ecc(
                                result_block + PTT_start_block,
                                offset,chip->block_buf+offset,size);
        }
        if(write_result != size)   //д��ʧ��
        {
            fail_counter++;     //д��ʧ�ܴ�������
            __DFFSD_RegisterInvalid(PTT_flash_tag,result_block);   //�Ǽǻ���
            //�����¿飬����ʹ�ü��׷��䷨�������������������
            result_block = __DFFSD_AllocateBlockSimple(PTT_flash_tag);
            if(result_block == CN_LIMIT_UINT32) //����ʧ��(�޿��п�)
            {
                return CN_LIMIT_UINT32;         //���ش���
            }
        }else                   //�ɹ�д��
        {
            //��Ȼ�ˣ���дһ�Σ�Ŀ����ĥ������ͼ�1
            //��ʹ�����������Ҳ���������һ��ĵ�һ��д�룬ĥ�������ȻҲҪ��1
            __DFFSD_IncAbrasionTimes(PTT_flash_tag,result_block);
            break;
        }
    }while(fail_counter <= 10);     //����������10������Ϊ�쳣�˳�����
    if(fail_counter > 10)           //��10�λ��������do-whileѭ��
        return CN_LIMIT_UINT32;
    else                            //�����Ѿ���ȷд��
        return result_block;
}

//----�ƶ��ļ���ָ��-----------------------------------------------------------
//����: ��һ���ļ��Ķ�ָ���ƶ���һ����λ��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp��Ŀ���ļ�ָ��
//      new_ptr���µĶ�ָ��
//����: true=�ɹ���false=�д���
//-----------------------------------------------------------------------------
bool_t __DFFSD_MoveFileReadPtr(struct tagStPTT_Flash *PTT_flash_tag,
                            FILE *fp,sint64_t new_ptr)
{
    if(new_ptr > fp->file_size)
        return false;

    switch (fp->open_mode)
    {
        case EN_R_RB:
        {
            fp->read_ptr = new_ptr;             //��λ��
        }break;
        case EN_W_WB:
        {
            return false;                       //��λ����Ч
        }break;
        case EN_A_AB:
        {
            return false;                       //��λ����Ч
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
            fp->read_ptr = new_ptr;             //��λ��
        }break;
        default: return false;
    }
    return true;
}

//----�ƶ��ļ�дָ��-----------------------------------------------------------
//����: ��һ���ļ���дָ���ƶ���һ����λ��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp��Ŀ���ļ�ָ��
//      new_ptr���µ�дָ��
//����: true=�ɹ���false=�д���
//-----------------------------------------------------------------------------
bool_t __DFFSD_MoveFileWritePtr(struct tagStPTT_Flash *PTT_flash_tag,
                             FILE *fp,sint64_t new_ptr)
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
        //��FDT�����޸��ļ�����
       __DFFSD_UpdateFDT_FileSize(PTT_flash_tag,fp,fp->write_ptr);
        fp->file_size = fp->write_ptr;  //��дָ�븲���ļ�����
    }
    return true;
}

//----д������д��flash--------------------------------------------------------
//����: ���ļ�д������������д��flash��
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp�����������ļ�ָ��
//���أ�ʵ��д��flash���ֽ���
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ���Ӷ��ļ�ʱ�����Ե�֧��,���Ӻ���__DFFSD_UpdateFileTime��
//   ����: ������
//-----------------------------------------------------------------------------
uint32_t __DFFSD_WriteBufToFlash(struct tagStPTT_Flash *PTT_flash_tag,
                                    struct tagFileRsc *fp)
{
    struct tagFlashChip *chip;
    struct tagFileAccess access_tag;
    sint64_t wr_point;
    uint32_t completed = 0,write_len;
    uint32_t offset_start;
    uint32_t block_no,start_block,end_block,loop;
    uint32_t true_block,last_block;
    uint32_t buffed_data; //�ļ�д�������е���������ʣ��������
    chip = PTT_flash_tag->chip;     //ȡ����(�ļ�)����оƬ

    if(fp->p_write_buf != NULL)
    {
        buffed_data = Ring_Check(fp->p_write_buf); //���д�������е�������
        if(buffed_data == 0)        //д������û������
            return 0;
    }else
        return 0;       //û��д������
    wr_point = fp->write_ptr-buffed_data;   //���㿪ʼд�����ݵ�λ��
    //������ʼдλ�ô����ļ��ڵڼ���
    start_block = (u32)(wr_point /chip->block_size);
    offset_start = wr_point % chip->block_size; //������ʼƫ��
    //������ֹ��дλ�ô����ļ��ڵڼ���
    end_block = (u32)((fp->write_ptr - 1)/chip->block_size);
    __DFFSD_GetAccessInfo(PTT_flash_tag,fp,&access_tag);
    //��λд���Ŀ�ţ������ڵ���Կ�š�
    block_no = access_tag.write_block_no;
    last_block = access_tag.last_block_no;
/*��������߼��о�������,����Ҫ������ʱ*/
    for(loop = start_block; loop <= end_block; loop++)
    {

        if(loop > start_block|| block_no == CN_LIMIT_UINT32)
        {//Ҫд��������ݣ������һ��.��һ�飬�Զ�������ˣ�����ģ��������Լ�
        //����
            //����һ���¿�
            block_no = __DFFSD_AllocateBlock(PTT_flash_tag);
            loop =0xfffffffe;//��Ϊ����׷�����ݵ�����
            if(block_no == CN_LIMIT_UINT32) //����ʧ��(�޿��п�)
                break;
        }
        //���㱾��д������ݳ���
        if(buffed_data - completed > chip->block_size - offset_start)
            write_len = chip->block_size - offset_start;
        else
            write_len = buffed_data - completed;
        Ring_Read(fp->p_write_buf,chip->block_buf+offset_start,write_len);
        if(loop == 0xfffffffe)    //׷������
        {
            if(offset_start == 0)   //��д��Ŀ���������Ч����
            {
                //д��Ŀ���
                true_block = __DFFSD_WriteNewBlock(PTT_flash_tag,block_no,
                                                        0,write_len);
                if(true_block != CN_LIMIT_UINT32)       //��ȷд��
                {
                    if(wr_point != 0)
                        __DFFSD_AddMAT_Item(PTT_flash_tag,last_block,true_block);
                }else       //д�����
                    break;
            }else                   //����׷�ӣ�����д��Ŀ��Ѿ��в�����Ч����
            {
                //д��Ŀ���
                true_block = __DFFSD_WriteAppendBlock(PTT_flash_tag,block_no,
                                                offset_start,write_len);
                if(true_block != CN_LIMIT_UINT32)
                {//��ȷд��
                    if(true_block != block_no)   //д��ʱ�����˿��滻
                        __DFFSD_UpdateMAT_Item(PTT_flash_tag,block_no,true_block);
                }else       //д�����
                    break;
            }
            last_block = true_block;
            access_tag.true_size += write_len;
            chip->erase_block(PTT_flash_tag->PCRB_no
                              + PTT_flash_tag->start_block);
        }else   //���ļ����м�д�룬�����������Ҫʹ��PCRB����б���
        {
            //д��Ŀ���
            true_block = __DFFSD_WriteUpdateBlock(PTT_flash_tag,block_no,
                                            offset_start,write_len);
            if(true_block != CN_LIMIT_UINT32)   //��ȷд��
            {
                if(true_block != block_no)   //д��ʱ�����˿��滻
                    __DFFSD_UpdateMAT_Item(PTT_flash_tag,block_no,true_block);
            }else       //д�����
                break;
            //ȷ����һѭ��д���Ŀ�ţ������ڵ���Կ�š�
            block_no = PTT_flash_tag->DDR_MAT[block_no].next;
            chip->erase_block(PTT_flash_tag->PCRB_no
                              + PTT_flash_tag->start_block);
        }
        completed += write_len;
        wr_point += write_len;
        offset_start = 0;   //�ڶ��鿪ʼ����ʼƫ�Ƶ�ַ�϶���0
    }
    __DFFSD_WriteDDR(PTT_flash_tag);//��DDR��ı仯д��flash��(DDR�����û�仯)
    __DFFSD_UpdateFileTime(fp);
    return completed;
}

//----������ǰͬ���ļ�������---------------------------------------------------
//����: ִ���ļ�д����֮ǰ�����ñ�����ͬ����������
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp��Ŀ���ļ�ָ��
//      read_len������ȡ����
//����: true=�ɹ���false=�д���
//�ر�˵��: struct file_rsc����home_left��ͨ����Ҳ�ǿ��Ժܿ��ҵ����ļ�������dbx
//          �ģ���home_left�����־������flash_driver�����ļ�ϵͳ�����֣�Ӧ�ý�
//          ֹ���������־��������Ҫ�ļ�ϵͳ�ṩPTT_flash_tagָ��
//-----------------------------------------------------------------------------
bool_t __DFFSD_SyncBufBeforeRead(struct tagStPTT_Flash *PTT_flash_tag,
                              FILE *fp,uint32_t read_len)
{
    switch(fp->open_mode)
    {
        case EN_R_RB:       //ֻ���ļ����������
        {
        }break;
        case EN_W_WB:       //ֻд�ļ����������
        {
        }break;
        case EN_A_AB:       //ֻд�ļ����������
        {
        }break;
        case EN_R_RB_PLUS:  //��дλ����ͬ����д�����������ݣ�����д��flash
        case EN_W_WB_PLUS:  //��en_r_rb_plus����ʽ��ͬ
        {
            __DFFSD_WriteBufToFlash(PTT_flash_tag,fp);
        }break;
        case EN_A_AB_PLUS:  //׷��д���ɶ��������λ����д�������н������д��
                            //����д��flash������ʲôҲ����
        {
            if(fp->read_ptr + read_len > fp->write_ptr)
                __DFFSD_WriteBufToFlash(PTT_flash_tag,fp);
        }break;
        default:break;
    }
    return true;
}


//----д��flash�ļ�------------------------------------------------------------
//���ܣ�������д���ļ���ִ�����в�����
//      1�����д���������㹻�Ŀռ䣬������д��д���������ɡ�
//      2�����д������û���㹻�Ŀռ䣬��ͬд������ԭ�����ݺ����뻺�����е�����
//         һ��д��flash
//      3��д����ɺ�����������Ƿ���Ҫͬ����ִ��֮��
//      4�����FDT���DDR���Ƿ���Ҫ�޸Ĳ�ִ��֮
//������buf�����뻺����
//      len������д���������
//      fp�����������ļ�ָ��
//���أ�����д�����������������д��������ԭ�е�����
//ע:   ��������û���ȵ���__DFFSD_write_buf_to_flash��������дbuf�е����ݣ�����
//      ������Ҫ��һ�ε���flashд������Ч�ʵͣ����ǰ�д��������������buf�е�����
//      �ϲ�һ��д��flash.
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ���Ӷ��ļ�ʱ�����Ե�֧��,���Ӻ���__DFFSD_UpdateFileTime��
//         todo:��δ���д��ʧ��?
//   ����: ������
//-----------------------------------------------------------------------------
uint32_t DFFSD_WriteFile(struct tagFileRsc *fp,uint8_t *buf,uint32_t len)
{
    struct tagFlashChip *chip;
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFileAccess access_tag;
    uint32_t completed = 0,write_len,write_sum;
    sint64_t wr_point;
    uint32_t offset_start;
    uint32_t block_no,start_block,end_block,loop,temp;
    uint32_t true_block,last_block;
    uint32_t buffed_data,spare_len; //�ļ�д�������е���������ʣ��������

    if((fp==NULL)||(len==0)||(buf==NULL))
        return 0;
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    //����ָ��
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;     //ȡ����(�ļ�)����оƬ
    buffed_data = Ring_Check(fp->p_write_buf); //��黺�����е�������
    if(Ring_Capacity(fp->p_write_buf) - buffed_data >= len)
    {//�����������㹻��������ֱ�Ӱ�����д�����������ɡ�
        completed = Ring_Write(fp->p_write_buf,(uint8_t*)buf,len);
        completed += buffed_data;
    }
    else
    {
        //���������û���㹻����������ѱ�����Ҫд���������ͬ������ԭ��������
        //һ��д��flash�����濪ʼִ��д�������
        //�������������е�������һ��һ��Ҫд��flash��������
        write_sum = len + buffed_data;
        spare_len = buffed_data; //������ʣ��������
        wr_point = fp->write_ptr - buffed_data;
        //������ʼдλ�ô����ļ��ڵڼ����ƫ����
        start_block = (u32)(wr_point /chip->block_size);
        offset_start = (u32)(wr_point % chip->block_size);
        //������ֹдλ�ô����ļ��ڵڼ���
        end_block = (u32)((fp->write_ptr + len -1)/chip->block_size);
        __DFFSD_GetAccessInfo(PTT_flash_tag,fp,&access_tag);
        //��λд���Ŀ�ţ������ڵ���Կ�š�
        block_no = access_tag.write_block_no;
        last_block = access_tag.last_block_no;
        for(loop = start_block; loop <= end_block; loop++)
        {
            if(((wr_point >= access_tag.true_size) && (wr_point != 0)))
            {
                //����һ���¿�
                block_no = __DFFSD_AllocateBlock(PTT_flash_tag);
                if(block_no == CN_LIMIT_UINT32) //����ʧ��(�޿��п�)
                    break;
            }
            //���㱾��д������ݳ���
            if(write_sum - completed > chip->block_size - offset_start)
                write_len = chip->block_size - offset_start;
            else
                write_len = write_sum - completed;
            if(spare_len != 0)  //д�������ﻹ������
            {
                temp = Ring_Read(fp->p_write_buf,
                                 chip->block_buf+offset_start,write_len);
                spare_len -= temp;  //д��������ȥʵ�ʶ�����������
                //ʵ�ʶ�������������write_len�٣�˵�����ζ��Ѿ���д���������ա�
                if(temp != write_len)
                {
                    //���������뻺�����п�������
                    memcpy(chip->block_buf+temp+offset_start,
                                buf,write_len-temp);
                }
            }
            else   //д�������Ѿ��գ������뻺������������
                memcpy(chip->block_buf+offset_start,
                                       buf+completed-buffed_data,write_len);
            if(wr_point >= access_tag.true_size)    //׷������
            {
                if(offset_start == 0)   //��д��Ŀ���������Ч����
                {
                    //д��Ŀ���
                    true_block = __DFFSD_WriteNewBlock(PTT_flash_tag,block_no,
                                                            0,write_len);
                    if(true_block != CN_LIMIT_UINT32)
                    {//��ȷд��
                        if(wr_point != 0)
                            __DFFSD_AddMAT_Item(PTT_flash_tag,last_block,true_block);
                    }
                    else //д�����
                        break;
                }
                else //����׷�ӣ�����д��Ŀ��Ѿ��в�����Ч����
                {
                    //д��Ŀ���
                    true_block = __DFFSD_WriteAppendBlock(PTT_flash_tag,block_no,
                                                    offset_start,write_len);
                    if(true_block != CN_LIMIT_UINT32)
                    {//��ȷд��
                        if(true_block != block_no)   //д��ʱ�����˿��滻
                            __DFFSD_UpdateMAT_Item(PTT_flash_tag,block_no,true_block);
                    }
                    else //д�����
                        break;
                }
                last_block = true_block;
                access_tag.true_size += write_len;
                wr_point += write_len;
                chip->erase_block(PTT_flash_tag->PCRB_no
                                  + PTT_flash_tag->start_block);
            }
            else //���ļ����м�д�룬�����������Ҫʹ��PCRB����б���
            {
                //д��Ŀ���
                true_block = __DFFSD_WriteUpdateBlock(PTT_flash_tag,block_no,
                                                offset_start,write_len);
                if(true_block != CN_LIMIT_UINT32)
                {//��ȷд��
                    if(true_block != block_no)   //д��ʱ�����˿��滻
                        __DFFSD_UpdateMAT_Item(PTT_flash_tag,block_no,true_block);
                }else       //д�����
                    break;
                //ȷ����һѭ��д���Ŀ�ţ������ڵ���Կ�š�
                block_no = PTT_flash_tag->DDR_MAT[block_no].next;
                wr_point += write_len;
                if(wr_point > access_tag.true_size)
                    access_tag.true_size = wr_point;
                chip->erase_block(PTT_flash_tag->PCRB_no
                                  + PTT_flash_tag->start_block);
            }
            completed += write_len;
            offset_start = 0;   //�ڶ��鿪ʼ����ʼƫ�Ƶ�ַ�϶���0
        }
        __DFFSD_UpdateFileTime(fp);

    }
    if((completed - buffed_data) > 0) //д��������
    {
        __DFFSD_SyncBufAfterWrite(PTT_flash_tag,fp,completed-buffed_data);
        __DFFSD_MoveFileWritePtr(PTT_flash_tag,fp,
                                    fp->write_ptr + completed-buffed_data);
    }
    __DFFSD_WriteDDR(PTT_flash_tag);//���DDR���б仯��д��flash��
    return completed - buffed_data;
}
//-----------------------------------------------------------------------------
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
uint32_t DFFSD_ReadFile(struct tagFileRsc *fp,uint8_t *buf,uint32_t len)
{
    struct tagFlashChip *chip;
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    uint32_t completed = 0,read_len,result_len;
    uint32_t offset_start;
    uint32_t block_no,start_block,end_block,loop;
    uint32_t read_result;
    if((fp==NULL)||(len==0)||(buf==NULL))
        return 0;
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    //����ָ��
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    __DFFSD_SyncBufBeforeRead(PTT_flash_tag,fp,len);
    chip = PTT_flash_tag->chip;     //ȡ����(�ļ�)����оƬ
    //������ʼ��дλ�ô����ļ��ڵڼ���
    start_block = (u32)(fp->read_ptr/chip->block_size);
    //������ֹ��дλ�ô����ļ��ڵڼ���
    end_block = (u32)((fp->read_ptr + len -1)/chip->block_size);
    offset_start = fp->read_ptr % chip->block_size;
    if(fp->read_ptr + len >fp->file_size)
        result_len = (u32)(fp->file_size - fp->read_ptr);
    else
        result_len = len;
    //��λ������Ŀ�ţ������ڵ���Կ�š�
    block_no = __DFFSD_LocateBlock(PTT_flash_tag,fp,fp->read_ptr);
    for(loop = start_block; loop <= end_block; loop++)
    {
        if(result_len - completed > chip->block_size - offset_start)
            read_len = chip->block_size - offset_start;
        else
            read_len = result_len - completed;
        //����Ŀ���
        read_result =chip->read_data_with_ecc(
                                block_no + PTT_flash_tag->start_block,
                                offset_start,buf+completed,read_len);
        if(read_result == 2)    //�������󣬻���
        {
            break;
        }
        completed += read_len;
        offset_start = 0;   //�ڶ��鿪ʼ����ʼƫ�Ƶ�ַ�϶���0
        block_no = PTT_flash_tag->DDR_MAT[block_no].next;
    }
    fp->read_ptr += completed;
    return completed;
}

//----ˢд������---------------------------------------------------------------
//����: ���ļ�д������������д��flash
//������fp�����������ļ�ָ��
//���أ�ʵ��д��flash���ֽ���
//-----------------------------------------------------------------------------
uint32_t DFFSD_FlushFile(struct tagFileRsc *fp)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    if(fp==NULL)
        return 0;
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    //����ָ��
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    return __DFFSD_WriteBufToFlash(PTT_flash_tag,fp);
}

//----��ѯ�ļ��ɶ�������-------------------------------------------------------
//���ܣ���ѯһ���ļ����ж��ٿɶ����ݣ����ڹ�̬�ļ�����flash�ļ�����˵�������ļ�
//      ����-��ǰָ�룬�����ı�׼���岢��һ����ˣ�����ͨ�Ŷ��ӡ����ң��ļ���д
//      ָ����α�ʾ������ļ�ϵͳ������ʵ���йأ��ļ�ϵͳ�ĺ��ĳ���Ӧ�����κ�
//      ���衣
//������fp�����������ļ�ָ��
//���أ��ļ��ɶ����ݵ��ֽ���
//-----------------------------------------------------------------------------
sint64_t DFFSD_QueryFileStocks(struct tagFileRsc *fp)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFdtInfo FDT_item;

    if(fp == NULL)
        return 0;
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,
                          (uint32_t)fp->file_medium_tag,&FDT_item);
    if(FDT_item.mattr & CN_FS_ATTR_DIRECTORY)
        return 0;       //Ŀ¼���ܶ�
    return fp->file_size - fp->read_ptr;
}


//----��ѯ�ļ���д������-------------------------------------------------------
//���ܣ���ѯһ���ļ�����д��������ݣ����ڹ�̬�ļ�����flash�ļ��������ܾ����ļ�
//      ���ʣ��ռ䣬�����ı�׼���岢��һ����ˣ�����ͨ�Ŷ��ӡ����ң�����ʣ��
//      �ռ���α�ʾ������ļ�ϵͳ������ʵ���йأ��ļ�ϵͳ�ĺ��ĳ���Ӧ�����κ�
//      ���衣������ΪPTT_flash_tag->free_sum�����ļ��Կ�д�����������
//      flash�ļ�ϵͳ�ļ��㷽ʽ��:
//      1���ļ��ѷ���Ŀ������-�ļ��ߴ硣
//      2�����Ϸ���ʣ��ռ�PTT_flash_tag->free_sum��
//      3����ȥ�ļ������������е�������
//������fp�����������ļ�ָ��
//���أ��ļ�����д����ֽ���
//-----------------------------------------------------------------------------
sint64_t DFFSD_QueryFileCubage(struct tagFileRsc *fp)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFlashChip *chip;
    struct tagFdtInfo FDT_item;
    if(fp == NULL)
        return 0;
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,
                          (uint32_t)fp->file_medium_tag,&FDT_item);
    if((FDT_item.mattr & CN_FS_ATTR_READ_ONLY)
                || (FDT_item.mattr & CN_FS_ATTR_DIRECTORY))
        return 0;       //ֻ���ļ���Ŀ¼����д��
/*
    //ȡ�ļ��ߴ�
    size = pick_little_32bit(FDT_item.file_size,0)
            +((uint64_t)pick_little_32bit(FDT_item.file_size,1)<<32);
    if(size == 0)   //�����ļ�����ռ��һ���ԭ��0�ߴ��ļ�Ҳ�Ѿ�������һ��
        size = chip->block_size;
    else
    {
        //�ļ�����ռ�������飬��������Ѿ����䵫δʹ�õĿռ�
        size = chip->block_size - size % chip->block_size;
        if(size == chip->block_size)
            size = 0;
    }
    buffed_size = Ring_Check(fp->p_write_buf);
*/      //����Ϊʲô�ᱻע�͵�������©��ע�ͣ��ȱ�ɾ�ɡ�

    return PTT_flash_tag->free_sum * chip->block_size;  // - buffed_size + size;
}

//----�����ļ�����-------------------------------------------------------------
//���ܣ������ļ����ȣ�����ʵ�ʳ��Ƚ���β������ʵ�ʳ��Ƚ�׷�ӿռ䡣����������Ҫ
//      ������������ʽд������֮ǰ��Ԥ�ȷ���洢����
//������fp�����������ļ�ָ��
//      new_size���µ��ļ����ȡ�
//���أ��µ��ļ����ȣ�����new_size�������ȣ�һ������Ϊ����û���㹻����������
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ���Ӷ��ļ�ʱ�����Ե�֧��,���Ӻ���__DFFSD_UpdateFileTime��
//   ����: ������
//-----------------------------------------------------------------------------
sint64_t DFFSD_SetFileSize(struct tagFileRsc *fp,s64 new_size)
{
    struct tagPTTDevice *PTT_device_tag;
    sint64_t result,temp;
    uint32_t origin_blocks,new_blocks,last_block_no,next_block_no;
    uint32_t loop;
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFlashChip *chip;
    struct tagFdtInfo FDT_item;
    //����Ŀ¼��FDT_item
    if(fp == NULL)
        return 0;
    if(new_size == fp->file_size)
        return new_size;    //�µ��ļ�������ԭ������ȣ�ֱ�ӷ��ء�
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,
                          (uint32_t)fp->file_medium_tag,&FDT_item);
    if((FDT_item.mattr & CN_FS_ATTR_READ_ONLY)
                || (FDT_item.mattr & CN_FS_ATTR_DIRECTORY))
        return 0;       //ֻ���ļ���Ŀ¼����ִ�����ó��ȵĲ���
    //��������if����ȷ��ԭ�ļ�ռ�õĿ��������ļ���������Ŀ���
    if(fp->file_size == 0)
        origin_blocks = 1;
    else
        origin_blocks = (u32)((fp->file_size + chip->block_size -1)/chip->block_size);
    if(new_size < 0)
        new_size = 0;
    if(new_size == 0)
        new_blocks = 1;
    else
        new_blocks = (u32)((new_size + chip->block_size -1)/chip->block_size);
    last_block_no = __DFFSD_LastBlock(PTT_flash_tag,fp);
    if(origin_blocks < new_blocks)     //�ļ�ռ�ÿ���������
    {
        for(loop = origin_blocks; loop < new_blocks; loop++)
        {
            next_block_no = __DFFSD_AllocateBlock(PTT_flash_tag);
            if(next_block_no != CN_LIMIT_UINT32)
            {
                __DFFSD_AddMAT_Item(PTT_flash_tag,last_block_no,next_block_no);
                last_block_no = next_block_no;
            }else
                break;
        }
        if(loop == new_blocks)
            result = new_size;
        else
            result = (sint64_t)loop * chip->block_size;
    }
    else if(origin_blocks > new_blocks)     //�ļ�ռ�ÿ��������
    {
        result = new_size;
        for(loop = origin_blocks; loop > new_blocks; loop--)
        {
            next_block_no = last_block_no;
            last_block_no = PTT_flash_tag->DDR_MAT[next_block_no].previous;
            __DFFSD_FreeBlock(PTT_flash_tag,next_block_no);
        }
        //���һ������ָ��ָ���Լ�
        PTT_flash_tag->DDR_MAT[last_block_no].next = last_block_no;
    }
    else //ռ�ÿ�������
        result = new_size;

    fp->file_size = result;
    //���´����д�������������������дָ�룬���ѷǷ�ָ��(���ļ����̶������ļ�
    //���ȵ�ָ��λ��)�Ƶ��Ϸ�λ�á�
    if(fp->p_read_buf != NULL)      //��������ָ��ǿգ�˵���ж�������
    {
        if(fp->read_ptr > result)   //��ָ�����ļ��ߴ����
        {
            Ring_Flush(fp->p_read_buf); //��ն�������
            fp->read_ptr = result;      //��ָ��ص��ļ�ĩ
        }
        else
        {
            temp = fp->read_ptr + Ring_Check(fp->p_read_buf);
            //��ָ��δ�����ļ����ȵ��������������ݳ����ļ����ȣ����������ֿ���
            if(temp > result)
            {
                Ring_SkipTail(fp->p_read_buf,(u32)(temp-result));
            }
        }
    }
    if(fp->p_write_buf != NULL)     //д������ָ��ǿգ�˵����д������
    {
        if(fp->write_ptr > result)  //дָ�볬���ļ�����
        {
            temp = result - fp->write_ptr;  //���㳬������
            Ring_SkipTail(fp->p_write_buf,(u32)temp);   //���������ļ����ȵĲ��ҿ���
                                                    //���ڻ������е����ݡ�
            fp->write_ptr = result; //дָ��ص��ļ�ĩ
        }
        else
        {
            //дָ��û�г����ļ����ȣ����账��
        }
    }
    __DFFSD_UpdateFDT_FileSize(PTT_flash_tag,fp,result);
    __DFFSD_WriteDDR(PTT_flash_tag); //�����޸�DDR,д��flash
    __DFFSD_UpdateFileTime(fp);
    return result;
}

//----�����ļ�ָ��-------------------------------------------------------------
//���ܣ����ļ�ָ�����õ�ָ����λ�ã�����Ӧ��ͬ����д��������
//������fp�����������ļ�ָ��
//      position�ĳ�Ա:
//          offset���ƶ���λ��
//          whence���Ӻδ���ʼ�����ƶ����ȣ��μ�SEEK_CUR�ȵĶ���
//���أ�0=�ɹ���1=ʧ��
//-----------------------------------------------------------------------------
uint32_t DFFSD_SeekFile(struct tagFileRsc *fp,struct tagSeekPara *position)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    sint64_t new_position,temp;
    uint32_t buffed_size;
    uint8_t  whence;
    if((position == NULL) ||(fp == NULL))
        return 1;
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    whence = position->whence;
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
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
            temp = new_position - fp->read_ptr;
            buffed_size = Ring_Check(fp->p_read_buf);
            if((temp > 0) && (temp < buffed_size))
                //���������ƶ����Ȼ������е�������
                Ring_DumbRead(fp->p_read_buf,(ptu32_t)temp);
            else
                Ring_Flush(fp->p_read_buf); //��ǰ�ƻ����ƶ������ڻ�������������
            if(new_position > fp->file_size)
                fp->read_ptr = fp->file_size;
            else if(new_position < 0)
                fp->read_ptr = 0;
            else
                fp->read_ptr = new_position;
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
                return 1;
            __DFFSD_WriteBufToFlash(PTT_flash_tag,fp);
            if(new_position > fp->file_size)
            {
                //�ӳ��ļ����������洢����������������ӵĴ洢��
                if(DFFSD_SetFileSize(fp,new_position) != new_position)
                    return 1;
                else
                {
                    fp->write_ptr = new_position;   //����дָ��
                }
            }else if(new_position < 0)
                fp->write_ptr = 0;
            else
                fp->write_ptr = new_position;
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
            __DFFSD_WriteBufToFlash(PTT_flash_tag,fp);//д���������ݱ���д��

            if(new_position > fp->file_size)    //��λ�ó����ļ�����
            {
                //�ӳ��ļ����������洢����������ʼ�������ӵĴ洢��
                if(DFFSD_SetFileSize(fp,new_position) != new_position)
                    return 1;
                else
                {
                    fp->write_ptr = new_position;   //����дָ��
                    fp->read_ptr = new_position;    //���ö�ָ��
                }
            }else if(new_position < 0)  //��λ��С��0����Ϊ0
            {
                fp->write_ptr = 0;      //д�������Ѿ�д�룬����дָ�뼴��
                fp->read_ptr = 0;    //���ö�ָ�룬��ָ��Ͷ����������Զ�������
                Ring_Flush(fp->p_read_buf); //��ն�������
            }
            else
            {
                fp->write_ptr = new_position;//д�������Ѿ�д�룬����дָ�뼴��
                fp->read_ptr = new_position;    //���ö�ָ��
                if(new_position < fp->read_ptr) //��λ��ʹ��ָ�����(���ļ���)
                {
                    Ring_Flush(fp->p_read_buf); //������������ȫ������
                }else
                {
                    //���㻺���������������ļ���ʲôλ��
                    temp = fp->read_ptr + Ring_Check(fp->p_read_buf);
                    if(new_position < temp)
                    {//��ָ��ǰ�ƣ���δ���������������ǵķ�Χ��
                        //�����������ݲ�������
                        Ring_DumbRead(fp->p_read_buf,(u32)(temp-new_position));
                    }else
                    {//��ָ��ǰ�ƣ������������������ǵķ�Χ��
                        Ring_Flush(fp->p_read_buf);     //������������ȫ������
                    }
                }
            }
        }break;
        default: return 1;
    }
    return 0;
}

//----���ļ������ļ�������-----------------------------------------------------
//����: �����½��ļ��Ĵ�ģʽ��Ϊ�ļ����仺������
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp�����ļ�ϵͳ�ṩ���ļ���ָ��
//      mode���ļ�����ģʽ
//����: true = ��ȣ�false=�����
//----------------------------------------------------------------------------
bool_t __DFFSD_allocate_file_buf_create(struct tagStPTT_Flash *PTT_flash_tag,
                              struct tagFileRsc *fp,
                              enum _FILE_OPEN_MODE_ mode)
{
    struct tagFlashChip *chip;
    uint32_t read_buf_len;
    uint32_t write_buf_len;
    uint8_t *buf;
    uint8_t *buf1;
    chip = PTT_flash_tag->chip;
    read_buf_len = chip->PTT_read_buf_size;
    write_buf_len = chip->PTT_write_buf_size;
    if(fp->file_attr.bits.folder)     //FDT��Ŀ��һ��Ŀ¼
    {
        Ring_Init(&fp->read_rw_buf,NULL,0);     //Ŀ¼�Ķ���������Ϊ��
        Ring_Init(&fp->write_buf,NULL,0);       //Ŀ¼��д��������Ϊ��
        fp->p_read_buf = NULL;
        fp->p_write_buf = NULL;
        fp->read_ptr = 0;
        fp->write_ptr = 0;
        fp->file_size = 0;
    }else       //��һ���ļ�����Ҫ����mode�����д������
    {
        switch(mode)
        {
            case EN_R_RB:
            {
                buf =(uint8_t*)M_MallocLcHeap(read_buf_len,chip->ChipHeap,0);//��ģʽֻ�ж�������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                Ring_Init(&fp->read_rw_buf,buf,read_buf_len);   //��ʼ����������
                Ring_Init(&fp->write_buf,NULL,0);           //��ʼ��д������Ϊ��
                fp->p_read_buf = &fp->read_rw_buf;  //��������ָ�븳ֵ
                fp->p_write_buf = NULL;             //д������ָ���ÿ�
                fp->read_ptr = 0;                   //��λ����0
                fp->write_ptr = 0;                  //дλ����0
            }break;
            case EN_W_WB:
            case EN_A_AB:
            {
                buf =(uint8_t*)M_MallocLcHeap(write_buf_len,chip->ChipHeap,0);//��ģʽֻ��д������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                Ring_Init(&fp->read_rw_buf,NULL,0);         //��ʼ����������Ϊ��
                Ring_Init(&fp->write_buf,buf,write_buf_len);//��ʼ��д������
                fp->p_read_buf = NULL;              //��������ָ���ÿ�
                fp->p_write_buf = &fp->write_buf;   //д������ָ�븳ֵ
                fp->read_ptr = 0;                   //��λ����0
                fp->write_ptr = 0;                  //дλ����0
            }break;
            case EN_R_RB_PLUS:
            case EN_W_WB_PLUS:
            case EN_A_AB_PLUS:
            {
                buf = (uint8_t*)M_MallocLcHeap(read_buf_len,chip->ChipHeap,0);      //�����������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                buf1 = (uint8_t*)M_MallocLcHeap(write_buf_len,chip->ChipHeap,0);    //����д������
                if(buf1 == NULL)
                {

                    free(buf);
                    return false;           //����������ʧ�ܣ����ش���
                }
                Ring_Init(&fp->read_rw_buf,buf,read_buf_len);   //��ʼ����������
                Ring_Init(&fp->write_buf,buf1,write_buf_len);   //��ʼ��д������
                fp->p_read_buf = &fp->read_rw_buf;      //���ö�������ָ��
                fp->p_write_buf = &fp->write_buf;       //����д������ָ��
                fp->read_ptr = 0;                       //��λ����0
                fp->write_ptr = 0;                      //дλ����0
            }break;
            default:break;
        }
    }
    return true;
}

//----������Ŀ���ļ�/Ŀ¼��----------------------------------------------------
//���ܣ������ļ�(Ŀ¼)��attr��������������Ŀ¼�����ļ�����������ļ�����ͬʱ����
//      1��洢����ɾ���ļ�����ʹ�ļ�����Ϊ0ʱ��Ҳ��������һ���ͷš���֮���κ�
//      ����£��ļ�����ռ��1��洢��.
//      ���ļ�/Ŀ¼��������ͬʱ�������´򿪵��ļ��������result�С�
//������name��Ŀ¼���ļ�����
//      parent����Ŀ¼ָ�롣
//      attr�����ļ���Ŀ¼��������
//���أ�true=�ɹ���false=ʧ�ܣ�һ������Ϊ����û���㹻����������
//��ע��������������������ɵ����߱�֤����Ϊ�洢���ʣ�ר�ش洢���ݵ�ְ���Ƿ�
//      ������������һ������Ρ�
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ���Ӷ��ļ�ʱ������֧��
//   ����: ������
//-----------------------------------------------------------------------------
bool_t DFFSD_CreateItem(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result,union file_attrs attr,
                         enum _FILE_OPEN_MODE_ mode)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagFlashChip *chip;
    uint32_t FDT_item_no_parent,FDT_item_no_eldest,FDT_item_no_least;
    uint32_t FDT_item_no_me,block_me,temp;
    struct tagFdtInfo FDT_item_me,FDT_item_parent,FDT_item_eldest,FDT_item_least;
    struct tagStPTT_Flash *PTT_flash_tag;
    if(parent == NULL)
        return false;
    PTT_device_tag =(struct tagPTTDevice *)(parent->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;
    FDT_item_no_parent = (uint32_t)parent->file_medium_tag;
    //����Ŀ¼��FDT��Ŀ
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no_parent,&FDT_item_parent);
    if(!(FDT_item_parent.mattr & CN_FS_ATTR_DIRECTORY))
        return false; //parent����һ��Ŀ¼�������ļ�
    FDT_item_no_me = __DFFSD_AllocateFDTItem(PTT_flash_tag);//����һ��FDT��Ŀ
    //debug Ҫ��Ҫ�Է����FDT���м�⣬�п����Ѿ���ʹ���ˡ������ʹ���ˣ�˵��
    //FDT�����ˣ���������ˡ����Բ����÷���ĳ������⡣��ͱ����ڴ�����ʱ��С��
    //��С�ģ�ǧ���ܽ����ɹ��ˣ���û�дӿ�������Ū�á�
    if(FDT_item_no_me == CN_LIMIT_UINT32)   //���䲻�ɹ�
        return false;
    if(FDT_item_no_parent == 0)
    {
        //��Ŀ¼�Ǹ�Ŀ¼������ʱFDT��Ŀʱ���޸ĸ�Ŀ¼��Ŀ�������ض�
        __DFFSD_ReadFDT_Item(PTT_flash_tag,0,&FDT_item_parent);
    }
    if( ! attr.bits.folder)//������item���ļ�
    {
        block_me = __DFFSD_AllocateBlock(PTT_flash_tag);
        if(block_me == CN_LIMIT_UINT32)//�洢������ʧ��
        {
            __DFFSD_FreeFDTItem(PTT_flash_tag,FDT_item_no_me);
            return false;
        }

        fill_little_32bit(FDT_item_me.fstart_dson, 0, block_me);
        PTT_flash_tag->DDR_MAT[block_me].attr = cn_file_block_start;
        PTT_flash_tag->DDR_MAT[block_me].previous = FDT_item_no_me;
        PTT_flash_tag->DDR_MAT[block_me].next = block_me;
        //����last_block_no��MAT����λ�ô���DDR�����ǵڼ���
        temp = __DFFSD_MAT_Start(PTT_flash_tag);
        temp = (temp + block_me*4*2) / chip->block_size;
        //DDR_bak�б�Ǹÿ�Ϊδд��
        PTT_flash_tag->writed_DDR_bak[temp] = false;
        //DDR_main�б�Ǹÿ�Ϊδд��
        PTT_flash_tag->writed_DDR_main[temp] = false;

    }
    else
    {
        block_me = CN_LIMIT_UINT32;
        fill_little_32bit(FDT_item_me.fstart_dson,0,CN_LIMIT_UINT32);
    }
    __DFFSD_AsmFDT_Item(&FDT_item_me,attr,name);
    fill_little_32bit(FDT_item_me.parent,0,FDT_item_no_parent);
    FDT_item_no_eldest = pick_little_32bit(FDT_item_parent.fstart_dson,0);
    if(FDT_item_no_eldest == CN_LIMIT_UINT32)   //��Ŀ¼�ǿ�Ŀ¼
    {
        //�����½�Ŀ¼��
        fill_little_32bit(FDT_item_me.previous,0,FDT_item_no_me);
        fill_little_32bit(FDT_item_me.next,0,FDT_item_no_me);
        //�½�Ŀ¼��д��flash
        __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_me,&FDT_item_me);
        //��Ŀ¼�������ָ���½�Ŀ¼��
        fill_little_32bit(FDT_item_parent.fstart_dson,0,FDT_item_no_me);
    }
    else //��Ŀ¼��������(����Ŀ¼���ļ�)
    {
        __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no_eldest,&FDT_item_eldest);
        FDT_item_no_least = pick_little_32bit(FDT_item_eldest.previous,0);
        //�����½�Ŀ¼��
        fill_little_32bit(FDT_item_me.previous,0,FDT_item_no_least);
        fill_little_32bit(FDT_item_me.next,0,FDT_item_no_eldest);
        //�½�Ŀ¼��д��flash
        __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_me,&FDT_item_me);
        if(FDT_item_no_least == FDT_item_no_eldest) //��Ŀ¼��ֻ��һ������
        {
            //�������ǰ��ָ���ָ���½�Ŀ¼��
            fill_little_32bit(FDT_item_eldest.previous,0,FDT_item_no_me);
            fill_little_32bit(FDT_item_eldest.next,0,FDT_item_no_me);
            //������д��flash
            __DFFSD_WriteFDT_Item(PTT_flash_tag,
                                   FDT_item_no_eldest,&FDT_item_eldest);
        }else       //��Ŀ¼���ж������
        {
            //��һ�������ǰָ��ָ���½�Ŀ¼��
            fill_little_32bit(FDT_item_eldest.previous,0,FDT_item_no_me);
            //��һ������д��flash
            __DFFSD_WriteFDT_Item(PTT_flash_tag,
                                   FDT_item_no_eldest,&FDT_item_eldest);
            //�������һ������
            __DFFSD_ReadFDT_Item(PTT_flash_tag,
                                  FDT_item_no_least,&FDT_item_least);
            //�½�����ĺ�ָ��ָ���½�Ŀ¼��
            fill_little_32bit(FDT_item_least.next,0,FDT_item_no_me);
            //���һ������д��flash
            __DFFSD_WriteFDT_Item(PTT_flash_tag,
                                   FDT_item_no_least,&FDT_item_least);
        }
    }
    //�����ϲ�Ŀ¼��ʱ������
    __DFFSD_UpdateItemTime(&FDT_item_parent);
    __DFFSD_WriteFDT_Item(PTT_flash_tag, FDT_item_no_parent, &FDT_item_parent);
    __DFFSD_WriteDDR(PTT_flash_tag);
    //����item��Ӧ���ļ�����ʱ��
    result->CreateTime = FDT_item_me.CreateTime;
    result->ModTime = FDT_item_me.ModTime;
    parent->CreateTime = FDT_item_parent.CreateTime;
    parent->ModTime = FDT_item_parent.ModTime;
    result->file_attr = attr;

    //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
    if(strlen(name) <= CN_FLASH_FILE_NAME_LIMIT)//��ȡ��������
        strcpy(result->name,name);
    else
    {
        memcpy(result->name,name,CN_FLASH_FILE_NAME_LIMIT);
        result->name[CN_FLASH_FILE_NAME_LIMIT] = '\0';
    }
    result->file_size = 0;
    result->file_medium_tag = (ptu32_t)FDT_item_no_me;
    result->file_semp = NULL;
    if(__DFFSD_allocate_file_buf_create(PTT_flash_tag,result,mode))//�����ļ�������
    {
        return true;
    }else
        return false;
}

//----�ƶ��ļ�-----------------------------------------------------------------
//���ܣ���һ���ļ���һ��Ŀ¼�ƶ�����һ��Ŀ¼
//������name��Ŀ¼���ļ�����
//      parent����Ŀ¼ָ�롣
//      attr�����ļ���Ŀ¼��������
//���أ�true=�ɹ���false=ʧ�ܣ�
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ���Ӷ��ļ�ʱ�����Ե�֧��(δ���)
//   ����: ������
//-----------------------------------------------------------------------------
bool_t DFFSD_MoveFile(struct tagFileRsc *src_fp, struct tagFileRsc *dest_fp)
{
    struct tagPTTDevice *PTT_device_tag;
    uint32_t src_FDT_item_no,dest_FDT_item_no,dest_FDT_item_no_parent;
    uint32_t FDT_item_no_previous,FDT_item_no_next;
    struct tagFdtInfo src_FDT_item,dest_FDT_item;
    struct tagFdtInfo FDT_item_previous,FDT_item_next;
    struct tagStPTT_Flash *PTT_flash_tag;

    if(dest_fp == NULL || src_fp == NULL)
        return false;
    PTT_device_tag =(struct tagPTTDevice *)(src_fp->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;

    src_FDT_item_no = (uint32_t)src_fp->file_medium_tag;
    dest_FDT_item_no = (uint32_t)dest_fp->file_medium_tag;
    //����Ŀ¼��FDT��Ŀ
    __DFFSD_ReadFDT_Item(PTT_flash_tag,src_FDT_item_no,&src_FDT_item);
    __DFFSD_ReadFDT_Item(PTT_flash_tag,dest_FDT_item_no,&dest_FDT_item);
    if((src_FDT_item.mattr & CN_FS_ATTR_DIRECTORY)||
                                (dest_FDT_item.mattr & CN_FS_ATTR_DIRECTORY))
        return false; //��Ŀ¼

    dest_FDT_item_no_parent = pick_little_32bit(dest_FDT_item.parent,0);
    fill_little_32bit(src_FDT_item.parent,0,dest_FDT_item_no_parent);
    __DFFSD_WriteFDT_Item(PTT_flash_tag,src_FDT_item_no,&src_FDT_item);



    FDT_item_no_previous = pick_little_32bit(dest_FDT_item.previous,0);
     FDT_item_no_next = pick_little_32bit(dest_FDT_item.next,0);
    //���ƶ���ֻ��һ��
    if((FDT_item_no_previous == src_FDT_item_no)&&(FDT_item_no_next == src_FDT_item_no))
    {
        __DFFSD_FreeFDTItem(PTT_flash_tag,dest_FDT_item_no);
                //�����½�Ŀ¼��
        fill_little_32bit(src_FDT_item.previous,0,src_FDT_item_no);
        fill_little_32bit(src_FDT_item.next,0,src_FDT_item_no);
        //�½�Ŀ¼��д��flash
        __DFFSD_WriteFDT_Item(PTT_flash_tag,src_FDT_item_no,&src_FDT_item);
        //ֻ��һ�ûʲô�ô���
    }
    else
    {
        if((FDT_item_no_previous != src_FDT_item_no)&&(FDT_item_no_previous==FDT_item_no_next))
        {//���ƶ��ģ���һ���ֵ�Ŀ¼���ļ���
            //������һ���ǰָ��ͺ�ָ�룬��ָ���Լ���FDT_item_no_previous��FDT_item_no_next
            //��ͬһ�����һ��
             __DFFSD_ReadFDT_Item(PTT_flash_tag,
                                  FDT_item_no_previous,&FDT_item_previous);
            fill_little_32bit(FDT_item_previous.previous,0,FDT_item_no_previous);
            fill_little_32bit(FDT_item_previous.next,0,FDT_item_no_previous);
           __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_previous,&FDT_item_previous);
        }
        else
        {
            __DFFSD_ReadFDT_Item(PTT_flash_tag,
                                  FDT_item_no_previous,&FDT_item_previous);
            __DFFSD_ReadFDT_Item(PTT_flash_tag,
                                  FDT_item_no_next,&FDT_item_next);
            //���ƶ����ǰ��Ŀ�ĺ�ָ�룬ָ���ƶ���ĺ���Ŀ
            fill_little_32bit(FDT_item_previous.next,0,FDT_item_no_next);
            //���ƶ���ĺ���Ŀ��ǰָ�룬ָ���ƶ����ǰ��Ŀ
            fill_little_32bit(FDT_item_next.previous,0,FDT_item_no_previous);
            __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_previous,&FDT_item_previous);
            __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_next,&FDT_item_next);
        }
    }


    FDT_item_no_previous = pick_little_32bit(src_FDT_item.previous,0);
     FDT_item_no_next = pick_little_32bit(src_FDT_item.next,0);
    //���ƶ���ֻ��һ��
    if((FDT_item_no_previous == src_FDT_item_no)&&(FDT_item_no_next == src_FDT_item_no))
    {
        //ֻ��һ�ûʲô�ô���
    }
    else
    {
        if((FDT_item_no_previous != src_FDT_item_no)&&(FDT_item_no_previous==FDT_item_no_next))
        {//���ƶ��ģ���һ���ֵ�Ŀ¼���ļ���
            //������һ���ǰָ��ͺ�ָ�룬��ָ���Լ���FDT_item_no_previous��FDT_item_no_next
            //��ͬһ�����һ��
             __DFFSD_ReadFDT_Item(PTT_flash_tag,
                                  FDT_item_no_previous,&FDT_item_previous);
            fill_little_32bit(FDT_item_previous.previous,0,FDT_item_no_previous);
            fill_little_32bit(FDT_item_previous.next,0,FDT_item_no_previous);
           __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_previous,&FDT_item_previous);
        }
        else
        {
            __DFFSD_ReadFDT_Item(PTT_flash_tag,
                                  FDT_item_no_previous,&FDT_item_previous);
            __DFFSD_ReadFDT_Item(PTT_flash_tag,
                                  FDT_item_no_next,&FDT_item_next);
            //���ƶ����ǰ��Ŀ�ĺ�ָ�룬ָ���ƶ���ĺ���Ŀ
            fill_little_32bit(FDT_item_previous.next,0,FDT_item_no_next);
            //���ƶ���ĺ���Ŀ��ǰָ�룬ָ���ƶ����ǰ��Ŀ
            fill_little_32bit(FDT_item_next.previous,0,FDT_item_no_previous);
            __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_previous,&FDT_item_previous);
            __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no_next,&FDT_item_next);
        }
    }

    __DFFSD_WriteDDR(PTT_flash_tag);//ddrҲ��ı䣬���޸ı䣬�������д
    return true;
}

//----����ļ��е����ļ���Ŀ¼����---------------------------------------------
//���ܣ�����ļ��е����ļ���Ŀ¼��������������Ŀ¼������Ŀ
//������PTT_device_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//      fp��Ŀ���ļ��е��ļ�ָ��
//���أ����ļ���Ŀ¼����
//-----------------------------------------------------------------------------
uint32_t DFFSD_CheckFolder(struct tagFileRsc *parent)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagFdtInfo FDT_item;
    struct tagStPTT_Flash *PTT_flash_tag;
    uint32_t FDT_item_no,FDT_item_no_next;
    uint32_t result;
    if(parent == NULL)
        return 0;
    if( ! parent->file_attr.bits.folder)      //�����ļ�������Ŀ¼
        return 0;
    result = 0;
    PTT_device_tag = (struct tagPTTDevice *)(parent->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    FDT_item_no = (uint32_t)parent->file_medium_tag;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    //��һ������Ŀ��FDT��Ŀ��
    FDT_item_no = pick_little_32bit(FDT_item.fstart_dson,0);
    if(FDT_item_no == CN_LIMIT_UINT32)
        return 0;
    FDT_item_no_next = FDT_item_no;
    do
    {
        result++;
        __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no_next,&FDT_item);
        FDT_item_no_next = pick_little_32bit(FDT_item.next,0);
    }while(FDT_item_no != FDT_item_no_next);
    return result;
}

//----ɾ����Ŀ���ļ�/Ŀ¼��---------------------------------------------------
//���ܣ�ɾ��һ���ļ�(Ŀ¼)��ֻ��ɾ����Ŀ¼���Ѿ��رյ��ļ���
//������fp����ɾ����Ŀ¼���ļ���ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t DFFSD_RemoveFile(struct tagFileRsc *fp)
{
    struct tagFdtInfo FDT_item;
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFlashChip *chip;
    uint32_t FDT_item_no,loop,block_no,block_no_next,blocks;
    if(fp == NULL)
        return false;
    if(( ! fp->file_attr.bits.folder) && (fp->open_counter != 0))
        return false;   //fp��һ���Ѿ��򿪵��ļ�
    if(fp->file_attr.bits.folder)
    {
        if(DFFSD_CheckFolder(fp) != 0)
            return false;   //��һ���ǿ�Ŀ¼
    }
    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    if( ! fp->file_attr.bits.folder) //�Ǹ��ļ�����Ҫ�ͷ��ļ�ռ�õĴ洢��
    {
        if(fp->file_size == 0)  //0�����ļ�Ҳռ��1��
            blocks = 1;
        else
            blocks = (u32)((fp->file_size + (s64)chip->block_size -1) / chip->block_size);
        __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);//ȡĿ¼��
        block_no = pick_little_32bit(FDT_item.fstart_dson,0);//ȡ�ļ��׿���
        for(loop = 0; loop < blocks; loop++)
        {
            //������һ����
            block_no_next = PTT_flash_tag->DDR_MAT[block_no].next;
            __DFFSD_FreeBlock(PTT_flash_tag,block_no); //�ͷ�һ��
            block_no = block_no_next;
        }
    }
    __DFFSD_FreeFDTItem(PTT_flash_tag,FDT_item_no); //�ͷ�FDT��

    __DFFSD_WriteDDR(PTT_flash_tag);
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
//      ���һ�Σ����ͷ��ڴ档����û���ȷ�ص���DFFSD_item_traversal_son������
//      ����Ŀ¼���ڴ������ȫ����������Ϊstruct file_rsc�ṹ������ڴ潫��
//      Ҫ�û��ֶ��ͷ�
//---------------------------------------------------------------------
struct tagFileRsc * DFFSD_ItemTraversalSon(struct tagFileRsc *parent,
                                           struct tagFileRsc *current)
{
    uint32_t FDT_item_no,first_FDT_item_no;
    struct tagFdtInfo FDT_item;
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;

    if(parent == NULL)
        return NULL;
    if(!parent->file_attr.bits.folder)       //��Ŀ¼�Ǹ��ļ���
        return NULL;
    if(parent->file_medium_tag == CN_LIMIT_UINT32)
        return NULL;

    PTT_device_tag =(struct tagPTTDevice *)(parent->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
       __DFFSD_ReadFDT_Item(PTT_flash_tag,parent->file_medium_tag,&FDT_item);
    first_FDT_item_no = pick_little_32bit(FDT_item.fstart_dson,0);
    if(first_FDT_item_no == CN_LIMIT_UINT32)    //��Ŀ¼�Ǹ���Ŀ¼
        return NULL;

    if(current == NULL)    //���curent_file�ǿյģ����ص�һ������
    {
        current = M_MallocLc(sizeof(struct tagFileRsc),CN_TIMEOUT_FOREVER);
        if(current == NULL)
            return NULL;
        __DFFSD_ReadFDT_Item(PTT_flash_tag,first_FDT_item_no,&FDT_item);
        __DFFSD_UasmFDT_Item(PTT_flash_tag,current,&FDT_item);
        current->file_medium_tag = (ptu32_t)first_FDT_item_no;
    }
    else if(current->file_medium_tag != CN_LIMIT_UINT32)
    {
        __DFFSD_ReadFDT_Item(PTT_flash_tag,current->file_medium_tag,&FDT_item);
        //parent��cn_limit_uint32,˵��current_file��Ӧ���ļ��Ѿ���ɾ��
        if(pick_little_32bit(FDT_item.parent,0) == CN_LIMIT_UINT32)
        {
            //��ǰ�ļ���ɾ����ȡ��һ������
            __DFFSD_ReadFDT_Item(PTT_flash_tag,first_FDT_item_no,&FDT_item);
            __DFFSD_UasmFDT_Item(PTT_flash_tag,current,&FDT_item);
            current->file_medium_tag = (ptu32_t)first_FDT_item_no;
        }
        else
        {
              FDT_item_no = pick_little_32bit(FDT_item.next,0);
            if(FDT_item_no == first_FDT_item_no)    //current_file�Ѿ������һ��
            {
                free(current);
                return NULL;
            }
            __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
            __DFFSD_UasmFDT_Item(PTT_flash_tag,current,&FDT_item);
            current->file_medium_tag = (ptu32_t)FDT_item_no;
        }
    }
    else
    {
        return NULL;    //��ǰλ�÷Ƿ�
    }
    return current;
}


//----�Ƚ�����-----------------------------------------------------------------
//����: �Ƚ������ļ�(Ŀ¼)���ַ����Ƿ���ȣ����ļ�ϵͳ֧��255�ַ�����flash
//      driverֻ֧��cn_file_name_len���ַ�����˵�fs_name�ĳ��ȳ���
//      cn_file_name_lenʱ����β�Ƚϡ�����β�󳤶��Բ���ȣ�ֱ�ӷ���false������
//      ����ȣ������ַ��Ƚϣ�ȫ�ȷ���true�����򷵻�false��
//����: fs_name�����ļ�ϵͳ�ṩ���ļ���ָ��
//      FDT_name��FDT���ж�ȡ���ļ���ָ��
//����: true = ��ȣ�false=�����
//----------------------------------------------------------------------------
bool_t __DFFSD_CmpFileName(char *fs_name,char *FDT_name)
{
    uint32_t fs_len,FDT_len;
    //fs_name���Ѿ������ַ������ȺϷ��Լ���ָ��
    fs_len = strlen(fs_name);
    //FDT_name���Ѿ������ַ������ȺϷ��Լ���ָ��
    FDT_len = strlen(FDT_name);
    if(fs_len > CN_FLASH_FILE_NAME_LIMIT)
        fs_len = CN_FLASH_FILE_NAME_LIMIT;
    if(fs_len != FDT_len)
        return false;
    fs_len = fs_len > FDT_len? FDT_len:fs_len;
    for(FDT_len = 0; FDT_len < fs_len; FDT_len++)
        if(fs_name[FDT_len] != FDT_name[FDT_len])
            return false;
    return true;
}

//----�����ļ�������-----------------------------------------------------------
//����: �����ļ��Ĵ�ģʽ��Ϊ�ļ����仺������
//����: PTT_flash_tag����������flash�����Ĵ洢ý���ǩ
//      fp�����ļ�ϵͳ�ṩ���ļ���ָ��
//      mode���ļ�����ģʽ
//����: true = ��ȣ�false=�����
//----------------------------------------------------------------------------
bool_t __DFFSD_allocate_file_buf(struct tagStPTT_Flash *PTT_flash_tag,
                              struct tagFileRsc *fp,
                              enum _FILE_OPEN_MODE_ mode)
{
    struct tagFlashChip *chip;
    uint32_t read_buf_len;
    uint32_t write_buf_len;
    uint8_t *buf;
    uint8_t *buf1;
    chip = PTT_flash_tag->chip;
    read_buf_len = chip->PTT_read_buf_size;
    write_buf_len = chip->PTT_write_buf_size;
    if(fp->file_attr.bits.folder)     //FDT��Ŀ��һ��Ŀ¼
    {
        Ring_Init(&fp->read_rw_buf,NULL,0);     //Ŀ¼�Ķ���������Ϊ��
        Ring_Init(&fp->write_buf,NULL,0);       //Ŀ¼��д��������Ϊ��
        fp->p_read_buf = NULL;
        fp->p_write_buf = NULL;
        fp->read_ptr = 0;
        fp->write_ptr = 0;
        fp->file_size = 0;
    }else       //��һ���ļ�����Ҫ����mode�����д������
    {
        switch(mode)
        {
            case EN_R_RB:
            {
                buf =(uint8_t*)M_MallocLcHeap(read_buf_len,chip->ChipHeap,0);//��ģʽֻ�ж�������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                Ring_Init(&fp->read_rw_buf,buf,read_buf_len);   //��ʼ����������
                Ring_Init(&fp->write_buf,NULL,0);           //��ʼ��д������Ϊ��
                fp->p_read_buf = &fp->read_rw_buf;  //��������ָ�븳ֵ
                fp->p_write_buf = NULL;             //д������ָ���ÿ�
                fp->read_ptr = 0;                   //��λ����0
                fp->write_ptr = 0;                  //дλ����0
            }break;
            case EN_W_WB:
            {
                buf =(uint8_t*)M_MallocLcHeap(write_buf_len,chip->ChipHeap,0);//��ģʽֻ��д������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                Ring_Init(&fp->read_rw_buf,NULL,0);         //��ʼ����������Ϊ��
                Ring_Init(&fp->write_buf,buf,write_buf_len);//��ʼ��д������
                fp->p_read_buf = NULL;              //��������ָ���ÿ�
                fp->p_write_buf = &fp->write_buf;   //д������ָ�븳ֵ
                fp->read_ptr = 0;                   //��λ����0
                fp->write_ptr = 0;                  //дλ����0
                DFFSD_SetFileSize(fp,0);//��ģʽ��Ҫ���ļ����
            }break;
            case EN_A_AB:
            {
                buf =(uint8_t*)M_MallocLcHeap(write_buf_len,chip->ChipHeap,0);//��ģʽֻ��д������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                Ring_Init(&fp->read_rw_buf,NULL,0);         //��ʼ����������Ϊ��
                Ring_Init(&fp->write_buf,buf,write_buf_len);//��ʼ��д������
                fp->p_read_buf = NULL;              //��������ָ���ÿ�
                fp->p_write_buf = &fp->write_buf;   //д������ָ�븳ֵ
                fp->read_ptr = 0;                   //��λ����0
                fp->write_ptr = fp->file_size;      //дλ��ָ���ļ�β
            }break;
            case EN_R_RB_PLUS:
            {
                buf = (uint8_t*)M_MallocLcHeap(read_buf_len,chip->ChipHeap,0);      //�����������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                buf1 = (uint8_t*)M_MallocLcHeap(write_buf_len,chip->ChipHeap,0);    //����д������
                if(buf1 == NULL)
                {

                    free(buf);
                return false;           //����������ʧ�ܣ����ش���
                }
                Ring_Init(&fp->read_rw_buf,buf,read_buf_len);   //��ʼ����������
                Ring_Init(&fp->write_buf,buf1,write_buf_len);   //��ʼ��д������
                fp->p_read_buf = &fp->read_rw_buf;      //���ö�������ָ��
                fp->p_write_buf = &fp->write_buf;       //����д������ָ��
                fp->read_ptr = 0;                       //��λ����0
                fp->write_ptr = 0;                      //дλ����0
            }break;
            case EN_W_WB_PLUS:
            {
                buf = (uint8_t*)M_MallocLcHeap(read_buf_len,chip->ChipHeap,0);      //�����������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                buf1 = (uint8_t*)M_MallocLcHeap(write_buf_len,chip->ChipHeap,0);    //����д������
                if(buf1 == NULL)
                {

                    free(buf);
                    return false;           //����������ʧ�ܣ����ش���
                }
                Ring_Init(&fp->read_rw_buf,buf,read_buf_len);   //��ʼ����������
                Ring_Init(&fp->write_buf,buf1,write_buf_len);   //��ʼ��д������
                fp->p_read_buf = &fp->read_rw_buf;      //���ö�������ָ��
                fp->p_write_buf = &fp->write_buf;       //����д������ָ��
                fp->read_ptr = 0;                       //��λ����0
                fp->write_ptr = 0;                      //дλ����0
                DFFSD_SetFileSize(fp,0);//��ģʽ��Ҫ���ļ����
            }break;
            case EN_A_AB_PLUS:
            {
                buf = (uint8_t*)M_MallocLcHeap(read_buf_len,chip->ChipHeap,0);      //�����������
                if(buf == NULL)
                    return false;           //����������ʧ�ܣ����ش���
                buf1 = (uint8_t*)M_MallocLcHeap(write_buf_len,chip->ChipHeap,0);    //����д������
                if(buf1 == NULL)
                {

                    free(buf);
                    return false;           //����������ʧ�ܣ����ش���
                }
                Ring_Init(&fp->read_rw_buf,buf,read_buf_len);   //��ʼ����������
                Ring_Init(&fp->write_buf,buf1,write_buf_len);   //��ʼ��д������
                fp->p_read_buf = &fp->read_rw_buf;      //���ö�������ָ��
                fp->p_write_buf = &fp->write_buf;       //����д������ָ��
                fp->read_ptr = 0;                       //��λ����0
                fp->write_ptr = fp->file_size;          //дλ��ָ���ļ�β
            }break;
            default:break;
        }
    }
    return true;
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
uint32_t DFFSD_OpenItem(char *name,struct tagFileRsc *parent,
                       struct tagFileRsc *result,enum _FILE_OPEN_MODE_ mode)
{
    uint32_t FDT_item_no,FDT_item_eldest;
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFdtInfo FDT_item;
    bool_t found = false;

    if((parent == NULL) || (result == NULL))
        return CN_LIMIT_UINT32;
    PTT_device_tag = (struct tagPTTDevice *)(parent->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;

    FDT_item_no = (uint32_t)parent->file_medium_tag;
    //��ȡ��Ŀ¼��FDT����
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    if(!(FDT_item.mattr & CN_FS_ATTR_DIRECTORY))
        return CN_LIMIT_UINT32;       //��Ŀ¼ʵ������һ���ļ�
    //��ȡ��Ŀ¼�ĵ�һ����Ŀ¼/�ļ���FDT�����
    FDT_item_eldest = pick_little_32bit(FDT_item.fstart_dson,0);
    if(FDT_item_eldest == CN_LIMIT_UINT32)
        return CN_FS_ITEM_INEXIST;  //parent��һ����Ŀ¼
    FDT_item_no = FDT_item_eldest;
    do  //ѭ����Ŀ¼��ֱ������
    {
        //��ȡ��FDT����
        __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
        FDT_item.name[CN_FLASH_FILE_NAME_LIMIT] = '\0'; //�ַ������������ݴ���
        if(__DFFSD_CmpFileName(name,FDT_item.name))
        {
            found = true;       //�ҵ�ƥ����ļ�(Ŀ¼)
            break;
        }else
        {
            if(pick_little_32bit(FDT_item.next,0) ==FDT_item_no)//������ѭ��
                break;
            FDT_item_no = pick_little_32bit(FDT_item.next,0);
            if(FDT_item_no == CN_LIMIT_UINT32)//��ֹ��ѭ��
                break;
        }
    }while(FDT_item_no != FDT_item_eldest);
    if(found)
    {
        result->file_medium_tag = (ptu32_t)FDT_item_no;
        result->file_semp = NULL;
        //��flash��FDT��Ŀ���ڴ澵��FDT_item����ȡ�ļ���Ϣ
        __DFFSD_UasmFDT_Item(PTT_flash_tag,result,&FDT_item);
        if(__DFFSD_allocate_file_buf(PTT_flash_tag,result,mode))//�����ļ�������
        {
            return CN_FS_OPEN_SUCCESS;
        }else
            return CN_FS_ITEM_EXIST;
    }else
        return CN_FS_ITEM_INEXIST;
}

//----���ң��ļ�/Ŀ¼��--------------------------------------------------------
//���ܣ��ڷ����в����ļ�(Ŀ¼)
//������name���ļ���Ŀ¼����
//      parent���ļ�/Ŀ¼�ĸ�Ŀ¼��
//      result�������ҵ����ļ�����Ϣ
//���أ�true=�ļ�/Ŀ¼���ڣ�����result�з����ļ���Ϣ(�������ļ�������)��
//      false = �ļ�/Ŀ¼�����ڣ����޸�result��
//----------------------------------------------------------------------------
bool_t DFFSD_LookForItem(char *name,struct tagFileRsc *parent,
                         struct tagFileRsc *result)
{
    uint32_t FDT_item_no,FDT_item_eldest;
    struct tagPTTDevice *PTT_device_tag;
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFdtInfo FDT_item;
    bool_t found = false;
    if(parent == NULL)
        return false;
    PTT_device_tag = (struct tagPTTDevice *)(parent->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    FDT_item_no = (uint32_t)parent->file_medium_tag;
    //��ȡ��Ŀ¼��FDT����
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    if(!(FDT_item.mattr & CN_FS_ATTR_DIRECTORY))
        return false;       //��Ŀ¼ʵ������һ���ļ�
    //��ȡ��Ŀ¼�ĵ�һ����Ŀ¼/�ļ���FDT�����
    FDT_item_eldest = pick_little_32bit(FDT_item.fstart_dson,0);
    if(FDT_item_eldest == CN_LIMIT_UINT32)
        return false;  //parent��һ����Ŀ¼
    FDT_item_no = FDT_item_eldest;
    do  //ѭ����Ŀ¼��ֱ������
    {
        //��ȡ��FDT����
        __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
        FDT_item.name[CN_FLASH_FILE_NAME_LIMIT] = '\0'; //�ַ������������ݴ���
        if(__DFFSD_CmpFileName(name,FDT_item.name))
        {
            if(result != NULL)
            {
                result->file_medium_tag = (ptu32_t)FDT_item_no;
                result->file_semp = NULL;

                //��flash��FDT��Ŀ���ڴ澵��FDT_item����ȡ�ļ���Ϣ
                __DFFSD_UasmFDT_Item(PTT_flash_tag,result,&FDT_item);
            }
            found = true;
            break;
        }else
        {
            FDT_item_no = pick_little_32bit(FDT_item.next,0);
        }
    }while(FDT_item_no != FDT_item_eldest);
    return found;
}

//----�ر��ļ�(Ŀ¼)-----------------------------------------------------------
//����: �ر�һ���򿪵��ļ�(Ŀ¼)��������ļ������ͷŶ�д����������ʹ������
//      opened_sum�����������Ŀ¼����ֻ��opened_sum������������ֻ�Ǵ���رչ���
//      ��洢������صĲ�����ϵͳ��Դ������file.cģ�鴦��
//������fp�����������ļ�ָ��
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t DFFSD_CloseItem(struct tagFileRsc *fp)
{
    if(fp == NULL)
        return false;
    if(fp->file_attr.bits.folder)
    {
        //�Ǹ�Ŀ¼������ûʲô��Ҫ�����
    }else
    {
        if(Ring_Capacity(&fp->write_buf) != 0)    //�鿴�ļ�д�������Ƿ��
        {
            DFFSD_FlushFile(fp);//д���������ݱ���д��
            free(Ring_GetBuf(&fp->write_buf)); //�ļ�д�������ǿգ��ͷ�֮
        }
        if(Ring_Capacity(&fp->read_rw_buf) != 0)     //�鿴�ļ����������Ƿ��
        {
            free(Ring_GetBuf(&fp->read_rw_buf));  //�ļ����������ǿգ��ͷ�֮
        }
    }
    return true;
}

//----�ļ���Ŀ¼������---------------------------------------------------------
//���ܣ��޸�һ���ļ���Ŀ¼�������֣������ֱ�����fp�С�fp�е����ֿ��Դﵽ255��
//      ������ֻ��ǰcn_flash_file_name_limit�ַ���Ч��
//������fp�����������ļ�ָ��
//���أ�true=�ɹ���false=ʧ��
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: ���Ӷ��ļ�ʱ�����Ե�֧��,���Ӻ���__DFFSD_UpdateItemTime
//   ����: ������
//-----------------------------------------------------------------------------
bool_t DFFSD_RenameItem(struct tagFileRsc *fp,char *newname)
{
    struct tagPTTDevice *PTT_device_tag;
    struct tagFdtInfo FDT_item;
    struct tagStPTT_Flash *PTT_flash_tag;
    uint32_t name_len;
    uint32_t FDT_item_no;
    if(fp == NULL)
        return false;

    PTT_device_tag = (struct tagPTTDevice *)(fp->home_PTT);
    PTT_flash_tag = (struct tagStPTT_Flash *)PTT_device_tag->PTT_medium_tag;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_ReadFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    //fp->name���ļ�ϵͳģ���ṩ�ģ������ֵĳ��ȱ���Ӧ���ǺϷ��ģ�����fp��Ϊ
    //һ����Դ��㣬�Ǳ�¶�������µģ��п��ܱ����޸ģ��ʲ���ʹ��strlen��
    name_len = strnlen(fp->name,CN_FLASH_FILE_NAME_LIMIT+1);
    if(name_len <= CN_FLASH_FILE_NAME_LIMIT)  //�������ֲ�����cn_flash_file_name_limit
        strcpy(FDT_item.name,newname);
    else
    {

        memcpy(FDT_item.name,newname,CN_FLASH_FILE_NAME_LIMIT);
        FDT_item.name[CN_FLASH_FILE_NAME_LIMIT] = '\0';
    }
    __DFFSD_UpdateItemTime(&FDT_item);
    __DFFSD_WriteFDT_Item(PTT_flash_tag,FDT_item_no,&FDT_item);
    return true;
}

//----��Ⲣ����FDT��-----------------------------------------------------------
//���ܣ�����ڴ�����ɾ���ļ��Ĺ����жϵ磬�����ܵ���FDT�������ڲ�����������ѣ�
//      ��������Ⲣ�޸�֮
//������PTT_device_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//���أ�true=������Ļ��߳ɹ����ģ�false=�д����޸�ʧ��
//-----------------------------------------------------------------------------
bool_t __DFFSD_RepairFDT(struct tagStPTT_Flash *PTT_flash_tag)
{
    return true;
}

//----��ѯĿ¼��ߴ�-----------------------------------------------------------
//���ܣ���ѯһ��������Ŀ¼��ߴ磬���ֽڼ��㡣��Ӧ�ó�����Ҫ��ȡ����Ŀ¼��ʱ��
//      �������ñ���������Ŀ¼��ߴ磬Ȼ����ܷ����ڴ档
//������PTT_device_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//���أ�Ŀ¼��ߴ�
//-----------------------------------------------------------------------------
uint32_t DFFSD_CheckFDT_Size(struct tagPTTDevice *PTT_device_tag)
{
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFlashChip *chip;
    uint32_t result,block_no,block_no_next;
    if(PTT_device_tag == NULL)
        return 0;
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;
    block_no_next = PTT_flash_tag->DDR_FDSB;
    result = 0;
    do
    {
        block_no = block_no_next;

           if(PTT_flash_tag->DDR_MAT[block_no].attr == cn_FDT_block)
            result ++;
        block_no_next = PTT_flash_tag->DDR_MAT[block_no].next;
    }while(block_no != block_no_next);
    return result * chip->block_size;
}

//----��Ŀ¼��-----------------------------------------------------------------
//���ܣ�����һ������������Ŀ¼����ԭʼ���ݶ�������������
//������PTT_device_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//      buf�������ȡ�����ݵĻ�����ָ�룬�û�����ǰ�������ſռ�
//���أ���
//-----------------------------------------------------------------------------
void DFFSD_ReadFDT(struct tagPTTDevice *PTT_device_tag,uint8_t *buf)
{
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFlashChip *chip;
    uint32_t block_no,block_no_next;
    uint32_t completed = 0;
    if((PTT_device_tag == NULL) || (buf == NULL))
        return;
    PTT_flash_tag = (struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
    chip = PTT_flash_tag->chip;
    block_no_next = PTT_flash_tag->DDR_FDSB;
    do
    {
        block_no = block_no_next;
        chip->read_data_with_ecc(block_no + PTT_flash_tag->start_block,0,
                                 buf+completed,chip->block_size);
        block_no_next = PTT_flash_tag->DDR_MAT[block_no].next;
        completed += chip->block_size;
    }while(block_no != block_no_next);
    return ;
}

//----��ѯ��������-----------------------------------------------------------
//����: ���ط����ܿռ䣬��Ч�ռ䣬���пռ���ֽ���
//����:
//����: ��
//-----------------------------------------------------------------------------
void DFFSD_CheckPTT(struct tagPTTDevice *PTT_device_tag,
                     sint64_t *sum_size,sint64_t *valid_size,sint64_t *free_size)
{
    struct tagStPTT_Flash *PTT_flash_tag;
    struct tagFlashChip *chip;
    if(PTT_device_tag == NULL)
    {
        if(sum_size != NULL)
            *sum_size = 0;
        if(valid_size != NULL)
            *valid_size = 0;
        if(free_size != NULL)
            *free_size = 0;
    }else
    {
        PTT_flash_tag =(struct tagStPTT_Flash*)PTT_device_tag->PTT_medium_tag;
        chip = PTT_flash_tag->chip;
        if(sum_size != NULL)
            *sum_size = PTT_flash_tag->block_sum * chip->block_size;
        if(valid_size != NULL)
            *valid_size = PTT_flash_tag->valid_sum * chip->block_size;
        if(free_size != NULL)
            *free_size = PTT_flash_tag->free_sum * chip->block_size;
    }
}
//----���PTT_device_tag-------------------------------------------------------
//����: ���PTT_device_tag
//������PTT_device_tag���������ķ���ָ�룬ͨ�ýṹ����flashר�á�
//      name��������
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_Fill_FDT_Device(struct tagPTTDevice *PTT_device_tag,
                                 char *name)
{
    PTT_device_tag->format = DFFSD_FormatPTT;
    PTT_device_tag->write = DFFSD_WriteFile;
    PTT_device_tag->read = DFFSD_ReadFile;
    PTT_device_tag->flush = DFFSD_FlushFile;
    PTT_device_tag->query_file_stocks = DFFSD_QueryFileStocks;
    PTT_device_tag->query_file_cubage = DFFSD_QueryFileCubage;
    PTT_device_tag->check_PTT = DFFSD_CheckPTT;
    PTT_device_tag->set_file_size = DFFSD_SetFileSize;
    PTT_device_tag->seek_file = DFFSD_SeekFile;
    PTT_device_tag->create_item = DFFSD_CreateItem;
//    PTT_device_tag->move_file= DFFSD_MoveFile;
    PTT_device_tag->remove_item = DFFSD_RemoveFile;
    PTT_device_tag->open_item = DFFSD_OpenItem;
    PTT_device_tag->close_item = DFFSD_CloseItem;
    PTT_device_tag->lookfor_item = DFFSD_LookForItem;
    PTT_device_tag->rename_item = DFFSD_RenameItem;
    PTT_device_tag->item_traversal_son = DFFSD_ItemTraversalSon;
    PTT_device_tag->check_fdt_size = DFFSD_CheckFDT_Size;
    PTT_device_tag->read_fdt = DFFSD_ReadFDT;
    PTT_device_tag->check_folder = DFFSD_CheckFolder;
    PTT_device_tag->opened_sum = 0;
    //name�϶��Ǹ����ȺϷ����ַ���
    if(strlen(name) <= CN_FLASH_PTT_NAME_LIMIT)
        strcpy(PTT_device_tag->name,name);  //����������
    else
    {
        memcpy(PTT_device_tag->name,name,CN_FLASH_PTT_NAME_LIMIT);
        PTT_device_tag->name[CN_FLASH_PTT_NAME_LIMIT] = '\0';
    }
}

//----DFFSDģ���ʼ��----------------------------------------------------------
//����: ������ϵͳ��Դ���������flashоƬ�����
//����: ��
//����: 1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_DFFSD(ptu32_t para)
{
    g_ptFlashfileSemp = Lock_SempCreate(1,0,CN_SEMP_BLOCK_FIFO,"flash_chip tree");
    if(g_ptFlashfileSemp == NULL)
        return 0;  //flash_chip���ź�������ʧ��
    //����Դ�����н���һ������㣬ϵͳ������оƬ��������������
    Rsc_AddTree(&tg_flash_chip_root_rsc,sizeof(struct  tagRscNode),
                        RSC_RSCNODE,"flash chip");
    return 1;
}

//----ɨ��оƬ-----------------------------------------------------------------
//����: ����ʱɨ��оƬ��ʶ��оƬ�еķ��������ѷ������뵽ϵͳ�豸����
//      �г�Ϊ��fs���豸�����豸��
//����: chip����ɨ���оƬ
//      start_block_no��ɨ����ʼ��ţ�ǰ��Ŀ��Ǳ�����
//����: ��
//------------
//���¼�¼:
// 1.����: 2015/3/19
//   ˵��: FLASH�д��ڵķ��������ڴ�֧����δ�����δ��ɣ�
//   ����: ������     
//-----------------------------------------------------------------------------
void __DFFSD_ScanChip(struct tagFlashChip *chip,uint32_t start_block_no)
{
    uint32_t loop,table_size,DDR_blocks,restored_block;
    //djy_handle_t fs_handle_right = NULL;
    struct tagStPTT_Flash *PTT_flash_tag = NULL;
    struct tagPTTDevice *PTT_device_tag = NULL;
    struct tagMDR_TempRecord MDR_record;
    struct tagFdtInfo FDT_item;

    chip->valid_sum = chip->block_sum - start_block_no;
    if(! __DFFSD_ReadMDR(chip,&MDR_record))
    {//ϵͳ��һ���������϶�û�н����������ʼ��ص�ʱ�򣬰�DFFSD_init_MDR�����ⲽ��ʼ��
        DFFSD_InitMDR(chip,start_block_no); //δ��ʼ����оƬ��Ĭ�����ó�ʼ��
        return ;
    }
    //���ļ�ϵͳ�豸�����ֽӿ�
    /*if((fs_handle_right = Driver_DevOpenRight("fs",0)) == NULL)
        return ;*/
    for(loop=0; loop < CN_PTT_SUM_MAX; loop++)   //�����������
    {
        PTT_device_tag = NULL;
        PTT_flash_tag = NULL;
        if(!chip->MDR.PTT_created[loop])
            continue;
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
        memset(PTT_flash_tag,0,sizeof(struct tagStPTT_Flash));
        memset(PTT_device_tag,0,sizeof(struct tagPTTDevice));
        PTT_flash_tag->ART_position = NULL;
        PTT_flash_tag->ART_block_no = NULL;
        PTT_flash_tag->ART_times = NULL;
        PTT_flash_tag->writed_DDR_main = NULL;
        PTT_flash_tag->writed_DDR_bak = NULL;
        PTT_flash_tag->DDR_DBL = NULL;
        PTT_flash_tag->DDR_MAT = NULL;
        PTT_flash_tag->chip = chip;     //flash�����洢������оƬ
        PTT_flash_tag->PTT_no = loop;   //��������оƬ�е����
        PTT_flash_tag->start_block = MDR_record.start_blocks[loop];
        PTT_flash_tag->PCRB_no = MDR_record.PCRB_block_no[loop]
                                    -PTT_flash_tag->start_block;
        PTT_flash_tag->DDR_main = MDR_record.DDR_main_block_no[loop]
                                    -PTT_flash_tag->start_block;
        PTT_flash_tag->DDR_bak = MDR_record.DDR_bak_block_no[loop]
                                    -PTT_flash_tag->start_block;
        PTT_flash_tag->block_sum = MDR_record.blocks_sum[loop];
        chip->valid_sum -= PTT_flash_tag->block_sum;

        //��亯��ָ��ͷ�����
        __DFFSD_Fill_FDT_Device(PTT_device_tag,MDR_record.name[loop]);
        //�����豸�Ĵ洢ý��ָ��flash�����洢��
        PTT_device_tag->PTT_medium_tag = (ptu32_t)PTT_flash_tag;

        PTT_device_tag->formatted = MDR_record.formatted[loop];
        //4+4+2=λ�ñ�4�ֽ�+��ű�4�ֽ�+������2�ֽ�
        table_size = PTT_flash_tag->block_sum * (4+4+2);
        //Ϊĥ��λ�ñ�����ڴ�
        PTT_flash_tag->ART_position = (uint32_t*)M_Malloc(table_size,0);
        if(PTT_flash_tag->ART_position == NULL)
            goto goto_exit_install_chip;
        //Ϊĥ���ű�����ڴ�
        PTT_flash_tag->ART_block_no = PTT_flash_tag->ART_position
                                      + PTT_flash_tag->block_sum;
        //Ϊĥ�����������ڴ�
        PTT_flash_tag->ART_times = (uint16_t *)(PTT_flash_tag->ART_block_no
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

        if(PTT_device_tag->formatted) //��������ʽ����
        {
            //����������лָ����ݣ�������Ҫ��ȡDDR,DDR���б�����ָ��鱣���Ŀ��ܡ�
            //����᲻�ɻָ����������Ϊ��ʹ���ļ����ƻ��������ļ���Ȼ���ã����
            //DDR���ƻ�������__read_DDR�����б������
            chip->restore_PCRB(PTT_flash_tag->PCRB_no+PTT_flash_tag->start_block,
                                &restored_block);
            if(__DFFSD_ScanDDR(PTT_flash_tag))
            {
                __DFFSD_SortART(PTT_flash_tag);
                __DFFSD_ReadFDT_Item(PTT_flash_tag,0,&FDT_item);
                __DFFSD_UasmRootFolder(PTT_flash_tag,&FDT_item);
            }else    //DDR�����������Ҫ���¸�ʽ������ʹ��
                PTT_device_tag->formatted = false;
            if(restored_block < chip->block_sum)
            {
                if(PTT_flash_tag->DDR_MAT[restored_block].attr == cn_FDT_block)
                    __DFFSD_RepairFDT(PTT_flash_tag);
            }
        }
        PTT_device_tag->chip_mutex_tag = chip->chip_mutex;
        if(Djyfs_FsDriverCtrl(enum_fs_add_PTT,
                            (ptu32_t)PTT_device_tag,0)
                            == EN_FS_NO_ERROR)
        {
            continue;   //���ͷŷ����豸����ڴ棬ֱ�ӽ�����һѭ��
        }else
        {
            goto goto_exit_install_chip;
        }

    }
    //������������
    return ;
goto_exit_install_chip:
    printk("ϵͳ��ʼ����������. ʧ��. ϵͳ�ڴ治��\r\n");
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
    if(PTT_device_tag !=NULL)   free(PTT_device_tag);
    return ;
}

//----��װһ��оƬ------------------------------------------------------------
//����: ��оƬ���Ѿ���ʼ���ķ����Ǽǵ��ļ�ϵͳ�У��÷�������δ��ʽ��
//����: chip������װ��оƬ
//      rev_blocks����оƬ����������
//����: �����ķ���������������0���ʾоƬ��δ��ʼ��
//----------------------------------------------------------------------------
bool_t DFFSD_InstallChip(struct tagFlashChip *chip,char *name,uint32_t rev_blocks)
{
#if CN_MDR_SIZE < CN_MDR_CDR_SIZE + CN_MDR_PTT_SIZE +CN_PTT_SUM_MAX*3*8*2+8
    return false;  //������������MDR������������С��2����Ŀ��û�����塣
#endif

    if(chip == NULL)
        return false;
    chip->chip_mutex = Lock_MutexCreate(name);
    if(chip->chip_mutex == NULL)
        return false;
      //�û�û��ָ���Դ�ר�õĶѣ���ʹ��ϵͳ�ѡ�
    if(chip->ChipHeap == NULL)
        chip->ChipHeap = M_FindHeap("sys");
    chip->block_buf = M_MallocHeap(chip->block_size,chip->ChipHeap,0);  //����黺�����ڴ�
    if(chip->block_buf == NULL)
    {
        Lock_MutexDelete(chip->chip_mutex);
        return false;
    }

    Lock_SempPend(g_ptFlashfileSemp,CN_TIMEOUT_FOREVER);
    //����flashоƬ���뵽flashоƬ����Դ����£����ڶ���β
    Rsc_AddSon(&tg_flash_chip_root_rsc,&chip->chip_node,
                        sizeof(struct tagFlashChip),RSC_FFS_FLASH_CHIP,name);
    Lock_SempPost(g_ptFlashfileSemp);
    __DFFSD_ScanChip(chip,rev_blocks);
    return true;
}

//-----------------------------------------------------------------------------
//����: �����ļ�ʱ������
//����:
//����:
//-----------------------------------------------------------------------------
void __DFFSD_UpdateFileTime(struct tagFileRsc *FileHandle)
{
    uint32_t FDT_Index;
    struct tagFdtInfo FDT;
    struct tagPTTDevice *PTT_Index;
    struct tagStPTT_Flash *PTT_Info;

    FDT_Index = FileHandle->file_medium_tag;
    PTT_Index =(struct tagPTTDevice *)(FileHandle->home_PTT);
    PTT_Info = (struct tagStPTT_Flash *)PTT_Index->PTT_medium_tag;

    __DFFSD_ReadFDT_Item(PTT_Info, FDT_Index, &FDT);
    __DFFSD_UpdateItemTime(&FDT);
     __DFFSD_WriteFDT_Item(PTT_Info, FDT_Index, &FDT);
    FileHandle->CreateTime = FDT.CreateTime;
    FileHandle->ModTime = FDT.ModTime;
}
