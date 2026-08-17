//
// Created by farooq on 7/6/26.
//

#ifndef SFML_RAYTRACER_QUAD_H
#define SFML_RAYTRACER_QUAD_H
#include <SFML/System/Vector3.hpp>
#include <iostream>

#include "hittable.h"

namespace internal {
inline bool inInterior(float a, float b, hit_record& rec) {
  interval unitInterval = interval(0, 1);
  if (!unitInterval.contains(a) || !unitInterval.contains(b)) return false;
  rec.u = a;
  rec.v = b;

  return true;
}
}  // namespace internal
struct nQuad {
  sf::Vector3f q;
  sf::Vector3f u;
  sf::Vector3f v;
  int materialIndex;

  sf::Vector3f n;
  sf::Vector3f normal;
  float D;
  sf::Vector3f w;
};
inline nQuad makeQuad(sf::Vector3f q, sf::Vector3f u, sf::Vector3f v, int materialIndex) {
  nQuad quad;
  quad.q = q;
  quad.u = u;
  quad.v = v;
  quad.materialIndex = materialIndex;

  quad.n = u.cross(v);
  quad.normal = quad.n.normalized();
  quad.D = quad.normal.dot(q);
  quad.w = quad.n / (quad.n.dot(quad.n));
  return quad;
}
inline aabb quadBbox(const nQuad& q) {
  aabb quadDiagonal1 = aabb(q.q, q.q + q.u + q.v);
  aabb quadDiagonal2 = aabb(q.q + q.u, q.q + q.v);
  return aabb(quadDiagonal1, quadDiagonal2);
}
inline bool hitQuad(const nQuad& q, const ray& r, interval ray_t, hit_record& rec) {
  auto denom = q.normal.dot(r.getDirection());
  if (std::fabs(denom) < 1e-8) return false;

  auto t = (q.D - (q.normal.dot(r.getOrigin()))) / denom;
  if (!ray_t.contains(t)) return false;

  sf::Vector3f intersection = r.at(t);
  sf::Vector3f planeHitPointVector = intersection - q.q;
  float alpha = q.w.dot(planeHitPointVector.cross(q.v));
  float beta = q.w.dot(q.u.cross(planeHitPointVector));

  if (!internal::inInterior(alpha, beta, rec)) return false;

  rec.t = t;
  rec.p = intersection;
  rec.materialIndex = q.materialIndex;
  rec.set_face_normal(r, q.normal);
  return true;
};

class quad : public hittable {
 public:
  quad(const sf::Vector3f& q, const sf::Vector3f& u, const sf::Vector3f& v, int mat) : materialIndex(mat), q(q), u(u), v(v) {
    sf::Vector3f n = u.cross(v);
    normal = n.normalized();
    D = normal.dot(q);
    w = n / (n.dot(n));
    setBoundingBox();
  }
  void setBoundingBox() {
    aabb quadDiagonal1 = aabb(q, q + u + v);
    aabb quadDiagonal2 = aabb(q + u, q + v);
    bbox = aabb(quadDiagonal1, quadDiagonal2);
  }

  aabb boundingBox() const override { return bbox; }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    auto denom = normal.dot(r.getDirection());
    if (std::fabs(denom) < 1e-8) return false;

    auto t = (D - (normal.dot(r.getOrigin()))) / denom;
    if (!ray_t.contains(t)) return false;

    sf::Vector3f intersection = r.at(t);
    sf::Vector3f planeHitPointVector = intersection - q;
    float alpha = w.dot(planeHitPointVector.cross(v));
    float beta = w.dot(u.cross(planeHitPointVector));

    if (!inInterior(alpha, beta, rec)) return false;

    rec.t = t;
    rec.p = intersection;
    rec.materialIndex = materialIndex;
    rec.set_face_normal(r, normal);
    return true;
  };

  inline bool inInterior(float a, float b, hit_record& rec) const {
    interval unitInterval = interval(0, 1);
    if (!unitInterval.contains(a) || !unitInterval.contains(b)) return false;
    rec.u = a;
    rec.v = b;

    return true;
  }

 private:
  int materialIndex;
  sf::Vector3f q, u, v;
  sf::Vector3f normal;
  sf::Vector3f w;
  aabb bbox;
  float D;
};
#endif  // SFML_RAYTRACER_QUAD_H
