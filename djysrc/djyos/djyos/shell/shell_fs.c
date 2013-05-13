//----------------------------------------------------
//Copyright (C), 2011.9,wjz.
//版权所有 (C), 2011.9,wjz
//所属模块: djysh模块
//作者：wjz
//版本：djysh1.0.0
//文件描述: djysh的文件系统shell命令。
//修订历史:
// 1. 日期:2010-6-15
//   作者:wjz
//   新版本号：djysh1.0.0
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

//----检查字符串是否有'*'或者'?'-----------------------------------------------
//功能: 检查字符串是否有星号或者问号,查找长度以命令行最大长度为限。
//参数: str,被测字符串
//返回: true=有'*'或者'?'，false=没'*'或者'?'
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

//-------带通配符的字符匹配-----------------------------------------------------
//功能: 带通配符的字符匹配最常见的通配符是'?'和'*'。其中,'?'可以代表一个字符
//        (不能没有),'*'可以代表任意多个字符(可以为空)。首先是'?'， 根据'?'
//    	的功能,'?'表示任意字符,也就是说在匹配过程中,'?'永远匹配成功 。
//    	本质上,'?'并没有修改算法，而仅仅修改了匹配规则 -- 遇到'?'则一定匹配 。
//    	然而'*'与此不同,'*'的作用是匹配任意多个字符，显然我们不能简单的修改匹配
//    	过程而满足要求。
//参数: src - 待匹配的字符串,不能含有'?'或'*'
//    	wildcard - 通配符字符串
//返回: true=匹配成功,
//      false=匹配失败,
//-----------------------------------------------------------------------------
bool_t __wildcard_match(char *src, char *wildcard)
{
	u32 i = 0;
	u32 j = 0;

	while (wildcard[j] || src[i])
	{
		if (wildcard[j] == '*')
		{
			while (wildcard[j] == '*')	// 找到一连串*号的最后一个*号
			{
				j++;
				if (wildcard[j] == '\0')
				{
					return true;	// 匹配
				}
			}
			while (src[i] != wildcard[j])	// 匹配src中与*后第一个字符
			{
				if (src[i])
				{
					i++;
				}
				else
				{
					return false;	// 不匹配
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

//----判断'\'后面是否有数据-----------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串中判断第一次遇到'\'之后是否还有
//      数据
//参数: path，被测路径，格式如"C:\aaaa\bbbb\cccc"
//返回: true=成功, 说明'\'之后有数据,或者没有'\'
//      false=失败，说明'\'之后没有数据
//备注: path应该是经过合法性检查的字符串，不会出现word超长的问题
//-----------------------------------------------------------------------------
bool_t __judge_backslash_if_data(char *path)
{
    uint32_t index;
    uint32_t name_len;

	name_len = strnlen(path,cn_shell_cmd_limit);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == '\\')    //遇到字符'\'
        {
            if(path[index+1] != '\0')
            	return true;
        	else
            	return false;
        }
    }
    //字符串里没有反斜杠
	if(index == name_len)
    	return true;
	else
    	return false;            
}

//----判断':'后面是否有数据-----------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串中判断第一次遇到'\'之后是否还有
//      数据
//参数: path，被测路径，格式如"C:\aaaa\bbbb\cccc"
//返回: true=成功, 说明':'之后有数据,或者没有':'
//      false=失败，说明':'之后没有数据
//备注: path应该是经过合法性检查的字符串，不会出现word超长的问题
//-----------------------------------------------------------------------------
bool_t __judge_colon_if_data(char *path)
{
    uint32_t index;
    uint32_t  name_len;

	name_len = strnlen(path,cn_shell_cmd_limit);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')    //遇到字符'\'
        {
            if(path[index+1] != '\0')
            	return true;
        	else
            	return false;
        }
    }
    //字符串里没有冒号
	if(index == name_len)
    	return true;
	return false;        
}

//----删除路径后的文件或者目录-------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串中删除最后一项，如果path是一个根
//      目录名，则不操作
//参数: path，被测路径，格式如"C:\aaaa\bbbb\cccc"
//返回: true=成功删除，或无须删除
//      false=失败
//-----------------------------------------------------------------------------
bool_t __cut_path_end_item(char *path)
{
	uint16_t name_len,loop;
    
	name_len = strnlen(path,cn_shell_cmd_limit);
	loop=name_len;
	if(path[name_len-1]=='\\')//有反斜杠说明是去除文件夹
    {
    	if(path[name_len-2]==':')
        	return true;//说明是根目录，不用操作
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
	else//没有反斜杠，说明去除的是文件
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
//----提取路径后的文件或者目录--------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串中提取最后一项。如果path是根目录
//      则不操作。path串保持不变。
//参数: path，被测路径，格式如"C:\aaaa\bbbb\cccc"
//    	name,保存提取的文件或者目录
//返回: true=成功, 提取成功
//      false=失败，提取失败
//-----------------------------------------------------------------------------
bool_t __pick_path_end_item(char *path,char *name)
{
	uint16_t name_len,loop;
	char *str;
    
	name_len = strnlen(path,cn_shell_cmd_limit);
	loop=name_len;
	if(path[name_len-1]=='\\')//有反斜杠，说明提取的是文件夹名
    {
    	if(path[name_len-2]==':')
        	return false;   //说明是根目录，不用操作
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
	else//没有反斜杠，说明提取的是文件名
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
    //执行到这步，说明只是一个文件或者目录，直接拷贝就ok
    strcpy(name,path);    
	return true;
}

//----判断字符串是绝对路径还是相对路径-------------------------------------------------
//功能: 检查一个可能包含多级路径名和文件名的字符串是否包含文件柜名的绝对路径
//参数: path，被测路径，格式如"C:\aaaa\bbbb\cccc"
//返回: true=是绝对路径
//      false=不是绝对路径
//备注: path应该是经过合法性检查的字符串，不会出现word超长的问题
//-----------------------------------------------------------------------------
bool_t __judge_if_absolute_path(char *path)
{
    uint32_t index;
    //synname是已经经过字符串长度合法性检查的指针
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
//----目录下匹配的文件提取到一个字符串--------------------------------------
//功能: 在一个目录下扫描存在的文件，然后每个文件通过匹配函数和wildcard_param进行
//    	一一匹配，把匹配的文件名提到到一个字符串里。
//参数: wildcard_param，含有通配符的参数
//    	get_merge_str ，最后符合要求的文件名合并而成的字符串
//返回: true=成功, 提取成功
//      false=失败，提取失败
//-----------------------------------------------------------------------------
char *get_folder_wildcard_str(char *wildcard_param,char *get_merge_str)
{
	struct file_rsc *fp = NULL,*current_fp = NULL;
	char work_path[256];
    u32 fat_block_no;

 
    if(__djyfs_if_contain_DBX_name(wildcard_param) ==0)
        fp = pg_work_path;//相对路径，直接使用工作路径
    else
    {    
    	strcpy(work_path,wildcard_param);
    	__pick_filename_word(work_path,wildcard_param);
        __cut_path_end_item(work_path);
        fp = djyfs_fopen(work_path, "r");    
    }
    //是文件，没意义（同时fp2是非根目录,根目录下，文件属性不确定）
    if(fp->attr.bits.archive&&(fp->file_medium_tag!=0))
        return NULL;
	current_fp =(struct file_rsc *)djyfs_folder_traversal_son(fp,current_fp);
	if(current_fp == NULL)
    {    
    	djyfs_fclose(fp);
    	printf_ng("该目录为空!\r\n");
    	return NULL;
    }
	fat_block_no = current_fp->file_medium_tag;
        //全部置0，这样就可以准确知道本来merge_str已经没有数据了
    memset(get_merge_str,0,sizeof(get_merge_str));
	do//循环读目录表，直到读完
    {
    	if(__wildcard_match(current_fp->name,wildcard_param))
        {//把匹配成功的目录串成一个新字符串
        	strcat(get_merge_str,current_fp->name);
        	strcat(get_merge_str," ");
        }    
        //当fp下所有的文件都被删除了，就没文件，返回false
        current_fp = (struct file_rsc *)djyfs_folder_traversal_son(fp,current_fp);
        if(current_fp == NULL)
            break;
    }while(fat_block_no != current_fp->file_medium_tag);
	djyfs_fclose(fp);
    djyfs_find_close(current_fp);

	return get_merge_str;
}

//----提取含有通配符路径中含有通配符那级的字符串-------------------------------
//功能: 在含有通配符的路径中，把有含有通配符那一级的字符串提去出来
//参数: 
//返回: true=成功, 提取成功
//      false=失败，提取失败
//-----------------------------------------------------------------------------
bool_t del_wildcard_files(char *wildcard_param)
{
	char *next_param;
	char *del_param;
    char merge_str[255];
    struct file_rsc*fp;

	if(wildcard_param == NULL)
    {
		printf_ng("命令语法不正确!\r\n");
    	return false;
    }
    //debug 处理全路径的，还没处理
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

//----在字符串末尾无反斜杠加上反斜杠--------------------------------------------------------------
//功能：在字符串末尾无反斜杠加上反斜杠,有反斜杠就不要加了
//参数：str 字符串，任何形式
//返回：true 成功
//    	false 遇到".."了
//------------------------------------------------------------------------------
bool_t str_end_increase_backslash(char *str)
{
	uint16_t name_len;

	if(strcmp(str,"..") ==0)//如果是"..",那么不要操作
    	return false;
	name_len = strnlen(str,0);
    //如果字符串最后一位不是'\\'，那么加反斜杠
	if(str[name_len-1] != '\\')
    {
    	str[name_len] = '\\';
        //如果str不是规定好的空间，很容易泄露，必须防止
       	str[name_len+1] = '\0';    
    }
	return true;
}

//----在字符串末尾有反斜杠去掉反斜杠--------------------------------------------
//功能：在字符串末尾无反斜杠加上反斜杠，没有反斜杠就不处理
//参数：str 字符串，任何形式
//返回：true 成功
//    	false 字符串没有反斜杠
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

//----显示目录下的文件和文件夹------------------------------------------------
//功能: 扫描根目录下的文件和文件夹，然后按照一定个格式，全部打印出来
//参数: ancestor_fp，被显示的文件夹
//返回: true=成功, 列出成功
//      false=失败，列出失败
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
        return false;//可能是文件柜未建立

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
    printf_ng(" 的目录\r\n\r\n");

	current_fp = NULL;
    current_fp = (struct file_rsc*)djyfs_folder_traversal_son(parent_fp,current_fp);
    if(current_fp == NULL)
    {
        printf_ng("该目录为空!\r\n");
        return false;
    }

    fdt_block_no = current_fp->file_medium_tag;

    do //循环读目录表，直到读完
    {
        printf_ng("%d/%02d/%02d %02d:%02d",
                (current_fp->year_high_modify<<8) + current_fp->year_low_modify,
                current_fp->month_modify,
                current_fp->date_modify,
                current_fp->hour_modify,
                current_fp->minute_modify);

        if(current_fp->attr.bits.folder)
        {
            printf_ng(" <DIR> ");//代表是目录，可以继续dir
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

    printf_ng("%d个文件 %d字节\r\n", files_items, files_size);
    printf_ng("%d个目录 %d可用字节\r\n", folder_items, available_dbx_size);
    return true;
}

//----显示含有通配符的文件或者文件夹------------------------------------------------
//功能: dir命令下，显示含有通配符的文件或者文件夹。扫描当前工作路径或者指定路径下
//    	的文件或者目录。然后用匹配函数一一匹配。符合要求的，显示出来。
//参数:wildcard_param，含有通配符的参数
//返回: true=成功, 列出成功
//      false=失败，列出失败
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
        printf_ng("\r\n%s 的目录\r\n\r\n", work_path);
        __cut_path_end_item(work_path);
    }
    else
    {
        if(!get_work_path_name(work_path,cn_shell_cmd_limit))
            return false;
        printf_ng("\r\n%s 的目录\r\n\r\n", work_path);
     }
	if(!get_folder_wildcard_str(wildcard_param,s))
        return false;
	if(s[0]==0)
    {
    	printf_ng("找不到文件!\r\n");
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
        	printf_ng(" <DIR> ");//代表是目录，可以继续dir
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

    printf_ng("%d个文件 %d字节\r\n", files_items, files_size);
    printf_ng("%d个目录 %d可用字节\r\n", folder_items, available_dbx_size);

	return true;
}


//----判断一个目录是否是文件夹------------------------------------------------
//功能: 通过访问fp的属性，判断是否是文件夹
//参数: fullname,路径 格式:"C:\aa\bb\cc"
//返回: true=成功, 
//      false=失败，
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
    	return false;//文件路径错误，还怎么判断是否目录
}

//----删除文件夹下级的文件------------------------------------------------
//功能: 在一个文件夹的下一级，删除其所有的文件
//参数: workpath，当前的工作路径，不过区别work_path，因为work_patch是全局变量，
//    	尽量少修改。
//返回: true=成功, 
//      false=失败，
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
    	printf_ng("该目录为空!\r\n");
    	djyfs_fclose(fp);
    	return false;  //parent是一个空目录
    }        
	fat_block_no = current_fp->file_medium_tag;
    //这个可能增加的反斜杠为了strcat可以保证成功
	str_end_increase_backslash(work_path);
	do//循环读目录表，直到读完
    {    
    	if(!current_fp->attr.bits.folder)
        {
        	strcat(work_path,current_fp->name);
        	djyfs_remove(work_path);
        	__cut_path_end_item(work_path);
        }
      //当fp下所有的文件都被删除了，就没文件，返回false
    	current_fp = (struct file_rsc *)djyfs_folder_traversal_son(fp,current_fp);
        if(current_fp == NULL)
            break;
    }while(fat_block_no != current_fp->file_medium_tag);
    djyfs_find_close(current_fp);
	djyfs_fclose(fp);
	return true;
}
//----建立一个文件夹------------------------------------------------
//功能: 指定建立一个文件夹
//参数: md_param，输入字符
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t fs_mkdir(char *md_param)
{  
	struct file_rsc *fp;

	if(md_param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
    //在创建的末尾加了'\'，无论如何，都是目录
    //这里不怕内存泄漏，因为md函数里，规定了长度
	str_end_increase_backslash(md_param);
	if(!__check_fullname(md_param))
     	return false; 
    //如果有存在文件或者目录，那么就不需要创建了
    if(!djyfs_fsearch(md_param))
    {
        fp = djyfs_fopen(md_param,"w");
           if(fp == NULL)
            return false;
        djyfs_fclose(fp);
    }
    else
    {
        printf_ng("\r\n子目录或者文件%d已经存在\r\n", md_param);

        return false;
    }
    return true;    
}
//----删除文件夹--------------------------------------------------------------
//功能: 把指定的文件夹删除掉
//参数: rd_param，输入字符
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t fs_remove_folder(char *rd_param)
{    
	struct file_rsc *fp;

	if(rd_param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
	if(!__check_fullname(rd_param))
     	return false; 
	fp = djyfs_fopen(rd_param,"r");
	if(fp == NULL)
    {    
    	printf_ng("系统找不到指定的文件\r\n");
    	return false; 
    }
	if(fp->attr.bits.folder)
    {
    	djyfs_fclose(fp);
    	if(djyfs_remove(rd_param) == enum_fs_folder_unblank)
        {
            printf_ng("目录%s不是空！\r\n", rd_param);
        }
    	return true;
    }
	else
    {
    	djyfs_fclose(fp);
    	printf_ng("目录名称无效!\r\n");
    	return false;
    }
    
}
//----删除文件夹--------------------------------------------------------------
//功能: 把指定的文件夹删除掉
//参数: rd_param，输入字符
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t fs_remove_file(char *del_param)
{    
	if(del_param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
	if(!__check_fullname(del_param))
     	return false; 
	if(!djyfs_fsearch(del_param))
    {
    	printf_ng("找不到%s\r\n", del_param);
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
//----显示指定目录下的文件和子目录列表-------------------------------------------------------
//功能：显示指定目录下的文件和子目录列表	
//参数：dir_param，dir输入的参数，可以是文件，路径或者不输入
//返回：true，显示成功
//    	false,显示失败
//-----------------------------------------------------------------------------
bool_t dir_parent(char *next_param)
{
    char *foldername=NULL;
    struct file_rsc *fp;
    uint32_t lenth=0;
    char work_path[cn_shell_cmd_limit+1];

    //这里，必须使用get_work_path了。下面，dir ..\folder，需要                                                        ->private_tag);
    get_work_path_name(work_path,cn_shell_cmd_limit);
    do   
    {
        foldername = (char *)sh_get_item(next_param,&next_param);
    	if(foldername == NULL&&lenth==0)
        	return false;
        //执行 如: cd ..\foder
        if(strcmp(foldername,"..") != 0)
        {
            str_end_increase_backslash(work_path);
            strcat(work_path,foldername);
            fp = djyfs_fopen(work_path,"r");
            if(fp == NULL)
            {
                printf_ng("系统找不到指定的路径!\r\n");
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
        //路径深度大于cn_path_depth_limit，会跳出循环。在这里，
        //就为了防止死循环，多一个保护吧
        lenth++;
     }while((lenth!=cn_path_depth_limit)&&(foldername != NULL));    
    return true;
}

//----更改当前工作路径到父目录-------------------------------------------------------
//功能：更改当前工作路径到原来当前工作路径的父目录
//参数：next_param，输入的参数
//返回：true，显示成功
//    	false,显示失败
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
        //执行 如: cd ..\foder
        if(strcmp(foldername,"..") != 0)
        {
            //是目录才能打开，不用考虑根目录的情况。
            //假如"cd ..\foder",在根目录下执行，就没意义。
            //以只读方式打开，如果不存在，就返回NULL，
            //就不用去专门搜索该目录下这个目录是否存在。
        	if(judge_is_folder(foldername))
            {
            	fp = pg_work_path;
            	pg_work_path = djyfs_fopen(foldername,"r");
            	djyfs_fclose(fp);
            	return true;
            }
        	else
            {
                printf_ng("系统找不到指定路径\r\n");
                return false;
            } 
        }
    	if(pg_work_path!=DBX_device_tag->opened_root)
        {    
        	fp=pg_work_path;//当前工作路径不能关闭,故转移
            pg_work_path = (struct file_rsc*)pg_work_path->file_node.parent;
           // pg_work_path->open_counter++;
            djyfs_fclose(fp);
        }
        else
            break;
        //路径深度大于cn_path_depth_limit，会跳出循环。在这里，
        //就为了防止死循环，多一个保护吧
        lenth++;
     }while((lenth!=cn_path_depth_limit)&&(foldername != NULL));    
    return true;
}


//----显示指定目录下的文件和子目录列表-------------------------------------------------------
//功能：显示指定目录下的文件和子目录列表	
//参数：dir_param，dir输入的参数，可以是文件，路径或者不输入
//返回：true，显示成功
//    	false,显示失败
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
    	printf_ng("该文件柜不存在\r\n");
        return false;
    }
}

//复制次数太多，出的问题吧!可能是磨损平衡吧!总之，这算是一个大bug。

//----复制一个文件到目标------------------------------------------------
//功能: 把一个文件复制到需要被复制的地方
//参数:  src，当前的工作路径，不过区别work_path，因为work_patch是全局变量，
//    	尽量少修改。
//    	dest，这个工作路径是复制到目标的路径
//返回: true=成功, 
//      false=失败，
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
    ////申请16k内存,防止内存不足
    //申请的内存不能太小，否则文件大，操作次数多，影响copy的速度
    //太大，申请不了就完蛋了，暂时定为16k，大概一个flash块的大小
    //待频繁测试之后，再确定
	buf_size = 16384;
	loop_times =(file_size+buf_size-1)/buf_size;
    if(loop_times==0)
    {//loop_times==0时候，说明文件大小为0.只需要建立，就ok
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
    	else//写入的数据小于申请的内存，不应该写入buf_size的大小
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
//----移动一个目录下的文件到目标------------------------------------------------
//功能: 把一个目录的文件移动到需要被移动的地方
//参数: src，当前的工作路径，不过区别work_path，因为work_patch是全局变量，
//    	尽量少修改。
//    	dest，这个工作路径是复制到目标的路径
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t copy_files_of_folder_to_dest(char *src,char *dest)
{

	uint32_t file_nunmber=0,fat_block_no;
	struct file_rsc *dest_fp,*src_fp,*current_fp=NULL;
    if(!djyfs_fsearch(dest))
    {
    	printf_ng("djyos不支持此操作,详情请看help!\r\n");
    	return false;
    }    
	dest_fp = djyfs_fopen(dest,"r");
    if(dest_fp == NULL)
        return false;
    //djyfs不支持多个文件合并写入到一个文件里
	if(dest_fp->attr.bits.archive)
    {
    	printf_ng("djyos不支持此操作,详情请看help!\r\n");
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
    //必须循环一次就关闭，否则数据不准确，有些文件可能被删除
   	djyfs_fclose(src_fp);
    if(current_fp == NULL)
    {
        printf_ng("%s*\r\n", src);
        printf_ng("系统找不到指定的文件\r\n");
        printf_ng("已经复制了	    	0个文件\r\n");//暂时这样
        djyfs_find_close(current_fp);
        return false;  //parent是一个空目录
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
        //必须循环一次就关闭一次，否则数据不准确，有些文件可能被删除
       	djyfs_fclose(src_fp);
        current_fp = (struct file_rsc *)djyfs_folder_traversal_son(src_fp,current_fp);
        if(current_fp ==NULL)
            break;
    } while(fat_block_no!=current_fp->file_medium_tag);   
        djyfs_find_close(current_fp);       
    	printf_ng("已经复制了	    	%d个文件\r\n", file_nunmber);
    	return true;                   
}

//复制次数太多，出的问题吧!可能是磨损平衡吧!总之，这算是一个大bug。

//----移动一个文件到目标------------------------------------------------
//功能: 把一个文件移动到需要被移动的地方
//参数:  src，当前的工作路径，不过区别work_path，因为work_patch是全局变量，
//    	尽量少修改。
//    	dest，这个工作路径是复制到目标的路径
//返回: true=成功, 
//      false=失败，
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
    ////申请16k内存,防止内存不足
    //申请的内存不能太小，否则文件大，操作次数多，影响copy的速度
    //太大，申请不了就完蛋了，暂时定为16k，大概一个flash块的大小
    //待频繁测试之后，再确定
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
    	else//写入的数据小于申请的内存，不应该写入buf_size的大小
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

//----移动一个目录下的文件到目标------------------------------------------------
//功能: 把一个目录的文件移动到需要被移动的地方
//参数: src，当前的工作路径，不过区别work_path，因为work_patch是全局变量，
//    	尽量少修改。
//    	dest，这个工作路径是复制到目标的路径
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t move_files_of_folder_to_dest(char *src,char *dest)
{

	uint32_t file_nunmber=0,fat_block_no=0xffffffff;
	struct file_rsc *dest_fp,*src_fp,*current_fp=NULL;
    if(!djyfs_fsearch(dest))
    {
    	printf_ng("djyos不支持此操作,详情请看help!\r\n");
    	return false;
    }    
	dest_fp = djyfs_fopen(dest,"r");
    if(dest_fp == NULL)
        return false;
    //djyfs不支持多个文件合并写入到一个文件里
	if(dest_fp->attr.bits.archive)
    {
    	printf_ng("djyos不支持此操作,详情请看help!\r\n");
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
    //必须循环一次就关闭，否则数据不准确，有些文件可能被删除
   	djyfs_fclose(src_fp);
    if(current_fp == NULL)
    {
        printf_ng("%s*\r\n", src);
        printf_ng("系统找不到指定的文件\r\n");
        printf_ng("已经移动了	    	0个文件\r\n");//暂时这样
        djyfs_find_close(current_fp);
        return false;  //parent是一个空目录
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
            if(fat_block_no==0xffffffff)//第一个的话，变成了child
                fat_block_no = current_fp->file_medium_tag;
        }
        src_fp = djyfs_fopen(src,"r");
    	if(src_fp == NULL)
        {    
        	djyfs_fclose(dest_fp);
        	return false;
        }                             
        //必须循环一次就关闭一次，否则数据不准确，有些文件可能被删除
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
    	printf_ng("已经移动了	    	%d个文件\r\n", file_nunmber);
    	return true;                   
}

//----显示指定目录下的文件和子目录列表-----------------------------------------
//功能：显示指定目录下的文件和子目录列表	
//参数：dir_param，dir输入的参数，可以是文件
//返回：true，显示成功
//    	false,显示失败
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
    if(strcmp(dir_param,"\\")==0)//执行 dir [\]
    {
        if(!get_DBX_name(work_path,cn_shell_cmd_limit))
            return false;
        strcat(work_path,":\\");
        //打开根目录 如:"C:\\"
        fp = djyfs_fopen(work_path,"r");
        if(fp == NULL)
        {
            printf_ng("找不到文件\r\n");
            return false;
        }
        dir_item_son(fp);
        djyfs_fclose(fp);
        return true;
    }
    if(dir_param[0]=='.' && dir_param[1]=='.')//执行dir	[..]
    {    
        if(dir_parent(dir_param))
            return true;
        else
            return false;
    }
    if(!__check_fullname(dir_param))
    {
        printf_ng("路径格式不正确!\r\n");
            return false;
    }
    result = judge_is_folder(dir_param);
    if(result == false)
    {
        printf_ng("系统找不到指定的路径\r\n");
        return false; 
    }
    fp = djyfs_fopen(dir_param,"r");
    if(fp == NULL)
    {
        printf_ng("找不到文件\r\n");
        return false;
    }
    dir_item_son(fp);
    djyfs_fclose(fp);
	return true;
}
//----列出文件柜内的空间参数--------------------------------------------------
//功能：把文件柜的大小，有效空间，空闲大小都列出来
//参数：DBX_name 文件柜名
//返回：true 成功
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
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
	pg_fs_lhdl = dev_open_left("fs",0); 
	DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_name,0); //打开文件柜设备
	if(DBX_lhdl == NULL)
    {
        printf_ng("该文件柜无效!\r\n");
        return false;
    }
    DBX_device_tag = (struct st_DBX_device_tag*)
                                dev_get_my_tag(dev_get_panp(DBX_lhdl));
	DBX_device_tag->check_DBX(DBX_device_tag,&sum_size,&valid_size,&free_size);
    
	//当存储设备超过4G，就得使用64位的打印函数。
	printf_ng("文件柜%s大小：%dB\r\n", DBX_name, (uint32_t)sum_size);
	printf_ng("文件柜%s有效大小：%dB\r\n", DBX_name, (uint32_t)valid_size);
	printf_ng("文件柜%s空闲大小：%dB\r\n", DBX_name, (uint32_t)free_size);

	dev_close_left(DBX_lhdl); //千万要把设备关闭,否则第二次运行这个函数，调用不到设备
	dev_close_left(pg_fs_lhdl);
 	return true;
}


//----将至少一个文件复制到另外一个位置--------------------------------------------------
//功能：将至少一个文件复制到另外一个位置
//参数：DBX_name 文件柜名
//返回：true 成功
//-----------------------------------------------------------------------------
bool_t sh_copy(char *param)
{
    char *next_param,source[255],destination[255];
    struct file_rsc *fp,*second_fp;
    char s[255];      
	if(param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
    strcpy(source,sh_get_word(param,&next_param));

	if(NULL == next_param)
    {
    	printf_ng("djyfs不支持此操作，详情请看help\r\n");
    	return false;
    }    
    strcpy(destination,sh_get_word(next_param,&next_param));
    if((!__check_fullname(source))||(!__check_fullname(destination)))
        return false; 
    if(!djyfs_fsearch(source))
    {
        printf_ng("系统找不到指定的文件\r\n");
        return false; 
    }
    //如果操作的文件一样，返回，不操作，因为效果一样
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
            printf_ng("是否覆盖%s？ <yes/no>", destination);
            djyfs_fclose(second_fp); 
            while(1)
            {
                djy_scanf(NULL,s);
                if(s[0] == 'y'||s[0] == 'Y')
                {
                	putchar_ng(s[0]);
                    printf_ng("\r\n");
                    djyfs_remove(destination);//为覆盖做准备
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
            //是目录，需要增加反斜杠，以便下面拷贝路径使用   
            str_end_increase_backslash(destination);
            fp = djyfs_fopen(source,"r");
            if(fp == NULL)
            {
                djyfs_fclose(second_fp);
                return false;
            }
            if(fp->attr.bits.archive)
            {//如果source_file是文件，把文件名插入到second_work_path末尾
            //使用这个函数，还得提供准确的路径。
                str_end_wipe_backslash(source);
                __pick_filename_word(source,s);
                strcat(destination,s);      
             }
                djyfs_fclose(second_fp);
                djyfs_fclose(fp);
        }
    }
    else//如果destination不存在，那么全部按照文件处理
        str_end_wipe_backslash(source);//可以不用加反斜杠吧?
    fp = djyfs_fopen(source,"r");
    if(fp == NULL)
        return 0;
    if(!fp->attr.bits.folder)//source是一个文件
    {   
        djyfs_fclose(fp);
        copy_file_to_dest(source,destination);
        printf_ng("已经复制了      1个文件\r\n");
    }
    else//source是一个目录
    {
        djyfs_fclose(fp);
        str_end_increase_backslash(source);
        copy_files_of_folder_to_dest(source,destination);
    }
    return true;    
}


//----将至少一个文件移动到另外一个位置--------------------------------------------------
//功能：将至少一个文件移动到另外一个位置
//参数：DBX_name 文件柜名
//返回：true 成功
//-----------------------------------------------------------------------------
bool_t sh_move(char *param)
{
    char *next_param,source[255],destination[255];
    struct file_rsc *fp,*second_fp;
    char s[255];      
	if(param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
    strcpy(source,sh_get_word(param,&next_param));

	if(NULL == next_param)
    {
    	printf_ng("djyfs不支持此操作，详情请看help\r\n");
    	return false;
    }    
    strcpy(destination,sh_get_word(next_param,&next_param));
    if((!__check_fullname(source))||(!__check_fullname(destination)))
        return false; 
    if(!djyfs_fsearch(source))
    {
        printf_ng("系统找不到指定的文件\r\n");
        return false; 
    }
    //如果操作的文件一样，返回，不操作，因为效果一样
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
            printf_ng("是否覆盖%s？ <yes/no>", destination);
            djyfs_fclose(second_fp); 
            while(1)
            {
                djy_scanf(NULL,s);
                if(s[0] == 'y')
                {
                	putchar_ng(s[0]);
                    printf_ng("\r\n");
                    djyfs_remove(destination);//为覆盖做准备
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
            //是目录，需要增加反斜杠，以便下面拷贝路径使用   
            str_end_increase_backslash(destination);
            fp = djyfs_fopen(source,"r");
            if(fp == NULL)
            {
                djyfs_fclose(second_fp);
                return false;
            }
            if(fp->attr.bits.archive)
            {//如果source_file是文件，把文件名插入到second_work_path末尾
            //使用这个函数，还得提供准确的路径。
                str_end_wipe_backslash(source);
                __pick_filename_word(source,s);
                strcat(destination,s);      
             }
                djyfs_fclose(second_fp);
                djyfs_fclose(fp);
        }
    }
    else//如果destination不存在，那么全部按照文件处理
        str_end_wipe_backslash(source);
    fp = djyfs_fopen(source,"r");
    if(fp == NULL)
        return 0;
    if(!fp->attr.bits.folder)//source是一个文件
    {   
        djyfs_fclose(fp);
        move_file_to_dest(source,destination);
        printf_ng("已经移动了      1个文件\r\n");
    }
    else//source是一个目录
    {
        djyfs_fclose(fp);
        str_end_increase_backslash(source);
        move_files_of_folder_to_dest(source,destination);
    }
    return true;    
}

//----修改指定文件或者文件夹的名字------------------------------------------------
//功能: 这是命令ren，修改指定的文件或者文件夹
//参数: param，输入参数
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t sh_ren(char *param)
{
	char *next_param;
	char *fullname;//指定被修改的文件
	char *filename;//修改后的名字

	if(param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
    fullname = sh_get_word(param,&next_param);
    filename = sh_get_word(next_param,&next_param);
	if(!__check_fullname(fullname))
    	return false; 
	if(!djyfs_fsearch(fullname))
    {
    	printf_ng("系统找不到指定的文件\r\n");
     	return false; 
    }        
	djyfs_rename(fullname,filename);
	return true;

}    
//----创建一个或者多个目录------------------------------------------------
//功能: 这是命令md，创建一个或者多个目录
//参数: param，输入参数
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t sh_md(char *param)
{
	char *next_param;
	char *del_param;
    //限制255字符，因为fs_mkdir的参数，要进行增加字符串
    //如果不限制，会内存泄漏。造成不可控
	char str[cn_shell_cmd_limit+1];
    
	del_param = sh_get_word(param,&next_param);
	if(del_param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
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


//----删除一个或者多个文件夹--------------------------------------------------------------
//功能: 把指定的一个或者多个文件夹删除掉
//参数: rd_param，输入字符
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t sh_rd(char *param)
{
	char *next_param;
	char *del_param;
	char str[255];

	if(param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
	if(__check_wildcard(param))
    {//windows是不支持删除通配符.讨论，是否需要?
    //需要的时候，该函数会把文件也提取出来，处理一下这个问题，rd也
    //可以使用通配符
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

//----删除一个或者多个文件-----------------------------------------------------
//功能: 删除一个或者多个文件
//参数: param，输入字符
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t sh_del(char *param)
{
	char *next_param;
	char *del_param;
	char new_param[255];
    //char s[255];

    //参数不能为空字符
	if(param == NULL)
    {
    	printf_ng("命令语法不正确!\r\n");
    	return false;
    }
	if(__check_wildcard(param))
    {//new_param，shell命令行总长度不会大于255字符
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

//----显示当前目录的名称或改变当前目录-----------------------------------------
//功能: 显示当前目录的名称或改变当前目录
//参数: chdir_param，输入字符
//返回: true=成功, 
//      false=失败，
//-----------------------------------------------------------------------------
bool_t sh_cd(char *chdir_param)
{        
	bool_t result;
	struct file_rsc *fp;
	char work_path[cn_shell_cmd_limit+1];
    
    //执行cd/chdir[](空字符) 显示当前目录名称
	if(chdir_param == NULL)
    {
    	print_work_path();
    	printf_ng("\r\n");
    	return true;
    }
    //方便用户输入目录的时候不用输入反斜杠
    //如"cd folder\"变成"cd folder"
    //执行cd/chdir[\] 更换到根目录
	if(strcmp(chdir_param,"\\") ==0)
    {
        if(!get_DBX_name(work_path,cn_shell_cmd_limit))
            return false;
    	strcat(work_path,":\\");
    	fp=pg_work_path;
    	pg_work_path = djyfs_fopen(work_path,"r");
    	djyfs_fclose(fp);
        //如果打开失败，是否要处理?到时候再说
    	return true;
    }
    //执行cd/chdir[..] 更换到上一级目录
	else if(chdir_param[0]=='.' && chdir_param[1]=='.')//执行 "cd .."
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
        //执行 cd/chdir[文件柜：]路径 显示当前目录的名称或改变当前目录
    	if(result == true)
        {//是目录才能打开	        
        	if(judge_is_folder(chdir_param))
            {
            	fp = pg_work_path;
            	pg_work_path = djyfs_fopen(chdir_param,"r");
            	djyfs_fclose(fp);
            	return true;
            }
        	else if(!__judge_backslash_if_data(chdir_param))
            {//如C:\,在judge_is_folder里无法成功判断
            	fp = pg_work_path;
            	pg_work_path = djyfs_fopen(chdir_param,"r");
            	djyfs_fclose(fp);
            	return true;    
            }
        	else
            {
                printf_ng("系统找不到指定的路径\r\n");
                return false;
            }
        }
    	else
            	printf_ng("系统找不到指定的路径\r\n");
    }
 	return true;
}
//-------显示指定目录下的文件和子目录列表--------------------------------------
//功能: 显示指定目录下的文件和子目录列表
//参数: param，输入字符
//返回: true=成功, 
//      false=失败，
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
        printf_ng("尺寸太大\r\n");
        return true;
    }
    word = sh_get_word(next_param,&next_param);
    if(word == NULL)
    {
        printf_ng("文件柜名不能为空");
        return true;
    }
    
    if(DFFSD_create_DBX(chip,blocks,word) == true)
    {
        printf_ng("创建文件柜成功");
    }
    else
    {
        printf_ng("创建文件柜失败");
    }
    printf_ng("\r\n");
    return true;
}

bool_t sh_format(char *param)
{
    if(param == NULL)
    {
        printf_ng("文件柜名不能为空");
        return true;
    }
    
    if(djyfs_format(1,0,param) == 0)
    {
        printf_ng("格式化成功");
    }
    else
    {
        printf_ng("格式化失败");
    }
    printf_ng("\r\n");
    return true;
}
#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))

