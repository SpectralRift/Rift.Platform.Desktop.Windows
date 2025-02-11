#include <Windows.h>

#include <Engine/Core/Platform.hpp>

#include <Engine/Platform/Windows/Win32_Window.hpp>
#include <Engine/Platform/Windows/Win32_Thread.hpp>

namespace engine::core::Platform {
    std::string GetName() {
        return "Win32";
    }

    void ShowMessageBox(std::string_view title, std::string_view message) {
        MessageBoxA(nullptr, message.data(), title.data(), MB_OK);
    }

    std::unique_ptr<runtime::AppHost> GetSuitableHost() {
        return nullptr;
    }

    std::unique_ptr<runtime::IWindow> CreateAppWindow() {
        return std::make_unique<platform::win32::Win32Window>();
    }

    std::unique_ptr<runtime::IThread> CreateThread() {
        return std::make_unique<platform::win32::Win32Thread>();
    }

    void Init() {

    }

    void PreEngineInit() {

    }

    void Shutdown() {

    }
}