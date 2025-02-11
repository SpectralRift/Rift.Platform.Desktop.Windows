#include <Engine/Platform/Windows/Win32_Thread.hpp>

namespace engine::platform::win32 {
    Win32Thread::~Win32Thread() {
        if (m_ThreadHandle) {
            Stop();
            Join();
            CloseHandle(m_ThreadHandle);
        }
    }

    void Win32Thread::Start() {
        printf("Win32Thread: Beginning thread '%s'\n", m_ThreadName.c_str());

        if (b_IsRunning) {
            printf("Win32Thread: Thread '%s' is already running.\n", m_ThreadName.c_str());
        }

        if (!m_TaskFunc) {
            printf("Win32Thread: Task function for '%s' is not set.\n", m_ThreadName.c_str());
        }

        b_IsRunning = true;
        m_ThreadHandle = CreateThread(nullptr, 0, &Win32Thread::ThreadProc, this, 0, &m_ThreadId);

        if (!m_ThreadHandle) {
            b_IsRunning = false;
            printf("Win32Thread: Failed to create thread '%s.\n", m_ThreadName.c_str());
        }

        printf("Win32Thread: Started thread '%s'\n", m_ThreadName.c_str());
    }

    void Win32Thread::Stop() {
        printf("Win32Thread: Stopping thread '%s'\n", m_ThreadName.c_str());

        if (b_IsRunning) {
            b_IsRunning = false;
        }
    }

    void Win32Thread::Join() {
        if (m_ThreadHandle) {
            WaitForSingleObject(m_ThreadHandle, INFINITE);
            CloseHandle(m_ThreadHandle);
            m_ThreadHandle = nullptr;
            m_ThreadId = 0;
        }
    }

    bool Win32Thread::IsRunning() const {
        return b_IsRunning;
    }

    void Win32Thread::SetName(std::string_view name) {
        m_ThreadName = name;
    }

    std::string Win32Thread::GetName() const {
        return m_ThreadName;
    }

    void Win32Thread::SetTaskFuncInternal(std::function<void()> task) {
        m_TaskFunc = std::move(task);
    }

    DWORD WINAPI Win32Thread::ThreadProc(LPVOID param) {
        auto *self = static_cast<Win32Thread *>(param);
        
        if (self && self->m_TaskFunc) {
            self->m_TaskFunc();
        }
        
        self->b_IsRunning = false;
        return 0;
    }

} // namespace engine::core::runtime
