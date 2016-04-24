/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains code to implement the "sqlite" command line
** utility for accessing SQLite databases.
*/
#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "sqlite3.h"
#include <stdarg.h>

void test_sqlite1(void * pParam)
{
    sqlite3 * db = NULL; // ����sqlite �ؼ��ṹָ��
    s32 result;
    char * errmsg = NULL;

    char **dbResult; // �� char ** ���ͣ�����* ��
    s32 nRow, nColumn;
    s32 i , j;
    s32 index;
    result = sqlite3_open( "yongshao\\db.db", &db );

    if( result != SQLITE_OK ) {
        printf("open sys:\\yongshao\\db.db error!!!!\n");
    }
    else {
        printf("open sys:\\yongshao\\db.db success!!!!\n");

        // ��ʼ��ѯ���ݿ� (�ص�����)
        //result = sqlite3_exec( db,"select * from MyTable_1", LoadMyInfo, NULL, errmsg );
        //()
        result = sqlite3_get_table ( db,"select * from MyTable_1", &dbResult, &nRow, &nColumn, &errmsg );
        if( SQLITE_OK == result )
        {
            // ��ѯ�ɹ�
            index = nColumn;
            printf("�鵽%d ����¼\n ", nRow );
            for( i = 0; i < nRow ; i++ )
            {
                printf("�� %d ����¼\n", i+1 );
                for( j = 0 ; j < nColumn; j++ )
                {
                    printf("�ֶ���:%s ? > �ֶ�ֵ:%s\n", dbResult[j], dbResult [index] );
                    ++index;
                }
                printf("-------\n");
            }
        }
        else{
            result = sqlite3_exec( db,"create table MyTable_1( ID integer primary key autoincrement, name nvarchar(32) )" , NULL, NULL, &errmsg );
            if(result != SQLITE_OK )
                printf("������ʧ�ܣ�������:%d ������ԭ��:%s\n " , result, errmsg );

        }
        // ����һ����¼
        result = sqlite3_exec( db, " insert into MyTable_1( name ) values ( '��·')" , 0, 0, &errmsg );
        if(result != SQLITE_OK )
            printf("�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, &errmsg );
        sqlite3_free_table ( dbResult );
        sqlite3_close( db );
        printf("close sys:\\yongshao\\db.db !!!!\n");
    }
}

int test(int argc, char **argv){

  int rc = 0;
  test_sqlite1(0);

  return rc;
}
