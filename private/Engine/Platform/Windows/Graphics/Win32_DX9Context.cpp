#include <Engine/Platform/Windows/Graphics/Win32_DX9Context.hpp>

#include <d3d9.h>

#include <Engine/Backend/DX9/DX9_Backend.hpp>

namespace engine::platform::win32 {
    bool Win32DX9Context::Create() {
        // Create Direct3D interface
        h_DX9Ctx = Direct3DCreate9(D3D_SDK_VERSION);
        if (!h_DX9Ctx) {
            printf("Win32DX9Context: Failed to create Direct3D9 context\n");
            return false;
        }

        // Set up the structure used to create the D3D device
        D3DPRESENT_PARAMETERS d3dpp = {};
        d3dpp.Windowed = true; // Run in windowed mode
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Discard old frames
        d3dpp.hDeviceWindow = h_Window;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        d3dpp.BackBufferCount = 1;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

        // Create the Direct3D device
        HRESULT hr = h_DX9Ctx->CreateDevice(
                D3DADAPTER_DEFAULT,
                D3DDEVTYPE_HAL,
                h_Window,
                D3DCREATE_HARDWARE_VERTEXPROCESSING,
                &d3dpp,
                &h_DX9Device
        );

        if (FAILED(hr)) {
            printf("Win32DX9Context: Failed to create Direct3D9 device.\n");
            h_DX9Ctx->Release();
            h_DX9Ctx = nullptr;
            return false;
        }

        return true;
    }

    void Win32DX9Context::Bind() {
        if (!h_DX9Device) {
            printf("Win32DX9Context: Cannot bind: Direct3D9 device is not initialized.\n");
            return;
        }

        // no need to bind; it is automatically associated to the right device
    }

    void Win32DX9Context::Discard() {

    }

    void Win32DX9Context::PreRender() {
        if (!h_DX9Device) {
            printf("Win32DX9Context: Direct3D9 device is not initialized.\n");
            return;
        }

        h_DX9Device->BeginScene();
    }

    void Win32DX9Context::PostRender() {
        if (!h_DX9Device) {
            printf("Win32DX9Context: Direct3D9 device is not initialized.\n");
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
            printf("Win32DX9Context: Cannot swap buffers: Direct3D9 device is not initialized.\n");
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
}