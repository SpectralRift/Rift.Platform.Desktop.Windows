#pragma once

#include <Engine/Core/Runtime/Input/IInputDevice.hpp>
#include <Engine/Platform/Windows/Win32_Window.hpp>

namespace engine::platform::win32 {
    struct Win32KeyboardDevice : public core::runtime::input::IInputDevice {
        Win32KeyboardDevice(Win32Window *window) : m_Window{window} {

        }

        bool Initialize() override;

        void Destroy() override;

        void Poll() override;

        std::string GetName() const override {
            return "Win32 Native Keyboard Device";
        }

        core::runtime::input::InputDeviceType GetType() const override {
            return core::runtime::input::InputDeviceType::INPUT_DEVICE_TYPE_KEYBOARD;
        }

        int GetPlayerId() override {
            return 0;
        }

        LRESULT WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:
        Win32Window *m_Window;
    };
}