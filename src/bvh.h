#ifndef SFML_RAYTRACER_BVH_H
#define SFML_RAYTRACER_BVH_H

#include <algorithm>

#include "aabb.h"
#include "hittable.h"
#include "primitive.h"
#include "scene.h"
#include "sphere.h"
inline int buildBVH(
    std::vector<flatBVHNode>& nodes,
    std::vector<primitiveRef>& objects,
    int start,
    int end) {
  int nodeIndex = nodes.size();
  nodes.push_back(flatBVHNode{});

  aabb bbox = aabb::empty;

  // Expand a bounding box for the whole scene.
  for (int object_index = start; object_index < end; object_index++) {
    bbox = aabb(bbox, objects[object_index].bbox);
  }

  nodes[nodeIndex].bbox = bbox;

  int axis = bbox.longestAxis();

  std::sort(
      objects.begin() + start,
      objects.begin() + end,
      [&](const auto& a, const auto& b) {
        return a.bbox.axis_interval(axis).min <
               b.bbox.axis_interval(axis).min;
      });

  int objectSpan = end - start;

  if (objectSpan <= 2) {
    nodes[nodeIndex].start = start;
    nodes[nodeIndex].count = objectSpan;
    nodes[nodeIndex].leftChild = -1;
    nodes[nodeIndex].rightChild = -1;
    return nodeIndex;
  }

  int mid = start + objectSpan / 2;

  int left = buildBVH(
      nodes,
      objects,
      start,
      mid);

  int right = buildBVH(
      nodes,
      objects,
      mid,
      end);

  nodes[nodeIndex].leftChild = left;
  nodes[nodeIndex].rightChild = right;
  nodes[nodeIndex].start = -1;
  nodes[nodeIndex].count = 0;

  return nodeIndex;
}

inline void buildSceneBVH(scene& scene) {
  scene.primitives.clear();

  for (int i = 0; i < scene.spheres.size(); i++) {
    scene.primitives.push_back({primitiveType::SPHERE,
                                i,
                                sphereBbox(scene.spheres[i])});
  }

  for (int i = 0; i < scene.quads.size(); i++) {
    scene.primitives.push_back({primitiveType::QUAD,
                                i,
                                quadBbox(scene.quads[i])});
  }

  scene.nodes.clear();

  scene.rootNode = buildBVH(
      scene.nodes,
      scene.primitives,
      0,
      scene.primitives.size());
}
inline bool hitBVH(
    const scene& scene,
    int nodeIndex,
    const ray& r,
    interval ray_t,
    hit_record& rec) {
  const flatBVHNode& node = scene.nodes[nodeIndex];

  if (!node.bbox.hit(r, ray_t))
    return false;

  bool hitAnything = false;

  if (node.count > 0) {
    interval currentInterval = ray_t;

    for (int i = 0; i < node.count; i++) {
      const auto& primitive = scene.primitives[node.start + i];
      bool hit = false;

      if (primitive.type == primitiveType::SPHERE) {
        hit = hitSphere(
            scene.spheres[primitive.index],
            r,
            currentInterval,
            rec);
      } else if (primitive.type == primitiveType::QUAD) {
        hit = hitQuad(
            scene.quads[primitive.index],
            r,
            currentInterval,
            rec);
      }

      if (hit) {
        hitAnything = true;
        currentInterval.max = rec.t;
      }
    }
  } else {
    bool hitLeft = hitBVH(
        scene,
        node.leftChild,
        r,
        ray_t,
        rec);

    interval rightInterval(
        ray_t.min,
        hitLeft ? rec.t : ray_t.max);

    bool hitRight = hitBVH(
        scene,
        node.rightChild,
        r,
        rightInterval,
        rec);

    hitAnything = hitLeft || hitRight;
  }

  return hitAnything;
}

#endif  // SFML_RAYTRACER_BVH_H
