//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: �ں�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ���ز���ϵͳ
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����:  ������.
//   �°汾�ţ�V1.1.0
//   �޸�˵��: ԭ�汾�У���ϵͳ��ʼ��Ҳ���������ˣ����汾����֮
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
//#include <string.h>
#include "int_hard.h"
#include "copy_table.h"

#if cn_cache_used == 1
#include "cache.h"
#endif

extern struct copy_table sysload_copy_table;

//----����ϵͳ�ں˼��س���-----------------------------------------------------
//���ܣ��������в���ϵͳ�ں˴��룬�Լ���siģʽ��ȫ��Ӧ�ó�����롣
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
void loader(void)
{
    u32 *src,*des;
    u32 i, j;

    for(i=0; i<sysload_copy_table.record_cnt; i++) {
		src = (u32*) sysload_copy_table.record[i].load_start_address;
		des = (u32*) sysload_copy_table.record[i].run_start_address;
		if(sysload_copy_table.record[i].type == 1) {	//copy
	    	if(src != des) {
				for(j=0; j<sysload_copy_table.record[i].size; src++,des++) {
					*des=*src;
					j+=4;
				}
	    	}
		} else if(sysload_copy_table.record[i].type == 0) {	//zero init
			for(j=0; j<sysload_copy_table.record[i].size; des++) {
				*des=0;
				j+=4;
			}
		}
    }

#if cn_cache_used == 1
    cache_clean_data();
    cache_invalid_inst();
#endif
}

//----����ϵͳ�ں˼��س���-----------------------------------------------------
//���ܣ��������в���ϵͳ�ں˴��룬�Լ���siģʽ��ȫ��Ӧ�ó�����롣
//����: �ޡ�
//����: �ޡ�
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
//----------------------------------------------------------------------------
void pre_start(void)
{
#ifdef debug
        loader();
#endif
        int_init();

        critical();
#ifndef debug
        loader();
#endif
    start_sys();        //��ʼ����ϵͳ
}
