//----------------------------------------------------
//Copyright (C), 2011.9,wjz.
//��Ȩ���� (C), 2011.9,wjz
//����ģ��: djyshģ��
//���ߣ�wjz
//�汾��djysh1.0.0
//�ļ�����: djysh���ļ�ϵͳshell���
//�޶���ʷ:
// 1. ����:2010-6-15
//   ����:wjz
//   �°汾�ţ�djysh1.0.0
//------------------------------------------------------

#include "os_inc.h"

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_shell == 1))

#include <string.h>
#include "char_term.h"
#include "shell.h"
#include "shell_fs.h"
#include "file.h"
#include "flashfile.h"
#include "mdr.h"

extern struct file_rsc *pg_work_path;

//----����ַ����Ƿ���'*'����'?'-----------------------------------------------
//����: ����ַ����Ƿ����ǺŻ����ʺ�,���ҳ�������������󳤶�Ϊ�ޡ�
//����: str,�����ַ���
//����: true=��'*'����'?'��false=û'*'����'?'
//----------------------------------------------------------------------------
bool_t __check_wildcard(char *str)
{
    uint32_t loop,name_len;
    
    if(str == NULL)
        return false;
    name_len = strnlen(str, cn_shell_cmd_limit);
    for(loop = 0; loop <= name_len; loop++)
    {
        if((str[loop]=='*')||(str[loop]=='?'))
            return true;   
    }
    return false;
}

//-------��ͨ������ַ�ƥ��-----------------------------------------------------
//����: ��ͨ������ַ�ƥ�������ͨ�����'?'��'*'������,'?'���Դ���һ���ַ�
//        (����û��),'*'���Դ����������ַ�(����Ϊ��)��������'?'�� ����'?'
//    	�Ĺ���,'?'��ʾ�����ַ�,Ҳ����˵��ƥ�������,'?'��Զƥ��ɹ� ��
//    	������,'?'��û���޸��㷨���������޸���ƥ����� -- ����'?'��һ��ƥ�� ��
//    	Ȼ��'*'��˲�ͬ,'*'��������ƥ���������ַ�����Ȼ���ǲ��ܼ򵥵��޸�ƥ��
//    	���̶�����Ҫ��
//����: src - ��ƥ����ַ���,���ܺ���'?'��'*'
//    	wildcard - ͨ����ַ���
//����: true=ƥ��ɹ�,
//      false=ƥ��ʧ��,
//-----------------------------------------------------------------------------
bool_t __wildcard_match(char *src, char *wildcard)
{
	u32 i = 0;
	u32 j = 0;

	while (wildcard[j] || src[i])
	{
		if (wildcard[j] == '*')
		{
			while (wildcard[j] == '*')	// �ҵ�һ����*�ŵ����һ��*��
			{
				j++;
				if (wildcard[j] == '\0')
				{
					return true;	// ƥ��
				}
			}
			while (src[i] != wildcard[j])	// ƥ��src����*���һ���ַ�
			{
				if (src[i])
				{
					i++;
				}
				else
				{
					return false;	// ��ƥ��
				}
			}
		}
		else
		{
			if ((src[i] == wildcard[j]) || (wildcard[j] == '?'))
			{
				i++;
				j++;
			}
			else
			{
				return false;
			}
		}
	}

	if ((src[i] == '\0') && (wildcard[j] == '\0'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//----�ж�'\'�����Ƿ�������-----------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ������жϵ�һ������'\'֮���Ƿ���
//      ����
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�ɹ�, ˵��'\'֮��������,����û��'\'
//      false=ʧ�ܣ�˵��'\'֮��û������
//��ע: pathӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __judge_backslash_if_data(char *path)
{
    uint32_t index;
    uint32_t name_len;

	name_len = strnlen(path,cn_shell_cmd_limit);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == '\\')    //�����ַ�'\'
        {
            if(path[index+1] != '\0')
            	return true;
        	else
            	return false;
        }
    }
    //�ַ�����û�з�б��
	if(index == name_len)
    	return true;
	else
    	return false;            
}

//----�ж�':'�����Ƿ�������-----------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ������жϵ�һ������'\'֮���Ƿ���
//      ����
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�ɹ�, ˵��':'֮��������,����û��':'
//      false=ʧ�ܣ�˵��':'֮��û������
//��ע: pathӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __judge_colon_if_data(char *path)
{
    uint32_t index;
    uint32_t  name_len;

	name_len = strnlen(path,cn_shell_cmd_limit);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')    //�����ַ�'\'
        {
            if(path[index+1] != '\0')
            	return true;
        	else
            	return false;
        }
    }
    //�ַ�����û��ð��
	if(index == name_len)
    	return true;
	return false;        
}

//----ɾ��·������ļ�����Ŀ¼-------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�����ɾ�����һ����path��һ����
//      Ŀ¼�����򲻲���
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�ɹ�ɾ����������ɾ��
//      false=ʧ��
//-----------------------------------------------------------------------------
bool_t __cut_path_end_item(char *path)
{
	uint16_t name_len,loop;
    
	name_len = strnlen(path,cn_shell_cmd_limit);
	loop=name_len;
	if(path[name_len-1]=='\\')//�з�б��˵����ȥ���ļ���
    {
    	if(path[name_len-2]==':')
        	return true;//˵���Ǹ�Ŀ¼�����ò���
    	else
        {
        	for(loop=name_len-2;loop>0;loop--)
            {
            	if(path[loop] == '\\')
                {
                	path[loop+1] = '\0';
                	break;
                }
            }
        }
    	return true;
    }
	else//û�з�б�ܣ�˵��ȥ�������ļ�
    {
    	for(loop=name_len-1;loop>0;loop--)
        {
        	if(path[loop] == '\\')
            {                    
            	path[loop+1] = '\0';
            	break;
            }
        }
    }
	return true;
}
//----��ȡ·������ļ�����Ŀ¼--------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�������ȡ���һ����path�Ǹ�Ŀ¼
//      �򲻲�����path�����ֲ��䡣
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//    	name,������ȡ���ļ�����Ŀ¼
//����: true=�ɹ�, ��ȡ�ɹ�
//      false=ʧ�ܣ���ȡʧ��
//-----------------------------------------------------------------------------
bool_t __pick_path_end_item(char *path,char *name)
{
	uint16_t name_len,loop;
	char *str;
    
	name_len = strnlen(path,cn_shell_cmd_limit);
	loop=name_len;
	if(path[name_len-1]=='\\')//�з�б�ܣ�˵����ȡ�����ļ�����
    {
    	if(path[name_len-2]==':')
        	return false;   //˵���Ǹ�Ŀ¼�����ò���
    	else
        {
        	for(loop=name_len-2;loop>0;loop--)
            {
            	if(path[loop] == '\\')
                {
                	str=&path[loop+1];
                    strcpy(name,str);
                    return true;
                }            
            }
        }
    }
	else//û�з�б�ܣ�˵����ȡ�����ļ���
    {
    	for(loop=name_len-1;loop>0;loop--)
        {
        	if(path[loop] == '\\')
            {                    
            	str=&path[loop+1];
                strcpy(name,str);
            	return true;
            }
        }

    }
    //ִ�е��ⲽ��˵��ֻ��һ���ļ�����Ŀ¼��ֱ�ӿ�����ok
    strcpy(name,path);    
	return true;
}

//----�ж��ַ����Ǿ���·���������·��-------------------------------------------------
//����: ���һ�����ܰ����༶·�������ļ������ַ����Ƿ�����ļ������ľ���·��
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�Ǿ���·��
//      false=���Ǿ���·��
//��ע: pathӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __judge_if_absolute_path(char *path)
{
    uint32_t index;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    uint32_t name_len;

	name_len = strnlen(path,cn_shell_cmd_limit);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')
        {
        	return true;
        }
    }
    	return false;
}
//----Ŀ¼��ƥ����ļ���ȡ��һ���ַ���--------------------------------------
//����: ��һ��Ŀ¼��ɨ����ڵ��ļ���Ȼ��ÿ���ļ�ͨ��ƥ�亯����wildcard_param����
//    	һһƥ�䣬��ƥ����ļ����ᵽ��һ���ַ����
//����: wildcard_param������ͨ����Ĳ���
//    	get_merge_str ��������Ҫ����ļ����ϲ����ɵ��ַ���
//����: true=�ɹ�, ��ȡ�ɹ�
//      false=ʧ�ܣ���ȡʧ��
//-----------------------------------------------------------------------------
char *get_folder_wildcard_str(char *wildcard_param,char *get_merge_str)
{
	struct file_rsc *fp = NULL,*current_fp = NULL;
	char work_path[256];
    u32 fat_block_no;

 
    if(__djyfs_if_contain_DBX_name(wildcard_param) ==0)
        fp = pg_work_path;//���·����ֱ��ʹ�ù���·��
    else
    {    
    	strcpy(work_path,wildcard_param);
    	__pick_filename_word(work_path,wildcard_param);
        __cut_path_end_item(work_path);
        fp = djyfs_fopen(work_path, "r");    
    }
    //���ļ���û���壨ͬʱfp2�ǷǸ�Ŀ¼,��Ŀ¼�£��ļ����Բ�ȷ����
    if(fp->attr.bits.archive&&(fp->file_medium_tag!=0))
        return NULL;
	current_fp =(struct file_rsc *)djyfs_folder_traversal_son(fp,current_fp);
	if(current_fp == NULL)
    {    
    	djyfs_fclose(fp);
    	printf_ng("��Ŀ¼Ϊ��!\r\n");
    	return NULL;
    }
	fat_block_no = current_fp->file_medium_tag;
        //ȫ����0�������Ϳ���׼ȷ֪������merge_str�Ѿ�û��������
    memset(get_merge_str,0,sizeof(get_merge_str));
	do//ѭ����Ŀ¼��ֱ������
    {
    	if(__wildcard_match(current_fp->name,wildcard_param))
        {//��ƥ��ɹ���Ŀ¼����һ�����ַ���
        	strcat(get_merge_str,current_fp->name);
        	strcat(get_merge_str," ");
        }    
        //��fp�����е��ļ�����ɾ���ˣ���û�ļ�������false
        current_fp = (struct file_rsc *)djyfs_folder_traversal_son(fp,current_fp);
        if(current_fp == NULL)
            break;
    }while(fat_block_no != current_fp->file_medium_tag);
	djyfs_fclose(fp);
    djyfs_find_close(current_fp);

	return get_merge_str;
}

//----��ȡ����ͨ���·���к���ͨ����Ǽ����ַ���-------------------------------
//����: �ں���ͨ�����·���У����к���ͨ�����һ�����ַ�����ȥ����
//����: 
//����: true=�ɹ�, ��ȡ�ɹ�
//      false=ʧ�ܣ���ȡʧ��
//-----------------------------------------------------------------------------
bool_t del_wildcard_files(char *wildcard_param)
{
	char *next_param;
	char *del_param;
    char merge_str[255];
    struct file_rsc*fp;

	if(wildcard_param == NULL)
    {
		printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
    //debug ����ȫ·���ģ���û����
	next_param = get_folder_wildcard_str(wildcard_param,merge_str);
	do
    {
        del_param = sh_get_word(next_param,&next_param);
    	if(del_param[0]==0||del_param == NULL)
        	return false;
        fp = djyfs_fopen(del_param,"r");
        if(fp == NULL)
            break;
        if(fp->attr.bits.archive)
        {
            djyfs_fclose(fp);
            djyfs_remove(del_param);
        }
        djyfs_fclose(fp);
    }while(del_param != NULL);
	return true;    
}

//----���ַ���ĩβ�޷�б�ܼ��Ϸ�б��--------------------------------------------------------------
//���ܣ����ַ���ĩβ�޷�б�ܼ��Ϸ�б��,�з�б�ܾͲ�Ҫ����
//������str �ַ������κ���ʽ
//���أ�true �ɹ�
//    	false ����".."��
//------------------------------------------------------------------------------
bool_t str_end_increase_backslash(char *str)
{
	uint16_t name_len;

	if(strcmp(str,"..") ==0)//�����"..",��ô��Ҫ����
    	return false;
	name_len = strnlen(str,0);
    //����ַ������һλ����'\\'����ô�ӷ�б��
	if(str[name_len-1] != '\\')
    {
    	str[name_len] = '\\';
        //���str���ǹ涨�õĿռ䣬������й¶�������ֹ
       	str[name_len+1] = '\0';    
    }
	return true;
}

//----���ַ���ĩβ�з�б��ȥ����б��--------------------------------------------
//���ܣ����ַ���ĩβ�޷�б�ܼ��Ϸ�б�ܣ�û�з�б�ܾͲ�����
//������str �ַ������κ���ʽ
//���أ�true �ɹ�
//    	false �ַ���û�з�б��
//------------------------------------------------------------------------------
bool_t str_end_wipe_backslash(char *str)
{
	uint16_t name_len;

	name_len = strnlen(str,0);
	if(str[name_len-1] == '\\')
    {
    	str[name_len-1] = '\0';
    	return true;
    }
	else
	return false;
    
}

//----��ʾĿ¼�µ��ļ����ļ���------------------------------------------------
//����: ɨ���Ŀ¼�µ��ļ����ļ��У�Ȼ����һ������ʽ��ȫ����ӡ����
//����: ancestor_fp������ʾ���ļ���
//����: true=�ɹ�, �г��ɹ�
//      false=ʧ�ܣ��г�ʧ��
//-----------------------------------------------------------------------------
bool_t dir_item_son(struct file_rsc *parent_fp)
{
    struct file_rsc *current_fp,*ancestor_fp;
    struct st_DBX_device_tag *DBX_device_tag;
    struct st_DBX_flash_tag *DBX_flash_tag;
    uint32_t files_size=0,index=0,fdt_block_no;
    uint32_t files_items=0,folder_items=0,available_dbx_size=0;
    struct rsc_node *point_num[cn_path_depth_limit];

    if(parent_fp == NULL)
        return false;//�������ļ���δ����

    DBX_device_tag = (struct st_DBX_device_tag *)(pg_work_path->home_DBX
                                                            ->private_tag);
    DBX_flash_tag = (struct st_DBX_flash_tag *)DBX_device_tag->DBX_medium_tag;
    
    current_fp = parent_fp;
    ancestor_fp = DBX_device_tag->opened_root;

    for(index = 0; index<cn_path_depth_limit; index++)
    {
        point_num[index] = (struct rsc_node *)current_fp;
        if(current_fp == ancestor_fp)
            break;
        current_fp = (struct file_rsc *)rsc_get_parent(
                                    (struct rsc_node *)current_fp);
    }
    printf_ng("%s:\\", point_num[index]->name);

    for( ; index>0; index--)
    {
        printf_ng("%s\\", point_num[index-1]->name);
    }
    printf_ng(" ��Ŀ¼\r\n\r\n");

	current_fp = NULL;
    current_fp = (struct file_rsc*)djyfs_folder_traversal_son(parent_fp,current_fp);
    if(current_fp == NULL)
    {
        printf_ng("��Ŀ¼Ϊ��!\r\n");
        return false;
    }

    fdt_block_no = current_fp->file_medium_tag;

    do //ѭ����Ŀ¼��ֱ������
    {
        printf_ng("%d/%02d/%02d %02d:%02d",
                (current_fp->year_high_modify<<8) + current_fp->year_low_modify,
                current_fp->month_modify,
                current_fp->date_modify,
                current_fp->hour_modify,
                current_fp->minute_modify);

        if(current_fp->attr.bits.folder)
        {
            printf_ng(" <DIR> ");//������Ŀ¼�����Լ���dir
            folder_items++;
        }
        else
        {
            printf_ng("       ");
            files_items++;
        }
		printf_ng("%9dB %s\r\n",
				(uint32_t)current_fp->file_size, current_fp->name);
        files_size+=current_fp->file_size;
        current_fp = (struct file_rsc*)djyfs_folder_traversal_son(parent_fp,current_fp);
    }while(fdt_block_no != current_fp->file_medium_tag);
    djyfs_find_close(current_fp);
    available_dbx_size = DBX_flash_tag->free_sum *\
                            DBX_flash_tag->chip->block_size;

    printf_ng("%d���ļ� %d�ֽ�\r\n", files_items, files_size);
    printf_ng("%d��Ŀ¼ %d�����ֽ�\r\n", folder_items, available_dbx_size);
    return true;
}

//----��ʾ����ͨ������ļ������ļ���------------------------------------------------
//����: dir�����£���ʾ����ͨ������ļ������ļ��С�ɨ�赱ǰ����·������ָ��·����
//    	���ļ�����Ŀ¼��Ȼ����ƥ�亯��һһƥ�䡣����Ҫ��ģ���ʾ������
//����:wildcard_param������ͨ����Ĳ���
//����: true=�ɹ�, �г��ɹ�
//      false=ʧ�ܣ��г�ʧ��
//-----------------------------------------------------------------------------
bool_t dir_wildcard(char *wildcard_param)
{    
	struct file_rsc *fp;
    struct st_DBX_device_tag *DBX_device_tag;
    struct st_DBX_flash_tag *DBX_flash_tag;
	uint32_t files_items=0,folder_items=0,files_size=0,available_dbx_size=0;
    u16 dbx_len;
	uint16_t loop=0;
	char *next_param=NULL,*dir_param=NULL;
	char s[cn_shell_cmd_limit+1]={0},work_path[cn_shell_cmd_limit+1];

    dbx_len = __djyfs_if_contain_DBX_name(wildcard_param);
    if(dbx_len !=0)
    {
        strcpy(work_path,wildcard_param);
        printf_ng("\r\n%s ��Ŀ¼\r\n\r\n", work_path);
        __cut_path_end_item(work_path);
    }
    else
    {
        if(!get_work_path_name(work_path,cn_shell_cmd_limit))
            return false;
        printf_ng("\r\n%s ��Ŀ¼\r\n\r\n", work_path);
     }
	if(!get_folder_wildcard_str(wildcard_param,s))
        return false;
	if(s[0]==0)
    {
    	printf_ng("�Ҳ����ļ�!\r\n");
    	return false;
    }
    DBX_device_tag = (struct st_DBX_device_tag *)(pg_work_path->home_DBX
                                                            ->private_tag); 
    DBX_flash_tag = (struct st_DBX_flash_tag *)DBX_device_tag->DBX_medium_tag;
	available_dbx_size = DBX_flash_tag->free_sum *\
                            	DBX_flash_tag->chip->block_size;

	dir_param = sh_get_word(s,&next_param);
	strcat(work_path,dir_param);
	do
    {
    	fp = djyfs_fopen(work_path,"r");
    	if(fp == NULL)
        	return false;
        printf_ng("%d/%02d/%02d %02d:%02d",
                (fp->year_high_modify<<8) + fp->year_low_modify,
                fp->month_modify,
                fp->date_modify,
                fp->hour_modify,
                fp->minute_modify);

    	if(fp->attr.bits.folder)
        {
        	printf_ng(" <DIR> ");//������Ŀ¼�����Լ���dir
          	folder_items++;
        }
    	else
        {
        	printf_ng("       ");
        	files_items++;
        }
        printf_ng("%9dB %s\r\n", (uint32_t)fp->file_size, fp->name);
    	files_size+=fp->file_size;
    	djyfs_fclose(fp);
    	__cut_path_end_item(work_path);
    	if(next_param==0)
        	loop=0 ;
    	else
        	loop=1;
    	if(next_param!=0)
        {
        	dir_param = sh_get_word(next_param,&next_param);
        	strcat(work_path,dir_param);
        }
    }while(loop);

    printf_ng("%d���ļ� %d�ֽ�\r\n", files_items, files_size);
    printf_ng("%d��Ŀ¼ %d�����ֽ�\r\n", folder_items, available_dbx_size);

	return true;
}


//----�ж�һ��Ŀ¼�Ƿ����ļ���------------------------------------------------
//����: ͨ������fp�����ԣ��ж��Ƿ����ļ���
//����: fullname,·�� ��ʽ:"C:\aa\bb\cc"
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t judge_is_folder(char *fullname)
{
	struct file_rsc *fp;
	bool_t result;
	result = djyfs_fsearch(fullname);
	if(result == true)
    {
    	fp = djyfs_fopen(fullname,"r");
       	if(fp == NULL)
           {
           	return false;
           }
       	if(!fp->attr.bits.folder)
           {
           	djyfs_fclose(fp);
        	return false; 
           }
    	djyfs_fclose(fp);
    	return true;
    }
	else
    	return false;//�ļ�·�����󣬻���ô�ж��Ƿ�Ŀ¼
}

//----ɾ���ļ����¼����ļ�------------------------------------------------
//����: ��һ���ļ��е���һ����ɾ�������е��ļ�
//����: workpath����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//    	�������޸ġ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t del_files_of_folder(char *work_path)
{
	struct file_rsc *fp,*current_fp = NULL;
    uint32_t fat_block_no;
    
	fp = djyfs_fopen(work_path,"r");
    if(fp == NULL)
        return false;
	current_fp = (struct file_rsc *)djyfs_folder_traversal_son(fp,current_fp);
    if(current_fp == NULL)
    {
    	printf_ng("��Ŀ¼Ϊ��!\r\n");
    	djyfs_fclose(fp);
    	return false;  //parent��һ����Ŀ¼
    }        
	fat_block_no = current_fp->file_medium_tag;
    //����������ӵķ�б��Ϊ��strcat���Ա�֤�ɹ�
	str_end_increase_backslash(work_path);
	do//ѭ����Ŀ¼��ֱ������
    {    
    	if(!current_fp->attr.bits.folder)
        {
        	strcat(work_path,current_fp->name);
        	djyfs_remove(work_path);
        	__cut_path_end_item(work_path);
        }
      //��fp�����е��ļ�����ɾ���ˣ���û�ļ�������false
    	current_fp = (struct file_rsc *)djyfs_folder_traversal_son(fp,current_fp);
        if(current_fp == NULL)
            break;
    }while(fat_block_no != current_fp->file_medium_tag);
    djyfs_find_close(current_fp);
	djyfs_fclose(fp);
	return true;
}
//----����һ���ļ���------------------------------------------------
//����: ָ������һ���ļ���
//����: md_param�������ַ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t fs_mkdir(char *md_param)
{  
	struct file_rsc *fp;

	if(md_param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
    //�ڴ�����ĩβ����'\'��������Σ�����Ŀ¼
    //���ﲻ���ڴ�й©����Ϊmd������涨�˳���
	str_end_increase_backslash(md_param);
	if(!__check_fullname(md_param))
     	return false; 
    //����д����ļ�����Ŀ¼����ô�Ͳ���Ҫ������
    if(!djyfs_fsearch(md_param))
    {
        fp = djyfs_fopen(md_param,"w");
           if(fp == NULL)
            return false;
        djyfs_fclose(fp);
    }
    else
    {
        printf_ng("\r\n��Ŀ¼�����ļ�%d�Ѿ�����\r\n", md_param);

        return false;
    }
    return true;    
}
//----ɾ���ļ���--------------------------------------------------------------
//����: ��ָ�����ļ���ɾ����
//����: rd_param�������ַ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t fs_remove_folder(char *rd_param)
{    
	struct file_rsc *fp;

	if(rd_param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
	if(!__check_fullname(rd_param))
     	return false; 
	fp = djyfs_fopen(rd_param,"r");
	if(fp == NULL)
    {    
    	printf_ng("ϵͳ�Ҳ���ָ�����ļ�\r\n");
    	return false; 
    }
	if(fp->attr.bits.folder)
    {
    	djyfs_fclose(fp);
    	if(djyfs_remove(rd_param) == enum_fs_folder_unblank)
        {
            printf_ng("Ŀ¼%s���ǿգ�\r\n", rd_param);
        }
    	return true;
    }
	else
    {
    	djyfs_fclose(fp);
    	printf_ng("Ŀ¼������Ч!\r\n");
    	return false;
    }
    
}
//----ɾ���ļ���--------------------------------------------------------------
//����: ��ָ�����ļ���ɾ����
//����: rd_param�������ַ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t fs_remove_file(char *del_param)
{    
	if(del_param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
	if(!__check_fullname(del_param))
     	return false; 
	if(!djyfs_fsearch(del_param))
    {
    	printf_ng("�Ҳ���%s\r\n", del_param);
    	return true; 
    }
	if(!judge_is_folder(del_param))
    {        
    	djyfs_remove(del_param);
    	return true;    
    }
	else
    {
    	del_files_of_folder(del_param);
    	return true; 
    }
}
//----��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�-------------------------------------------------------
//���ܣ���ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�	
//������dir_param��dir����Ĳ������������ļ���·�����߲�����
//���أ�true����ʾ�ɹ�
//    	false,��ʾʧ��
//-----------------------------------------------------------------------------
bool_t dir_parent(char *next_param)
{
    char *foldername=NULL;
    struct file_rsc *fp;
    uint32_t lenth=0;
    char work_path[cn_shell_cmd_limit+1];

    //�������ʹ��get_work_path�ˡ����棬dir ..\folder����Ҫ                                                        ->private_tag);
    get_work_path_name(work_path,cn_shell_cmd_limit);
    do   
    {
        foldername = (char *)sh_get_item(next_param,&next_param);
    	if(foldername == NULL&&lenth==0)
        	return false;
        //ִ�� ��: cd ..\foder
        if(strcmp(foldername,"..") != 0)
        {
            str_end_increase_backslash(work_path);
            strcat(work_path,foldername);
            fp = djyfs_fopen(work_path,"r");
            if(fp == NULL)
            {
                printf_ng("ϵͳ�Ҳ���ָ����·��!\r\n");
                return false;
            }
            dir_item_son(fp);
            djyfs_fclose(fp);
            break;
        }
        __cut_path_end_item(work_path);
        if(next_param == NULL)
        {
            fp = djyfs_fopen(work_path,"r");
            dir_item_son(fp);
            djyfs_fclose(fp);
            break;
        }
        //·����ȴ���cn_path_depth_limit��������ѭ���������
        //��Ϊ�˷�ֹ��ѭ������һ��������
        lenth++;
     }while((lenth!=cn_path_depth_limit)&&(foldername != NULL));    
    return true;
}

//----���ĵ�ǰ����·������Ŀ¼-------------------------------------------------------
//���ܣ����ĵ�ǰ����·����ԭ����ǰ����·���ĸ�Ŀ¼
//������next_param������Ĳ���
//���أ�true����ʾ�ɹ�
//    	false,��ʾʧ��
//-----------------------------------------------------------------------------

bool_t cd_parent(char *next_param)
{
    char *foldername=NULL;
    struct file_rsc *fp;
    uint32_t lenth=0;
    struct st_DBX_device_tag *DBX_device_tag;

    DBX_device_tag = (struct st_DBX_device_tag *)(pg_work_path->home_DBX
                                                            ->private_tag);
    do   
    {
        foldername = (char *)sh_get_item(next_param,&next_param);
    	if(foldername == NULL&&lenth==0)
        	return false;
        if(foldername == NULL&&lenth!=0)
        	return true;
        //ִ�� ��: cd ..\foder
        if(strcmp(foldername,"..") != 0)
        {
            //��Ŀ¼���ܴ򿪣����ÿ��Ǹ�Ŀ¼�������
            //����"cd ..\foder",�ڸ�Ŀ¼��ִ�У���û���塣
            //��ֻ����ʽ�򿪣���������ڣ��ͷ���NULL��
            //�Ͳ���ȥר��������Ŀ¼�����Ŀ¼�Ƿ���ڡ�
        	if(judge_is_folder(foldername))
            {
            	fp = pg_work_path;
            	pg_work_path = djyfs_fopen(foldername,"r");
            	djyfs_fclose(fp);
            	return true;
            }
        	else
            {
                printf_ng("ϵͳ�Ҳ���ָ��·��\r\n");
                return false;
            } 
        }
    	if(pg_work_path!=DBX_device_tag->opened_root)
        {    
        	fp=pg_work_path;//��ǰ����·�����ܹر�,��ת��
            pg_work_path = (struct file_rsc*)pg_work_path->file_node.parent;
           // pg_work_path->open_counter++;
            djyfs_fclose(fp);
        }
        else
            break;
        //·����ȴ���cn_path_depth_limit��������ѭ���������
        //��Ϊ�˷�ֹ��ѭ������һ��������
        lenth++;
     }while((lenth!=cn_path_depth_limit)&&(foldername != NULL));    
    return true;
}


//----��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�-------------------------------------------------------
//���ܣ���ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�	
//������dir_param��dir����Ĳ������������ļ���·�����߲�����
//���أ�true����ʾ�ɹ�
//    	false,��ʾʧ��
//-----------------------------------------------------------------------------

bool_t cd_dbx(char *dbx)
{
    char work_path[cn_shell_cmd_limit+1]; 
    struct file_rsc *fp;
    
    strcpy(work_path,dbx);
    strcat(work_path,"\\");
    if(djyfs_fsearch(work_path))
    {
        fp = djyfs_fopen(work_path,"r");
        if(fp != NULL)
        {
            pg_work_path = fp;
            return true;
        }
        else
            return false;
    }
    else
    { 
    	printf_ng("���ļ��񲻴���\r\n");
        return false;
    }
}

//���ƴ���̫�࣬���������!������ĥ��ƽ���!��֮��������һ����bug��

//----����һ���ļ���Ŀ��------------------------------------------------
//����: ��һ���ļ����Ƶ���Ҫ�����Ƶĵط�
//����:  src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//    	�������޸ġ�
//    	dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t copy_file_to_dest(char *src,char *dest)
{
	uint32_t file_size,loop,loop_times,buf_size;
	uint32_t buf_size_offset,size;
	struct file_rsc *src_fp;
	struct file_rsc *dest_fp;
	char* buf;
    
	src_fp = djyfs_fopen(src,"r");
	if(src_fp == NULL)
    	return false;
	file_size = src_fp->file_size;
	djyfs_fclose(src_fp);
    ////����16k�ڴ�,��ֹ�ڴ治��
    //������ڴ治��̫С�������ļ��󣬲��������࣬Ӱ��copy���ٶ�
    //̫�����벻�˾��군�ˣ���ʱ��Ϊ16k�����һ��flash��Ĵ�С
    //��Ƶ������֮����ȷ��
	buf_size = 16384;
	loop_times =(file_size+buf_size-1)/buf_size;
    if(loop_times==0)
    {//loop_times==0ʱ��˵���ļ���СΪ0.ֻ��Ҫ��������ok
        dest_fp = djyfs_fopen(dest,"a+");
        djyfs_fclose(dest_fp);
        return true;
    }
    buf = (char *)m_malloc(file_size,0);
    if(buf == NULL)
        return false;
	buf_size_offset = file_size%buf_size;
    if(buf_size_offset == 0)
        buf_size_offset = buf_size;
    src_fp = djyfs_fopen(src,"r");
    dest_fp = djyfs_fopen(dest,"a+");
	for(loop=0;loop<loop_times;loop++)
    {
    	if(loop!=loop_times-1)
        	size = buf_size;
    	else//д�������С��������ڴ棬��Ӧ��д��buf_size�Ĵ�С
        	size = buf_size_offset;
    	djyfs_fseek(src_fp, (sint64_t)(size*loop), SEEK_SET);
       	djyfs_fread(buf, (size_t)size, 1,src_fp);
    	djyfs_fwrite((void *)buf,(size_t)size,1,dest_fp);    
    }
    djyfs_fclose(src_fp);
    djyfs_fclose(dest_fp);
	m_free(buf);
	return true;
}
//----�ƶ�һ��Ŀ¼�µ��ļ���Ŀ��------------------------------------------------
//����: ��һ��Ŀ¼���ļ��ƶ�����Ҫ���ƶ��ĵط�
//����: src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//    	�������޸ġ�
//    	dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t copy_files_of_folder_to_dest(char *src,char *dest)
{

	uint32_t file_nunmber=0,fat_block_no;
	struct file_rsc *dest_fp,*src_fp,*current_fp=NULL;
    if(!djyfs_fsearch(dest))
    {
    	printf_ng("djyos��֧�ִ˲���,�����뿴help!\r\n");
    	return false;
    }    
	dest_fp = djyfs_fopen(dest,"r");
    if(dest_fp == NULL)
        return false;
    //djyfs��֧�ֶ���ļ��ϲ�д�뵽һ���ļ���
	if(dest_fp->attr.bits.archive)
    {
    	printf_ng("djyos��֧�ִ˲���,�����뿴help!\r\n");
    	djyfs_fclose(dest_fp);
    	return false;
    }
	djyfs_fclose(dest_fp);
    
	src_fp = djyfs_fopen(src,"r");
    if(src_fp == NULL)
    {    
    	djyfs_fclose(dest_fp);
    	return false;
    }                             
	current_fp = (struct file_rsc *)djyfs_folder_traversal_son(src_fp,current_fp);
    //����ѭ��һ�ξ͹رգ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
   	djyfs_fclose(src_fp);
    if(current_fp == NULL)
    {
        printf_ng("%s*\r\n", src);
        printf_ng("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        printf_ng("�Ѿ�������	    	0���ļ�\r\n");//��ʱ����
        djyfs_find_close(current_fp);
        return false;  //parent��һ����Ŀ¼
    }    
    fat_block_no = current_fp->file_medium_tag;
    do
    {

        if(current_fp->attr.bits.archive)
        {
            strcat(src,current_fp->name);
            strcat(dest,current_fp->name);
            copy_file_to_dest(src,dest);
            printf_ng("%s\r\n", src);
            __cut_path_end_item(src);
            __cut_path_end_item(dest);
            file_nunmber++;
        }
        src_fp = djyfs_fopen(src,"r");
    	if(src_fp == NULL)
        {    
        	djyfs_fclose(dest_fp);
        	return false;
        }                             
        //����ѭ��һ�ξ͹ر�һ�Σ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
       	djyfs_fclose(src_fp);
        current_fp = (struct file_rsc *)djyfs_folder_traversal_son(src_fp,current_fp);
        if(current_fp ==NULL)
            break;
    } while(fat_block_no!=current_fp->file_medium_tag);   
        djyfs_find_close(current_fp);       
    	printf_ng("�Ѿ�������	    	%d���ļ�\r\n", file_nunmber);
    	return true;                   
}

//���ƴ���̫�࣬���������!������ĥ��ƽ���!��֮��������һ����bug��

//----�ƶ�һ���ļ���Ŀ��------------------------------------------------
//����: ��һ���ļ��ƶ�����Ҫ���ƶ��ĵط�
//����:  src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//    	�������޸ġ�
//    	dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t move_file_to_dest(char *src,char *dest)
{
	uint32_t file_size,loop,loop_times,buf_size;
	uint32_t buf_size_offset,size;
	struct file_rsc *src_fp;
	struct file_rsc *dest_fp;
	char* buf;

	src_fp = djyfs_fopen(src,"r");
	if(src_fp == NULL)
    	return false;
	file_size = src_fp->file_size;
	djyfs_fclose(src_fp);
    ////����16k�ڴ�,��ֹ�ڴ治��
    //������ڴ治��̫С�������ļ��󣬲��������࣬Ӱ��copy���ٶ�
    //̫�����벻�˾��군�ˣ���ʱ��Ϊ16k�����һ��flash��Ĵ�С
    //��Ƶ������֮����ȷ��
	buf_size = 16384;
	loop_times =(file_size+buf_size-1)/buf_size;
    if(loop_times == 0)
    {
    	dest_fp = djyfs_fopen(dest,"a+");
    	djyfs_fclose(dest_fp);  
        djyfs_remove(src);
    }
    buf = (char *)m_malloc(file_size,0);
    if(buf == NULL)
        return false;
	buf_size_offset = file_size%buf_size;
    if(buf_size_offset == 0)
        buf_size_offset = buf_size;
    src_fp = djyfs_fopen(src,"r");
    dest_fp = djyfs_fopen(dest,"a+");
	for(loop=0;loop<loop_times;loop++)
    {
    	if(loop!=loop_times-1)
        	size = buf_size;
    	else//д�������С��������ڴ棬��Ӧ��д��buf_size�Ĵ�С
        	size = buf_size_offset;
    	djyfs_fseek(src_fp, (sint64_t)(size*loop), SEEK_SET);
       	djyfs_fread(buf, (size_t)size, 1,src_fp);
    	djyfs_fwrite((void *)buf,(size_t)size,1,dest_fp);
    }
    djyfs_fclose(src_fp);
    djyfs_fclose(dest_fp);
    djyfs_remove(src);
	m_free(buf);

	return true;
}

//----�ƶ�һ��Ŀ¼�µ��ļ���Ŀ��------------------------------------------------
//����: ��һ��Ŀ¼���ļ��ƶ�����Ҫ���ƶ��ĵط�
//����: src����ǰ�Ĺ���·������������work_path����Ϊwork_patch��ȫ�ֱ�����
//    	�������޸ġ�
//    	dest���������·���Ǹ��Ƶ�Ŀ���·��
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t move_files_of_folder_to_dest(char *src,char *dest)
{

	uint32_t file_nunmber=0,fat_block_no=0xffffffff;
	struct file_rsc *dest_fp,*src_fp,*current_fp=NULL;
    if(!djyfs_fsearch(dest))
    {
    	printf_ng("djyos��֧�ִ˲���,�����뿴help!\r\n");
    	return false;
    }    
	dest_fp = djyfs_fopen(dest,"r");
    if(dest_fp == NULL)
        return false;
    //djyfs��֧�ֶ���ļ��ϲ�д�뵽һ���ļ���
	if(dest_fp->attr.bits.archive)
    {
    	printf_ng("djyos��֧�ִ˲���,�����뿴help!\r\n");
    	djyfs_fclose(dest_fp);
    	return false;
    }
	djyfs_fclose(dest_fp);
    
	src_fp = djyfs_fopen(src,"r");
    if(src_fp == NULL)
    {    
    	djyfs_fclose(dest_fp);
    	return false;
    }                             
	current_fp =(struct file_rsc *) djyfs_folder_traversal_son(src_fp,current_fp);
    //����ѭ��һ�ξ͹رգ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
   	djyfs_fclose(src_fp);
    if(current_fp == NULL)
    {
        printf_ng("%s*\r\n", src);
        printf_ng("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        printf_ng("�Ѿ��ƶ���	    	0���ļ�\r\n");//��ʱ����
        djyfs_find_close(current_fp);
        return false;  //parent��һ����Ŀ¼
    }    
    //strcpy(name,current_fp->name);
    do
    {

        if(current_fp->attr.bits.archive)
        {
            strcat(src,current_fp->name);
            strcat(dest,current_fp->name);
            move_file_to_dest(src,dest);
            printf_ng("%s\r\n", src);
            __cut_path_end_item(src);
            __cut_path_end_item(dest);
            file_nunmber++;
        }
        else if(!current_fp->attr.bits.archive)
        {
            if(fat_block_no==0xffffffff)//��һ���Ļ��������child
                fat_block_no = current_fp->file_medium_tag;
        }
        src_fp = djyfs_fopen(src,"r");
    	if(src_fp == NULL)
        {    
        	djyfs_fclose(dest_fp);
        	return false;
        }                             
        //����ѭ��һ�ξ͹ر�һ�Σ��������ݲ�׼ȷ����Щ�ļ����ܱ�ɾ��
       	djyfs_fclose(src_fp);
        current_fp = (struct file_rsc *)djyfs_folder_traversal_son(src_fp,current_fp);
        if(current_fp ==NULL)
            break;
        if(current_fp->attr.bits.archive)
        {
           continue;
        }
    	if(fat_block_no==current_fp->file_medium_tag)
        	break;
    } while(current_fp !=NULL);   
        djyfs_find_close(current_fp);       
    	printf_ng("�Ѿ��ƶ���	    	%d���ļ�\r\n", file_nunmber);
    	return true;                   
}

//----��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�-----------------------------------------
//���ܣ���ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�	
//������dir_param��dir����Ĳ������������ļ�
//���أ�true����ʾ�ɹ�
//    	false,��ʾʧ��
//-----------------------------------------------------------------------------

bool_t fs_dir(char *dir_param)
{
	struct file_rsc *fp;
	bool_t result;
	char work_path[cn_shell_cmd_limit+1];

    if(dir_param == NULL)
        return false;
    if(__check_wildcard(dir_param))
    {
    	dir_wildcard(dir_param);
    	return true;
    }     
    if(strcmp(dir_param,"\\")==0)//ִ�� dir [\]
    {
        if(!get_DBX_name(work_path,cn_shell_cmd_limit))
            return false;
        strcat(work_path,":\\");
        //�򿪸�Ŀ¼ ��:"C:\\"
        fp = djyfs_fopen(work_path,"r");
        if(fp == NULL)
        {
            printf_ng("�Ҳ����ļ�\r\n");
            return false;
        }
        dir_item_son(fp);
        djyfs_fclose(fp);
        return true;
    }
    if(dir_param[0]=='.' && dir_param[1]=='.')//ִ��dir	[..]
    {    
        if(dir_parent(dir_param))
            return true;
        else
            return false;
    }
    if(!__check_fullname(dir_param))
    {
        printf_ng("·����ʽ����ȷ!\r\n");
            return false;
    }
    result = judge_is_folder(dir_param);
    if(result == false)
    {
        printf_ng("ϵͳ�Ҳ���ָ����·��\r\n");
        return false; 
    }
    fp = djyfs_fopen(dir_param,"r");
    if(fp == NULL)
    {
        printf_ng("�Ҳ����ļ�\r\n");
        return false;
    }
    dir_item_son(fp);
    djyfs_fclose(fp);
	return true;
}
//----�г��ļ����ڵĿռ����--------------------------------------------------
//���ܣ����ļ���Ĵ�С����Ч�ռ䣬���д�С���г���
//������DBX_name �ļ�����
//���أ�true �ɹ�
//-----------------------------------------------------------------------------
bool_t sh_chkdbx(char *DBX_name)
{
	sint64_t sum_size;
	sint64_t valid_size;
	sint64_t free_size;

	struct st_DBX_device_tag *DBX_device_tag;
	djy_handle_t DBX_lhdl;
	static djy_handle_t pg_fs_lhdl;
    
    if(DBX_name == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
	pg_fs_lhdl = dev_open_left("fs",0); 
	DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_name,0); //���ļ����豸
	if(DBX_lhdl == NULL)
    {
        printf_ng("���ļ�����Ч!\r\n");
        return false;
    }
    DBX_device_tag = (struct st_DBX_device_tag*)
                                dev_get_my_tag(dev_get_panp(DBX_lhdl));
	DBX_device_tag->check_DBX(DBX_device_tag,&sum_size,&valid_size,&free_size);
    
	//���洢�豸����4G���͵�ʹ��64λ�Ĵ�ӡ������
	printf_ng("�ļ���%s��С��%dB\r\n", DBX_name, (uint32_t)sum_size);
	printf_ng("�ļ���%s��Ч��С��%dB\r\n", DBX_name, (uint32_t)valid_size);
	printf_ng("�ļ���%s���д�С��%dB\r\n", DBX_name, (uint32_t)free_size);

	dev_close_left(DBX_lhdl); //ǧ��Ҫ���豸�ر�,����ڶ�������������������ò����豸
	dev_close_left(pg_fs_lhdl);
 	return true;
}


//----������һ���ļ����Ƶ�����һ��λ��--------------------------------------------------
//���ܣ�������һ���ļ����Ƶ�����һ��λ��
//������DBX_name �ļ�����
//���أ�true �ɹ�
//-----------------------------------------------------------------------------
bool_t sh_copy(char *param)
{
    char *next_param,source[255],destination[255];
    struct file_rsc *fp,*second_fp;
    char s[255];      
	if(param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
    strcpy(source,sh_get_word(param,&next_param));

	if(NULL == next_param)
    {
    	printf_ng("djyfs��֧�ִ˲����������뿴help\r\n");
    	return false;
    }    
    strcpy(destination,sh_get_word(next_param,&next_param));
    if((!__check_fullname(source))||(!__check_fullname(destination)))
        return false; 
    if(!djyfs_fsearch(source))
    {
        printf_ng("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        return false; 
    }
    //����������ļ�һ�������أ�����������ΪЧ��һ��
    if(strcmp(source,destination)==0)
        return true;
    if(djyfs_fsearch(destination))
    {
        second_fp = djyfs_fopen(destination,"r");
        if(second_fp == NULL)
        {
            return false;
        }
        if(second_fp->attr.bits.archive)
        {
            printf_ng("�Ƿ񸲸�%s�� <yes/no>", destination);
            djyfs_fclose(second_fp); 
            while(1)
            {
                djy_scanf(NULL,s);
                if(s[0] == 'y'||s[0] == 'Y')
                {
                	putchar_ng(s[0]);
                    printf_ng("\r\n");
                    djyfs_remove(destination);//Ϊ������׼��
                    break;
                }
                else if(s[0] == 'n'||s[0] == 'N')
                {
                	putchar_ng(s[0]);
                    printf_ng("\r\n");
                    return false;        
                }
            }   
        }
        else
        {   
            //��Ŀ¼����Ҫ���ӷ�б�ܣ��Ա����濽��·��ʹ��   
            str_end_increase_backslash(destination);
            fp = djyfs_fopen(source,"r");
            if(fp == NULL)
            {
                djyfs_fclose(second_fp);
                return false;
            }
            if(fp->attr.bits.archive)
            {//���source_file���ļ������ļ������뵽second_work_pathĩβ
            //ʹ����������������ṩ׼ȷ��·����
                str_end_wipe_backslash(source);
                __pick_filename_word(source,s);
                strcat(destination,s);      
             }
                djyfs_fclose(second_fp);
                djyfs_fclose(fp);
        }
    }
    else//���destination�����ڣ���ôȫ�������ļ�����
        str_end_wipe_backslash(source);//���Բ��üӷ�б�ܰ�?
    fp = djyfs_fopen(source,"r");
    if(fp == NULL)
        return 0;
    if(!fp->attr.bits.folder)//source��һ���ļ�
    {   
        djyfs_fclose(fp);
        copy_file_to_dest(source,destination);
        printf_ng("�Ѿ�������      1���ļ�\r\n");
    }
    else//source��һ��Ŀ¼
    {
        djyfs_fclose(fp);
        str_end_increase_backslash(source);
        copy_files_of_folder_to_dest(source,destination);
    }
    return true;    
}


//----������һ���ļ��ƶ�������һ��λ��--------------------------------------------------
//���ܣ�������һ���ļ��ƶ�������һ��λ��
//������DBX_name �ļ�����
//���أ�true �ɹ�
//-----------------------------------------------------------------------------
bool_t sh_move(char *param)
{
    char *next_param,source[255],destination[255];
    struct file_rsc *fp,*second_fp;
    char s[255];      
	if(param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
    strcpy(source,sh_get_word(param,&next_param));

	if(NULL == next_param)
    {
    	printf_ng("djyfs��֧�ִ˲����������뿴help\r\n");
    	return false;
    }    
    strcpy(destination,sh_get_word(next_param,&next_param));
    if((!__check_fullname(source))||(!__check_fullname(destination)))
        return false; 
    if(!djyfs_fsearch(source))
    {
        printf_ng("ϵͳ�Ҳ���ָ�����ļ�\r\n");
        return false; 
    }
    //����������ļ�һ�������أ�����������ΪЧ��һ��
    if(strcmp(source,destination)==0)
        return true;
    if(djyfs_fsearch(destination))
    {
        second_fp = djyfs_fopen(destination,"r");
        if(second_fp == NULL)
        {
            return false;
        }
        if(second_fp->attr.bits.archive)
        {
            printf_ng("�Ƿ񸲸�%s�� <yes/no>", destination);
            djyfs_fclose(second_fp); 
            while(1)
            {
                djy_scanf(NULL,s);
                if(s[0] == 'y')
                {
                	putchar_ng(s[0]);
                    printf_ng("\r\n");
                    djyfs_remove(destination);//Ϊ������׼��
                    break;
                }
                else if(s[0] == 'n')
                {
                	putchar_ng(s[0]);
                    printf_ng("\r\n");
                    return false;        
                }
            }   
        }
        else
        {   
            //��Ŀ¼����Ҫ���ӷ�б�ܣ��Ա����濽��·��ʹ��   
            str_end_increase_backslash(destination);
            fp = djyfs_fopen(source,"r");
            if(fp == NULL)
            {
                djyfs_fclose(second_fp);
                return false;
            }
            if(fp->attr.bits.archive)
            {//���source_file���ļ������ļ������뵽second_work_pathĩβ
            //ʹ����������������ṩ׼ȷ��·����
                str_end_wipe_backslash(source);
                __pick_filename_word(source,s);
                strcat(destination,s);      
             }
                djyfs_fclose(second_fp);
                djyfs_fclose(fp);
        }
    }
    else//���destination�����ڣ���ôȫ�������ļ�����
        str_end_wipe_backslash(source);
    fp = djyfs_fopen(source,"r");
    if(fp == NULL)
        return 0;
    if(!fp->attr.bits.folder)//source��һ���ļ�
    {   
        djyfs_fclose(fp);
        move_file_to_dest(source,destination);
        printf_ng("�Ѿ��ƶ���      1���ļ�\r\n");
    }
    else//source��һ��Ŀ¼
    {
        djyfs_fclose(fp);
        str_end_increase_backslash(source);
        move_files_of_folder_to_dest(source,destination);
    }
    return true;    
}

//----�޸�ָ���ļ������ļ��е�����------------------------------------------------
//����: ��������ren���޸�ָ�����ļ������ļ���
//����: param���������
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t sh_ren(char *param)
{
	char *next_param;
	char *fullname;//ָ�����޸ĵ��ļ�
	char *filename;//�޸ĺ������

	if(param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
    fullname = sh_get_word(param,&next_param);
    filename = sh_get_word(next_param,&next_param);
	if(!__check_fullname(fullname))
    	return false; 
	if(!djyfs_fsearch(fullname))
    {
    	printf_ng("ϵͳ�Ҳ���ָ�����ļ�\r\n");
     	return false; 
    }        
	djyfs_rename(fullname,filename);
	return true;

}    
//----����һ�����߶��Ŀ¼------------------------------------------------
//����: ��������md������һ�����߶��Ŀ¼
//����: param���������
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t sh_md(char *param)
{
	char *next_param;
	char *del_param;
    //����255�ַ�����Ϊfs_mkdir�Ĳ�����Ҫ���������ַ���
    //��������ƣ����ڴ�й©����ɲ��ɿ�
	char str[cn_shell_cmd_limit+1];
    
	del_param = sh_get_word(param,&next_param);
	if(del_param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
	strcpy(str,del_param);
	do
    {    
    	fs_mkdir(str);
    	del_param = sh_get_word(next_param,&next_param);
    	strcpy(str,del_param);    
    }while(del_param!=NULL);
	return true;
}


//----ɾ��һ�����߶���ļ���--------------------------------------------------------------
//����: ��ָ����һ�����߶���ļ���ɾ����
//����: rd_param�������ַ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t sh_rd(char *param)
{
	char *next_param;
	char *del_param;
	char str[255];

	if(param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
	if(__check_wildcard(param))
    {//windows�ǲ�֧��ɾ��ͨ���.���ۣ��Ƿ���Ҫ?
    //��Ҫ��ʱ�򣬸ú�������ļ�Ҳ��ȡ����������һ��������⣬rdҲ
    //����ʹ��ͨ���
    	next_param = get_folder_wildcard_str(param,str);
    }
    else
        next_param = param;
	do
    {
        del_param = sh_get_word(next_param,&next_param);
        if(del_param == NULL)
            break;
        fs_remove_folder(del_param);   
    }while(del_param != NULL);
    
	return true;
}

//----ɾ��һ�����߶���ļ�-----------------------------------------------------
//����: ɾ��һ�����߶���ļ�
//����: param�������ַ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t sh_del(char *param)
{
	char *next_param;
	char *del_param;
	char new_param[255];
    //char s[255];

    //��������Ϊ���ַ�
	if(param == NULL)
    {
    	printf_ng("�����﷨����ȷ!\r\n");
    	return false;
    }
	if(__check_wildcard(param))
    {//new_param��shell�������ܳ��Ȳ������255�ַ�
    	del_wildcard_files(param);
        return true;
    }
	else
        strcpy(new_param,param);
	del_param = sh_get_word(new_param,&next_param);
	do
    {
        fs_remove_file(del_param);    
    	del_param = sh_get_word(next_param,&next_param);
    }while(del_param != NULL);
	return true;
}

//----��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼-----------------------------------------
//����: ��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼
//����: chdir_param�������ַ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t sh_cd(char *chdir_param)
{        
	bool_t result;
	struct file_rsc *fp;
	char work_path[cn_shell_cmd_limit+1];
    
    //ִ��cd/chdir[](���ַ�) ��ʾ��ǰĿ¼����
	if(chdir_param == NULL)
    {
    	print_work_path();
    	printf_ng("\r\n");
    	return true;
    }
    //�����û�����Ŀ¼��ʱ�������뷴б��
    //��"cd folder\"���"cd folder"
    //ִ��cd/chdir[\] ��������Ŀ¼
	if(strcmp(chdir_param,"\\") ==0)
    {
        if(!get_DBX_name(work_path,cn_shell_cmd_limit))
            return false;
    	strcat(work_path,":\\");
    	fp=pg_work_path;
    	pg_work_path = djyfs_fopen(work_path,"r");
    	djyfs_fclose(fp);
        //�����ʧ�ܣ��Ƿ�Ҫ����?��ʱ����˵
    	return true;
    }
    //ִ��cd/chdir[..] ��������һ��Ŀ¼
	else if(chdir_param[0]=='.' && chdir_param[1]=='.')//ִ�� "cd .."
    {
        if(cd_parent(chdir_param))
            return true;
        else
            return false;
    }
	else
    {
    	if(!__check_fullname(chdir_param))
         	return false; 
    	result = djyfs_fsearch(chdir_param);
        //ִ�� cd/chdir[�ļ���]·�� ��ʾ��ǰĿ¼�����ƻ�ı䵱ǰĿ¼
    	if(result == true)
        {//��Ŀ¼���ܴ�	        
        	if(judge_is_folder(chdir_param))
            {
            	fp = pg_work_path;
            	pg_work_path = djyfs_fopen(chdir_param,"r");
            	djyfs_fclose(fp);
            	return true;
            }
        	else if(!__judge_backslash_if_data(chdir_param))
            {//��C:\,��judge_is_folder���޷��ɹ��ж�
            	fp = pg_work_path;
            	pg_work_path = djyfs_fopen(chdir_param,"r");
            	djyfs_fclose(fp);
            	return true;    
            }
        	else
            {
                printf_ng("ϵͳ�Ҳ���ָ����·��\r\n");
                return false;
            }
        }
    	else
            	printf_ng("ϵͳ�Ҳ���ָ����·��\r\n");
    }
 	return true;
}
//-------��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�--------------------------------------
//����: ��ʾָ��Ŀ¼�µ��ļ�����Ŀ¼�б�
//����: param�������ַ�
//����: true=�ɹ�, 
//      false=ʧ�ܣ�
//-----------------------------------------------------------------------------
bool_t sh_dir(char *param)
{
	char *next_param,*dir_param;
    char str[cn_shell_cmd_limit+1];

	if(param == NULL)
    {
    	dir_item_son(pg_work_path);
    	return true;
    }
	dir_param = sh_get_word(param,&next_param);
	do
    {
        strcpy(str,dir_param);
        fs_dir(str);
    	dir_param = sh_get_word(next_param,&next_param);
    }while(dir_param != NULL);
    
	return true;
}


bool_t sh_create_dbx(char *param)
{
    struct flash_chip *chip;
    struct rsc_node *chip_tree;
    u64  dbx_size;
    u32 blocks;
    char *word,*next_param;

    chip_tree = rsc_search_tree("flash chip");
    chip = (struct flash_chip *)rsc_get_son(chip_tree);
    
    word = sh_get_word(param,&next_param);
    dbx_size = __sh_atol(word);
    blocks = dbx_size / chip->block_size+1;
    if(blocks > chip->valid_sum)
    {
        printf_ng("�ߴ�̫��\r\n");
        return true;
    }
    word = sh_get_word(next_param,&next_param);
    if(word == NULL)
    {
        printf_ng("�ļ���������Ϊ��");
        return true;
    }
    
    if(DFFSD_create_DBX(chip,blocks,word) == true)
    {
        printf_ng("�����ļ���ɹ�");
    }
    else
    {
        printf_ng("�����ļ���ʧ��");
    }
    printf_ng("\r\n");
    return true;
}

bool_t sh_format(char *param)
{
    if(param == NULL)
    {
        printf_ng("�ļ���������Ϊ��");
        return true;
    }
    
    if(djyfs_format(1,0,param) == 0)
    {
        printf_ng("��ʽ���ɹ�");
    }
    else
    {
        printf_ng("��ʽ��ʧ��");
    }
    printf_ng("\r\n");
    return true;
}
#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))

