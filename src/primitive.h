
#ifndef SFML_RAYTRACER_PRIMITIVE_H
#define SFML_RAYTRACER_PRIMITIVE_H
#include "aabb.h"
enum class primitiveType {
  SPHERE,
  QUAD
};
struct primitiveRef {
  primitiveType type;
  int index;
  aabb bbox;
};

#endif  // SFML_RAYTRACER_PRIMITIVE_H
