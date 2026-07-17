/*
 * Big thanks to Ray Tracing in One Weekend
 *
 * An SFML adaptation of the book.
 */


#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <optional>
#include <cstdint>
#include <thread>
#include "bvh.h"
#include "quad.h"


const int screenWidth = 800;
const int screenHeight = 600;

sf::Texture screenTexture;
sf::Sprite imageSprite(screenTexture);


camera cam(screenWidth, screenHeight);
void coverScreen(hittable_list& world) {
    cam.samplesPerPixel = 1;
    cam.maxDepth = 90;
    cam.fovDegrees = 45.f;
    cam.lookFrom = {13, 2, 3};
    cam.lookAt = {0, 0, 0};

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            sf::Vector3f center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - sf::Vector3f(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randomVector3f() * randomVector3f();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = randomVector3f(0.5,1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }


    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(sf::Vector3f(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(sf::Vector3f(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(sf::Vector3f(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(sf::Vector3f(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(sf::Vector3f(4, 1, 0), 1.0, material3));
}
void spheres(hittable_list& world) {
    cam.samplesPerPixel = 1;
    cam.maxDepth = 90;
    cam.fovDegrees = 90.f;

    cam.lookFrom = {0, 1, 1};
    cam.lookAt = {0, 1, -1};
    cam.background = {0.5,0.8,0.92};
    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(sf::Vector3f(0, 1, -1), 0.4, material1));

    auto material2 = std::make_shared<lambertian>(sf::Vector3f(0.85, 0.06, 0.8));
    world.add(std::make_shared<sphere>(sf::Vector3f(-1, 1, -1), 0.4, material2));

    auto material3 = std::make_shared<metal>(sf::Vector3f(0.05, 0.55, 0.2), 0.0);
    world.add(std::make_shared<sphere>(sf::Vector3f(1, 1, -1), 0.4, material3));
}
void earth(hittable_list& world) {
    cam.samplesPerPixel = 1;
    cam.maxDepth = 90;
    cam.fovDegrees = 45.f;

    cam.lookFrom = {2, 2, 1};
    cam.lookAt = {0, 1, -1};

    auto earthTexture = std::make_shared<imageTexture>("earthTexture.jpg");
    auto material1 = std::make_shared<lambertian>(earthTexture);
    world.add(std::make_shared<sphere>(sf::Vector3f(0, 1, -1), 0.4, material1));

}
void light(hittable_list& world) {
    cam.samplesPerPixel = 1;
    cam.maxDepth = 90;
    cam.fovDegrees = 90.f;

    cam.lookFrom = {0, 1, 1};
    cam.lookAt = {0, 1, -1};

    auto material1 = std::make_shared<lambertian>(sf::Vector3f(0.85, 0.06, 0.25));
    world.add(std::make_shared<sphere>(sf::Vector3f(0, 1, -1), 0.4, material1));

    auto material2 = std::make_shared<diffuseLight>(sf::Vector3f(1,1,1));
    world.add(std::make_shared<quad>(sf::Vector3f(0.5,1,-1), sf::Vector3f(0,0,1),sf::Vector3f(0,1,0),material2));

    auto material3 = std::make_shared<metal>(sf::Vector3f(0.8,0.8,0.8),0);
    world.add(std::make_shared<quad>(sf::Vector3f(-0.5,1,-1), sf::Vector3f(0,0,0.5),sf::Vector3f(0,0.5,0),material3));
}
void quadsAndSphere(hittable_list& world) {
    cam.samplesPerPixel = 1;
    cam.maxDepth = 90;
    cam.fovDegrees = 90.f;
    cam.background = {0.5,0.8,0.92};
    cam.lookFrom = {0, 1, 1};
    cam.lookAt = {0, 1, -1};

    auto material1 = std::make_shared<lambertian>(sf::Vector3f(0.85, 0.06, 0.25));
    world.add(std::make_shared<sphere>(sf::Vector3f(0, 1, -1), 0.4, material1));

    auto material2 = std::make_shared<metal>(sf::Vector3f(0.2,0.2,0.2),0);
    world.add(std::make_shared<quad>(sf::Vector3f(0.5,1,-1), sf::Vector3f(0,0,3),sf::Vector3f(0,1,0),material2));
}

void sphere(hittable_list& world) {
    cam.samplesPerPixel = 1;
    cam.maxDepth = 90;
    cam.fovDegrees = 90.f;
    cam.background = {0.5,0.8,0.92};
    cam.lookFrom = {0, 1, 1};
    cam.lookAt = {0, 1, -1};

    auto material1 = std::make_shared<lambertian>(sf::Vector3f(1.0,0.0,0.0));
    world.add(std::make_shared<class sphere>(sf::Vector3f(0, 1, -1), 0.4, material1));
}

int main() {


    //sfml whatever jargon idk boiler plate spaghetti
    sf::RenderWindow window( sf::VideoMode( { screenWidth, screenHeight } ), "SFML RayTracer" );
    window.setFramerateLimit(60);
    screenTexture.resize({screenWidth, screenHeight});
    imageSprite.setTexture(screenTexture,true);
    // Initialize ImGui-SFML
    if (!ImGui::SFML::Init(window)) {
        return 1; // failed to initialize and terminate
    }
    sf::Clock deltaClock;


    //world stuff
    hittable_list world;
    auto groundMaterial = std::make_shared<checkerTexture>(1.f,sf::Vector3f(0.65, 0.65, 0.65),sf::Vector3f(0.2,0.2,0.2));
    world.add(std::make_shared<class sphere>(sf::Vector3f(0,-1000,-1),1000,std::make_shared<lambertian>(groundMaterial)));


    spheres(world);




    world = hittable_list(std::make_shared<bvhNode>(world));

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(0.5);

    double seconds = 0;
    std::atomic<bool> rendering = false;
    std::thread renderThread;
    while (window.isOpen()) {

        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            ImGui::SFML::ProcessEvent(window, *event);

            // Check for window close
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        //
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        //     cam.lookFrom.z-=1;
        // }
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        //     cam.lookFrom.z+=1;
        // }


        // Update ImGui-SFML
        ImGui::SFML::Update(window, deltaClock.restart());

        // Build ImGui UI

        ImGui::Begin("Stats", nullptr);
        ImGui::Text("FPS: %.2f", 1.f/cam.lastFrameTime);
        ImGui::InputInt("Samples Per Pixel", &cam.samplesPerPixel);
        //ImGui::ColorEdit3("Ground Color", &groundMaterial->albedo.x);

        std::atomic<int> completedPixels = 0;

        if (ImGui::Button(rendering ? "Stop Rendering"
                            : "Begin Rendering"))
        {
            if (!rendering)
            {
                rendering = true;

                renderThread = std::thread([&]
                {
                    while (rendering.load(std::memory_order_relaxed))
                    {
                        cam.render(world);
                    }
                });
            }
            else
            {
                rendering = false;

                if (renderThread.joinable())
                    renderThread.join();

                std::fill(cam.accumulated.begin(), cam.accumulated.end(), sf::Vector3f(0.f,0.f,0.f));
                cam.accumulatedFrame = 1;
            }
        }
        float progress =
            float(cam.completedPixels.load(std::memory_order_relaxed))
            / cam.totalPixels;
        ImGui::ProgressBar(progress);
        ImGui::Text("Accumulated Frames: %.0f",
            cam.accumulatedFrame);
        ImGui::Text("Time to Render Last Frame %.2f", cam.lastFrameTime);

        ImGui::End();
        {
            std::lock_guard<std::mutex> lock(cam.imageMutex);
            screenTexture.update(cam.frontBuffer.data());
        }
        // Draw
        window.clear(sf::Color{0, 0, 0});
        window.draw(imageSprite);
        ImGui::SFML::Render(window);

        window.display();
    }

    // Shutdown ImGui-SFML
    ImGui::SFML::Shutdown();

    return 0;
}