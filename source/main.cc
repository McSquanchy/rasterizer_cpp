//
// Created by kevin on 29/09/2021.
//

#include "SDL2/SDL.h"
#include "color/colormode.h"
#include "constants/resolutions.h"
#include "renderer/renderer.h"
#include "lighting/point_light.h"
#include "utility/file/obj_reader.h"
#include "lighting/ambient_light.h"
#include "lighting/directional_light.h"

int WinMain([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  std::vector<std::unique_ptr<LightSource>> lights;
//  lights.push_back(std::make_unique<DirectionalLight>(glm::vec3{0, 0, 4}));
  lights.push_back(std::make_unique<PointLight>(glm::vec3{0, 0, 0}));
  lights.push_back(std::make_unique<AmbientLight>());

  std::vector<Triangle> geometry;
  if (!utility::ReadFile(geometry, "resources/obj/cube.obj")) {
    return -1;
  }

  Mesh cube{
      geometry,
      {0, 0, 8},
      {1, 1, 1},
      {0, 0, 0}
  };

  Renderer<sRGB, CustomResolution> renderer({480, 480});
  renderer.AddMesh(cube);
  renderer.SetCamera({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
  renderer.SetVerticalFOV(36.0f);
  renderer.SetLights(lights);
  renderer.Run();
  return 0;
}