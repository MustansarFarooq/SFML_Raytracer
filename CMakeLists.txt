cmake_minimum_required(VERSION 3.24)
project(SFML_Raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ImGui-SFML
find_package(ImGui-SFML REQUIRED)

# Add executable
add_executable(SFML_Raytracer main.cpp)

# Link SFML and ImGui-SFML using imported targets
target_link_libraries(SFML_Raytracer PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        ImGui-SFML::ImGui-SFML
)