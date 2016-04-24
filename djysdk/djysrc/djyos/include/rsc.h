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
//����ģ��: ��Դ����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��������ӡ��ƶ���ɾ����������Դ���ķ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __rsc_h__
#define __rsc_h__

#ifdef __cplusplus
extern "C" {
#endif
#define CN_RSC_NAME_LIMIT   255

struct tagRscNode
{
    struct tagRscNode *next,*previous,*parent,*child;
    u16  RscSize;       //����node�����ݽṹ�ĳߴ磬���ڵ���
    u8  rscv;
    u8  RscType;        //��Դ����
    char *name;         //��Դ��,�������ļ�ϵͳΪ�ļ�����Ŀ¼��,�����豸���豸��
                        //����gui���Ǵ�����.
};

//������Դ����
#define RSC_RSCNODE             0       //���ڵ�
#define RSC_RSCV_TYPE           1       //��������
#define RSC_DEVICE              2       //�豸
#define RSC_SEMP                3       //�ź���
#define RSC_MUTEX               4       //������
#define RSC_MSGQ                5       //��Ϣ����
#define RSC_MEMPOOL             6       //�ڴ��
#define RSC_SOFT_TIMER          7       //��ʱ��
#define RSC_SOCKET              8       //socket
#define RSC_NET_CARD            9        //����
#define RSC_SPIBUS              10       //spi����
#define RSC_SPI_DEVICE          11       //spi����
#define RSC_IICBUS              12       //i2c����
#define RSC_IIC_DEVICE          13       //i2c����
#define RSC_STDIN_OUT           14       //��׼���������豸
#define RSC_SHELL_CMD           15       //shell ����
#define RSC_DISPLAY             16       //��ʾ��
#define RSC_GKWIN               17       //�ں˴���(��ʾ)
#define RSC_FILE                18       //�ļ����ļ���
#define RSC_FILE_SYS            19       //�ļ�ϵͳ(ָ�����ļ�ϵͳ)
#define RSC_PTT                 20       //�ļ�ϵͳ����
#define RSC_FFS_FLASH_CHIP      21       //����flash�ļ�ϵͳ��flashоƬ
#define RSC_EFS_NOR_CHIP        22       //����easy nor�ļ�ϵͳ��flashоƬ
#define RSC_CHARSET             23       //�ַ���
#define RSC_FONT                24       //����


#define RSCTYPE_USER_BASE       0x80
#define RSCTYPE_USER            (RSCTYPE_USER_BASE + 0)

ptu32_t Rsc1_ModuleInit(ptu32_t para);
ptu32_t Rsc2_ModuleInit(ptu32_t para);
void Rsc_CleanNode(struct tagRscNode *node);
struct  tagRscNode * Rsc_AddTree(struct  tagRscNode *node,u16 size,u8 RscType,char *name);
struct  tagRscNode * Rsc_AddToPrevious(struct  tagRscNode *node,struct  tagRscNode *new_node,
                                u16 size,u8 RscType,char *name);
struct  tagRscNode * Rsc_AddToNext(struct  tagRscNode *node,struct  tagRscNode *new_node,
                                u16 size,u8 RscType,char *name);
struct  tagRscNode * Rsc_AddSon(struct  tagRscNode *parent_node,
                               struct  tagRscNode *new_node,
                               u16 size,u8 RscType,char *name);
struct  tagRscNode * Rsc_AddHeadSon(struct  tagRscNode *parent_node,
                                    struct  tagRscNode *new_node,
                                    u16 size,u8 RscType,char *name);
bool_t Rsc_DisplaceNode(struct tagRscNode *oldnode,struct tagRscNode *newnode);
struct  tagRscNode * Rsc_DelBranch(struct  tagRscNode *branch);
struct  tagRscNode * Rsc_DelNode(struct  tagRscNode *node);
bool_t Rsc_MoveToTree(struct tagRscNode *parent,struct  tagRscNode *node);
bool_t Rsc_MoveToLast(struct  tagRscNode *node);
bool_t Rsc_MoveToHead(struct  tagRscNode *node);
bool_t Rsc_MoveToNext(struct  tagRscNode *elder,struct  tagRscNode *node);
bool_t Rsc_MoveToPrevious(struct  tagRscNode *lesser,struct  tagRscNode *node);
bool_t Rsc_RoundPrevious(struct  tagRscNode *parent);
bool_t Rsc_RoundNext(struct  tagRscNode *parent);
bool_t Rsc_RenameNode(struct tagRscNode *node,char *new_name);
struct  tagRscNode *Rsc_GetTree(struct  tagRscNode *scion_node);
struct  tagRscNode *Rsc_SearchTree(char *tree_name);
struct  tagRscNode *Rsc_GetRoot(void);
char *Rsc_GetName(struct  tagRscNode *node);
u16 Rsc_GetNodeSize(struct  tagRscNode *node);
u8 Rsc_GetNodeType(struct  tagRscNode *node);
struct  tagRscNode *Rsc_GetParent(struct  tagRscNode *son_node);
struct  tagRscNode *Rsc_GetSon(struct  tagRscNode *parent_node);
struct  tagRscNode *Rsc_GetPrevious(struct  tagRscNode *next_node);
struct  tagRscNode *Rsc_GetNext(struct  tagRscNode *previous_node);
struct  tagRscNode *Rsc_GetHead(struct  tagRscNode *rnode);
struct  tagRscNode *Rsc_GetTwig(struct  tagRscNode *parent_node);
u32 Rsc_GetClass(struct  tagRscNode *node);
struct  tagRscNode *Rsc_TraveSon(struct  tagRscNode *parent_node,
                                  struct  tagRscNode *current_son);
struct  tagRscNode *Rsc_TraveScion(struct  tagRscNode *parent_node,
                                 struct  tagRscNode *current_node);
struct  tagRscNode *Rsc_SearchSibling(struct  tagRscNode *layer,char *name);
struct  tagRscNode *Rsc_SearchSon(struct  tagRscNode *parent,char *name);
struct  tagRscNode *Rsc_SearchScion(struct  tagRscNode *parent_node,char *name);
struct  tagRscNode *Rsc_Search(struct  tagRscNode *parent_node,char *path);
u32 Rsc_NodeSequencing(struct tagRscNode *node);
bool_t Rsc_IsLast(struct tagRscNode *node);
bool_t Rsc_IsHead(struct tagRscNode *node);

#ifdef __cplusplus
}
#endif

#endif //__rsc_h__

