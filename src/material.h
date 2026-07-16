//
// Created by farooq on 5/16/26.
//

#ifndef SFML_RAYTRACER_MATERIAL_H
#define SFML_RAYTRACER_MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material {
    public:
    virtual ~material() = default;

    virtual sf::Vector3f emmited(float u, float v, const sf::Vector3f& p) {
        return {0,0,0};
    }

     // a material should just tell ius whether to scatter the ray or have it absorbed
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, sf::Vector3f& attenuation, ray& scattered
        ) const {
        return false;
    }

};

class lambertian : public material {
public:
    lambertian(const sf::Vector3f& albedo) : tex(std::make_shared<solidColor>(albedo)) {}
    lambertian(std::shared_ptr<texture> tex) : tex(tex) {}
    bool scatter(const ray &r_in, const hit_record &rec, sf::Vector3f& attenuation, ray &scattered) const override {
        sf::Vector3f scatter_direction = rec.normal + randomUnitVector(); //lambertian distribution
        if (nearZero(scatter_direction)) scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);

        attenuation = tex->value(rec.u,rec.v,rec.p); //if you want attenuation some of the time, you could do albedo/p, where p is a constant. currently we have no attenuation
        return true;
    }
private:
    std::shared_ptr<texture> tex;
};

class metal : public material {
public:
    metal(const sf::Vector3f& albedo, float fuzz) : tex(std::make_shared<solidColor>(albedo)), fuzz(fuzz){}
    metal(std::shared_ptr<texture> tex, float fuzz) : tex(tex), fuzz(fuzz) {}
    bool scatter(const ray &r_in, const hit_record &rec, sf::Vector3f& attenuation, ray &scattered) const override {
        sf::Vector3f reflected = reflect(r_in.getDirection(), rec.normal);
        if (fuzz!=0) {
            reflected = reflected.normalized() + fuzz*randomUnitVector();
        }
        scattered = ray(rec.p, reflected);

        attenuation = tex->value(rec.u,rec.v,rec.p);
        return scattered.getDirection().dot(rec.normal) > 0;
    }
private:
    float fuzz;
    std::shared_ptr<texture> tex;
};

class dielectric : public material {
public:
    dielectric(float refractiveIndex) : refractiveIndex(refractiveIndex){}

    bool scatter(const ray &r_in, const hit_record &rec, sf::Vector3f& attenuation, ray &scattered) const override {
        attenuation = {1.0,1.0,1.0};
        double rri =  rec.front_face ? (1/refractiveIndex) : refractiveIndex;

        sf::Vector3f unitDirection = r_in.getDirection().normalized();
        float cosTheta = std::fmin(rec.normal.dot(-unitDirection), 1.0);
        float sinTheta = std::sqrt(1.0-cosTheta*cosTheta);
        bool cannotRefract = rri*sinTheta > 1.0;
        sf::Vector3f direction;
        if (cannotRefract || reflectance(cosTheta,rri)>random_double()) {
            direction = reflect(unitDirection,rec.normal);
        }
        else {
            direction = refract(unitDirection,rec.normal,rri);
        }


        scattered = ray(rec.p, direction);

        return true;
    }
private:
    float refractiveIndex;
    static double reflectance(double cosine, double rri) {
        double r0 = (1-rri)/(1+rri);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1-cosine),5);
    }
};

class diffuseLight : public material {
public:
    diffuseLight(sf::Vector3f emitColor) : tex(std::make_shared<solidColor>(emitColor)) {};
    diffuseLight(std::shared_ptr<texture> tex) : tex(tex) {};

    sf::Vector3f emmited(float u, float v, const sf::Vector3f &p) override {
        return tex->value(u,v,p);
    }


private:
    std::shared_ptr<texture> tex;
};


#endif //SFML_RAYTRACER_MATERIAL_H