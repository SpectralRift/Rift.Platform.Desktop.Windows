#include <Engine/Platform/Windows/Win32_Window.hpp>

#include <Engine/Platform/Windows/Graphics/Win32_GLContext.hpp>
#include <Engine/Platform/Windows/Graphics/Win32_DX9Context.hpp>

#include <Engine/Platform/Windows/Input/Win32_MouseDevice.hpp>
#include <Engine/Platform/Windows/Input/Win32_KeyboardDevice.hpp>

#include <Engine/Input/InputManager.hpp>

namespace engine::platform::win32 {
    bool Win32Window::b_IsWinClassRegistered{false};

    bool Win32Window::Create() {
        const char *className = "RiftWin32";

        if (!b_IsWinClassRegistered) {
            WNDCLASS wc = {};
            wc.lpfnWndProc = WindowProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpszClassName = className;

            RegisterClass(&wc);
        }

        h_Window = CreateWindowEx(
                0,
                className,
                "EngineWindow",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                nullptr,
                nullptr,
                GetModuleHandle(NULL),
                this
        );

        if (h_Window == nullptr) {
            return false;
        }

        // set window user pointer to this
        SetWindowLongA(h_Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

//        h_GraphicsContext = std::make_unique<Win32DX9Context>(this);
        h_GraphicsContext = std::make_unique<Win32GLContext>(this);
//        h_GraphicsContext = std::make_unique<platform::universal::UEGLContext>(this);

        h_DevContext = GetDC(h_Window);

        if (!h_GraphicsContext->Create()) {
            printf("Win32Window: Failed to create the graphical context!\n");
            return false;
        }

        m_MouseDevice = std::make_shared<Win32MouseDevice>(this);
        m_KeyboardDevice = std::make_shared<Win32KeyboardDevice>(this);

        input::InputManager::Instance()->RegisterDevice(m_MouseDevice.get());
        input::InputManager::Instance()->RegisterDevice(m_KeyboardDevice.get());

        b_IsQuit = false;

        return true;
    }

    void Win32Window::Destroy() {
        if (m_MouseDevice) {
            input::InputManager::Instance()->UnregisterDevice(m_MouseDevice.get());
        }

        if (m_KeyboardDevice) {
            input::InputManager::Instance()->UnregisterDevice(m_KeyboardDevice.get());
        }

        if (h_GraphicsContext) {
            h_GraphicsContext->Destroy();
        }

        if (h_Window) {
            DestroyWindow(h_Window);
            h_Window = nullptr;
        }

        b_IsQuit = true;
    }

    void Win32Window::SetTitle(std::string_view title) {
        if (h_Window) {
            SetWindowText(h_Window, title.data());
        }
    }

    void Win32Window::SetPosition(const core::math::Vector2 &position) {
        if (h_Window) {
            SetWindowPos(h_Window, nullptr, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
    }

    void Win32Window::SetSize(const core::math::Vector2 &size) {
        if (h_Window) {
            SetWindowPos(h_Window, nullptr, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOZORDER);
        }
    }

    bool Win32Window::IsValid() {
        return h_Window != nullptr && !b_IsQuit;
    }

    core::math::Vector2 Win32Window::GetPosition() {
        RECT rect = {};

        if (h_Window && GetWindowRect(h_Window, &rect)) {
            return {(float) rect.left, (float) rect.top};
        }

        return {};
    }

    core::math::Vector2 Win32Window::GetSize() {
        RECT rect = {};

        if (h_Window && GetClientRect(h_Window, &rect)) {
            return {(float) (rect.right - rect.left), (float) (rect.bottom - rect.top)};
        }

        return {};
    }

    void Win32Window::ProcessEvents() {
        MSG msg = {};

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                b_IsQuit = true;
            }
        }
    }

    void Win32Window::Show() {
        if (h_Window) {
            ShowWindow(h_Window, SW_SHOW);
        }
    }

    void Win32Window::Hide() {
        if (h_Window) {
            ShowWindow(h_Window, SW_HIDE);
        }
    }

    LRESULT CALLBACK Win32Window::WindowProc(HWND h_Window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        auto window = reinterpret_cast<Win32Window *>(GetWindowLongPtr(h_Window, GWLP_USERDATA));

        if (window) {
            if (uMsg == WM_SIZE) {
                auto dxCtx = dynamic_cast<Win32DX9Context *>(window->h_GraphicsContext.get());

                if (dxCtx) {
                    dxCtx->Resize({(float) LOWORD(lParam), (float) HIWORD(lParam)});
                    return 0;
                }
            }

            switch (uMsg) {
                case WM_DESTROY:
                case WM_CLOSE:
                    PostQuitMessage(0);
                    return 0;
                default:
                    if (window->m_MouseDevice->WinProc(h_Window, uMsg, wParam, lParam) == 0) {
                        return 0;
                    }

                    if (window->m_KeyboardDevice->WinProc(h_Window, uMsg, wParam, lParam) == 0) {
                        return 0;
                    }

                    break;
            }
        }

        return DefWindowProc(h_Window, uMsg, wParam, lParam);
    }
}