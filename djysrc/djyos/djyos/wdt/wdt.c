//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: 看门狗模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 看门狗模块
//其他说明:
//修订历史:
//2. 日期:2009-02-25
//   作者:  罗侍田.
//   新版本号：1.0.1
//   修改说明: 修正了wdt_create函数的一处错误，该bug由网友sniper提交
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"
#if(cn_wdts_limit != 0)
static struct wdt_rsc tg_wdt_rsc_pool[cn_wdts_limit];
#endif
static struct mem_cell_pool *pg_wdt_rsc_pool;              //内存池头指针。
static struct wdt_rsc *pg_wdt_rsc_tree;
static u32 u32g_timeout_GCD;       //看门狗溢出时间的最大公约数
static u32 wdt_check_timeout;
static u32 wdt_hard_timeout = cn_limit_uint32;
static uint16_t u16g_wdt_evtt;


//----初始化看门狗模块--------------------------------------------------------
//功能：初始化看门狗模块，建立根资源结点，创建信号量（用于保护看门狗资源队列）
//参数：无
//返回：1 = 成功创建，0 = 创建失败。
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
    //建立根资源节点
    pg_wdt_rsc_tree = (struct wdt_rsc*)
        rsc_add_tree_root(&wdt_root.wdt_node,sizeof(struct wdt_rsc),"watch dog");
    semp_create_knl(&wdt_root.wdt_semp,1,1,"watch dog");
    //这里设置100ms wdt_check一次
    u32g_timeout_GCD = 100*mS;
    wdt_check_timeout = 100*mS;
    u16g_wdt_evtt = djy_evtt_regist(enum_correlative,cn_prio_wdt,0,1,wdt_check,
                                100,"wdt service");
    if(u16g_wdt_evtt == cn_invalid_evtt_id)
        return 0;
	djy_event_pop(u16g_wdt_evtt,enum_para_async,NULL,0,NULL,0,0,0);
	return 1;
}


//----求新的最大公约数---------------------------------------------------------
//功能: 加入溢出周期为new_num的看门狗以后，重新计算看门狗溢出周期的最大公约数
//参数：new_num，新看门狗的溢出周期
//返回：最大公约数
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

//----重新计算最大公约数-------------------------------------------------------
//功能: 删除一只看门狗以后，需重新计算最大公约数
//参数：无
//返回：最大公约数
//----------------------------------------------------------------------------
u32 __wdt_get_GCD(void)
{
    struct rsc_node *current_node;
    struct wdt_rsc *wdt;
    u32 i=0;

	current_node = &pg_wdt_rsc_tree->wdt_node;
    while(i<=cn_wdts_limit)
    {
        
        //取下一个看门狗资源结点，需要用信号量保护
        semp_pend(&pg_wdt_rsc_tree->wdt_semp,cn_timeout_forever);
        current_node = rsc_trave_scion(&pg_wdt_rsc_tree->wdt_node,current_node);
        semp_post(&pg_wdt_rsc_tree->wdt_semp);
        if(current_node == NULL)        //已经遍历完所有看门狗节点
        {
            return u32g_timeout_GCD;
        }
        else                            //尚未遍历完所有看门狗节点
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
//----设置看门狗事件同步时间----------------------------------------------------
//功能: 设置看门狗事件，需要多久进行一次闹钟同步，检查小狗是否狗叫。利用硬件狗叫
//      时间以及所有小狗狗叫时间的最大公约数的最小数的0.8倍时间，作为同步时间。
//参数：hard_timeout，硬件狗叫时间
//返回：最大公约数
//注意: 硬件看门狗初始化的时候，必定调用该函数，设置同步时间，同时，也设置
//      wdt_hard_timeout
//----------------------------------------------------------------------------
void wdt_set_check_timeout(u32 hard_timeout)
{
    u32 min;
    //如果没有硬件看门狗，那么使用u32g_timeout_GCD作为wdt_check_timeout
    if(wdt_hard_timeout == cn_limit_uint32)
        wdt_check_timeout = u32g_timeout_GCD;
    //如果hard_timeout，不等于0，那么就是用户调用这函数，设置硬件狗叫时间
    //如果等于0，那么是wdt_check调用，用来判断是否重新修改了wdt_check_timeout
    if(hard_timeout != 0)
        wdt_hard_timeout = hard_timeout;
    min = (u32g_timeout_GCD<wdt_hard_timeout)? u32g_timeout_GCD:wdt_hard_timeout;
    wdt_check_timeout = min*0.8;//0.8,只是估算，是为了小于狗叫时间之内，喂狗一次
}

//----添加一只看门狗-----------------------------------------------------------
//功能：创建一个看门狗
//参数：wdt，被初始化的看门狗指针
//      judge，用户提供的用于判断是否狗叫的函数指针
//      timeout，调用judge的时间间隔，单位是微秒，不能为0，将被向上调整为
//      cn_tick_us的整数倍
//返回：无
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
    if(rsc_get_son(&pg_wdt_rsc_tree->wdt_node))  //资源队列中已经有看门狗
    {
        rsc_add_son(&pg_wdt_rsc_tree->wdt_node,&wdt->wdt_node,
                     sizeof(struct wdt_rsc),wdt_name);
        u32g_timeout_GCD = __wdt_inc_GCD(wdt->timeout);
    }else                                   //这是添加的第一只看门狗
    {
        rsc_add_son(&pg_wdt_rsc_tree->wdt_node,&wdt->wdt_node,
                     sizeof(struct wdt_rsc),wdt_name);
        u32g_timeout_GCD = wdt->timeout;
    }
    semp_post(&pg_wdt_rsc_tree->wdt_semp);
    return wdt;
}

//----删除一只看门狗-----------------------------------------------------------
//功能：删除一只存在的看门狗，重新计算狗溢出时间的最大公约数
//参数：wdt，被初始化的看门狗指针
//返回：无
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

//----看门狗事件处理函数-------------------------------------------------------
//功能：本事件除非被信号量阻塞，一直就绪，持续监视系统tick，只要tick有变化，就
//      扫描一遍所有看门狗，按各看门狗设定的时间间隔调用judge函数，如果发现异常，
//      就调用“狗叫善后”函数，按善后函数的指示复位或者清除看门狗。如果没有发现
//      异常，就清除看门狗。
//参数：my_event，看门狗事件指针。
//返回：无
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
        //check一次，计算一次check_timeout,因为看门狗随时会新增、删除
        wdt_set_check_timeout(0);
        djy_event_delay(wdt_check_timeout);
        new_time = djy_get_time();        //取系统时钟
        if(new_time != old_time)        //看系统tick是否变化
        {
            current_node = &pg_wdt_rsc_tree->wdt_node;   //取看门狗的根资源指针
            while(1)        //遍历看门狗资源树并处理之
            {
                //取下一个看门狗资源结点，需要用信号量保护
                semp_pend(&pg_wdt_rsc_tree->wdt_semp,cn_timeout_forever);
                current_node = rsc_trave_scion(&pg_wdt_rsc_tree->wdt_node,
                                                current_node);
                semp_post(&pg_wdt_rsc_tree->wdt_semp);
                if(current_node == NULL)        //已经遍历完所有看门狗节点
                    break;
                else                            //尚未遍历完所有看门狗节点
                {
                    wdt = (struct wdt_rsc *)current_node;
                    //看时钟是否已经跨过timeout时间点
                    if(new_time / wdt->timeout != old_time / wdt->timeout)
                    {
                        if(! wdt->judge())      //狗叫
                        {
                            djy_error_login(enum_wdt_yip,"watch dog yip");
                            wdt_action = wdt->yip_remedy();     //调用善后程序
                            switch(wdt_action)
                            {
                                case cn_wdt_action_none:break;
                                case cn_wdt_action_reboot:       //需复位
                                {
                                   wdt_reboot();
                                }break;
                                case cn_wdt_action_hard_reset:       //需复位
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
       wdt_clear();        //清除看门狗
    }
}


