#ifndef SFML_RAYTRACER_SPHERE_H
#define SFML_RAYTRACER_SPHERE_H

#include <SFML/System/Vector3.hpp>
#include "ray.h"
#include "hittable.h"
#include "interval.h"


class sphere : public hittable {
public:
    sphere(const sf::Vector3f& center, float radius) : center(center), radius(std::fmax(0,radius)) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        float a = r.getDirection().lengthSquared();
        float h = r.getDirection().dot(center-r.getOrigin());
        float c = (center-r.getOrigin()).dot((center-r.getOrigin()))-radius*radius;
        float discriminant = h*h - a*c;
        if (discriminant < 0) {
            return false;
        }
        float sqrtDiscriminant =std::sqrt(discriminant);
        float root =(h-sqrtDiscriminant)/(a);
        if (!ray_t.surrounds(root)) {
            root = (h+sqrtDiscriminant)/(a);
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }
        rec.t  = root;
        rec.p = r.at(rec.t);
        sf::Vector3f outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);


        return true;
    }
private:
    sf::Vector3f center;
    float radius;
};


#endif //SFML_RAYTRACER_SPHERE_H