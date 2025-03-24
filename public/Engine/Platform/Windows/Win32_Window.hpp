#pragma once

#include <Windows.h>

#include <string>
#include <stdexcept>

#include <Engine/Core/Runtime/IWindow.hpp>

namespace engine::platform::win32 {
    struct Win32MouseDevice;
    struct Win32KeyboardDevice;

    struct Win32Window : public core::runtime::IWindow {
        Win32Window() : h_Window(nullptr), h_DevContext(nullptr), b_IsQuit(false) {}

        bool Create() override;

        void Destroy() override;

        void SetTitle(std::string_view title) override;

//        void setIcon(const core::graphics::Bitmap& icon) override;

        bool IsValid() override;

        void SetPosition(const core::math::Vector2 &position) override;

        void SetSize(const core::math::Vector2 &size) override;

        core::math::Vector2 GetPosition() override;

        core::math::Vector2 GetSize() override;

        void ProcessEvents() override;

        void Show() override;

        void Hide() override;

        void *GetHandle() override {
            return h_Window;
        }

        void *GetDeviceCtx() override {
            return h_DevContext;
        }

        core::runtime::graphics::IGraphicsContext *GetGraphicsContext() override {
            return h_GraphicsContext.get();
        }
    protected:
        HWND h_Window;
        HDC h_DevContext;

        std::unique_ptr<core::runtime::graphics::IGraphicsContext> h_GraphicsContext;
        std::shared_ptr<Win32MouseDevice> m_MouseDevice;
        std::shared_ptr<Win32KeyboardDevice> m_KeyboardDevice;

        static bool b_IsWinClassRegistered;
        bool b_IsQuit;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}