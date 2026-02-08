#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <optional>

int main() {
sf::RenderWindow window( sf::VideoMode( { 800, 600 } ), "thank god this works!" );
    window.setFramerateLimit(60);

    // Initialize ImGui-SFML
    if (!ImGui::SFML::Init(window)) {
        return 1; // failed to initialize
    }

    sf::Clock deltaClock;

    bool show_demo_window = true;
    float slider_value = 0.5f;

    while (window.isOpen()) {
        // SFML 3.x pollEvent returns std::optional<sf::Event>
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
        ImGui::Begin("Hello, ImGui-SFML!");
        ImGui::Text("This is a test window using SFML 3.x");
        ImGui::SliderFloat("Slider", &slider_value, 0.0f, 1.0f);
        if (ImGui::Button("Reset slider")) {
            slider_value = 0.0f;
        }
        ImGui::End();

        // Optional demo window
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Draw
        window.clear(sf::Color{50, 50, 50});
        ImGui::SFML::Render(window);
        window.display();
    }

    // Shutdown ImGui-SFML
    ImGui::SFML::Shutdown();

    return 0;
}