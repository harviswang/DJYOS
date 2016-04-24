//----------------------------------------------------
//Copyright (C), 2004-2011,  ������.
//��Ȩ���� (C), 2004-2011,   ������.
//����ģ��:windowsģ���ַ�����ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: ��ȡwindows�����������룬��Ϊdjyos���ַ���������豸����djyos�����
//          �ն˵��ַ������windows��cmd����
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-10-09
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "driver.h"
#include "ring.h"
#include "cpu_peri.h"
#include "windows.h"
#include "stdio.h"
#include "djyos.h"
#include "int.h"

#define cmd_buf_len  1024

static struct tagMultiplexObjectCB * pMultiplexCmd;      //��·����Ŀ�����ͷָ��
static u32 MplCmdStatus;
HANDLE win32_scanf;

char cmd_ptcl_recv_buf[cmd_buf_len];
//static tagDevHandle pg_cmd_hdl;
struct tagRingBuf recv_ring_buf;           //���ν��ջ�����.
char scanf_buf[cmd_buf_len-2];
HANDLE hStdOut, hNewScreenBuffer,hStdIn;

u16 u16g_evtt_right_write;      //�豸���ֱ�д�󵯳����¼�����

u32 cmd_int(ufast_t uart_int_line)
{
    u32 len;
    len = strlen(scanf_buf);
    //copy����Ӳ����������Э�黺����
    Ring_Write(&recv_ring_buf, (u8*)scanf_buf,len);
    MplCmdStatus |= CN_MULTIPLEX_SENSINGBIT_READ;
    Multiplex_Set(pMultiplexCmd,MplCmdStatus);
    return 0;
}

u32 WINAPI win32_scanf_pro( LPVOID lpParameter )
{
    u32 num;
    while(1)
    {
        ReadConsoleA( hStdIn, // screen buffer to write to
                        &scanf_buf,        // buffer to copy from
                        256,     // col-row size of chiBuffer
                        (PDWORD)&num,    // top left src cell in chiBuffer
                        NULL);  // dest. screen buffer rectangle
        scanf_buf[num] = '\0';
        Int_TapLine(cn_int_line_cmd);
/*        do
        {
//            scanf("%c",&ch);
            r =ReadConsoleA( hStdIn, // screen buffer to write to
                            &scanf_buf,        // buffer to copy from
                            256,     // col-row size of chiBuffer
                            &num,    // top left src cell in chiBuffer
                            NULL);  // dest. screen buffer rectangle
            if(num != 0)
            {
                scanf_buf[len] = ch;
                len++;
            }
            if(r == 0)
                r = GetLastError();
        }while(ch != '\r');
        scanf_buf[len] = '\0';
        len = 0;
        Int_TapLine(cn_int_line_cmd);
*/    }
}

//----��ʼ���������豸---------------------------------------------------------
//����: ����
//����: ��
//����: ������
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Cmd(ptu32_t para)
{

    tagDevHandle cmd_dev;


    pMultiplexCmd = NULL;
    MplCmdStatus = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�
    Ring_Init(  &recv_ring_buf,
                (u8 *)cmd_ptcl_recv_buf,
                cmd_buf_len);

    //���½���windows �ն������豸
    cmd_dev = Driver_DeviceCreate( NULL,"windows_cmd",
                                NULL,NULL,
                               (devWriteFunc) cmd_DriverWrite,
                               (devReadFunc ) cmd_DriverRead,
                               (devCtrlFunc ) cmd_DriverCtrl,
                               cmd_MultiplexAdd,
                               0
                               );
    if(cmd_dev == NULL)
        goto exit_from_add_device;
//    pg_cmd_hdl = Driver_OpenDevice("windows_cmd",O_RDWR,0);      //�����־��
    Int_IsrConnect(cn_int_line_cmd,cmd_int);
    Int_SettoAsynSignal(cn_int_line_cmd);
    Int_RestoreAsynLine(cn_int_line_cmd);
    win32_scanf = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)win32_scanf_pro, 0,
                                     0, NULL );
    SetThreadAffinityMask( win32_scanf, 1 );

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
/*    hNewScreenBuffer = CreateConsoleScreenBuffer(
                       GENERIC_READ | GENERIC_WRITE,          // read/write access
                       FILE_SHARE_READ | FILE_SHARE_WRITE,    // shared
                       NULL,                    // default security attributes
                       CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
                       NULL);                   // reserved; must be NULL
    SetConsoleActiveScreenBuffer(hNewScreenBuffer);
*/
    return 1;

exit_from_add_device:
    return 0;
}

bool_t cmd_MultiplexAdd (ptu32_t PrivateTag,
                         struct tagMultiplexSetsCB *MultiplexSets,
                         u32 DevAlias,
                         u32 SensingBit)
{
    bool_t result=false;

    result = Multiplex_AddObject(MultiplexSets,
                            &pMultiplexCmd,
                            MplCmdStatus,
                            (ptu32_t)DevAlias,SensingBit);
    return result;
}


//----uart�豸����д����-------------------------------------------------------
//����: ������д��uart�豸,һ����uart��ʹ���ߵ���.
//      1.������д�봮��serial_DCB_t�Ļ��η��ͻ�������.
//      2.�������Ӳ������,�������д��Ӳ�����ͻ�����,����������,���򲻲���
//����: uart_dev,���������豸������ָ��.
//      src_buf,����Դָ��
//      len,Ҫ���͵����г���
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
ptu32_t cmd_DriverWrite(ptu32_t PrivateTag,u8 *buf,
                         u32 len,u32 offset,bool_t BlockOption,u32 timeout)
{
    u32 sended;

    if((len==0) || (buf == 0))
        return 0;

    sended = WriteConsole(   hStdOut, // screen buffer to write to
                    buf,        // buffer to copy from
                    len,     // col-row size of chiBuffer
                    NULL,    // top left src cell in chiBuffer
                    NULL);  // dest. screen buffer rectangle
//    for(sended = 0; sended < len; sended++)
//        putchar(buf[sended]);
    return sended;
}

//----�����豸���ֶ�����------------------------------------------------------
//����: ���ֶ������ϲ��豸,��ȡ���ڽ��ջ��λ�����������
//����: uart_dev,���������豸������ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
//----------------------------------------------------------------------------
ptu32_t cmd_DriverRead(ptu32_t PrivateTag,u8 *buf,
                                     u32 len,u32 offset,u32 timeout)
{
    u32 result;
    if(buf == NULL)
        return 0;
//    Lock_SempPend(&tg_cmd_buf_semp,timeout);
    result = Ring_Read(&recv_ring_buf,(u8*)buf,len);
    MplCmdStatus &= (~CN_MULTIPLEX_SENSINGBIT_READ);
    Multiplex_Set(pMultiplexCmd,MplCmdStatus);
    return result;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���
//����: uart_handle,�������Ĵ����豸ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
ptu32_t cmd_DriverCtrl(ptu32_t PrivateTag,u32 cmd,
                   u32 data1,u32 data2)
{

    switch(cmd)
    {
        case cn_cmd_connect_recv_evtt:
            u16g_evtt_right_write = (uint16_t)data1;
            break;
        case cn_cmd_disconnect_recv_evtt:
            u16g_evtt_right_write = CN_EVTT_ID_INVALID;
            break;
        default: break;
    }
    return 0;
}
u32 Uart_SendServiceDirectly(char *str)
{
    return 0;
}


