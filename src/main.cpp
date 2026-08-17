#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "SFML/System/Vector3.hpp"
#include "bvh.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "scene.h"
#include "sphere.h"
const int screenWidth = 800;
const int screenHeight = 600;

sf::Texture screenTexture;
sf::Sprite imageSprite(screenTexture);
scene scene;
camera cam(screenWidth, screenHeight, scene.materials);

void coverScreen(hittable_list& world) {
  cam.samplesPerPixel = 1;
  cam.maxDepth = 10;
  cam.fovDegrees = 45.f;
  cam.lookFrom = {13, 2, 3};
  cam.lookAt = {0, 0, 0};

  cam.background = {0.5, 0.8, 0.92};
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choose_mat = random_double();
      sf::Vector3f center(a + 0.9 * random_double(), 0.2,
                          b + 0.9 * random_double());

      if ((center - sf::Vector3f(4, 0.2, 0)).length() > 0.9) {
        material sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = randomVector3f() * randomVector3f();
          sphere_material.type = materialType::lambertian;
          sphere_material.albedo = albedo;

          scene.materials.push_back(sphere_material);
          int lambertianI = scene.materials.size() - 1;

          nSphere s;
          s.center = center;
          s.radius = 0.2f;
          s.materialIndex = lambertianI;
          scene.spheres.push_back(s);

        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = randomVector3f(0.5, 1);
          auto fuzz = random_double(0, 0.5);

          sphere_material.type = materialType::metal;
          sphere_material.albedo = albedo;
          sphere_material.fuzz = fuzz;
          scene.materials.push_back(sphere_material);
          int metalI = scene.materials.size() - 1;

          nSphere s;
          s.center = center;
          s.radius = 0.2f;
          s.materialIndex = metalI;
          scene.spheres.push_back(s);
        } else {
          // glass
          sphere_material.type = materialType::dielectric;
          sphere_material.refractiveIndex = 1.5f;
          scene.materials.push_back(sphere_material);
          int glassI = scene.materials.size() - 1;

          nSphere s;
          s.center = center;
          s.radius = 0.2f;
          s.materialIndex = glassI;
          scene.spheres.push_back(s);
        }
      }
    }
  }

  material material1;
  material1.type = materialType::dielectric;
  material1.refractiveIndex = 1.5;
  scene.materials.push_back(material1);
  int mat1 = scene.materials.size() - 1;

  nSphere sphere1;
  sphere1.center = {0, 1, 0};
  sphere1.radius = 1;
  sphere1.materialIndex = mat1;
  scene.spheres.push_back(sphere1);

  material material2;
  material2.type = materialType::lambertian;
  material2.albedo = {0.4, 0.2, 0.1};
  scene.materials.push_back(material2);
  int mat2 = scene.materials.size() - 1;

  nSphere sphere2;
  sphere2.center = {-4, 1, 0};
  sphere2.radius = 1;
  sphere2.materialIndex = mat2;
  scene.spheres.push_back(sphere2);

  material material3;
  material3.type = materialType::metal;
  material3.albedo = {0.7, 0.6, 0.5};
  material3.fuzz = 0;
  scene.materials.push_back(material3);
  int mat3 = scene.materials.size() - 1;

  nSphere sphere3;
  sphere3.center = {4, 1, 0};
  sphere3.radius = 1;
  sphere3.materialIndex = mat3;
  scene.spheres.push_back(sphere3);
}

void Spheres(hittable_list& world) {
  cam.samplesPerPixel = 1;
  cam.maxDepth = 90;
  cam.fovDegrees = 90.f;

  cam.lookFrom = {0, 1, 1};
  cam.lookAt = {0, 1, -1};
  // cam.background = {0.016, 0.1, 0.25};

  material light;
  light.type = materialType::diffuseLight;
  light.emmitted = sf::Vector3f(1, 1, 1);
  scene.materials.push_back(light);
  int lightMat = scene.materials.size() - 1;
  scene.quads.push_back(makeQuad(sf::Vector3f(-2, 2.2, -2), sf::Vector3f(4, 0, 0), sf::Vector3f(0, 0, 2), lightMat));

  material glass;
  glass.type = materialType::dielectric;
  glass.refractiveIndex = 1.5f;
  scene.materials.push_back(glass);
  int material1 = scene.materials.size() - 1;

  nSphere sphere1;
  sphere1.center = sf::Vector3f(0, 1, -1);
  sphere1.radius = 0.4f;
  sphere1.materialIndex = material1;
  scene.spheres.push_back(sphere1);

  // world.add(std::make_shared<class sphere>(sf::Vector3f(0, 1, -1), 0.4f, material1));

  material lambertian;
  lambertian.type = materialType::lambertian;
  lambertian.albedo = sf::Vector3f(0.85f, 0.06f, 0.8f);
  scene.materials.push_back(lambertian);
  int material2 = scene.materials.size() - 1;

  nSphere sphere2;
  sphere2.center = sf::Vector3f(-1, 1, -1);
  sphere2.radius = 0.4f;
  sphere2.materialIndex = material2;
  scene.spheres.push_back(sphere2);
  // world.add(std::make_shared<class sphere>(sf::Vector3f(-1, 1, -1), 0.4f, material2));

  material metal;
  metal.type = materialType::metal;
  metal.albedo = sf::Vector3f(0.05f, 0.55f, 0.2f);
  scene.materials.push_back(metal);
  int material3 = scene.materials.size() - 1;

  nSphere sphere3;
  sphere3.center = {1, 1, -1};
  sphere3.radius = 0.4f;
  sphere3.materialIndex = material3;
  scene.spheres.push_back(sphere3);
  // world.add(std::make_shared<class sphere>(sf::Vector3f(1, 1, -1), 0.4f, material3));
}
//
// void earth(hittable_list &world) {
//   cam.samplesPerPixel = 1;
//   cam.maxDepth = 90;
//   cam.fovDegrees = 45.f;
//
//   cam.lookFrom = {2, 2, 1};
//   cam.lookAt = {0, 1, -1};
//
//   auto earthTexture = std::make_shared<imageTexture>("earthTexture.jpg");
//   auto material1 = std::make_shared<lambertian>(earthTexture);
//   world.add(
//       std::make_shared<class sphere>(sf::Vector3f(0, 1, -1), 0.4,
//       material1));
// }
//
// void light(hittable_list &world) {
//   cam.samplesPerPixel = 1;
//   cam.maxDepth = 90;
//   cam.fovDegrees = 90.f;
//
//   cam.lookFrom = {0, 1, 1};
//   cam.lookAt = {0, 1, -1};
//
//   auto material1 = std::make_shared<lambertian>(sf::Vector3f(0.85, 0.06,
//   0.25)); world.add(
//       std::make_shared<class sphere>(sf::Vector3f(0, 1, -1), 0.4,
//       material1))
//
//   auto material2 = std::make_shared<diffuseLight>(sf::Vector3f(1, 1, 1));
//   world.add(std::make_shared<quad>(sf::Vector3f(0.5, 1, -1),
//                                    sf::Vector3f(0, 0, 1), sf::Vector3f(0, 1,
//                                    0), material2));
//
//   auto material3 = std::make_shared<metal>(sf::Vector3f(0.8, 0.8, 0.8), 0);
//   world.add(std::make_shared<quad>(sf::Vector3f(-0.5, 1, -1),
//                                    sf::Vector3f(0, 0, 0.5),
//                                    sf::Vector3f(0, 0.5, 0), material3));
// }
// void quadsAndSphere(hittable_list &world) {
//   cam.samplesPerPixel = 1;
//   cam.maxDepth = 90;
//   cam.fovDegrees = 90.f;
//   cam.background = {0.5, 0.8, 0.92};
//   cam.lookFrom = {0, 1, 1};
//   cam.lookAt = {0, 1, -1};
//
//   auto material1 = std::make_shared<lambertian>(sf::Vector3f(0.85, 0.06,
//   0.25)); world.add(
//       std::make_shared<class sphere>(sf::Vector3f(0, 1, -1), 0.4,
//       material1));
//
//   auto material2 = std::make_shared<metal>(sf::Vector3f(0.2, 0.2, 0.2), 0);
//   world.add(std::make_shared<quad>(sf::Vector3f(0.5, 1, -1),
//                                    sf::Vector3f(0, 0, 3), sf::Vector3f(0, 1,
//                                    0), material2));
// }
//
void sphere(hittable_list& world) {
  cam.samplesPerPixel = 1;
  cam.maxDepth = 90;
  cam.fovDegrees = 90.f;
  cam.background = {0.5, 0.8, 0.92};
  cam.lookFrom = {0, 1, 1};
  cam.lookAt = {0, 1, -1};

  material material1;
  material1.albedo = sf::Vector3f(1.0, 0.f, 0.f);
  material1.type = materialType::lambertian;
  scene.materials.push_back(material1);
  int sphereMat = scene.materials.size() - 1;
  nSphere sphere;
  sphere.center = {0, 1, -1};
  sphere.radius = 0.4;
  sphere.materialIndex = sphereMat;
  scene.spheres.push_back(sphere);
  // world.add(std::make_shared<class sphere>(sf::Vector3f(0, 1, -1), 0.4, sphereMat));
}
int main(int, char**) {
  // sfml whatever jargon idk boiler plate spaghetti
  sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "SFML RayTracer");

  window.setFramerateLimit(60);
  screenTexture.resize({screenWidth, screenHeight});
  imageSprite.setTexture(screenTexture, true);
  // Initialize ImGui-SFML
  if (!ImGui::SFML::Init(window)) {
    return 1;  // failed to initialize and terminate
  }
  sf::Clock deltaClock;

  // world stuff
  // ground

  material ground;
  ground.albedo = sf::Vector3f(0.8f, 0.2f, 0.2f);
  ground.fuzz = 0.2;
  ground.type = materialType::lambertian;
  scene.materials.push_back(ground);
  int groundMaterial = scene.materials.size() - 1;

  scene.quads.push_back(makeQuad({-100, 0, -100}, {200, 0, 0}, {0, 0, 200}, groundMaterial));
  // world.add(std::make_shared<quad>(sf::Vector3f(-100, 0, -100), sf::Vector3f(200, 0, 0), sf::Vector3f(0, 0, 200), groundMaterial));

  hittable_list world = hittable_list(scene);
  Spheres(world);

  buildSceneBVH(scene);

  std::cout << scene.nodes.size() << '\n';
  std::cout << scene.primitives.size() << '\n';
  std::cout << std::endl;
  std::cout << std::endl;

  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(0.5);

  float seconds = 0;
  std::atomic<bool> rendering = false;
  std::thread renderThread;

  auto t1 = std::chrono::steady_clock::now();

  sf::Vector2i windowCenter = {static_cast<int>(window.getSize().x / 2),
                               static_cast<int>(window.getSize().y / 2)};
  sf::Mouse::setPosition(windowCenter, window);
  bool firstFrame = true;
  while (window.isOpen()) {
    std::optional<sf::Event> event;
    while ((event = window.pollEvent())) {
      ImGui::SFML::ProcessEvent(window, *event);
      // Check for window close
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    auto t2 = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = (t2 - t1);
    float deltaTime = elapsed.count();
    t1 = t2;

    // mouse handeling
    sf::Vector2i delta;
    bool cameraActive = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    if (!firstFrame && cameraActive) {
      window.setMouseCursorVisible(false);
      window.setMouseCursorGrabbed(true);
      sf::Vector2i currentMouse = sf::Mouse::getPosition(window);
      delta = currentMouse - windowCenter;
      sf::Mouse::setPosition(windowCenter, window);
    } else {
      window.setMouseCursorVisible(true);
      window.setMouseCursorGrabbed(false);
    }
    cam.movement(deltaTime, delta);

    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::Begin("Stats", nullptr);
    ImGui::Text("FPS: %.2f", 1.f / cam.lastFrameTime);
    ImGui::InputInt("Samples Per Pixel", &cam.samplesPerPixel);

    // ImGui::ColorEdit3("Ground Color", &groundMaterial->albedo.x);

    if (ImGui::Button(rendering
                          ? "Stop Rendering"
                          : "Begin Rendering")) {
      if (!rendering) {
        rendering = true;

        renderThread = std::thread([&] {
          while (rendering.load(std::memory_order_relaxed)) {
            cam.render(world);
          }
        });
      } else {
        rendering = false;

        if (renderThread.joinable())
          renderThread.join();

        cam.resetBuffer();
      }
    }
    float progress =
        float(cam.completedPixels.load(std::memory_order_relaxed)) /
        cam.totalPixels;
    ImGui::ProgressBar(progress);
    ImGui::Text("Accumulated Frames: %.0f", cam.accumulatedFrame);
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

    firstFrame = false;
  }

  ImGui::SFML::Shutdown();

  return 0;
}
