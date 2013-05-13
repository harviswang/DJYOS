//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:�ļ�ϵͳ��洢�����޹صĲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))

#include <ctype.h>
#include <string.h>
#include "file.h"
#include "flashfile.h"

//ansi c standard ֧�ֵ��ļ������������

//�� The wide-character input functions �� those functions described in 7.24 that perform
//input into wide characters and wide strings: fgetwc, fgetws, getwc, getwchar,
//fwscanf, wscanf, vfwscanf, and vwscanf.

//�� The wide-character output functions �� those functions described in 7.24 that
//perform output from wide characters and wide strings: fputwc, fputws, putwc,
//putwchar, fwprintf, wprintf, vfwprintf, and vwprintf.

//�� The wide-character input/output functions �� the union of the ungetwc function,
//the wide-character input functions, and the wide-character output functions.

//�� The byte input/output functions �� those functions described in this subclause that
//perform input/output: fgetc, fgets, fprintf, fputc, fputs, djyfs_fread,
//fscanf, djyfs_fwrite, getc, getchar, gets, printf, putc, putchar, puts,
//scanf, ungetc, vfprintf, vfscanf, vprintf, and vscanf.

//Operations on files���ļ���������
//int djyfs_remove(const char *filename);
//int djyfs_rename(const char *old, const char *new);
//FILE *tmpfile(void);
//char *tmpnam(char *s);
//
//File access functions���ļ����ʺ���
//int djyfs_fclose(FILE *stream);
//int djyfs_fflush(FILE *stream);
//FILE *djyfs_fopen(const char * filename,const char * mode);
//FILE *freopen(const char * filename,const char * mode,FILE * restrict stream);
//void setbuf(FILE * restrict stream,char * restrict buf);
//int setvbuf(FILE * restrict stream,char * restrict buf,int mode, size_t size);
//
//Formatted input/output functions����ʽ�������������
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
//Character input/output functions���ַ������������
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
//Direct input/output functions��ֱ�������������
//size_t djyfs_fread(void * restrict ptr,size_t size, size_t nmemb,FILE * restrict stream);
//size_t djyfs_fwrite(const void * restrict ptr,size_t size, size_t nmemb,FILE * restrict stream);
//
//File positioning functions���ļ���дλ�ú���
//int fgetpos(FILE * restrict stream,fpos_t * restrict pos);
//int djyfs_fseek(FILE *stream, long int offset, int whence);
//int fsetpos(FILE *stream, const fpos_t *pos);
//long int ftell(FILE *stream);
//void rewind(FILE *stream);
//
//Error-handling functions����������
//void clearerr(FILE *stream);
//int feof(FILE *stream);
//int ferror(FILE *stream);
//void perror(const char *s);

//�ļ�(Ŀ¼)���������淶:
//��"a:\aaa\bbb\ccc"�У�
// 1."a"������ΪDBX_name
// 2."aaa"��"bbb"Ϊspathname
// 3."ccc"������Ϊfilename
//4."aaa"��"bbb"��"ccc"ͳ��Ϊword
//5.���������������ʱ������Ϊfullname
//6."a:\aaa\bbb\"������Ϊfullpath
//7."\aaa\bbb\"������Ϊabspath
//8."aaa\bbb\"��Ϊrelativepath
//9."\aaa\bbb\ccc"����Ϊsynname
//10."aaa\bbb\ccc"����Ϊrelativename(����ڵ�ǰ·��)

static struct pan_device *pg_fs_dev;       //�ļ�ϵͳ�豸
static struct file_rsc tg_content_buf[cn_opened_fsnode_limit];//����Ŀ¼���ڴ��
static struct mem_cell_pool *pg_content_pool;              //�ڴ��ͷָ�롣
//�趨һ������·��,�൱��windows/dos�ĵ�ǰ·��
struct file_rsc *pg_work_path = NULL;    //����·��ָ��
static djy_handle_t pg_fs_lhdl;
static struct rsc_node tg_opened_file_root; //�򿪵��ļ���Դ�ĸ��ڵ㡣

ptu32_t module_init_djyfs(ptu32_t para)
{
    //�����ļ�ϵͳ�豸���ļ�ϵͳ�豸����û�ж�д���������ж��ļ�ϵͳ�Ķ�д����
    //��������������豸--�ļ���(�׳ƴ���)���еġ�
    //ctrl���������趨�ļ�ϵͳ�Ĺ���·��������/ɾ���ļ���ȡ�
    pg_fs_dev = dev_add_device(     dev_get_root_device(),"fs",
                                    NULL,NULL,  //���ź�������
                                    (dev_write_func) NULL_func ,
                                    (dev_read_func ) NULL_func,
                                    (dev_ctrl_func ) djyfs_fs_driver_ctrl ,
                                    (dev_write_func ) NULL_func ,
                                    (dev_read_func  ) NULL_func ,
                                    (dev_ctrl_func  ) djyfs_fs_app_ctrl,
                                    0           //��ʹ��˽�б�ǩ
                                    );          //"fs"��һ�����豸
    if(pg_fs_dev == NULL)
        return 0;   //�����ļ�ϵͳ�豸ʧ��
    //����Դ�����н���һ������㣬���д򿪵��ļ���Ŀ¼�������ڴ˽���¡�
    //��ʼ���ļ���ʱ��Ϊÿ��������ӵĸ��ļ���ĸ�Ŀ¼���Ϊ�˽����ӽ�㡣
    rsc_add_tree_root(&tg_opened_file_root,sizeof(struct  file_rsc),"opened files");
    //����Ŀ¼���ڴ�أ�ÿ���򿪵��ļ���һ�ÿ��Ŀ¼Ҳ��һ�����Ƕ༶Ŀ¼��
    //��ÿ���������㣬����򿪵Ķ���ļ���·�����غϣ��غϲ��ֲ�����ռ��Ŀ¼�
    pg_content_pool = mb_create_pool(tg_content_buf,
                                cn_opened_fsnode_limit,
                                sizeof(struct file_rsc),0,0,
                                "�򿪵�Ŀ¼���ڴ��");
    pg_fs_lhdl = dev_open_left("fs",0);  //���ļ�ϵͳ�豸���ֽӿ�
    return 1;
}

//----���õ�ǰ����·��---------------------------------------------------
//����: �û����Ը����Լ����������õ�ǰ����·����Ĭ������£�����Ϊ��һ��
//      �ļ����һ����Ŀ¼��set_work_path����cofig-prj.h������
//����: 
//����: 1�����óɹ�
//      0, ����ʧ��
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
    		printf_ng("�û����ù���·��ʧ��\r\n");
            return 0;
        }
    	else
        	return 1;
    }
    //ָ���һ���ļ���ĸ�Ŀ¼
    strcpy(DBX_name,tg_opened_file_root.child->name);
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_name,0); //���ļ����豸
    if(DBX_lhdl == NULL)
        return 0;
	DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    pg_work_path = DBX_device_tag->opened_root;
    dev_close_left(DBX_lhdl);
	return 1;        
}

//----��ȡ��ǰ����·����---------------------------------
//����: ��ȡ��ǰ����Ŀ¼����ȫ·���ַ���
//����: fullname�������ַ����Ļ�����
//      namebuf_len,���ֻ������ĳ���
//����: true����ȡ�ɹ�
//      false,��ȡʧ��
//-------------------------------------------------------------------------
bool_t get_work_path_name(char *fullname,u32 namebuf_len)
{
	struct rsc_node *parent,*opened_tree;
    uint32_t index=0,len;
    struct rsc_node *point_num[cn_path_depth_limit];

	if(pg_work_path == NULL)// ˵��·��δ��ʼ�������ļ���δ����
    {
		printf_ng("�ļ���δ����\r\n");
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
            return false;       //·�����ȳ�������������
        strcat(fullname,point_num[index]->name);
        strcat(fullname,"\\");  

    }
	return true; 

}

//----��ȡ��ǰ�ļ�����(����)---------------------------------------------------
//����: ��ȡ��ǰ����Ŀ¼���ڵ��ļ�����(��Ӧwindows��dos�Ĵ���)
//����: DBX_name�������ַ����Ļ�����
//      namebuf_len,���ֻ������ĳ���
//����: true����ȡ�ɹ�
//      false,��ȡʧ��
//-------------------------------------------------------------------------
bool_t get_DBX_name(char *DBX_name,u32 namebuf_len)
{
	struct rsc_node *parent,*opened_tree;

	if(pg_work_path == NULL)// ˵��·��δ��ʼ�������ļ���δ����
    {
		printf_ng("�ļ���δ����\r\n");
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

//----��ӡ��ǰĿ¼-------------------------------------------------
//����: ��ӡ��ǰĿ¼��
//����: node���õ�����Դ�ڵ�
//����: true����ӡ�ɹ�
//      false,��ӡʧ��
//-----------------------------------------------------------------------------
bool_t print_work_path(void)
{
	struct rsc_node *parent,*opened_tree;
    uint32_t index=0;
    struct rsc_node *point_num[cn_path_depth_limit];

	if(pg_work_path == NULL)// ˵��·��δ��ʼ�������ļ���δ����
    {
		printf_ng("�ļ���δ����\r\n");
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
    //ȫ·����Ҫ������"C:\\"
    //index-=para;//��ӡ��������ӡ����·����para����ȫ·��
    while(index!=0)
    {
    	index--;
    	printf_ng("%s\\", point_num[index]->name);

    }
	return true; 

}

//----�����ļ�----------------------------------------------------------------
//����: ���ļ�ϵͳ�豸��ctrl�������õ�һ���������������ļ�ϵͳ�в���һ���ļ���
//      Ŀ¼���Ƿ���ڡ����������ļ����lookfor_item�����������ֻ���ز���
//      ������������ر�����Ŀ�����Ϣ��
//����: DBX_lhdl�����������ļ���
//      synname,�ļ�������Ŀ¼���ַ�����
//����: �ҵ��򷵻�true���Ҳ�������false
//-----------------------------------------------------------------------------
bool_t __djyfs_DBX_lookfor_item(struct pan_device *DBX_lhdl,char *synname)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *parent,*son;
    struct file_rsc temp_fp;
    uint16_t next_char_off;
    char    name[256];
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    if(__djyfs_if_abs_path(synname))  //synname���Ѿ�ȥ�����ļ��������������ж�
    {
        parent = DBX_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        next_char_off = 1;
    }else
    {
        parent = pg_work_path;  //�ӵ�ǰ·����ʼ����
        next_char_off = 0;
    }

    //djyfs_fclose�Ѿ��ж��ļ�������ˣ���������ֻ��"\\"��·������˵����Ŀ¼����
   	if(strcmp(synname,"\\")==0)
    	return true;//���ص��Ǹ�Ŀ¼

    while(__pick_word(synname,next_char_off,name))
    {//��·���𼶴�Ŀ¼���ļ�
        
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
        next_char_off += strlen(name) +1;    //+1��Ϊ�����ַ�'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                    != NULL)
        {//Ŀ���Ѿ���
            parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
        }else
        {//Ŀ���ļ�(Ŀ¼)��δ��
            //����Ŀ�겢��ʼ���ļ����ݽṹ
            if(DBX_device_tag->lookfor_item(name,parent,&temp_fp))
            {//Ŀ����ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                son = &temp_fp;   //�Ե�ǰ�򿪵�Ŀ��Ϊ�´�ʹ�õĸ�Ŀ¼
                parent = son;
            }else       //Ŀ¼�����ڣ�����false
            {
                return false;
            }
        }
        if(next_char_off >= strlen(synname))
            return true;
    }
    return true;
}

//----���ļ����е��ļ�-------------------------------------------------
//����: ���ļ����豸��ctrl�������õ�һ���������򿪸��ļ����е�һ���ļ�����������
//      �����ļ����lookfor_itemȷ��Ŀ���ļ��Ƿ���ڣ�Ȼ����·�������Ŀ¼��
//      Ϊÿһ��Ŀ¼���� struct file_rsc �ṹ�ڴ棬�������ҵ���Դ�����ϡ�����
//      ���ļ���Ҳ�ҵ���Դ�����ϡ�
//����: DBX_lhdl�����������ļ���
//      synname,�ļ�������Ŀ¼���ַ������ļ���Ŀ¼ͳ��item
//      mode���ļ���ģʽ
//����: �򿪵��ļ����򿪵�Ŀ¼��Դ�������򷵻�NULL
//-----------------------------------------------------------------------------
djyfs_file *__djyfs_DBX_open_file(struct pan_device *DBX_lhdl,
                                char *synname,char *mode)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *parent,*son;
    struct file_rsc *opened = NULL;   //�������һ���Ѿ��򿪵�Ŀ¼��
    uint32_t open_result;
    uint16_t next_char_off;
    char    name[256];
    union file_attrs attr;
    enum file_open_mode my_mode;
    bool_t need_to_creat = true;    //��ʾ���ļ�������ʱ���Ƿ���Ҫ����
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
    if(__djyfs_if_abs_path(synname))  //synname���Ѿ�ȥ�����ļ��������������ж�
    {
        parent = DBX_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        next_char_off = 1;
    }else
    {
        parent = pg_work_path;  //�ӵ�ǰ·����ʼ����
        next_char_off = 0;
    }
    //��djyfs_fclose�Ҳ����Ū��Ը�Ŀ¼�Ĳ���
   	if(strcmp(synname,"\\")==0)
    	return DBX_device_tag->opened_root;//���ص��Ǹ�Ŀ¼
        
    while(__pick_path_word(synname,next_char_off,name))
    {//��·���𼶴�Ŀ¼����������ļ�
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
        next_char_off += strlen(name) +1;    //+1��Ϊ�����ַ�'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                    != NULL)
        {//Ŀ���Ѿ���
            opened = son;
            if(son->open_counter != cn_limit_uint32)
                son->open_counter ++;
        }else
        {//Ŀ���ļ�(Ŀ¼)��δ��
            son = (struct file_rsc *)mb_malloc(pg_content_pool,0);
            if(son == NULL)
                goto exit_open_err;    //�����ڴ�ʧ��
            son->home_DBX = parent->home_DBX;
            //����Ŀ¼����ʼ���ļ����ݽṹ
            open_result = DBX_device_tag->open_item(name,parent,son,my_mode);
            if(open_result == cn_fs_open_success)
            {//Ŀ¼���ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                //��Ŀ¼��ʵ���Ͼ��ǰ�Ŀ¼���ҵ��򿪵��ļ���Դ���ϡ�
                rsc_add_head_son(&parent->file_node,&son->file_node,
                                sizeof(struct file_rsc),son->name);
                if(DBX_device_tag->opened_sum != cn_limit_uint32)
                    DBX_device_tag->opened_sum ++;
                son->open_counter = 1;
                son->eno = enum_fs_no_error;
            }else if(open_result == cn_fs_item_exist)   //Ŀ¼���ڣ���ģʽ��ƥ��
            {
                goto exit_open_err;
            }else
            {//Ŀ¼�����ڣ����Ƿ���Ҫ������
                if(need_to_creat)
                {//��Ҫ����
                    attr.all = 0;      //�Ȱ��������Գ�ʼ��Ϊfalse
                    attr.bits.folder = 1;  //��������Ŀ¼
                    if(DBX_device_tag->create_item(name,parent,attr) == false)
                    {//����Ŀ¼ʧ��
                        mb_free(pg_content_pool,son);
                        goto exit_open_err;
                    }else
                    {
                        if(DBX_device_tag->open_item(name,parent,son,my_mode)
                                            == cn_fs_open_success)
                        {
                            //��Ŀ¼ʵ���Ͼ��ǰ�Ŀ¼���ҵ��򿪵��ļ���Դ����
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
                {//����Ҫ����������r��r+��ʽ���ļ�ʱ��������Ҳ��������
                    mb_free(pg_content_pool,son);
                    goto exit_open_err;
                }
            }
            //���򿪵���Ŀ(Ŀ¼)�����ļ���
            son->home_DBX = DBX_lhdl;
        }
        parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
    }
    //���ˣ�Ŀ¼�Ѿ�ȫ����(�򴴽�)��������ļ�
    if(__pick_filename_word(synname,name))  //synname���а����ļ�����?
    {//���ļ�
        son = (struct file_rsc *)rsc_search_son(&parent->file_node,name);
        if(son == NULL)
        {//�ļ���δ��
            son = (struct file_rsc *)mb_malloc(pg_content_pool,0);
            if(son == NULL)
                goto exit_open_err;    //�����ڴ�ʧ��
            son->home_DBX = parent->home_DBX;
            //���Ҳ����ļ�
            open_result = DBX_device_tag->open_item(name,parent,son,my_mode);
            if(open_result == cn_fs_open_success)
            {//�ļ����ڲ��ҿ��԰�my_modeģʽ�򿪣��Ѿ���ʼ���ļ���Ϣ
                rsc_add_head_son(&parent->file_node,&son->file_node,
                                sizeof(struct file_rsc),son->name);
                if(DBX_device_tag->opened_sum != cn_limit_uint32)
                    DBX_device_tag->opened_sum ++;
                son->open_counter = 1;
                son->eno = enum_fs_no_error;
            }else if(open_result == cn_fs_item_exist)   //�ļ����ڣ���ģʽ��ƥ��
            {
                goto exit_open_err;
            }else
            {//�ļ������ڣ����Ƿ���Ҫ������
                if(need_to_creat)
                {//��Ҫ����
                    attr.all = 0;
                    attr.bits.archive = 1;     //���ļ�
                    if(DBX_device_tag->create_item(name,parent,attr) == false)
                    {//�����ļ�ʧ��
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
                {//r��r+��ʽ������Ҫ������
                    mb_free(pg_content_pool,son);
                    goto exit_open_err;
                }
            }
        }else
        {//�ļ��Ѿ���
        //�Ѿ��򿪵ģ����������������·��������û�з�б�ܵȵȣ�������
        //���ж�һ�£��кô���
            if(son->attr.bits.folder)
            {
                if(son->open_counter != cn_limit_uint32)
                son->open_counter ++;
            }else   
            {
                if(my_mode == son->open_mode)
                {//�Ѿ��򿪵��ļ���ֻ������ͬ�ķ�ʽ�ٴδ򿪡����Ҷ�дָ�벻�ı�
                    if(son->open_counter != cn_limit_uint32)
                        son->open_counter ++;
                    return son;
                }else
                    return NULL;
            }
        }
    }else   //��Ŀ¼��Ŀ¼��if(index != 0)���ǰ�Ѿ��򿪣�������ƴ򿪴���
        return son;//���ش򿪵�Ŀ¼
    son->home_DBX = DBX_lhdl;
    return son;
exit_open_err:
    //ɾ���Ѿ���ӵ���Դ�ڵ���ͷŷ�����ڴ�,opened������Ǳ������ӵ�
    //��һ����Դ�ڵ����һ���ڵ㡣
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
//----�ļ�ϵͳ�豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ���ϲ㷢�������ļ�ϵͳ��������
//������fs_lhdl�������豸���ָ��
//      left_cmd�����������֣�ʹ��ʱת����enum fs_left_cmd����
//      data1��data2������������ص�����
//���أ������������
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

//----�ļ�ϵͳ�豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ�д洢�豸driver�������ļ�ϵͳ��������
//������fs_rhdl�������豸���ָ��
//      right_cmd�����������֣�ʹ��ʱת����enum fs_right_cmd����
//      data1��data2������������ص�����
//���أ�����������أ��μ�Դ����ע��
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
        {//�ɹ���ӷ���enum_fs_no_error�����򷵻�enum_fs_creat_dbx_error
        //data1Ϊ�ļ���ר�����ݽṹָ��
        //data2Ϊ�ź������ݽṹstruct DBX_semp_paraָ��.��Ȼ����ͨ��data1�ҵ���
        //����������ģ������Ƕȣ������������������Ǵ�data1�ҵ��ź����ķ���:
        //data1�� struct st_DBX_device_tag* ���ͣ�ָ���ļ���ר�����ݽṹ��
        //ͨ��data1��medium_tag��Ա�����ҵ�����оƬ���ݽṹ���Ӷ��ҵ���оƬ
        //���ź��������ǣ��ļ�ϵͳ��Ӧ�÷���medium_tag��Ա������data2���롣
            //��ȡ�ļ����豸ר�����ݽṹ
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
                                   //�ļ����豸˽�б�ǩָ���ļ���ṹר�����ݽṹ
                                       (ptu32_t)DBX_device_tag
                                       );
            if(DBX_device == NULL)
                return enum_fs_creat_dbx_error;
            if(DBX_device_tag->formatted)   //�����Ѿ���ʽ���õ��ļ���
            {
                //�����Ŀ¼��Դ����ڴ�
                root_folder = mb_malloc(pg_content_pool,0);
                if(root_folder == NULL)
                {   //���벻���ڴ棬�ͷ����Ƚ������豸
                    dev_delete_device(DBX_device);
                    return enum_fs_creat_dbx_error;
                }
                memset(root_folder,0,sizeof(struct file_rsc));
                //�����ļ���ĸ�Ŀ¼��Դ�ڵ���뵽�ļ�����Դ����£����ڶ���β
                rsc_add_son(&tg_opened_file_root,&root_folder->file_node,
                                  sizeof(struct file_rsc),DBX_device_tag->name);
                root_folder->home_DBX = DBX_device;
                //�ļ����豸�Ĵ��ļ��ĸ����ָ��ø���Դ��㡣
                DBX_device_tag->opened_root = root_folder;
            }else                           //����δ��ʽ�����ļ���
                DBX_device_tag->opened_root = NULL;
            return enum_fs_no_error;
        }break; //for enum_fs_add_DBX
        case enum_fs_del_DBX:
        {
            //��ȡ�ļ����豸ר�����ݽṹ
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

//----����д�ļ���------------------------------------------------------------
//����: ���ļ����豸���ֽӿڰ�����д���ļ�,���������ļ��������д�������ã�
//      ����˳��:
//      �û�����fwrite
//      -->dev_write_left
//      -->DBX_device->left_hand.io_write
//      -->djyfs_DBX_app_write
//����: DBX_lhdl,�ļ������ļ���
//      buf��д�����ݻ�����ָ��
//      write_para��д�����
//      file����д���ļ�ָ�롣
//----------------------------------------------------------------------------
uint32_t djyfs_DBX_app_write(struct pan_device *DBX_lhdl,ptu32_t buf,
                        ptu32_t write_para,ptu32_t file)
{
    struct rw_para *pl_write_para = (struct rw_para *)write_para;
    uint32_t write_len;  //��Ҫд������ݳ���
    sint64_t rest_len;
    uint32_t result;
    struct file_rsc *fp = (struct file_rsc *)file;  //ȡ���ļ�ָ��
    struct st_DBX_device_tag *DBX_device_tag = 
                        (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    write_len = pl_write_para->nmemb * pl_write_para->size;  //������д��ĳ���
    //��ѯ�����豸���пռ�
    rest_len = DBX_device_tag->query_file_cubage(fp);
    if(rest_len >= write_len)
    {//�����豸�����㹻�Ŀռ�
        result=DBX_device_tag->write(fp,(uint8_t*)buf,write_len);
        result = result / pl_write_para->nmemb;
    }else
    {//�����豸��û���㹻�Ŀռ䣬д��������������¼
        write_len = (rest_len / pl_write_para->nmemb) * pl_write_para->nmemb;
        result=DBX_device_tag->write(fp,(uint8_t*)buf,write_len);
        result = result / pl_write_para->nmemb;
    }
    return result;
}
//----���ֶ��ļ���------------------------------------------------------------
//����: ���ļ����豸���ֽӿڶ����ļ�,���������ļ�������ֶ��������ã�����˳��:
//      �û�����fread
//      -->dev_read_left
//      -->DBX_device->left_hand.io_read
//      -->djyfs_DBX_app_read
//����: DBX_lhdl,�ļ������ļ���
//      buf���������ݵĻ�����ָ��
//      read_para����������
//      file���������ļ�ָ�롣
//----------------------------------------------------------------------------
ptu32_t djyfs_DBX_app_read(struct pan_device *DBX_lhdl,ptu32_t buf,
                        ptu32_t read_para,ptu32_t file)
{
    struct rw_para *pl_read_para = (struct rw_para *)read_para;
    uint32_t read_len;  //��Ҫ��ȡ�����ݳ���
    sint64_t stock_size;
    uint32_t result;
    
    struct file_rsc *fp = (struct file_rsc *)file;
    struct st_DBX_device_tag *DBX_device_tag;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    read_len = pl_read_para->nmemb * pl_read_para->size;
    //��ѯ�����豸�ж������ݿɶ����ļ��ߴ粢���ɿ���������
    stock_size = DBX_device_tag->query_file_stocks(fp);
    if(stock_size >= read_len)
    {//�����豸�����㹻������
        //��ȡ�����豸
        result = DBX_device_tag->read(fp,(uint8_t*)buf,read_len);
        result = result / pl_read_para->nmemb;
    }else
    {//�����豸��û���㹻�����ݣ���Ҫ������ȡ��������ȷ��������������Ԫ
        read_len = (stock_size / pl_read_para->nmemb) * pl_read_para->nmemb;
        //��Ҫ�������豸��ȡ�������ֽ���
        read_len = read_len * pl_read_para->size;
        //��ȡ�����豸
        result = DBX_device_tag->read(fp,(uint8_t*)buf,read_len);
        result = result / pl_read_para->nmemb;
    }

    return result;
}
//----�����ַ����Ƿ�����ļ�����-----------------------------------------------
//����: ���Ը������ַ����Ƿ�����ļ�����
//����: fullname���������ַ���
//����: ���ļ��������򷵻��ļ������ĳ��ȣ����򷵻�0
//��ע: �ڲ�����������ǰ��ȷ��fullname�ǺϷ����ַ�����
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

//----����Ŀ¼���Ƿ����·��--------------------------------------------------
//����: ����Ŀ¼���Ƿ����·��
//����: fullname���������ַ���
//����: true = fullname�Ǿ���·����false = ���Ǿ���·��
//��ע: �ڲ�����������ǰ��ȷ��fullname�ǺϷ����ַ�����
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

//----����Ŀ¼���Ƿ�����ļ���--------------------------------------------------
//����: ����Ŀ¼���Ƿ�����ļ���
//����: fullname���������ַ���
//����: true = fullname�����ļ�����false = �������ļ���
//��ע: �ڲ�����������ǰ��ȷ��fullname�ǺϷ����ַ�����
//----------------------------------------------------------------------------
bool_t __djyfs_if_contain_filename(char *fullname)
{
    //fullname���Ѿ������ַ������ȺϷ��Լ���ָ��
    if(fullname[strlen(fullname)-1] != '\\')
        return true;
    else
        return false;
}

//----�鿴�ļ�(Ŀ¼)�Ƿ��Ѿ���----------------------------------------------
//����: �鿴һ���ļ�(Ŀ¼)�Ƿ��Ѿ��򿪣��������Ƿ��ڱ��򿪵��ļ���Դ��
//����: synname��������ļ�(Ŀ¼)��
//����: true = �Ѿ��򿪣�false = δ��(����������)
//��ע: ����ǰ��֤fullname�ǺϷ����ַ���
//----------------------------------------------------------------------------
bool_t __djyfs_item_if_opened(struct st_DBX_device_tag *DBX_device_tag,
                              char *synname)
{
    struct file_rsc *parent,*son;
    uint16_t offset,name_len,index=0;
    char    name[256];

    if(__djyfs_if_abs_path(synname))
    {
        parent = DBX_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        offset = 1;
    }else
    {
        parent = pg_work_path;  //�ӵ�ǰ·����ʼ����
        offset = 0;
    }
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    name_len = strlen(synname);
    for( ; offset< name_len; offset++)
    {//��·���𼶲鿴Ŀ¼�Ƿ��Ѿ���
        if(synname[offset] == '\\')    //�����ַ�'\'
        {
            name[index] = '\0';
            index = 0;
            if((son=(struct file_rsc*)rsc_search_son(&parent->file_node,name))
                                            != NULL)
            {//Ŀ���Ѿ���
                parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
            }else
            {
                return false;
            }
        }else   //δ�����ַ�'\'��
        {
            name[index] = synname[offset];     //���Ƶ�ǰ�ַ����ļ�����
            index++;
        }
        if(index >= cn_file_name_limit)    //Ŀ¼�����ȳ��ޡ�
            return false;
    }
    //���ˣ����filename��������ļ���(ĩ�ַ�����'\')��name��������ļ�����
    //index�Ǵ����ȣ���������'\0'δ��ֵ������·�����Ѿ��򿪡�
    //���filename������Ŀ¼������name�������һ��Ŀ¼�����Ѿ��򿪣�index=0��
    if(index != 0)
    {
        name[index] = '\0';
        if(rsc_search_son(&parent->file_node,name) != NULL)
        {
            return true;        //�ļ��Ѿ���
        }else
        {
            return false;       //�ļ���δ��
        }
    }else
        return true;
}

//----ɾ���ļ�(Ŀ¼)----------------------------------------------------------
//���ܣ�ɾ��һ���ļ���Ŀ¼��������������ɾ���ǿ�Ŀ¼���Լ�ֻ���ļ���Ŀ¼����Ҫ
//      ɾ��ֻ���ļ���ֻ���Ȱ�����Ϊ��ֻ���ļ����ܽ��С����������ļ��������
//      ���ƺ������ã�����˳��:
//      �û�����remove
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (��enum_DBX_remove����)
//      -->__djyfs_DBX_remove_item
//����: DBX_lhdl�����������ļ���
//      synname,�ļ�������Ŀ¼���ַ������ļ���Ŀ¼ͳ��item
//���أ��ο��ļ�ϵͳ��������б�
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
    	return (uint32_t)enum_fs_object_opened;       //����ɾ���Ѿ��򿪵Ķ���
    }
    if(__djyfs_if_abs_path(synname))  //synname���Ѿ�ȥ�����ļ��������������ж�
    {
        parent = DBX_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        next_char_off = 1;
    }
	else
    {
        parent = pg_work_path;  //�ӵ�ǰ·����ʼ����
        next_char_off = 0;
    }
    //�Ǹ�Ŀ¼������ɾ��
   	if(strcmp(synname,"\\")==0)
    	return enum_fs_dbx_error;
    while(__pick_word(synname,next_char_off,name))
    {//��·���𼶴�Ŀ¼����������ļ�
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
        next_char_off += strlen(name) +1;    //+1��Ϊ�����ַ�'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                    != NULL)
        {//Ŀ���Ѿ���
            parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
        }
      else
        {//Ŀ���ļ�(Ŀ¼)��δ��
            //����Ŀ�겢��ʼ���ļ����ݽṹ
            if(DBX_device_tag->lookfor_item(name,parent,&item))
            {//Ŀ����ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                son = &item;   //�Ե�ǰ�򿪵�Ŀ��Ϊ�´�ʹ�õĸ�Ŀ¼
                parent = son;
            }else       //Ŀ¼�����ڣ�����false
            {
                return (uint32_t)enum_fs_object_nonentity;   //��ɾ���Ķ��󲻴���
            }
        }
        if(next_char_off >= strlen(synname))
            break;
    }
    item.open_counter = 0;     //�򿪴�������
    item.home_DBX = DBX_lhdl;
    if(item.attr.bits.read_only == true)
        return (uint32_t)enum_fs_object_readonly;       //����ɾ��ֻ������
    if(item.attr.bits.folder)
    {//�Ǹ�Ŀ¼����Ҫ�ж��Ƿ��
        if(DBX_device_tag->check_folder(&item) != 0)
            return (uint32_t)enum_fs_folder_unblank;
    }
    if(DBX_device_tag->remove_item(&item))
        return enum_fs_no_error;
    else
        return enum_fs_remove_error;
}

//----Ŀ¼(�ļ�)����-----------------------------------------------------------
//���ܣ���һ��Ŀ¼���ļ����������������޸�ֻ��Ŀ¼���ļ��������֣�Ҫ�޸�ֻ����
//      �����������޸�ֻ���ļ������ԣ����������ļ�������ֿ��ƺ������ã�
//      ����˳��:
//      �û�����rename
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (��enum_DBX_rename����)
//      -->__DBX_rename_item
//����: DBX_lhdl�����������ļ���
//      old_synname,�ļ���Ŀ¼ԭ���ַ�����
//      newname���ļ���Ŀ¼���������ַ���
//���أ��ο��ļ�ϵͳ��������б�
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
    	return (uint32_t)enum_fs_object_opened;       //����ɾ���Ѿ��򿪵Ķ���
    }
    if(__djyfs_if_abs_path(old_synname))  //synname���Ѿ�ȥ�����ļ��������������ж�
    {
        parent = DBX_device_tag->opened_root;  //�Ӹ�Ŀ¼��ʼ����
        next_char_off = 1;
    }else
    {
        parent = pg_work_path;  //�ӵ�ǰ·����ʼ����
        next_char_off = 0;
    }
    //�Ǹ�Ŀ¼�������޸�
   	if(strcmp(old_synname,"\\")==0)
    	return enum_fs_rename_error;
     while(__pick_word(old_synname,next_char_off,name))
    {//��·���𼶴�Ŀ¼����������ļ�
        //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
    //	printf_ng(name);
        next_char_off += strlen(name) +1;    //+1��Ϊ�����ַ�'\'
        if((son = (struct file_rsc *)rsc_search_son(&parent->file_node,name))
                   != NULL)
        {//Ŀ���Ѿ���
           parent = son;   //�Ե�ǰ�򿪵�Ŀ¼Ϊ�´�ʹ�õĸ�Ŀ¼
        }else
       {//Ŀ���ļ�(Ŀ¼)��δ��
            //����Ŀ�겢��ʼ���ļ����ݽṹ
            if(DBX_device_tag->lookfor_item(name,parent,&item))
            {//Ŀ����ڣ������Ѿ���ʼ��Ŀ¼��Ϣ
                son = &item;   //�Ե�ǰ�򿪵�Ŀ��Ϊ�´�ʹ�õĸ�Ŀ¼
                parent = son;
            }else       //Ŀ¼�����ڣ�����false
            {
                return (uint32_t)enum_fs_object_nonentity;   //��ɾ���Ķ��󲻴���
            }
        }
        if(next_char_off >= strlen(old_synname))
            break;
    }
    if(item.attr.bits.read_only == true)
    {
       	return enum_fs_object_readonly;   //ֻ���ļ����������
     }

    if( DBX_device_tag->rename_item(son,newname))
        return enum_fs_no_error;
    else
        return enum_fs_rename_error;

}


//----�ر��ļ�(Ŀ¼)----------------------------------------------------------
//���ܣ��ر�һ���ļ���Ŀ¼�������������ļ�������ֿ��ƺ������ã�����˳��:
//      �û�����fclose
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (��enum_DBX_close����)
//      -->__DBX_close_item
//������DBX_lhdl�����ر��ļ���Ŀ¼���������ļ���
//      file�����رյ��ļ���Ŀ¼��ָ��
//���أ�true=�ɹ��ر���false=ʧ��
//----------------------------------------------------------------------------
bool_t __DBX_close_item(struct pan_device *DBX_lhdl,struct file_rsc *fp)
{
    struct st_DBX_device_tag *DBX_device_tag;
    struct file_rsc *parent,*son;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    if(fp->attr.bits.folder)
    {
        if(rsc_get_son(&fp->file_node) != NULL)
            return false;       //���رյ�Ŀ¼�����Ѿ����򿪵�������ܹر�
    }
  
    //������ļ�����Դ������ɾ���������Ѿ��յ�Ŀ¼
    son = fp;
    parent = (struct file_rsc *)rsc_get_parent(&son->file_node);
    do
    {//�ӱ�ɾ����㿪ʼ��������ɾ����Ŀ¼����Ŀ¼��ָ��Դ�����ϵĿ�Ŀ¼��㣬
     //������洢���ϸ�Ŀ¼���ܲ��ǿյ�
        if(son == pg_work_path)  //��ǰ����·�����ܱ�ɾ��
            break;

      //����Ǹ�Ŀ¼�Ļ����ǲ����ͷŵġ�ֱ�ӷ��أ���������
      	if(parent == DBX_device_tag->opened_root)
         {
        	DBX_device_tag->close_item(son);
        	if(rsc_del_node(&son->file_node) != NULL) //�Ӵ��ļ���Դ������ɾ�����
            	mb_free(pg_content_pool,son);               //�ͷŽ���ڴ� 
        	if(DBX_device_tag->opened_sum != 0)
            	DBX_device_tag->opened_sum --;   
        	break;        
          }
      //����ɾ�����ļ��������ֵܱ�����ô�����ͷŸ��ڵ�
    	if(rsc_get_next(&son->file_node) != &son->file_node)
        {
        	DBX_device_tag->close_item(son);
        	if(rsc_del_node(&son->file_node) != NULL) //�Ӵ��ļ���Դ������ɾ�����
            	mb_free(pg_content_pool,son); 
        	if(DBX_device_tag->opened_sum != 0)
            	DBX_device_tag->opened_sum --;
        	break;
        }
    	DBX_device_tag->close_item(son);
        if(rsc_del_node(&son->file_node) != NULL) //�Ӵ��ļ���Դ������ɾ�����
        	mb_free(pg_content_pool,son);               //�ͷŽ���ڴ�

        if(son->open_counter>1)
        {//��Ŀ¼��α��򿪣���ֻ��Ҫ�򿪴�����1���ʹﵽ�ر�һ�ε�Ŀ��
            son->open_counter--;
            break;
        }
        son = parent;               //��һѭ���ͷŸ����
        parent=(struct file_rsc*)rsc_get_parent(&parent->file_node);//���������
        if(DBX_device_tag->opened_sum != 0)
            DBX_device_tag->opened_sum --;
    }while(son != DBX_device_tag->opened_root);    //ֱ����Ŀ¼
    return true;
}

//----���ļ�д�뵽�洢��------------------------------------------------------
//����: ���ļ�д�����������ݺ�buf�е�����һ��д�뵽�洢���У������д��������
//      ����˳��:
//      �û�����fflush
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (��enum_DBX_flush����)
//      -->__DBX_flush_file
//������DBX_lhdl���������ļ��������ļ���
//      buf�����ݻ�����ָ��
//      fp�����رյ��ļ���Ŀ¼��ָ��
//���أ�д��洢���ʵ�������
//----------------------------------------------------------------------------
uint32_t __DBX_flush_file(struct pan_device *DBX_lhdl,struct file_rsc *fp)
{
    struct st_DBX_device_tag *DBX_device_tag;
    uint32_t result;
    DBX_device_tag = (struct st_DBX_device_tag*)dev_get_my_tag(DBX_lhdl);
    result = DBX_device_tag->flush(fp);
    return result;
}

//----flush�ļ���������ļ�----------------------------------------------------
//����: ���ļ����������ļ���д������������д�뵽�洢���У������д��������
//      ����˳��:
//      �û�����fflush
//      -->dev_ctrl_left
//      -->DBX_device->left_hand.io_ctrl
//      -->DBX_left_ctrl (��enum_DBX_flush_all����)
//      -->__DBX_flush_all
//������DBX_lhdl���������ļ��������ļ���
//���أ�0
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
    {//��ѭ�����������ļ������д򿪵��ļ���Ŀ¼
        item = (struct file_rsc *)temp;
        if( ! item->attr.bits.folder)    //������ļ�����ˢ���洢��
            __DBX_flush_file(DBX_lhdl,item);
        current = temp; //rsc_search_tree�����´ε��õ����
    }
    return 0;
}
//----�ļ����豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ���ϲ㷢�������ļ�ϵͳ��������
//������DBX_lhdl�������豸���ָ��
//      left_cmd�����������֣�ʹ��ʱת����enum DBX_left_cmd����
//      data1��data2������������ص�����
//���أ�����������أ��μ�Դ����ע��
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
        {//��ʽ���ļ���data1��data2�Ǹ�ʽ������
            if(DBX_device_tag->opened_root == NULL)
            {
                //�����Ŀ¼��Դ����ڴ�
                root_folder = mb_malloc(pg_content_pool,0);
                if(root_folder == NULL)
                {
                    DBX_device_tag->formatted = false;
                    return enum_fs_create_root_folder_error;
                }
                memset(root_folder,0,sizeof(struct file_rsc));
                //�����ļ���ĸ�Ŀ¼��Դ�ڵ���뵽�ļ�����Դ����£�
                rsc_add_son(&tg_opened_file_root,&root_folder->file_node,
                                  sizeof(struct file_rsc),DBX_device_tag->name);
                root_folder->home_DBX = DBX_lhdl;
                //�ļ����豸�Ĵ��ļ��ĸ����ָ��ø���Դ��㡣
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
        {//�����ļ���data1���ļ���(Ŀ¼��)
            return DBX_device_tag->formatted;
        }break; //for enum_chk_DBX_format
        case enum_DBX_lookfor:
        {//�����ļ���data1���ļ���(Ŀ¼��)
            return (ptu32_t)__djyfs_DBX_lookfor_item(DBX_lhdl,(char*)data1);
        }break; //for enum_DBX_lookfor
        case enum_DBX_open:
        {//���ļ���data1���ļ�����data2��ģʽ�ַ����������ļ�ָ��
            return (ptu32_t)__djyfs_DBX_open_file(DBX_lhdl,(char*)data1,(char*)data2);
        }break; //for enum_DBX_open
        case enum_DBX_remove:
        {//data1���ļ���Ŀ¼������data2����
            return (ptu32_t)__djyfs_DBX_remove_item(DBX_lhdl,(char*)data1);
        }break;     //for enum_DBX_remove
        case enum_DBX_rename:
        {//data1��ԭ�ļ���Ŀ¼������data2�����ļ���
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

//----�ļ����豸���ֿ��ƺ���-------------------------------------------------
//���ܣ�����˼�壬ִ��һϵ�д洢�豸driver�������ļ�ϵͳ��������
//������DBX_rhdl�������豸���ָ��
//      right_cmd�����������֣�ʹ��ʱת����enum DBX_right_cmd����
//      data1��data2������������ص�����
//���أ�����������أ��μ�Դ����ע��
//-----------------------------------------------------------------------------
ptu32_t DBX_right_ctrl(struct pan_device *DBX_rhdl,uint32_t right_cmd,
                                ptu32_t data1,ptu32_t data2)
{
    switch((enum DBX_right_cmd)right_cmd)
    {
        case enum_DBX_clr_readbuf:
        {//����ļ��Ķ���������data1���ļ�ָ��
            if(((struct file_rsc*)data1)->p_read_buf != NULL)
               ring_flush(((struct file_rsc*)data1)->p_read_buf);
            return 0;
        }break; //for enum_DBX_clr_readbuf
        case enum_DBX_clr_writebuf:
        {//����ļ���д��������data1���ļ�ָ��
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
        {//�޸�(����)һ���ļ���ý���ǣ�data1Ϊ�ļ�ָ�룬data2Ϊ�±��
            ;
        }break; //for enum_DBX_update_medium_tag
        case enum_DBX_search_medium_tag:
        {//����һ���ļ���ý���ǣ�data1Ϊ���ļ���ý���ǣ������ļ�ָ��
            ;
        }break; //for enum_DBX_search_medium_tag
        default:break;
    }
    return 0;
}

//----���ļ������ֽӿ�------------------------------------------------------
//����: �����ļ�(Ŀ¼)��������֮��Ӧ���ļ������ֽӿڡ�
//����: fullname���ļ�(Ŀ¼)��ָ��
//���أ��豸��������������������NULL:
//      1.�ļ��񲻴���  2.fullname��pg_work_path_name��û��ָ���ļ�����
//----------------------------------------------------------------------------
djy_handle_t __open_DBX_left(char *fullname)
{
    djy_handle_t DBX_lhdl;
    char DBX_name[256];
    uint16_t name_len;
    
    name_len = __djyfs_if_contain_DBX_name(fullname);
    if(name_len != 0)
    {//fullname�ǰ����ļ��������ڵ�ȫ·��(Ҳ�����ǵ�ǰ�ļ���)
        memcpy(DBX_name,fullname,name_len); //�Ӳ�����copy�ļ�����
        DBX_name[name_len] = '\0';          //д�봮������
    }else
    {
        //fullname��û��ָ���ļ�������Ӧ�ӵ�ǰ����·����ȡ
    	if(!get_DBX_name(DBX_name,cn_file_name_limit))
            return NULL;
    }
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_name,0); //���ļ����豸
    return DBX_lhdl;
}

//----��Ŀ¼��ȡһ��-----------------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ���curent_offsetλ�ÿ�ʼȡ��һ��
//      ��'\'�ָ���word
//����: synname�������ַ�������ʽ��"\aaa\bbb\ccc.txt"
//      start_offset��������ʼλ��ָ��
//      name������ȡ�õ��ַ����Ļ�����ָ��
//����: true=�ɹ�
//      false=ʧ�ܣ�1���Ѿ�������һ��Ŀ¼��
//                  2��synname�и����Ͳ�����Ŀ¼��
//                  3���ֳ�����255�ַ�
//      ��*name�з���ȡ�õ��ַ���
//��ע: synnameӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __pick_word(char *synname,uint32_t curent_offset,char *name)
{
    uint32_t index = 0;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    uint32_t name_len = strlen(synname);
    uint32_t offset = curent_offset;
    for( ; offset< name_len; offset++)
    {
        if(synname[offset] == '\\')    //�����ַ�'\'
        {
            name[index] = '\0';
            return true;
        }else   //δ�����ַ�'\'��
        {
            if(index == cn_file_name_limit)
                return false;
            name[index] = synname[offset];     //���Ƶ�ǰ�ַ����ļ�����
            index++;
        }
    }

	name[index] = '\0';//�ļ�(��Ŀ¼)�Ľ�ֹ���š�
    return true;   //����ȷ��ȡĿ¼�֣����򲻻���������
}

//----ȡһĿ¼����-------------------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�����curent_offsetλ�ÿ�ʼȡ��һ��
//      Ŀ¼��
//����: synname�������ַ�������ʽ��"\aaa\bbb\ccc.txt"
//      start_offset��������ʼλ��ָ��
//      name������ȡ�õ��ַ����Ļ�����ָ��
//����: true=�ɹ�
//      false=ʧ�ܣ�1���Ѿ�������һ��Ŀ¼��
//                  2��synname�и����Ͳ�����Ŀ¼��
//                  3���ֳ�����255�ַ�
//      ��*name�з���ȡ�õ��ַ���
//��ע: synnameӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __pick_path_word(char *synname,uint32_t curent_offset,char *name)
{
    uint32_t index = 0;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    uint32_t name_len = strlen(synname);
    uint32_t offset = curent_offset;
    for( ; offset< name_len; offset++)
    {
        if(synname[offset] == '\\')    //�����ַ�'\'
        {
            name[index] = '\0';
            return true;
        }else   //δ�����ַ�'\'��
        {
            if(index == cn_file_name_limit)
                return false;
            name[index] = synname[offset];     //���Ƶ�ǰ�ַ����ļ�����
            index++;
        }
    }
    return false;   //����ȷ��ȡĿ¼�֣����򲻻���������
}

//----ȡ�ļ�����---------------------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ�����ȡ���ļ���
//����: synname�������ַ�������ʽ��"\aaa\bbb\ccc.txt"
//      start_offset��������ʼλ��ָ��
//      name������ȡ�õ��ַ����Ļ�����ָ��
//����: true=�ɹ�
//      false=ʧ�ܣ�synname�в������ļ���
//      ��*name�з���ȡ�õ��ַ���
//��ע: synnameӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __pick_filename_word(char *synname,char *name)
{
    uint32_t index = 0;
    //synname���Ѿ������ַ������ȺϷ��Լ���ָ��
    uint32_t name_len = strlen(synname);
    uint32_t offset;
    for(offset = name_len; offset >0; offset--)
    {//�����һ���ַ������ַ�'\'
        if(synname[offset-1] == '\\')    //�����ַ�'\'
            break;
    }
    if(offset == name_len)      //���һ���ַ�����'\'������û���ļ���
        return false;
    else if((name_len -offset) > cn_file_name_limit)    //�ļ�������
        return false;
    for( ; offset < name_len; offset++)
    {
        name[index] = synname[offset];
        index++;
    }
    name[index] = '\0';
    return true;
}

//----����·���е��ļ��񲿷�---------------------------------------------------
//����: �����ļ�(Ŀ¼)���ַ����й����ļ���Ĳ��֣���ȡsynname�����磬
//      ����"c:\aaa\bbb\ccc.txt"��������"\aaa\bbb\ccc.txt"
//����: fullname,�����ַ���
//����: ȡ�õ�·����
//----------------------------------------------------------------------------
char *__pick_synname(char *fullname)
{
    uint16_t name_len;

    name_len = __djyfs_if_contain_DBX_name(fullname);
	if(name_len != 0)//����·��
       	return fullname + name_len +1;
	else
    	return fullname;//���·��������ԭ�����ַ���
}


//----���·�����Ƿ�Ϸ�------------------------------------------------------
//����: ���·�����Ƿ�Ϸ�����windows���ݣ��ݲ���������·��������������:
//      1.���ַ�':'�ָ����ļ��������ȱ���Ϸ���':'����������ַ�'\'
//      2.�����������������'\'��
//      3.���������'* ? / | < > "'��7���ַ�,��Щ�ַ���������������;
//      4.��������ֳ��ȳ���255�ַ���object
//����: fullname,�����ַ���
//����: true=�Ϸ���false=�Ƿ�
//----------------------------------------------------------------------------
bool_t __check_fullname(char *fullname)
{
    uint32_t colon_offset = cn_limit_uint32,syn_offset;
    uint32_t loop,temp;
    ufast_t depth = 0;
    if(fullname == NULL)
        return false;
    //����ʹ��strlen���fullname���ȣ���Ϊ�����������м����Ŀ�����޶�������
    //������
    for(loop = 0; loop <= cn_DBX_name_limit; loop++)
    {//��鴮�е�һ��':'������ʲôλ��
        if((fullname[loop]=='*')||(fullname[loop]=='/')||(fullname[loop]=='?')
           ||(fullname[loop]=='<')||(fullname[loop]=='>')||(fullname[loop]=='|')
           ||(fullname[loop]=='"'))
            return false;   //���ƴ��в��ܳ����⼸���ַ���
        if(fullname[loop] == ':')
        {
            colon_offset = loop; //��һ��':'���ֵ�λ�ã����ڼ���ļ���������
            break;
        }

        if(fullname[loop] == '\\')   //����word�ָ�����
            break;
    	if(fullname[loop] == '\0')    //����ֹͣ����
        	break;
    }
    if(colon_offset == 0)
        return false;       //���ַ�������':'
    if(colon_offset != cn_limit_uint32) //':'λ�úϷ�(���ļ������ҳ��ȺϷ�)
    {
        if(fullname[colon_offset+1] != '\\')
            return false;       //ð�ź���������ַ�'\'
        syn_offset = colon_offset + 2;
    }else if(fullname[0] =='\\')
        syn_offset = 1;
    else
        syn_offset = 0;
    temp = 0;
    //�����û�г���cn_file_name_limit�ַ���object���������û�зǷ��ַ�
    //�����ļ�������ȫ·�����������ַ��������ᳬ��
    //cn_path_depth_limit * (cn_file_name_limit+1),+1�ǰ���word�ָ���'\'
    for(loop = syn_offset;
               (loop < cn_path_depth_limit*(cn_file_name_limit+1)+syn_offset);
               loop++)
    {
        //���ļ��������濪ʼ�������Ƿ��ַ�����ʱ��ð��':'Ҳ�ǷǷ��ַ�
        if((fullname[loop]=='*')||(fullname[loop]=='/')||(fullname[loop]=='?')
           ||(fullname[loop]=='<')||(fullname[loop]=='>')||(fullname[loop]=='|')
           ||(fullname[loop]=='"')||(fullname[loop]==':'))
            return false;   //�����⼸���Ƿ��ַ���
        if(fullname[loop] == '\0')
            return true;
        if(fullname[loop] == '\\')
        {
            if(temp == 0)
                return false;       //˵������������word�ָ���'\'
            temp = 0;//�ļ��ĳ������㣬���¿�ʼ����
            depth++;
        }else
        {
            if(depth >= cn_path_depth_limit)    //·������
                return false;
            temp++;
            if(temp > cn_file_name_limit)       //word���ȳ���
                return false;
        }
    }
    return false;       //���ܵ�����϶�û���ҵ���������
}

//----��鵥�����Ƿ�Ϸ�------------------------------------------------------
//����: ���һ������������Ƿ�Ϸ��������ܺ�'\/?:*'�⼸���ַ�
//����: word,�����ַ����������ǵ�һ·������Ҳ�������ļ���
//����: true=�Ϸ���false=�Ƿ�
//----------------------------------------------------------------------------
bool_t __check_word(char *word)
{
    uint32_t loop;
    //word���Ѿ������ַ������ȺϷ��Լ���ָ��
    for(loop = 0; loop < strlen(word); loop++)
    {
        if((word[loop]=='*')||(word[loop]=='/')
                        ||(word[loop]=='?')||(word[loop] == ':'))
            return false;   //�����⼸���Ƿ��ַ���
    }
    return true;
}

//----��ȡһ��Ŀ¼�µ���Ŀ¼(�ļ�)-------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڻ�ȡ�ļ���Ŀ¼������Ϣ
//����: fullname,�ļ���Ŀ¼����
//����: �ҵ��򷵻�true���Ҳ�������false
//��ע: ��������Ƿ�Ҫ���windows���Ƶ�FindFirstFile������API����ʱ������
//      api��ʱ����˵��
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
        //�����ļ�������ֿ��ƺ���
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

//----���ļ�----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴ��ļ�ϵͳ�е�һ���ļ�
//����: fullname,�ļ���
//      mode���ļ���ģʽ
//����: �ɹ����򷵻��ļ�ָ�룬���򷵻�NULL��
//-----------------------------------------------------------------------------
djyfs_file *djyfs_fopen(char *fullname, char *mode)
{
    struct file_rsc *result;
    djy_handle_t DBX_lhdl;
    struct st_DBX_device_tag *DBX_device_tag;
    char *synname;
    //fullname����һ���Ϸ����ַ���
    if((__check_fullname(fullname) == false)||(mode ==  NULL))     
        return NULL;
    DBX_lhdl = __open_DBX_left(fullname);
    if(DBX_lhdl == NULL)
        return NULL;    //�ļ��񲻴��ڣ���fullname�͵�ǰ·����û��ָ���ļ�����
    DBX_device_tag = (struct st_DBX_device_tag*)
                                dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(fullname);
        //�����ļ�������ֿ��ƺ���
        result = (struct file_rsc *)dev_ctrl_left(DBX_lhdl,enum_DBX_open,
                          (ptu32_t)synname,(ptu32_t)mode);
    }else
        result = NULL;
    dev_close_left(DBX_lhdl);
    return result;
}

//----�����ļ�----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڼ���ļ���Ŀ¼���Ƿ����
//����: fullname,�ļ���Ŀ¼����
//����: �ҵ��򷵻�true���Ҳ�������false
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
        return false;    //�ļ��񲻴��ڣ���fullname�͵�ǰ·����û��ָ���ļ�����
    DBX_device_tag = (struct st_DBX_device_tag*)
                                dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(fullname);
       //�����ļ�������ֿ��ƺ���
       result = (bool_t)dev_ctrl_left(DBX_lhdl,enum_DBX_lookfor,
                                        (ptu32_t)synname,0);
    }else
        result = false;
    dev_close_left(DBX_lhdl);
    return result;
}

//----ɾ���ļ�----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã�����ɾ��һ���ļ���Ŀ¼��
//����: fullname,�ļ���Ŀ¼����
//����: 0=�ɹ���1=δ��ʽ������0��1=����ԭ��ɾ��ʧ�ܣ����鿴�ļ�ϵͳ��������
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
        return 1;    //�ļ��񲻴��ڣ���fullname�͵�ǰ·����û��ָ���ļ�����
    DBX_device_tag = (struct st_DBX_device_tag*)
                                    dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(fullname);
    	result = dev_ctrl_left(DBX_lhdl,enum_DBX_remove,(ptu32_t)synname,0);
        //�����ļ�������ֿ��ƺ���
        if(result == enum_fs_no_error)
            result = 0;
    }else
        result = 1;
    dev_close_left(DBX_lhdl);
    return result;
}

//----�޸��ļ�(Ŀ¼)����-------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã������޸�һ���ļ���Ŀ¼��������
//����: old_fullname,�ļ���Ŀ¼������
//      new_filename��������
//����: 0=�ɹ�����0 = ʧ��
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
        return false;    //�ļ��񲻴��ڣ���fullname�͵�ǰ·����û��ָ���ļ�����
    DBX_device_tag = (struct st_DBX_device_tag*)
                                    dev_get_my_tag(dev_get_panp(DBX_lhdl));
    if(DBX_device_tag->formatted)
    {
        synname = __pick_synname(old_fullname);
        //�����ļ�������ֿ��ƺ���
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

//----�ر��ļ�-----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڹرմ������ļ�
//����: fp�����رյ��ļ�
//����: 0=�ɹ���cn_limit_uint32=ʧ�ܡ�
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
     //��Ŀ¼�͵�ǰ����·��������ر�
    if(fp == DBX_device_tag->opened_root || fp == pg_work_path)
    {
        dev_close_left(DBX_lhdl);
        return true; 
    }

    if(fp->open_counter>1)
    {//���ļ���α��򿪣���ֻ��Ҫ�򿪴�����1���ʹﵽ�ر�һ�ε�Ŀ��
        fp->open_counter--;
    	dev_close_left(DBX_lhdl);
        return 0;
    }
    result = (bool_t)dev_ctrl_left(DBX_lhdl,enum_DBX_close,(ptu32_t)fp,0);
    dev_close_left(DBX_lhdl);
    if(result)
    {
        //��ֹ��ιر�
    	fp = NULL;
        return 0;
    }
    else
        return cn_limit_uint32;
}

//----���ļ�-----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴӴ������ļ�������
//����: buf��������������ݵĻ�����
//      size�������ĳߴ磬nmemb����
//      nmemb��ÿ�����ݳ���(�ֽ���)
//      fp���������ļ�
//����: ʵ�ʶ�������������nmemb����
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
        return 0;   //��ȡ����������0
    DBX_pan_device = fp->home_DBX;
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_pan_device->node.name,0);
    if(DBX_lhdl == NULL)
        return false;
    if(fp->attr.bits.folder)
    { //Ŀ¼���ܶ�
        result = 0;
        goto goto_end;
    }
    if((fp->open_mode == enum_w_wb)||(fp->open_mode == enum_a_ab))
    { //ֻдģʽ�����ܶ�
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

//----д�ļ�-------------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڰ�����д��������ļ�
//����: buf���ṩд������ݵĻ�����
//      size��д��ĳߴ磬nmemb����
//      nmemb��ÿ�����ݳ���(�ֽ���)
//      fp����д���ļ�
//����: ʵ��д�����������nmemb����
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
        return 0;   //д�����������0
    DBX_pan_device = fp->home_DBX;
    DBX_lhdl = dev_open_left_scion(pg_fs_lhdl,DBX_pan_device->node.name,0);
    if(DBX_lhdl == NULL)
        return false;
    if(fp->attr.bits.folder)
    { //Ŀ¼����д
        result = 0;
        goto goto_end;
    }
    if(fp->open_mode == enum_r_rb)
    {//ֻ���ļ�������д
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
//----ˢ���ļ�-----------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴӴ������ļ�������
//����: buf��������������ݵĻ�����
//      size�������ĳߴ磬nmemb����
//      nmemb��ÿ�����ݳ���(�ֽ���)
//      fp�����رյ��ļ�
//����: 0=�ɹ���ָ������û�л���������ֻ����ʱҲ����0ֵ������EOFָ��һ������
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
    {   //Ŀ¼����д
        result = EOF;
        goto goto_end;
    }
    if(fp->open_mode == enum_r_rb)
    {//ֻ���ļ�������ˢ��
        result = EOF;
        goto goto_end;
    }
    result = dev_ctrl_left(DBX_lhdl,enum_DBX_flush,
                                (ptu32_t)fp,0);
goto_end:
    dev_close_left(DBX_lhdl);
    return result;
}

//----�����ļ�ָ��-------------------------------------------------------------
//����: �����ļ�ϵͳ�ṩ��һ��API���ã����ڴӴ������ļ�������
//����: buf��������������ݵĻ�����
//      size�������ĳߴ磬nmemb����
//      nmemb��ÿ�����ݳ���(�ֽ���)
//      fp�����رյ��ļ�
//����: 0=�ɹ�����0=ʧ��
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
    {//Ŀ¼û��λ��ָ��
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

//----��ʽ���ļ���-------------------------------------------------------------
//����: ��ʽ���ļ���
//����: format_para1����ʽ������
//      format_para1����ʽ������
//      dbx_name������ʽ�����ļ�������
//����: 0=�ɹ�����0=ʧ��
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
            return false;   //���ƴ��в��ܳ����⼸���ַ���
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
//----��ʱflush-------------------------------------------------------------
//����: ÿ��5�룬�������Ѿ��򿪵��ļ�д�뵽fluash
//����: my_event�����̷����¼����͡�
//����: ��
//----------------------------------------------------------------------------
void flush_all_timer(struct event_script *my_event)
{
 	djy_handle_t DBX_lhdl;
    while(1)
    {   //ÿ��5�룬flushһ�Ρ�
        djy_timer_sync(5000);
        //�ú��������û���־���·�������Զ����õ�ǰ����·����
        //�򿪵�ǰ����·����DBX_lhdl
    	DBX_lhdl = __open_DBX_left("");
        if(DBX_lhdl == NULL)
            return;//��Ȼ�ļ���δ��������ô���������û�ã��˳�
        __DBX_flush_all(DBX_lhdl);
    	dev_close_left(DBX_lhdl);
    }
}

void flush_all_init(void)
{
 	uint16_t flush_evtt;
                
	flush_evtt = djy_evtt_regist(true,true,cn_prio_RRS,1,
                                        flush_all_timer,800,NULL);

//debug ��֪����false������true������Ϥ��0.5.0����˵
     djy_event_pop(flush_evtt,false,NULL,0,NULL,0,0,0);

}
#endif

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))


