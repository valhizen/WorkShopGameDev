cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("WorkShopGameDev")
 
find_package(OpenGL REQUIRED)

# ── SDL3: platform-aware setup ────────────────────────────────────────────────
if(WIN32)
    # Windows: use the prebuilt binaries in external/sdl
    set(SDL3_ROOT "${CMAKE_SOURCE_DIR}/external/sdl")

    add_library(SDL3::SDL3 SHARED IMPORTED)
    set_target_properties(SDL3::SDL3 PROPERTIES
        IMPORTED_IMPLIB               "${SDL3_ROOT}/lib/SDL3.lib"
        IMPORTED_LOCATION             "${SDL3_ROOT}/lib/SDL3.dll"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL3_ROOT}/include"
    )

    # Copy SDL3.dll next to each executable after build
    function(copy_sdl3_dll target)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_ROOT}/lib/SDL3.dll"
                "$<TARGET_FILE_DIR:${target}>/SDL3.dll"
            COMMENT "Copying SDL3.dll for ${target}"
        )
    endfunction()

else()
    # Linux / macOS: use the system-installed SDL3
    # Install via:
    #   Ubuntu/Debian: sudo apt install libsdl3-dev
    #   Fedora:        sudo dnf install SDL3-devel
    #   macOS:         brew install sdl3
    find_package(SDL3 QUIET)

    if(NOT SDL3_FOUND)
        # Fallback: try pkg-config (common on older distros)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL3 REQUIRED sdl3)

        add_library(SDL3::SDL3 INTERFACE IMPORTED)
        set_target_properties(SDL3::SDL3 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES      "${SDL3_LIBRARIES}"
            INTERFACE_COMPILE_OPTIONS     "${SDL3_CFLAGS_OTHER}"
        )
        target_link_directories(SDL3::SDL3 INTERFACE ${SDL3_LIBRARY_DIRS})
    endif()

    # No DLL to copy on Linux/macOS - it's a shared system library
    function(copy_sdl3_dll target)
        # no-op on non-Windows
    endfunction()
endif()

# ── External sources ──────────────────────────────────────────────────────────
set(GLAD_DIR    "${CMAKE_SOURCE_DIR}/external/glad")
set(GLAD_SOURCE "${GLAD_DIR}/src/glad.c")
set(GLM_DIR     "${CMAKE_SOURCE_DIR}/external/glm")
set(IMGUI_DIR   "${CMAKE_SOURCE_DIR}/external/imgui")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
)

# ── Shared settings interface ─────────────────────────────────────────────────
add_library(CommonSettings INTERFACE)
target_include_directories(CommonSettings INTERFACE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLAD_DIR}/include
    ${GLM_DIR}
)
target_link_libraries(CommonSettings INTERFACE OpenGL::GL SDL3::SDL3)
target_compile_features(CommonSettings INTERFACE cxx_std_20)

# Linux needs libdl and pthreads for OpenGL/GLAD to work correctly
if(UNIX AND NOT APPLE)
    target_link_libraries(CommonSettings INTERFACE dl pthread)
endif()

# ── Executables ───────────────────────────────────────────────────────────────
add_executable(Setup      "src/Setup/main.cpp")
target_link_libraries(Setup PRIVATE CommonSettings)
copy_sdl3_dll(Setup)

add_executable(Window     "src/Window/main.cpp")
target_link_libraries(Window PRIVATE CommonSettings)
copy_sdl3_dll(Window)

add_executable(Input      "src/Input/main.cpp")
target_link_libraries(Input PRIVATE CommonSettings)
copy_sdl3_dll(Input)

add_executable(OpenglInit "src/OpenglInit/main.cpp" ${GLAD_SOURCE})
target_link_libraries(OpenglInit PRIVATE CommonSettings)
copy_sdl3_dll(OpenglInit)

add_executable(ImguiInit  "src/ImguiInit/main.cpp" ${GLAD_SOURCE} ${IMGUI_SOURCES})
target_link_libraries(ImguiInit PRIVATE CommonSettings)
copy_sdl3_dll(ImguiInit)

add_executable(Triangle   "src/Triangle/main.cpp" "src/Triangle/Shader.hpp" ${GLAD_SOURCE} ${IMGUI_SOURCES})
target_link_libraries(Triangle PRIVATE CommonSettings)
copy_sdl3_dll(Triangle)

add_executable(Circle     "src/Circle/main.cpp" "src/Circle/Shader.hpp" ${GLAD_SOURCE} ${IMGUI_SOURCES})
target_link_libraries(Circle PRIVATE CommonSettings)
copy_sdl3_dll(Circle)