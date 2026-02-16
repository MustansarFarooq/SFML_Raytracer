#include "ray.h"
#include <cmath>

double ray::hitSphere(sf::Vector3f sphereCenter, float sphereRadius) {
    float a = this->direction.dot(this->direction);
    float b = -2.f * this->direction.dot(sphereCenter-this->origin);
    float c = (sphereCenter-this->origin).dot((sphereCenter-this->origin))-sphereRadius*sphereRadius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1;
    }
    else {
        return (-b-std::sqrt(discriminant))/(2.f*a);
    }
}

sf::Color ray::rayColor() {
    float t = hitSphere(sf::Vector3f{0,0,-1},0.5);
    if (t > 0.f) {
        sf::Vector3f normal = (this->at(t)-sf::Vector3f{0,0,-1}).normalized();
        return {
            static_cast<uint8_t>(127.f*(normal.x+1)),
            static_cast<uint8_t>(127.f*(normal.y+1)),
            static_cast<uint8_t>(127.f*(normal.z+1)),
            255};
    }



    sf::Vector3f normalizedDir = this->getDirection().normalized();
    float a = 0.5f*(normalizedDir.y + 1.0f);

    sf::Vector3f white(255.f, 255.f, 255.f);
    sf::Vector3f blue(127.f, 178.f, 255.f);

    sf::Vector3f result = (1.0f-a)*white + a*blue; //a nice sky-blue color

    return {
        static_cast<uint8_t>(result.x),
        static_cast<uint8_t>(result.y),
        static_cast<uint8_t>(result.z),
        255};
}

