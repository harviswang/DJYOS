//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:文件系统
//作者：lst
//版本：V1.0.0
//文件描述:文件系统与存储介质无关的部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))

#include <ctype.h>
#include <string.h>
#include "file.h"
#include "flashfile.h"

//ansi c standard 支持的文件输入输出操作

//— The wide-character input functions — those functions described in 7.24 that perform
//input into wide characters and wide strings: fgetwc, fgetws, getwc, getwchar,
//fwscanf, wscanf, vfwscanf, and vwscanf.

//— The wide-character output functions — those functions described in 7.24 that
//perform output from wide characters and wide strings: fputwc, fputws, putwc,
//putwchar, fwprintf, wprintf, vfwprintf, and vwprintf.

//— The wide-character input/output functions — the union of the ungetwc function,
//the wide-character input functions, and the wide-character output functions.

//— The byte input/output functions — those functions described in this subclause that
//perform input/output: fgetc, fgets, fprintf, fputc, fputs, djyfs_fread,
//fscanf, djyfs_fwrite, getc, getchar, gets, printf, putc, putchar, puts,
//scanf, ungetc, vfprintf, vfscanf, vprintf, and vscanf.

//Operations on files，文件操作函数
//int djyfs_remove(const char *filename);
//int djyfs_rename(const char *old, const char *new);
//FILE *tmpfile(void);
//char *tmpnam(char *s);
//
//File access functions，文件访问函数
//int djyfs_fclose(FILE *stream);
//int djyfs_fflush(FILE *stream);
//FILE *djyfs_fopen(const char * filename,const char * mode);
//FILE *freopen(const char * filename,const char * mode,FILE * restrict stream);
//void setbuf(FILE * restrict stream,char * restrict buf);
//int setvbuf(FILE * restrict stream,char * restrict buf,int mode, size_t size);
//
//Formatted input/output functions，格式化输入输出函数
//int fprintf(FILE * restrict stream,const char * restrict format, ...);
//int fscanf(FILE * restrict stream,const char * restrict format, ...);
//int printf(const char * restrict format, ...);
//int scanf(const char * restrict format, ...);
//int snprintf(char * restrict s, size_t n,const char * restrict format, ...);
//int sprintf(char * restrict s,const char * restrict format, ...);
//int sscanf(const char * restrict s,const char * restrict format, ...);
//int vfprintf(FILE * restrict stream,const char * restrict format,va_list arg);#include <stdarg.h>
//int vfscanf(FILE * restrict stream,const char * restrict format,va_list arg);
//int vprintf(const char * restrict format,va_list arg);
//int vscanf(const char * restrict format,va_list arg);
//int vsnprintf(char * restrict s, size_t n,const char * restrict format,va_list arg);
//int vsprintf(char * restrict s,const char * restrict format,va_list arg);
//int vsscanf(const char * restrict s,const char * restrict format,va_list arg);
//
//Character input/output functions，字符输入输出函数
//int fgetc(FILE *stream);
//char *fgets(char * restrict s, int n,FILE * restrict stream);
//int fputc(int c, FILE *stream);
//int fputs(const char * restrict s,FILE * restrict stream);
//int getc(FILE *stream);
//int getchar(void);
//char *gets(char *s);
//int putc(int c, FILE *stream);
//int putchar(int c);
//int puts(const char *s);
//int ungetc(int c, FILE *stream);
//
//Direct input/output functions，直接输入输出函数
//size_t djyfs_fread(void * restrict ptr,size_t size, size_t nmemb,FILE * restrict stream);
//size_t djyfs_fwrite(const void * restrict ptr,size_t size, size_t nmemb,FILE * restrict stream);
//
//File positioning functions，文件读写位置函数
//int fgetpos(FILE * restrict stream,fpos_t * restrict pos);
//int djyfs_fseek(FILE *stream, long int offset, int whence);
//int fsetpos(FILE *stream, const fpos_t *pos);
//long int ftell(FILE *stream);
//void rewind(FILE *stream);
//
//Error-handling functions，出错处理函数
//void clearerr(FILE *stream);
//int feof(FILE *stream);
//int ferror(FILE *stream);
//void perror(const char *s);

//文件(目录)变量命名规范:
//在"a:\aaa\bbb\ccc"中：
// 1."a"被命名为DBX_name
// 2."aaa"、"bbb"为spathname
// 3."ccc"被命名为filename
//4."aaa"、"bbb"、"ccc"统称为word
//5.整串或可能是整串时，命名为fullname
//6."a:\aaa\bbb\"被命名为fullpath
//7."\aaa\bbb\"被命名为abspath
//8."aaa\bbb\"称为relativepath
//9."\aaa\bbb\ccc"命名为synname
//10."aaa\bbb\ccc"命名为relativename(相对于当前路径)

static struct pan_device *pg_fs_dev;       //文件系统设备
static struct file_rsc tg_content_buf[cn_opened_fsnode_limit];//定义目录项内存池
static struct mem_cell_pool *pg_content_pool;              //内存池头指针。
//设定一个工作路径,相当于windows/dos的当前路径
struct file_rsc *pg_work_path = NULL;    //工作路径指针
static djy_handle_t pg_fs_lhdl;
static struct rsc_node tg_opened_file_root; //打开的文件资源的根节点。

ptu32_t module_init_djyfs(ptu32_t para)
{
    //建立文件系统设备，文件系统设备本身没有读写函数，所有对文件系统的读写操作
    //都是针对它的子设备--文件柜(俗称磁盘)进行的。
    //ctrl函数用于设定文件系统的工作路径、增加/删除文件柜等。
    pg_fs_dev = dev_add_device(     dev_get_root_device(),"fs",
                                    NULL,NULL,  //无信号量保护
                                    (dev_write_func) NULL_func ,
                                    (dev_read_func ) NULL_func,
                                    (dev_ctrl_func ) djyfs_fs_driver_ctrl ,
                                    (dev_write_func ) NULL_func ,
                                    (dev_read_func  ) NULL_func ,
                                    (dev_ctrl_func  ) djyfs_fs_app_ctrl,
                                    0           //不使用私有标签
                                    );          //"fs"是一个根设备
    if(pg_fs_dev == NULL)
        return 0;   //建立文件系统设备失败
    //在资源链表中建立一个根结点，所有打开的文件和目录都建立在此结点下。
    //初始化文件柜时，为每个立即添加的该文件柜的根目录结点为此结点的子结点。
    rsc_add_tree_root(&tg_opened_file_root,sizeof(struct  file_rsc),"opened files");
    //建立目录项内存池，每个打开的文件算一项，每个目录也算一项，如果是多级目录，
    //则每级单独计算，如果打开的多个文件的路径有重合，重合部分不单独占用目录项。
    pg_content_pool = mb_create_pool(tg_content_buf,
                                cn_opened_fsnode_limit,
                                sizeof(struct file_rsc),0,0,
                                "打开的目录项内存池");
    pg_fs_lhdl = dev_open_left("fs",0);  //打开文件系统设备左手接口
    return 1;
}

//----设置当前工作路径---------------------------------------------------
//功能: 用户可以根据自己的需求，设置当前工作路径。默认情况下，设置为第一个
//      文件柜第一个根目录。set_work_path，在cofig-prj.h里配置
//参数: 
//返回: 1，设置成功
//      0, 设置失败
//-------------------------------------------------------------------------
uint32_t djyfs_user_set_work_path()
{
    struct st_DBX_device_tag *DBX_device_tag;
	djy_handle_t DBX_lhdl;
    char DBX_name[256];
    
	if(strcmp(cfg_work_path,"")!=0)
    {
    	pg_work_path = djyfs_fopen(cfg_work_path,"w+");
    	if(pg_work_path == NULL)
        {
    		printf_ng("用户设置工作路径失败\r\n");
            return 0;
        }
    	else
        	return 1;
    }
    //指向第一个文件柜的根目录
    strcpy(DBX_name,tg_opened_file_root.child->name);
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_name,0); //打开文件柜设备
    if(DBX_lhdl == NULL)
        return 0;
	DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    pg_work_path = DBX_device_tag->opened_root;
    dev_close_left(DBX_lhdl);
	return 1;        
}

//----获取当前工作路径名---------------------------------
//功能: 获取当前工作目录名的全路径字符串
//参数: fullname，保存字符串的缓冲区
//      namebuf_len,名字缓冲区的长度
//返回: true，获取成功
//      false,获取失败
//-------------------------------------------------------------------------
bool_t get_work_path_name(char *fullname,u32 namebuf_len)
{
	struct rsc_node *parent,*opened_tree;
    uint32_t index=0,len;
    struct rsc_node *point_num[cn_path_depth_limit];

	if(pg_work_path == NULL)// 说明路径未初始化或者文件柜未建立
    {
		printf_ng("文件柜未建立\r\n");
    	return false;
    }    
    opened_tree = rsc_get_tree((struct rsc_node*)pg_work_path);
    parent = (struct rsc_node*)pg_work_path;
    point_num[index] = parent;    
    while(parent->parent != opened_tree)
    {
    	index++;
        parent = parent->parent;
        point_num[index] = parent;   
    }
    strcpy(fullname,point_num[index]->name);
	strcat(fullname,":\\");
    len = strlen(point_num[index]->name)+2;
    while(index!=0)
    {
    	index--;
        len += strlen(point_num[index]->name)+1;
        if(len >= namebuf_len)
            return false;       //路径长度超过缓冲区长度
        strcat(fullname,point_num[index]->name);
        strcat(fullname,"\\");  

    }
	return true; 

}

//----获取当前文件柜名(磁盘)---------------------------------------------------
//功能: 获取当前工作目录所在的文件柜名(对应windows和dos的磁盘)
//参数: DBX_name，保存字符串的缓冲区
//      namebuf_len,名字缓冲区的长度
//返回: true，获取成功
//      false,获取失败
//-------------------------------------------------------------------------
bool_t get_DBX_name(char *DBX_name,u32 namebuf_len)
{
	struct rsc_node *parent,*opened_tree;

	if(pg_work_path == NULL)// 说明路径未初始化或者文件柜未建立
    {
		printf_ng("文件柜未建立\r\n");
    	return false;
    }    
    opened_tree = rsc_get_tree((struct rsc_node*)pg_work_path);
    parent = (struct rsc_node*)pg_work_path;  
    while(parent->parent != opened_tree)
    {
        parent = parent->parent;  
    }
    if(strlen(parent->name) >= namebuf_len)
        return false;
    else
    {
        strcpy(DBX_name,parent->name);
    	return true; 
    }

}

//----打印当前目录-------------------------------------------------
//功能: 打印当前目录。
//参数: node，得到的资源节点
//返回: true，打印成功
//      false,打印失败
//-----------------------------------------------------------------------------
bool_t print_work_path(void)
{
	struct rsc_node *parent,*opened_tree;
    uint32_t index=0;
    struct rsc_node *point_num[cn_path_depth_limit];

	if(pg_work_path == NULL)// 说明路径未初始化或者文件柜未建立
    {
		printf_ng("文件柜未建立\r\n");
    	return false;
    }    
    opened_tree = rsc_get_tree((struct rsc_node*)pg_work_path);
    parent = (struct rsc_node*)pg_work_path;
    point_num[index] = parent;    
    while(parent->parent != opened_tree)
    {
    	index++;
        parent = parent->parent;
        point_num[index] = parent;   
    }
    printf_ng("%s:\\", point_num[index]->name);
    //全路径需要。比如"C:\\"
    //index-=para;//打印级数，打印工作路径上para级的全路径
    while(index!=0)
    {
    	index--;
    	printf_ng("%s\\", point_num[index]->name);

    }
	return true; 

}

//----查找文件----------------------------------------------------------------
//功能: 由文件系统设备的ctrl函数调用的一个函数，用于在文件系统中查找一个文件（
//      目录）是否存在。函数调用文件柜的lookfor_item函数完成任务，只返回查找
//      结果，并不返回被查找目标的信息。
//参数: DBX_lhdl，被操作的文件柜
//      synname,文件名或者目录名字符串，
//返回: 找到则返回true，找不到返回false
//-----------------------------------------------------------------------------
bool_t __djyfs_DBX_lookfor_item(struct pan_device *DBX_lhdl,char *synname)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *parent,*son;
    struct file_rsc temp_fp;
    uint16_t next_char_off;
    char    name[256];
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    if(__djyfs_if_abs_path(synname))  //synname中已经去掉了文件柜名，无需再判断
    {
        parent = DBX_device_tag->opened_root;  //从根目录开始操作
        next_char_off = 1;
    }else
    {
        parent = pg_work_path;  //从当前路径开始操作
        next_char_off = 0;
    }

    //djyfs_fclose已经判断文件柜存在了，所以遇到只有"\\"的路径，就说明根目录存在
   	if(strcmp(synname,"\\")==0)
    	return true;//返回的是根目录

    while(__pick_word(synname,next_char_off,name))
    {//沿路径逐级打开目录和文件
        
        //name是模块内部提供的字符串指针，已经经过字符串长度合法性检查
        next_char_off += strlen(name) +1;    //+1是为跳过字符'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                    != NULL)
        {//目标已经打开
            parent = son;   //以当前打开的目录为下次使用的父目录
        }else
        {//目标文件(目录)尚未打开
            //查找目标并初始化文件数据结构
            if(DBX_device_tag->lookfor_item(name,parent,&temp_fp))
            {//目标存在，并且已经初始化目录信息
                son = &temp_fp;   //以当前打开的目标为下次使用的父目录
                parent = son;
            }else       //目录不存在，返回false
            {
                return false;
            }
        }
        if(next_char_off >= strlen(synname))
            return true;
    }
    return true;
}

//----打开文件柜中的文件-------------------------------------------------
//功能: 由文件柜设备的ctrl函数调用的一个函数，打开该文件柜中的一个文件。函数首先
//      调用文件柜的lookfor_item确定目标文件是否存在，然后沿路径逐个打开目录，
//      为每一个目录分配 struct file_rsc 结构内存，并把他挂到资源链表上。最后打
//      开文件，也挂到资源链表上。
//参数: DBX_lhdl，被操作的文件柜
//      synname,文件名或者目录名字符串，文件和目录统称item
//      mode，文件打开模式
//返回: 打开的文件及打开的目录资源，错误则返回NULL
//-----------------------------------------------------------------------------
djyfs_file *__djyfs_DBX_open_file(struct pan_device *DBX_lhdl,
                                char *synname,char *mode)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *parent,*son;
    struct file_rsc *opened = NULL;   //保存最后一个已经打开的目录项
    uint32_t open_result;
    uint16_t next_char_off;
    char    name[256];
    union file_attrs attr;
    enum file_open_mode my_mode;
    bool_t need_to_creat = true;    //表示当文件不存在时，是否需要创建
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    if((strcmp(mode,"r")==0) || (strcmp(mode,"rb")==0))
    {
        need_to_creat = false;
        my_mode = enum_r_rb;
    }else if((strcmp(mode,"w")==0) || (strcmp(mode,"wb")==0))
        my_mode = enum_w_wb;
    else if((strcmp(mode,"a")==0) || (strcmp(mode,"ab")==0))
        my_mode = enum_a_ab;
    else if((strcmp(mode,"r+")==0)||(strcmp(mode,"rb+")==0)
                            ||(strcmp(mode,"r+b")==0))
    {
        need_to_creat = false;
        my_mode = enum_r_rb_plus;
    }else if((strcmp(mode,"w+")==0) || (strcmp(mode,"wb+")==0)
                            || (strcmp(mode,"w+b")==0))
        my_mode = enum_w_wb_plus;
    else if((strcmp(mode,"a+")==0) || (strcmp(mode,"ab+")==0)
                            || (strcmp(mode,"a+b")==0))
        my_mode = enum_a_ab_plus;
    else
        return NULL;
    if(__djyfs_if_abs_path(synname))  //synname中已经去掉了文件柜名，无需再判断
    {
        parent = DBX_device_tag->opened_root;  //从根目录开始操作
        next_char_off = 1;
    }else
    {
        parent = pg_work_path;  //从当前路径开始操作
        next_char_off = 0;
    }
    //在djyfs_fclose里，也必须弄清对根目录的操作
   	if(strcmp(synname,"\\")==0)
    	return DBX_device_tag->opened_root;//返回的是根目录
        
    while(__pick_path_word(synname,next_char_off,name))
    {//沿路径逐级打开目录，但不会打开文件
        //name是模块内部提供的字符串指针，已经经过字符串长度合法性检查
        next_char_off += strlen(name) +1;    //+1是为跳过字符'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                    != NULL)
        {//目标已经打开
            opened = son;
            if(son->open_counter != cn_limit_uint32)
                son->open_counter ++;
        }else
        {//目标文件(目录)尚未打开
            son = (struct file_rsc *)mb_malloc(pg_content_pool,0);
            if(son == NULL)
                goto exit_open_err;    //分配内存失败
            son->home_DBX = parent->home_DBX;
            //查找目录并初始化文件数据结构
            open_result = DBX_device_tag->open_item(name,parent,son,my_mode);
            if(open_result == cn_fs_open_success)
            {//目录存在，并且已经初始化目录信息
                //打开目录，实际上就是把目录结点挂到打开的文件资源树上。
                rsc_add_head_son(&parent->file_node,&son->file_node,
                                sizeof(struct file_rsc),son->name);
                if(DBX_device_tag->opened_sum != cn_limit_uint32)
                    DBX_device_tag->opened_sum ++;
                son->open_counter = 1;
                son->eno = enum_fs_no_error;
            }else if(open_result == cn_fs_item_exist)   //目录存在，但模式不匹配
            {
                goto exit_open_err;
            }else
            {//目录不存在，看是否需要创建。
                if(need_to_creat)
                {//需要创建
                    attr.all = 0;      //先把所有属性初始化为false
                    attr.bits.folder = 1;  //创建的是目录
                    if(DBX_device_tag->create_item(name,parent,attr) == false)
                    {//创建目录失败
                        mb_free(pg_content_pool,son);
                        goto exit_open_err;
                    }else
                    {
                        if(DBX_device_tag->open_item(name,parent,son,my_mode)
                                            == cn_fs_open_success)
                        {
                            //打开目录实际上就是把目录结点挂到打开的文件资源树。
                            rsc_add_head_son(&parent->file_node,
                                               &son->file_node,
                                               sizeof(struct file_rsc),
                                               son->name);
                            if(DBX_device_tag->opened_sum != cn_limit_uint32)
                                DBX_device_tag->opened_sum ++;
                            son->open_counter = 1;
                            son->eno = enum_fs_no_error;
                        }else
                        {
                            mb_free(pg_content_pool,son);
                            goto exit_open_err;
                        }
                    }
                }else
                {//不需要创建，当以r或r+方式打开文件时，不存在也不创建。
                    mb_free(pg_content_pool,son);
                    goto exit_open_err;
                }
            }
            //被打开的项目(目录)所属文件柜
            son->home_DBX = DBX_lhdl;
        }
        parent = son;   //以当前打开的目录为下次使用的父目录
    }
    //至此，目录已经全部打开(或创建)，下面打开文件
    if(__pick_filename_word(synname,name))  //synname串中包含文件名吗?
    {//打开文件
        son = (struct file_rsc *)rsc_search_son(&parent->file_node,name);
        if(son == NULL)
        {//文件尚未打开
            son = (struct file_rsc *)mb_malloc(pg_content_pool,0);
            if(son == NULL)
                goto exit_open_err;    //分配内存失败
            son->home_DBX = parent->home_DBX;
            //查找并打开文件
            open_result = DBX_device_tag->open_item(name,parent,son,my_mode);
            if(open_result == cn_fs_open_success)
            {//文件存在并且可以按my_mode模式打开，已经初始化文件信息
                rsc_add_head_son(&parent->file_node,&son->file_node,
                                sizeof(struct file_rsc),son->name);
                if(DBX_device_tag->opened_sum != cn_limit_uint32)
                    DBX_device_tag->opened_sum ++;
                son->open_counter = 1;
                son->eno = enum_fs_no_error;
            }else if(open_result == cn_fs_item_exist)   //文件存在，但模式不匹配
            {
                goto exit_open_err;
            }else
            {//文件不存在，看是否需要创建。
                if(need_to_creat)
                {//需要创建
                    attr.all = 0;
                    attr.bits.archive = 1;     //是文件
                    if(DBX_device_tag->create_item(name,parent,attr) == false)
                    {//创建文件失败
                        mb_free(pg_content_pool,son);
                        goto exit_open_err;
                    }else
                    {
                        if(DBX_device_tag->open_item(name,parent,son,my_mode)
                                    == cn_fs_open_success)
                        {
                            rsc_add_head_son(&parent->file_node,
                                               &son->file_node,
                                               sizeof(struct file_rsc),
                                               son->name);
                            son->open_counter = 1;
                            son->eno = enum_fs_no_error;
                            if(DBX_device_tag->opened_sum != cn_limit_uint32)
                                DBX_device_tag->opened_sum ++;
                        }else
                        {
                            mb_free(pg_content_pool,son);
                            goto exit_open_err;
                        }
                    }
                }else
                {//r或r+方式，不需要创建。
                    mb_free(pg_content_pool,son);
                    goto exit_open_err;
                }
            }
        }else
        {//文件已经打开
        //已经打开的，由于误操作，或者路径后面有没有反斜杠等等，在这里
        //再判断一下，有好处。
            if(son->attr.bits.folder)
            {
                if(son->open_counter != cn_limit_uint32)
                son->open_counter ++;
            }else   
            {
                if(my_mode == son->open_mode)
                {//已经打开的文件，只能以相同的方式再次打开。并且读写指针不改变
                    if(son->open_counter != cn_limit_uint32)
                        son->open_counter ++;
                    return son;
                }else
                    return NULL;
            }
        }
    }else   //打开目录，目录在if(index != 0)语句前已经打开，且无需计打开次数
        return son;//返回打开的目录
    son->home_DBX = DBX_lhdl;
    return son;
exit_open_err:
    //删除已经添加的资源节点和释放分配的内存,opened保存的是本次增加的
    //第一个资源节点的上一级节点。
    if(opened == NULL)
        return NULL;
    son = (struct file_rsc *)rsc_get_twig(&opened->file_node);
    while(son != NULL)
    {
        rsc_del_node(&son->file_node);
        mb_free(pg_content_pool,son);
        son = (struct file_rsc *)rsc_get_twig(&opened->file_node);
    }
    rsc_del_node(&opened->file_node);
    mb_free(pg_content_pool,opened);
    return NULL;
}
//----文件系统设备左手控制函数-------------------------------------------------
//功能：顾名思义，执行一系列上层发下来的文件系统控制命令
//参数：fs_lhdl，左手设备句柄指针
//      left_cmd，左手命令字，使用时转换成enum fs_left_cmd类型
//      data1、data2，跟命令字相关的数据
//返回：与命令字相关
//-----------------------------------------------------------------------------
ptu32_t djyfs_fs_app_ctrl(struct pan_device *fs_lhdl,uint32_t left_cmd,
                                ptu32_t data1,ptu32_t data2)
{
    switch(left_cmd)
    {
        case enum_fs_work_path:
        {
        }break; //for enum_fs_work_path
        default:break;
    }
    return 0;
}

//----文件系统设备右手控制函数-------------------------------------------------
//功能：顾名思义，执行一系列存储设备driver发出的文件系统控制命令
//参数：fs_rhdl，右手设备句柄指针
//      right_cmd，右手命令字，使用时转换成enum fs_right_cmd类型
//      data1、data2，跟命令字相关的数据
//返回：与命令字相关，参见源程序注释
//-----------------------------------------------------------------------------
ptu32_t djyfs_fs_driver_ctrl(struct pan_device *fs_rhdl,uint32_t right_cmd,
                                ptu32_t data1,ptu32_t data2)
{
    struct pan_device * DBX_device;
    struct file_rsc *root_folder;
    struct st_DBX_device_tag *DBX_device_tag;
    switch(right_cmd)
    {
        case enum_fs_add_DBX:
        {//成功添加返回enum_fs_no_error，否则返回enum_fs_creat_dbx_error
        //data1为文件柜专有数据结构指针
        //data2为信号量数据结构struct DBX_semp_para指针.虽然可以通过data1找到信
        //号量，但从模块独立角度，不能这样做。以下是从data1找到信号量的方法:
        //data1是 struct st_DBX_device_tag* 类型，指向文件柜专有数据结构，
        //通过data1的medium_tag成员可以找到具体芯片数据结构，从而找到该芯片
        //的信号量。但是，文件系统不应该访问medium_tag成员，故用data2传入。
            //提取文件柜设备专有数据结构
            DBX_device_tag = (struct st_DBX_device_tag*)data1;
            DBX_device = dev_add_device(pg_fs_dev,DBX_device_tag->name,
                                       ((struct DBX_semp_para *)data2)->right,
                                       ((struct DBX_semp_para *)data2)->left,
                                       (dev_write_func) NULL_func,
                                       (dev_read_func ) NULL_func,
                                       (dev_ctrl_func ) DBX_right_ctrl,
                                       (dev_write_func ) djyfs_DBX_app_write,
                                       (dev_read_func  ) djyfs_DBX_app_read,
                                       (dev_ctrl_func  ) DBX_left_ctrl,
                                   //文件柜设备私有标签指向文件柜结构专用数据结构
                                       (ptu32_t)DBX_device_tag
                                       );
            if(DBX_device == NULL)
                return enum_fs_creat_dbx_error;
            if(DBX_device_tag->formatted)   //这是已经格式化好的文件柜
            {
                //申请根目录资源结点内存
                root_folder = mb_malloc(pg_content_pool,0);
                if(root_folder == NULL)
                {   //申请不到内存，释放早先建立的设备
                    dev_delete_device(DBX_device);
                    return enum_fs_creat_dbx_error;
                }
                memset(root_folder,0,sizeof(struct file_rsc));
                //把新文件柜的根目录资源节点加入到文件根资源结点下，排在队列尾
                rsc_add_son(&tg_opened_file_root,&root_folder->file_node,
                                  sizeof(struct file_rsc),DBX_device_tag->name);
                root_folder->home_DBX = DBX_device;
                //文件柜设备的打开文件的根结点指向该根资源结点。
                DBX_device_tag->opened_root = root_folder;
            }else                           //这是未格式化的文件柜
                DBX_device_tag->opened_root = NULL;
            return enum_fs_no_error;
        }break; //for enum_fs_add_DBX
        case enum_fs_del_DBX:
        {
            //提取文件柜设备专有数据结构
            DBX_device_tag = (struct st_DBX_device_tag*)data1;
            DBX_device = DBX_device_tag->opened_root->home_DBX;
            if(rsc_del_node(&DBX_device_tag->opened_root->file_node))
            {
                if(dev_delete_device(DBX_device))
                    return enum_fs_no_error;
                else
                    return enum_fs_remove_error;
            }else
                return enum_fs_remove_error;
        }break; //for enum_fs_del_DBX
        default:break;
    }
    return enum_fs_no_error;
}

//----左手写文件柜------------------------------------------------------------
//功能: 从文件柜设备左手接口把数据写入文件,本函数由文件柜的左手写函数调用，
//      调用顺序:
//      用户调用fwrite
//      -->dev_write_left
//      -->DBX_device->left_hand.io_write
//      -->djyfs_DBX_app_write
//参数: DBX_lhdl,文件所属文件柜
//      buf，写入数据缓冲区指针
//      write_para，写入参数
//      file，被写的文件指针。
//----------------------------------------------------------------------------
uint32_t djyfs_DBX_app_write(struct pan_device *DBX_lhdl,ptu32_t buf,
                        ptu32_t write_para,ptu32_t file)
{
    struct rw_para *pl_write_para = (struct rw_para *)write_para;
    uint32_t write_len;  //需要写入的数据长度
    sint64_t rest_len;
    uint32_t result;
    struct file_rsc *fp = (struct file_rsc *)file;  //取得文件指针
    struct st_DBX_device_tag *DBX_device_tag = 
                        (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    write_len = pl_write_para->nmemb * pl_write_para->size;  //计算需写入的长度
    //查询物理设备空闲空间
    rest_len = DBX_device_tag->query_file_cubage(fp);
    if(rest_len >= write_len)
    {//物理设备上有足够的空间
        result=DBX_device_tag->write(fp,(uint8_t*)buf,write_len);
        result = result / pl_write_para->nmemb;
    }else
    {//物理设备上没有足够的空间，写入整数个完整记录
        write_len = (rest_len / pl_write_para->nmemb) * pl_write_para->nmemb;
        result=DBX_device_tag->write(fp,(uint8_t*)buf,write_len);
        result = result / pl_write_para->nmemb;
    }
    return result;
}
//----左手读文件柜------------------------------------------------------------
//功能: 从文件柜设备左手接口读出文件,本函数由文件柜的左手读函数调用，调用顺序:
//      用户调用fread
//      -->dev_read_left
//      -->DBX_device->left_hand.io_read
//      -->djyfs_DBX_app_read
//参数: DBX_lhdl,文件所属文件柜
//      buf，保存数据的缓冲区指针
//      read_para，读出参数
//      file，被读的文件指针。
//----------------------------------------------------------------------------
ptu32_t djyfs_DBX_app_read(struct pan_device *DBX_lhdl,ptu32_t buf,
                        ptu32_t read_para,ptu32_t file)
{
    struct rw_para *pl_read_para = (struct rw_para *)read_para;
    uint32_t read_len;  //需要读取的数据长度
    sint64_t stock_size;
    uint32_t result;
    
    struct file_rsc *fp = (struct file_rsc *)file;
    struct st_DBX_device_tag *DBX_device_tag;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    read_len = pl_read_para->nmemb * pl_read_para->size;
    //查询物理设备有多少数据可读，文件尺寸并不可靠，不能用
    stock_size = DBX_device_tag->query_file_stocks(fp);
    if(stock_size >= read_len)
    {//物理设备上有足够的数据
        //读取物理设备
        result = DBX_device_tag->read(fp,(uint8_t*)buf,read_len);
        result = result / pl_read_para->nmemb;
    }else
    {//物理设备上没有足够的数据，需要调整读取数量，以确保读到整数个单元
        read_len = (stock_size / pl_read_para->nmemb) * pl_read_para->nmemb;
        //需要从物理设备读取的数据字节数
        read_len = read_len * pl_read_para->size;
        //读取物理设备
        result = DBX_device_tag->read(fp,(uint8_t*)buf,read_len);
        result = result / pl_read_para->nmemb;
    }

    return result;
}
//----测试字符串是否包含文件柜名-----------------------------------------------
//功能: 测试给定的字符串是否包含文件柜名
//参数: fullname，待检查的字符串
//返回: 含文件柜名，则返回文件柜名的长度，否则返回0
//备注: 内部函数，调用前先确保fullname是合法的字符串。
//----------------------------------------------------------------------------
uint16_t __djyfs_if_contain_DBX_name(char *fullname)
{
    uint16_t offset;
    for(offset = 0; offset < cn_DBX_name_limit+1; offset++)
    {
        if(fullname[offset] == ':')
            return offset;
    	if(fullname[offset] == '\0')
        	return 0;
    }
        return 0;
}

//----测试目录串是否绝对路径--------------------------------------------------
//功能: 测试目录串是否绝对路径
//参数: fullname，待检查的字符串
//返回: true = fullname是绝对路径，false = 不是绝对路径
//备注: 内部函数，调用前先确保fullname是合法的字符串。
//----------------------------------------------------------------------------
bool_t __djyfs_if_abs_path(char *fullname)
{
    if(fullname[0] == '\\')
    {
        return true;
    }else
    {
        if(__djyfs_if_contain_DBX_name(fullname) != 0)
            return true;
        else
            return false;
    }
}

//----测试目录串是否包含文件名--------------------------------------------------
//功能: 测试目录串是否包含文件名
//参数: fullname，待检查的字符串
//返回: true = fullname包含文件名，false = 不包含文件名
//备注: 内部函数，调用前先确保fullname是合法的字符串。
//----------------------------------------------------------------------------
bool_t __djyfs_if_contain_filename(char *fullname)
{
    //fullname是已经经过字符串长度合法性检查的指针
    if(fullname[strlen(fullname)-1] != '\\')
        return true;
    else
        return false;
}

//----查看文件(目录)是否已经打开----------------------------------------------
//功能: 查看一个文件(目录)是否已经打开，即看它是否在被打开的文件资源中
//参数: synname，待查的文件(目录)名
//返回: true = 已经打开，false = 未打开(或其他错误)
//备注: 调用前保证fullname是合法的字符串
//----------------------------------------------------------------------------
bool_t __djyfs_item_if_opened(struct st_DBX_device_tag *DBX_device_tag,
                              char *synname)
{
    struct file_rsc *parent,*son;
    uint16_t offset,name_len,index=0;
    char    name[256];

    if(__djyfs_if_abs_path(synname))
    {
        parent = DBX_device_tag->opened_root;  //从根目录开始操作
        offset = 1;
    }else
    {
        parent = pg_work_path;  //从当前路径开始操作
        offset = 0;
    }
    //synname是已经经过字符串长度合法性检查的指针
    name_len = strlen(synname);
    for( ; offset< name_len; offset++)
    {//沿路径逐级查看目录是否已经打开
        if(synname[offset] == '\\')    //遇到字符'\'
        {
            name[index] = '\0';
            index = 0;
            if((son=(struct file_rsc*)rsc_search_son(&parent->file_node,name))
                                            != NULL)
            {//目标已经打开
                parent = son;   //以当前打开的目录为下次使用的父目录
            }else
            {
                return false;
            }
        }else   //未遇到字符'\'。
        {
            name[index] = synname[offset];     //复制当前字符到文件名串
            index++;
        }
        if(index >= cn_file_name_limit)    //目录名长度超限。
            return false;
    }
    //至此，如果filename传入的是文件名(末字符不是'\')，name保存的是文件名，
    //index是串长度，串结束符'\0'未赋值，整个路径都已经打开。
    //如果filename出入是目录名，则name保存最后一个目录，且已经打开，index=0。
    if(index != 0)
    {
        name[index] = '\0';
        if(rsc_search_son(&parent->file_node,name) != NULL)
        {
            return true;        //文件已经打开
        }else
        {
            return false;       //文件尚未打开
        }
    }else
        return true;
}

//----删除文件(目录)----------------------------------------------------------
//功能：删除一个文件（目录），本函数不能删除非空目录，以及只读文件（目录）。要
//      删除只读文件，只有先把它改为非只读文件才能进行。本函数由文件柜的左手
//      控制函数调用，调用顺序:
//      用户调用remove
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (用enum_DBX_remove命令)
//      -->__djyfs_DBX_remove_item
//参数: DBX_lhdl，被操作的文件柜
//      synname,文件名或者目录名字符串，文件和目录统称item
//返回：参考文件系统错误代码列表
//----------------------------------------------------------------------------
uint32_t __djyfs_DBX_remove_item(struct pan_device *DBX_lhdl,char *synname)
{
    struct file_rsc item;
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *parent,*son;
    uint16_t next_char_off;
    char    name[256];
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);

    if(__djyfs_item_if_opened(DBX_device_tag,synname) == true)
    {
    	return (uint32_t)enum_fs_object_opened;       //不能删除已经打开的对象
    }
    if(__djyfs_if_abs_path(synname))  //synname中已经去掉了文件柜名，无需再判断
    {
        parent = DBX_device_tag->opened_root;  //从根目录开始操作
        next_char_off = 1;
    }
	else
    {
        parent = pg_work_path;  //从当前路径开始操作
        next_char_off = 0;
    }
    //是根目录，无需删除
   	if(strcmp(synname,"\\")==0)
    	return enum_fs_dbx_error;
    while(__pick_word(synname,next_char_off,name))
    {//沿路径逐级打开目录，但不会打开文件
        //name是模块内部提供的字符串指针，已经经过字符串长度合法性检查
        next_char_off += strlen(name) +1;    //+1是为跳过字符'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                    != NULL)
        {//目标已经打开
            parent = son;   //以当前打开的目录为下次使用的父目录
        }
      else
        {//目标文件(目录)尚未打开
            //查找目标并初始化文件数据结构
            if(DBX_device_tag->lookfor_item(name,parent,&item))
            {//目标存在，并且已经初始化目录信息
                son = &item;   //以当前打开的目标为下次使用的父目录
                parent = son;
            }else       //目录不存在，返回false
            {
                return (uint32_t)enum_fs_object_nonentity;   //被删除的对象不存在
            }
        }
        if(next_char_off >= strlen(synname))
            break;
    }
    item.open_counter = 0;     //打开次数清零
    item.home_DBX = DBX_lhdl;
    if(item.attr.bits.read_only == true)
        return (uint32_t)enum_fs_object_readonly;       //不能删除只读对象
    if(item.attr.bits.folder)
    {//是个目录，需要判断是否空
        if(DBX_device_tag->check_folder(&item) != 0)
            return (uint32_t)enum_fs_folder_unblank;
    }
    if(DBX_device_tag->remove_item(&item))
        return enum_fs_no_error;
    else
        return enum_fs_remove_error;
}

//----目录(文件)改名-----------------------------------------------------------
//功能：把一个目录（文件）改名，不允许修改只读目录（文件）的名字，要修改只读文
//      件，必须先修改只读文件的属性，本函数由文件柜的左手控制函数调用，
//      调用顺序:
//      用户调用rename
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (用enum_DBX_rename命令)
//      -->__DBX_rename_item
//参数: DBX_lhdl，被操作的文件柜
//      old_synname,文件或目录原名字符串，
//      newname，文件或目录的新名字字符串
//返回：参考文件系统错误代码列表
//----------------------------------------------------------------------------
uint32_t __DBX_rename_item(struct pan_device *DBX_lhdl,
                            char *old_synname,char *newname)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc item;
	struct file_rsc *parent,*son;
    uint16_t next_char_off;
    char    name[256];
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);

    if(__djyfs_item_if_opened(DBX_device_tag,old_synname) == true)
    {
    	return (uint32_t)enum_fs_object_opened;       //不能删除已经打开的对象
    }
    if(__djyfs_if_abs_path(old_synname))  //synname中已经去掉了文件柜名，无需再判断
    {
        parent = DBX_device_tag->opened_root;  //从根目录开始操作
        next_char_off = 1;
    }else
    {
        parent = pg_work_path;  //从当前路径开始操作
        next_char_off = 0;
    }
    //是根目录，无需修改
   	if(strcmp(old_synname,"\\")==0)
    	return enum_fs_rename_error;
     while(__pick_word(old_synname,next_char_off,name))
    {//沿路径逐级打开目录，但不会打开文件
        //name是模块内部提供的字符串指针，已经经过字符串长度合法性检查
    //	printf_ng(name);
        next_char_off += strlen(name) +1;    //+1是为跳过字符'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                   != NULL)
        {//目标已经打开
           parent = son;   //以当前打开的目录为下次使用的父目录
        }else
       {//目标文件(目录)尚未打开
            //查找目标并初始化文件数据结构
            if(DBX_device_tag->lookfor_item(name,parent,&item))
            {//目标存在，并且已经初始化目录信息
                son = &item;   //以当前打开的目标为下次使用的父目录
                parent = son;
            }else       //目录不存在，返回false
            {
                return (uint32_t)enum_fs_object_nonentity;   //被删除的对象不存在
            }
        }
        if(next_char_off >= strlen(old_synname))
            break;
    }
    if(item.attr.bits.read_only == true)
    {
       	return enum_fs_object_readonly;   //只读文件不允许改名
     }

    if( DBX_device_tag->rename_item(son,newname))
        return enum_fs_no_error;
    else
        return enum_fs_rename_error;

}


//----关闭文件(目录)----------------------------------------------------------
//功能：关闭一个文件（目录），本函数由文件柜的左手控制函数调用，调用顺序:
//      用户调用fclose
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (用enum_DBX_close命令)
//      -->__DBX_close_item
//参数：DBX_lhdl，被关闭文件（目录）所属的文件柜
//      file，被关闭的文件（目录）指针
//返回：true=成功关闭则，false=失败
//----------------------------------------------------------------------------
bool_t __DBX_close_item(struct pan_device *DBX_lhdl,struct file_rsc *fp)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *parent,*son;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    if(fp->attr.bits.folder)
    {
        if(rsc_get_son(&fp->file_node) != NULL)
            return false;       //被关闭的目录仍有已经被打开的子项，不能关闭
    }
  
    //下面把文件从资源链表中删除，包括已经空的目录
    son = fp;
    parent = (struct file_rsc *)rsc_get_parent(&son->file_node);
    do
    {//从被删除结点开始，逐级向上删除空目录。空目录是指资源链表上的空目录结点，
     //而物理存储器上该目录可能不是空的
        if(son == pg_work_path)  //当前工作路径不能被删除
            break;

      //如果是根目录的话，是不能释放的。直接返回，不做处理
      	if(parent == DBX_device_tag->opened_root)
         {
        	DBX_device_tag->close_item(son);
        	if(rsc_del_node(&son->file_node) != NULL) //从打开文件资源链表中删除结点
            	mb_free(pg_content_pool,son);               //释放结点内存 
        	if(DBX_device_tag->opened_sum != 0)
            	DBX_device_tag->opened_sum --;   
        	break;        
          }
      //当被删除的文件，还有兄弟辈，那么不能释放父节点
    	if(rsc_get_next(&son->file_node) != &son->file_node)
        {
        	DBX_device_tag->close_item(son);
        	if(rsc_del_node(&son->file_node) != NULL) //从打开文件资源链表中删除结点
            	mb_free(pg_content_pool,son); 
        	if(DBX_device_tag->opened_sum != 0)
            	DBX_device_tag->opened_sum --;
        	break;
        }
    	DBX_device_tag->close_item(son);
        if(rsc_del_node(&son->file_node) != NULL) //从打开文件资源链表中删除结点
        	mb_free(pg_content_pool,son);               //释放结点内存

        if(son->open_counter>1)
        {//该目录多次被打开，故只需要打开次数减1，就达到关闭一次的目的
            son->open_counter--;
            break;
        }
        son = parent;               //下一循环释放父结点
        parent=(struct file_rsc*)rsc_get_parent(&parent->file_node);//父结点上移
        if(DBX_device_tag->opened_sum != 0)
            DBX_device_tag->opened_sum --;
    }while(son != DBX_device_tag->opened_root);    //直到根目录
    return true;
}

//----把文件写入到存储器------------------------------------------------------
//功能: 把文件写缓冲区的数据和buf中的数据一起写入到存储器中，并清空写缓冲区。
//      调用顺序:
//      用户调用fflush
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (用enum_DBX_flush命令)
//      -->__DBX_flush_file
//参数：DBX_lhdl，被操作文件所属的文件柜
//      buf，数据缓冲区指针
//      fp，被关闭的文件（目录）指针
//返回：写入存储介质的数据量
//----------------------------------------------------------------------------
uint32_t __DBX_flush_file(struct pan_device *DBX_lhdl,struct file_rsc *fp)
{
    struct st_DBX_device_tag *DBX_device_tag;
    uint32_t result;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    result = DBX_device_tag->flush(fp);
    return result;
}

//----flush文件柜的所有文件----------------------------------------------------
//功能: 把文件柜中所有文件的写缓冲区的数据写入到存储器中，并清空写缓冲区。
//      调用顺序:
//      用户调用fflush
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (用enum_DBX_flush_all命令)
//      -->__DBX_flush_all
//参数：DBX_lhdl，被操作文件所属的文件柜
//返回：0
//----------------------------------------------------------------------------
uint32_t __DBX_flush_all(struct pan_device *DBX_lhdl)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct  rsc_node  *current,*start,*temp;
    struct  file_rsc *item;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    start = &DBX_device_tag->opened_root->file_node;
    current = &DBX_device_tag->opened_root->file_node;
    while((temp = rsc_trave_scion(start,current)) != NULL)
    {//此循环遍历整个文件柜所有打开的文件和目录
        item = (struct file_rsc *)temp;
        if( ! item->attr.bits.folder)    //如果是文件，则刷到存储器
            __DBX_flush_file(DBX_lhdl,item);
        current = temp; //rsc_search_tree函数下次调用的起点
    }
    return 0;
}
//----文件柜设备左手控制函数-------------------------------------------------
//功能：顾名思义，执行一系列上层发下来的文件系统控制命令
//参数：DBX_lhdl，左手设备句柄指针
//      left_cmd，左手命令字，使用时转换成enum DBX_left_cmd类型
//      data1、data2，跟命令字相关的数据
//返回：与命令字相关，参见源程序注释
//-----------------------------------------------------------------------------
ptu32_t DBX_left_ctrl(struct pan_device *DBX_lhdl,uint32_t left_cmd,
                                uint32_t data1,uint32_t data2)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *root_folder;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    switch((enum DBX_left_cmd)left_cmd)
    {
        case enum_DBX_format:
        {//格式化文件柜，data1、data2是格式化参数
            if(DBX_device_tag->opened_root == NULL)
            {
                //申请根目录资源结点内存
                root_folder = mb_malloc(pg_content_pool,0);
                if(root_folder == NULL)
                {
                    DBX_device_tag->formatted = false;
                    return enum_fs_create_root_folder_error;
                }
                memset(root_folder,0,sizeof(struct file_rsc));
                //把新文件柜的根目录资源节点加入到文件根资源结点下，
                rsc_add_son(&tg_opened_file_root,&root_folder->file_node,
                                  sizeof(struct file_rsc),DBX_device_tag->name);
                root_folder->home_DBX = DBX_lhdl;
                //文件柜设备的打开文件的根结点指向该根资源结点。
                DBX_device_tag->opened_root = root_folder;
                    
            }        
            if(DBX_device_tag->format(data1,data2,DBX_device_tag))
            {

                DBX_device_tag->formatted = true;
                return enum_fs_no_error;
            }else
            {
                mb_free(pg_content_pool, DBX_device_tag->opened_root);
                DBX_device_tag->formatted = false;
                DBX_device_tag->opened_root = NULL;
                return enum_fs_format_DBX_error;
            }
        }break; //for enum_DBX_format
        case enum_chk_DBX_format:
        {//查找文件，data1是文件名(目录名)
            return DBX_device_tag->formatted;
        }break; //for enum_chk_DBX_format
        case enum_DBX_lookfor:
        {//查找文件，data1是文件名(目录名)
            return (ptu32_t)__djyfs_DBX_lookfor_item(DBX_lhdl,(char*)data1);
        }break; //for enum_DBX_lookfor
        case enum_DBX_open:
        {//打开文件，data1是文件名，data2是模式字符串，返回文件指针
            return (ptu32_t)__djyfs_DBX_open_file(DBX_lhdl,(char*)data1,(char*)data2);
        }break; //for enum_DBX_open
        case enum_DBX_remove:
        {//data1是文件（目录）名，data2不用
            return (ptu32_t)__djyfs_DBX_remove_item(DBX_lhdl,(char*)data1);
        }break;     //for enum_DBX_remove
        case enum_DBX_rename:
        {//data1是原文件（目录）名，data2是新文件名
            return (ptu32_t)__DBX_rename_item(DBX_lhdl,(char*)data1,(char*)data2);
        }break; //for enum_DBX_rename
        case enum_DBX_close:
        {
            return (ptu32_t)__DBX_close_item(DBX_lhdl,(struct file_rsc*)data1);
        }break; //for enum_DBX_close
        case enum_DBX_flush:
        {
            if(__DBX_flush_file(DBX_lhdl,(struct file_rsc *)data1)==0)
                return (ptu32_t)EOF;
            else
                return 0;
        } break; //for enum_DBX_flush
        case enum_DBX_seek:
        {
            return (ptu32_t)DBX_device_tag->seek_file((struct file_rsc *)data2,
                                                     (struct seek_para *)data1);
        } break; //for enum_DBX_seek
        default : break;
    }
    return 0;
}

//----文件柜设备右手控制函数-------------------------------------------------
//功能：顾名思义，执行一系列存储设备driver发出的文件系统控制命令
//参数：DBX_rhdl，右手设备句柄指针
//      right_cmd，右手命令字，使用时转换成enum DBX_right_cmd类型
//      data1、data2，跟命令字相关的数据
//返回：与命令字相关，参见源程序注释
//-----------------------------------------------------------------------------
ptu32_t DBX_right_ctrl(struct pan_device *DBX_rhdl,uint32_t right_cmd,
                                ptu32_t data1,ptu32_t data2)
{
    switch((enum DBX_right_cmd)right_cmd)
    {
        case enum_DBX_clr_readbuf:
        {//清除文件的读缓冲区，data1是文件指针
            if(((struct file_rsc*)data1)->p_read_buf != NULL)
               ring_flush(((struct file_rsc*)data1)->p_read_buf);
            return 0;
        }break; //for enum_DBX_clr_readbuf
        case enum_DBX_clr_writebuf:
        {//清除文件的写缓冲区，data1是文件指针
            if(((struct file_rsc*)data1)->p_write_buf != NULL)
               ring_flush(((struct file_rsc*)data1)->p_write_buf);
            return 0;
        }break; //for enum_DBX_clr_writebuf
        case enum_DBX_flush_one:
        {//
            return (ptu32_t)__DBX_flush_file(DBX_rhdl,(struct file_rsc *)data1);
        }break; //for enum_DBX_flush_one
        case enum_DBX_flush_all:
        {//
            return (ptu32_t)__DBX_flush_all(DBX_rhdl);
        }break; //for enum_DBX_flush_all
        case enum_DBX_update_medium_tag:
        {//修改(设置)一个文件的媒体标记，data1为文件指针，data2为新标记
            ;
        }break; //for enum_DBX_update_medium_tag
        case enum_DBX_search_medium_tag:
        {//搜索一个文件的媒体标记，data1为该文件的媒体标记，返回文件指针
            ;
        }break; //for enum_DBX_search_medium_tag
        default:break;
    }
    return 0;
}

//----打开文件柜左手接口------------------------------------------------------
//功能: 根据文件(目录)名，打开与之对应的文件柜左手接口。
//参数: fullname，文件(目录)名指针
//返回：设备句柄，出现下列情况返回NULL:
//      1.文件柜不存在  2.fullname和pg_work_path_name均没有指定文件柜名
//----------------------------------------------------------------------------
djy_handle_t __open_DBX_left(char *fullname)
{
    djy_handle_t DBX_lhdl;
    char DBX_name[256];
    uint16_t name_len;
    
    name_len = __djyfs_if_contain_DBX_name(fullname);
    if(name_len != 0)
    {//fullname是包括文件柜名在内的全路径(也可以是当前文件柜)
        memcpy(DBX_name,fullname,name_len); //从参数中copy文件柜名
        DBX_name[name_len] = '\0';          //写入串结束符
    }else
    {
        //fullname中没有指定文件柜名，应从当前工作路径中取
    	if(!get_DBX_name(DBX_name,cn_file_name_limit))
            return NULL;
    }
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_name,0); //打开文件柜设备
    return DBX_lhdl;
}

//----从目录中取一字-----------------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串curent_offset位置开始取出一个
//      由'\'分隔的word
//参数: synname，输入字符串，格式如"\aaa\bbb\ccc.txt"
//      start_offset，搜索起始位置指针
//      name，保存取得的字符串的缓冲区指针
//返回: true=成功
//      false=失败，1、已经完成最后一个目录字
//                  2、synname中根本就不包含目录字
//                  3、字长超过255字符
//      在*name中返回取得的字符串
//备注: synname应该是经过合法性检查的字符串，不会出现word超长的问题
//-----------------------------------------------------------------------------
bool_t __pick_word(char *synname,uint32_t curent_offset,char *name)
{
    uint32_t index = 0;
    //synname是已经经过字符串长度合法性检查的指针
    uint32_t name_len = strlen(synname);
    uint32_t offset = curent_offset;
    for( ; offset< name_len; offset++)
    {
        if(synname[offset] == '\\')    //遇到字符'\'
        {
            name[index] = '\0';
            return true;
        }else   //未遇到字符'\'。
        {
            if(index == cn_file_name_limit)
                return false;
            name[index] = synname[offset];     //复制当前字符到文件名串
            index++;
        }
    }

	name[index] = '\0';//文件(非目录)的截止符号。
    return true;   //若正确提取目录字，程序不会运行至此
}

//----取一目录名字-------------------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串中curent_offset位置开始取出一个
//      目录名
//参数: synname，输入字符串，格式如"\aaa\bbb\ccc.txt"
//      start_offset，搜索起始位置指针
//      name，保存取得的字符串的缓冲区指针
//返回: true=成功
//      false=失败，1、已经完成最后一个目录字
//                  2、synname中根本就不包含目录字
//                  3、字长超过255字符
//      在*name中返回取得的字符串
//备注: synname应该是经过合法性检查的字符串，不会出现word超长的问题
//-----------------------------------------------------------------------------
bool_t __pick_path_word(char *synname,uint32_t curent_offset,char *name)
{
    uint32_t index = 0;
    //synname是已经经过字符串长度合法性检查的指针
    uint32_t name_len = strlen(synname);
    uint32_t offset = curent_offset;
    for( ; offset< name_len; offset++)
    {
        if(synname[offset] == '\\')    //遇到字符'\'
        {
            name[index] = '\0';
            return true;
        }else   //未遇到字符'\'。
        {
            if(index == cn_file_name_limit)
                return false;
            name[index] = synname[offset];     //复制当前字符到文件名串
            index++;
        }
    }
    return false;   //若正确提取目录字，程序不会运行至此
}

//----取文件名串---------------------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串中取出文件名
//参数: synname，输入字符串，格式如"\aaa\bbb\ccc.txt"
//      start_offset，搜索起始位置指针
//      name，保存取得的字符串的缓冲区指针
//返回: true=成功
//      false=失败，synname中不包含文件名
//      在*name中返回取得的字符串
//备注: synname应该是经过合法性检查的字符串，不会出现word超长的问题
//-----------------------------------------------------------------------------
bool_t __pick_filename_word(char *synname,char *name)
{
    uint32_t index = 0;
    //synname是已经经过字符串长度合法性检查的指针
    uint32_t name_len = strlen(synname);
    uint32_t offset;
    for(offset = name_len; offset >0; offset--)
    {//从最后一个字符搜索字符'\'
        if(synname[offset-1] == '\\')    //遇到字符'\'
            break;
    }
    if(offset == name_len)      //最后一个字符就是'\'，串中没有文件名
        return false;
    else if((name_len -offset) > cn_file_name_limit)    //文件名超长
        return false;
    for( ; offset < name_len; offset++)
    {
        name[index] = synname[offset];
        index++;
    }
    name[index] = '\0';
    return true;
}

//----忽略路径中的文件柜部分---------------------------------------------------
//功能: 忽略文件(目录)名字符串中关于文件柜的部分，提取synname，比如，
//      输入"c:\aaa\bbb\ccc.txt"，将返回"\aaa\bbb\ccc.txt"
//参数: fullname,输入字符串
//返回: 取得的路径串
//----------------------------------------------------------------------------
char *__pick_synname(char *fullname)
{
    uint16_t name_len;

    name_len = __djyfs_if_contain_DBX_name(fullname);
	if(name_len != 0)//绝对路径
       	return fullname + name_len +1;
	else
    	return fullname;//相对路径，返回原来的字符串
}


//----检查路径名是否合法------------------------------------------------------
//功能: 检查路径名是否合法，与windows兼容，暂不考虑网络路径名，方法如下:
//      1.由字符':'分隔的文件柜名长度必须合法，':'后面必须是字符'\'
//      2.不允许出现连续两个'\'。
//      3.不允许出现'* ? / | < > "'这7个字符,这些字符可能用于特殊用途
//      4.不允许出现长度超过255字符的object
//参数: fullname,被测字符串
//返回: true=合法，false=非法
//----------------------------------------------------------------------------
bool_t __check_fullname(char *fullname)
{
    uint32_t colon_offset = cn_limit_uint32,syn_offset;
    uint32_t loop,temp;
    ufast_t depth = 0;
    if(fullname == NULL)
        return false;
    //无需使用strlen检查fullname长度，因为接下来的所有检查项目都在限定长度内
    //操作。
    for(loop = 0; loop <= cn_DBX_name_limit; loop++)
    {//检查串中第一个':'出现在什么位置
        if((fullname[loop]=='*')||(fullname[loop]=='/')||(fullname[loop]=='?')
           ||(fullname[loop]=='<')||(fullname[loop]=='>')||(fullname[loop]=='|')
           ||(fullname[loop]=='"'))
            return false;   //名称串中不能出现这几个字符的
        if(fullname[loop] == ':')
        {
            colon_offset = loop; //第一个':'出现的位置，用于检查文件柜名长度
            break;
        }

        if(fullname[loop] == '\\')   //出现word分隔符。
            break;
    	if(fullname[loop] == '\0')    //出现停止符号
        	break;
    }
    if(colon_offset == 0)
        return false;       //首字符不能是':'
    if(colon_offset != cn_limit_uint32) //':'位置合法(含文件柜名且长度合法)
    {
        if(fullname[colon_offset+1] != '\\')
            return false;       //冒号后面必须是字符'\'
        syn_offset = colon_offset + 2;
    }else if(fullname[0] =='\\')
        syn_offset = 1;
    else
        syn_offset = 0;
    temp = 0;
    //检查有没有超过cn_file_name_limit字符的object，并检查有没有非法字符
    //不算文件柜名，全路径所包含的字符个数不会超过
    //cn_path_depth_limit * (cn_file_name_limit+1),+1是包含word分隔符'\'
    for(loop = syn_offset;
               (loop < cn_path_depth_limit*(cn_file_name_limit+1)+syn_offset);
               loop++)
    {
        //从文件柜名后面开始继续检查非法字符，此时连冒号':'也是非法字符
        if((fullname[loop]=='*')||(fullname[loop]=='/')||(fullname[loop]=='?')
           ||(fullname[loop]=='<')||(fullname[loop]=='>')||(fullname[loop]=='|')
           ||(fullname[loop]=='"')||(fullname[loop]==':'))
            return false;   //含有这几个非法字符，
        if(fullname[loop] == '\0')
            return true;
        if(fullname[loop] == '\\')
        {
            if(temp == 0)
                return false;       //说明遇到连续的word分隔符'\'
            temp = 0;//文件的长度清零，重新开始计算
            depth++;
        }else
        {
            if(depth >= cn_path_depth_limit)    //路径过深
                return false;
            temp++;
            if(temp > cn_file_name_limit)       //word长度超限
                return false;
        }
    }
    return false;       //能跑到这里，肯定没有找到串结束符
}

//----检查单项名是否合法------------------------------------------------------
//功能: 检查一个单项的名字是否合法，即不能含'\/?:*'这几个字符
//参数: word,被测字符串，可能是单一路径名，也可能是文件名
//返回: true=合法，false=非法
//----------------------------------------------------------------------------
bool_t __check_word(char *word)
{
    uint32_t loop;
    //word是已经经过字符串长度合法性检查的指针
    for(loop = 0; loop < strlen(word); loop++)
    {
        if((word[loop]=='*')||(word[loop]=='/')
                        ||(word[loop]=='?')||(word[loop] == ':'))
            return false;   //含有这几个非法字符，
    }
    return true;
}

//----获取一个目录下的子目录(文件)-------------------------------------
//功能: 这是文件系统提供的一个API调用，用于获取文件（目录）是信息
//参数: fullname,文件（目录）名
//返回: 找到则返回true，找不到返回false
//备注: 这个考虑是否要搞成windows类似的FindFirstFile等三种API。到时候增加
//      api的时候再说。
//---------------------------------------------------------------------
djyfs_file *djyfs_folder_traversal_son(struct file_rsc *ancestor_file,
                                        	struct file_rsc *current_file)
{
    struct file_rsc * result;
    struct st_DBX_device_tag *DBX_device_tag;
    
    if(ancestor_file == NULL)
        return NULL;
    DBX_device_tag = (struct st_DBX_device_tag *)(ancestor_file->home_DBX
                                                            ->private_tag);
    if(DBX_device_tag->formatted)
    {
        //调用文件柜的左手控制函数
        result = DBX_device_tag->item_traversal_son(ancestor_file,current_file);
    }else
        result = NULL;
    return result;
}
void djyfs_find_close(struct file_rsc *current_file)
{
    if(current_file == NULL)
        return;
    
    m_free(current_file);
}

//----打开文件----------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于打开文件系统中的一个文件
//参数: fullname,文件名
//      mode，文件打开模式
//返回: 成功打开则返回文件指针，否则返回NULL。
//-----------------------------------------------------------------------------
djyfs_file *djyfs_fopen(char *fullname, char *mode)
{
    struct file_rsc *result;
    djy_handle_t DBX_lhdl;
    struct st_DBX_device_tag *DBX_device_tag;
    char *synname;
    //fullname不是一个合法的字符串
    if((__check_fullname(fullname) == false)||(mode ==  NULL))     
        return NULL;
    DBX_lhdl = __open_DBX_left(fullname);
    if(DBX_lhdl == NULL)
        return NULL;    //文件柜不存在，或fullname和当前路径均没有指定文件柜名
    DBX_device_tag = (struct st_DBX_device_tag*)
                                dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(fullname);
        //调用文件柜的左手控制函数
        result = (struct file_rsc *)dev_ctrl_left(DBX_lhdl,enum_DBX_open,
                          (ptu32_t)synname,(ptu32_t)mode);
    }else
        result = NULL;
    dev_close_left(DBX_lhdl);
    return result;
}

//----查找文件----------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于检查文件（目录）是否存在
//参数: fullname,文件（目录）名
//返回: 找到则返回true，找不到返回false
//-----------------------------------------------------------------------------
bool_t djyfs_fsearch(char *fullname)
{
    bool_t result;
    djy_handle_t DBX_lhdl;
    struct st_DBX_device_tag *DBX_device_tag;
    char *synname;

    if(fullname == NULL)
        return false;
    if(!__check_fullname(fullname))
        return false;
    DBX_lhdl = __open_DBX_left(fullname);
    if(DBX_lhdl == NULL)
        return false;    //文件柜不存在，或fullname和当前路径均没有指定文件柜名
    DBX_device_tag = (struct st_DBX_device_tag*)
                                dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(fullname);
       //调用文件柜的左手控制函数
       result = (bool_t)dev_ctrl_left(DBX_lhdl,enum_DBX_lookfor,
                                        (ptu32_t)synname,0);
    }else
        result = false;
    dev_close_left(DBX_lhdl);
    return result;
}

//----删除文件----------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于删除一个文件（目录）
//参数: fullname,文件（目录）名
//返回: 0=成功，1=未格式化，非0和1=各种原因删除失败，详情看文件系统出错代码表
//-----------------------------------------------------------------------------
uint32_t djyfs_remove(char *fullname)
{
    uint32_t result;
    djy_handle_t DBX_lhdl;
    struct st_DBX_device_tag *DBX_device_tag;
    char *synname;

    if(fullname == NULL)
        return 1;
    if(!__check_fullname(fullname))
        return 1;
    DBX_lhdl = __open_DBX_left(fullname);
    if(DBX_lhdl == NULL)
        return 1;    //文件柜不存在，或fullname和当前路径均没有指定文件柜名
    DBX_device_tag = (struct st_DBX_device_tag*)
                                    dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(fullname);
    	result = dev_ctrl_left(DBX_lhdl,enum_DBX_remove,(ptu32_t)synname,0);
        //调用文件柜的左手控制函数
        if(result == enum_fs_no_error)
            result = 0;
    }else
        result = 1;
    dev_close_left(DBX_lhdl);
    return result;
}

//----修改文件(目录)名字-------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于修改一个文件（目录）的名字
//参数: old_fullname,文件（目录）旧名
//      new_filename，新名字
//返回: 0=成功，非0 = 失败
//-----------------------------------------------------------------------------
uint32_t djyfs_rename(char *old_fullname,char *new_filename)
{
    uint32_t result;
    djy_handle_t DBX_lhdl;
    struct st_DBX_device_tag *DBX_device_tag;
    char *synname;

    if(old_fullname == NULL)
        return 1;
    if(__check_word(new_filename) == false)
        return 1;
    if(!__check_fullname(old_fullname))
        return 1;
    DBX_lhdl = __open_DBX_left(old_fullname);
    if(DBX_lhdl == NULL)
        return false;    //文件柜不存在，或fullname和当前路径均没有指定文件柜名
    DBX_device_tag = (struct st_DBX_device_tag*)
                                    dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(old_fullname);
        //调用文件柜的左手控制函数
        if(dev_ctrl_left(DBX_lhdl,enum_DBX_rename,(ptu32_t)synname,
                             (ptu32_t)new_filename) == enum_fs_no_error)
            result = 0;
        else
            result = 1;
    }else
        result = 1;
    dev_close_left(DBX_lhdl);
    return result;
}

//----关闭文件-----------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于关闭打来的文件
//参数: fp，被关闭的文件
//返回: 0=成功，cn_limit_uint32=失败。
//-----------------------------------------------------------------------------
uint32_t djyfs_fclose(djyfs_file *fp)
{
    bool_t result;
    struct pan_device *DBX_pan_device;
    djy_handle_t DBX_lhdl;
    struct st_DBX_device_tag*DBX_device_tag;
    if(fp == NULL)
        return cn_limit_uint32;
    DBX_pan_device = fp->home_DBX;
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_pan_device->node.name,0);
    if(DBX_lhdl == NULL)
        return cn_limit_uint32;
     DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
     //根目录和当前工作路径，无需关闭
    if(fp == DBX_device_tag->opened_root || fp == pg_work_path)
    {
        dev_close_left(DBX_lhdl);
        return true; 
    }

    if(fp->open_counter>1)
    {//该文件多次被打开，故只需要打开次数减1，就达到关闭一次的目的
        fp->open_counter--;
    	dev_close_left(DBX_lhdl);
        return 0;
    }
    result = (bool_t)dev_ctrl_left(DBX_lhdl,enum_DBX_close,(ptu32_t)fp,0);
    dev_close_left(DBX_lhdl);
    if(result)
    {
        //防止多次关闭
    	fp = NULL;
        return 0;
    }
    else
        return cn_limit_uint32;
}

//----读文件-----------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于从打来的文件读数据
//参数: buf，保存读出的数据的缓冲区
//      size，读出的尺寸，nmemb块数
//      nmemb，每块数据长度(字节数)
//      fp，被读的文件
//返回: 实际读出的数据量，nmemb块数
//-----------------------------------------------------------------------------
size_t djyfs_fread(void *buf,size_t size, size_t nmemb,djyfs_file *fp)
{
    size_t result;
    struct pan_device *DBX_pan_device;
    djy_handle_t DBX_lhdl;
    struct rw_para  param;
    if(fp == NULL)
        return false;
    if((size == 0) || (nmemb == 0))
        return 0;   //读取的数据量是0
    DBX_pan_device = fp->home_DBX;
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_pan_device->node.name,0);
    if(DBX_lhdl == NULL)
        return false;
    if(fp->attr.bits.folder)
    { //目录不能读
        result = 0;
        goto goto_end;
    }
    if((fp->open_mode == enum_w_wb)||(fp->open_mode == enum_a_ab))
    { //只写模式，不能读
        result = 0;
        goto goto_end;
    }
    param.nmemb = nmemb;
    param.size = size;
    result = dev_read_left(DBX_lhdl,(ptu32_t)buf,(ptu32_t)&param,(ptu32_t)fp);

goto_end:
    dev_close_left(DBX_lhdl);
    return result;
}

//----写文件-------------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于把数据写入打来的文件
//参数: buf，提供写入的数据的缓冲区
//      size，写入的尺寸，nmemb块数
//      nmemb，每块数据长度(字节数)
//      fp，被写的文件
//返回: 实际写入的数据量，nmemb块数
//-----------------------------------------------------------------------------
size_t djyfs_fwrite(void *buf,size_t size, size_t nmemb,djyfs_file *fp)
{
    size_t result;
    struct pan_device *DBX_pan_device;
    djy_handle_t DBX_lhdl;
    struct rw_para  param;
    if(fp == NULL)
        return 0;
    if((size == 0) || (nmemb == 0))
        return 0;   //写入的数据量是0
    DBX_pan_device = fp->home_DBX;
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_pan_device->node.name,0);
    if(DBX_lhdl == NULL)
        return false;
    if(fp->attr.bits.folder)
    { //目录不能写
        result = 0;
        goto goto_end;
    }
    if(fp->open_mode == enum_r_rb)
    {//只读文件，不能写
        result = 0;
        goto goto_end;
    }
    param.nmemb = nmemb;
    param.size = size;
    result = dev_write_left(DBX_lhdl,(ptu32_t)buf,
                                (ptu32_t)&param,(ptu32_t)fp);
goto_end:
    dev_close_left(DBX_lhdl);
    return result;
}
//----刷新文件-----------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于从打来的文件读数据
//参数: buf，保存读出的数据的缓冲区
//      size，读出的尺寸，nmemb块数
//      nmemb，每块数据长度(字节数)
//      fp，被关闭的文件
//返回: 0=成功，指定的流没有缓冲区或者只读打开时也返回0值。返回EOF指出一个错误
//-----------------------------------------------------------------------------
uint32_t djyfs_fflush(djyfs_file *fp)
{
    struct pan_device *DBX_pan_device;
    djy_handle_t DBX_lhdl;
    size_t result;
    if(fp == NULL)
        return EOF;
    DBX_pan_device = fp->home_DBX;
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_pan_device->node.name,0);
    if(DBX_lhdl == NULL)
        return EOF;
    if(fp->attr.bits.folder)
    {   //目录不能写
        result = EOF;
        goto goto_end;
    }
    if(fp->open_mode == enum_r_rb)
    {//只读文件，无需刷新
        result = EOF;
        goto goto_end;
    }
    result = dev_ctrl_left(DBX_lhdl,enum_DBX_flush,
                                (ptu32_t)fp,0);
goto_end:
    dev_close_left(DBX_lhdl);
    return result;
}

//----设置文件指针-------------------------------------------------------------
//功能: 这是文件系统提供的一个API调用，用于从打来的文件读数据
//参数: buf，保存读出的数据的缓冲区
//      size，读出的尺寸，nmemb块数
//      nmemb，每块数据长度(字节数)
//      fp，被关闭的文件
//返回: 0=成功，非0=失败
//-----------------------------------------------------------------------------
uint32_t djyfs_fseek(djyfs_file *fp, sint64_t offset, int whence)
{
    struct pan_device *DBX_pan_device;
    djy_handle_t DBX_lhdl;
    uint32_t result;
    struct seek_para new_pos;
    if(fp == NULL)
        return cn_limit_uint32;
    DBX_pan_device = fp->home_DBX;
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_pan_device->node.name,0);
    if(DBX_lhdl == NULL)
        return cn_limit_uint32;
    if(fp->attr.bits.folder)
    {//目录没有位置指针
        result = cn_limit_uint32;
        goto goto_end;
    }
    new_pos.offset = offset;
    new_pos.whence = whence;
    result = dev_ctrl_left(DBX_lhdl,enum_DBX_seek,
                                (ptu32_t)&new_pos,(ptu32_t)fp);
goto_end:
    dev_close_left(DBX_lhdl);
    return result;
}

//----格式化文件柜-------------------------------------------------------------
//功能: 格式化文件柜
//参数: format_para1，格式化参数
//      format_para1，格式化参数
//      dbx_name，被格式化的文件柜名字
//返回: 0=成功，非0=失败
//-----------------------------------------------------------------------------
bool_t djyfs_format(uint32_t format_para1,uint32_t format_para2,char *dbx_name)
{
    uint32_t name_len,loop;
    bool_t result;
    djy_handle_t DBX_lhdl;
    struct st_DBX_device_tag *DBX_device_tag;
    if(dbx_name == NULL)
        return false;
    name_len = strnlen(dbx_name,cn_DBX_name_limit);
    if((name_len == 0) || (name_len == cn_limit_uint32))
        return false;
    for(loop = 0;loop < name_len; loop++)
    {
        if((dbx_name[loop]=='*')||(dbx_name[loop]=='/')||(dbx_name[loop]=='?')
           ||(dbx_name[loop]=='<')||(dbx_name[loop]=='>')||(dbx_name[loop]=='|')
           ||(dbx_name[loop]=='"')||(dbx_name[loop]==':'))
            return false;   //名称串中不能出现这几个字符的
    }
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,dbx_name,0);
    if(DBX_lhdl == NULL)
        return false;
    DBX_device_tag = (struct st_DBX_device_tag*)
                                    dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->opened_sum != 0)
        result = false;
    else
        result = (bool_t)dev_ctrl_left(DBX_lhdl,enum_DBX_format,
                                       format_para1,format_para2);
    dev_close_left(DBX_lhdl);
    return result;
}

#if 0
//----定时flush-------------------------------------------------------------
//功能: 每隔5秒，把所有已经打开的文件写入到fluash
//参数: my_event，键盘服务事件类型。
//返回: 无
//----------------------------------------------------------------------------
void flush_all_timer(struct event_script *my_event)
{
 	djy_handle_t DBX_lhdl;
    while(1)
    {   //每隔5秒，flush一次。
        djy_timer_sync(5000);
        //该函数，如果没发现绝对路径，会自动调用当前工作路径，
        //打开当前工作路径的DBX_lhdl
    	DBX_lhdl = __open_DBX_left("");
        if(DBX_lhdl == NULL)
            return;//竟然文件柜未建立，那么这个函数就没用，退出
        __DBX_flush_all(DBX_lhdl);
    	dev_close_left(DBX_lhdl);
    }
}

void flush_all_init(void)
{
 	uint16_t flush_evtt;
                
	flush_evtt = djy_evtt_regist(true,true,cn_prio_RRS,1,
                                        flush_all_timer,800,NULL);

//debug 不知道用false，还是true。等熟悉了0.5.0，再说
     djy_event_pop(flush_evtt,false,NULL,0,NULL,0,0,0);

}
#endif

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))


