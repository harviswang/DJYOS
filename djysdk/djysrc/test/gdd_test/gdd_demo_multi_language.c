
#include  	"stdlib.h"
#include 	"stdio.h"
#include    "djyos.h"
#include    "gdd.h"


/*========================================================================================*/
const   char *wstr =

                "����: ������Բ���!\r\n"
                "����: �����Z�Ԝyԇ!\r\n"
                "����: �����Z����Υƥ���!\r\n"
                "Ӣ��: Multi-lingual testing!\r\n"
                "����: ���ߧ�ԧ��٧��ߧ�� ��֧��ڧ��ӧѧߧڧ�!\r\n"
                "����: Tests de langues ��trang��res!\r\n"
                "����: Mehrsprachige testen!\r\n"
                "�ڿ�����: ���ѧԧѧ��ާ�ӧߧڧ� ��֧���ӧѧߧߧ�!\r\n"

                ;

#define IDB_CLOSE   0x1000


static  u32 win_proc(MSG *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc0;

    hwnd =pMsg->hwnd;

    switch(pMsg->Code)
    {
        case    MSG_CREATE:
                GetClientRect(hwnd,&rc0);
                CreateWindow(BUTTON,"�ر�",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,RectW(&rc0)-64,RectH(&rc0)-28,60,24,hwnd,IDB_CLOSE,NULL);
                GDD_CreateTimer(hwnd,1,5000,TMR_START);
                break;
                ////
        case    MSG_TIMER:
        //      PostMessage(hwnd,MSG_CLOSE,0,0);
                break;
                /////

        case    MSG_NOTIFY:
                {
                    u16 event,id;

                    event =HI16(pMsg->Param1);
                    id =LO16(pMsg->Param1);

                    if(event==BTN_UP && id==IDB_CLOSE)
                    {
                        PostMessage(hwnd,MSG_CLOSE,0,0);
                    }
                }
                break;
                ////

        case    MSG_PAINT:
                {

                    hdc =BeginPaint(hwnd);
                    GetClientRect(hwnd,&rc0);
                    SetFillColor(hdc,RGB(170,160,135));
                    FillRect(hdc,&rc0);


                    SetTextColor(hdc,RGB(0,0,0));
                    TextOut(hdc,2,4,wstr,-1);

                    EndPaint(hwnd,hdc);

                }
                break;
                ////
        case    MSG_CLOSE:
                printf("win2_close.\r\n");
                DestroyWindow(hwnd);
                return 1;
                ////
        case    MSG_DESTROY:
                printf("win2_destroy.\r\n");
                PostQuitMessage(hwnd,0);
                return 1;
                ////

        default:
                return  DefWindowProc(pMsg);


    }
    return  0;
}

/*========================================================================================*/
void    WDD_Demo_MultiLanguage(void)
{
    HWND hwnd;
    MSG msg;
    RECT rc;

    //WDD_SleepMS(200);

    GetClientRect(GetDesktopWindow(),&rc);

    //InflateRect(&rc,-20,-20);

    //����������
    //hwnd = CreateWindow(win_proc,L"Unicode������Բ���",WS_MAIN_WINDOW,rc.x,rc.y,rc.w,rc.h,NULL,0x0000,NULL);
    hwnd = CreateWindow(win_proc,"Unicode������Բ���",0,
                        rc.left,rc.top,RectW(&rc),RectH(&rc),NULL,0x0000,NULL);

    ShowWindow(hwnd,TRUE);  //��ʾ����

    while(GetMessage(&msg,hwnd))
    {
        DispatchMessage(&msg);
    }
    printf("win2_exit.\r\n");

}
