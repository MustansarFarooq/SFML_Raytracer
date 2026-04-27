/*
 * Big thanks to Ray Tracing in One Weekend
 *
 * An SFML adaptation of the book.
 */


#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <optional>
#include <cstdint>



const int screenWidth = 800;
const int screenHeight = 600;

std::vector<std::uint8_t> image(screenWidth*screenHeight*4);
sf::Texture imageTexture;
sf::Sprite imageSprite(imageTexture);


camera cam(screenWidth, screenHeight);


int main() {

    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    //sfml whatever jargon idk boiler plate spaghetti
    sf::RenderWindow window( sf::VideoMode( { screenWidth, screenHeight } ), "SFML RayTracer" );
    window.setFramerateLimit(60);
    imageTexture.resize({screenWidth, screenHeight});
    imageSprite.setTexture(imageTexture,true);
    // Initialize ImGui-SFML
    if (!ImGui::SFML::Init(window)) {
        return 1; // failed to initialize
    }
    sf::Clock deltaClock;
    float slider_value = 0.5f;

    //world stuff
    hittable_list world;
    world.add(std::make_shared<sphere>(sf::Vector3f(0,0,-1),0.5));
    world.add(std::make_shared<sphere>(sf::Vector3f(0,-100.5,-1),100));
    while (window.isOpen()) {

        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            ImGui::SFML::ProcessEvent(window, *event);

            // Check for window close
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        //legit probably the most important line here rn. everything else is lowk boilerplate
        cam.render(world,image);


        // Update ImGui-SFML
        ImGui::SFML::Update(window, deltaClock.restart());

        // Build ImGui UI
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(100, 50), ImGuiCond_Always);
        ImGui::Begin("Stats", nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize);
        ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        ImGui::End();
        imageTexture.update(image.data());
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