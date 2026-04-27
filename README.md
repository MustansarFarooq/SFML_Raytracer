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
```


Just right after implementing antialiasing, simple diffuse materials, fixed shadow acne, True Lambertian Reflection, and gamma correction:

<img width="960" height="724" alt="Screenshot_2026-04-26_21 11 41" src="https://github.com/user-attachments/assets/121edaa0-403e-462b-a509-5b02d8587ac7" />

NOTES:
- Samples per Pixel = 100
- Max depth per ray = 50


