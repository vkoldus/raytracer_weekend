cmake_minimum_required(VERSION 3.28)
project(pathtracing)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3)
find_package(imgui)
find_package(Eigen3)

add_executable(pathtracing src/main.cpp src/bindings/imgui_impl_glfw.cpp src/bindings/imgui_impl_opengl2.cpp
        include/images.h
        include/path_tracer_main.h
        include/ray.h
        include/interpolation.h
        include/camera.h
        include/types.h
        include/objects.h
)

include_directories(include)

target_link_libraries(pathtracing glfw)
target_link_libraries(pathtracing imgui::imgui)
target_link_libraries(pathtracing Eigen3::Eigen)

