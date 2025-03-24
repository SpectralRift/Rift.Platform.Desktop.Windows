#include <Engine/Platform/Windows/Graphics/Win32_GLContext.hpp>

#include <cstdio>
#include <system_error>

#include <Engine/Backend/OpenGL/GL_Backend.hpp>

namespace engine::platform::win32 {
    bool Win32GLContext::Create() {
        h_DeviceCtx = GetDC(GetHwnd());

        PIXELFORMATDESCRIPTOR pfd{};

        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;

        int pixelFormat = ChoosePixelFormat(h_DeviceCtx, &pfd);
        if (!SetPixelFormat(h_DeviceCtx, pixelFormat, &pfd)) {
            printf("Win32GLContext: SetPixelFormat failed: %s\n", std::system_category().message(GetLastError()).c_str());
            return false;
        }

        h_GlCtx = wglCreateContext(h_DeviceCtx);
        if (!h_GlCtx) {
            printf("Win32GLContext: wglCreateContext failed: %s\n", std::system_category().message(GetLastError()).c_str());
            Destroy();
            return false;
        }

        return true;
    }

    void Win32GLContext::Bind() {
        if (GetHwnd() != nullptr && h_GlCtx) {
            wglMakeCurrent(h_DeviceCtx, h_GlCtx);
        }
    }

    void Win32GLContext::Discard() {
        wglMakeCurrent(nullptr, nullptr);
    }

    void Win32GLContext::Destroy() {
        if (h_GlCtx) {
            wglDeleteContext(h_GlCtx);
            h_GlCtx = nullptr;
        }

        if (GetHwnd() && h_DeviceCtx) {
            ReleaseDC(GetHwnd(), h_DeviceCtx);
            h_DeviceCtx = nullptr;
        }
    }

    void Win32GLContext::Present() {
        if (h_DeviceCtx) {
            wglSwapLayerBuffers(h_DeviceCtx, WGL_SWAP_MAIN_PLANE);
        }
    }

    core::runtime::graphics::IGraphicsBackend *Win32GLContext::GetBackend() {
        if (!m_Backend) {
            m_Backend = std::make_unique<backend::ogl::GLBackend>();

            Bind();
            if(!m_Backend->Initialize()) {
                printf("Win32GLContext: Failed to initialize the backend.\n");
                return nullptr;
            }
            Discard();
        }

        return m_Backend.get();
    }
}