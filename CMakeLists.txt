cmake_minimum_required (VERSION 3.24)

project ("ray tracing")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. Fetch SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# 2. Fetch ImGui
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

# 3. Fetch ImGui-SFML (CRITICAL FIXES FOR MINGW FETCHCONTENT)
set(IMGUI_SFML_FIND_SFML OFF CACHE INTERNAL "")
set(IMGUI_SFML_IF_FIND_PACKAGE OFF CACHE INTERNAL "")

FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

# 4. Define Your Executable Target
add_executable(SFML_Raytracer 
    src/main.cpp
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
    src/cameraControls.cpp
)

# 5. Include Directories and Target Linking
target_include_directories(SFML_Raytracer PRIVATE include)
target_link_libraries(SFML_Raytracer PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)
