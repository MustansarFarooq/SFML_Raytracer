#ifndef SFML_RAYTRACER_RAYTRACER_H
#define SFML_RAYTRACER_RAYTRACER_H

#include <cmath>
#include <limits>
#include <memory>
#include <pcg_random.hpp>
#include <random>
#include <SFML/System/Vector3.hpp>
#include <array>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    thread_local  pcg32 generator{std::random_device{}()};
    return std::ldexp(static_cast<double>(generator()), -32);
}

/// @brief Returns a random real in [min,max).
inline double random_double(double min, double max) {

    return min + (max-min)*random_double();
}

inline double random_int(int min, int max) {
    // Returns a random real in [min,max).
    return static_cast<int>(random_double(min, max + 1));
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

inline bool nearZero(sf::Vector3f& v) {
    double s = 1e-8;
    return (std::fabs(v.x) < s) && (std::fabs(v.y) < s) && (std::fabs(v.z) < s);
}

inline sf::Vector3f reflect(const sf::Vector3f& incident, const sf::Vector3f& normal) {
    return incident - 2.0f * incident.dot(normal) * normal;
}

/// @param relativeRefractiveIndex The relative refractive index, equal to Eta initial divided by Eta final
inline sf::Vector3f refract(const sf::Vector3f& incident, const sf::Vector3f& normal, float relativeRefractiveIndex) {
    float cosTheta = std::fmin(normal.dot(-incident), 1.0);
    sf::Vector3f refractPerp = relativeRefractiveIndex * (incident + cosTheta * normal);
    sf::Vector3f refractParallel = static_cast<float>(-std::sqrt(std::fabs(1.0-refractPerp.lengthSquared()))) * normal;
    return refractPerp + refractParallel;
}

inline sf::Vector3f operator*(const sf::Vector3f & lhs, const sf::Vector3f & rhs) {
    return {
        lhs.x*rhs.x,
        lhs.y*rhs.y,
        lhs.z*rhs.z
    };
};

inline std::array<float, 3> toFloat(sf::Vector3f& v) {
    return {v.x,v.y,v.z};
}

#endif //SFML_RAYTRACER_RAYTRACER_H