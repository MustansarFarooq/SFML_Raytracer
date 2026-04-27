#ifndef SFML_RAYTRACER_RAYTRACER_H
#define SFML_RAYTRACER_RAYTRACER_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <SFML/System/Vector3.hpp>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline sf::Vector3f randomVector3f() {
    return sf::Vector3f(random_double(),random_double(),random_double());
}

inline sf::Vector3f randomVector3f(double min, double max) {
    return sf::Vector3f(random_double(min,max),random_double(min,max),random_double(min,max));
}

inline sf::Vector3f randomUnitVector() {
    while (true) {
        sf::Vector3f p = randomVector3f(-1,1);
        float lengsq = p.lengthSquared();
        if (1e-160 < lengsq && lengsq <= 1) {
            float invLen = 1.0f / std::sqrt(lengsq);

            return sf::Vector3f(
                p.x * invLen,
                p.y * invLen,
                p.z * invLen
            );
        }
    }
}

 inline sf::Vector3f randomOnHemisphere(const sf::Vector3f& normal) {
    sf::Vector3f onUnitSphere = randomUnitVector();
    if (onUnitSphere.dot(normal) > 0.0) return onUnitSphere;
    else return -onUnitSphere;
}

#endif //SFML_RAYTRACER_RAYTRACER_H