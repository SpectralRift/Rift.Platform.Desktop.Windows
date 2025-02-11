#define SHOW_PRIVATE_API

#include <Engine/Platform/Windows/Input/Win32_MouseDevice.hpp>
#include <Engine/Core/Runtime/Input/InputManager.hpp>
#include <Engine/Core/Runtime/Input/InputEvent.hpp>

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
            core::runtime::input::InputManager::Instance()->PushEvent(
                    new core::runtime::input::MouseMoveEvent(
                            this,
                            {
                                    (float) LOWORD(lParam),
                                    (float) HIWORD(lParam)
                            }
                    )
            );

            return 0;
        } else {
            switch (uMsg) {
                case WM_LBUTTONDOWN:
                    core::runtime::input::InputManager::Instance()->PushEvent(
                            new core::runtime::input::MouseButtonDownEvent(
                                    this,
                                    core::runtime::input::INPUT_DEVICE_BUTTON_MOUSE_LEFT
                            )
                    );
                    goto SET_MOUSE_POS;
                case WM_LBUTTONUP:
                    core::runtime::input::InputManager::Instance()->PushEvent(
                            new core::runtime::input::MouseButtonUpEvent(
                                    this,
                                    core::runtime::input::INPUT_DEVICE_BUTTON_MOUSE_LEFT
                            )
                    );
                    goto SET_MOUSE_POS;
                case WM_MBUTTONDOWN:
                    core::runtime::input::InputManager::Instance()->PushEvent(
                            new core::runtime::input::MouseButtonDownEvent(
                                    this,
                                    core::runtime::input::INPUT_DEVICE_BUTTON_MOUSE_MIDDLE
                            )
                    );
                    goto SET_MOUSE_POS;
                case WM_MBUTTONUP:
                    core::runtime::input::InputManager::Instance()->PushEvent(
                            new core::runtime::input::MouseButtonUpEvent(
                                    this,
                                    core::runtime::input::INPUT_DEVICE_BUTTON_MOUSE_MIDDLE
                            )
                    );
                    goto SET_MOUSE_POS;
                case WM_RBUTTONDOWN:
                    core::runtime::input::InputManager::Instance()->PushEvent(
                            new core::runtime::input::MouseButtonDownEvent(
                                    this,
                                    core::runtime::input::INPUT_DEVICE_BUTTON_MOUSE_RIGHT
                            )
                    );
                    goto SET_MOUSE_POS;
                case WM_RBUTTONUP:
                    core::runtime::input::InputManager::Instance()->PushEvent(
                            new core::runtime::input::MouseButtonUpEvent(
                                    this,
                                    core::runtime::input::INPUT_DEVICE_BUTTON_MOUSE_RIGHT
                            )
                    );
                    goto SET_MOUSE_POS;
                default:
                    break;
            }
        }

        return 1;
    }
}