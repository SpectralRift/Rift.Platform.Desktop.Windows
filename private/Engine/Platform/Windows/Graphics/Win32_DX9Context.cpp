#include <Engine/Platform/Windows/Graphics/Win32_DX9Context.hpp>

#include <d3d9.h>

#include <Engine/Backend/DX9/DX9_Backend.hpp>

namespace engine::platform::win32 {
    D3DPRESENT_PARAMETERS GetDefaultD3DPresentParams(HWND hwnd) {
        D3DPRESENT_PARAMETERS d3dpp = {};

        d3dpp.Windowed = true; // run in windowed mode
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = hwnd;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        d3dpp.BackBufferCount = 1;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

        return d3dpp;
    }

    bool Win32DX9Context::Create() {
        // Create Direct3D interface
        h_DX9Ctx = Direct3DCreate9(D3D_SDK_VERSION);
        if (!h_DX9Ctx) {
            printf("Win32DX9Context: Failed to create D3D9 context\n");
            return false;
        }

        D3DPRESENT_PARAMETERS d3dpp = GetDefaultD3DPresentParams(GetHwnd());

        // create the D3D device
        HRESULT hr = h_DX9Ctx->CreateDevice(
                D3DADAPTER_DEFAULT,
                D3DDEVTYPE_HAL,
                GetHwnd(),
                D3DCREATE_HARDWARE_VERTEXPROCESSING,
                &d3dpp,
                &h_DX9Device
        );

        if (FAILED(hr)) {
            printf("Win32DX9Context: Failed to create D3D9 device.\n");
            h_DX9Ctx->Release();
            h_DX9Ctx = nullptr;
            return false;
        }

        return true;
    }

    void Win32DX9Context::Bind() {
        if (!h_DX9Device) {
            printf("Win32DX9Context: Cannot bind: D3D9 device is not initialized.\n");
            return;
        }

        // no need to bind; the context is associated with the device
    }

    void Win32DX9Context::Discard() {
        // no need to unbind; the context is associated with the device
    }

    void Win32DX9Context::PreRender() {
        if (!h_DX9Device) {
            printf("Win32DX9Context: D3D9 device is not initialized.\n");
            return;
        }

        if(!h_DX9Surface) {
            h_DX9Device->GetRenderTarget(0, &h_DX9Surface);
        }

        h_DX9Device->BeginScene();
        h_DX9Device->SetRenderTarget(0, h_DX9Surface);
    }

    void Win32DX9Context::PostRender() {
        if (!h_DX9Device) {
            printf("Win32DX9Context: D3D9 device is not initialized.\n");
            return;
        }

        h_DX9Device->EndScene();
    }

    void Win32DX9Context::Destroy() {
        if (h_DX9Device) {
            h_DX9Device->Release();
            h_DX9Device = nullptr;
        }

        if (h_DX9Ctx) {
            h_DX9Ctx->Release();
            h_DX9Ctx = nullptr;
        }
    }

    void Win32DX9Context::Present() {
        if (!h_DX9Device) {
            printf("Win32DX9Context: Cannot swap buffers: D3D9 device is not initialized.\n");
            return;
        }

        HRESULT hr = h_DX9Device->Present(nullptr, nullptr, nullptr, nullptr);

        if (FAILED(hr)) {
            printf("Win32DX9Context: Failed to present the frame: %x\n", hr);
        }
    }

    core::runtime::graphics::IGraphicsBackend *Win32DX9Context::GetBackend() {
        if (!m_Backend) {
            m_Backend = std::make_unique<backend::dx9::DX9Backend>(h_DX9Device);

            if(!m_Backend->Initialize()) {
                printf("Win32DX9Context: Failed to initialize the backend.\n");
                return nullptr;
            }
        }

        return m_Backend.get();
    }

    void Win32DX9Context::Resize(core::math::Vector2 size) {
        D3DPRESENT_PARAMETERS d3dpp = GetDefaultD3DPresentParams(GetHwnd());

        d3dpp.BackBufferWidth = (unsigned int) size.x;
        d3dpp.BackBufferHeight = (unsigned int) size.y;

        h_DX9Device->Reset(&d3dpp);
    }
}