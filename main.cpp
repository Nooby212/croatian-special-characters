#pragma comment(lib, "user32.lib")
#define ushort unsigned short
#include <iostream>
#include <string>
#include <Windows.h>

static const ushort characters[10] = {0x010C, 0x010D, 0x0106, 0x0107, 0x0160, 0x0161, 0x0110, 0x0111, 0x017D, 0x017E};
MSG msg;
static ushort counter = 0;
HHOOK hHook = NULL;
/*
 *      Č = U+010C | \U0000010C
 *      č = U+010D | \U0000010D
 *
 *      Ć = U+0106 | \U00000106
 *      ć = U+0107 | \U00000107
 *
 *      Š = U+0160 | \U00000160
 *      š = U+0161 | \U00000161
 *
 *      Đ = U+0110 | \U00000110
 *      đ = U+0111 | \U00000111
 *
 *      Ž = U+017D | \U0000017D
 *      ž = U+017E | \U0000017E
 *
 *      0x010C, 0x010D, 0x0106, 0x0107, 0x0160, 0x0161, 0x0110, 0x0111, 0x017D, 0x017E
 */

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* kbd =(KBDLLHOOKSTRUCT*)lParam;
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        if (GetAsyncKeyState(VK_F1) && GetAsyncKeyState(VK_F1) & 0x8000) {
            if (kbd->vkCode == 0x43) {
                INPUT inputs[4] = {0};

                inputs[0].type = INPUT_KEYBOARD;
                inputs[0].ki.wVk = VK_BACK;

                inputs[1].type = INPUT_KEYBOARD;
                inputs[1].ki.wVk = VK_BACK;
                inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

                inputs[2].type = INPUT_KEYBOARD;
                inputs[2].ki.wScan = characters[counter%10];
                inputs[2].ki.dwFlags = KEYEVENTF_UNICODE;

                inputs[3].type = INPUT_KEYBOARD;
                inputs[3].ki.wScan = characters[counter%10];
                inputs[3].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

                SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

                counter++;
                return 1;
            }
        }
        if (kbd->vkCode == VK_RETURN) counter = 0;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam) ;
}

int main() {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0) ;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}