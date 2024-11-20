cmake_minimum_required(VERSION 3.10)

project(imApp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# luajit only has pkg-config
find_package(PkgConfig REQUIRED)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)

pkg_check_modules(LUAJIT REQUIRED IMPORTED_TARGET luajit)

find_path(SOKOL_INCLUDE_DIRS "sokol_app.h")

add_executable(imApp src/main.cpp)

target_include_directories(imApp PRIVATE ${SOKOL_INCLUDE_DIRS})
target_link_libraries(imApp PRIVATE fmt::fmt spdlog::spdlog imgui::imgui sol2 PkgConfig::LUAJIT)
