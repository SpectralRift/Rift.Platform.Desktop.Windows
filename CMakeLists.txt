cmake_minimum_required(VERSION 3.14)
project(Rift_Platform_Desktop_Windows VERSION 0.1.1)

add_library(
        Rift_Platform_Desktop_Windows
        STATIC
        private/Engine/Platform/Windows/Win32_Window.cpp
        private/Engine/Platform/Windows/Win32_Platform.cpp
        private/Engine/Platform/Windows/Graphics/Win32_GLContext.cpp
        private/Engine/Platform/Windows/Input/Win32_MouseDevice.cpp
        private/Engine/Platform/Windows/Input/Win32_KeyboardDevice.cpp
        private/Engine/Platform/Windows/Win32_Thread.cpp
        private/Engine/Platform/Windows/Graphics/Win32_DX9Context.cpp
)

target_include_directories(
        Rift_Platform_Desktop_Windows
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private"
)

target_link_libraries(Rift_Platform_Desktop_Windows RiftEngine_Core_Runtime RiftEngine_Backend_OpenGL Rift_Input Rift_Backend_DX9 Rift_Runtime_Logging)