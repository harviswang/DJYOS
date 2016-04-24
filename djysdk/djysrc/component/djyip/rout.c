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
// ģ������: ͨ�ŵ���·��ѡ���ά��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 11:35:21 AM/Jul 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "os.h"

#include "arp.h"
#include "rout.h"
#include "tcpip_cdef.h"

//RouteTabTree
//������Դ������Ϊ������ʱ�����ô�㣬ͨ�������������Ҳ���ô�
//RoutItem��Դ��
#define CN_NETROUTITEM_MAX         0x20
static tagNetRoutItem              sgRoutItemSpace[CN_NETROUTITEM_MAX]; //��Դ�ռ�
static tagNetRoutItem              *pgItemFreeLst;                       //���ж���

//����Rout�ļ���ȫ�ֱ���
static tagRoutTab                  *pgRoutTabLst = NULL;   //ROUT����
static tagRoutTab                  *pgLoopRout = NULL;     //LOOP�ػ���·
static tagRoutTab                  *pgRoutDefault= NULL;   //Ĭ����·
#define CN_ROUTITEMSCAN_CYCLE      (5*60*1000*mS)          //ÿ5����ɨ��
#define CN_ROUTITEM_LIFE           20                      //Ĭ�ϵ�20����������
#define CN_ROUTITEM_WAITIME        (5*mS)                  //������Ӧ�÷ǳ���
static u8  sgBroadRoutMac[6] = {0xFF, 0XFF,0XFF,0XFF,0XFF,0XFF};
// =============================================================================
// �������ܣ�����һ��RoutItem
// �����������
// ���������
// ����ֵ  �����ط���Ŀ���Item,NULL��ʾ����ʧ��
// ˵��    ��
// =============================================================================
tagNetRoutItem *__Rout_MallocItem(void)
{
	atom_low_t atom;
	tagNetRoutItem  *result = NULL;
	
	atom = Int_LowAtomStart();
	if(NULL != pgItemFreeLst)
	{
		result = pgItemFreeLst;
		pgItemFreeLst = result->next;		
		result->pre = NULL;
		result->next = NULL;	
	}
	Int_LowAtomEnd(atom);
	
	return result;
}
// =============================================================================
// �������ܣ��ͷ�һ��RoutItem
// ������������ͷŵ�RoutItem
// ���������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
void __Rout_FreeItem(tagNetRoutItem *item)
{
	atom_low_t atom;
	
	atom = Int_LowAtomStart();
	memset((char *)item,0, sizeof(tagNetRoutItem));
	item->next = pgItemFreeLst;
	if(NULL != pgItemFreeLst)
	{
		pgItemFreeLst->pre = item;
	}
	pgItemFreeLst = item;
	Int_LowAtomEnd(atom);
	
	return ;
}
// =============================================================================
// �������ܣ�����RoutItem�����ʼ��
// �����������
// �����������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
void __Rout_FreeItemInit(void)
{
	u32 i = 0;
	
	memset((char *)&sgRoutItemSpace[0],0,CN_NETROUTITEM_MAX*sizeof(tagNetRoutItem));
	
	pgItemFreeLst = &sgRoutItemSpace[0];
	pgItemFreeLst->stat = EN_ROUTITEM_FREE;
	for(i = 1; i<CN_NETROUTITEM_MAX;i++ )
	{
		sgRoutItemSpace[i].next = pgItemFreeLst;
		pgItemFreeLst->pre = &sgRoutItemSpace[i];
		pgItemFreeLst = pgItemFreeLst->pre;
		pgItemFreeLst->stat = EN_ROUTITEM_FREE;
	}
	
	return ;
}
// =============================================================================
// �������ܣ�__Rout_MatchRoutTab
//          ��Ŀ��IPƥ��һ��Rout
// ���������ipdst
// ���������
// ����ֵ  ��tagRoutTab��NULL��ʾû�з���������
// ˵��    �����ҷ���������Rout��,������ͬһ������ε�Rout,�������Ǹ��ؾ���
// =============================================================================
tagRoutTab *__Rout_MatchRoutTab(u32 ipdst)
{
	tagRoutTab *result = NULL;
	tagRoutTab *tmp = NULL;
	
	tmp = pgRoutTabLst;
	
	while(NULL != tmp)
	{

		if((ipdst &tmp->netaddr.ipmsk )== (tmp->netaddr.ip &tmp->netaddr.ipmsk))
		{
			result = tmp;
			break;
		}
		else
		{
			tmp = tmp->nxt;
		}
	}
	return result;
}
// =============================================================================
// �������ܣ�__Rout_MatchRoutItem
//          ��Rout�в���ƥ��IP��Item
// ���������rout,����������·��
//           ip����ƥ���IP
// ���������
// ����ֵ  ��ƥ��IP��routitem,NULL��ʾû��ƥ���
// ˵��    ��
// =============================================================================
tagNetRoutItem *__Rout_MatchRoutItem(tagRoutTab *routtab, u32 ip)
{
	tagNetRoutItem *result = NULL;
	tagNetRoutItem *tmp = NULL;
	tmp = routtab->itemlst;
	while(tmp != NULL)
	{
		if(ip == tmp->ip) 
		{
			result = tmp;
			break;
		}
		else
		{
			tmp = tmp->next;
		}
	}
	
	return result;
}

// =============================================================================
// �������ܣ����һ��������·
// ���������netdev,ָ���������豸
//          netaddr,netaddress
// ���������
// ����ֵ  ����ӵ���·
// ˵��    ��
// =============================================================================
tagRoutTab * Rout_AddRout(tagNetDev *netdev,tagHostIpv4Addr  *netaddr)
{
	tagRoutTab           *result;
	tagRoutTab           *tmp;

	result = NULL;
	if((NULL == netdev)||(NULL == netaddr))
	{
		result = NULL;
	}
	else
	{
		//check if any rout has the same ip and netdev
		tmp = (tagRoutTab *)pgRoutTabLst;
		while(NULL != tmp)
		{
			if((netaddr->ip == tmp->netaddr.ip)&&
				(netdev == tmp->netdev))
			{
				result = tmp;
				break;
			}
			else
			{
				tmp = tmp->nxt;
			}
		}
		if(NULL != result)
		{
			//there has been the same rout
			printk("Rout:same rout has existed!\n\r");
			result = NULL;
		}
		else
		{
			//add the rout
			result = M_Malloc(sizeof(tagRoutTab),CN_TIMEOUT_FOREVER);
			if(NULL !=  result)
			{
				//��ʼ���ã�������ӵ�RoutTab��Դ��
				result->itemlst = NULL;
				result->netdev = netdev;
				result->netaddr = *netaddr;
				result->itemsync = Lock_MutexCreate(NULL);
				
				//add it to the tablist
				result->nxt = pgRoutTabLst;
				pgRoutTabLst = result;
				//loop�������ǲ�һ����
				if(EN_LINK_INTERFACE_LOOP == netdev->iftype)
				{
					pgLoopRout = result;
				}
				else
				{
					//add the gateway item
					result->itemlst = __Rout_MallocItem();
					result->itemlst->ip = result->netaddr.gateway;
					result->itemlst->type = EN_ROUTITEM_STATIC;
					result->itemlst->stat = EN_ROUTITEM_PENDING;
					result->itemlst->life = CN_ROUTITEM_LIFE;
					if(NULL == pgRoutDefault)
					{
						//if not defined yet, we use the first rout as the  default
						pgRoutDefault = result;
					}
				}
			}
		}	
	}
	return result;
}

// =============================================================================
// �������ܣ�����ĳ����·ΪĬ����·
// ��������������õ���·
// ���������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
bool_t Rout_SetDefault(tagRoutTab *rout)
{
	bool_t result = false;
	
	if(NULL != rout)
	{
		pgRoutDefault = rout;
		result = true;
	}
	return result;
}

// =============================================================================
// �������ܣ�Rout_IsLocalIp
//          �ж�һ��IP�ǲ��Ǳ���IP
// ���������ip,���ж���IP
// ���������
// ����ֵ  ��true���Ǳ������ӣ������Ǳ�������
// ˵��    ���ú�����Ҫ�������ж��ϲ��ָ���������ǲ��Ǳ���IP
//        INADDR_ANY��127.0.0.1һ���Ǳ���IP
//        �������ROUT�����Ƿ��ж�Ӧ��ROUT
// =============================================================================
bool_t  Rout_IsLocalIp(u32 ip)
{
	bool_t result = false;
	tagRoutTab *tmp;

	if(INADDR_ANY == ip)
	{
		result = true;
	}
	else
	{
		tmp = pgRoutTabLst;
		while(NULL != tmp)   //�������е�����
		{
			if(ip == tmp->netaddr.ip)
			{
				//the rout has the same ip, so bingo
				result = true;
				break;
			}
			else
			{
				tmp = tmp->nxt;
			}
		}
	}
	return result;
}

// =============================================================================
// �������ܣ��ж�Ŀ��IP�ǲ��Ǳ���IP
// ���������ip,���ж���IP
// ���������
// ����ֵ  ��trueĿ�ĵ�ַ�Ƿ������صģ�������
// ˵��    ���ú�����Rout_IsLocalIp�������Ǹú�����Ҫ�ж��ǲ��ǹ㲥IP
// =============================================================================
bool_t  Rout_IsLocalDst(u32 ip)
{
	bool_t result = false;
	tagRoutTab *tmp;

	if(INADDR_ANY == ip)
	{
		result = true;
	}
	else
	{
		//�������е�����
		tmp = pgRoutTabLst;
		while(NULL != tmp)
		{
			if((ip == tmp->netaddr.ip)||\
			   ((ip&tmp->netaddr.ipmsk)==(tmp->netaddr.ip&tmp->netaddr.ipmsk)))
			{
				//the rout has the same ip, so bingo
				result = true;
				break;
			}
			else
			{
				tmp = tmp->nxt;
			}
		}
	}
	return result;
}
// =============================================================================
// �������ܣ�����һ��ͨ����·
// ���������ipdst,��Ҫ�����Ŀ��IP
// ���������outpara,���ҵ��Ķ�Ӧ��·
// ����ֵ  ��true�����ҳɹ���false��û�в��ҵ���ֻ��trueʱoutpara�����ݲ�������
// ˵��    ��1,��������ֱ��ʹ��LOOP����
//        2,�������ֱ��ʹ��Ĭ�ϵ���������ص�MAC��ַ
//        3,ͬһ�����������Ҷ�Ӧ��Item,������ARP����
// =============================================================================
bool_t Rout_Resolve(u32 ipdst, tagRoutLink *outpara)
{
	bool_t               result;
	tagNetRoutItem       *routitem;
	tagRoutTab           *rout;
	
	result = false;
	rout = __Rout_MatchRoutTab(ipdst);
	if(NULL == rout)
	{
		//֤��������,OK��ֻҪ���͸�Ĭ�����������ؼ���
		if(NULL != pgRoutDefault)
		{
			outpara->dev = pgRoutDefault->netdev;
			outpara->ip = pgRoutDefault->netaddr.ip;
			routitem = __Rout_MatchRoutItem(rout,pgRoutDefault->netaddr.ip);
			if(routitem->stat == EN_ROUTITEM_PENDING)
			{
				Arp_Request(rout->netdev,ipdst,rout->netaddr.ip);
				Lock_MutexPost(rout->itemsync);
				Djy_EventDelay(CN_ROUTITEM_WAITIME);
				Lock_MutexPend(rout->itemsync, CN_TIMEOUT_FOREVER);
				if(routitem->stat == EN_ROUTITEM_STABLE)
				{
					outpara->ip = rout->netaddr.ip;
					outpara->dev = rout->netdev;
					memcpy(outpara->mac,routitem->mac,6);
					result = true;
				}
			}
			else
			{
				outpara->ip = rout->netaddr.ip;
				outpara->dev = rout->netdev;
				memcpy(outpara->mac,routitem->mac,6);
				result = true;
			}			
			result = true;
		}
	}
	else
	{
		if(Lock_MutexPend(rout->itemsync, CN_TIMEOUT_FOREVER))
		{
			//�������磬��ôҪ������ʹ��LOOP��������
			if(ipdst == rout->netaddr.ip)
			{
				//loop
				if(NULL != pgLoopRout)
				{
					result = true;
					outpara->dev = pgLoopRout->netdev;
					outpara->ip =pgLoopRout->netaddr.ip;
					memcpy(outpara->mac,pgLoopRout->netdev->mac,6);
					result = true;
				}
			}
			else if(ipdst == ((~rout->netaddr.ipmsk)|((rout->netaddr.ip&rout->netaddr.ipmsk))))
			{
				//�㲥����
				outpara->ip = rout->netaddr.ip;
				outpara->dev = rout->netdev;
				memcpy(outpara->mac,sgBroadRoutMac,6);
				result = true;
			}
			else
			{
				//�������磬����Item
				routitem = __Rout_MatchRoutItem(rout,ipdst);
				if(NULL == routitem)
				{
					//��Ҫ����
					routitem = __Rout_MallocItem();
					if(NULL != routitem)
					{
						routitem->ip = ipdst;
						routitem->life = CN_ROUTITEM_LIFE;
						routitem->type = EN_ROUTITEM_DYNAMIC;
						routitem->stat = EN_ROUTITEM_PENDING;
						//add the routitem to the rout lsrt
						routitem->next = rout->itemlst;
						if(NULL != rout->itemlst)
						{
							rout->itemlst->pre = routitem;
						}
						rout->itemlst = routitem;
					}
				}
				if(routitem->stat == EN_ROUTITEM_PENDING)
				{
					Arp_Request(rout->netdev,ipdst,rout->netaddr.ip);
					Lock_MutexPost(rout->itemsync);
					Djy_EventDelay(CN_ROUTITEM_WAITIME);
					Lock_MutexPend(rout->itemsync, CN_TIMEOUT_FOREVER);
					if(routitem->stat == EN_ROUTITEM_STABLE)
					{
						outpara->ip = rout->netaddr.ip;
						outpara->dev = rout->netdev;
						memcpy(outpara->mac,routitem->mac,6);
						result = true;
					}
				}
				else
				{
					outpara->ip = rout->netaddr.ip;
					outpara->dev = rout->netdev;
					memcpy(outpara->mac,routitem->mac,6);
					result = true;
				}
			}
			Lock_MutexPost(rout->itemsync);
		}
	}
	
	return result;
}
// =============================================================================
// �������ܣ��յ�ͨ�ŷ���Ϣʱ�������������ǵ�Route��Ŀ
// ���������ip,���µ�IP
//          mac,��Ӧ��IP��MAC
// �����������
// ����ֵ  ����
// ˵��    ������IP��Ӧ��RoutItem
// =============================================================================
void Rout_ItemUpdate(u32 ip, u8 *mac)
{
    tagRoutTab        *rout;
    tagNetRoutItem    *routitem;    
    if(NULL != mac)
    {
        rout = __Rout_MatchRoutTab(ip);
        if(NULL != rout)
        {
        	if(Lock_MutexPend(rout->itemsync, CN_TIMEOUT_FOREVER))
        	{
        		routitem = __Rout_MatchRoutItem(rout,ip);
        		if(NULL!= routitem)
        		{
        			routitem->life = CN_ROUTITEM_LIFE;
        			routitem->stat = EN_ROUTITEM_STABLE;
        			memcpy(routitem->mac, mac, 6);   //������ARP����
        		}
        		else
        		{
        			//��Ȼû��ƥ��ģ�����ǹ㲥֮��ģ�Ӧ�ÿ��Խ������Ӧ��Item
        			//�Ͼ����ܺܿ�ͻ�ͨ����
        			if(ip != ((~(rout->netaddr.ip&rout->netaddr.ipmsk))|rout->netaddr.ip))
        			{
            			routitem = __Rout_MallocItem();
            			if(NULL != routitem)
            			{
    						routitem->ip = ip;
    	        			memcpy(routitem->mac, mac, 6);  
    	        			routitem->life = CN_ROUTITEM_LIFE;
    	        			routitem->stat = EN_ROUTITEM_STABLE;
    						routitem->type = EN_ROUTITEM_DYNAMIC;
    						//add the routitem to the rout lsrt
    						routitem->next = rout->itemlst;
    						if(NULL != rout->itemlst)
    						{
    							rout->itemlst->pre = routitem;
    						}
    						rout->itemlst = routitem;
            			}
        			}		
        		}
				Lock_MutexPost(rout->itemsync);
        	}
        }
    }
  
	return;
}

// =============================================================================
// �������ܣ�Rout_ItemScanTask
//          ��ʱˢ��Route����
// �����������
// ���������
// ����ֵ  ����
// ˵��    ���ú�����Route�������߳�
//        ��������Ϊ1Minute����Item�����������ս��ʱ�򣬸�Item���ᱻɾ��
// =============================================================================
void Rout_ItemScanTask(void)
{
	tagRoutTab      *rout;
	tagNetRoutItem  *item;
	tagNetRoutItem  *itemtmp;
	
	while(1)
	{
		rout = pgRoutTabLst;
		while(NULL != rout)
		{
			//ÿ��rout�ı���
			Lock_MutexPend(rout->itemsync, CN_TIMEOUT_FOREVER);
			item = rout->itemlst;
			while(NULL != item)
			{
				itemtmp = item->next;
				if(EN_ROUTITEM_DYNAMIC ==item->type)
				{
					item->life--;
					if(0 == item->life)
					{
						//del the item for it is timeout
						if(NULL != item->pre)
						{
							item->pre->next = item->next;
						}
						if(NULL != item->next)
						{
							item->next->pre = item->pre;
						}
						__Rout_FreeItem(item);
					}
				}
				item = itemtmp;
			}//ɨ��ָ��rout������item
			Lock_MutexPost(rout->itemsync);			
			rout = rout->nxt;	
		}
		Djy_EventDelay(CN_ROUTITEMSCAN_CYCLE);
	}
}
// =============================================================================
// �������ܣ�Rout_Init
//          ��·��ʼ��
// �����������
// ���������
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ��1,����Route�ڴ��
//        2,����Route�����ڴ��
//        3,����Route�����Ӧ��HOOK�ڴ��
//        3,��ʼ��Route����Դ����������������ͬ������Ϊ�Ը����ı����������ɾ��������
//          ԭ���Բ��������ڸ������в��Ǻܳ������Զ�ϵͳ����Ӱ�첻��
// =============================================================================
bool_t Rout_Init(void)
{
	u16    evtt_id;	
	bool_t result = true;
	//ȫ�ֱ�����ʼ��
	//��ʼ��Item����
	__Rout_FreeItemInit();
	//���Rout��ɨ������
    evtt_id = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1,
    		(ptu32_t (*)(void))Rout_ItemScanTask,NULL,0x1000, "Rout Tab Scan");
	if (evtt_id == CN_EVTT_ID_INVALID)
	{
		printk("Rout:Creat RoutScanEvent Failed\n\r");
		result = false;
	}
	else
	{
		evtt_id = Djy_EventPop(evtt_id, NULL, 0, NULL,NULL, 0);
		printk("Rout:Creat RoutScanEvent Success\n\r");
	}
	return result;
}





