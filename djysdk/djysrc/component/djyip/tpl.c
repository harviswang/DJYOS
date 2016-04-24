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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: �������Դ������
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 7:46:28 PM/Jul 14, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "tpl.h"
// NET��Դ���ڵ�
struct tagRscNode              *pgNetRscNodeTree = NULL;  //�����ڵ�
static struct tagMutexLCB      *pgNetRscNodeSync = NULL;

typedef struct _tagNetRscNode
{
	struct tagRscNode node;
	int number;   //family �ڵ���ΪAF_INET��ֵ
	            //type   �ڵ���ΪSTREAM��ֵ
}tagNetRscNode;

// =============================================================================
// �������ܣ�TransProto_GetTypeNode
//          ����ָ����family��type�ҵ�ָ����type��Դ�ڵ�
// ���������family����,AF_INET�ȣ�
//          type,(���ͣ�SOCK_DGRAM��)
//          create,true���Ҳ�����Ӧ�Ľڵ�ͽ��������򲻽���
// �����������
// ����ֵ  �������ҵ��Ļ��߽�����type�ڵ㣬���������
// ˵��    �����û�ж�Ӧ��FAMILY��TYPE��ᴴ����Ӧ��FAMILY����TYPE
// =============================================================================
tagNetRscNode *__TPL_GetTypeNode(int family, int type, bool_t needcreate)
{
	bool_t  findit;
	tagNetRscNode *result;
	tagNetRscNode *temp;
	tagNetRscNode *headson;
	
	if(NULL != pgNetRscNodeTree)
	{
		findit = false;
		//����family
		headson = (tagNetRscNode *)Rsc_GetSon(pgNetRscNodeTree);
		if(NULL == headson) //û����Ҫ����
		{
			findit = false;
		}
		else
		{
			temp = headson;
			do{
				if(temp->number == family)
				{
					findit = true;
					result = temp;
					temp = headson;
				}
				else
				{
					temp = (tagNetRscNode *)Rsc_GetNext(&temp->node);
				}
			}while(headson != temp);
		}
		if(true == findit)  
		{
			//�ҵ���family�ڵ㣬���ڸ�family�ڵ��ϲ���type�ڵ�
			findit = false;
			headson = (tagNetRscNode *)Rsc_GetSon(&result->node);
			if(NULL == headson)
			{
				findit = false;
			}
			else
			{
				temp = headson;
				do{
					if(type == temp->number)
					{
						result = temp;
						findit = true;
						temp = headson;
					}
					else
					{
						temp = (tagNetRscNode *)Rsc_GetNext(&temp->node);
					}
					
				}while(temp != headson);
			}
			if(true != findit)//û���ҵ�type�ڵ�
			{
				if(true == needcreate)
				{
					temp = result;
					result = M_Malloc(sizeof(tagNetRscNode), CN_TIMEOUT_FOREVER);
					if(NULL != result)
					{
						result->number = type;
						Rsc_AddSon(&temp->node, &result->node,\
								    sizeof(tagNetRscNode),RSC_RSCNODE,NULL);
					}
				}
				else
				{
					result = NULL;
				}
			}
		}
		else 
		{
			//û���ҵ�family�ڵ㣬���Ƿ���Ҫ����
			if(true == needcreate)
			{
				result = M_Malloc(sizeof(tagNetRscNode), CN_TIMEOUT_FOREVER);
				if(result != NULL)
				{
					result->number = family;
					Rsc_AddSon(pgNetRscNodeTree, &result->node, \
							sizeof(tagNetRscNode),RSC_RSCNODE,NULL);
					//������Ӧ��type�ڵ�
					temp = result;
					result = M_Malloc(sizeof(tagNetRscNode), CN_TIMEOUT_FOREVER);
					if(NULL != result)
					{
						result->number = type;
						Rsc_AddSon(&temp->node, &result->node, \
								sizeof(tagNetRscNode),RSC_RSCNODE,NULL);
					}
				}
			}
			else
			{
				result = NULL;
			}
		}	
	}
	return result;
}
// =============================================================================
// �������ܣ�TPL_RegisterProto
//          ���һ������Ĵ����Э��
// ���������family����,AF_INET�ȣ�
//          type,(���ͣ�SOCK_DGRAM��)
//          protocal,��Э��ţ�
//          proto,����Ĵ����Э��
// ���������
// ����ֵ  ��ע��ɹ�����Դ���ڵ㣬NULL��ʾע�᲻�ɹ����ռ䲻�㣩
// ˵��    �����û�ж�Ӧ��FAMILY��TYPE��ᴴ����Ӧ��FAMILY����TYPE
// =============================================================================
tagTplProtoRscNode* TPL_RegisterProto(int family, int type, int protocal,\
		                           tagTlayerProto *proto)
{
	tagTplProtoRscNode *result = NULL;
	tagNetRscNode *typenode = NULL;
	tagTplProtoRscNode *headson = NULL;
	tagTplProtoRscNode *temp = NULL;

	if((NULL != proto)&&(NULL != pgNetRscNodeTree))
	{
		if(true == Lock_MutexPend(pgNetRscNodeSync, CN_TIMEOUT_FOREVER))
		{
			typenode = __TPL_GetTypeNode(family, type, true);
			if(NULL != typenode) //�ҵ����ߴ����ɹ���
			{
				headson = (tagTplProtoRscNode *)Rsc_GetSon(&typenode->node);
				if(NULL == headson)
				{
					//���ڵ�type�»�û�нڵ㣬����ȫ���Խ����µ�
					result = M_Malloc(sizeof(tagTplProtoRscNode), CN_TIMEOUT_FOREVER);
					if(NULL != result)
					{
						result->protocal = protocal;
						result->proto = *proto;
						Rsc_AddSon(&typenode->node, &result->node,\
								sizeof(tagTplProtoRscNode),RSC_RSCNODE,NULL);
					}
				}
				else
				{
					//�����Ƿ����ظ��ģ����ظ��Ĳ�������ӣ�����Ҫ���
					temp = headson;
				    do{
				    	if(temp->protocal == protocal)
				    	{
				    		result = temp;
				    		temp = headson;
				    	}
				    	else
				    	{
				    		temp = (tagTplProtoRscNode *)Rsc_GetNext(&temp->node);
				    	}
				    }while(temp != headson);
				    if(result != NULL) //�Ѿ��д��ڵ�
				    {
				    	result = NULL;
				    }
				    else
				    {
						//û��protocal�Ľڵ㣬����
						result = M_Malloc(sizeof(tagTplProtoRscNode), CN_TIMEOUT_FOREVER);
						if(NULL != result)
						{
							result->protocal = protocal;
							result->proto = *proto;
							Rsc_AddSon(&typenode->node, &result->node,\
									sizeof(tagTplProtoRscNode),RSC_RSCNODE,NULL);
						}
				    }
				}
			}
			Lock_MutexPost(pgNetRscNodeSync);
		}	
	}
	return result;
}

// =============================================================================
// �������ܣ�TPL_GetProto
//          ���һ������Ĵ����Э��
// ���������family����,AF_INET�ȣ�
//          type,(���ͣ�SOCK_DGRAM��)
//          protocal,��Э��ţ�
// ���������
// ����ֵ  ��family�����type���͵ĵ�protocal��Э�飬NULL��ʾû��ָ����Э��
// ˵��    :
// =============================================================================
tagTlayerProto *TPL_GetProto(int family, int type, int protocal)
{
	tagTlayerProto  *result;
	tagNetRscNode  *typenode;
	tagTplProtoRscNode *headson;
	tagTplProtoRscNode *temp;
	
	result = NULL;
	
	if(true == Lock_MutexPend(pgNetRscNodeSync, CN_TIMEOUT_FOREVER))
	{
		typenode = __TPL_GetTypeNode(family, type, false);
		if(NULL != typenode) //�ҵ����ߴ����ɹ���
		{
			headson = (tagTplProtoRscNode *)Rsc_GetSon(&typenode->node);
			if(NULL != headson)
			{
				//�����Ƿ����ظ��ģ����ظ��Ĳ�������ӣ�����Ҫ���
				temp = headson;
			    do{
			    	if(temp->protocal == protocal)
			    	{
			    		result = &temp->proto;
			    		temp = headson;
			    	}
			    	else
			    	{
			    		temp = (tagTplProtoRscNode *)Rsc_GetNext(&temp->node);
			    	}
			    }while(temp != headson);
			}
		}
		Lock_MutexPost(pgNetRscNodeSync);
	}
	return result;
}



// =============================================================================
// �������ܣ�TPL_Init
//          �������Դ�����ڵ��ʼ��
// �����������ʱ������
// ���������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��ע�⣬�ó�ʼ������Ӧ�������еĴ����Э���ʼ��֮ǰִ�У�������ӵ�Э����Ч
// =============================================================================
bool_t TPL_Init(ptu32_t para)
{
	bool_t result;
	static struct tagRscNode   treenode;
	
	result =false;
	//������Դ��
	pgNetRscNodeTree = Rsc_AddTree(&treenode,sizeof(struct tagRscNode), RSC_RSCNODE,NULL);
	if (NULL != pgNetRscNodeTree)
	{
		//������Դ��������
		pgNetRscNodeSync = Lock_MutexCreate(NULL);
		if (NULL == pgNetRscNodeSync)
		{
			Rsc_DelNode(pgNetRscNodeTree);
			pgNetRscNodeTree = NULL;
		}
		else
		{
			result = true;
		}
	}
	return result;
}



