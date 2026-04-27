#ifndef SFML_RAYTRACER_CAMERA_H
#define SFML_RAYTRACER_CAMERA_H
#include <iostream>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include "raytracer.h"
#include "hittable.h"

class camera {
public:
    int samplesPerPixel;
    int maxDepth = 10;
    camera(const int &w, const int &h) : screenWidth(w), screenHeight(h) { initialize(); };

    void render(const hittable &world, std::vector<std::uint8_t> &image) {
        pixelSamplesScale = 1.f/samplesPerPixel;
        for (int y = 0; y < screenHeight; ++y) {
            for (int x = 0; x < screenWidth; ++x) {
                int index = (y * screenWidth + x) * 4;

                sf::Vector3f pixelColor{0, 0, 0};

                for (int sample = 0; sample < samplesPerPixel; ++sample) {
                    ray r = getRay(x, y);
                    sf::Vector3f c = rayColor(r, maxDepth, world);
                    pixelColor += sf::Vector3f(c.x, c.y, c.z);
                }

                pixelColor *= static_cast<float>(pixelSamplesScale);
                pixelColor.x = std::sqrt(pixelColor.x);
                pixelColor.y = std::sqrt(pixelColor.y);
                pixelColor.z = std::sqrt(pixelColor.z);
                sf::Color finalColor(
                    static_cast<uint8_t>(255.999f*pixelColor.x),
                    static_cast<uint8_t>(255.999f*pixelColor.y),
                    static_cast<uint8_t>(255.999f*pixelColor.z),
                    255
                );
                int pixelIndex = y * screenWidth + x;
                int totalPixels = screenWidth * screenHeight;

                std::cout << 100.0 * pixelIndex / totalPixels << "%\r";
                setPixel(image, finalColor, index);
            }
        }
    }

private:
    double pixelSamplesScale;

    const int screenHeight;
    const int screenWidth;
    //pixel steps
    sf::Vector3f pixelStepX;
    sf::Vector3f pixelStepY;
    sf::Vector3f cameraPosition;

    const float fovDegrees = 90.f;
    float aspectRatio = static_cast<float>(screenWidth) / screenHeight;
    sf::Vector3f topLeftCorner;


    void setPixel(std::vector<std::uint8_t> &imageBuffer, sf::Color color, int i) {
        imageBuffer[i] = color.r;
        imageBuffer[i + 1] = color.g;
        imageBuffer[i + 2] = color.b;
        imageBuffer[i + 3] = color.a;
    }

    ray getRay(int i, int j) {
        sf::Vector3f offset = sample_square();
        sf::Vector3f pixelSample = topLeftCorner
                                   + pixelStepX * (i + offset.x)
                                   //add the 0.5f because we want to get the center of the pixel, not the top left corner of it
                                   - pixelStepY * (j + offset.y); //subtract because y increases as we go down in sfml

        sf::Vector3f rayOrigin = cameraPosition;
        sf::Vector3f rayDir = pixelSample - rayOrigin;

        return ray(rayOrigin, rayDir);
    }

    //just directly yoinked from rtiow
    // Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
    sf::Vector3f sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return sf::Vector3f(random_double() - 0.5, random_double() - 0.5, 0);
    }

    void initialize() {
        cameraPosition = {0, 0, 0};
        const float viewingPlaneDistance = 1.0f; //the distance between the camera and the center of the viewing plane
        float viewPlaneWidth = 2 * viewingPlaneDistance * std::tan(degrees_to_radians(fovDegrees) / 2);
        float viewPlaneHeight = viewPlaneWidth / aspectRatio;
        //distance from left to right, up to down, of each view plane as vec3f
        sf::Vector3f planeRight = {viewPlaneWidth, 0, 0};
        sf::Vector3f planeUp = {0, viewPlaneHeight, 0};

        pixelStepX = planeRight / static_cast<float>(screenWidth);
        pixelStepY = planeUp / static_cast<float>(screenHeight);

        //get the position of the top left corner in coordinate space ig
        topLeftCorner = cameraPosition + sf::Vector3f{
                            -viewPlaneWidth / 2,
                            viewPlaneHeight / 2,
                            -viewingPlaneDistance
                        };
    }

    sf::Vector3f rayColor(const ray &r, int depth, const hittable &world) const {
        hit_record rec;

        if (depth <= 0) return {0,0,0};
        if (world.hit(r, interval(0.001, infinity), rec)) {
            sf::Vector3f dir = randomOnHemisphere(rec.normal);
            return 0.5f * rayColor(ray(rec.p, dir),depth-1, world);

        }
        sf::Vector3f normalizedDir = r.getDirection().normalized();
        float a = 0.5f * (normalizedDir.y + 1.0f);

        sf::Vector3f white(1.f, 1.f, 1.f);
        sf::Vector3f blue(0.5f, 0.7f, 1.f);

        sf::Vector3f result = (1.0f - a) * white + a * blue; //a nice sky-blue color
        return result;
    }
};

#endif //SFML_RAYTRACER_CAMERA_H
