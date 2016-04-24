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
//------------------------------------------------------
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
#ifndef __FDT_H__
#define __FDT_H__

#ifdef __cplusplus
extern "C" {
#endif

bool_t __DFFSD_InitFDT(struct tagStPTT_Flash *PTT_flash_tag);
bool_t __DFFSD_AddFDTBlock(struct tagStPTT_Flash *PTT_flash_tag);
void __DFFSD_AddFDTFreeItem(struct tagStPTT_Flash *PTT_flash_tag,
                         uint32_t FDT_item_no);
bool_t __DFFSD_AcnodeFDTItem(struct tagStPTT_Flash *PTT_flash_tag,
                         uint32_t FDT_item_no);
void __DFFSD_FreeFDTItem(struct tagStPTT_Flash *PTT_flash_tag,
                         uint32_t FDT_item_no);
uint32_t __DFFSD_AllocateFDTItem(struct tagStPTT_Flash *PTT_flash_tag);
void __DFFSD_CompressFDT(struct tagStPTT_Flash *PTT_flash_tag);
bool_t __DFFSD_WriteFDT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                           uint32_t FDT_item_no,struct tagFdtInfo *FDT_item);
void __DFFSD_ReadFDT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                           uint32_t FDT_item_no,struct tagFdtInfo *FDT_item);
void __DFFSD_AsmFDT_Item(struct tagFdtInfo *FDT_item,union file_attrs attr,
                          char *name);
void __DFFSD_UasmFDT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                                struct tagFileRsc *fp,
                                struct tagFdtInfo *FDT_item);
void __DFFSD_UasmRootFolder(struct tagStPTT_Flash *PTT_flash_tag,
                                struct tagFdtInfo *FDT_item);
void __DFFSD_UpdateFDT_FstartDson(struct tagStPTT_Flash *PTT_flash_tag,
                       uint32_t FDT_item_no,uint32_t new_block_no);
void __DFFSD_UpdateFDT_FileSize(struct tagStPTT_Flash *PTT_flash_tag,
                                  struct tagFileRsc *fp,sint64_t new_size);
void __DFFSD_UpdateFDT_ItemName(struct tagStPTT_Flash *PTT_flash_tag,
                                  struct tagFileRsc *fp,char *name);
void __DFFSD_UpdateFDT_ItemAttr(struct tagStPTT_Flash *PTT_flash_tag,
                                  struct tagFileRsc *fp,union file_attrs attr);
//void __DFFSD_UpdateFDT_ItemCtime(struct tagStPTT_Flash *PTT_flash_tag,
//                                  struct tagFileRsc *fp,struct time new_time);
//void __DFFSD_UpdateFDT_ItemUtime(struct tagStPTT_Flash *PTT_flash_tag,
//                                  struct tagFileRsc *fp,struct time new_time);

#ifdef __cplusplus
}
#endif

#endif // __FDT_H__

