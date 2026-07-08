//
// Created by farooq on 7/6/26.
//

#ifndef SFML_RAYTRACER_QUAD_H
#define SFML_RAYTRACER_QUAD_H
#include <memory>
#include <SFML/System/Vector3.hpp>

#include "hittable.h"

class quad : public hittable {
public:


    quad(const sf::Vector3f& q,const sf::Vector3f& u,const sf::Vector3f& v, std::shared_ptr<material> m) : mat(m), q(q), u(u), v(v) {
        sf::Vector3f n = u.cross(v);
        normal = n.normalized();
        D = normal.dot(q);
        w = n / (n.dot(n));
        setBoundingBox();
    }
    void setBoundingBox() {
        aabb quadDiagonal1 = aabb(q,q+u+v);
        aabb quadDiagonal2 = aabb(q+u,q+v);
        bbox = aabb(quadDiagonal1,quadDiagonal2);
    }

    aabb boundingBox() const override {return bbox;}



    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        auto denom = normal.dot(r.getDirection());
        if (std::fabs(denom) < 1e-8) return false;

        auto t = (D-(normal.dot(r.getOrigin())))/denom;
        if (!ray_t.contains(t)) return false;

        sf::Vector3f intersection = r.at(t);
        sf::Vector3f planeHitPointVector = intersection - q;
        float alpha = w.dot(planeHitPointVector.cross(v));
        float beta = w.dot(u.cross(planeHitPointVector));

        if (!inInterior(alpha, beta, rec)) return false;


        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r,normal);
        return true;
    };

    virtual bool inInterior(float a, float b, hit_record & rec) const {
        interval unitInterval = interval(0,1);
        if (!unitInterval.contains(a) || !unitInterval.contains(b)) return false;
        rec.u = a;
        rec.v = b;

        return true;
    }

private:
    std::shared_ptr<material> mat;
    sf::Vector3f q,u,v;
    sf::Vector3f normal;
    sf::Vector3f w;
    aabb bbox;
    float D;
};
#endif //SFML_RAYTRACER_QUAD_H