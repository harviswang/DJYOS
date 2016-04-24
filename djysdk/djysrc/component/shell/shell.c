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
//����ģ��: djyshģ��
//���ߣ�wjz
//�汾��V0.1.0
//�ļ�����: djyos��shell��ģ�飬�ṩshell��ʵ�ֹ����Լ���������
//�޶���ʷ:
// 1. ����:2010-09-10֮ǰ
//   ����:lst
//   �°汾�ţ�djysh0.0.1
//     �޸�˵����shellʵ�ֵ���ơ�
// 2. ����:2011-06-15
//   ����:wjz
//   �°汾�ţ�djysh0.0.2
//   �޸�˵��: ����ļ�ϵͳ�����Լ�ע���ļ�ϵͳshellģ�顣
// 2. ����:2011-09-24
//   ����:wjz
//   �°汾�ţ�djysh1.0.0
//   �޸�˵��: ����¼����¼����͡��жϡ��ڴ桢���������
//               djysh1.0.0��������Ϊһ����������shell�ˡ�
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "rsc.h"
#include "djyos.h"
#include "stddev.h"
#include "shell.h"
#include "version.h"
#include "time.h"
#include "systime.h"
#include "char_term.h"

bool_t  Sh_ResetCpu();
bool_t  Sh_ResetCpuHot();

extern bool_t __erase_all_nand(char *param);
bool_t Sh_ShowForCycle(char *param);
bool_t Sh_ListRscSon(char *param);
bool_t Sh_ListRscTree(char *param);
void Sh_ListRscAll(void);
void __Sh_ShowSon(struct tagRscNode *branche);
void __Sh_ShowBranche(struct tagRscNode *branche);
bool_t Sh_ShowMemory(char *param);
bool_t Sh_FillMemory(char *param);
bool_t Sh_CmdParser(const char *buf,const char *keyword,char **param);
void Sh_CmdHelp(void);
bool_t Sh_ListCmdHelp(char *param);
bool_t Sh_Ver(char *param);
bool_t Sh_Date(char *param);
bool_t Sh_Time(char *param);
bool_t Sh_ExecCommand(char *cmd);
bool_t Sh_UninstallCmdByName(char *param);

ptu32_t Sh_Service(void);
ptu32_t ModuleInstall_Sh(ptu32_t para);

void (*fng_pPrintWorkPath)(void) = NULL;
bool_t (*fng_pCD_PTT)(const char *PTT_Name) = NULL;

extern bool_t Sh_PrintWorkPath(void);
//���������,����ϵͳ�Դ�������
struct tagShellCmdTab const shell_cmd_table[] =
{
    {
        "rsc-tree",
        Sh_ListRscTree,
        "�г�ϵͳ�е���Դ���ֺͲ�ι�ϵ",
        "��ʽ: rsc_tree TreeName,ʡ��TreeName���г�ȫ����Դ"
    },
    {
        "rsc-son",
        Sh_ListRscSon,
        "�г�����Դ",
        "��ʽ: rsc_tree RscName,ʡ��RscName���г�����Դ"
    },
    {
        "speed",
        Sh_ShowForCycle,
        "����forѭ�������ٶ�",
        NULL
    },
    {
        "d",
        Sh_ShowMemory,
        "��ȡ�ڴ��������",
        "�����ʽ:d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���"
    },
    {
        "f",
        Sh_FillMemory,
        "д���ݵ��ڴ�",
        NULL
    },
    {
        "help",
        Sh_ListCmdHelp,
        "DJYOS�����������Ϣ",
        NULL
    },
    {
        "ver",
        Sh_Ver,
        "��ʾdjyos�İ汾",
        NULL
    },
    {
        "date",
        Sh_Date,
        "��ʾ������������",
        NULL
    },
    {
        "time",
        Sh_Time,
        "��ʾ��ǰʱ���������������ʱ��",
        NULL
    },

    {
        "uninstall-cmd",
        Sh_UninstallCmdByName,
        "ɾ��shell����--������",
        NULL
    },
};

static struct tagShellCmdRsc tg_shell_cmd_rsc
                        [sizeof(shell_cmd_table)/sizeof(struct tagShellCmdTab)];
static struct tagRscNode cmd_list_node;

//----��ǰ����·����ָ�븳ֵ-----------------------------------------------------
//����: ����ָ�븳ֵ����ָ��ָ���ȡ��ǰ����·���ĺ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Sh_PrintWorkPathSet(void)
{
    fng_pPrintWorkPath = (void*)Sh_PrintWorkPath;
}

//----����ģ���ʼ������-------------------------------------------------------
//����: ��ʼ������ģ��
//����: ��
//����: 1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Sh(ptu32_t para)
{
    u16 shell_evtt;
    u32 loop;
    para = para;        //�����������澯
    shell_evtt = Djy_EvttRegist(EN_CORRELATIVE,  //�������¼�
                                CN_PRIO_REAL,       //Ĭ�����ȼ�
                                0,                  //�̱߳���������������Ч
                                0,                  //�߳����ޣ���������Ч
//                                1,                  //�������г�������
                                Sh_Service,         //��ں���
                                NULL,               //��ϵͳ����ջ
                                0x1000,             //ջ�ߴ�
                                "shell"             //�¼�������
                                );
    if(shell_evtt == CN_EVTT_ID_INVALID)
        return false;
    if(Djy_EventPop(shell_evtt,NULL,0,0,0,0)
                        == CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(shell_evtt);
    }
    Rsc_AddTree(&cmd_list_node,sizeof(struct tagRscNode),RSC_RSCNODE,"shell cmd list");
    for(loop = 0;
            loop<sizeof(shell_cmd_table)/sizeof(struct tagShellCmdTab);loop++)
    {
        tg_shell_cmd_rsc[loop].shell_cmd_func
                                    = shell_cmd_table[loop].shell_cmd_func;
        tg_shell_cmd_rsc[loop].help_hint
                                    = shell_cmd_table[loop].help_hint;
        tg_shell_cmd_rsc[loop].help_detailed
                                    = shell_cmd_table[loop].help_detailed;
        Rsc_AddSon(&cmd_list_node,&(tg_shell_cmd_rsc[loop].cmd_node),
                    sizeof(struct tagShellCmdRsc),RSC_SHELL_CMD,shell_cmd_table[loop].cmd);
    }
    return 1;
}

//----���shell����------------------------------------------------------------
//����: ����û���shell�������si�汾��һ�������ƵĿ�ִ��Ӱ�񣬲������ű���
//      ��ͨ��shellֱ�ӵ��ú������������������ӵ�������С���dlsp�汾�У�����
//      ͨ��ֱ�ӵ��ú����ķ�ʽʵ�֣����������ķ�ʽ����Ȼ���ٶȿ�����ơ�
//����: cmd_tab�������ָ�룬�ɲ���shell_cmd_table�ķ�ʽ����
//      cmd_rsc����Դ�ڵ�ָ�룬�ɲ���tg_shell_cmd_rsc�ķ�ʽ����
//      cmd_num, ����ӵı��а�����������
//����: true = �ɹ���
//      false= ʧ�ܣ������ǲ�������Ҳ������������а�������������������������
//          ����������������������������Ȼ�������������
//-----------------------------------------------------------------------------
bool_t Sh_InstallCmd(struct tagShellCmdTab const *cmd_tab,
                      struct tagShellCmdRsc *cmd_rsc,u32 cmd_num)
{
    u32 loop;
    bool_t result = true;
    if( (cmd_tab == NULL) || (cmd_rsc == NULL) || (cmd_num == 0))
        return result;

    for(loop = 0; loop < cmd_num;loop++)
    {
        if(Rsc_SearchSon(&cmd_list_node,cmd_tab[loop].cmd) == NULL)
        {
            cmd_rsc[loop].shell_cmd_func = cmd_tab[loop].shell_cmd_func;
            cmd_rsc[loop].help_hint = cmd_tab[loop].help_hint;
            cmd_rsc[loop].help_detailed = cmd_tab[loop].help_detailed;
            Rsc_AddSon(&cmd_list_node,&(cmd_rsc[loop].cmd_node),
                        sizeof(struct tagShellCmdRsc),RSC_SHELL_CMD,cmd_tab[loop].cmd);
        }
        else
        {
            result = false;
        }
    }
    return result;
}
//----ж��shell����------------------------------------------------------------
//����: ж���û�shell����
//����: cmd_tab�������ָ�룬�ɲ���shell_cmd_table�ķ�ʽ����,cmd_num����Ĵ�С
//����: �ɹ�ж�صĸ���
//-----------------------------------------------------------------------------
u32 sh_uninstall_cmd_bytab(struct tagShellCmdTab const *cmd_tab,u32 cmd_num)
{
    u32 loop;
    u32 result = 0;
    struct tagRscNode *tagShellCmdRsc;
    if( (cmd_tab == NULL) || (cmd_num == 0))
        return result;

    for(loop = 0; loop < cmd_num;loop++)
    {
        tagShellCmdRsc = Rsc_SearchSon(&cmd_list_node,cmd_tab[loop].cmd);
        if(NULL != tagShellCmdRsc)
        {
            Rsc_DelNode(tagShellCmdRsc);
            result ++;
        }

    }
    return result;
}
bool_t Sh_UninstallCmdByName(char *param)
{
    bool_t result = 0;
    char *cmdname;
    struct tagRscNode *tagShellCmdRsc;
    char *next_param;
    if(NULL == param)
    {
        return false;
    }
    cmdname = Sh_GetWord(param,&next_param);
    if( cmdname == NULL)
        return result;

    tagShellCmdRsc = Rsc_SearchSon(&cmd_list_node,cmdname);
    if(NULL != tagShellCmdRsc)
    {
        Rsc_DelNode(tagShellCmdRsc);
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}

//----��ȡĿ¼---------------------------------------------------------------
//����: ��buf����ȡһ����'\\'���н����������ĵ��ʣ�next���ڷ�����һ�������׵�ַ��
//      ���û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
//-----------------------------------------------------------------------------
char *Sh_GetItem(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    while(1)
    {
        if((buf[i] == '\\') || (buf[i] == 0))
        {
            if(buf[i] == 0)
                return buf;
            else
            {
                buf[i] = '\0';
                break;
            }
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if((buf[i]!='\\') && (buf[i] != '\n') && (buf[i] != '\r'))
        {
            *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}


//----��ȡ����---------------------------------------------------------------
//����: ��buf����ȡһ���ɿո���н����������ĵ��ʣ�next���ڷ�����һ�������׵�ַ��
//      ���û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
//-----------------------------------------------------------------------------
char *Sh_GetWord(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    if(buf == NULL)
        return NULL;
    while(1)
    {
        if((buf[i] == ' ') || (buf[i] == 0))
        {
            if(buf[i] == 0)
                return buf;
            else
            {
                buf[i] = '\0';
                break;
            }
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *next = NULL;
            else
                *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}

//----��ʾforѭ��ʱ��----------------------------------------------------------
//����: ��ʾһ��for(;;)ѭ����ִ��ʱ�䣬ѭ�������ֱ���8λ��16λ��32λ��
//����: ��
//����: ��
//-----------------------------------------------------------------------------
extern u32 g_u32CycleSpeed; //for(i=j;i>0;i--);ÿѭ��������*1.024

bool_t Sh_ShowForCycle(char *para)
{
    para = para;        //�����������澯
    printf("��forѭ��ִ��ʱ��: %dnS\r\n", g_u32CycleSpeed);

    return true;
}

//----��ʾ����Դ----------------------------------------------------------
//����: ������Ҫ����ʾ��Դ��������Դ�����֣�������Դ����Ϊ�գ�����ʾ"������Դ"
//����: �����ַ��������г��ĸ���Դ����ʡȴ���г�������µ���Դ
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ListRscSon(char *param)
{
    struct tagRscNode *rsc_tree;
    if(param == NULL)
        rsc_tree = Rsc_GetRoot();
    else
    {
        rsc_tree = Rsc_Search(Rsc_GetRoot(),param);
        if(rsc_tree == NULL)
        {
            printf("û�ҵ� %s ��Դ\r\n",param);
        }
    }
    __Sh_ShowSon(rsc_tree);
    return true;
}


//----��ʾ��Դ��----------------------------------------------------------
//����: ������Ҫ����ʾ��Դ��������Դ�����֣�������Դ����Ϊ�գ�����ʾ"������Դ"
//����: �����ַ��������г���������ʡȴ���г�ȫ����Դ
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ListRscTree(char *param)
{
    struct tagRscNode *rsc_tree;
    if(param == NULL)
        rsc_tree = Rsc_GetRoot();
    else
    {
        rsc_tree = Rsc_Search(Rsc_GetRoot(),param);
        if(rsc_tree == NULL)
        {
            printf("û�ҵ� %s ��Դ��\r\n",param);
        }
    }
    __Sh_ShowBranche(rsc_tree);
    return true;
}

//----��ʾ����Դ---------------------------------------------------------------
//����: ��ʾĳ��Դ�����ʼ��һ����Դ��֧������������Դ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __Sh_ShowSon(struct tagRscNode *branche)
{
    struct tagRscNode *current_node = branche;
    while(1)
    {
        current_node = Rsc_TraveSon(branche,current_node);
        if(current_node == NULL)
        {
            printf("\r\n");
            break;
        }
        if(current_node->name != NULL)
        {
            printf("  %s\r\n", current_node->name);
        }else
        {
            printf("  ������Դ\r\n");
        }
    }
}

//----��ʾ��Դ��---------------------------------------------------------------
//����: ��ʾĳ��Դ�����ʼ��һ����Դ��֧������������Դ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __Sh_ShowBranche(struct tagRscNode *branche)
{
    struct tagRscNode *current_node = branche;
    ucpu_t len;
    char neg[20];
    for(len = 0; len<20; len++)
        neg[len] = '-';
    while(1)
    {
        current_node = Rsc_TraveScion(branche,current_node);
        if(current_node == NULL)
        {
            printf("\r\n");
            break;
        }
        len = Rsc_GetClass(current_node);
        neg[len] = '\0';
        printf("%s", neg);
        neg[len] = '-';
        if(current_node->name != NULL)
        {
            printf("%s\r\n", current_node->name);
        }else
        {
            printf("������Դ\r\n");
        }
    }
}

//----��ʾ�ڴ�-----------------------------------------------------------------
//����: ��ʾĳ��ַ��ʼ��һ���ڴ棬ÿ����ʾ16����Ԫ��ֻ��ʾ�������޸�
//����: param������������������Ҫ�����������ÿո����
//          ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//          ����2:��ʾ�ĵ�Ԫ��
//          ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��8��������ֵ�����ش���
//����: true=������ʾ��false=����
//-----------------------------------------------------------------------------
bool_t Sh_ShowMemory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //��ȡ3������
    word_addr = Sh_GetWord(param,&next_param);
    word_un = Sh_GetWord(next_param,&next_param);
    word_ub = Sh_GetWord(next_param,&next_param);
    word_trail = Sh_GetWord(next_param,&next_param);
    if((word_addr == NULL)||(word_un == NULL)
            ||(word_ub == NULL)||(word_trail != NULL))
    {
        printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n>d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���\r\n");
        return false;
    }
    addr = strtol(word_addr, (char **)NULL, 0);
    unit_num = strtol(word_un, (char **)NULL, 0);
    unit_bytes = strtol(word_ub, (char **)NULL, 0);
#if (CN_BYTE_BITS == 8)  //�ֽ�λ��=8����������
    switch(unit_bytes)
    {
        case 1:
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%02x ", *(uint8_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;//ֻ�ܴ�ż����ʼ
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;//ֻ�ܴ�ż����ʼ
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 8:
            addr &= ~(ptu32_t)7;//ֻ�ܴ�ż����ʼ
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x", *(uint32_t*)addr);
                addr +=4;
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %32 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        default :
            printf("\r\n��������\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //�ֽ�λ��=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 4:
        {
            addr &= ~(ptu32_t)3;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                Djy_PutChar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //�ֽ�λ��=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                Djy_PutChar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    printf("\r\n");
    return true;
}

//----д���ڴ�-----------------------------------------------------------------
//����: д��ĳ��ַ��ʼ��һ���ڴ�
//����: param������������������Ҫ�����������ÿո����
//          ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//          ����2:��ʾ�ĵ�Ԫ��
//          ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��������ֵ�����ش���
//          ����4:����������
//����: true=������ʾ��false=����
//-----------------------------------------------------------------------------
bool_t Sh_FillMemory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = Sh_GetWord(param,&next_param);
    addr = strtol(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    unit_num = strtol(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    unit_bytes = strtol(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    data = strtol(word, (char **)NULL, 0);
#if (CN_BYTE_BITS == 8)  //�ֽ�λ��=8����������
    switch(unit_bytes)
    {
        case 1:
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(uint8_t*)addr = (u8)data;
                addr ++;
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr +=2;
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=4;
            }
            break;
        default :
            printf("\r\n��������\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //�ֽ�λ��=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr ++;
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=2;
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //�ֽ�λ��=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr ++;
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    return true;
}

//----�������---------------------------------------------------------------
//����: ����buf�������ַ�����һ�������Ƿ�keyword���������param�з���keyword
//      ���һ�������ҷǻس����е��ַ���ָ�룬��ָ��ʵ�ʰ�����һ�������в�����
//      ��ַ�����û�в�������param=NULL��
//����: buf�����������ַ���
//      keyword����ƥ�������
//      param�����������в���ָ��
//����: true = ����ƥ�䣬false=���ƥ��
//-----------------------------------------------------------------------------
bool_t Sh_CmdParser(const char *buf,const char *keyword,char **param)
{
    uint32_t i=0;
    char buf2[255];
    bool_t result;

    if(buf == NULL)
        return false;
    *param = NULL;
    //��ȡ��������������Ĵ������������������ո���±��i
    for(i=0;i<255;i++)
    {
        if((buf[i]==' ')||(buf[i] == '\0'))
            break;
    }
    memcpy(buf2,buf,i);
    buf2[i] = '\0';
    //�Ƚ�û�д�����������
    if(strcmp(buf2,keyword)!=0)
        result = false;
    else
        result = true;

    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *param = NULL;
            else
                *param = (char*)&buf[i];
            break;
        }
        i++;
    }
    return result;
}

//----��ʾ����-----------------------------------------------------------------
//����: ��ʾ������Ϣ
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Sh_CmdHelp(void)
{
    struct tagShellCmdRsc *current_cmd;
    printf("\r\n�йؾ����������ϸ��Ϣ��������help [������]\r\n");
    printf("\r\n");
    current_cmd = (struct tagShellCmdRsc *)&cmd_list_node;
    while(1)
    {
        current_cmd = (struct tagShellCmdRsc *)
                    Rsc_TraveScion(&cmd_list_node,(struct tagRscNode *)current_cmd);
        if(current_cmd == NULL)
        {
            break;
        }
        else
        {
            if(current_cmd->help_hint != NULL)
            {
                //todo: ǰ�����£���һ��%sӦ����%-32s�Ŷԣ���6��ͬ
                printf("%-24s%s",
                            current_cmd->cmd_node.name,current_cmd->help_hint);
            }
            else
            {
                printf("%-24sû���ṩ��Ҫ������Ϣ",current_cmd->cmd_node.name);
            }
        }
        printf("\r\n");
    }
}


//----��ʾ�ļ�ϵͳ�������-----------------------------------------------------
//����:
//����: �����ַ���������:
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ListCmdHelp(char *param)
{
    char *cmd,*next_param;
    bool_t result;
    struct tagShellCmdRsc *help_cmd;

    if(param == NULL)
    {
        Sh_CmdHelp();
        result = true;
    }
    else
    {
        cmd = Sh_GetWord(param,&next_param);
        help_cmd = (struct tagShellCmdRsc *)Rsc_SearchSon(&cmd_list_node,cmd);
        if(help_cmd != NULL)
        {
            if(help_cmd->help_detailed != NULL)
                printf("%s",help_cmd->help_detailed);
            else
                printf("û���ṩ��ϸ������Ϣ");
            printf("\r\n");
            result = true;
        }else
        {
            printf("��Ч������\r\n");
            result = false;
        }
    }
    return result;;
}
bool_t Sh_Ver(char *param)
{
    printf("\r\n%s\r\n", djyos_kernel_version);

    return true;
}

bool_t Sh_Date(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[256];
    char buf[8];
    int cmdlen;
    int res;

    nowtime = TM_Time(NULL);
    Tm_LocalTime_r(&nowtime,&dtm);

    printf("\r\n��ǰ���ڣ�%d/%02d/%02d %s",
            dtm.tm_year, dtm.tm_mon, dtm.tm_mday, g_cTmWdays[dtm.tm_wday]);
    printf("\r\n���������ڣ�");

    cmdlen = EasyScanf(NULL, command);
    if ((8<=cmdlen) && (cmdlen<=10))
    {
        //����Ϊ��2012/02/12����ʽ�������������ʱ��
        strcat(command, ",");    // �������ڷֿ����ں�ʱ��
        itoa(dtm.tm_hour, buf, 10);
        strcat(command, buf);
        strcat(command, ":");
        itoa(dtm.tm_min, buf, 10);
        strcat(command, buf);
        strcat(command, ":");
        itoa(dtm.tm_sec, buf, 10);
        strcat(command, buf);

        res = Tm_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_YEAR_ERROR:
            printf("��ݴ���");
            break;
        case EN_CLOCK_MON_ERROR:
            printf("�·ݴ���");
            break;
        case EN_CLOCK_DAY_ERROR:
            printf("���ڴ���");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}

bool_t Sh_Time(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[256];
    char time[16];
    char buf[8];
    int cmdlen;
    int res;

    nowtime = TM_Time(NULL);
    Tm_LocalTime_r(&nowtime,&dtm);

    printf("\r\n��ǰʱ�䣺%02d:%02d:%02d", dtm.tm_hour, dtm.tm_min, dtm.tm_sec);
    printf("\r\n������ʱ�䣺");

    cmdlen = EasyScanf(NULL, command);
    if ((5<=cmdlen) && (cmdlen<=8))
    {
        time[0] = '\0';
        strcpy(time, command);    // ����õ�ʱ���ַ������Ƶ�buf��
        //ʱ��Ϊ��03:32:1����ʽ������ǰ�������
        command[0] = '\0';
        itoa(dtm.tm_year, buf, 10);
        strcat(command, buf);
        strcat(command, "/");
        itoa(dtm.tm_mon, buf, 10);
        strcat(command, buf);
        strcat(command, "/");
        itoa(dtm.tm_mday, buf, 10);
        strcat(command, buf);
        strcat(command, ",");    // �������ڷֿ����ں�ʱ��
        strcat(command, time);

        res = Tm_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_HOUR_ERROR:
            printf("Сʱ����");
            break;
        case EN_CLOCK_MIN_ERROR:
            printf("���Ӵ���");
            break;
        case EN_CLOCK_SEC_ERROR:
            printf("���Ӵ���");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}

//----ת����������Ĵ�д��ĸΪСд----------------------------------------------
//����: ת��shell�����������"buf"�пո�ǰ�Ĵ�д��ĸΪСд��
//����: cmd��shell���������������"buf"(cmd)
//����: 0
//-----------------------------------------------------------------------------
#if 0
void capital_convert_lowercase(char *cmd)
{

    while(*cmd!='\0')
    {
           if(*cmd>='A'&&*cmd<='Z')
            *cmd=*cmd+('a'-'A');
            cmd++; //��һ���ַ�
         //�����ո�
         if(*cmd==' ')
             break;
    }
    return ;
}
#endif


//----�ж�':'�����Ƿ�������-----------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ������жϵ�һ������':'֮���Ƿ���
//      ����
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�ɹ�, ˵��':'֮��������,����û��':'
//      false=ʧ�ܣ�˵��':'֮��û������
//��ע: pathӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __judge_colon_is_data(char *path)
{
    uint32_t index;
    uint32_t  name_len;

    name_len = strlen(path);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')    //�����ַ�'\'
        {
            if(path[index+1] != '\0')
                return true;
            else
                return false;
        }
    }
    //�ַ�����û��ð��
    if(index == name_len)
        return true;
    return false;
}

//----ִ�п���̨����---------------------------------------------------
//����: ������ִ�п���̨����
//����: ��
//����: ��
//��ע: ��ʱ���ù�ϣ��ȣ����и��ӿ��ٵıȽϡ���������ıȽϣ�������ˡ�
//      �ǻ����������ˡ�
//---------------------------------------------------------------------
bool_t Sh_ExecCommand(char *buf)
{
    bool_t result = false;
    char *cmd,*next_param;
    struct tagShellCmdRsc *exec_cmd;
    //�������ƶ�ȡ255�ַ�����������ʾ�������С�
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("�����ַ���̫��\r\n");
    }
    //ת��buf�еĴ�д��ĸΪСд
//    capital_convert_lowercase(buf);

    if(!__judge_colon_is_data(buf))
    {
        if (fng_pCD_PTT != NULL)
        {
            if( ! fng_pCD_PTT(buf))
            {
                printf("���� s% ������",buf);
            }
        }
        else
        {
            printf("not file system");
        }
        return true;
    }
    cmd = Sh_GetWord(buf,&next_param);
    strlwr(cmd);
    exec_cmd = (struct tagShellCmdRsc *)Rsc_SearchSon(&cmd_list_node,cmd);
    if(exec_cmd != NULL)
    {
        result = exec_cmd->shell_cmd_func(next_param);
    }else
    {
        printf("\r\n�޴�����\r\n");
        result = true;
    }
    return result;
}
//----����̨������-----------------------------------------------------------
//����: ����console������ַ�����console����س���ʱ��ִ�����һ������ó���
//      255�ַ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
ptu32_t Sh_Service(void)
{
    char command[CN_SHELL_CMD_LIMIT+1];
    while(1)
    {
        EasyScanf(NULL,command);
        if(strlen(command) != 0)
            Sh_ExecCommand(command);  //ִ������
        if ((fng_pPrintWorkPath != NULL))
            fng_pPrintWorkPath( );
        printf(">");
    }
}

