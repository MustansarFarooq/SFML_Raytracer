#include <SFML/Window/Keyboard.hpp>
#include "camera.h"

void camera::movement(float dt, sf::Vector2i mouseDelta) {
    bool moved = false;
    yaw += mouseDelta.x * mouseSensitivity;
    pitch -= mouseDelta.y * mouseSensitivity;
    pitch = std::clamp(pitch, -89.0f, 89.0f);
    sf::Vector3f forward = (lookAt - lookFrom).normalized();

    float pitchR = degrees_to_radians(pitch);
    float yawR = degrees_to_radians(yaw);
    if (mouseDelta != sf::Vector2i(0, 0)) {
        forward = {
            static_cast<float>(cos(pitchR) * cos(yawR)),
            static_cast<float>(sin(pitchR)),
            static_cast<float>(cos(pitchR) * sin(yawR))
        };
        forward = forward.normalized();
        lookAt = lookFrom + forward;
        moved = true;
        
    }


    sf::Vector3f right = (vup.cross(forward)).normalized();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        lookFrom += forward * dt * moveSpeed;
        lookAt = lookFrom + forward;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        lookFrom -= forward * dt * moveSpeed;
        lookAt = lookFrom + forward;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        lookFrom += right * dt * moveSpeed;
        lookAt = lookFrom + forward;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        lookFrom -= right * dt * moveSpeed;
        lookAt = lookFrom + forward;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        lookFrom += vup * dt * moveSpeed;
        lookAt = lookFrom + forward;
        moved = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        lookFrom -= vup * dt * moveSpeed;
        lookAt = lookFrom + forward;
        moved = true;
    }


    if (moved) {
        initialize();
        resetBuffer();
    }
}
