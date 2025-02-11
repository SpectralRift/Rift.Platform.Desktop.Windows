#pragma once

#include <Engine/Core/Runtime/Input/IInputDevice.hpp>
#include <Engine/Platform/Windows/Win32_Window.hpp>

namespace engine::platform::win32 {
    struct Win32MouseDevice : public core::runtime::input::IAxisDevice {
        Win32MouseDevice(Win32Window *window) : m_Window{window}, m_MousePosition{}, m_StateMouseLeft{false},
                                                m_StateMouseRight{false}, m_StateMouseMiddle{false} {

        }

        bool Initialize() override;

        void Destroy() override;

        void Poll() override;

        std::string GetName() const override {
            return "Win32 Native Mouse Device";
        }

        core::runtime::input::InputDeviceType GetType() const override {
            return core::runtime::input::InputDeviceType::INPUT_DEVICE_TYPE_MOUSE;
        }

        int GetPlayerId() override {
            return 0;
        }

        LRESULT WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:
        Win32Window *m_Window;
        core::math::Vector2 m_MousePosition;
        bool m_StateMouseLeft;
        bool m_StateMouseRight;
        bool m_StateMouseMiddle;
    };
}