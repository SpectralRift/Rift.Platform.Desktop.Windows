#include <Windows.h>

// undefine CreateMutex to avoid naming conflicts
#ifdef CreateMutex
#undef CreateMutex
#endif

#include <Engine/Core/Platform.hpp>
#include <Engine/Core/Runtime/DesktopHost.hpp>

#include <Engine/Platform/Windows/Win32_Window.hpp>
#include <Engine/Platform/Windows/Win32_Thread.hpp>
#include <Engine/Platform/Windows/Win32_Mutex.hpp>

namespace engine::core::Platform {
    std::string GetName() {
        return "Win32";
    }

    void ShowMessageBox(std::string_view title, std::string_view message) {
        MessageBoxA(nullptr, message.data(), title.data(), MB_OK);
    }

    std::unique_ptr<runtime::AppHost> GetSuitableHost() {
        return std::make_unique<runtime::DesktopHost>();
    }

    std::unique_ptr<runtime::IWindow> CreateAppWindow() {
        return std::make_unique<platform::win32::Win32Window>();
    }

    std::unique_ptr<runtime::IThread> CreateThread() {
        return std::make_unique<platform::win32::Win32Thread>();
    }

    std::unique_ptr<runtime::IMutex> CreateMutex() {
        return std::make_unique<platform::win32::Win32Mutex>();
    }

    std::shared_ptr<input::IVirtualKeyboard> GetVirtualKeyboard() {
        return nullptr;
    }

    void Init() {

    }

    void PreEngineInit() {

    }

    void Shutdown() {

    }
}