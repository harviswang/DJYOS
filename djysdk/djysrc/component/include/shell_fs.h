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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------


#ifndef __SHELL_FS_H__
#define __SHELL_FS_H__
#include "file.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CN_ITEM_IS_FOLDER   0
#define CN_ITEM_IS_FILE     1
#define CN_ITEM_NO_EXIST    2

char *ShFs_GetFolderWildcardStr(char *wildcard_param,char *get_merge_str);
bool_t ShFs_DelWildcardFiles(char *wildcard_param);
bool_t ShFs_DelTreeWildcardFiles(char *wildcard_param);
bool_t ShFs_StrEndIncreaseBackSlash(char *str);
bool_t ShFs_StrEndWipeBackSlash(char *str);
bool_t ShFs_DirItemSon(struct tagFileRsc *parent_fp);
bool_t ShFs_DirWildcardFiles(char *wildcard_param);
bool_t ShFs_JudgeIsFolder(char *fullname);
bool_t ShFs_DelItemsOfFolderOrLoop(char *work_path);
bool_t ShFs_DelFilesOfFolder(char *work_path);
bool_t mkdir(char *md_param);
bool_t ShFs_RemoveTree(char *tree_param);
bool_t ShFs_RemoveFolder(char *rd_param);
bool_t ShFs_CD_Parent(char *next_param);
bool_t ShFs_CD_PTT(char *dbx);
bool_t ShFs_Chkptt(char *PTT_name);
bool_t ShFs_Copy(char *param);
bool_t ShFs_Move(char *param);
bool_t ShFs_Ren(char *param);
bool_t ShFs_MD(char *param);
bool_t ShFs_DelTree(char *param);
bool_t ShFs_RD(char *param);
bool_t ShFs_Del(char *param);
bool_t ShFs_CD(char *chdir_param);
bool_t ShFs_Dir(char *param);
bool_t ShFs_CreatePTT(char *param);
bool_t ShFs_Format(char *param);

bool_t ShFs_TestWrite(char *param);
bool_t ShFs_TestRead(char *param);



#ifdef __cplusplus
}
#endif

#endif //__SHELL_FS_H__