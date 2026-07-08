# A simple raytracer that uses SFML

## Features implemented:
- [X] Simple Ray-Sphere Intersection
- [X] Materials (Lambertian, Metals, Dielectrics)
- [X] Positionable Camera
- [X] BVH
- [X] Textures
- [X] Quads
- [ ] Emissive Materials
- [ ] Multithreading
- [ ] Real-time camera movement


### Multiple spheres (Rendered Before BVH and Textures)
<img width="810" height="610" alt="Final Render I" src="https://github.com/user-attachments/assets/ad5b84a9-52c6-45ac-9787-99af703b0140" />

### A checkered floor
<img width="810" height="610" alt="Checkered" src="https://github.com/user-attachments/assets/7b1b0b8e-cb8a-4bc4-8460-c175fb0d9506" />

### Image Texture
<img width="810" height="610" alt="Earth" src="https://github.com/user-attachments/assets/c2c9c4e6-641c-4472-9038-f9cc2c92f600" />

### Quads
<img width="810" height="610" alt="image" src="https://github.com/user-attachments/assets/259eefdc-5ed4-4b9e-8251-2e93e8327fb4" />


#### Build

```bash
git clone https://github.com/MustansarFarooq/SFML_Raytracer.git
cd SFML_Raytracer

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build
./build/SFML_Raytracer
```

