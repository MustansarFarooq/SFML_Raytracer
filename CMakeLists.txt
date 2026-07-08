cmake_minimum_required(VERSION 3.24)
project(SFML_Raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# find_package(SFML) seems to break it. chatgpt says it may be linux?
# Find ImGui-SFML
find_package(ImGui-SFML REQUIRED)
find_package(OpenMP REQUIRED)
# Add executable
add_executable(SFML_Raytracer src/main.cpp
        src/ray.h
        src/hittable.h
        src/sphere.h
        src/hittable_list.h
        src/raytracer.h
        src/interval.h
        src/camera.h
        src/material.h
        src/aabb.h
        src/bvh.h
        src/texture.h
        include/stb_image.h
        src/image.h
        src/quad.h
)
target_include_directories(SFML_Raytracer PRIVATE include)
# Link SFML and ImGui-SFML using imported targets
target_link_libraries(SFML_Raytracer PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        ImGui-SFML::ImGui-SFML
        OpenMP::OpenMP_CXX
)