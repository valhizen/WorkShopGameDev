# CMakeList.txt : CMake project for WorkShopGameDev, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("WorkShopGameDev")

# Find the packages
find_package(OpenGL REQUIRED)

# External sources
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")
set(GLAD_SOURCE ${GLAD_DIR}/src/glad.c)
set(GLM_DIR "${CMAKE_SOURCE_DIR}/external/glm")
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
)

# Dependency setting
add_library(CommonSettings INTERFACE)
target_include_directories(CommonSettings INTERFACE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLAD_DIR}/include
    ${GLM_DIR}
)
target_link_libraries(CommonSettings INTERFACE OpenGL::GL)
target_compile_features(CommonSettings INTERFACE cxx_std_20)

if(WIN32)
    # TODO: ADD TARGET HERE
elseif(UNIX)
    target_link_libraries(CommonSettings INTERFACE "${CMAKE_SOURCE_DIR}/external/sdl/lib/libSDL3.so.0.4.10")
endif()

# Add source to this project's executable.

# Setup
add_executable (Setup "src/Setup/main.cpp")
target_link_libraries(Setup PRIVATE CommonSettings)

# Window
add_executable (Window "src/Window/main.cpp")
target_link_libraries(Window PRIVATE CommonSettings)

# Input
add_executable (Input "src/Input/main.cpp")
target_link_libraries(Input PRIVATE CommonSettings)

# OpenglInit
add_executable (OpenglInit "src/OpenglInit/main.cpp" ${GLAD_SOURCE})
target_link_libraries(OpenglInit PRIVATE CommonSettings)

# ImguiInit
add_executable (ImguiInit "src/ImguiInit/main.cpp" ${GLAD_SOURCE} ${IMGUI_SOURCES})
target_link_libraries(ImguiInit PRIVATE CommonSettings)

# Triangle
add_executable (Triangle "src/Triangle/main.cpp" "src/Triangle/Shader.hpp" ${GLAD_SOURCE} ${IMGUI_SOURCES})
target_link_libraries(Triangle PRIVATE CommonSettings)

# Circle
add_executable (Circle "src/Circle/main.cpp" "src/Circle/Shader.hpp" ${GLAD_SOURCE} ${IMGUI_SOURCES})
target_link_libraries(Circle PRIVATE CommonSettings)

# TODO: Add tests and install targets if needed.
