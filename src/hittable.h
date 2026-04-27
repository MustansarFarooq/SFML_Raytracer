//
// class meant to be for any objet that you can have a ray hit ig
// big thanks to ray tracing in one weekend again
//

#ifndef SFML_RAYTRACER_HITTABLE_H
#define SFML_RAYTRACER_HITTABLE_H

#include "interval.h"
#include "ray.h"



//data struct for where a ray hit
struct hit_record {
    sf::Vector3f p;
    sf::Vector3f normal;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const sf::Vector3f& outward_normal) {
        front_face = r.getDirection().dot(outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


#endif //SFML_RAYTRACER_HITTABLE_H