//----------------------------------------------------
//Copyright (C), 2004-2011,  luoshitian.
//��Ȩ���� (C), 2004-2011,   ������.
//����ģ��:������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:cache������صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-10-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CACHE_H__
#define __CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

enum cache_operation_type
{
    write_to_no_invalid_no_lock,    //дͨ������Ҫ�������֧������
};
struct cache_type_flag
{
    u32 ints_cache_type:12;     //ָ��cache����
    u32 data_cache_type:12;     //����cache����
    u32 uni_cache:1;            //1=ָ������ݶ���cache��0=ͳһcache
    u32 operation_type:4;       //cache�������͡�
};

#if cn_cache_used == 1

#if defined( arm720t)
    #define cn_cache_size       0x2000
    #define cn_cache_way        4
    #define cn_cache_line_size  16
#elif defined( arm740t)     //740t cache size�ɱ䣬��arch.h�ж���cache�ߴ�
    #define cn_cache_way        4
    #define cn_cache_line_size  16
#elif defined( arm920t)
    #define cn_cache_size       0x4000
    #define cn_cache_way        64
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //��������c7�е�ƫ��λ��
    #define cn_c7_offset_way    26  //·������c7�е�ƫ��λ��
    #define cn_c9_offset_way    26  //·������c9�е�ƫ��λ��
#elif defined( arm922t)
    #define cn_cache_size       0x2000
    #define cn_cache_way        64
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //��������c7�е�ƫ��λ��
    #define cn_c7_offset_way    26  //·������c7�е�ƫ��λ��
    #define cn_c9_offset_way    26  //·������c9�е�ƫ��λ��
#elif defined( arm940t)
    #define cn_cache_size       0x1000
    #define cn_cache_way        64
    #define cn_cache_line_size  16
    #define cn_c7_offset_set    4   //��������c7�е�ƫ��λ��
    #define cn_c7_offset_way    26  //·������c7�е�ƫ��λ��
    #define cn_c9_offset_way    0   //·������c9�е�ƫ��λ��
#elif defined( arm926ej_s)  //926ej_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define cn_cache_way        4
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    4   //��������c7�е�ƫ��λ��
    #define cn_c7_offset_way    30  //·������c7�е�ƫ��λ��
#elif defined( arm946e_s)    //946e_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define cn_cache_way        4
    #define cn_cache_line_size  16
    #define cn_c7_offset_set    4   //��������c7�е�ƫ��λ��
    #define cn_c7_offset_way    30  //·������c7�е�ƫ��λ��
    #define cn_c9_offset_way    0   //·������c9�е�ƫ��λ��
#elif defined( arm1022e)
    #define cn_cache_size       0x4000
    #define cn_cache_way        64
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //��������c7�е�ƫ��λ��
    #define cn_c7_offset_way    26  //·������c7�е�ƫ��λ��
    #define cn_c9_offset_way    26  //·������c9�е�ƫ��λ��
#elif defined( arm1026ej_s)//1026ej_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define cn_cache_way        4
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //��������c7�е�ƫ��λ��
    #define cn_c7_offset_way    30  //·������c7�е�ƫ��λ��
#elif defined( strong_arm)
    #define cn_cache_size       0x4000
    #define cn_cache_way        32
    #define cn_cache_line_size  16
#elif defined( xscale)
    #define cn_cache_size       0x8000
    #define cn_cache_way        32
    #define cn_cache_line_size  32
#endif

void cache_invalid_all(void);
void cache_invalid_inst(void);
void cache_invalid_data(void);
void cache_enable_all(void);
void cache_enable_inst(void);
void cache_enable_data(void);
void cache_disable_all(void);
void cache_disable_inst(void);
void cache_disable_data(void);
void cache_enable_write_buf(void);
void cache_disable_write_buf(void);
void cache_clean_data(void);


#else
//���°�cache��������ȫ���ÿա�
#define cache_invalid_all()
#define cache_invalid_inst()
#define cache_invalid_data()
#define cache_enable_all()
#define cache_enable_inst()
#define cache_enable_data()
#define cache_disable_all()
#define cache_disable_inst()
#define cache_disable_data()
#define cache_enable_write_buf()
#define cache_disable_write_buf()
#define cache_clean_data()

#endif

#ifdef __cplusplus
}
#endif

#endif /*__CACHE_H__*/

