cmake_minimum_required(VERSION 3.28)
project(pathtracing)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3)
find_package(imgui)

add_executable(pathtracing src/main.cpp src/bindings/imgui_impl_glfw.cpp src/bindings/imgui_impl_opengl2.cpp
        include/images.h
        include/path_tracer_main.h)

include_directories(include)

target_link_libraries(pathtracing glfw)
target_link_libraries(pathtracing imgui::imgui)


