#ifndef SFML_RAYTRACER_SCENE_H
#define SFML_RAYTRACER_SCENE_H
#include <vector>

#include "bvhNode.h"
#include "material.h"
#include "primitive.h"
#include "quad.h"
#include "sphere.h"
struct scene {
  std::vector<nSphere> spheres;
  std::vector<nQuad> quads;
  std::vector<material> materials;
  std::vector<primitiveRef> primitives;
  std::vector<flatBVHNode> nodes;
  int rootNode;
};

#endif  // SFML_RAYTRACER_SCENE_H
