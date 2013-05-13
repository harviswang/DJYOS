//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
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
#define cn_rsc_name_limit   255

struct rsc_node
{
    struct rsc_node *next,*previous,*parent,*child;
    u32  node_size;    //����node�����ݽṹ�ĳߴ磬���ڵ���
    char *name;       //��Դ��,�������ļ�ϵͳΪ�ļ�����Ŀ¼��,�����豸���豸��
                      //����gui���Ǵ�����.
};

ptu32_t module_init_rsc1(ptu32_t para);
ptu32_t module_init_rsc2(ptu32_t para);
struct  rsc_node * rsc_add_tree_root(struct  rsc_node *node,u32 size,char *name);
struct  rsc_node * rsc_insert_node(struct  rsc_node *node,struct  rsc_node *new_node,
                                u32 size,char *name);
struct  rsc_node * rsc_add_node(struct  rsc_node *node,struct  rsc_node *new_node,
                                u32 size,char *name);
struct  rsc_node * rsc_add_son(struct  rsc_node *parent_node,
                               struct  rsc_node *new_node,
                               u32 size,char *name);
struct  rsc_node * rsc_add_head_son(struct  rsc_node *parent_node,
                                    struct  rsc_node *new_node,
                                    u32 size,char *name);
bool_t rsc_displace_node(struct rsc_node *oldnode,struct rsc_node *newnode);
struct  rsc_node * rsc_del_branch(struct  rsc_node *branch);
struct  rsc_node * rsc_del_node(struct  rsc_node *node);
bool_t rsc_moveto_tree(struct rsc_node *parent,struct  rsc_node *node);
bool_t rsc_moveto_last(struct  rsc_node *node);
bool_t rsc_moveto_head(struct  rsc_node *node);
bool_t rsc_moveto_next(struct  rsc_node *elder,struct  rsc_node *node);
bool_t rsc_moveto_previous(struct  rsc_node *lesser,struct  rsc_node *node);
bool_t rsc_round_previous(struct  rsc_node *parent);
bool_t rsc_round_next(struct  rsc_node *parent);
bool_t rsc_rename_node(struct rsc_node *node,char *new_name);
struct  rsc_node *rsc_get_tree(struct  rsc_node *scion_node);
struct  rsc_node *rsc_search_tree(char *tree_name);
struct  rsc_node *rsc_get_root(void);
struct  rsc_node *rsc_get_parent(struct  rsc_node *son_node);
struct  rsc_node *rsc_get_son(struct  rsc_node *parent_node);
struct  rsc_node *rsc_get_previous(struct  rsc_node *next_node);
struct  rsc_node *rsc_get_next(struct  rsc_node *previous_node);
struct  rsc_node *rsc_get_head(struct  rsc_node *rnode);
struct  rsc_node *rsc_get_twig(struct  rsc_node *parent_node);
u32 rsc_get_class(struct  rsc_node *node);
struct  rsc_node *rsc_trave_scion(struct  rsc_node *parent_node,
                                 struct  rsc_node *current_node);
struct  rsc_node *rsc_search_sibling(struct  rsc_node *layer,char *name);
struct  rsc_node *rsc_search_son(struct  rsc_node *parent,char *name);
struct  rsc_node *rsc_search_scion(struct  rsc_node *parent_node,char *name);
struct  rsc_node *rsc_search(struct  rsc_node *parent_node,char *path);
u32 rsc_node_ranking(struct rsc_node *node);
bool_t rsc_is_last(struct rsc_node *node);
bool_t rsc_is_head(struct rsc_node *node);

#ifdef __cplusplus
}
#endif

#endif //__rsc_h__

