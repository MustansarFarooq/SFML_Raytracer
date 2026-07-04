#ifndef SFML_RAYTRACER_CAMERA_H
#define SFML_RAYTRACER_CAMERA_H
#include <iostream>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include "raytracer.h"
#include "hittable.h"
#include "material.h"
#include <atomic>
#include <chrono>
#include <mutex>
class camera {
public:
    std::vector<std::uint8_t> frontBuffer;
    std::vector<std::uint8_t> backBuffer;
    std::mutex imageMutex;

    int samplesPerPixel;
    int maxDepth = 10;
    float fovDegrees;
    sf::Vector3f lookFrom = {0,0,0};
    sf::Vector3f lookAt = {0,0,-1};
    sf::Vector3f vup = {0,1,0};

    std::atomic<int> completedPixels = 0;

    int totalPixels;
    std::vector<sf::Vector3f> accumulated;
    float accumulatedFrame = 1;


    double lastFrameTime = 0.0;
    camera(const int &w, const int &h) : screenWidth(w), screenHeight(h) {
        totalPixels = screenWidth * screenHeight;
        accumulated.resize(totalPixels);
        frontBuffer.resize(totalPixels*4);
        backBuffer.resize(totalPixels*4);
    }

    void render(const hittable &world) {

        auto start = std::chrono::steady_clock::now();
        initialize();
        pixelSamplesScale = 1.f/samplesPerPixel;
        completedPixels = 0;

        for (int y = 0; y < screenHeight; ++y) {
            for (int x = 0; x < screenWidth; ++x) {
                int index = (y * screenWidth + x);

                sf::Vector3f pixelColor{0, 0, 0};

                for (int sample = 0; sample < samplesPerPixel; ++sample) {
                    ray r = getRay(x, y);
                    sf::Vector3f c = rayColor(r, maxDepth, world);
                    pixelColor += sf::Vector3f(c.x, c.y, c.z);
                }
                accumulated[index] += pixelColor;
                pixelColor = accumulated[index]/accumulatedFrame;
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

                setPixel(backBuffer, finalColor, index*4);
                ++completedPixels;


            }
        }
        accumulatedFrame++;

        {
            std::lock_guard<std::mutex> lock(imageMutex);
            frontBuffer.swap(backBuffer);
        }

        auto end = std::chrono::steady_clock::now();
        lastFrameTime = std::chrono::duration<double>(end - start).count();
    }

private:
    float pixelSamplesScale;

    const int screenHeight;
    const int screenWidth;
    //pixel steps
    sf::Vector3f pixelStepX;
    sf::Vector3f pixelStepY;
    sf::Vector3f cameraPosition;


    float aspectRatio = static_cast<float>(screenWidth) / screenHeight;
    sf::Vector3f topLeftCorner;

    sf::Vector3f u,v,w;

    void setPixel(std::vector<std::uint8_t>& imageBuffer, sf::Color color, int i) {
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
                                   + pixelStepY * (j + offset.y); //subtract because y increases as we go down in sfml

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
        cameraPosition = lookFrom;
        const float viewingPlaneDistance = (lookFrom-lookAt).length(); //the distance between the camera and the center of the viewing plane
        float viewPlaneWidth = 2 * viewingPlaneDistance * std::tan(degrees_to_radians(fovDegrees) / 2);
        float viewPlaneHeight = viewPlaneWidth / aspectRatio;
        //distance from left to right, up to down, of each view plane as vec3f
        sf::Vector3f planeRight = {viewPlaneWidth, 0, 0};
        sf::Vector3f planeUp = {0, viewPlaneHeight, 0};


        w = (lookFrom-lookAt).normalized();
        u = (vup.cross(w)).normalized();
        v = w.cross(u);
        sf::Vector3f viewPlane_u = viewPlaneWidth*u;
        sf::Vector3f viewPlane_v = viewPlaneHeight*-v;


        pixelStepX = viewPlane_u / static_cast<float>(screenWidth);
        pixelStepY = viewPlane_v / static_cast<float>(screenHeight);

        //get the position of the top left corner in coordinate space ig
        topLeftCorner = cameraPosition - (viewingPlaneDistance*w) - viewPlane_u/2.f - viewPlane_v/2.f;

        // topLeftCorner = cameraPosition + sf::Vector3f{
        //                     -viewPlaneWidth / 2,
        //                     viewPlaneHeight / 2,
        //                     -viewingPlaneDistance
        //                 };
    }

    sf::Vector3f rayColor(const ray &r, int depth, const hittable &world) const {
        hit_record rec;

        if (depth <= 0) return {0,0,0};
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            sf::Vector3f attenuation;
            if(rec.mat->scatter(r,rec,attenuation, scattered)) {
                return attenuation*rayColor(scattered,depth-1,world);
            }
            return sf::Vector3f(0,0,0);

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
