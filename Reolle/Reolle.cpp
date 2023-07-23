#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <string>
#include <conio.h>
#include <stdio.h>

HWND hwnd; // 全局变量

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
        //窗口出现出现问题的时候，重新刷新窗口
    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HHOOK keyboardHook = NULL;
bool toggle = false;
bool isKeyDown = false; // 新增变量

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN && !isKeyDown) // 检查按键状态
        {
            if (p->vkCode == VK_OEM_3) // ~ key
            {
                int cx = GetSystemMetrics(SM_CXFULLSCREEN);
                int cy = GetSystemMetrics(SM_CYFULLSCREEN);
                toggle = !toggle;
                if (toggle)
                    SetWindowPos(hwnd, NULL, 300, 100, 840, 640, SWP_SHOWWINDOW);
                else
                    SetWindowPos(hwnd, NULL, 300, 100, 840, 640, SWP_HIDEWINDOW);
                isKeyDown = true; // 设置按键状态
            }
        }
        else if (wParam == WM_KEYUP && p->vkCode == VK_OEM_3) // 检查按键弹起
        {
            isKeyDown = false; // 设置按键状态
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

// WinMain函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 注册窗口类
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"SampleWindowClass";
    RegisterClassEx(&wc);
    HWND Minecraft_hwnd = FindWindowA(NULL, "Minecraft");
    // 创建窗口
    hwnd = CreateWindowEx(0,L"SampleWindowClass",L"",WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,840,640,Minecraft_hwnd,NULL,hInstance,NULL);
    // 显示窗口
    ShowWindow(hwnd, nCmdShow);

    // 主程序
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);

    SetWindowPos(hwnd, NULL, 100, 100, 840, 640, SWP_HIDEWINDOW);

    bool isPressed = false;
    bool win_Bool = true;
    RECT rect;

    // 进入消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        //GetWindowRect(hwnd,&rect);oo
        //SetLayeredWindowAttributes(hwnd, 0, 128, LWA_ALPHA);
        //要使窗口中所有白色像素变为透明，可以这样调用：
        //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
    }

    UnhookWindowsHookEx(keyboardHook);

    return 0;
}