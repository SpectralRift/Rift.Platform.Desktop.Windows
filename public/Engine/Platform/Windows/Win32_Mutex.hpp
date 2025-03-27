#pragma once

#include <Windows.h>

#include <Engine/Core/Runtime/IMutex.hpp>

namespace engine::platform::win32 {
    struct Win32Mutex : public core::runtime::IMutex {
        Win32Mutex();

        void Lock() override;
        void Unlock() override;
    protected:
        CRITICAL_SECTION m_CriticalSection;
    };
}