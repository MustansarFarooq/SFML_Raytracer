//
// Created by farooq on 5/16/26.
//

#ifndef SFML_RAYTRACER_MATERIAL_H
#define SFML_RAYTRACER_MATERIAL_H

#include "SFML/System/Vector3.hpp"
#include "hittable.h"
#include "ray.h"
#include "texture.h"

inline double reflectance(double cosine, double rri) {
  double r0 = (1 - rri) / (1 + rri);
  r0 = r0 * r0;
  return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

enum class materialType {
  lambertian,
  metal,
  dielectric,
  diffuseLight
};

struct material {
  materialType type;
  sf::Vector3f albedo;
  float fuzz = 0.f;
  float refractiveIndex = 1.5f;  // default refraciveIndex for glass
  sf::Vector3f emmitted = {0.f, 0.f, 0.f};
};

inline bool scatter(material& mat, const ray& r_in, const hit_record& rec, sf::Vector3f& attenuation, ray& scattered) {
  switch (mat.type) {
    case materialType::lambertian: {
      sf::Vector3f scatter_direction = rec.normal + randomUnitVector();  // lambertian distribution
      if (nearZero(scatter_direction)) scatter_direction = rec.normal;

      scattered = ray(rec.p, scatter_direction);

      attenuation = mat.albedo;
      // attenuation = tex->value(rec.u, rec.v, rec.p);  // if you want attenuation some of the time,
      //  you could do albedo/p, where p is a
      //  constant. currently we have no attenuation
      return true;

      break;
    }
    case materialType::metal: {
      sf::Vector3f reflected = reflect(r_in.getDirection(), rec.normal);
      if (mat.fuzz != 0) {
        reflected = reflected.normalized() + mat.fuzz * randomUnitVector();
      }
      scattered = ray(rec.p, reflected);

      attenuation = mat.albedo;
      // attenuation = tex->value(rec.u, rec.v, rec.p);
      return scattered.getDirection().dot(rec.normal) > 0;
      break;
    }
    case materialType::dielectric: {
      attenuation = {1.0, 1.0, 1.0};
      double rri = rec.front_face ? (1 / mat.refractiveIndex) : mat.refractiveIndex;

      sf::Vector3f unitDirection = r_in.getDirection().normalized();
      float cosTheta = std::fmin(rec.normal.dot(-unitDirection), 1.0);
      float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
      bool cannotRefract = rri * sinTheta > 1.0;
      sf::Vector3f direction;
      if (cannotRefract || reflectance(cosTheta, rri) > random_double()) {
        direction = reflect(unitDirection, rec.normal);
      } else {
        direction = refract(unitDirection, rec.normal, rri);
      }

      scattered = ray(rec.p, direction);

      return true;
    }
    case materialType::diffuseLight: {
      return false;
    }
  }
}
#endif  // SFML_RAYTRACER_MATERIAL_H
