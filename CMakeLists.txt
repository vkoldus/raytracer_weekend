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
        include/images.hpp
        include/path_tracer/path_tracer.hpp
        include/path_tracer/ray.hpp
        include/interpolation.hpp
        include/path_tracer/camera.hpp
        include/types.hpp
        include/path_tracer/objects.hpp
        include/state/app_state.hpp
        include/ui/rendering_window.hpp
        include/ui/config_window.hpp
        include/path_tracer/rendering_service.hpp
        include/consts.hpp
        include/random.hpp
        include/gamma.hpp
        include/path_tracer/materials/material.hpp
        include/path_tracer/world.hpp
        include/path_tracer/materials/lambertian.hpp
        include/path_tracer/materials/metal.hpp
        include/path_tracer/materials/dielectric.hpp
)

include_directories(include)

target_link_libraries(pathtracing glfw)
target_link_libraries(pathtracing imgui::imgui)
target_link_libraries(pathtracing Eigen3::Eigen)
target_link_libraries(pathtracing PNG::PNG)
