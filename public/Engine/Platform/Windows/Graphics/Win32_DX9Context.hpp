#pragma once

#include <Windows.h>

#include <Engine/Core/Runtime/Graphics/IGraphicsContext.hpp>

// DX9 API Forward Definitions
struct IDirect3D9;
struct IDirect3DDevice9;

namespace engine::platform::win32 {
    struct Win32DX9Context : public core::runtime::graphics::IGraphicsContext {
        Win32DX9Context(HWND win) : h_Window{win} {}

        bool Create() override;

        void Bind() override;

        void Discard() override;

        void Destroy() override;

        void Present() override;

        core::runtime::graphics::IGraphicsBackend *GetBackend() override;

        void PreRender() override;
        void PostRender() override;

    protected:
        IDirect3D9 *h_DX9Ctx;
        IDirect3DDevice9 *h_DX9Device;
        std::unique_ptr<core::runtime::graphics::IGraphicsBackend> m_Backend;
        HWND h_Window;
    };
}