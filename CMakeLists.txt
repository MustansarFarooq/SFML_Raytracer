cmake_minimum_required(VERSION 3.24)
project(SFML_Raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# find_package(SFML) seems to break it. chatgpt says it may be linux?
# Find ImGui-SFML
find_package(ImGui-SFML REQUIRED)

# Add executable
add_executable(SFML_Raytracer src/main.cpp
        src/ray.cpp
        src/ray.h
)

# Link SFML and ImGui-SFML using imported targets
target_link_libraries(SFML_Raytracer PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        ImGui-SFML::ImGui-SFML
)