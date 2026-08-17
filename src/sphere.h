#ifndef SFML_RAYTRACER_SPHERE_H
#define SFML_RAYTRACER_SPHERE_H

#include <SFML/System/Vector3.hpp>
#include <atomic>

#include "hittable.h"
#include "interval.h"
#include "ray.h"
namespace internal {
inline void getSphere_uv(const sf::Vector3f& p, double& u, double& v) {
  float theta = std::acos(-p.y);
  float phi = std::atan2(-p.z, p.x) + pi;

  u = phi / (2 * pi);
  v = theta / pi;
}
}  // namespace internal

struct nSphere {
  sf::Vector3f center;
  float radius;
  int materialIndex;
};
inline bool hitSphere(const nSphere& s, const ray& r, interval ray_t, hit_record& rec) {
  float a = r.getDirection().lengthSquared();
  float h = r.getDirection().dot(s.center - r.getOrigin());
  float c = (s.center - r.getOrigin()).dot((s.center - r.getOrigin())) - s.radius * s.radius;
  float discriminant = h * h - a * c;
  if (discriminant < 0) {
    return false;
  }
  float sqrtDiscriminant = std::sqrt(discriminant);
  float root = (h - sqrtDiscriminant) / (a);
  if (!ray_t.surrounds(root)) {
    root = (h + sqrtDiscriminant) / (a);
    if (!ray_t.surrounds(root)) {
      return false;
    }
  }
  rec.t = root;
  rec.p = r.at(rec.t);
  sf::Vector3f outward_normal = (rec.p - s.center) / s.radius;
  rec.set_face_normal(r, outward_normal);
  internal::getSphere_uv(outward_normal, rec.u, rec.v);
  rec.materialIndex = s.materialIndex;
  return true;
}

inline aabb sphereBbox(const nSphere& s) {
  sf::Vector3f v = {s.radius, s.radius, s.radius};
  return aabb(s.center - v, s.center + v);
}
// class sphere : public hittable {
//  public:
//   sphere(const sf::Vector3f& center, float radius, int mat) : center(center), radius(std::fmax(0, radius)), materialIndex(mat) {
//     sf::Vector3f v = {radius, radius, radius};
//     bbox = aabb(center - v, center + v);
//   }
//
//   aabb boundingBox() const override { return bbox; }
//
//   bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
//     float a = r.getDirection().lengthSquared();
//     float h = r.getDirection().dot(center - r.getOrigin());
//     float c = (center - r.getOrigin()).dot((center - r.getOrigin())) -
//               radius * radius;
//     float discriminant = h * h - a * c;
//     if (discriminant < 0) {
//       return false;
//     }
//     float sqrtDiscriminant = std::sqrt(discriminant);
//     float root = (h - sqrtDiscriminant) / (a);
//     if (!ray_t.surrounds(root)) {
//       root = (h + sqrtDiscriminant) / (a);
//       if (!ray_t.surrounds(root)) {
//         return false;
//       }
//     }
//     rec.t = root;
//     rec.p = r.at(rec.t);
//     sf::Vector3f outward_normal = (rec.p - center) / radius;
//     rec.set_face_normal(r, outward_normal);
//     getSphere_uv(outward_normal, rec.u, rec.v);
//     rec.materialIndex = materialIndex;
//
//     return true;
//   }
//
//  private:
//   sf::Vector3f center;
//   float radius;
//   int materialIndex;
//   aabb bbox;
//
//   static void getSphere_uv(const sf::Vector3f& p, double& u, double& v) {
//     float theta = std::acos(-p.y);
//     float phi = std::atan2(-p.z, p.x) + pi;
//
//     u = phi / (2 * pi);
//     v = theta / pi;
//   }
// };

#endif  // SFML_RAYTRACER_SPHERE_H
