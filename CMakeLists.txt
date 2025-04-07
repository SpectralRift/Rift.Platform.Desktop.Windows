cmake_minimum_required(VERSION 3.14)
project(Rift_Platform_Desktop_Windows VERSION 0.1.1)

set(PLATFORM_WINDOWS_USE_WINDOWED_SUBSYSTEM FALSE)
set(RIFT_WIN32_DEPS_LIST "Rift.Core.Runtime;Rift.Backend.OpenGL;Rift.Input;Rift.Runtime.Logging")
set(RIFT_WIN32_ADDITIONAL_FILES "")

add_library(
        Rift_Platform_Desktop_Windows
        STATIC
        private/Engine/Platform/Windows/Win32_Window.cpp
        private/Engine/Platform/Windows/Win32_Platform.cpp
        private/Engine/Platform/Windows/Graphics/Win32_GLContext.cpp
        private/Engine/Platform/Windows/Input/Win32_MouseDevice.cpp
        private/Engine/Platform/Windows/Input/Win32_KeyboardDevice.cpp
        private/Engine/Platform/Windows/Win32_Thread.cpp
        private/Engine/Platform/Windows/Win32_Mutex.cpp
        private/Engine/Platform/Windows/Win32_EngineMain.cpp
        ${RIFT_WIN32_ADDITIONAL_FILES}

)

if("Rift.Backend.D3D9" IN_LIST RIFT_ACTIVE_MODULES)
    message(STATUS "The Rift.Backend.D3D9 module was identified as active; enabling D3D9 support.")

    list(APPEND RIFT_WIN32_DEPS_LIST "Rift.Backend.D3D9")
    list(APPEND RIFT_WIN32_ADDITIONAL_FILES "private/Engine/Platform/Windows/Graphics/Win32_D3D9Context.cpp")

    target_compile_definitions(Rift_Platform_Desktop_Windows PUBLIC BACKEND_D3D9_ENABLED)
endif()

target_include_directories(
        Rift_Platform_Desktop_Windows
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private"
)

if (PLATFORM_WINDOWS_USE_WINDOWED_SUBSYSTEM)
    # configure parent project to use windows subsystem
    target_compile_definitions(Rift_Platform_Desktop_Windows PUBLIC USE_WINDOWED_SUBSYSTEM)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif ()

rift_resolve_module_libs("${RIFT_WIN32_DEPS_LIST}" RIFT_WIN32_DEPS)

target_link_libraries(Rift_Platform_Desktop_Windows ${RIFT_WIN32_DEPS})