#ifndef SFML_RAYTRACER_AABB_H
#define SFML_RAYTRACER_AABB_H
#include "interval.h"
#include "ray.h"
#include "SFML/System/Vector3.hpp"

/// @brief Axis Aligned Bounding Box
class aabb {
public:
    interval x, y, z;
    static const aabb empty, universe;

    aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {}
    aabb() {};
    const interval& axis_interval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    aabb(const sf::Vector3f& a, const sf::Vector3f& b)
    {
        x = (a.x <= b.x) ? interval(a.x, b.x) : interval(b.x, a.x);
        y = (a.y <= b.y) ? interval(a.y, b.y) : interval(b.y, a.y);
        z = (a.z <= b.z) ? interval(a.z, b.z) : interval(b.z, a.z);
    }
    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    int longestAxis() const {
        if (x.size() > y.size())
            return x.size() > z.size() ? 0 : 2;
        else
            return y.size() > z.size() ? 1 : 2;
    }

    bool hit(const ray& r, interval ray_t) const {
        const sf::Vector3f& ray_orig = r.getOrigin();
        const float* orig = &ray_orig.x;
        const sf::Vector3f& ray_dir = r.getDirection();
        const float* dir = &ray_dir.x;

        for (int i = 0; i < 3; i++) {
            const interval& ax =  axis_interval(i);
            const double dirInv = 1.f/dir[i];

            float t0 = (ax.min - orig[i]) * dirInv;
            float t1 = (ax.max - orig[i]) * dirInv;

            //you check for every axis and update ray_t on whether it satisfies the interval ray_t.
            if (t0 < t1) {
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
            } else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t0 < ray_t.max) ray_t.max = t0;
            }

            if (ray_t.max <= ray_t.min) return false;
        }
        return true;
    }
};
const aabb aabb::empty    = aabb(interval::empty,    interval::empty,    interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif //SFML_RAYTRACER_AABB_H