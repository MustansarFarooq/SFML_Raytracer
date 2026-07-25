
#ifndef SFML_RAYTRACER_HITTABLE_LIST_H
#define SFML_RAYTRACER_HITTABLE_LIST_H

#include <vector>
#include "interval.h"

#include "hittable.h"

#include "aabb.h"
class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() {};

    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->boundingBox());
    };
    void clear() {objects.clear();};
    aabb boundingBox() const override { return bbox; }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
       bool hit_anything = false;
        hit_record temp_record;
        double closestSoFar = ray_t.max;
        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closestSoFar), temp_record)) {
                hit_anything = true;
                closestSoFar = temp_record.t;
                rec = temp_record;
            }
        }
        return hit_anything;
    }

private:
    aabb bbox;

};


#endif //SFML_RAYTRACER_HITTABLE_LIST_H