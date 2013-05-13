//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: �ں�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: Ԥ���ز���ϵͳ
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include <string.h>
#include "copy_table.h"

#if cn_cache_used == 1
#include "cache.h"
#endif

void pre_start(void);

extern struct copy_table preload_copy_table;

//----Ԥ���س���---------------------------------------------------------------
//���ܣ����������������жϹ���ģ�飬��������
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
void load_preload(void)
{
    void (*volatile pl_1st)(void) = pre_start;

    u32 *src,*des;
    u32 i, j;
    for(i=0; i<preload_copy_table.record_cnt; i++) {
		src = (u32*) preload_copy_table.record[i].load_start_address;
		des = (u32*) preload_copy_table.record[i].run_start_address;
		if(preload_copy_table.record[i].type == 1) {	//copy
	    	if(src != des) {
				for(j=0; j<preload_copy_table.record[i].size; src++,des++) {
					*des=*src;
					j+=4;
				}
	    	}
		} else if(preload_copy_table.record[i].type == 0) {	//zero init
			for(j=0; j<preload_copy_table.record[i].size; des++) {
				*des=0;
				j+=4;
			}
		}
    }

#if cn_cache_used == 1
    cache_clean_data();
    cache_invalid_inst();
#endif
    pl_1st();   //��ָ����Զ�̵���
}

