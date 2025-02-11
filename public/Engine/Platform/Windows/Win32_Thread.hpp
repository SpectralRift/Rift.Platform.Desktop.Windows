#pragma once

#include <Engine/Core/Runtime/IThread.hpp>

#include <Windows.h>

#include <atomic>
#include <functional>
#include <string>
#include <stdexcept>

namespace engine::platform::win32 {
    struct Win32Thread : public core::runtime::IThread {
    public:
        Win32Thread() : m_ThreadHandle(nullptr), m_ThreadId(0), b_IsRunning(false), m_TaskFunc(nullptr) {}

        ~Win32Thread() override;

        void Start() override;

        void Stop() override;

        void Join() override;

        bool IsRunning() const override;

        void SetName(std::string_view name) override;

        std::string GetName() const override;

    protected:
        void SetTaskFuncInternal(std::function<void()> task) override;

        static DWORD WINAPI ThreadProc(LPVOID param);

        HANDLE m_ThreadHandle;
        DWORD m_ThreadId;
        std::atomic<bool> b_IsRunning;
        std::function<void()> m_TaskFunc;
        std::string m_ThreadName;
    };
}