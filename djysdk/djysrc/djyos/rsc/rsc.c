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
//�ļ�����: ��������ӡ��ƶ���ɾ����������Դ�ڵ�ķ���
//����˵��:
//�޶���ʷ:
//4. ����:20130220
//   ����:  ������.
//   �°汾�ţ�v1.3.0
//   �޸�˵��:
//      1.������rsc�������ܵ������������⣬bug������ë���ύ�ġ�
//3. ����:20090518
//   ����:  ������.
//   �°汾�ţ�v1.2.0
//   �޸�˵��:
//      1.�ڸ����ϣ��ֿ�����������Դ����������Ҳ��Ӧ�ص�����
//      2.����rsc_get_root������ԭrsc_get_root����Ϊrsc_get_tree���Ҳ��ټ�ְ
//      3.������rsc_get_root��ȡ��1.1.0�汾��"�޸�3"
//      4.��ȫ��NULL�������
//2. ����:20090131
//   ����:  ������.
//   �°汾�ţ�v1.1.0
//   �޸�˵��:
//      1.���˸��ܸ��ڵ㣬�����ģ���ʼ������
//      2.���ź��������������ʰ�ȫ
//      3.�Ľ��˱���������������NULL���������������
//      4.������һЩbug
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "int.h"
#include "rsc.h"
#include "lock.h"
//-------------��Դ���㷨˵��----------------------
//��ͬ���ڵ�����˫��ѭ������
//ÿ���ڵ��childָ��ֱ��ָ��Ľڵ��Ϊ�ýڵ�����ڶ��еĶ���ͷ������previous�ڵ�
//�Ƕ���β��
//���нڵ��Parentָ���ָ�򸸽ڵ�

//�ر���ʾ����Դ���еĲ������ʰ�ȫ����Դ����ģ�鸺����Դ�ڵ�����Ĳ�������
//          ������ʹ���߸���
//��������޸Ķ���ʹ��ԭ�Ӳ�����֤������걸��
//������Ķ�������ʹ���κΰ�ȫ���ƣ�����Դ����ģ������֤��Դ�����İ�ȫ��



static struct tagRscNode s_tRscRoot;
static struct tagRscNode *s_ptRscRoot;
static struct tagMutexLCB s_tMutexRscRoot;


//----��ʼ����Դ����ģ��-------------------------------------------------------
//����: ������Դ�ڵ���ڣ������������ѳ��������ڶ�������������ִ��������ʼ����
//      ���ܵ��ã���һ�������������ʼ��ִ��ǰ����
//����: ��
//����: 1
//------------------------------------------------------------------------------
ptu32_t Rsc1_ModuleInit(ptu32_t para)
{
    para = para;        //�����������澯
    s_tRscRoot.name = "resouce queue";
    s_tRscRoot.child = NULL;
    s_tRscRoot.next =&s_tRscRoot;
    s_tRscRoot.previous =&s_tRscRoot;
    s_tRscRoot.parent = NULL;
    s_tRscRoot.RscSize = sizeof(s_tRscRoot);
    s_tRscRoot.RscType = RSC_RSCNODE;
    s_ptRscRoot = &s_tRscRoot;
    return 1;
}
ptu32_t Rsc2_ModuleInit(ptu32_t para)
{
    para = para;        //�����������澯
    Lock_MutexCreate_s(&s_tMutexRscRoot,"mutex for resouce queue");
    return 1;
}
//----��Ӹ��ڵ�----------------------------------------------------------------
//����: ����Դ�������һ�����ڵ�(�ع���ģ�飬���ʱ����δ��ʼ����)
//����: node,����ӵĽڵ�ָ��
//      size���½ڵ������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct  tagRscNode * __Lock_RscAddLockTree(struct  tagRscNode *node,
                                u32 size,char *name)
{
    struct  tagRscNode *root_node;
    if(node == NULL)
        return NULL;
    node->parent = s_ptRscRoot;
    node->child=NULL;
    node->RscSize = size;
    node->RscType = RSC_RSCNODE;
    node->name = name;
    if(s_ptRscRoot->child == NULL)
    {
        s_ptRscRoot->child = node;
        node->next = node;
        node->previous = node;
    }else
    {
        root_node = s_ptRscRoot->child;
        node->next = root_node;
        node->previous = root_node->previous;
        root_node->previous->next = node;
        root_node->previous = node;
    }
    return node;
}

//----��ʼ���ڵ�----------------------------------------------------------------
//����: ��һ���ڵ������ָ�����㣬������ڵ㸺��size������ָ��
//����: node,����ӵĽڵ�ָ��
//����: ��
//------------------------------------------------------------------------------
void Rsc_CleanNode(struct tagRscNode *node)
{
    node->child = NULL;
    node->parent = NULL;
    node->next = NULL;
    node->previous = NULL;
}

//----��Ӹ��ڵ�----------------------------------------------------------------
//����: ����Դ�������һ�����ĸ��ڵ�
//����: node,����ӵĽڵ�ָ��
//      size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct  tagRscNode * Rsc_AddTree(struct  tagRscNode *node,
                                 u16 size,u8 RscType,char *name)
{
    struct  tagRscNode *root_node;
    atom_low_t   low_atom;

    if(node == NULL)
        return NULL;
    if(Rsc_SearchTree(name) != NULL)
        return NULL;
    node->parent = s_ptRscRoot;
    node->child=NULL;
    node->RscSize = size;
    node->RscType = RscType;
    node->name = name;
    low_atom = Int_LowAtomStart();
    //��Դ���������ٴ���һ���ź����ڵ㣬�����ж��Ƿ��
    root_node = s_ptRscRoot->child;
    node->next = root_node;
    node->previous = root_node->previous;
    root_node->previous->next = node;
    root_node->previous = node;
    Int_LowAtomEnd(low_atom);
    return node;
}

//----����ڵ�------------------------------------------------------------------
//����: ����Դ�����в���һ���ڵ㣬�½ڵ���ԭ�ڵ��previousλ��
//����  node,�ڴ˽ڵ�ǰ�����ڵ�
//      new_node,��������½ڵ�
//      size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct  tagRscNode * Rsc_AddToPrevious(struct  tagRscNode *node,
                                   struct tagRscNode *new_node,
                                   u16 size,u8 RscType,char *name)
{
    atom_low_t   low_atom;

    if((node==NULL)||(new_node==NULL))
        return NULL;
    low_atom = Int_LowAtomStart();
    new_node->next=node;
    new_node->previous=node->previous;
    new_node->parent=node->parent;
    new_node->child=NULL;
    new_node->RscSize = size;
    new_node->RscType = RscType;
    new_node->name = name;
    node->previous->next=new_node;
    node->previous=new_node;
    Int_LowAtomEnd(low_atom);
    return new_node;
}

//----���ӽڵ�------------------------------------------------------------------
//����: ����Դ����������һ���ڵ㣬�½ڵ���ԭ�ڵ��nextλ��
//����  node,�ڴ˽ڵ�������ڵ�
//      new_node,��������½ڵ�
//      size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct  tagRscNode * Rsc_AddToNext(struct  tagRscNode *node,
                                struct  tagRscNode *new_node,
                                u16 size,u8 RscType,char *name)
{
    atom_low_t   low_atom;

    if((node==NULL)||(new_node==NULL))
        return NULL;
    low_atom = Int_LowAtomStart();
    new_node->previous=node;
    new_node->next=node->next;
    new_node->parent=node->parent;
    new_node->child=NULL;
    new_node->RscSize = size;
    new_node->RscType = RscType;
    new_node->name = name;
    node->next->previous=new_node;
    node->next=new_node;
    Int_LowAtomEnd(low_atom);
    return new_node;
}

//----�����ӽڵ�----------------------------------------------------------------
//����: ��ָ���ڵ�����һ���ӽڵ�,�½ڵ㽫�ڶ���βλ��
//����  parent_node,�½ڵ�ĸ��ڵ�
//      new_node,��������½ڵ�
//      size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct  tagRscNode * Rsc_AddSon(struct  tagRscNode *parent_node,
                               struct  tagRscNode *new_node,
                               u16 size,u8 RscType,char *name)
{
    atom_low_t   low_atom;
    struct  tagRscNode *p;
    if((parent_node==NULL)||(new_node==NULL))
        return NULL;
    low_atom = Int_LowAtomStart();
    new_node->RscSize = size;
    new_node->RscType = RscType;
    new_node->name = name;
    new_node->child=NULL;
    if(parent_node->child==NULL)
    {
        parent_node->child=new_node;
        new_node->parent=parent_node;
        new_node->next=new_node;
        new_node->previous=new_node;
    }else
    {
        p=parent_node->child;
        new_node->next=p;
        new_node->previous=p->previous;
        new_node->parent=parent_node;
        p->previous->next=new_node;
        p->previous=new_node;
    }
    Int_LowAtomEnd(low_atom);
    return new_node;
}

//----����һ��ͷ�ڵ�---------------------------------------------------------
//����: ��ָ���ڵ�����һ���ӽڵ�,�½ڵ㽫�ڶ���ͷλ��
//����  parent_node,�½ڵ�ĸ��׽ڵ�
//      new_node,��������½ڵ�
//      size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct  tagRscNode * Rsc_AddHeadSon(struct  tagRscNode *parent_node,
                                    struct  tagRscNode *new_node,
                                    u16 size,u8 RscType,char *name)
{
    atom_low_t   low_atom;
    struct  tagRscNode *p;
    if((parent_node==NULL)||(new_node==NULL))
        return NULL;
    low_atom = Int_LowAtomStart();
    new_node->RscSize = size;
    new_node->RscType = RscType;
    new_node->name = name;
    new_node->child=NULL;
    if(parent_node->child==NULL)
    {
        parent_node->child=new_node;
        new_node->parent=parent_node;
        new_node->next=new_node;
        new_node->previous=new_node;
    }else
    {
        p=parent_node->child;
        new_node->next=p;
        new_node->previous=p->previous;
        new_node->parent=parent_node;
        p->previous->next=new_node;
        p->previous=new_node;
        parent_node->child = parent_node->child->previous;
    }
    Int_LowAtomEnd(low_atom);
    return new_node;
}

//----�滻�ڵ�-----------------------------------------------------------------
//����: ��һ���½ڵ��滻����Դ�����е�ԭ�нڵ㣬�½ڵ�ԭ�����벻�Ƕ����еĽڵ�
//����: oldnode�����滻�Ľڵ�
//      newnode���½ڵ�
//����: ��
//-----------------------------------------------------------------------------
bool_t Rsc_DisplaceNode(struct tagRscNode *oldnode,struct tagRscNode *newnode)
{
    struct tagRscNode *temp1,*temp2;
    atom_low_t   low_atom;
    low_atom = Int_LowAtomStart();
    temp1 = oldnode->child;
    if(temp1 != NULL)   //��oldnode�����ӽڵ�ĸ�ָ��ָ��newnode
    {
        temp2 = temp1;
        do
        {
            temp2->parent = newnode;
            temp2 = temp2->next;
        }while(temp2 != temp1);
    }
    newnode->child = temp1;
    newnode->next = oldnode->next;
    newnode->previous = oldnode->previous;
    newnode->parent = oldnode->parent;
    oldnode->next->previous = newnode;
    oldnode->previous->next = newnode;
    if(oldnode->parent->child == oldnode) //����Ƕ���ͷ�ڵ�
        oldnode->parent->child = newnode; //���ڵ���ӽڵ�ָ�����ͷ�ڵ�
    Int_LowAtomEnd(low_atom);
    return true;
}

//----ɾ��һ����֦-------------------------------------------------------------
//����: ��һ����֦����Դ������ɾ��
//����: branch����ɾ���ķ�֧��
//����: ��ɾ����ָ֧�룬NULL��ʾ��֧������
//-----------------------------------------------------------------------------
struct  tagRscNode * Rsc_DelBranch(struct  tagRscNode *branch)
{
    struct  tagRscNode * result;
    atom_low_t   low_atom;

    if(branch == NULL)
        return NULL;
    low_atom = Int_LowAtomStart();
    if(branch->next == branch)   //˵���ýڵ�û���ֵܽڵ�.
    {
        branch->parent->child = NULL;
    }else
    {
        if(branch->parent->child == branch)
        {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
            branch->parent->child = branch->next;
        }
        branch->previous->next = branch->next;
        branch->next->previous = branch->previous;
    }
    result = branch;

    Int_LowAtomEnd(low_atom);
    return result;
}
//---ɾ��һ���ڵ�---------------------------------------------------------------
//����: ��һ���ڵ����Դ�����жϿ��ڵ�,�ýڵ㲻�����ӽڵ�
//����: node,��ɾ���Ľڵ�,��ýڵ����ӽڵ���ɾ��
//����: ���ر�ɾ���ڵ�ָ�룬�����򷵻�NULL
//------------------------------------------------------------------------------
struct  tagRscNode * Rsc_DelNode(struct  tagRscNode *node)
{
    atom_low_t   low_atom;
    struct  tagRscNode * result;
    if(node == NULL)
        return NULL;
    low_atom = Int_LowAtomStart();
    if(node->child != NULL) //�ӽڵ�ǿ�,������
        result = NULL;
    else
    {
        if(node->next == node)   //˵���ýڵ�û���ֵܽڵ�.
        {
            node->parent->child = NULL;
        }else
        {
            if(node->parent->child == node)
            {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
                node->parent->child = node->next;
            }
            node->previous->next = node->next;
            node->next->previous = node->previous;
        }
        result = node;
    }
    Int_LowAtomEnd(low_atom);
    return result;
}

//---�ƶ�һ����---------------------------------------------------------------
//����: �ƶ�һ��������Ľڵ������Ϊ������
//����: node,���ƶ��Ľڵ�ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//-----------------------------------------------------------------------------
bool_t Rsc_MoveToTree(struct tagRscNode *parent,struct  tagRscNode *node)
{
    return true;
}

//----�ƶ��ڵ㵽���---------------------------------------------------------
//����: ��nextָ�뷽���ƶ���Դ�����е�һ���ڵ㣬��ͬ������ͷ��ǰһ���ڵ�λ��
//����: node,���ƶ��Ľڵ�ָ��
//����: ��
//-----------------------------------------------------------------------------
bool_t Rsc_MoveToLast(struct  tagRscNode *node)
{
    struct  tagRscNode *eldest;
    atom_low_t   low_atom;

    if(node == NULL)
        return false;
    low_atom = Int_LowAtomStart();

    if(node->parent != NULL)    //���ڵ㲻���ƶ�
    {
        eldest = node->parent->child;
        if(eldest == node)
        //���node��ͷ�ڵ�,��ֱ���ƶ����ڵ����ָ�뵽��һ���ڵ�Ϳ�����.
            node->parent->child = node->next;
        //���´�������ȡ���ڵ�
        node->next->previous = node->previous;
        node->previous->next = node->next;
        //���°�node�������βλ��,������ѭ������,ͷ�ڵ��ǰ�����β�ڵ�.
        node->next = eldest;
        node->previous = eldest->previous;
        eldest->previous->next = node;
        eldest->previous = node;
    }
    Int_LowAtomEnd(low_atom);
    return true;
}

//----�ƶ��ڵ��Ϊ����ͷ-------------------------------------------------------
//����: ��previousָ�뷽���ƶ���Դ�����е�һ���ڵ㣬��Ϊͬ������ͷ
//����: node,���ƶ��Ľڵ�ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t Rsc_MoveToHead(struct  tagRscNode *node)
{
    atom_low_t   low_atom;

    if(node == NULL)
        return false;
    low_atom = Int_LowAtomStart();
    if(node->parent)    //���ڵ㲻���ƶ�
    {
        Rsc_MoveToLast(node);
        node->parent->child = node;
    }
    Int_LowAtomEnd(low_atom);
    return true;
}

//----�ƶ��ڵ㵽ĳ�ڵ�nextλ��-------------------------------------------------
//����: �ƶ���Դ�����е�һ���ڵ�node����һ���ڵ�elder��nextλ��
//����: node,���ƶ��Ľڵ�ָ��
//      elder,Ŀ��ڵ�,node�ƶ������ڵ����
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t Rsc_MoveToNext(struct  tagRscNode *elder,struct  tagRscNode *node)
{
    atom_low_t   low_atom;

    if((elder==NULL)||(node==NULL)||(elder==node))
        return false;
    else if(elder->parent != node->parent)
        return false;
    low_atom = Int_LowAtomStart();
    //���´�������ȡ���ڵ�
    node->next->previous = node->previous;
    node->previous->next = node->next;
    node->previous = elder;
    node->next = elder->next;
    elder->next->previous = node;
    elder->next = node;

Int_LowAtomEnd(low_atom);
    return true;
}

//----�ƶ��ڵ㵽ĳ�ڵ�previousλ��---------------------------------------------
//����: �ƶ���Դ�����е�һ���ڵ�node����һ���ڵ�lesser��previousλ��
//����: node,���ƶ��Ľڵ�ָ��
//      lesser,Ŀ��ڵ�,node�ƶ������ڵ�ǰ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t Rsc_MoveToPrevious(struct  tagRscNode *lesser,struct  tagRscNode *node)
{
    atom_low_t   low_atom;

    if((lesser==NULL)||(node==NULL)||(lesser==node))
        return false;
    else if(lesser->parent != node->parent)
        return false;
    low_atom = Int_LowAtomStart();

    //���´�������ȡ���ڵ�
    node->next->previous = node->previous;
    node->previous->next = node->next;
    node->next = lesser;
    node->previous = lesser->previous;
    lesser->previous->next = node;
    lesser->previous = node;

    Int_LowAtomEnd(low_atom);
    return true;
}

//----����ͷλ�ú���--------------------------------------------------------------
//����: parent���ӽڵ�����λ�ò���,����ͷ��previous�����ƶ�һ��
//����: parent,���ڵ�ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t Rsc_RoundPrevious(struct  tagRscNode *parent)
{
    atom_low_t   low_atom;

    if(parent == NULL)
        return false;
    low_atom = Int_LowAtomStart();
    if(parent->child != NULL)
    {
        parent->child = parent->child->previous;
    }
    Int_LowAtomEnd(low_atom);
    return true;
}

//----����ͷλ��ǰ��-----------------------------------------------------------
//����: parent���ӽڵ�����λ�ò���,����ͷ��next�����ƶ�һ��
//����: parent,���ڵ�ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t Rsc_RoundNext(struct  tagRscNode *parent)
{
    atom_low_t   low_atom;

    if(parent == NULL)
        return false;
    low_atom = Int_LowAtomStart();
    if(parent->child != NULL)
    {
        parent->child = parent->child->next;
    }
    Int_LowAtomEnd(low_atom);
    return true;
}

//----��Դ����-----------------------------------------------------------------
//����: �޸���Դ��
//����: node�����޸ĵ���Դ�ڵ�ָ��
//      new_name��������
//����: true = �ɹ�ִ�У�false = ʧ��
//-----------------------------------------------------------------------------
bool_t Rsc_RenameNode(struct tagRscNode *node,char *new_name)
{
    if((node == NULL) || (new_name == NULL))
        return false;
    node->name = new_name;  //�����ж��������ַ����ĳ��ȣ���Դ���������ǿմ�
    return true;
}

//----��ȡ�����ڵ�-------------------------------------------------------------
//����: ����ָ���ڵ����ڵ����ĸ��ڵ�ָ��
//����: scion_node,Ŀ��ڵ㡣
//����: ���ڵ�ָ��
//----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetTree(struct  tagRscNode *scion_node)
{
    atom_low_t   low_atom;
    struct  tagRscNode *node = scion_node;
    if(node == NULL)    //Ŀ��ڵ��
        return NULL;
    low_atom = Int_LowAtomStart();
    while(node->parent != s_ptRscRoot)
    {
        if(NULL != node->parent)
            node = node->parent;
        else
        {
            node = NULL;
            break;
        }
    }
    Int_LowAtomEnd(low_atom);
    return node;
}

//----��ȡ��Դ���ڵ�-----------------------------------------------------------
//����: ����������Դϵͳ�ĸ��ڵ�ָ��
//����: scion_node,Ŀ��ڵ㡣
//����: ��Դ��ָ��
//----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetRoot(void)
{
    return s_ptRscRoot;
}

//----��ȡ�ڵ�����---------------------------------------------------------------
//����: ����ĳ�ڵ������,
//����: node,����������Դ�ڵ�
//����: �ýڵ�����֣�Ӧ����һ��char *��ָ��
//ע��: �ýڵ����û�����֣���˷��صļ�ʹ�ǳɹ�Ҳ����ΪNULL����Ϊȷʵû������
//      �ɹ�ʧ�ܲ�Ӧ�þݴ����ж�
//-----------------------------------------------------------------------------
char *Rsc_GetName(struct  tagRscNode *node)
{
    char *name;
    if(node == NULL)
        return NULL;

    name = node->name;
    return name;
}

//----��ȡ�ڵ�ߴ�---------------------------------------------------------------
//����: ����ĳ�ڵ�����ݽṹ�ߴ�,�����ڵ㱾��,
//����: node,����������Դ�ڵ�
//����: �ýڵ�ĳߴ�
//-----------------------------------------------------------------------------
u16 Rsc_GetNodeSize(struct  tagRscNode *node)
{
    if(node == NULL)
        return 0;

    return node->RscSize;
}

//----��ȡ�ڵ�ߴ�---------------------------------------------------------------
//����: ����ĳ�ڵ������,
//����: node,����������Դ�ڵ�
//����: �ýڵ������
//-----------------------------------------------------------------------------
u8 Rsc_GetNodeType(struct  tagRscNode *node)
{
    if(node == NULL)
        return 0;

    return node->RscType;
}

//----��ȡ���ڵ�---------------------------------------------------------------
//����: ����ĳ�ڵ�ĸ��ڵ�,
//����: son_node,��Ҫ���ҵ���֦������һ���ӽڵ�
//����: ���ڵ�.
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetParent(struct  tagRscNode *son_node)
{
    struct  tagRscNode *node;
    if(son_node == NULL)
        return NULL;

    node = son_node->parent;

    return node;
}

//----��ȡ�ӽڵ�--------------------------------------------------------------
//����: �����ӽڵ㣬�򸸽ڵ��childָ��ָ���ӽڵ�Ķ���ͷ��ʵ�ʷ����ӽڵ�Ķ���ͷ
//����: parent_node,��Ҫ���ҵ���֦�ĸ��ڵ�
//����: �ӽڵ����ͷ
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetSon(struct  tagRscNode *parent_node)
{
    struct  tagRscNode *node;
    if(parent_node == NULL)
        return NULL;
    node = parent_node->child;

    return node;
}

//----��ȡǰһ���ڵ�-----------------------------------------------------------
//����: ����next_node�ڵ��previous�ڵ�
//����: next_node,��Ҫ���ҵ���֦���ֽڵ��е�һ���ڵ�
//����: previous�ڵ�.
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetPrevious(struct  tagRscNode *next_node)
{
    struct  tagRscNode *node;
    if(next_node == NULL)
        return NULL;

    node = next_node->previous;

    return node;
}

//----��ȡ�ܽڵ�--------------------------------------------------------------
//����: ����previous_node�ڵ��next�ڵ�
//����: previous_node,��Ҫ���ҵ���֦���ֽڵ��е�һ���ڵ�
//����: next�ڵ�.
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetNext(struct  tagRscNode *previous_node)
{
    struct  tagRscNode *node;
    if(previous_node == NULL)
        return NULL;

    node = previous_node->next;

    return node;
}

//----��ȡ����ͷ---------------------------------------------------------------
//����: ����rnode�ڵ����ڶ��еĶ���ͷ
//����: rnode,��Ҫ���ҵ���֦������ڵ�
//����: ����ͷָ��.
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetHead(struct  tagRscNode *rnode)
{
    struct  tagRscNode *node;
    if(rnode == NULL)
        return NULL;

    node = rnode->parent->child;

    return node;
}

//----��ȡ��֦һ��ĩ�ҽڵ�-----------------------------------------------------
//����: ����ĳ��֦��һ��ĩ�ҽڵ�,ĩ�ҽڵ���ָû���ӽڵ�Ľڵ�.
//����: parent_node,��Ҫ��������֦�ĸ��ڵ�
//����: ��֦��һ��ĩ�ҽڵ�.��û���ӽڵ�ʱ������NULL
//��ע: ����Ҫɾ��������֦ʱ,������������,���rsc_del_node����,�������ñ�����,
//      ���ѷ��صĽڵ�ɾ��,ֱ������������NULL.��Ҫɾ��һ���ļ��л���ɾ��һ����
//      ���Ӵ��ڵ�gui����ʱ,����Ҫ�õ�ɾ��������֦�Ĳ���.
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_GetTwig(struct  tagRscNode *ancestor_node)
{
    struct  tagRscNode *result=NULL,*current;
    if(ancestor_node == NULL)
        return NULL;

    current = ancestor_node;
    while(current->child != NULL)
    {
        current = current->child;
    }
    if(current == ancestor_node)
        result = NULL;
    else
        result = current;

    return result;
}

//----ȡ�ڵ�ļ���-----------------------------------------------------
//����: �鿴ĳ�ڵ��Ǹ��ڵ�ĵڼ����ӽڵ㣬
//����: node,����Ľڵ�
//����: �ӽڵ㼶����0������ڵ㡣cn_limit_uint32�������
//-----------------------------------------------------------------------------
u32 Rsc_GetClass(struct  tagRscNode *node)
{
    u32 n=0;
    struct tagRscNode *pl_node = node;
    if(pl_node == NULL)
        return CN_LIMIT_UINT32;

    while(pl_node->parent != s_ptRscRoot)
    {
        if(NULL != pl_node->parent)
            pl_node = pl_node->parent;
        else
        {
            n = CN_LIMIT_UINT32;
            break;

        }
        n++;
    }

    return n;
}

//----�����ӽڵ�--------------------------------------------------------------
//����: �ӵ�ǰ�ڵ㿪ʼ,��ȡ��һ���ڵ��ָ��,ֱ��������ȫ���ӽڵ�
//����: parent_node,��Ҫ��������֦�����Ƚڵ�
//      current_node,��ǰ����λ��,
//����: ��ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
//------------------------------------------------------------------------------
struct  tagRscNode *Rsc_TraveSon(struct  tagRscNode *parent_node,
                                  struct  tagRscNode *current_son)
{
    if((parent_node==NULL)||(current_son==NULL))
    {
        return NULL;
    }

    if(current_son == parent_node)
    {
        current_son = parent_node->child;
    }
    else
    {
        current_son = current_son->next;
        if(current_son == parent_node->child)
            current_son = NULL;
    }

    return current_son;
}

//----����һ����֦--------------------------------------------------------------
//����: �ӵ�ǰ�ڵ㿪ʼ,��ȡ��һ���ڵ��ָ��,��������·��,ֱ��������������֦.����
//      ·��Ϊ:��ǰ�ڵ���ӽڵ�,����ӽڵ�Ϊ��������next�ڵ�,next�ڵ���Ϊ������
//      �����ڵ��next�ڵ�,ֱ���������.
//      ������ֻ��������ڵ㣬������ancestor_node����
//����: parent_node,��Ҫ��������֦�����Ƚڵ�
//      current_node,��ǰ����λ��,
//����: ��ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
//��ע: ����Ҫ����Դ������ĳһ����֦�������������еĽڵ���һ����ĳ�ֲ���ʱ,��
//      �������ñ�����,��һ�ε���current_node = parent_node,���current_node
//      = �ϴη���ֵ��ֱ�����ؿ�.
//      �������������ӡ������....��˳������������ֱϵ��������ϵ��ȷ����������
//      �ڵ㶼�ܹ����ʵ�������Է���˳��������Ҫ�󣬲���ʹ�ñ�����
//------------------------------------------------------------------------------
struct  tagRscNode *Rsc_TraveScion(struct  tagRscNode *ancestor_node,
                                  struct  tagRscNode *current_node)
{
    struct  tagRscNode *result=NULL,*current_copy;
    bool_t up = false;
    if((ancestor_node==NULL)||(current_node==NULL))
    {
        return NULL;
    }

    if((current_node != ancestor_node) || ancestor_node->child)
    {
        current_copy = current_node;
        do
        {
            if((up == false) && (current_copy->child != NULL))
            {   //�ӽڵ�ǿ�,�����ӽڵ�
                result = current_copy->child;
                break;
            }else if(current_copy->next != current_copy->parent->child)
            {   //�ӽڵ��,�����ڵ㲻�Ƕ���β�ڵ�,����next�ڵ�
                result = current_copy->next;
                break;
            }else
            {   //���ӽڵ�,�ұ��ڵ��Ѿ��Ƕ���β�ڵ�,��Ҫ�жϵ�ǰ���ڵ��next�ڵ�
                current_copy = current_copy->parent;
                up = true;
            }
        }while(current_copy != ancestor_node);
    }

    return result;
}

//----���ֵܽڵ���������Դ-----------------------------------------------------
//����: ��ĳһ����Դ�ڵ�������ֵܽڵ��������������ֵ���Դ
//����: brother,�ֵܽڵ�������һ����Դָ��.
//      name,��Ҫ��������Դ��
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_SearchSibling(struct  tagRscNode *brother,char *name)
{
    struct  tagRscNode *temp,*result = NULL;
    if((brother == NULL)||(name == NULL))
        return NULL;
    temp = brother;

    do
    {
        if(strcmp(temp->name,name)==0)
        {
            result = temp;
            break;
        }
        temp = temp->next;
    }while (temp != brother);

    return result;
}

//----������Դ��---------------------------------------------------------------
//����: ����Դ���ڵ��ֱ���ӽڵ��в�����Ϊname����Դ��
//����: ��Դ����
//����: ��Դ���ڵ�ָ�롣NULL = û�ҵ�
//-----------------------------------------------------------------------------
struct tagRscNode *Rsc_SearchTree(char *tree_name)
{
    struct  tagRscNode *temp,*result = NULL;
    if(tree_name == NULL)
        return NULL;
    temp = s_ptRscRoot->child;

    do
    {
        if(strcmp(temp->name,tree_name)==0)
        {
            result = temp;
            break;
        }
        temp = temp->next;
    }while (temp != s_ptRscRoot->child);

    return result;
}

//----���ӽڵ���������Դ-----------------------------------------------------
//����: ��ĳһ����Դ�ڵ�������ӽڵ��������������ֵ���Դ
//����: parent,���ڵ���Դָ��.
//      name,��Ҫ��������Դ��,����Դ��,������·��.
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_SearchSon(struct  tagRscNode *parent,char *name)
{
    struct  tagRscNode *temp,*result = NULL;
    if((name == NULL)||(parent == NULL))
        return NULL;

    temp = parent->child;
    if(temp == NULL)
        result = NULL;
    else
    {
        do
        {
            if(strcmp(temp->name,name)==0)
            {
                result = temp;
                break;
            }
            temp = temp->next;
        }while (temp != parent->child);
    }

    return result;
}

//----�ں����������Դ��-------------------------------------------------------
//����: ��rsc_search_layer����,����ancestor_node�����к���ڵ�,ֱ���ҵ�һ������
//      ƥ����Դ��
//����: ancestor_node,��֦�ĸ��ڵ�
//      name,��Ҫ��������Դ��,����Դ��,������·��.
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//------------------------------------------------------------------------------
struct tagRscNode *Rsc_SearchScion(struct tagRscNode *ancestor_node,char *name)
{
    struct  tagRscNode  *current,*temp,*result = NULL;
    if((ancestor_node == NULL)||(name == NULL))
        return NULL;
    current = ancestor_node;
    //��rsc_trave_scion���Ѿ����ź����������˴����뱣��
    while((temp = Rsc_TraveScion(ancestor_node,current)) != NULL)
    {
        if(strcmp(temp->name,name) == 0)
        {
            result = temp;
            break;
        }
        current = temp;
    }
    return result;
}

//----��·��������Դ��---------------------------------------------------------
//����: ��rsc_search_scion����,��ͬ���ǣ�path�ǰ���·�������ַ�����ִ�о�ȷ������
//����: ancestor_node,��֦�ĸ��ڵ�
//      path,����·��������Դ��
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct  tagRscNode *Rsc_Search(struct  tagRscNode *ancestor_node,char *path)
{
    char *path_name,*dest_name;
    bool_t   match;
    u32 i;

    struct  tagRscNode *current,*result = NULL;
    if((path == NULL)||(ancestor_node == NULL))
        return NULL;

    current = ancestor_node->child;
    if(current == NULL)
    {

        return NULL;
    }
    path_name = path;
    while(1)
    {
        dest_name = current->name;
        match=false;
        for(i=0;i<CN_RSC_NAME_LIMIT;i++)   //��Դ����������
        {
            if((path_name[i] == 0)||((path_name[i]=='\\')&&(path_name[i+1]==0)))
            {   //�Ѿ��ﵽ�ַ�������,�����
                result = current;
                break;
            }else if(path_name[i] == '\\')
            {   //�ַ������,��path_nameδ����
                match = true;
                break;
            }
            if(path_name[i] != dest_name[i])
            {   //�ַ��������
                match = false;
                break;
            }
        }
        if(result != NULL)  //����ƥ���ҵ���path�ַ���ĩ,���ҽ���
            break;

        if(match != true)
        {   //���ڵ����ֲ�ƥ��,������һ���ڵ�
            current = current->next;
            if(current == current->parent->child)
            //���������нڵ��Ѿ�������,û���ҵ�ƥ���.
                break;
        }else
        {   //���ڵ�����ƥ��,���ǻ�û��path������,���������ӽڵ�
            path_name += i+1;
            current = current->child;
            if(current == NULL)
            //������Դ���Ѿ���������,��path_name��δ����
                break;
        }
    }

    return result;
}

//----�Ƿ����ͷ-------------------------------------------------------------
//����: �жϸ����ڵ��Ƿ��ֵܽڵ��еĶ���ͷ�ڵ�
//����: node�������Խڵ�
//����: true=�Ƕ���ͷ��false=���Ƕ���ͷ
//-----------------------------------------------------------------------------
bool_t Rsc_IsHead(struct tagRscNode *node)
{
    if(node == s_ptRscRoot)
        return true;
    if(node  == node->parent->child)
        return true;
    else
        return false;
}

//----�Ƿ����β---------------------------------------------------------------
//����: �жϸ����ڵ��Ƿ��ֵܽڵ��е����һ���ڵ�
//����: node�������Խڵ�
//����: true=�����һ���ڵ㣬false=����
//-----------------------------------------------------------------------------
bool_t Rsc_IsLast(struct tagRscNode *node)
{
    if(node == s_ptRscRoot)
        return true;
    if(node->next  == node->parent->child)
        return true;
    else
        return false;
}

//----ȡ�ڵ�˳��-------------------------------------------------------------
//����: ȡ�����ڵ����ֵܽڵ��е�λ�ã�head=0����һ����1������֮
//����: node�������Խڵ�
//����: �ڵ�λ�ú�
//-----------------------------------------------------------------------------
u32 Rsc_NodeSequencing(struct tagRscNode *node)
{
    u32 ranking;
    struct tagRscNode *temp;
    if(node == s_ptRscRoot)
        return 0;
    ranking = 0;
    temp = node->parent->child;
    while(temp != node)
    {
        ranking +=1;
        temp = temp->next;
    }
    return ranking;
}
//-----------------------------------------------------------------
//����: ��һ����֦����Դ������ɾ��,������֦�ڵ�
//����: Branch����ɾ���ķ�֧��
//����: ��ɾ����ָ֧�룬NULL��ʾ��֧������
//------------
//���¼�¼:
//1.����: 2015/3/21
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
bool_t Rsc_ClearBranch_t(struct tagRscNode *Branch)
{
    atom_low_t low_atom;

    if(NULL == Branch)
        return (false);
    low_atom = Int_LowAtomStart();
    Branch->child = NULL;
    Int_LowAtomEnd(low_atom);
    return (true);

}
