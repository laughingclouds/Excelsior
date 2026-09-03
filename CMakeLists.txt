# CMakeList.txt : CMake project for Excelsior, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.25)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Excelsior")

# https://github.com/libsdl-org/SDL.git

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory("vendored/SDL" EXCLUDE_FROM_ALL)

# Add source to this project's executable.
add_executable (Excelsior WIN32 "src/Excelsior.cpp")

find_package(OpenGL REQUIRED)

target_sources(Excelsior
    PRIVATE
        "src/gl.c"            # opengl loader source file
        "src/Application.cpp"
        "src/Application.hpp"
        "src/Stroke.cpp"
        "src/Stroke.hpp"
)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendored/imgui")

target_sources(Excelsior
    PRIVATE
        # Core files
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui.h"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_internal.h"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        # Backends
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
        "${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_sdl3.h"
)

target_include_directories(Excelsior
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
)

target_link_libraries(Excelsior
    PRIVATE
        SDL3::SDL3
        OpenGL::GL
)

target_compile_features(Excelsior PRIVATE cxx_std_20)
set_target_properties(Excelsior PROPERTIES 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# TODO: Add tests and install targets if needed.
