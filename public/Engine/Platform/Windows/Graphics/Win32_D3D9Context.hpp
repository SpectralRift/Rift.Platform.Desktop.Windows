#pragma once

#include <Windows.h>

#include <Engine/Core/Runtime/Graphics/IGraphicsContext.hpp>
#include <Engine/Platform/Windows/Win32_Window.hpp>

// D3D9 API Forward Definitions
struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;

namespace engine::platform::win32 {
    struct Win32D3D9Context : public core::runtime::graphics::IGraphicsContext {
        Win32D3D9Context(Win32Window* win) : m_OwnerWindow{win}, h_D3D9Surface{nullptr}, h_D3D9Device{nullptr} {}

        bool Create() override;

        void Bind() override;

        void Discard() override;

        void Destroy() override;

        void Present() override;

        core::runtime::graphics::IGraphicsBackend *GetBackend() override;

        core::runtime::IWindow *GetOwnerWindow() override {
            return m_OwnerWindow;
        }

        void PreRender() override;

        void PostRender() override;

        void Resize(core::math::Vector2 size);

    protected:
        HWND GetHwnd() { return (HWND) m_OwnerWindow->GetHandle(); }

        IDirect3D9 *h_D3D9Ctx;
        IDirect3DDevice9 *h_D3D9Device;
        IDirect3DSurface9 *h_D3D9Surface;

        std::unique_ptr<core::runtime::graphics::IGraphicsBackend> m_Backend;
        Win32Window* m_OwnerWindow;
    };
}