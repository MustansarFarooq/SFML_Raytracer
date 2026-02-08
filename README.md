# A simple raytracer that uses SFML

## Build

```bash
git clone <this repo>
cd sfml-imgui-minimal

# Make sure vcpkg is cloned somewhere, e.g. ~/vcpkg
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build
./build/sfml_imgui_minimal
