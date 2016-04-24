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
// =============================================================================
// Copyright (C) 2012-2020 ��������������޹�˾ All Rights Reserved
// ģ������: NetDevHal.c:
// ģ��汾: V1.00
// ������Ա: Zqf
// ����ʱ��: 9:45:16 AM/Sep 9, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#ifndef __NETDEV_H
#define __NETDEV_H
#include "stdint.h"
#include "stddef.h"

#include "os.h"

#include "pkg.h"

enum _EN_LINK_INTERFACE_TYPE
{
    EN_LINK_INTERFACE_ETHERNET = 0,  //��̫����
    EN_LINK_INTERFACE_SLIP,          //����
    EN_LINK_INTERFACE_LOOP,          //�ػ�
    EN_LINK_INTERFACE_RAW,           //����·�㣬ֱ��ʹ��IP������
    EN_LINK_INTERFACE_LAST,
};
enum _EN_LINK_INTERFACE_STATE        //����״̬
{
    EN_LINK_INTERFACE_STATE_OK = 0,     //����������������
    EN_LINK_INTERFACE_STATE_ERR,        //����������
    EN_LINK_INTERFACE_STATE_STOP,       //����ͣ�ã������շ�����
    EN_LINK_INTERFACE_STATE_RESET,      //����
};

typedef struct __tagNetDev tagNetDev;

//netdev snd module function
typedef bool_t (*fnIfSend)(tagNetDev *netdev,tagNetPkg *pkglst,u32 netdevtask);

typedef struct _tagNetDevPara   //�������ʱ�ṩ�Ĳ���,����para��������Ӧ������
{
    char                           *name;    //��������
    u8                             iftype;   //��������
    u8                             mac[6];   //mac��ַ
    fnIfSend                       ifsend;   //�����ķ��ͺ������������������з���
                                             //���ͺ����Լ����ƺ�������������������Դ����
    u32                            devfunc;  //����֧�ֵĹ���
    u16                            pkglen;   //����Ͱ�����
    u16                            linklen;  //��·�����ݰ�����
    ptu32_t                        private;  //ÿ�����������ɱ�Ǻ�˽������
}tagNetDevPara;

struct __tagNetDev
{
    struct __tagNetDev             *pre;     //ǰһ������
    struct __tagNetDev             *nxt;     //��һ������
    char                           *name;    //��������
    u8                             iftype;   //��������
    u8                             ifstate;  //����״̬
    u8                             mac[6];   //mac��ַ
    fnIfSend                       ifsend;   //�����ķ��ͺ�����һ����ԣ�ִ������������

    u32                            devfunc;  //����֧�ֵĹ���,��TCP UDPУ��ȣ���NetCDef.h�ж���
    u16                            pkglen;   //����㷢�͵�������ݰ�����
    u16                            linklen;  //��·�����ݰ�����
    ptu32_t                        private;  //Ŀǰδʹ�ã��ӿڵ��Լ�����

};


//�������ú���
tagNetDev  *NetDev_AddDev(tagNetDevPara *para);
bool_t NetDev_DelDev(tagNetDev *netdev);
ptu32_t NetDev_GetDevPrivate(tagNetDev *netdev);

//Э��ջ���øú����������ݵ�����
bool_t NetDev_DevSend(tagNetDev *netdev,tagNetPkg *pkglst,u32 netdevtask);
//�����������øú����������ݰ���Э��ջ
bool_t NetDev_PostPkg(tagNetDev *netdev, tagNetPkg *pkglst);

u8  *NetDev_GetDevMac(tagNetDev *netdev);       //��·��ʹ��
u32 NetDev_GetDevFun(tagNetDev *netdev);        //Э��ջʹ��

typedef struct __tagNetDevPkgMsg
{
    struct __tagNetDevPkgMsg *nxt;    //the nxt node
    tagNetDev                *dev;    //the function the netdev has done
    tagNetPkg                *pkglst; //the rcv pkg of the card
}tagNetDevPkgMsg;


#endif
