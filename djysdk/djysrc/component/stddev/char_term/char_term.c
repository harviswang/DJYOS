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
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "version.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "multiplex.h"
#include "driver.h"
#include "shell.h"
#include "stddev.h"
#include "systime.h"
#include "djyos.h"
#include "char_term.h"

static s32 s_s32CharTermId;
static u32 s_u32Stdinout = 0;
struct tagSemaphoreLCB *g_ptSempCharTermHdl;
tpInputMsgQ tg_pCharInputMsgQ;    //  ��׼�����豸����Ϣ����

ptu32_t Term_Scan(void)
{
   struct tagCharTermineralMsg msg={0,0};
   struct tagMultiplexSetsCB *CharTermSets;
   tagDevHandle StdinoutHdl;
   u32 ReadLevel = 16;

   Driver_MultiplexCtrl(s_u32Stdinout,&ReadLevel,NULL);
   CharTermSets  = Multiplex_Creat(1);
   Driver_MultiplexAdd(CharTermSets,&s_u32Stdinout,1,CN_MULTIPLEX_SENSINGBIT_READ);
   StdinoutHdl = Driver_OpenDeviceAlias(s_u32Stdinout,O_RDONLY,0);

   while(1)
   {
       Multiplex_Wait(CharTermSets,NULL,100*mS);
       if(NULL != StdinoutHdl)
       {
           //CN_CHAR_BUF_LIMIT
           msg.num = Driver_ReadDevice(StdinoutHdl,msg.input_char,
                                       CN_CHAR_BUF_LIMIT,0,0);
//           Driver_CloseDevice(StdinoutHdl);
           if(msg.num != 0)
           {
               msg.time = DjyGetTimeTick();
               Stddev_InputMsg(s_s32CharTermId,(u8*)&msg,sizeof(msg));
           }
       }
   }
    return 0;
}

//----�ն�ģ���ʼ������-------------------------------------------------------
//����: ��ʼ������ģ��
//����: para,�������ʹ�õ��豸������紮��
//����: �ն��豸��id(ע:�ⲻ�Ƿ��豸),�����򷵻�-1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_CharTerm(ptu32_t para)
{
    u16 term_scan_evtt;
    if(para == 0)
        return (ptu32_t)-1;
    s_s32CharTermId = Stddev_InstallDevice("char_term", EN_CHAR_TERMINERAL,NULL);
    if(s_s32CharTermId == -1)
        goto exit_install_device;
    g_ptSempCharTermHdl = Lock_SempCreate(1, 1, CN_SEMP_BLOCK_FIFO,"char term semp");
    s_u32Stdinout = (u32)para;
    if (g_ptSempCharTermHdl == NULL)
        goto exit_create_semp;
    tg_pCharInputMsgQ = Stddev_CreatInputMsgQ(10,"StdInDev");
    if(tg_pCharInputMsgQ == NULL)
        goto exit_create_msgq;
    term_scan_evtt = Djy_EvttRegist(EN_CORRELATIVE,          //�������¼�
                                CN_PRIO_REAL,       //Ĭ�����ȼ�
                                0,                  //�̱߳���������������Ч
                                0,                  //�߳����ޣ���������Ч
//                                1,                  //�������г�������
                                Term_Scan,          //��ں���
                                NULL,               //��ϵͳ����ջ
                                1024,               //ջ�ߴ�
                                "Term_Scan"         //�¼�������
                                );
    if(term_scan_evtt == CN_EVTT_ID_INVALID)
    {
        goto exit_install_evtt;
    }
    if(Djy_EventPop(term_scan_evtt,NULL,0,para,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        goto exit_pop_event;
    }
    Stddev_SetFocus("char_term", tg_pCharInputMsgQ);

    printf("\r\n��ӭʹ�ö����߲���ϵͳ\r\n");
    printf("%s\r\n",djyos_kernel_version);
    printf("����ʱ�䣺%s %s\r\n", __DATE__, __TIME__);
    if ((fng_pPrintWorkPath != NULL))
    {
        fng_pPrintWorkPath( );
    }
    printf(">");

    return (ptu32_t)s_s32CharTermId;

exit_pop_event:
    Djy_EvttUnregist(term_scan_evtt);
exit_install_evtt:
    Stddev_DeleteInputMsgQ(tg_pCharInputMsgQ);
exit_create_msgq:
    Lock_SempDelete(g_ptSempCharTermHdl);
exit_create_semp:
    Stddev_UnInstallDevice("char_term");
exit_install_device:
    return (ptu32_t)-1;
}

//static u32 u32s_ptimes;
s32 Djy_GetChar(void)
{
    static struct tagInputDeviceMsg input_msg;
    struct tagCharTermineralMsg *char_msg;
    static u32 offset=0;
    s32 ch;

    if(offset == 0)
    {
        Stddev_ReadMsg(tg_pCharInputMsgQ,&input_msg,CN_TIMEOUT_FOREVER);
    }
    char_msg = &(input_msg.input_data.char_termineral);
    ch = char_msg->input_char[offset++];
    if(offset >= char_msg->num)
    {
        offset = 0;
    }
    return ch;
}

s32 Djy_Gets(char buf[])
{
//    char ch;
//
//    buf[0] = '\0';
//    do
//    {
//        ch = (char)Djy_GetChar();
//        if ((ch != '\r') && (ch != '\n'))
//        {
//            buf++ = ch;
//        }
//        else
//        {
//            buf++ = '\0';
//            return 1;
//        }
//    } while (1);
//
    return 0;
}

//----��ʽ����---------------------------------------------------------
//����: ����ʽ�����ַ�������C�⺯�����ƣ���������������ࡣ
//����: const char *fmt, ...
//����:
//ע�⣺��ǰ�����ڻ�ȡ�ַ�������������ȫδʵ�֣�
//-----------------------------------------------------------------------------
s32 EasyScanf(const char *format, char *buf)
{
    char ch;
    u8 chars = 0;

    Lock_SempPend(g_ptSempCharTermHdl, CN_TIMEOUT_FOREVER);
//  u32s_ptimes = Djy_GetEvttPopTimes(Djy_MyEvttId());
//  Stddev_SetFocus("char_term",Djy_MyEvttId());
    while (1)
    {
        ch = (char)Djy_GetChar();
        if(ch == 0x1B ) // ����'ESC'����ֱ�ӽ���
        {
            continue;
        }
        if ((ch == '\r') || (ch == '\n'))
        {
            buf[chars] = '\0';
            printf("\r\n");
            break;
        }
        else
        {
            if (ch == 8)    // Backspace
            {
                if(chars > 0)
                {
                    chars --;
                    printf("\b \b");
                }
            }
            else
            {
                if(chars < 255)  //������32�ֽڣ������ı�����
                {
                    Djy_PutChar(ch);
                    buf[chars] = ch;
                    chars++;
                }
            }

        }
    }
    Lock_SempPost(g_ptSempCharTermHdl);
    return chars;
}

//-----------------------------------------------------------------------------
//����: ���һ���ַ����ն��豸��
//����: �ַ�c
//����:
//ע��?ע��?�����Ҫ�����ն˷����߳�?�������������
//-----------------------------------------------------------------------------
void Djy_PutChar(char c)
{
    tagDevHandle StdinoutHdl;
    StdinoutHdl = Driver_OpenDeviceAlias(s_u32Stdinout,O_WRONLY,1000*mS);
    if(NULL != StdinoutHdl)
    {
        Driver_WriteDevice(StdinoutHdl,(u8*)&c,1,0,
                       CN_BLOCK_BUFFER,CN_TIMEOUT_FOREVER);
        Driver_CloseDevice(StdinoutHdl);
    }
}

//-----------------------------------------------------------------------------
//����: ����ַ������ն��豸��
//����: �ַ���
//����:
//ע�⣺ע�⣬����������Ƿ����ն˷����̣߳�������������������Ҫ�����۲�����
//      ����ʹ�ã������ϵ���ƭ
//-----------------------------------------------------------------------------
void Djy_Puts(char *v_string)
{
    tagDevHandle StdinoutHdl;
    StdinoutHdl = Driver_OpenDeviceAlias(s_u32Stdinout,O_WRONLY,1000*mS);
    if(NULL != StdinoutHdl)
    {
        Driver_WriteDevice(StdinoutHdl,(u8*)v_string,\
                        strlen(v_string),0,CN_BLOCK_BUFFER,CN_TIMEOUT_FOREVER);
        Driver_CloseDevice(StdinoutHdl);
    }
}

