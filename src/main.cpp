/*
 * Big thanks to Ray Tracing in One Weekend
 *
 * An SFML adaptation of the book.
 */




#include <cmath>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <optional>
#include <cstdint>

#include "ray.h"


void setPixel(std::vector<std::uint8_t>& imageBuffer, sf::Color color, int i) {
    imageBuffer[i] = color.r;
    imageBuffer[i + 1] = color.g;
    imageBuffer[i + 2] = color.b;
    imageBuffer[i + 3] = color.a;
}






const int screenWidth = 800;
const int screenHeight = 600;
const float toDegrees = std::numbers::pi/180.0f;

//set up the camera and the view plane
sf::Vector3f cameraPosition = {0,0,0};
const float viewingPlaneDistance = 1.0f; //the distance between the camera and the center of the viewing plane
const float fovDegrees = 90.f;
float aspectRatio = static_cast<float>(screenWidth)/screenHeight;


float viewPlaneWidth = 2 * viewingPlaneDistance * std::tan(fovDegrees*toDegrees/2);
float viewPlaneHeight = viewPlaneWidth/aspectRatio;


//distance from left to right, up to down, of each view plane as vec3f
sf::Vector3f planeRight = {viewPlaneWidth,0,0};
sf::Vector3f planeUp = {0,viewPlaneHeight,0};

//get the position of the top left corner in coordinate space ig
sf::Vector3f topLeftCorner = cameraPosition + sf::Vector3f{
    -viewPlaneWidth/2,
    viewPlaneHeight/2,
    -viewingPlaneDistance};

//pixel steps
sf::Vector3f pixelStepX = planeRight/static_cast<float>(screenWidth);
sf::Vector3f pixelStepY = planeUp/static_cast<float>(screenHeight);


std::vector<std::uint8_t> image(screenWidth*screenHeight*4);
sf::Texture imageTexture;
sf::Sprite imageSprite(imageTexture);

int main() {



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

    while (window.isOpen()) {

        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            ImGui::SFML::ProcessEvent(window, *event);

            // Check for window close
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

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
        for (int y = 0; y < screenHeight; ++y)
        {
            for (int x = 0; x < screenWidth; ++x)
            {
                int index = (y * screenWidth + x) * 4;

                sf::Vector3f pixelPosition = topLeftCorner
                                            + pixelStepX*(x+0.5f)  //add the 0.5f because we want to get the center of the pixel, not the top left corner of it
                                            - pixelStepY*(y+0.5f); //subtract because y increases as we go down in sfml

                sf::Vector3f rayDirection = pixelPosition - cameraPosition; //Ray tracing
                ray r = ray(cameraPosition,rayDirection);

                sf::Color color(r.rayColor());

                setPixel(image, color, index);
            }
        }

        imageTexture.update(image.data());
        // Draw
        window.clear(sf::Color{50, 50, 50});
        window.draw(imageSprite);
        ImGui::SFML::Render(window);

        window.display();
    }

    // Shutdown ImGui-SFML
    ImGui::SFML::Shutdown();

    return 0;
}