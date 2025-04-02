#include <Engine/Platform/Windows/Graphics/Win32_D3D9Context.hpp>

#include <d3d9.h>

#include <Engine/Backend/D3D9/D3D9_Backend.hpp>

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

    bool Win32D3D9Context::Create() {
        // Create Direct3D interface
        h_D3D9Ctx = Direct3DCreate9(D3D_SDK_VERSION);
        if (!h_D3D9Ctx) {
            printf("Win32D3D9Context: Failed to create D3D9 context\n");
            return false;
        }

        D3DPRESENT_PARAMETERS d3dpp = GetDefaultD3DPresentParams(GetHwnd());

        // create the D3D device
        HRESULT hr = h_D3D9Ctx->CreateDevice(
                D3DADAPTER_DEFAULT,
                D3DDEVTYPE_HAL,
                GetHwnd(),
                D3DCREATE_HARDWARE_VERTEXPROCESSING,
                &d3dpp,
                &h_D3D9Device
        );

        if (FAILED(hr)) {
            printf("Win32D3D9Context: Failed to create D3D9 device.\n");
            h_D3D9Ctx->Release();
            h_D3D9Ctx = nullptr;
            return false;
        }

        return true;
    }

    void Win32D3D9Context::Bind() {
        if (!h_D3D9Device) {
            printf("Win32D3D9Context: Cannot bind: D3D9 device is not initialized.\n");
            return;
        }

        // no need to bind; the context is associated with the device
    }

    void Win32D3D9Context::Discard() {
        // no need to unbind; the context is associated with the device
    }

    void Win32D3D9Context::PreRender() {
        if (!h_D3D9Device) {
            printf("Win32D3D9Context: D3D9 device is not initialized.\n");
            return;
        }

        if(!h_D3D9Surface) {
            h_D3D9Device->GetRenderTarget(0, &h_D3D9Surface);
        }

        h_D3D9Device->BeginScene();
        h_D3D9Device->SetRenderTarget(0, h_D3D9Surface);
    }

    void Win32D3D9Context::PostRender() {
        if (!h_D3D9Device) {
            printf("Win32D3D9Context: D3D9 device is not initialized.\n");
            return;
        }

        h_D3D9Device->EndScene();
    }

    void Win32D3D9Context::Destroy() {
        if (h_D3D9Device) {
            h_D3D9Device->Release();
            h_D3D9Device = nullptr;
        }

        if (h_D3D9Ctx) {
            h_D3D9Ctx->Release();
            h_D3D9Ctx = nullptr;
        }
    }

    void Win32D3D9Context::Present() {
        if (!h_D3D9Device) {
            printf("Win32D3D9Context: Cannot swap buffers: D3D9 device is not initialized.\n");
            return;
        }

        HRESULT hr = h_D3D9Device->Present(nullptr, nullptr, nullptr, nullptr);

        if (FAILED(hr)) {
            printf("Win32D3D9Context: Failed to present the frame: %x\n", hr);
        }
    }

    core::runtime::graphics::IGraphicsBackend *Win32D3D9Context::GetBackend() {
        if (!m_Backend) {
            m_Backend = std::make_unique<backend::dx9::D3D9Backend>(h_D3D9Device);

            if(!m_Backend->Initialize()) {
                printf("Win32D3D9Context: Failed to initialize the backend.\n");
                return nullptr;
            }
        }

        return m_Backend.get();
    }

    void Win32D3D9Context::Resize(core::math::Vector2 size) {
        D3DPRESENT_PARAMETERS d3dpp = GetDefaultD3DPresentParams(GetHwnd());

        d3dpp.BackBufferWidth = (unsigned int) size.x;
        d3dpp.BackBufferHeight = (unsigned int) size.y;

        h_D3D9Device->Reset(&d3dpp);
    }
}