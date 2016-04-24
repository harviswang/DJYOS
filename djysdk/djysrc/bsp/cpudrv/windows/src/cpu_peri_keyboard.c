//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ɨ��Ӳ��������������ֻ��ֱ�Ӹ���������ǰֵ�����������
//����˵��:
//�޶���ʷ:
//2. ����: 2009-05-29
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: ����key_hard_init����
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"
#include "string.h"
#include "keyboard.h"
#include "cpu.h"
#include <windows.h>
#include <tchar.h>
#include "cpu_peri_lcd_touch_mouse.h"

TCHAR szKeyDriver[]  =_TEXT("�������");    // �������ı�
LRESULT CALLBACK    WndProc_key(HWND, UINT, WPARAM, LPARAM);

HDC     tg_key_hdc ;
static bool_t key_flag = false;
static u8 u8g_key_value;
HWND tg_keyboard_wnd;
extern RECT tg_WindowRect;
extern s32 s32g_window_height,s32g_window_width;

//----��ʼ������Ӳ��-----------------------------------------------------------
//����: ���⣬��ʼ�������ɨ�����
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t key_hard_init(void)
{
    RECT WindowRect;
    RECT ClientRect;

    RegisterWinClass(hInstance,WndProc_key,szKeyDriver);
    tg_keyboard_wnd = CreateWindow(szKeyDriver, szKeyDriver, WS_CAPTION | WS_MINIMIZEBOX,
                       CW_USEDEFAULT,CW_USEDEFAULT, 0, 0,
                       NULL, NULL, hInstance, NULL);

    if (!tg_keyboard_wnd)
    {
        return FALSE;
    }
    GetWindowRect(tg_keyboard_wnd, &WindowRect);
    GetClientRect(tg_keyboard_wnd, &ClientRect);
    WindowRect.right += (cn_key_board_width - ClientRect.right);
    WindowRect.bottom += (cn_key_board_height - ClientRect.bottom);
    MoveWindow(tg_keyboard_wnd, WindowRect.left, WindowRect.top, cn_key_board_width, cn_key_board_height, false);

    ShowWindow(tg_keyboard_wnd, SW_SHOW);
    UpdateWindow(tg_keyboard_wnd);
    tg_key_hdc = GetDC (tg_keyboard_wnd) ;
    return true;
}

LRESULT CALLBACK WndProc_key(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static u32 move_count = 0;
    switch (message)
    {
        case WM_CREATE:
            CreateWindow(_TEXT("button"),_TEXT("F1"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            20,40,40,40,
                            hWnd,(HMENU)VK_F1,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("F2"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            20,160,40,40,
                            hWnd,(HMENU)VK_F2,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("��"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            140,40,40,40,
                            hWnd,(HMENU)VK_UP,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("��"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            140,160,40,40,
                            hWnd,(HMENU)VK_DOWN,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("��"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            80,110,40,40,
                            hWnd,(HMENU)VK_LEFT,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("��"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            200,110,40,40,
                            hWnd,(HMENU)VK_RIGHT,
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("����"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            260,40,40,40,
                            hWnd,(HMENU)'\x1b',
                            GetWindowLong(hWnd,GWL_HINSTANCE),NULL);
            CreateWindow(_TEXT("button"),_TEXT("ȷ��"),
                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                            260,160,40,40,
                            hWnd,(HMENU)'\r',
                           ((LPCREATESTRUCT) lParam)->hInstance,NULL);
            break;
        case WM_KILLFOCUS:
            if(hWnd == GetParent((HWND)wParam))
            {
                u8g_key_value = (u8)GetWindowLong((HWND)wParam,GWL_ID);
                key_flag = true;
            }
            break;
        case WM_COMMAND:
            key_flag = false;
            SetFocus(hWnd);
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_F1:
                case VK_F2:
                case VK_LEFT:
                case VK_UP:
                case VK_RIGHT:
                case VK_DOWN:
                    u8g_key_value = (u8)wParam;
                    key_flag = true;
                    break;
                default:
                    break;
            }
            break ;
        case WM_KEYUP:
            key_flag = false;
            break ;
        case WM_CHAR:
            switch(wParam)
            {
                case '\r':          //ȷ��
                case '\x1b':        //ȡ��
                    key_flag = true;
                    u8g_key_value = (u8)wParam;
                    break;
                default:
                    break;
            }
            break ;
        case WM_MOVE:
            if(move_count < 1)
            {
                move_count++;
                break;
            }
            move_count = 3;
            MoveWindow(tg_keyboard_wnd, tg_WindowRect.left+s32g_window_width +5,
                       tg_WindowRect.top, cn_key_board_width, cn_key_board_height,
                       true);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//----����Ӳ��ɨ��------------------------------------------------------------
//����: ��4����,�ɶ��븴�ϼ������2��������,key0=gpf0,
//      key1=gpf2,key2=gpg3,key3=gpg11
//����: key,��ֵ����ָ��,�ɴ��2������ֵ
//����: �����ļ��ĸ���0,1,2
//----------------------------------------------------------------------------
u32 keyboard_scan(void)
{
    if(key_flag)
    {
        return (u32)u8g_key_value;
    }
    else
        return 0;
}

//----��ʼ������ģ��-----------------------------------------------------------
//����: ��ʼ��һ����windows�ļ��̺Ͱ�ťģ��ļ��̣��ü��̹�8������
//����: ��
//����: ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_KeyBoardWindows(ptu32_t para)
{
    static struct tagKeyBoardPrivate windows_keyboard;
    key_hard_init();
    windows_keyboard.read_keyboard = keyboard_scan;
    Keyboard_InstallDevice("sim keyboard",&windows_keyboard);
    windows_keyboard.vtime_limit = para;
    windows_keyboard.vtime_count = 0;
    windows_keyboard.key_bak = 0;
    windows_keyboard.key_now = 0;
    return 1;
}


