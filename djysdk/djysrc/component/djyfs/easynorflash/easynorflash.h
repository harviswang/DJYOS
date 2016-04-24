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
//�ļ�����:easynor�ļ�ϵͳ��ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: ����
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __EASY_FLASHFILE_H__
#define __EASY_FLASHFILE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "rsc.h"

#define CN_EASYNOR_PTT_NAME_LIMIT       31          //��������󳤶�,���һ���ֽ�'\0'
#define CN_EASYNOR_FILENAME_LIMIT       31          //��������󳤶�,���һ���ֽ�'\0'
#define CN_EASYNOR_CFG_LIMIT            16384       //������Ϣ����
#define CN_EASYNOR_CFG_ITEM_LIMIT       256         //ÿ��������ĳ���

//�ļ���������Աƫ����
#define CN_EASYNOR_NAME_OFF             0           //�ļ�����size=32
#define CN_EASYNOR_STARTBLOCK_OFF       32          //�ļ���ʼ�飬size=4
#define CN_EASYNOR_MAXSIZE_OFF          36          //�ļ���󳤶�
#define CN_EASYNOR_FILESIZE_OFF         40          //�ļ��ߴ磬�����룬size=8����26��
#define CN_EASYNOR_FILESIZE_END         240         //���һ���ļ��ߴ�
#define CN_EASYNOR_VERIFITY_OFF         248         //У�����="easyfile"

#define CN_EASYNOR_FILESIZE_NUM         26
struct tagEasynorFlashChip
{
    struct tagRscNode   chip_node;
    u32 block_sum;         //�ܿ���
    u32 block_size;        //��ߴ�
    u32 start_block;       //���ڴ洢�ļ����׿���
    u32 files_sum;            //�ļ���
    u8 *block_buf;         //�����ṩ�Ļ�������size= block_size
    char PTT_name[CN_EASYNOR_PTT_NAME_LIMIT+1];        //�������ķ�������

    //�����ǵ��͵�flash��������ָ�룬��дһ������оƬ������ʱ����Ϊ����оƬ��
    //��������,��Щ����������ÿһ��оƬ����Ҫ������Ҫ����NULL��

    //���Ѳ����涨������д��flashǰ���Ƿ���Ҫ���������buf=NULL������offset
    //��size�趨�������Ƿ񱻲���
    //true = ��׼���ã�����Ҫ����(��տ�),false = ��Ҫ������
    bool_t   (*query_block_ready)(u32 block,u32 offset,
                                  u8 *buf,u32 size);
    //����1�飬����: true = �ɹ�������false = ����
    bool_t (*erase_block)(u32 block);
    //��һ��֮�ڵ����ⳤ�����ݣ�����У�������ȷ����
    //����У��Ķ����������� ʵ�ʶ�ȡ�������������������򷵻�cn_limit_uint32
    u32 (*read_data)(u32 block,u32 offset,u8 *buf,u32 size);
    //����У��Ķ����������أ�cn_limit_uint32=������������ֵ��ʵ��д������
    u32 (*write_data)(u32 block,u32 offset,u8 *buf,u32 size);
};

struct tagEasyFlashFileInfo
{
    u32 start_block;        //�洢���ļ����׿�š�
    u32 max_size;           //���ļ���󳤶ȣ�filesize���ܳ���������ȡ�
    u32 start_item;            //���ļ�������ڷ�����е�λ��
    u32 filesize;
};



ptu32_t ModuleInstall_Easynor(ptu32_t para);
u32 Easynor_InstallChip(struct tagEasynorFlashChip *chip);
bool_t Easynor_CreateFile(char *filename, char *PTTname,u32 max_size);


#ifdef __cplusplus
}
#endif

#endif // __EASY_FLASHFILE_H__

