#define SHOW_PRIVATE_API

#include <Engine/Platform/Windows/Input/Win32_KeyboardDevice.hpp>
#include <Engine/Input/InputManager.hpp>

// import FNV hashing for input name hashing
#include <Engine/Core/Hashing/FNV.hpp>

namespace engine::platform::win32 {
    bool Win32KeyboardDevice::Initialize() {
        return true;
    }

    void Win32KeyboardDevice::Destroy() {

    }

    void Win32KeyboardDevice::Poll() {

    }

    LRESULT Win32KeyboardDevice::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
            case WM_CHAR:
                input::InputManager::Instance()->PushInputChar(wParam);
                break;
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP: {
                input::InputKeyHandle keyHandle{0};

                switch (wParam) {
                    // modifier keys
                    case VK_LCONTROL: keyHandle = engine::FNVConstHash("Key_LeftCtrl"); break;
                    case VK_RCONTROL: keyHandle = engine::FNVConstHash("Key_RightCtrl"); break;
                    case VK_LSHIFT: keyHandle = engine::FNVConstHash("Key_LeftShift"); break;
                    case VK_RSHIFT: keyHandle = engine::FNVConstHash("Key_RightShift"); break;
                    case VK_LMENU: keyHandle = engine::FNVConstHash("Key_LeftAlt"); break;
                    case VK_RMENU: keyHandle = engine::FNVConstHash("Key_RightAlt"); break;
                    case VK_LWIN: keyHandle = engine::FNVConstHash("Key_LeftSuper"); break;
                    case VK_RWIN: keyHandle = engine::FNVConstHash("Key_RightSuper"); break;

                    // other special keys
                    case VK_SPACE: keyHandle = engine::FNVConstHash("Key_Space"); break;
                    case VK_RETURN: keyHandle = engine::FNVConstHash("Key_Enter"); break;
                    case VK_ESCAPE: keyHandle = engine::FNVConstHash("Key_Escape"); break;
                    case VK_BACK: keyHandle = engine::FNVConstHash("Key_Backspace"); break;
                    case VK_TAB: keyHandle = engine::FNVConstHash("Key_Tab"); break;
                    case VK_INSERT: keyHandle = engine::FNVConstHash("Key_Insert"); break;
                    case VK_DELETE: keyHandle = engine::FNVConstHash("Key_Delete"); break;
                    case VK_HOME: keyHandle = engine::FNVConstHash("Key_Home"); break;
                    case VK_END: keyHandle = engine::FNVConstHash("Key_End"); break;
                    case VK_PRIOR: keyHandle = engine::FNVConstHash("Key_PageUp"); break;
                    case VK_NEXT: keyHandle = engine::FNVConstHash("Key_PageDown"); break;
                    case VK_UP: keyHandle = engine::FNVConstHash("Key_ArrowUp"); break;
                    case VK_DOWN: keyHandle = engine::FNVConstHash("Key_ArrowDown"); break;
                    case VK_LEFT: keyHandle = engine::FNVConstHash("Key_ArrowLeft"); break;
                    case VK_RIGHT: keyHandle = engine::FNVConstHash("Key_ArrowRight"); break;
                    case VK_CAPITAL: keyHandle = engine::FNVConstHash("Key_CapsLock"); break;
                    case VK_SCROLL: keyHandle = engine::FNVConstHash("Key_ScrollLock"); break;
                    case VK_NUMLOCK: keyHandle = engine::FNVConstHash("Key_NumLock"); break;
                    case VK_SNAPSHOT: keyHandle = engine::FNVConstHash("Key_PrintScreen"); break;
                    case VK_PAUSE: keyHandle = engine::FNVConstHash("Key_Pause"); break;

                    case VK_OEM_7: keyHandle = engine::FNVConstHash("Key_Apostrophe"); break;

                    default:
                        // handle letter and numeric keys
                        if ((wParam >= 'A' && wParam <= 'Z') || (wParam >= '0' && wParam <= '9')) {
                            keyHandle = engine::FNVConstHash(std::string("Key_") + static_cast<char>(wParam));
                        } else if(wParam >= VK_F1 && wParam <= VK_F24) {
                            keyHandle = engine::FNVConstHash(std::string("Key_") + std::to_string(wParam - VK_F1 + 1));
                        }
                        break;
                }

                if(keyHandle != 0) {
                    input::InputManager::Instance()->PushKeyStateChange(keyHandle, (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN));
                }
            } break;

            default:
                break;
        }

        return 1;
    }
}