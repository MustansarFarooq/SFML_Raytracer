#ifndef SFML_RAYTRACER_RAY_H
#define SFML_RAYTRACER_RAY_H


#include <SFML/System/Vector3.hpp>
#include "ray.h"
class hittable;


class ray {
public:
    ray(){};

    ray(sf::Vector3f origin, sf::Vector3f direction) : origin(origin), direction(direction){};


    const sf::Vector3f &getOrigin() const { return origin; };
    const sf::Vector3f &getDirection() const { return direction; };
    sf::Vector3f at(float t) const {return origin + t * direction;};
private:
    sf::Vector3f origin;
    sf::Vector3f direction;
    double hitSphere(sf::Vector3f sphereCenter, float sphereRadius);
};


#endif //SFML_RAYTRACER_RAY_H