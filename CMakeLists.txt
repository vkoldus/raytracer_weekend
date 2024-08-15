cmake_minimum_required(VERSION 3.28)
project(pathtracing)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3)
find_package(imgui)
find_package(Eigen3)
find_package(PNG)

add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror)
set_source_files_properties(src/bindings/imgui_impl_opengl2.cpp PROPERTIES COMPILE_FLAGS "-Wno-macro-redefined -Wno-sign-conversion")

add_executable(pathtracing src/main.cpp src/bindings/imgui_impl_glfw.cpp src/bindings/imgui_impl_opengl2.cpp
        include/images.h
        include/path_tracer/path_tracer.h
        include/path_tracer/ray.h
        include/interpolation.h
        include/path_tracer/camera.h
        include/types.h
        include/path_tracer/objects.h
        include/state/app_state.h
        include/ui/rendering_window.h
        include/ui/config_window.h
        include/path_tracer/rendering_service.h
        include/consts.h
        include/random.h
        include/gamma.h
        include/path_tracer/materials/material.h
        include/path_tracer/world.h
        include/path_tracer/materials/lambertian.h
        include/path_tracer/materials/metal.h
        include/path_tracer/materials/dielectric.h
)

include_directories(include)

target_link_libraries(pathtracing glfw)
target_link_libraries(pathtracing imgui::imgui)
target_link_libraries(pathtracing Eigen3::Eigen)
target_link_libraries(pathtracing PNG::PNG)
