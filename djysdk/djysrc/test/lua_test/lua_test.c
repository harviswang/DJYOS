
#include    "os_inc.h"

#include    "GDD_Private.h"
#include    "GDD.h"
#include    <wchar.h>

#include    "lauxlib.h"
#include    "lua.h"

/*============================================================================*/
static int average(lua_State *L)
{
/* �õ��������� */
int n = lua_gettop(L);
double sum = 0;
int i;
/* ѭ�������֮�� */
for (i = 1; i <= n; i++)
{
/* ��� */
sum += lua_tonumber(L, i);
}
/* ѹ��ƽ��ֵ */
lua_pushnumber(L, sum / n);
/* ѹ��� */
lua_pushnumber(L, sum);
/* ���ط���ֵ�ĸ��� */
return 2;
}

static int Lsleep(lua_State *L){
  int d;

  d=lua_tonumber(L, 1);
  GUI_SleepMS(d);
  lua_pushnumber(L, 1);
  return 1;
}

static int Lmyputs0(lua_State *L)
{
   char *buf;
   int len;

   buf=(char *)lua_tostring(L,1);
   len=lua_tonumber(L, 2);
   printf(buf);
  // myputs0(buf,len);

   lua_pushnumber(L, 1);
   return 1;
}

//static char buf[1024];
static char *buf;

void    lua_test(void)
{
    lua_State *L;
 //   float a,c2;

    djy_printf("lua_test.\r\n");

    L   = lua_open();
    djy_printf("L=%08XH.\r\n",L);

    luaopen_base(L);
    //ע_���Լ���c������lua��
    lua_register(L, "average", average);
    lua_register(L, "Lsleep", Lsleep);
    lua_register(L, "Lmyputs0", Lmyputs0);

    //������lua�ű��� �ű������ֵ������Լ������c����, Lmyputs0---�����������   average --ƽ��ֵ������  Lsleep--��ʱ����

    //strcpy(buf," Lmyputs0(\"start!\",6); c2=2; a1 = {\"one\",\"two\",\"three\"}    a=average(10, 20, 30, 40, 50); count=0;  while 1>0 do  \r\n  count=count+1;  Lmyputs0(a1[(count%3)+1],6);  Lsleep(300); \r\n end; \r\n Lmyputs0(\"end!\",4)");


    buf=(char*)0x001F0000;
    djy_printf("====================Start\r\n");
    djy_printf("\r\n");
    djy_printf("%s",buf);
    djy_printf("\r\n");
    djy_printf("====================End\r\n");
    //��lua���нű�!
    luaL_dostring(L, buf);

    djy_printf("lua_test end.\r\n");

}

/*============================================================================*/

