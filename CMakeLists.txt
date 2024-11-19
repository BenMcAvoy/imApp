cmake_minimum_required(VERSION 3.5)

project(imAppRoot)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Linux -pthread shenanigans
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

#=== LIBRARY: cimgui + Dear ImGui
file(GLOB IMGUI_SOURCES vendor/imgui/*.cpp vendor/imgui/*.h)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui INTERFACE vendor/imgui)

add_library(cimgui STATIC
    vendor/cimgui/cimgui.cpp
    vendor/cimgui/cimgui.h)
target_link_libraries(cimgui PUBLIC imgui)
target_include_directories(cimgui INTERFACE vendor/cimgui)

#=== LIBRARY: sokol
file (GLOB SOKOL_HEADERS vendor/sokol-libs/*.h vendor/sokol-libs/util/*.h)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    add_library(sokol STATIC vendor/sokol.c ${SOKOL_HEADERS})
    target_compile_options(sokol PRIVATE -x objective-c)
    target_link_libraries(sokol PUBLIC
        "-framework QuartzCore"
        "-framework Cocoa"
        "-framework MetalKit"
        "-framework Metal")
else()
    add_library(sokol STATIC vendor/sokol.c ${SOKOL_HEADERS})
    if (CMAKE_SYSTEM_NAME STREQUAL Linux)
        target_link_libraries(sokol INTERFACE X11 Xi Xcursor GL dl m)
        target_link_libraries(sokol PUBLIC Threads::Threads)
    endif()
endif()
target_link_libraries(sokol PUBLIC cimgui imgui)
target_include_directories(sokol INTERFACE vendor/sokol-libs/ vendor/sokol-libs/util)

#=== EXECUTABLE: app
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_executable(app WIN32 src/app.cpp)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT app)
else()
    add_executable(app src/app.cpp)
endif()
target_link_libraries(app sokol)

# Emscripten-specific linker options
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
	# use our own minimal shell.html, this path is relative to ${CMAKE_BINARY_DIR}
    target_link_options(app PRIVATE --shell-file ../vendor/shell.html)
    # link with WebGL2
    target_link_options(app PRIVATE -sUSE_WEBGL2=1)
    # WASM+JS size optimizations
    target_link_options(app PRIVATE -sNO_FILESYSTEM=1 -sASSERTIONS=0 -sMALLOC=emmalloc --closure=1)
endif()

# explicitly strip dead code
if (CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    target_link_options(app PRIVATE LINKER:-dead_strip)
endif()

# this hack removes the xxx-CMakeForceLinker.cxx dummy file
set_target_properties(app PROPERTIES LINKER_LANGUAGE C)
