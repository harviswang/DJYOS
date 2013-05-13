//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ���Ź�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ���Ź�ģ��
//����˵��:
//�޶���ʷ:
//2. ����:2009-02-25
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������wdt_create������һ�����󣬸�bug������sniper�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#if(cn_wdts_limit != 0)
static struct wdt_rsc tg_wdt_rsc_pool[cn_wdts_limit];
#endif
static struct mem_cell_pool *pg_wdt_rsc_pool;              //�ڴ��ͷָ�롣
static struct wdt_rsc *pg_wdt_rsc_tree;
static u32 u32g_timeout_GCD;       //���Ź����ʱ������Լ��
static u32 wdt_check_timeout;
static u32 wdt_hard_timeout = cn_limit_uint32;
static uint16_t u16g_wdt_evtt;


//----��ʼ�����Ź�ģ��--------------------------------------------------------
//���ܣ���ʼ�����Ź�ģ�飬��������Դ��㣬�����ź��������ڱ������Ź���Դ���У�
//��������
//���أ�1 = �ɹ�������0 = ����ʧ�ܡ�
//----------------------------------------------------------------------------
ptu32_t module_init_wdt(ptu32_t para)
{
    static struct wdt_rsc wdt_root;
#if(cn_wdts_limit != 0)
    pg_wdt_rsc_pool = mb_create_pool(&tg_wdt_rsc_pool,cn_wdts_limit,
                                sizeof(struct wdt_rsc),0,0,"wdt pool");
#else
    pg_wdt_rsc_pool = mb_create_pool(NULL,cn_wdts_limit,
                                sizeof(struct wdt_rsc),0,0,"wdt pool");
#endif
    //��������Դ�ڵ�
    pg_wdt_rsc_tree = (struct wdt_rsc*)
        rsc_add_tree_root(&wdt_root.wdt_node,sizeof(struct wdt_rsc),"watch dog");
    semp_create_knl(&wdt_root.wdt_semp,1,1,"watch dog");
    //��������100ms wdt_checkһ��
    u32g_timeout_GCD = 100*mS;
    wdt_check_timeout = 100*mS;
    u16g_wdt_evtt = djy_evtt_regist(enum_correlative,cn_prio_wdt,0,1,wdt_check,
                                100,"wdt service");
    if(u16g_wdt_evtt == cn_invalid_evtt_id)
        return 0;
	djy_event_pop(u16g_wdt_evtt,enum_para_async,NULL,0,NULL,0,0,0);
	return 1;
}


//----���µ����Լ��---------------------------------------------------------
//����: �����������Ϊnew_num�Ŀ��Ź��Ժ����¼��㿴�Ź�������ڵ����Լ��
//������new_num���¿��Ź����������
//���أ����Լ��
//----------------------------------------------------------------------------
u32 __wdt_inc_GCD(u32 new_num)
{
    u32 i,temp;

    temp=(new_num<u32g_timeout_GCD)? new_num:u32g_timeout_GCD;
    
    for(i=temp;i<=u32g_timeout_GCD&&i<=new_num;i--)
    {
        if((u32g_timeout_GCD%i==0)&&(new_num%i==0))
            return i;
    }

    return 1;
}

//----���¼������Լ��-------------------------------------------------------
//����: ɾ��һֻ���Ź��Ժ������¼������Լ��
//��������
//���أ����Լ��
//----------------------------------------------------------------------------
u32 __wdt_get_GCD(void)
{
    struct rsc_node *current_node;
    struct wdt_rsc *wdt;
    u32 i=0;

	current_node = &pg_wdt_rsc_tree->wdt_node;
    while(i<=cn_wdts_limit)
    {
        
        //ȡ��һ�����Ź���Դ��㣬��Ҫ���ź�������
        semp_pend(&pg_wdt_rsc_tree->wdt_semp,cn_timeout_forever);
        current_node = rsc_trave_scion(&pg_wdt_rsc_tree->wdt_node,current_node);
        semp_post(&pg_wdt_rsc_tree->wdt_semp);
        if(current_node == NULL)        //�Ѿ����������п��Ź��ڵ�
        {
            return u32g_timeout_GCD;
        }
        else                            //��δ���������п��Ź��ڵ�
        {
            wdt = (struct wdt_rsc *)current_node;
            if(i==0)
            {
                u32g_timeout_GCD = wdt->timeout;
            }
            else
            {
                u32g_timeout_GCD = __wdt_inc_GCD(wdt->timeout);   
            }
                
        }
        i++;
    }
    return u32g_timeout_GCD;
}
//----���ÿ��Ź��¼�ͬ��ʱ��----------------------------------------------------
//����: ���ÿ��Ź��¼�����Ҫ��ý���һ������ͬ�������С���Ƿ񹷽С�����Ӳ������
//      ʱ���Լ�����С������ʱ������Լ������С����0.8��ʱ�䣬��Ϊͬ��ʱ�䡣
//������hard_timeout��Ӳ������ʱ��
//���أ����Լ��
//ע��: Ӳ�����Ź���ʼ����ʱ�򣬱ض����øú���������ͬ��ʱ�䣬ͬʱ��Ҳ����
//      wdt_hard_timeout
//----------------------------------------------------------------------------
void wdt_set_check_timeout(u32 hard_timeout)
{
    u32 min;
    //���û��Ӳ�����Ź�����ôʹ��u32g_timeout_GCD��Ϊwdt_check_timeout
    if(wdt_hard_timeout == cn_limit_uint32)
        wdt_check_timeout = u32g_timeout_GCD;
    //���hard_timeout��������0����ô�����û������⺯��������Ӳ������ʱ��
    //�������0����ô��wdt_check���ã������ж��Ƿ������޸���wdt_check_timeout
    if(hard_timeout != 0)
        wdt_hard_timeout = hard_timeout;
    min = (u32g_timeout_GCD<wdt_hard_timeout)? u32g_timeout_GCD:wdt_hard_timeout;
    wdt_check_timeout = min*0.8;//0.8,ֻ�ǹ��㣬��Ϊ��С�ڹ���ʱ��֮�ڣ�ι��һ��
}

//----���һֻ���Ź�-----------------------------------------------------------
//���ܣ�����һ�����Ź�
//������wdt������ʼ���Ŀ��Ź�ָ��
//      judge���û��ṩ�������ж��Ƿ񹷽еĺ���ָ��
//      timeout������judge��ʱ��������λ��΢�룬����Ϊ0���������ϵ���Ϊ
//      cn_tick_us��������
//���أ���
//-----------------------------------------------------------------------------
struct wdt_rsc * wdt_create(bool_t (*judge)(void),
                  u32 (*yip_remedy)(void),
                  u32 timeout,char *wdt_name)
{
    struct wdt_rsc *wdt;
    if(judge == NULL)
        return NULL;
    wdt = mb_malloc(pg_wdt_rsc_pool,0);
    if(wdt == NULL)
        return NULL;
    wdt->judge = judge;
    wdt->yip_remedy = yip_remedy;
    if(timeout == 0)
        wdt->timeout = cn_tick_us;
    else
        wdt->timeout = timeout;
    semp_pend(&pg_wdt_rsc_tree->wdt_semp,cn_timeout_forever);
    if(rsc_get_son(&pg_wdt_rsc_tree->wdt_node))  //��Դ�������Ѿ��п��Ź�
    {
        rsc_add_son(&pg_wdt_rsc_tree->wdt_node,&wdt->wdt_node,
                     sizeof(struct wdt_rsc),wdt_name);
        u32g_timeout_GCD = __wdt_inc_GCD(wdt->timeout);
    }else                                   //������ӵĵ�һֻ���Ź�
    {
        rsc_add_son(&pg_wdt_rsc_tree->wdt_node,&wdt->wdt_node,
                     sizeof(struct wdt_rsc),wdt_name);
        u32g_timeout_GCD = wdt->timeout;
    }
    semp_post(&pg_wdt_rsc_tree->wdt_semp);
    return wdt;
}

//----ɾ��һֻ���Ź�-----------------------------------------------------------
//���ܣ�ɾ��һֻ���ڵĿ��Ź������¼��㹷���ʱ������Լ��
//������wdt������ʼ���Ŀ��Ź�ָ��
//���أ���
//-----------------------------------------------------------------------------
void wdt_delete(struct wdt_rsc *wdt)
{
    if(wdt == NULL)
        return;
    semp_pend(&pg_wdt_rsc_tree->wdt_semp,cn_timeout_forever);
    rsc_del_node(&wdt->wdt_node);
    semp_post(&pg_wdt_rsc_tree->wdt_semp);
    mb_free(pg_wdt_rsc_pool,wdt);
    u32g_timeout_GCD = __wdt_get_GCD();
    return;
}

//----���Ź��¼�������-------------------------------------------------------
//���ܣ����¼����Ǳ��ź���������һֱ��������������ϵͳtick��ֻҪtick�б仯����
//      ɨ��һ�����п��Ź����������Ź��趨��ʱ��������judge��������������쳣��
//      �͵��á������ƺ󡱺��������ƺ�����ָʾ��λ����������Ź������û�з���
//      �쳣����������Ź���
//������my_event�����Ź��¼�ָ�롣
//���أ���
//-----------------------------------------------------------------------------
void wdt_check(void)
{
    static uint32_t old_time,new_time;
    struct rsc_node *current_node;
    struct wdt_rsc *wdt;
    uint32_t wdt_action;

    old_time = djy_get_time();
    while(1)
    {
        //checkһ�Σ�����һ��check_timeout,��Ϊ���Ź���ʱ��������ɾ��
        wdt_set_check_timeout(0);
        djy_event_delay(wdt_check_timeout);
        new_time = djy_get_time();        //ȡϵͳʱ��
        if(new_time != old_time)        //��ϵͳtick�Ƿ�仯
        {
            current_node = &pg_wdt_rsc_tree->wdt_node;   //ȡ���Ź��ĸ���Դָ��
            while(1)        //�������Ź���Դ��������֮
            {
                //ȡ��һ�����Ź���Դ��㣬��Ҫ���ź�������
                semp_pend(&pg_wdt_rsc_tree->wdt_semp,cn_timeout_forever);
                current_node = rsc_trave_scion(&pg_wdt_rsc_tree->wdt_node,
                                                current_node);
                semp_post(&pg_wdt_rsc_tree->wdt_semp);
                if(current_node == NULL)        //�Ѿ����������п��Ź��ڵ�
                    break;
                else                            //��δ���������п��Ź��ڵ�
                {
                    wdt = (struct wdt_rsc *)current_node;
                    //��ʱ���Ƿ��Ѿ����timeoutʱ���
                    if(new_time / wdt->timeout != old_time / wdt->timeout)
                    {
                        if(! wdt->judge())      //����
                        {
                            djy_error_login(enum_wdt_yip,"watch dog yip");
                            wdt_action = wdt->yip_remedy();     //�����ƺ����
                            switch(wdt_action)
                            {
                                case cn_wdt_action_none:break;
                                case cn_wdt_action_reboot:       //�踴λ
                                {
                                   wdt_reboot();
                                }break;
                                case cn_wdt_action_hard_reset:       //�踴λ
                                {
                                    wdt_hard_reset();
                                }break;
                                default:break;
                            }
                        }
                    }
                }
            }
            old_time = new_time;
        }
       wdt_clear();        //������Ź�
    }
}


