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

<img width="960" height="724" alt="Screenshot_2026-04-26_21 11 41" src="https://github.com/user-attachments/assets/a5bbe8a3-f1f2-4540-8fa4-32b1c1cf0516" />

