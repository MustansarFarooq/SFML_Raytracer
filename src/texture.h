#ifndef SFML_RAYTRACER_TEXTURE_H
#define SFML_RAYTRACER_TEXTURE_H
#include <memory>
#include <cmath>
#include <SFML/System/Vector3.hpp>

#include "image.h"

class texture {
public:
    virtual ~texture() = default;

    /// @brief Given a location on the object, (u,v), or the hit point in the world space, p, what should the texture return
    virtual sf::Vector3f value(double u, double v, const sf::Vector3f& p) const = 0;
};

class solidColor : public texture {
public:
    solidColor(sf::Vector3f albedo) : albedo(albedo){};

    solidColor(double r, double g, double b) : solidColor(sf::Vector3f(r,g,b)) {};

    sf::Vector3f value(double u, double v, const sf::Vector3f &p) const override {
        return albedo;
    }
private:
    sf::Vector3f albedo;
};

class checkerTexture : public texture {
public:
    checkerTexture(float scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) : invScale(1.f/scale), even(even), odd(odd){};
    checkerTexture(float scale, sf::Vector3f c1, sf::Vector3f c2) : checkerTexture(scale, std::make_shared<solidColor>(c1), std::make_shared<solidColor>(c2)) {};
    sf::Vector3f value(double u, double v, const sf::Vector3f &p) const override {
        int x = std::floor(invScale * p.x);
        int y = std::floor(invScale * p.y);
        int z = std::floor(invScale * p.z);

        bool isEven = (x+y+z) % 2 == 0;

        return isEven ? even->value(u,v,p) : odd->value(u,v,p);

    }
private:
    float invScale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};

class imageTexture : public texture {
public:
    imageTexture(const char* fname) : img(fname) {};


    sf::Vector3f value(double u, double v, const sf::Vector3f &p) const override {
        u = interval(0,1).clamp(u);
        v = 1.0 - interval(0,1).clamp(v);

        auto i = int(u * img.width());
        auto j = int(v* img.height());
        auto pixel = img.pixelData(i,j);

        return sf::Vector3f(pixel[0],pixel[1],pixel[2]);

    }
private:
    image img;
};

#endif //SFML_RAYTRACER_TEXTURE_H