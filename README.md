# A simple raytracer that uses SFML

## Build

```bash
git clone https://github.com/MustansarFarooq/SFML_Raytracer.git
cd SFML_Raytracer

# Make sure vcpkg is cloned somewhere, e.g. ~/vcpkg
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build
./build/SFML_Raytracer
