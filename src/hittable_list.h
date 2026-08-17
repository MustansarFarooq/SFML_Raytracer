
#ifndef SFML_RAYTRACER_HITTABLE_LIST_H
#define SFML_RAYTRACER_HITTABLE_LIST_H

#include <vector>

#include "aabb.h"
#include "bvh.h"
#include "hittable.h"
#include "interval.h"
#include "scene.h"
class hittable_list : public hittable {
 public:
  std::vector<std::shared_ptr<hittable>> objects;
  struct scene& scene;

  hittable_list(struct scene& s) : scene(s) {};

  // hittable_list(std::shared_ptr<hittable> object) { add(object); }

  void add(std::shared_ptr<hittable> object) {
    objects.push_back(object);
    bbox = aabb(bbox, object->boundingBox());
  };
  void clear() { objects.clear(); };
  aabb boundingBox() const override { return bbox; }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    if (scene.nodes.empty()) {
      return false;
    }
    return hitBVH(scene, 0, r, ray_t, rec);

    // bool hit_anything = false;
    // hit_record temp_record;
    // float closestSoFar = ray_t.max;
    //
    // for (auto& sphere : scene.spheres) {
    //   if (hitSphere(sphere, r, interval(ray_t.min, closestSoFar), temp_record)) {
    //     hit_anything = true;
    //     closestSoFar = temp_record.t;
    //     rec = temp_record;
    //   }
    // }
    // for (auto& quad : scene.quads) {
    //   if (hitQuad(quad, r, interval(ray_t.min, closestSoFar), temp_record)) {
    //     hit_anything = true;
    //     closestSoFar = temp_record.t;
    //     rec = temp_record;
    //   }
    // }
    // return hit_anything;
  }

 private:
  aabb bbox;
};

#endif  // SFML_RAYTRACER_HITTABLE_LIST_H
