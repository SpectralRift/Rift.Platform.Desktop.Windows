#define SHOW_PRIVATE_API

#include <Engine/Platform/Windows/Input/Win32_MouseDevice.hpp>
#include <Engine/Input/InputManager.hpp>

// import FNV hashing for input name hashing
#include <Engine/Core/Hashing/FNV.hpp>

namespace engine::platform::win32 {
    bool Win32MouseDevice::Initialize() {
        return true;
    }

    void Win32MouseDevice::Destroy() {

    }

    void Win32MouseDevice::Poll() {

    }

    LRESULT Win32MouseDevice::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_MOUSEMOVE) {
            SET_MOUSE_POS:
            input::InputManager::Instance()->PushMousePosition({(float) LOWORD(lParam), (float) HIWORD(lParam)});
            return 0;
        } else {
            switch (uMsg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    input::InputManager::Instance()->PushKeyStateChange(engine::FNVConstHash("Mouse_Left"), uMsg == WM_LBUTTONDOWN);
                    goto SET_MOUSE_POS;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    input::InputManager::Instance()->PushKeyStateChange(engine::FNVConstHash("Mouse_Middle"), uMsg == WM_MBUTTONDOWN);
                    goto SET_MOUSE_POS;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    input::InputManager::Instance()->PushKeyStateChange(engine::FNVConstHash("Mouse_Right"), uMsg == WM_RBUTTONDOWN);
                    goto SET_MOUSE_POS;
                default:
                    break;
            }
        }

        return 1;
    }
}