#include <Engine/Platform/Windows/Win32_Mutex.hpp>

#include <Windows.h>

namespace engine::platform::win32 {
    Win32Mutex::Win32Mutex() {
        InitializeCriticalSection(&m_CriticalSection);
    }

    void Win32Mutex::Lock() {
        EnterCriticalSection(&m_CriticalSection);
    }

    void Win32Mutex::Unlock() {
        LeaveCriticalSection(&m_CriticalSection);
    }
}