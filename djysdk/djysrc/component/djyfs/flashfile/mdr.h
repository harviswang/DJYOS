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
#ifndef __MDR_H__
#define __MDR_H__

#ifdef __cplusplus
extern "C" {
#endif

extern struct tagSemaphoreLCB *Fs_Semaphore; //����fs�����õ��ź���

void DFFSD_InitMDR(struct tagFlashChip *chip,uint32_t reserves);
bool_t __DFFSD_ReadMDR_Bak(struct tagFlashChip *chip,uint8_t *buf);
void __DFFSD_ReadMDR_Main(struct tagFlashChip *chip,uint8_t *buf);
bool_t __DFFSD_Allocate_PTT(struct tagFlashChip *chip,
                            uint32_t size,uint32_t *result);
bool_t __DFFSD_write_MDR_PTT(struct tagFlashChip *chip,
                           uint32_t PTT_no,uint8_t *MDR_PTT_buf);
bool_t DFFSD_Create_PTT(struct tagFlashChip *chip, uint32_t size,char *name);
uint32_t __DFFSD_Probe_MDR_Bak(struct tagFlashChip *chip);
bool_t __DFFSD_Verify_MDR_CDR(uint8_t *buf);
bool_t __DFFSD_verify_MDR_PTT(uint8_t *buf);
bool_t __DFFSD_Verify_EndFlag(uint8_t *buf);
uint32_t __DFFSD_Verify_MDR_Record(uint8_t *buf);
void __DFFSD_UasmRoll(struct tagFlashChip *chip,uint8_t *MDR_buf,
                       struct tagMDR_TempRecord *MDR_record,uint32_t end_offset);
bool_t __DFFSD_CheckMDR_Bak(struct tagFlashChip *chip);
uint32_t __DFFSD_FindRollNext(struct tagFlashChip *chip,uint8_t *MDR_buf);
ufast_t __DFFSD_UasmPTT(struct tagFlashChip *chip,
                         struct tagMDR_TempRecord *MDR_record,uint8_t *MDR_buf);
bool_t __DFFSD_ReadMDR(struct tagFlashChip *chip,struct tagMDR_TempRecord *MDR_record);
void __DFFSD_FillECC_MDR_Record(uint8_t *buf);
void __DFFSD_Fill_ECC_MDR_CDR(uint8_t *buf);
void __DFFSD_Fill_ECC_MDR_PTT(uint8_t *buf);
void __DFFSD_WriteEndFlagMain(struct tagFlashChip *chip);
void __DFFSD_WriteEndFlagBak(struct tagFlashChip *chip);
void __DFFSD_WriteMDR_Main(struct tagFlashChip *chip,uint8_t *MDR_buf,uint32_t len);
void __DFFSD_WriteMDR_Bak(struct tagFlashChip *chip,uint8_t *MDR_buf,uint32_t len);
void __DFFSD_WriteMDR_PTT_Main(struct tagFlashChip *chip,uint8_t *MDR_PTT_buf,
                                                      uint32_t PTT_no);
void __DFFSD_WriteMDR_PTT_Bak(struct tagFlashChip *chip,uint8_t *MDR_PTT_buf,
                                                 uint32_t PTT_no);
void __DFFSD_WriteMDR_RollRecord(struct tagFlashChip *chip,uint32_t PTT_no,
                                 uint32_t type,uint32_t value);
uint32_t __DFFSD_AsmRollBuf(struct tagFlashChip *chip,
                          struct tagMDR_TempRecord *MDR_record,uint8_t *roll_buf);
void __DFFSD_WriteRollMain(struct tagFlashChip *chip,struct tagMDR_TempRecord *MDR_record);
void __DFFSD_WriteRollBak(struct tagFlashChip *chip,struct tagMDR_TempRecord *MDR_record);
void __DFFSD_WriteMDR_Item(struct tagStPTT_Flash *PTT_flash_tag,
                    uint32_t type,uint32_t value);
void __DFFSD_SetMDR_PTT_Formatted(struct tagFlashChip *chip,uint32_t PTT_no);
void __DFFSD_EraseMDR_Main(struct tagFlashChip *chip);
void __DFFSD_EraseMDR_Bak(struct tagFlashChip *chip);

#ifdef __cplusplus
}
#endif

#endif // __MDR_H__

