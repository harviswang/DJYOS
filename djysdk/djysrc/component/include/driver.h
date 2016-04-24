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
//����ģ��:���豸����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ���豸������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "errno.h"
#include "rsc.h"
#include "multiplex.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CN_DEV_NAME_LIMIT   255     //�豸�����Ȳ��ܳ���255�ַ�
typedef struct tagDjyDevice * tagDevHandle;
struct  tagDjyDevice;
struct  tagRscNode;

#define O_RDONLY    1   //ֻ��ģʽ
#define O_WRONLY    2   //ֻдģʽ
#define O_RDWR      3   //��дģʽ

//���豸ģ�������룬��enum������enum_drv_no_error��ʼ������1.
enum _DRV_ERROR_CODE_
{
    EN_DRV_NO_ERROR = CN_DRV_NO_ERROR,  //û�д���
    EN_DRV_HANDLE_ERROR,                //�������
    EN_DRV_HOMONYMY,                    //�豸��������
    EN_DRV_DEV_DEL,                   //ɾ���豸ʧ��
};

//�����豸��������
#define CN_DRV_CTRL_START           0   //�����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define CN_DRV_CTRL_SHUTDOWN        1   //�رյ�Դ,�ϵ�ǰ��Ҫ�ض�������豸��Ҫ
#define CN_DRV_CTRL_SLEEP           2   //�豸����͹���״̬,���ڵ�Դ����
#define CN_DRV_CTRL_RESUME          3   //�豸�ָ�,�ӵ͹���״̬����
#define CN_DRV_CTRL_CHECK           4   //����豸״̬
#define CN_DRV_CTRL_SET_FUNC        5   //����������/���/����ʱ�ص�����
#define CN_DRV_CTRL_SET_LEVEL       6   //�����豸�¼�����д����ˮƽ

#define CN_DRV_CTRL_USER            0x80   //�Զ�����豸���������0x100��ʼ
//�û������������������0x80��ʼ������:
//#define CN_XXX_CTRL_MYCMD1        (CN_DRV_CTRL_USER+0)
//#define CN_XXX_CTRL_MYCMD2        (CN_DRV_CTRL_USER+1)

//����dev_Write���������(block_option����)���ﵽ�����󣬺������ء�
#define CN_BLOCK_BUFFER         0       //dev_Write����������Ƿ��͵�������
#define CN_BLOCK_COMPLETE       1       //dev_Write����������Ǵ������

//����ֵ:�ɹ�д����ֽ���������豸�л���������д�����������㡣����һ�������豸��
//����ֵ��ʾ��������ʱ�ɹ�д�뵽�豸��������������������ȷ���Ƿ��Ѿ���������
//�����ȥ�ˡ�
typedef u32 (*devWriteFunc)(ptu32_t PrivateTag,u8 *buf,
                         u32 len,u32 offset,bool_t BlockOption,u32 timeout);
//����ֵ:�ɹ���ȡ���ֽ���
typedef u32 (*devReadFunc) (ptu32_t PrivateTag,u8 *buf,
                                     u32 len,u32 offset,u32 timeout);
//����ֵ:�յ���֧�ֵ��������-1��0��ʾ�ɹ�ִ�У���������ֵ�ĺ����Զ�
typedef u32 (*devCtrlFunc) (ptu32_t PrivateTag,u32 cmd,
                                     ptu32_t data1,ptu32_t data2);
//����ֵ:true=�ɹ�ִ�У�false=ʧ�ܡ�
//SendsingBit��Ϊ0��ʾ��MultiplexSets��ɾ�����豸
typedef bool_t (*devMultiplexAddFunc) (ptu32_t PrivateTag,
                                       struct tagMultiplexSetsCB *MultiplexSets,
                                       u32 DevAlias,
                                       u32 SensingBit);
#define DEV_READ_MUTEX_SYS      1       //bit0,1=���豸�Ļ�������ϵͳ����ġ�
#define DEV_WRITE_MUTEX_SYS     2       //bit1,1=д�豸�Ļ�������ϵͳ����ġ�
//ע�⣬���ṩdev_open��dev_close���������������Щ�豸��Ҫ��openʱ����dev_open
//���һЩ���ù����ģ�����dev_Ctrl������CN_DRV_CTRL_START���ͬ��Ҳ���ṩ
//dev_close�ӿ�
struct tagDjyDevice
{
    struct  tagRscNode Node;
    devWriteFunc  dWrite;
    devReadFunc   dRead;
    devCtrlFunc   dCtrl;
    devMultiplexAddFunc dMultiplexAdd;  //���豸driver��֧�ֶ�·���ã����ÿա�
    struct tagMutexLCB *dReadMutex;     //������,�����豸��ռʽ������
    struct tagMutexLCB *dWriteMutex;    //������,�����豸��ռʽд����
    u32    MutexFlag;                   //��־���������û��ṩ��,����ϵͳ����ġ�
    u32 delete_lock;                    //ɾ���������� 0 ��ʾ���豸����ɾ��
    ptu32_t PrivateTag;                 //���豸���е�����
};

ptu32_t ModuleInstall_Driver(ptu32_t para);
tagDevHandle Driver_DeviceCreate(  tagDevHandle         ParentDevice,
                                char                    *name,
                                struct tagMutexLCB      *dReadMutex,
                                struct tagMutexLCB      *dWriteMutex,
                                devWriteFunc            WriteFunc ,
                                devReadFunc             ReadFunc,
                                devCtrlFunc             Ctrl ,
                                devMultiplexAddFunc     MultiplexAdd,
                                ptu32_t                 tag);
bool_t Driver_LockDevice(u32 DevAlias);
bool_t Driver_UnLockDevice(u32 DevAlias);
bool_t Driver_DeleteDevice(tagDevHandle handle);
u32 Driver_FindDevice(char * name);
u32 Driver_FindScionDevice(tagDevHandle ancestor,
                                    char * scion_name);
tagDevHandle Driver_OpenDevice(char *name,u32 flags,u32 timeout);
tagDevHandle Driver_OpenScionDevice(tagDevHandle ancestor,
                                 char *scion_name,u32 flags, u32 timeout);
tagDevHandle Driver_OpenDeviceAlias(u32 DevAlias,u32 flags,u32 timeout);
bool_t Driver_CloseDevice(tagDevHandle handle);
u32 Driver_ReadDevice(tagDevHandle handle,u8 *buf,u32 len,u32 offset,u32 timeout);
u32 Driver_WriteDevice(tagDevHandle handle,u8 *buf,
                  u32 len,u32 offset,bool_t BlockOption,u32 timeout);
u32 Driver_CtrlDevice(tagDevHandle handle,u32 cmd,ptu32_t data1,ptu32_t data2);
u32 Driver_MultiplexCtrl(u32 DevAlias,u32 *ReadLevel,u32 *WriteLevel);
u32 Driver_MultiplexAdd(struct tagMultiplexSetsCB *MultiplexSets,
                        u32 *DevAliases,u32 num,u32 SensingBit);
void Driver_MultiplexDel(struct tagMultiplexSetsCB *MultiplexSets,
                         u32 *DevAliases,u32 num);

#ifdef __cplusplus
}
#endif
#endif //__DRIVER_H___
