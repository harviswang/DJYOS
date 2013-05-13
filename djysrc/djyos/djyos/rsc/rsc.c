//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 资源管理
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 创建、添加、移动、删除、搜索资源结点的服务
//其他说明:
//修订历史:
//3. 日期:20090518
//   作者:  罗侍田.
//   新版本号：v1.2.0
//   修改说明:
//      1.在概念上，分开了树根和资源根，函数名也相应地调整了
//      2.增加rsc_get_root函数，原rsc_get_root改名为rsc_get_tree，且不再兼职
//      3.因有了rsc_get_root，取消1.1.0版本的"修改3"
//      4.补全了NULL参数检查
//2. 日期:20090131
//   作者:  罗侍田.
//   新版本号：v1.1.0
//   修改说明:
//      1.加了个总根节点，添加了模块初始化函数
//      2.用信号量保护并发访问安全
//      3.改进了遍历函数，允许用NULL代表遍历整个队列
//      4.修正了一些bug
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>

//-------------资源链算法说明----------------------
//各同级结点连成双向循环链表
//每个结点的child指针直接指向的结点称为该结点的所在队列的队列头，它的next结点
//是队列尾。
//所有结点的Parent指针均指向父结点

//特别提示，资源队列的并发访问安全由资源管理模块负责，资源结点自身的并发访问
//          保护由使用者负责。

static struct rsc_node tg_rsc_root;
static struct rsc_node *pg_rsc_root;
static struct semaphore_LCB tg_semp_rsc_root;

//----初始化资源管理模块-------------------------------------------------------
//功能: 创建总根结点，本函数被分裂成两个，第二个函数必须在执行完锁初始化后才能
//      调用，第一个则必须在锁初始化执行前调用
//参数: 无
//返回: 1
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
//----添加根结点----------------------------------------------------------------
//功能: 在资源链中添加一个根结点(特供锁模块，因此时锁尚未初始化，)
//参数: node,新添加的结点指针
//      size，新结点连负载的尺寸，字节数
//      name，资源名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 新加入的结点
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

//----添加根结点----------------------------------------------------------------
//功能: 在资源链中添加一个根结点
//参数: node,新添加的结点指针
//      size，新结点连负载的尺寸，字节数
//      name，资源名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 新加入的结点
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
    //资源队列中至少存在一个信号量结点，无须判断是否空
    root_node = pg_rsc_root->child;
    node->next = root_node;
    node->previous = root_node->previous;
    root_node->previous->next = node;
    root_node->previous = node;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----插入结点------------------------------------------------------------------
//功能: 在资源链表中插入一个结点，新结点在原结点的previous位置
//参数  node,在此结点前面插入结点
//      new_node,待插入的新结点
//      size，新结点连负载的尺寸，字节数
//      name，资源名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 新加入的结点
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

//----增加结点------------------------------------------------------------------
//功能: 在资源链表中增加一个结点，新结点在原结点的next位置
//参数  node,在此结点后面插入结点
//      new_node,待插入的新结点
//      size，新结点连负载的尺寸，字节数
//      name，资源名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 新加入的结点
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

//----插入子结点----------------------------------------------------------------
//功能: 给指定结点增加一个子结点,新结点将在队列尾位置
//参数  parent_node,新结点的父结点
//      new_node,待插入的新结点
//      size，新结点连负载的尺寸，字节数
//      name，资源名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 新加入的结点
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

//----插入一个头结点---------------------------------------------------------
//功能: 给指定结点增加一个子结点,新结点将在队列头位置
//参数  parent_node,新结点的父亲结点
//      new_node,待插入的新结点
//      size，新结点连负载的尺寸，字节数
//      name，资源名字，所指向的字符串内存区不能是局部变量，可以是空
//返回: 新加入的结点
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

//----替换结点-----------------------------------------------------------------
//功能: 用一个新结点替换掉资源队列中的原有结点，新结点原来必须不是队列中的节点
//参数: oldnode，被替换的节点
//      newnode，新结点
//返回: 无
//-----------------------------------------------------------------------------
bool_t rsc_displace_node(struct rsc_node *oldnode,struct rsc_node *newnode)
{
    struct rsc_node *temp1,*temp2;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;
    temp1 = oldnode->child;
    if(temp1 != NULL)   //把oldnode所有子结点的父指针指向newnode
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
    if(oldnode->parent->child == oldnode) //如果是队列头结点
        oldnode->parent->child = newnode; //父结点的子结点指向队列头结点
    semp_post(&tg_semp_rsc_root);
    return true;
}

//----删除一个树枝-------------------------------------------------------------
//功能: 把一个树枝从资源队列中删除
//参数: branch，被删除的分支。
//返回: 被删除分支指针，NULL表示分支不存在
//-----------------------------------------------------------------------------
struct  rsc_node * rsc_del_branch(struct  rsc_node *branch)
{
    struct  rsc_node * result;
    if(branch == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    if(branch->next == branch)   //说明该结点没有兄弟结点.
    {
        branch->parent->child = NULL;
    }else
    {
        if(branch->parent->child == branch)
        {   //说明该结点是队列头结点,需要改变队列头结点
            branch->parent->child = branch->next;
        }
        branch->previous->next = branch->next;
        branch->next->previous = branch->previous;
    }
    result = branch;
    
    semp_post(&tg_semp_rsc_root);
    return result;
}
//---删除一个结点---------------------------------------------------------------
//功能: 把一个结点从资源链表中断开结点,该结点不能有子结点
//参数: node,被删除的结点,如该结点有子结点则不删除
//返回: 返回被删除节点指针，出错则返回NULL
//------------------------------------------------------------------------------
struct  rsc_node * rsc_del_node(struct  rsc_node *node)
{
    struct  rsc_node * result;
    if(node == NULL)
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    if(node->child != NULL) //子结点非空,不操作
        result = NULL;
    else
    {
        if(node->next == node)   //说明该结点没有兄弟结点.
        {
            node->parent->child = NULL;
        }else
        {
            if(node->parent->child == node)
            {   //说明该结点是队列头结点,需要改变队列头结点
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

//---移动一棵树---------------------------------------------------------------
//功能: 移动一棵树到别的结点下面成为其子树
//参数: node,被移动的结点指针
//返回: true = 成功执行，false = 失败
//-----------------------------------------------------------------------------
bool_t rsc_moveto_tree(struct rsc_node *parent,struct  rsc_node *node)
{
    return true;
}

//----移动结点到最后---------------------------------------------------------
//功能: 朝next指针方向移动资源队列中的一个结点，到同级队列头的前一个接点位置
//参数: node,被移动的结点指针
//返回: 无
//-----------------------------------------------------------------------------
bool_t rsc_moveto_last(struct  rsc_node *node)
{
    struct  rsc_node *eldest;
    if(node == NULL)
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;

    if(node->parent != NULL)    //根结点不能移动
    {
        eldest = node->parent->child;
        if(eldest == node)
        //如果node是头结点,则直接移动父结点的子指针到下一个结点就可以了.
            node->parent->child = node->next;
        //以下从链表中取出结点
        node->next->previous = node->previous;
        node->previous->next = node->next;
        //以下把node插入队列尾位置,由于是循环链表,头结点的前面就是尾结点.
        node->next = eldest;
        node->previous = eldest->previous;
        eldest->previous->next = node;
        eldest->previous = node;
    }
    semp_post(&tg_semp_rsc_root);
    return true;
}

//----移动结点成为队列头-------------------------------------------------------
//功能: 朝previous指针方向移动资源队列中的一个结点，成为同级队列头
//参数: node,被移动的结点指针
//返回: true = 成功执行，false = 失败
//------------------------------------------------------------------------------
bool_t rsc_moveto_head(struct  rsc_node *node)
{
    if(node == NULL)
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;
    if(node->parent)    //根结点不能移动
    {
        rsc_moveto_last(node);
        node->parent->child = node;
    }
    semp_post(&tg_semp_rsc_root);
    return false;
}

//----移动结点到某结点next位置-------------------------------------------------
//功能: 移动资源队列中的一个结点node到另一个结点elder的next位置
//参数: node,被移动的结点指针
//      elder,目标结点,node移动到本结点后面
//返回: true = 成功执行，false = 失败
//------------------------------------------------------------------------------
bool_t rsc_moveto_next(struct  rsc_node *elder,struct  rsc_node *node)
{
    if((elder==NULL)||(node==NULL))
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;

    //以下从链表中取出结点
    node->next->previous = node->previous;
    node->previous->next = node->next;
    //以下把node插入小弟位置,由于是循环链表,长兄结点的前面就是小弟结点.
    node->previous = elder;
    node->next = elder->next;
    elder->next->previous = node;
    elder->next = node;

    semp_post(&tg_semp_rsc_root);
    return false;
}

//----移动结点到某结点previous位置---------------------------------------------
//功能: 移动资源队列中的一个结点node到另一个结点lesser的previous位置
//参数: node,被移动的结点指针
//      lesser,目标结点,node移动到本结点前面
//返回: true = 成功执行，false = 失败
//------------------------------------------------------------------------------
bool_t rsc_moveto_previous(struct  rsc_node *lesser,struct  rsc_node *node)
{
    if((lesser==NULL)||(node==NULL))
        return false;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return false;

    //以下从链表中取出结点
    node->next->previous = node->previous;
    node->previous->next = node->next;
    //以下把node插入小弟位置,由于是循环链表,长兄结点的前面就是小弟结点.
    node->next = lesser;
    node->previous = lesser->previous;
    lesser->previous->next = node;
    lesser->previous = node;

    semp_post(&tg_semp_rsc_root);
    return false;
}

//----队列头位置后移--------------------------------------------------------------
//功能: parent的子接点的相对位置不变,队列头朝previous方向移动一格。
//参数: parent,父结点指针
//返回: true = 成功执行，false = 失败
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

//----队列头位置前移-----------------------------------------------------------
//功能: parent的子接点的相对位置不变,队列头朝next方向移动一格。
//参数: parent,父结点指针
//返回: true = 成功执行，false = 失败
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

//----资源更名-----------------------------------------------------------------
//功能: 修改资源名
//参数: node，被修改的资源结点指针
//      new_name，新名字
//返回: true = 成功执行，false = 失败
//-----------------------------------------------------------------------------
bool_t rsc_rename_node(struct rsc_node *node,char *new_name)
{
    if((node == NULL) || (new_name == NULL))
        return false;
    node->name = new_name;  //无需判断新名字字符串的长度，资源允许名字是空串
    return false;
}

//----返回树根结点-------------------------------------------------------------
//功能: 返回指定结点所在的树的根结点指针
//参数: scion_node,目标结点。
//返回: 根结点指针
//----------------------------------------------------------------------------
struct  rsc_node *rsc_get_tree(struct  rsc_node *scion_node)
{
    struct  rsc_node *node = scion_node;
    if(node == NULL)    //目标结点空
        return NULL;
    if(semp_pend(&tg_semp_rsc_root,cn_timeout_forever) ==false)
        return NULL;
    while(node->parent != pg_rsc_root)
        node = node->parent;
    semp_post(&tg_semp_rsc_root);
    return node;
}

//----返回资源根结点-----------------------------------------------------------
//功能: 返回整个资源系统的根结点指针
//参数: scion_node,目标结点。
//返回: 资源树指针
//----------------------------------------------------------------------------
struct  rsc_node *rsc_get_root(void)
{
    return pg_rsc_root;
}

//----查找资源树---------------------------------------------------------------
//功能: 在资源根结点的直接子结点中查找名为name的资源树
//参数: 资源名称
//返回: 资源树结点指针。NULL = 没找到
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
//----返回父结点---------------------------------------------------------------
//功能: 返回某结点的父结点,
//参数: son_node,需要查找的树枝的任意一个子结点
//返回: 父结点.
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

//----返回子结点--------------------------------------------------------------
//功能: 返回子结点，因父结点的child指针指向子接点的队列头，实际返回子接点的队列头
//参数: parent_node,需要查找的树枝的父结点
//返回: 子结点队列头
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

//----返回前一个结点-----------------------------------------------------------
//功能: 返回next_node结点的previous接点
//参数: next_node,需要查找的树枝的兄结点中的一个接点
//返回: previous结点.
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

//----返回弟结点--------------------------------------------------------------
//功能: 返回previous_node结点的next接点
//参数: previous_node,需要查找的树枝的兄结点中的一个接点
//返回: next结点.
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

//----返回队列头---------------------------------------------------------------
//功能: 返回rnode结点所在队列的队列头
//参数: rnode,需要查找的树枝的任意结点
//返回: 队列头指针.
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

//----返回树枝一个末梢结点-----------------------------------------------------
//功能: 返回某树枝的一个末梢结点,末梢结点是指没有子结点的结点.
//参数: parent_node,需要搜索的树枝的父结点
//返回: 树枝的一个末梢结点.当没有子结点时，返回NULL
//备注: 当需要删除整个树枝时,本函数很有用,结合rsc_del_node函数,反复调用本函数,
//      并把返回的结点删除,直到本函数返回NULL.需要删除一个文件夹或者删除一个存
//      在子窗口的gui窗口时,就需要用到删除整个树枝的操作.
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

//----取结点的级别-----------------------------------------------------
//功能: 查看某结点是根节点的第几级子节点，
//参数: node,被查的结点
//返回: 子节点级数，0代表根节点。cn_limit_uint32代表出错。
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

//----设置资源名字-------------------------------------------------------------
//功能: 设置资源的名字，注意，保存名字的字符串存储空间由用户负责，资源管理只针对
//      字符串指针。
//参数: node，被操作的资源结点
//      newname，新的名字指针
//返回: 无
//-----------------------------------------------------------------------------
void rsc_set_name(struct rsc_node *node,char *newname)
{
    node->name = newname;
    return;
}
//----遍历一个树枝--------------------------------------------------------------
//功能: 从当前结点开始,获取下一个结点的指针,沿着搜索路线,直到搜索完整个树枝.搜索
//      路线为:当前结点的子结点,如果子结点为空则搜索next结点,next结点又为空则搜
//      索父结点的next结点,直到搜索完成.
//      本函数只返回子孙结点，不返回ancestor_node本身
//参数: parent_node,需要搜索的树枝的祖先结点
//      current_node,当前搜索位置,
//返回: 当前搜索位置的下一个结点指针,如果已经搜索完成,则返回NULL.
//备注: 当需要对资源链表中某一个树枝或者整个链表中的结点逐一进行某种操作时,可
//      反复调用本函数,第一次调用current_node = parent_node,其后current_node
//      = 上次返回值，直到返回空.
//      本函数按父、子、孙、曾孙....的顺序搜索，先搜直系，再搜旁系，确保所有子孙
//      结点都能够访问到，如果对访问顺序有特殊要求，不能使用本函数
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
            {   //子结点非空,返回子结点
                result = current_copy->child;
                break;
            }else if(current_copy->next != current_copy->parent->child)
            {   //子结点空,但本结点不是队列尾结点,返回next结点
                result = current_copy->next;
                break;
            }else
            {   //无子结点,且本结点已经是队列尾结点,需要判断当前父结点的next结点
                current_copy = current_copy->parent;
                up = true;
            }
        }while(current_copy != ancestor_node);
    }
    semp_post(&tg_semp_rsc_root);
    return result;
}

//----在兄弟结点中搜索资源-----------------------------------------------------
//功能: 在某一个资源结点的所有兄弟结点中搜索给定名字的资源
//参数: brother,兄弟结点中任意一个资源指针.
//      name,需要搜索的资源名
//返回: 如果搜索到资源返回资源结点指针,否则返回NULL
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

//----在子结点中搜索资源-----------------------------------------------------
//功能: 在某一个资源结点的所有子结点中搜索给定名字的资源
//参数: parent,父结点资源指针.
//      name,需要搜索的资源名,纯资源名,不包含路径.
//返回: 如果搜索到资源返回资源结点指针,否则返回NULL
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

//----在后代中搜索资源名-------------------------------------------------------
//功能: 与rsc_search_layer类似,搜索ancestor_node的所有后代结点,直到找到一个名称
//      匹配资源。
//参数: ancestor_node,树枝的父结点
//      name,需要搜索的资源名,纯资源名,不包含路径.
//返回: 如果搜索到资源返回资源结点指针,否则返回NULL
//------------------------------------------------------------------------------
struct rsc_node *rsc_search_scion(struct rsc_node *ancestor_node,char *name)
{
    struct  rsc_node  *current,*temp,*result = NULL;
    if((ancestor_node == NULL)||(name == NULL))
        return NULL;
    current = ancestor_node;
    //在rsc_trave_scion中已经有信号量保护，此处无须保护
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

//----沿路径搜索资源名---------------------------------------------------------
//功能: 与rsc_search_scion类似,不同的是，path是包含路径名的结点，执行精确搜索。
//参数: ancestor_node,树枝的父结点
//      path,包含路径名的资源名
//返回: 如果搜索到资源返回资源结点指针,否则返回NULL
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
        for(i=0;i<cn_rsc_name_limit;i++)   //资源名长度限制
        {
            if((path_name[i] == 0)||((path_name[i]=='\\')&&(path_name[i+1]==0)))
            {   //已经达到字符串结束,且相等
                result = current;
                break;
            }else if(path_name[i] == '\\')
            {   //字符串相等,但path_name未结束
                match = true;
                break;
            }
            if(path_name[i] != dest_name[i])
            {   //字符串不相等
                match = false;
                break;
            }
        }
        if(result != NULL)  //名字匹配且到达path字符串末,查找结束
            break;

        if(match != true)
        {   //本结点名字不匹配,查找下一个结点
            current = current->next;
            if(current == current->parent->child)
            //本级的所有结点已经搜索完,没有找到匹配的.
                break;
        }else
        {   //本结点名字匹配,但是还没到path串结束,继续查找子结点
            path_name += i+1;
            current = current->child;
            if(current == NULL)
            //或者资源链已经搜索结束,但path_name并未结束
                break;
        }
    }
    semp_post(&tg_semp_rsc_root);
    return result;
}

//----是否队列头-------------------------------------------------------------
//功能: 判断给定结点是否兄弟结点中的队列头结点
//参数: node，被测试结点
//返回: true=是队列头，false=不是队列头
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

//----是否队列尾---------------------------------------------------------------
//功能: 判断给定结点是否兄弟结点中的最后一个结点
//参数: node，被测试结点
//返回: true=是最后一个结点，false=不是
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

//----取结点顺序-------------------------------------------------------------
//功能: 取给定结点在兄弟结点中的位置，head=0，下一个是1，类推之
//参数: node，被测试结点
//返回: 结点位置号
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

