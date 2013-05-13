//----------------------------------------------------
//Copyright (C), 2011.9,wjz.
//��Ȩ���� (C), 2011.9,wjz
//����ģ��: djyshģ��
//���ߣ�wjz
//�汾��V0.1.0
//�ļ�����: djyos��shell��ģ�飬�ṩshell��ʵ�ֹ����Լ���������
//�޶���ʷ:
// 1. ����:2010-09-10֮ǰ
//   ����:lst
//   �°汾�ţ�djysh0.0.1
//     �޸�˵����shellʵ�ֵ���ơ�
// 2. ����:2011-06-15
//   ����:wjz
//   �°汾�ţ�djysh0.0.2
//   �޸�˵��: ����ļ�ϵͳ�����Լ�ע���ļ�ϵͳshellģ�顣
// 2. ����:2011-09-24
//   ����:wjz
//   �°汾�ţ�djysh1.0.0
//   �޸�˵��: ����¼����¼����͡��жϡ��ڴ桢���������
//               djysh1.0.0��������Ϊһ����������shell�ˡ�
//------------------------------------------------------

#include "os_inc.h"

#if (cfg_shell == 1)

#include "char_term.h"
#include <string.h>
#include "shell_fs.h"
#include "shell.h"

bool_t (*sh_ext_command)(char *buf);

//----����ģ���ʼ������-------------------------------------------------------
//����: ��ʼ������ģ��
//����: ��
//����: 1
//-----------------------------------------------------------------------------
ptu32_t module_init_shell(ptu32_t para)
{
    uint16_t shell_evtt;
    shell_evtt = djy_evtt_regist(enum_correlative,              //�������¼�
                                cn_prio_real,       //Ĭ�����ȼ�
                                0,                  //�������������mark����Ч
                                1,                  
                                sh_service,         //��ں���
                                0x50000,                //ջ�ߴ�
                                "shell"             //�¼�������
                                );
    if(shell_evtt == cn_invalid_evtt_id)
        return false;
    if(djy_event_pop(shell_evtt,enum_para_async,NULL,0,NULL,0,0,0)
                        == cn_invalid_event_id)
    {
        djy_evtt_unregist(shell_evtt);
    }
    
    return 1;
}

void sh_install_sh_ext(bool_t (*sh_user_ext)(char *buf))
{
    if(sh_user_ext != NULL)
        sh_ext_command = sh_user_ext;
}


//----��ʾforѭ��ʱ��----------------------------------------------------------
//����: ��ʾһ��for(;;)ѭ����ִ��ʱ�䣬ѭ�������ֱ���8λ��16λ��32λ��
//����: ��
//����: ��
//-----------------------------------------------------------------------------
bool_t sh_show_for_cycle(char *param)
{
    printf_ng("32λѭ������ %dnS\r\n", djy_get_for_time(4));
    printf_ng("16λѭ������ %dnS\r\n", djy_get_for_time(2));
    printf_ng("8λѭ������ %dnS\r\n", djy_get_for_time(1));

    return true;
}

//----��ʾ��Դ����----------------------------------------------------------
//����: ������Ҫ����ʾ��Դ��������Դ�����֣�������Դ����Ϊ�գ�����ʾ"������Դ"
//����: �����ַ���������:
//          all����ʾȫ����Դ����
//          flash chip����ʾ�ļ�ϵͳ���ص�flashоƬ
//          semaphore����ʾϵͳ�е������ź���
//          mutex����ʾϵͳ�е����л�����
//          device����ʾϵͳ�е����з��豸
//          opened files����ʾ�򿪵��ļ���·��
//          pool����ʾ�����ڴ��
//����: true
//-----------------------------------------------------------------------------
bool_t sh_list_rsc(char *param)
{
    struct rsc_node *rsc_tree;
    if(param == NULL)
        sh_list_rsc_all();
    else
    {
        rsc_tree = rsc_search_tree(param);
        if(rsc_tree == NULL)
        {
            printf_ng("û�ҵ� %s ��Դ��\r\n",param);
        }
        else
            __sh_show_branche(rsc_tree);
    }
    return true;
}

//----��ʾȫ����Դ----------------------------------------------------------
//����: ��sh_list_rsc�������ã���ʾȫ����Դ
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_rsc_all(void)
{
    struct rsc_node *rsc_root;
    rsc_root = rsc_get_root();
    __sh_show_branche(rsc_root);
}

//----��ʾ��Դ��---------------------------------------------------------------
//����: ��ʾĳ��Դ�����ʼ��һ����Դ��֧������������Դ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __sh_show_branche(struct rsc_node *branche)
{
    struct rsc_node *current_node = branche;
    ucpu_t len;
    char neg[20];
    for(len = 0; len<20; len++)
        neg[len] = '-';
    while(1)
    {
        current_node = rsc_trave_scion(branche,current_node);
        if(current_node == NULL)
        {
            printf_ng("\r\n");
            break;
        }
        len = rsc_get_class(current_node);
        neg[len] = '\0';
        printf_ng("%s", neg);
        neg[len] = '-';
        if(current_node->name != NULL)
        {
            printf_ng("%s\r\n", current_node->name);
        }else
        {
            printf_ng("������Դ\r\n");
        }
    }
}

//----��ʾ�ڴ�-----------------------------------------------------------------
//����: ��ʾĳ��ַ��ʼ��һ���ڴ棬ÿ����ʾ16����Ԫ��ֻ��ʾ�������޸�
//����: param������������������Ҫ�����������ÿո����
//          ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//          ����2:��ʾ�ĵ�Ԫ��
//          ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��8��������ֵ�����ش���
//����: true=������ʾ��false=����
//-----------------------------------------------------------------------------
bool_t sh_show_memory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word,*next_param;

    //��ȡ3������
    word = sh_get_word(param,&next_param);
    addr = __sh_atol(word);
    word = sh_get_word(next_param,&next_param);
    unit_num = __sh_atol(word);
    word = sh_get_word(next_param,&next_param);
    unit_bytes = __sh_atol(word);

    word = sh_get_word(next_param,&next_param);
    if(word != NULL)
    {
        printf_ng("\r\n��������\r\n");
        return false;
    }
#if (cn_byte_bits == 8)  //�ֽ�λ��=8����������
    switch(unit_bytes)
    {
        case 1:
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%2x ", *(uint8_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;//ֻ�ܴ�ż����ʼ
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%4x ", *(uint16_t*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;//ֻ�ܴ�ż����ʼ
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%8x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %32 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
            break;
        case 8:
            addr &= ~(ptu32_t)7;//ֻ�ܴ�ż����ʼ
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%8x", *(uint32_t*)addr);
                addr +=4;
            	printf_ng("%8x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %64 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
            break;
        default :
            printf_ng("\r\n��������\r\n");
            return false;
        	break;
    }
#elif (cn_byte_bits == 16)  //�ֽ�λ��=16
    switch(unit_bytes)
    {
        case 1:
        {
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%4x ", *(uint16_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%8x ", *(uint32_t*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
        } break;
        case 4:
        {
            addr &= ~(ptu32_t)3;
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%8x ", *(uint32_t*)addr);
                addr +=2;
            	printf_ng("%8x ", *(uint32_t*)addr);
                putchar_ng(' ');
                addr +=2;
                if(addr %32 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf_ng("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (cn_byte_bits == 32)  //�ֽ�λ��=32
    switch(unit_bytes)
    {
        case 1:
        {
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%8x ", *(uint32_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
            	printf_ng("%8x ", *(uint32_t*)addr);
                addr +=2;
            	printf_ng("%8x ", *(uint32_t*)addr);
                printf_char(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf_ng("\r\n");
                    if(len != unit_num)
                    {
                    	printf_ng("0x%8x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf_ng("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    printf_ng("\r\n");
    return true;
}

//----д���ڴ�-----------------------------------------------------------------
//����: д��ĳ��ַ��ʼ��һ���ڴ�
//����: param������������������Ҫ�����������ÿո����
//          ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//          ����2:��ʾ�ĵ�Ԫ��
//          ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��������ֵ�����ش���
//          ����4:����������
//����: true=������ʾ��false=����
//-----------------------------------------------------------------------------
bool_t sh_fill_memory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = sh_get_word(param,&next_param);
    addr = __sh_atol(word);
    word = sh_get_word(next_param,&next_param);
    unit_num = __sh_atol(word);
    word = sh_get_word(next_param,&next_param);
    unit_bytes = __sh_atol(word);
    word = sh_get_word(next_param,&next_param);
    data = __sh_atol(word);
#if (cn_byte_bits == 8)  //�ֽ�λ��=8����������
    switch(unit_bytes)
    {
        case 1:
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(uint8_t*)addr = (u8)data;
                addr ++;
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr +=2;
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=4;
            }
            break;
        default :
            printf_ng("\r\n��������\r\n");
            return false;
            break;
    }
#elif (cn_byte_bits == 16)  //�ֽ�λ��=16
    switch(unit_bytes)
    {
        case 1:
        {
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr ++;
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=2;
            }
        } break;
        default :
        {
            printf_ng("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (cn_byte_bits == 32)  //�ֽ�λ��=32
    switch(unit_bytes)
    {
        case 1:
        {
        	printf_ng("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr ++;
            }
        } break;
        default :
        {
            printf_ng("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    return true;
}

//----��ȡ����---------------------------------------------------------------
//����: ��buf����ȡһ���ɿո���н����������ĵ��ʣ�next���ڷ�����һ�������׵�ַ��
//      ���û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
//-----------------------------------------------------------------------------
char *sh_get_word(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    if(buf == NULL)
        return NULL;
    while(1)
    {
        if((buf[i] == ' ') || (buf[i] == 0))
        {
            if(buf[i] == 0)
                return buf;
            else
            {
                buf[i] = '\0';
                break;
            }
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *next = NULL;
            else
                *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}

//----��ȡĿ¼---------------------------------------------------------------
//����: ��buf����ȡһ����'\\'���н����������ĵ��ʣ�next���ڷ�����һ�������׵�ַ��
//      ���û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
//-----------------------------------------------------------------------------
char *sh_get_item(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    while(1)
    {
        if((buf[i] == '\\') || (buf[i] == 0))
        {
            if(buf[i] == 0)
                return buf;
            else
            {
                buf[i] = '\0';
                break;
            }
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if((buf[i]!='\\') && (buf[i] != '\n') && (buf[i] != '\r'))
        {
            *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}

//----�������---------------------------------------------------------------
//����: ����buf�������ַ�����һ�������Ƿ�keyword���������param�з���keyword
//      ���һ�������ҷǻس����е��ַ���ָ�룬��ָ��ʵ�ʰ�����һ�������в�����
//      ��ַ�����û�в�������param=NULL��
//����: buf�����������ַ���
//      keyword����ƥ�������
//      param�����������в���ָ��
//����: true = ����ƥ�䣬false=���ƥ��
//-----------------------------------------------------------------------------
bool_t sh_cmd_parser(const char *buf,const char *keyword,char **param)
{
    uint32_t i=0;
    char buf2[255];

    if(buf == NULL)
    	return false;
    *param = NULL;
    //��ȡ���������������ֹͣ���Ż��ߺ���ո���±��i
    for(i=0;i<255;i++)
    {    
        if((buf[i]==' ')||(buf[i] == '\0'))
            break;
    }
    memcpy(buf2,buf,i);
    buf2[i] = '\0';
    //�Ƚ�û�д�����������
    if(strcmp(buf2,keyword)!=0)
        return false;

    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *param = NULL;
            else
                *param = (char*)&buf[i];
            break;
        }
        i++;
    }
    return true;
}

//----��ʾcd�İ���-------------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�cd�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_cd_help(void)
{
    printf_ng("DBX_name[:]         ����ָ���ļ�������Ŀ¼\r\n");
    printf_ng("                    ���磺D�� ����D�ļ����Ŀ¼\r\n");
    printf_ng("cd/chdir[�ļ���]·�� ��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼\r\n");
    printf_ng("                    ����: 1)cd C:\\folder ����folderĿ¼\r\n");
    printf_ng("                          2)cd folder ����folderĿ¼\r\n");
    printf_ng("cd/chdir[](���ַ�) ��ʾ��ǰĿ¼����\r\n");
    printf_ng("cd/chdir[\\]        ��������Ŀ¼\r\n");
	printf_ng("cd/chdir[..]       ��������һ��Ŀ¼\r\n");
	printf_ng("                   ����: 1)cd ..\\.. ������������Ŀ¼ \r\n");
	printf_ng("                         2)cd ..\\folder ������һ��Ŀ¼�к���folder��Ŀ¼ \r\n");
}
//----��ʾdir�İ���------------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�dir�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_dir_help(void)
{

	printf_ng("dir	[�ļ���]·��  [�ļ���]·��... ...\r\n");
	printf_ng("            ��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�\r\n");
	printf_ng("dir	[�ļ���][����ͨ���·��][�ļ���][����ͨ���·��]... ...\r\n");
	printf_ng("            ��ʾָ��Ŀ¼��ƥ����ļ�����Ŀ¼�б�\r\n");
	printf_ng("dir	[\\][\\]... ...	��ʾ��Ŀ¼��һ�����ļ�����Ŀ¼�б�\r\n");
	printf_ng("dir	[..][..]... ...	��ʾ��ǰĿ¼��һ�����ļ�����Ŀ¼�б�\r\n");
	printf_ng("dir	[](���ַ�)      ��ʾ��ǰĿ¼�µ��ļ�����Ŀ¼�б�\r\n");
}
//----��ʾrd�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�rd�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_rd_help(void)
{
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾren�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�ren�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_ren_help(void)
{
	printf_ng("Ren/rename [�ļ���:][·��]�ļ���1 �ļ���2\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾ�ź���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�chkdsk�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_md_help(void)
{
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾdeltree�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�deltree�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_deltree_help(void)
{
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾdel�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�del�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_del_help(void)
{
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾcopy�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�copy�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_copy_help(void)
{
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾmove�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�move�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_move_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾchkdsk�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�chkdsk�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_chkdbx_help(void)
{
	printf_ng("chkdbx �ļ��� ��ʾ���ļ����µ��ļ�������\r\n");

}
//----��ʾver�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�ver�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_ver_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾchcp�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�chcp�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_chcp_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}

//----��ʾformat�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�format�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_format_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}

//----��ʾtime�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�time�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_time_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}

//----��ʾdate�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�date�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_date_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾcls�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�cls�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_cls_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾlist-rsc�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�cls�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_listrsc_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾget-evtt�İ���----------------------------------------------------------
//����: ��sh_list_fscmd_help�������ã�cls�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_getevtt_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾget-envent�İ���----------------------------------------------------------
//����: ��sh_list_cmd_help�������ã�cls�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_getevent_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}
//----��ʾget-int�İ���----------------------------------------------------------
//����: ��sh_list_cmd_help�������ã�cls�İ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_list_getint_help(void)
{
 	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
	printf_ng("\r\n");
}



void shell_cmd_help()
{
	printf_ng("\r\n�й�ĳ���������ϸ��Ϣ��������help [ĳ������]\r\n\r\n");

	printf_ng("list-rsc        �г�ϵͳ�е���Դ���ֺͲ�ι�ϵ\r\n");
	printf_ng("speed           ����ѭ�������ٶ�\r\n");
    printf_ng("event           ��ʾ�¼���\r\n");
    printf_ng("evtt            ��ʾ�¼����ͱ�\r\n");
    printf_ng("mem             �г��ڴ�ʹ�����\r\n");
    printf_ng("d               ��ȡ�ڴ��������\r\n");
    printf_ng("f               д���ݵ��ڴ�\r\n");
    printf_ng("help            DJYOS�����������Ϣ\r\n");
	printf_ng("ver             ��ʾdjyos�İ汾\r\n");
	printf_ng("date            ��ʾ������������\r\n");
	printf_ng("time            ��ʾ��ǰʱ���������������ʱ��\r\n");
	printf_ng("\r\n");

	printf_ng("cd              ��ʾ��ǰĿ¼�����ƻ������\r\n");
	printf_ng("chkdbx          ��ʾ�ļ���ռ�Ĳ���\r\n");
	printf_ng("md              ����һ��Ŀ¼\r\n");
	printf_ng("mkdir           ����һ��Ŀ¼\r\n");
	printf_ng("rd              ɾ��һ��Ŀ¼\r\n");
	printf_ng("rmdir           ɾ��һ��Ŀ¼\r\n");
	printf_ng("copy            ����һ���ļ�\r\n");
	printf_ng("move            ��һ�������ļ��ƶ���ָ��Ŀ¼��\r\n");
	printf_ng("ren             ������һ���ļ�\r\n");
	printf_ng("rename          ������һ���ļ�\r\n");
	printf_ng("del             ɾ������һ���ļ�\r\n");
	printf_ng("dir             ��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�\r\n");
	printf_ng("\r\n");

	printf_ng("�ļ�ϵͳ�������ؽ�������ã���\r\n");
	printf_ng("chip-erase      ����NANDFLASHоƬ����������\r\n");
	printf_ng("create-dbx      �����ļ���\r\n");
	printf_ng("format          ��ʽ���ļ�ϵͳ\r\n");
    
}


//----��ʾ�ļ�ϵͳ�������----------------------------------------------------------
//����: ������Ҫ����ʾ��Դ��������Դ�����֣�������Դ����Ϊ�գ�����ʾ"������Դ"
//����: �����ַ���������:
//����: true
//-----------------------------------------------------------------------------
bool_t sh_list_cmd_help(char *param)
{
    char *next_para;

   	if(sh_cmd_parser(param,"cd",&next_para) == true)
        sh_list_cd_help();
    else if(sh_cmd_parser(param,"dir",&next_para) == true)
        sh_list_dir_help();
    else if(sh_cmd_parser(param,"rd",&next_para) == true)
        sh_list_rd_help();
    else if(sh_cmd_parser(param,"ren",&next_para) == true)
        sh_list_rd_help();
    else if(sh_cmd_parser(param,"md",&next_para) == true)
        sh_list_md_help();
    else if(sh_cmd_parser(param,"deltree",&next_para) == true)
        sh_list_deltree_help();
    else if(sh_cmd_parser(param,"del",&next_para) == true)
        sh_list_del_help();
    else if(sh_cmd_parser(param,"copy",&next_para) == true)
        sh_list_copy_help();
	else if(sh_cmd_parser(param,"move",&next_para) == true)
        sh_list_move_help();
  	else if(sh_cmd_parser(param,"chkdbx",&next_para) == true)
        sh_list_chkdbx_help();
   	else if(sh_cmd_parser(param,"ver",&next_para) == true)
        sh_list_ver_help();
   	else if(sh_cmd_parser(param,"time",&next_para) == true)
        sh_list_time_help();
	else if(sh_cmd_parser(param,"date",&next_para) == true)
        sh_list_date_help();
  	else if(sh_cmd_parser(param,"chcp",&next_para) == true)
        sh_list_chcp_help();
   	else if(sh_cmd_parser(param,"format",&next_para) == true)
        sh_list_format_help();
   	else if(sh_cmd_parser(param,"cls",&next_para) == true)
        sh_list_cls_help();
   	else if(param == NULL)
        shell_cmd_help();
    else
        printf_ng("��������\r\n");
    return true;
}
bool_t sh_ver(char *param)
{
	printf_ng("\r\n%s\r\n", djyos_kernel_version);

	return true;
}

extern struct mem_global *djy_get_MCB(void);
bool_t sh_mem(char *param)
{
    struct mem_global *MCB;
    ptu32_t ua_pages_num,free_pages_num,ua_block_max,ua_free_block_max;

    if(param != NULL)
    {
        //�����ȴ�
        return true;
    }
    MCB=(struct mem_global *)djy_get_MCB();
    
    ua_pages_num = MCB->ua_pages_num;
    ua_pages_num = ua_pages_num<<cn_page_size_suffix_zero;
    printf_ng("%dB ϵͳ�ڴ��ܳߴ�\r\n", ua_pages_num);
    
    free_pages_num = MCB->free_pages_num;
    free_pages_num = free_pages_num<<cn_page_size_suffix_zero;
    printf_ng("%dB ϵͳ�����ڴ�ߴ�\r\n", free_pages_num);
    
    ua_block_max = MCB->ua_block_max;
    printf_ng("%dB ϵͳ����ڴ��ߴ�\r\n", ua_block_max);
    
    ua_free_block_max = MCB->ua_free_block_max;
    printf_ng("%dB ϵͳ��������ڴ��ߴ�\r\n", ua_free_block_max);

    return true;
}
#if 1

/*
 * ���������޸�srcbuf�е��ַ�srccΪ'\0'
 */
char * strsep_ng(char **stringp, char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if ((s = *stringp)== NULL)
	{
		return (NULL);
	}

	for (tok = s;;)
	{
		c = *s++;
		spanp = delim;
		do
		{
			if ((sc =*spanp++) == c)
			{
				if (c == 0)
				s = NULL;
				else
				s[-1] = 0;
				*stringp = s;

				return (tok);
			}
		} while (sc != 0);
	}

	return NULL;
}

//shell ��ʱ��ŵ�shell�� ��û��д����������dos����д����ok��
bool_t sh_date(char *buf)
{
	s64 time_buf, time;
	struct tm dtm;
    char command[256];
    char *pcmd;
    char **ppcmd;
    char *nextcmd;
    char *sc = "/";
    int cmdlen;

    u32 ymd_index;
    u32 year, month, date;

	time = djy_time(&time_buf);

	dtm = localtime(time);

	printf_ng("\r\n��ǰ���ڣ�%d/%02d/%02d %s",
			dtm.tm_year, dtm.tm_mon, dtm.tm_mday, tm_wdays[dtm.tm_wday]);
	printf_ng("\r\n���������ڣ�");

    cmdlen = djy_scanf(NULL, command);
	if (cmdlen)
	{
		pcmd = command;
		ppcmd = &pcmd;

		ymd_index = 0;
		nextcmd = strsep_ng(ppcmd, sc);
		while (nextcmd)
		{
			if (ymd_index == 0)
			{
				year = __sh_atol(nextcmd);
				if (year < 1970)
				{
					ymd_index = 0;
					year = 0;
					printf_ng("\r\n��ݴ���!");
					break ;
				}
				ymd_index++;
			}
			else if (ymd_index == 1)
			{
				month = __sh_atol(nextcmd);
				if (month ==0 || month >12)
				{
					ymd_index = 0;
					month = 0;
					printf_ng("\r\n�·ݴ���!");
					break ;
				}
				ymd_index++;
			}
			else if (ymd_index == 2)
			{
				date = __sh_atol(nextcmd);
				if (date == 0 || date >31)
				{
					ymd_index = 0;
					date = 0;
					printf_ng("\r\n���ڴ���!");
					break ;
				}
				ymd_index++;
			}
			else
			{
				ymd_index = 0;
				break;
			}

			nextcmd = strsep_ng(ppcmd, sc);
		}

		if (ymd_index == 3)
		{
			dtm.tm_year = year;
			dtm.tm_mon = month;
			dtm.tm_mday = date;

			extern bool_t (*ck_set_rtc_hard)(struct tm *DateTime);
		    if(ck_set_rtc_hard != NULL)
		        ck_set_rtc_hard(&dtm);
		}
	}

	printf_ng("\r\n");
    return true;
}

bool_t sh_time(char *buf)
{
	struct tm dtm;
	s64 time_buf, time;
    char command[256];
    char *pcmd;
    char **ppcmd;
    char *nextcmd;
    char *sc = ":";
    int cmdlen;

    u32 hms_index;
    u32 hour, min, sec;

	time = djy_time(&time_buf);

	dtm = localtime(time);

	printf_ng("\r\n��ǰʱ�䣺%02d:%02d:%02d", dtm.tm_hour, dtm.tm_min, dtm.tm_sec);
	printf_ng("\r\n������ʱ�䣺");

    cmdlen = djy_scanf(NULL, command);
	if (cmdlen)
	{
		pcmd = command;
		ppcmd = &pcmd;

		hms_index = 0;
		nextcmd = strsep_ng(ppcmd, sc);
		while (nextcmd)
		{
			if (hms_index == 0)
			{
				hour = __sh_atol(nextcmd);
				if (hour >23)
				{
					hms_index = 0;
					hour = 0;
					printf_ng("\r\nСʱ����!");
					break ;
				}
				hms_index++;
			}
			else if (hms_index == 1)
			{
				min = __sh_atol(nextcmd);
				if (min >59)
				{
					hms_index = 0;
					min = 0;
					printf_ng("\r\n���Ӵ���!");
					break ;
				}
				hms_index++;
			}
			else if (hms_index == 2)
			{
				sec = __sh_atol(nextcmd);
				if (sec >59)
				{
					hms_index = 0;
					sec = 0;
					printf_ng("\r\n���Ӵ���!");
					break ;
				}
				hms_index++;
			}
			else
			{
				hms_index = 0;
				break;
			}

			nextcmd = strsep_ng(ppcmd, sc);
		}

		if (hms_index == 3)
		{
			dtm.tm_hour = hour;
			dtm.tm_min = min;
			dtm.tm_sec = sec;
			extern bool_t (*ck_set_rtc_hard)(struct tm *DateTime);
		    if(ck_set_rtc_hard != NULL)
		        ck_set_rtc_hard(&dtm);
		}
	}

	printf_ng("\r\n");
    return true;
}
#endif



//----ת����������Ĵ�д��ĸΪСд----------------------------------------------
//����: ת��shell�����������"buf"�пո�ǰ�Ĵ�д��ĸΪСд��
//����: cmd��shell���������������"buf"(cmd)
//����: 0
//-----------------------------------------------------------------------------
void capital_convert_lowercase(char *cmd)
{

	while(*cmd!='\0')
    {
           if(*cmd>='A'&&*cmd<='Z')
            *cmd=*cmd+('a'-'A');
            cmd++; //��һ���ַ�
         //�����ո�
         if(*cmd==' ')
         	break;
    }
	return ;
}

//----��ʾ�¼���---------------------------------------------------------------
//����: ��ʾ�¼��б�
//����: ��
//����: true
//-----------------------------------------------------------------------------
bool_t sh_show_event(char *param)
{
    u32 cmd;
    u16 evtt;
    cmd = cn_db_info_event;
#if(cfg_debug_info == 1)
    evtt = djy_get_evtt_id("debug_info");
    djy_event_pop(evtt,enum_para_sync,NULL,0,&cmd,4,0,0);
#else
    printf_ng("û�а���������Ϣģ��\r\n");
#endif

    return true;
}

//----��ʾ�¼����ͱ�-----------------------------------------------------------
//����: ��ʾ�¼������б�
//����: ��
//����: true
//-----------------------------------------------------------------------------
bool_t sh_show_evtt(char *param)
{
    u32 cmd;
    u16 evtt;
    cmd = cn_db_info_evtt;
#if(cfg_debug_info == 1)
    evtt = djy_get_evtt_id("debug_info");
    djy_event_pop(evtt,enum_para_sync,NULL,0,&cmd,4,0,0);
#else
    printf_ng("û�а���������Ϣģ��\r\n");
#endif

    return true;
}

extern bool_t __erase_all_nand(char *param);

//���������
struct cmd_table shell_cmd_table[] =
{
    {"list-rsc", sh_list_rsc},
    {"speed", sh_show_for_cycle},
    {"event", sh_show_event},
    {"evtt", sh_show_evtt},
    {"d", sh_show_memory},
    {"f", sh_fill_memory},
    {"help", sh_list_cmd_help},
    {"ver", sh_ver},
	{"mem", sh_mem},
	{"date", sh_date},
	{"time", sh_time},


#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))
    {"rmdir", sh_rd},
    {"rd", sh_rd},
    {"dir", sh_dir},
    {"rename", sh_ren},
    {"ren", sh_ren},
    {"del", sh_del},
    {"copy", sh_copy},
    {"move", sh_move},
    {"chkdbx", sh_chkdbx},
    {"chdir", sh_cd},
    {"cd", sh_cd},
    {"mkdir", sh_md},
    {"md", sh_md},
    {"chip-erase", __erase_all_nand},
    {"create-dbx", sh_create_dbx},
    {"format", sh_format},
#endif

    {NULL,NULL}
};

//----ִ�п���̨����---------------------------------------------------
//����: ������ִ�п���̨����
//����: ��
//����: ��
//��ע: ��ʱ���ù�ϣ��ȣ����и��ӿ��ٵıȽϡ���������ıȽϣ�������ˡ�
//      �ǻ����������ˡ�
//---------------------------------------------------------------------
bool_t sh_exec_command(char *buf)
{
    bool_t result;
    char *param;
    uint32_t i=0;
    //�������ƶ�ȡ255�ַ�����������ʾ�������С�������Ͳ��ò�������ʾ
    //̫����
	if(strnlen(buf, cn_shell_cmd_limit)==cn_limit_uint32)
	{
        printf_ng("�����ַ���̫��\r\n");
	}
    //ת��buf�еĴ�д��ĸΪСд
	capital_convert_lowercase(buf);
    
    if(!__judge_colon_if_data(buf))
    {
        if(cd_dbx(buf))
        {
            return true;
        }
    }
    do
    {
        if(sh_cmd_parser(buf,shell_cmd_table[i].cmd,&param)==true)
        {
            result = shell_cmd_table[i].shell_cmd_func(param);
            break;
        }
        i++;
    }while(shell_cmd_table[i].cmd != NULL);
    if(shell_cmd_table[i].cmd == NULL)
    {
        if(sh_ext_command == NULL)
        {
            printf_ng("\r\n�޴�����\r\n");
            result = false;
        }else
        {
            if(!sh_ext_command(buf))
            {
                
                printf_ng("\r\n�޴�����\r\n");
                result = false;
            }else
                result = true;
        }
    }    
    return result;
}
//----����̨������-----------------------------------------------------------
//����: ����console������ַ�����console����س���ʱ��ִ�����һ������ó���
//      255�ַ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void sh_service(void)
{
    char command[cn_shell_cmd_limit+1];
    while(1)
    {
        djy_scanf(NULL,command);
        sh_exec_command(command);  //ִ������
#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))
        print_work_path( );
#endif
        putchar_ng('>');
    }
}

#endif  //#if (cfg_shell == 1)
