//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ��Դ����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��������ӡ��ƶ���ɾ����������Դ���ķ���
//����˵��:
//�޶���ʷ:
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
#include "os_inc.h"
#include <string.h>

//-------------��Դ���㷨˵��----------------------
//��ͬ���������˫��ѭ������
//ÿ������childָ��ֱ��ָ��Ľ���Ϊ�ý������ڶ��еĶ���ͷ������next���
//�Ƕ���β��
//���н���Parentָ���ָ�򸸽��

//�ر���ʾ����Դ���еĲ������ʰ�ȫ����Դ����ģ�鸺����Դ�������Ĳ�������
//          ������ʹ���߸���

static struct rsc_node tg_rsc_root;
static struct rsc_node *pg_rsc_root;
static struct semaphore_LCB tg_semp_rsc_root;

//----��ʼ����Դ����ģ��-------------------------------------------------------
//����: �����ܸ���㣬�����������ѳ��������ڶ�������������ִ��������ʼ�������
//      ���ã���һ�������������ʼ��ִ��ǰ����
//����: ��
//����: 1
//------------------------------------------------------------------------------
ptu32_t module_init_rsc1(ptu32_t para)
{
    tg_rsc_root.name = "resouce queue";
    tg_rsc_root.child = NULL;
    tg_rsc_root.next =&tg_rsc_root;
    tg_rsc_root.previous =&tg_rsc_root;
    tg_rsc_root.parent = NULL;
    tg_rsc_root.node_size = sizeof(tg_rsc_root);
    pg_rsc_root = &tg_rsc_root;
    return 1;
}
ptu32_t module_init_rsc2(ptu32_t para)
{
    semp_create_knl(&tg_semp_rsc_root,1,1,"semaphore for resouce queue");
    return 1;
}
//----��Ӹ����----------------------------------------------------------------
//����: ����Դ�������һ�������(�ع���ģ�飬���ʱ����δ��ʼ����)
//����: node,����ӵĽ��ָ��
//      size���½�������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľ��
//------------------------------------------------------------------------------
struct  rsc_node * __rsc_add_lock_tree(struct  rsc_node *node,
                                u32 size,char *name)
{
    struct  rsc_node *root_node;
    if(node == NULL)
        return NULL;
    node->parent = pg_rsc_root;
    node->child=NULL;
    node->node_size = size;
    node->name = name;
    if(pg_rsc_root->child == NULL)
    {
        pg_rsc_root->child = node;
        node->next = node;
        node->previous = node;
    }else
    {
        root_node = pg_rsc_root->child;
        node->next = root_node;
        node->previous = root_node->previous;
        root_node->previous->next = node;
        root_node->previous = node;
    }
    return node;
}

//----��Ӹ����----------------------------------------------------------------
//����: ����Դ�������һ�������
//����: node,����ӵĽ��ָ��
//      size���½�������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľ��
//------------------------------------------------------------------------------
struct  rsc_node * rsc_add_tree_root(struct  rsc_node *node,
                                u32 size,char *name)
{
    struct  rsc_node *root_node;
    if(node == NULL)
        return NULL;
    node->parent = pg_rsc_root;
    node->child=NULL;
    node->node_size = size;
    node->name = name;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    //��Դ���������ٴ���һ���ź�����㣬�����ж��Ƿ��
    root_node = pg_rsc_root->child;
    node->next = root_node;
    node->previous = root_node->previous;
    root_node->previous->next = node;
    root_node->previous = node;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----������------------------------------------------------------------------
//����: ����Դ�����в���һ����㣬�½����ԭ����previousλ��
//����  node,�ڴ˽��ǰ�������
//      new_node,��������½��
//      size���½�������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľ��
//------------------------------------------------------------------------------
struct  rsc_node * rsc_insert_node(struct  rsc_node *node,
                                   struct rsc_node *new_node,
                                   u32 size,char *name)
{
    if((node==NULL)||(new_node==NULL))
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    new_node->next=node;
    new_node->previous=node->previous;
    new_node->parent=node->parent;
    new_node->child=NULL;
    new_node->node_size = size;
    new_node->name = name;
    node->previous->next=new_node;
    node->previous=new_node;
    semp_post(&tg_semp_rsc_root);
    return new_node;
}

//----���ӽ��------------------------------------------------------------------
//����: ����Դ����������һ����㣬�½����ԭ����nextλ��
//����  node,�ڴ˽����������
//      new_node,��������½��
//      size���½�������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľ��
//------------------------------------------------------------------------------
struct  rsc_node * rsc_add_node(struct  rsc_node *node,
                                struct  rsc_node *new_node,
                                u32 size,char *name)
{
    if((node==NULL)||(new_node==NULL))
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    new_node->previous=node;
    new_node->next=node->next;
    new_node->parent=node->parent;
    new_node->child=NULL;
    new_node->node_size = size;
    new_node->name = name;
    node->next->previous=new_node;
    node->next=new_node;
    semp_post(&tg_semp_rsc_root);
    return new_node;
}

//----�����ӽ��----------------------------------------------------------------
//����: ��ָ���������һ���ӽ��,�½�㽫�ڶ���βλ��
//����  parent_node,�½��ĸ����
//      new_node,��������½��
//      size���½�������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľ��
//------------------------------------------------------------------------------
struct  rsc_node * rsc_add_son(struct  rsc_node *parent_node,
                               struct  rsc_node *new_node,
                               u32 size,char *name)
{
    struct  rsc_node *p;
    if((parent_node==NULL)||(new_node==NULL))
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    new_node->node_size = size;
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
    semp_post(&tg_semp_rsc_root);
    return new_node;
}

//----����һ��ͷ���---------------------------------------------------------
//����: ��ָ���������һ���ӽ��,�½�㽫�ڶ���ͷλ��
//����  parent_node,�½��ĸ��׽��
//      new_node,��������½��
//      size���½�������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľ��
//------------------------------------------------------------------------------
struct  rsc_node * rsc_add_head_son(struct  rsc_node *parent_node,
                                    struct  rsc_node *new_node,
                                    u32 size,char *name)
{
    struct  rsc_node *p;
    if((parent_node==NULL)||(new_node==NULL))
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    new_node->node_size = size;
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
    semp_post(&tg_semp_rsc_root);
    return new_node;
}

//----�滻���-----------------------------------------------------------------
//����: ��һ���½���滻����Դ�����е�ԭ�н�㣬�½��ԭ�����벻�Ƕ����еĽڵ�
//����: oldnode�����滻�Ľڵ�
//      newnode���½��
//����: ��
//-----------------------------------------------------------------------------
bool_t rsc_displace_node(struct rsc_node *oldnode,struct rsc_node *newnode)
{
    struct rsc_node *temp1,*temp2;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;
    temp1 = oldnode->child;
    if(temp1 != NULL)   //��oldnode�����ӽ��ĸ�ָ��ָ��newnode
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
    if(oldnode->parent->child == oldnode) //����Ƕ���ͷ���
        oldnode->parent->child = newnode; //�������ӽ��ָ�����ͷ���
    semp_post(&tg_semp_rsc_root);
    return true;
}

//----ɾ��һ����֦-------------------------------------------------------------
//����: ��һ����֦����Դ������ɾ��
//����: branch����ɾ���ķ�֧��
//����: ��ɾ����ָ֧�룬NULL��ʾ��֧������
//-----------------------------------------------------------------------------
struct  rsc_node * rsc_del_branch(struct  rsc_node *branch)
{
    struct  rsc_node * result;
    if(branch == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    if(branch->next == branch)   //˵���ý��û���ֵܽ��.
    {
        branch->parent->child = NULL;
    }else
    {
        if(branch->parent->child == branch)
        {   //˵���ý���Ƕ���ͷ���,��Ҫ�ı����ͷ���
            branch->parent->child = branch->next;
        }
        branch->previous->next = branch->next;
        branch->next->previous = branch->previous;
    }
    result = branch;
    
    semp_post(&tg_semp_rsc_root);
    return result;
}
//---ɾ��һ�����---------------------------------------------------------------
//����: ��һ��������Դ�����жϿ����,�ý�㲻�����ӽ��
//����: node,��ɾ���Ľ��,��ý�����ӽ����ɾ��
//����: ���ر�ɾ���ڵ�ָ�룬�����򷵻�NULL
//------------------------------------------------------------------------------
struct  rsc_node * rsc_del_node(struct  rsc_node *node)
{
    struct  rsc_node * result;
    if(node == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    if(node->child != NULL) //�ӽ��ǿ�,������
        result = NULL;
    else
    {
        if(node->next == node)   //˵���ý��û���ֵܽ��.
        {
            node->parent->child = NULL;
        }else
        {
            if(node->parent->child == node)
            {   //˵���ý���Ƕ���ͷ���,��Ҫ�ı����ͷ���
                node->parent->child = node->next;
            }
            node->previous->next = node->next;
            node->next->previous = node->previous;
        }
        result = node;
    }
    semp_post(&tg_semp_rsc_root);
    return result;
}

//---�ƶ�һ����---------------------------------------------------------------
//����: �ƶ�һ��������Ľ�������Ϊ������
//����: node,���ƶ��Ľ��ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//-----------------------------------------------------------------------------
bool_t rsc_moveto_tree(struct rsc_node *parent,struct  rsc_node *node)
{
    return true;
}

//----�ƶ���㵽���---------------------------------------------------------
//����: ��nextָ�뷽���ƶ���Դ�����е�һ����㣬��ͬ������ͷ��ǰһ���ӵ�λ��
//����: node,���ƶ��Ľ��ָ��
//����: ��
//-----------------------------------------------------------------------------
bool_t rsc_moveto_last(struct  rsc_node *node)
{
    struct  rsc_node *eldest;
    if(node == NULL)
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;

    if(node->parent != NULL)    //����㲻���ƶ�
    {
        eldest = node->parent->child;
        if(eldest == node)
        //���node��ͷ���,��ֱ���ƶ���������ָ�뵽��һ�����Ϳ�����.
            node->parent->child = node->next;
        //���´�������ȡ�����
        node->next->previous = node->previous;
        node->previous->next = node->next;
        //���°�node�������βλ��,������ѭ������,ͷ����ǰ�����β���.
        node->next = eldest;
        node->previous = eldest->previous;
        eldest->previous->next = node;
        eldest->previous = node;
    }
    semp_post(&tg_semp_rsc_root);
    return true;
}

//----�ƶ�����Ϊ����ͷ-------------------------------------------------------
//����: ��previousָ�뷽���ƶ���Դ�����е�һ����㣬��Ϊͬ������ͷ
//����: node,���ƶ��Ľ��ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t rsc_moveto_head(struct  rsc_node *node)
{
    if(node == NULL)
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;
    if(node->parent)    //����㲻���ƶ�
    {
        rsc_moveto_last(node);
        node->parent->child = node;
    }
    semp_post(&tg_semp_rsc_root);
    return false;
}

//----�ƶ���㵽ĳ���nextλ��-------------------------------------------------
//����: �ƶ���Դ�����е�һ�����node����һ�����elder��nextλ��
//����: node,���ƶ��Ľ��ָ��
//      elder,Ŀ����,node�ƶ�����������
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t rsc_moveto_next(struct  rsc_node *elder,struct  rsc_node *node)
{
    if((elder==NULL)||(node==NULL))
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;

    //���´�������ȡ�����
    node->next->previous = node->previous;
    node->previous->next = node->next;
    //���°�node����С��λ��,������ѭ������,���ֽ���ǰ�����С�ܽ��.
    node->previous = elder;
    node->next = elder->next;
    elder->next->previous = node;
    elder->next = node;

    semp_post(&tg_semp_rsc_root);
    return false;
}

//----�ƶ���㵽ĳ���previousλ��---------------------------------------------
//����: �ƶ���Դ�����е�һ�����node����һ�����lesser��previousλ��
//����: node,���ƶ��Ľ��ָ��
//      lesser,Ŀ����,node�ƶ��������ǰ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t rsc_moveto_previous(struct  rsc_node *lesser,struct  rsc_node *node)
{
    if((lesser==NULL)||(node==NULL))
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;

    //���´�������ȡ�����
    node->next->previous = node->previous;
    node->previous->next = node->next;
    //���°�node����С��λ��,������ѭ������,���ֽ���ǰ�����С�ܽ��.
    node->next = lesser;
    node->previous = lesser->previous;
    lesser->previous->next = node;
    lesser->previous = node;

    semp_post(&tg_semp_rsc_root);
    return false;
}

//----����ͷλ�ú���--------------------------------------------------------------
//����: parent���ӽӵ�����λ�ò���,����ͷ��previous�����ƶ�һ��
//����: parent,�����ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t rsc_round_previous(struct  rsc_node *parent)
{
    if(parent == NULL)
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;
    if(parent->child != NULL)
    {
        parent->child = parent->child->previous;
    }
    semp_post(&tg_semp_rsc_root);
    return false;
}

//----����ͷλ��ǰ��-----------------------------------------------------------
//����: parent���ӽӵ�����λ�ò���,����ͷ��next�����ƶ�һ��
//����: parent,�����ָ��
//����: true = �ɹ�ִ�У�false = ʧ��
//------------------------------------------------------------------------------
bool_t rsc_round_next(struct  rsc_node *parent)
{
    if(parent == NULL)
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;
    if(parent->child != NULL)
    {
        parent->child = parent->child->next;
    }
    semp_post(&tg_semp_rsc_root);
    return false;
}

//----��Դ����-----------------------------------------------------------------
//����: �޸���Դ��
//����: node�����޸ĵ���Դ���ָ��
//      new_name��������
//����: true = �ɹ�ִ�У�false = ʧ��
//-----------------------------------------------------------------------------
bool_t rsc_rename_node(struct rsc_node *node,char *new_name)
{
    if((node == NULL) || (new_name == NULL))
        return false;
    node->name = new_name;  //�����ж��������ַ����ĳ��ȣ���Դ���������ǿմ�
    return false;
}

//----�����������-------------------------------------------------------------
//����: ����ָ��������ڵ����ĸ����ָ��
//����: scion_node,Ŀ���㡣
//����: �����ָ��
//----------------------------------------------------------------------------
struct  rsc_node *rsc_get_tree(struct  rsc_node *scion_node)
{
    struct  rsc_node *node = scion_node;
    if(node == NULL)    //Ŀ�����
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    while(node->parent != pg_rsc_root)
        node = node->parent;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----������Դ�����-----------------------------------------------------------
//����: ����������Դϵͳ�ĸ����ָ��
//����: scion_node,Ŀ���㡣
//����: ��Դ��ָ��
//----------------------------------------------------------------------------
struct  rsc_node *rsc_get_root(void)
{
    return pg_rsc_root;
}

//----������Դ��---------------------------------------------------------------
//����: ����Դ������ֱ���ӽ���в�����Ϊname����Դ��
//����: ��Դ����
//����: ��Դ�����ָ�롣NULL = û�ҵ�
//-----------------------------------------------------------------------------
struct rsc_node *rsc_search_tree(char *tree_name)
{
    struct  rsc_node *temp,*result = NULL;
    if(tree_name == NULL)
        return NULL;
    temp = pg_rsc_root->child;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    do
    {
        if(strcmp(temp->name,tree_name)==0)
        {
            result = temp;
            break;
        }
        temp = temp->next;
    }while (temp != pg_rsc_root->child);
    semp_post(&tg_semp_rsc_root);
    return result;
}
//----���ظ����---------------------------------------------------------------
//����: ����ĳ���ĸ����,
//����: son_node,��Ҫ���ҵ���֦������һ���ӽ��
//����: �����.
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_get_parent(struct  rsc_node *son_node)
{
    struct  rsc_node *node;
    if(son_node == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    node = son_node->parent;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----�����ӽ��--------------------------------------------------------------
//����: �����ӽ�㣬�򸸽���childָ��ָ���ӽӵ�Ķ���ͷ��ʵ�ʷ����ӽӵ�Ķ���ͷ
//����: parent_node,��Ҫ���ҵ���֦�ĸ����
//����: �ӽ�����ͷ
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_get_son(struct  rsc_node *parent_node)
{
    struct  rsc_node *node;
    if(parent_node == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    node = parent_node->child;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----����ǰһ�����-----------------------------------------------------------
//����: ����next_node����previous�ӵ�
//����: next_node,��Ҫ���ҵ���֦���ֽ���е�һ���ӵ�
//����: previous���.
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_get_previous(struct  rsc_node *next_node)
{
    struct  rsc_node *node;
    if(next_node == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) == false)
        return NULL;
    node = next_node->previous;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----���صܽ��--------------------------------------------------------------
//����: ����previous_node����next�ӵ�
//����: previous_node,��Ҫ���ҵ���֦���ֽ���е�һ���ӵ�
//����: next���.
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_get_next(struct  rsc_node *previous_node)
{
    struct  rsc_node *node;
    if(previous_node == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) == false)
        return NULL;
    node = previous_node->next;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----���ض���ͷ---------------------------------------------------------------
//����: ����rnode������ڶ��еĶ���ͷ
//����: rnode,��Ҫ���ҵ���֦��������
//����: ����ͷָ��.
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_get_head(struct  rsc_node *rnode)
{
    struct  rsc_node *node;
    if(rnode == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) == false)
        return NULL;
    node = rnode->parent->child;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----������֦һ��ĩ�ҽ��-----------------------------------------------------
//����: ����ĳ��֦��һ��ĩ�ҽ��,ĩ�ҽ����ָû���ӽ��Ľ��.
//����: parent_node,��Ҫ��������֦�ĸ����
//����: ��֦��һ��ĩ�ҽ��.��û���ӽ��ʱ������NULL
//��ע: ����Ҫɾ��������֦ʱ,������������,���rsc_del_node����,�������ñ�����,
//      ���ѷ��صĽ��ɾ��,ֱ������������NULL.��Ҫɾ��һ���ļ��л���ɾ��һ����
//      ���Ӵ��ڵ�gui����ʱ,����Ҫ�õ�ɾ��������֦�Ĳ���.
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_get_twig(struct  rsc_node *ancestor_node)
{
    struct  rsc_node *result=NULL,*current;
    if(ancestor_node == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
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
    semp_post(&tg_semp_rsc_root);
    return result;
}

//----ȡ���ļ���-----------------------------------------------------
//����: �鿴ĳ����Ǹ��ڵ�ĵڼ����ӽڵ㣬
//����: node,����Ľ��
//����: �ӽڵ㼶����0������ڵ㡣cn_limit_uint32�������
//-----------------------------------------------------------------------------
u32 rsc_get_class(struct  rsc_node *node)
{
    u32 n=0;
    struct rsc_node *pl_node = node;
    if(pl_node == NULL)
        return cn_limit_uint32;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return cn_limit_uint32;
    while(pl_node->parent != pg_rsc_root)
    {
        pl_node = pl_node->parent;
        n++;
    }
    semp_post(&tg_semp_rsc_root);
    return n;
}

//----������Դ����-------------------------------------------------------------
//����: ������Դ�����֣�ע�⣬�������ֵ��ַ����洢�ռ����û�������Դ����ֻ���
//      �ַ���ָ�롣
//����: node������������Դ���
//      newname���µ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void rsc_set_name(struct rsc_node *node,char *newname)
{
    node->name = newname;
    return;
}
//----����һ����֦--------------------------------------------------------------
//����: �ӵ�ǰ��㿪ʼ,��ȡ��һ������ָ��,��������·��,ֱ��������������֦.����
//      ·��Ϊ:��ǰ�����ӽ��,����ӽ��Ϊ��������next���,next�����Ϊ������
//      ��������next���,ֱ���������.
//      ������ֻ���������㣬������ancestor_node����
//����: parent_node,��Ҫ��������֦�����Ƚ��
//      current_node,��ǰ����λ��,
//����: ��ǰ����λ�õ���һ�����ָ��,����Ѿ��������,�򷵻�NULL.
//��ע: ����Ҫ����Դ������ĳһ����֦�������������еĽ����һ����ĳ�ֲ���ʱ,��
//      �������ñ�����,��һ�ε���current_node = parent_node,���current_node
//      = �ϴη���ֵ��ֱ�����ؿ�.
//      �������������ӡ������....��˳������������ֱϵ��������ϵ��ȷ����������
//      ��㶼�ܹ����ʵ�������Է���˳��������Ҫ�󣬲���ʹ�ñ�����
//------------------------------------------------------------------------------
struct  rsc_node *rsc_trave_scion(struct  rsc_node *ancestor_node,
                                  struct  rsc_node *current_node)
{
    struct  rsc_node *result=NULL,*current_copy;
    bool_t up = false;
    if((ancestor_node==NULL)||(current_node==NULL))
    {
        return NULL;
    }
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    if((current_node != ancestor_node) || ancestor_node->child)
    {
        current_copy = current_node;
        do
        {
            if((up == false) && (current_copy->child != NULL))
            {   //�ӽ��ǿ�,�����ӽ��
                result = current_copy->child;
                break;
            }else if(current_copy->next != current_copy->parent->child)
            {   //�ӽ���,������㲻�Ƕ���β���,����next���
                result = current_copy->next;
                break;
            }else
            {   //���ӽ��,�ұ�����Ѿ��Ƕ���β���,��Ҫ�жϵ�ǰ������next���
                current_copy = current_copy->parent;
                up = true;
            }
        }while(current_copy != ancestor_node);
    }
    semp_post(&tg_semp_rsc_root);
    return result;
}

//----���ֵܽ����������Դ-----------------------------------------------------
//����: ��ĳһ����Դ���������ֵܽ���������������ֵ���Դ
//����: brother,�ֵܽ��������һ����Դָ��.
//      name,��Ҫ��������Դ��
//����: �����������Դ������Դ���ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_search_sibling(struct  rsc_node *brother,char *name)
{
    struct  rsc_node *temp,*result = NULL;
    if((brother == NULL)||(name == NULL))
        return NULL;
    temp = brother;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    do
    {
        if(strcmp(temp->name,name)==0)
        {
            result = temp;
            break;
        }
        temp = temp->next;
    }while (temp != brother);
    semp_post(&tg_semp_rsc_root);
    return result;
}

//----���ӽ����������Դ-----------------------------------------------------
//����: ��ĳһ����Դ���������ӽ���������������ֵ���Դ
//����: parent,�������Դָ��.
//      name,��Ҫ��������Դ��,����Դ��,������·��.
//����: �����������Դ������Դ���ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_search_son(struct  rsc_node *parent,char *name)
{
    struct  rsc_node *temp,*result = NULL;
    if((name == NULL)||(parent == NULL))
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
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
    semp_post(&tg_semp_rsc_root);
    return result;
}

//----�ں����������Դ��-------------------------------------------------------
//����: ��rsc_search_layer����,����ancestor_node�����к�����,ֱ���ҵ�һ������
//      ƥ����Դ��
//����: ancestor_node,��֦�ĸ����
//      name,��Ҫ��������Դ��,����Դ��,������·��.
//����: �����������Դ������Դ���ָ��,���򷵻�NULL
//------------------------------------------------------------------------------
struct rsc_node *rsc_search_scion(struct rsc_node *ancestor_node,char *name)
{
    struct  rsc_node  *current,*temp,*result = NULL;
    if((ancestor_node == NULL)||(name == NULL))
        return NULL;
    current = ancestor_node;
    //��rsc_trave_scion���Ѿ����ź����������˴����뱣��
    while((temp = rsc_trave_scion(ancestor_node,current)) != NULL)
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
//����: ��rsc_search_scion����,��ͬ���ǣ�path�ǰ���·�����Ľ�㣬ִ�о�ȷ������
//����: ancestor_node,��֦�ĸ����
//      path,����·��������Դ��
//����: �����������Դ������Դ���ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct  rsc_node *rsc_search(struct  rsc_node *ancestor_node,char *path)
{
    char *path_name,*dest_name;
    bool_t   match;
    u32 i;

    struct  rsc_node *current,*result = NULL;
    if((path == NULL)||(ancestor_node == NULL))
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    current = ancestor_node->child;
    if(current == NULL)
    {
        semp_post(&tg_semp_rsc_root);
        return NULL;
    }
    path_name = path;
    while(1)
    {
        dest_name = current->name;
        match=false;
        for(i=0;i<cn_rsc_name_limit;i++)   //��Դ����������
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
        {   //��������ֲ�ƥ��,������һ�����
            current = current->next;
            if(current == current->parent->child)
            //���������н���Ѿ�������,û���ҵ�ƥ���.
                break;
        }else
        {   //���������ƥ��,���ǻ�û��path������,���������ӽ��
            path_name += i+1;
            current = current->child;
            if(current == NULL)
            //������Դ���Ѿ���������,��path_name��δ����
                break;
        }
    }
    semp_post(&tg_semp_rsc_root);
    return result;
}

//----�Ƿ����ͷ-------------------------------------------------------------
//����: �жϸ�������Ƿ��ֵܽ���еĶ���ͷ���
//����: node�������Խ��
//����: true=�Ƕ���ͷ��false=���Ƕ���ͷ
//-----------------------------------------------------------------------------
bool_t rsc_is_head(struct rsc_node *node)
{
    if(node == pg_rsc_root)
        return true;
    if(node  == node->parent->child)
        return true;
    else
        return false;
}

//----�Ƿ����β---------------------------------------------------------------
//����: �жϸ�������Ƿ��ֵܽ���е����һ�����
//����: node�������Խ��
//����: true=�����һ����㣬false=����
//-----------------------------------------------------------------------------
bool_t rsc_is_last(struct rsc_node *node)
{
    if(node == pg_rsc_root)
        return true;
    if(node->next  == node->parent->child)
        return true;
    else
        return false;
}

//----ȡ���˳��-------------------------------------------------------------
//����: ȡ����������ֵܽ���е�λ�ã�head=0����һ����1������֮
//����: node�������Խ��
//����: ���λ�ú�
//-----------------------------------------------------------------------------
u32 rsc_node_ranking(struct rsc_node *node)
{
    u32 ranking;
    struct rsc_node *temp;
    if(node == pg_rsc_root)
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

