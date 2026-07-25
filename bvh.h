#ifndef SFML_RAYTRACER_BVH_H
#define SFML_RAYTRACER_BVH_H
#include "hittable.h"
#include "aabb.h"
#include "hittable_list.h"
class bvhNode : public hittable {

public:
    bvhNode(hittable_list list) : bvhNode(list.objects,0,list.objects.size()) {};

    bvhNode(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end) {
        bbox = aabb::empty;

        for (size_t object_index = start; object_index < end; object_index++) {
            bbox = aabb(bbox,objects[object_index]->boundingBox());
        }

        int axis = bbox.longestAxis();
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                        : box_z_compare;

        size_t objectSpan = end-start;

        if (objectSpan == 1 ) {
            left = right = objects[start];
        }
        else if (objectSpan == 2 ) {
            left = objects[start];
            right = objects[start+1];
        }
        else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            auto mid = start + objectSpan/2;
            left = std::make_shared<bvhNode>(objects, start, mid);
            right = std::make_shared<bvhNode>(objects, mid, end);
        }
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        if (!bbox.hit(r,ray_t))
            return false;

        bool hitLeft = left->hit(r,ray_t,rec);
        bool hitRight = right->hit(r, interval(ray_t.min, hitLeft ? rec.t : ray_t.max),rec);
        return hitLeft || hitRight;

    }

    aabb boundingBox() const override {return bbox;}
private:
    aabb bbox;
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;

    static bool boxCompare( const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axisIndex) {
        interval aAxisInterval = a->boundingBox().axis_interval(axisIndex);
        interval bAxisInterval = b->boundingBox().axis_interval(axisIndex);

        return aAxisInterval.min < bAxisInterval.min;
    }

    static bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return boxCompare(a,b,0);
    }
    static bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return boxCompare(a,b,1);
    }
    static bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return boxCompare(a,b,2);
    }
};



#endif //SFML_RAYTRACER_BVH_H