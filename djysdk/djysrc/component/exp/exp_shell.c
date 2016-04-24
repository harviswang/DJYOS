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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// ģ������: SysExp_Shell.c
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 9:20:36 AM/Mar 27, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:���ļ���Ҫ��Ϊ��ʵ��SHELL��һЩ�ӿڹ���
#include "config-prj.h"

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "exp.h"
#include "exp_record.h"
#include "exp_api.h"
#include "shell.h"
#include "systime.h"
// =============================================================================
// �������ܣ�Exp_ShellBrowseAssignedRecord
//           shell�鿴ָ����Ŀ���쳣��Ϣ
// ���������param,ָ����Ŀ�ַ���
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
bool_t Exp_ShellBrowseAssignedRecord(char *param)
{
    bool_t   result;
    char *word,*next_param;
    u32  item_num;
    u32  item_len;
    u8   *infomem;
    struct tagExpRecordPara recordpara;

    if(NULL == param)
    {
        result = false;
        goto browse_end;
    }

    word = Sh_GetWord(param,&next_param);
    item_num = strtol(word, (char **)NULL, 0);

    result = Exp_RecordCheckLen(item_num, &item_len);
    if(false == result)
    {
        goto browse_end;
    }

    infomem = M_MallocLc(item_len, CN_TIMEOUT_FOREVER);
    if(NULL == infomem)
    {
        result = false;
        goto browse_end;
    }

    result = Exp_RecordGet(item_num,item_len,infomem, &recordpara);
    if(false == result)
    {
        free(infomem);
        goto browse_end;
    }
    result = Exp_InfoDecoder(&recordpara);

    free(infomem);

browse_end:
    return result;

}
// =============================================================================
// �������ܣ�Exp_ShellBrowseRecordNum
//           shell�鿴�洢���쳣��Ŀ��
// ���������param,��
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
bool_t Exp_ShellBrowseRecordNum(char *param)
{
    bool_t  result;

    u32 expnum;

    result = Exp_RecordCheckNum(&expnum);
    if(true == result)
    {
        printk("ShellSysExp:There has been 0x%08x Exp record!\n\r",expnum);
        if(expnum > 0)
        {
            printk("ShellSysExp:VALID NO.(1-%d)\n\r",expnum);
        }
        else
        {
            printk("There has been no exception record yet!\n\r");
        }
    }
    return result;
}

// =============================================================================
// �������ܣ�Exp_ShellRecordClear
//           shell������е��쳣��Ϣ
// ���������param,��
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
bool_t Exp_ShellRecordClear(char *param)
{
    bool_t  result;

    result = Exp_RecordClear();

    if(true == result)
    {
        printk("ShellSysExp:Clear SysExp info success!\n\r");
    }
    else
    {
        printk("ShellSysExp:Clear SysExp info failed!\n\r");
    }

    return result;
}
struct tagShellCmdTab  gtExpShellCmdTab[] =
{
    {
        "expi",
        Exp_ShellBrowseAssignedRecord,
        "�鿴ָ���쳣��Ŀ",
        NULL
    },
    {
        "expn",
        Exp_ShellBrowseRecordNum,
        "�鿴�쳣��Ŀ����",
        NULL
    },
    {
        "expc",
        Exp_ShellRecordClear,
        "��������쳣��Ŀ",
        NULL
    }
};
#define CN_EXPSHELL_NUM  ((sizeof(gtExpShellCmdTab))/(sizeof(struct tagShellCmdTab)))
static struct tagShellCmdRsc sgExpShellRsc[CN_EXPSHELL_NUM];
// =============================================================================
// �������ܣ�Exp_ShellInit
//          expshellģ���ʼ��
// ���������param,��
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
bool_t Exp_ShellInit()
{
    bool_t result;
    if(Sh_InstallCmd(gtExpShellCmdTab,sgExpShellRsc,CN_EXPSHELL_NUM))
    {
        printk("�ɹ�����쳣�����shell����\n\r");
        result = true;
    }
    else
    {
        printk("����쳣�����shell����ʧ��!S\n\r");
        result = false;
    }
    return result;
}

