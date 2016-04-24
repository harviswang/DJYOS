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
//�ļ�����:flash�ļ�ϵͳ��DDR��Ĳ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _DDR_H__
#define _DDR_H__

#ifdef __cplusplus
extern "C" {
#endif

ufast_t __DFFSD_get_block_purpose(struct tagStPTT_Flash *PTT_flash_tag,
                            uint32_t u32_pre_ptr,uint32_t u32_next_ptr,
                            uint32_t block_no);
void __DFFSD_RebulidDDR_Bak(struct tagStPTT_Flash *PTT_flash_tag);
void __DFFSD_RebulidDDR_Main(struct tagStPTT_Flash *PTT_flash_tag);
uint32_t __DFFSD_MAT_Start(struct tagStPTT_Flash *PTT_flash_tag);
bool_t __DFFSD_ReadDDR(struct tagStPTT_Flash *PTT_flash_tag,
                            ufast_t which);
bool_t __DFFSD_VerifyDDR_Bak(struct tagStPTT_Flash *PTT_flash_tag);
bool_t __read_DDR_main(struct tagStPTT_Flash *PTT_flash_tag);
bool_t __read_DDR_bak(struct tagStPTT_Flash *PTT_flash_tag);
bool_t __DFFSD_ScanDDR(struct tagStPTT_Flash *PTT_flash_tag);
void __DFFSD_ART_Flip(struct tagStPTT_Flash *PTT_flash_tag,
                            ufast_t from,ufast_t to,uint32_t position);
bool_t __DFFSD_ExchangeBusy0Free8(struct tagStPTT_Flash *PTT_flash_tag);
uint32_t __DFFSD_AllocateBlock(struct tagStPTT_Flash *PTT_flash_tag);
bool_t __DFFSD_WriteDDR_Exe(struct tagStPTT_Flash *PTT_flash_tag,ufast_t which);
bool_t __DFFSD_WriteDDR(struct tagStPTT_Flash *PTT_flash_tag);
void __DFFSD_AddMAT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                           uint32_t last_block_no,uint32_t new_block_no);
void __DFFSD_UpdateMAT_Item(struct tagStPTT_Flash *PTT_flash_tag,
                       uint32_t old_block_no,uint32_t new_block_no);
void __DFFSD_WhirlART_Table(struct tagStPTT_Flash *PTT_flash_tag);
void __DFFSD_IncAbrasionTimes(struct tagStPTT_Flash *PTT_flash_tag,
                                uint32_t block_no);
void __DFFSD_RegisterInvalid(struct tagStPTT_Flash *PTT_flash_tag,
                            uint32_t block_no);
uint32_t __DFFSD_AllocateBlockSimple(struct tagStPTT_Flash *PTT_flash_tag);
void __DFFSD_FreeBlock(struct tagStPTT_Flash *PTT_flash_tag,
                    uint32_t block_no);
void __DFFSD_ExchangeART_Item(struct tagStPTT_Flash *PTT_flash_tag,
                                uint32_t pos1,uint32_t pos2);
void __DFFSD_SortART(struct tagStPTT_Flash *PTT_flash_tag);
uint32_t __DFFSD_CalculateDDR_Size(struct tagStPTT_Flash *PTT_flash_tag);

#ifdef __cplusplus
}
#endif

#endif // _DDR_H__
