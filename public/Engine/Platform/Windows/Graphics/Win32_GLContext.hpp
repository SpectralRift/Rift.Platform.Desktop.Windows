#pragma once

#include <Windows.h>

#include <Engine/Core/Runtime/Graphics/IGraphicsContext.hpp>
#include <Engine/Platform/Windows/Win32_Window.hpp>

namespace engine::platform::win32 {
    struct Win32GLContext : public core::runtime::graphics::IGraphicsContext {
        Win32GLContext(Win32Window* win) : m_OwnerWindow{win} {}

        bool Create() override;

        void Bind() override;

        void Discard() override;

        void Destroy() override;

        void Present() override;

        core::runtime::graphics::IGraphicsBackend *GetBackend() override;

        core::runtime::IWindow *GetOwnerWindow() override {
            return m_OwnerWindow;
        }
    protected:
        HWND GetHwnd() { return (HWND) m_OwnerWindow->GetHandle(); }

        HGLRC h_GlCtx;
        std::unique_ptr<core::runtime::graphics::IGraphicsBackend> m_Backend;
        Win32Window* m_OwnerWindow;
        HDC h_DeviceCtx;
    };
}