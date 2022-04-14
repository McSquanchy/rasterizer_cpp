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

int WinMain([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  std::vector<std::unique_ptr<LightSource>> lights;
  lights.push_back(std::make_unique<PointLight>(glm::vec3{-5, 0, 0}));
  lights.push_back(std::make_unique<AmbientLight>());

  std::vector<Triangle> cube_geometry, second_cube;

  if (!utility::ReadFile(cube_geometry, "resources/obj/cube.obj", "resources/materials/pikachu_tx.bmp")) {
    return -1;
  }

  if (!utility::ReadFile(second_cube, "resources/obj/cube.obj", "resources/materials/tx_concrete.bmp")) {
    return -1;
  }

  Mesh cube{
      cube_geometry,
      {0, 0, 6},
      {1, 1, 1},
      {0, 0, 0}
  };

  Mesh other_cube{
      second_cube,
      {-1.2f, .6f, 8},
      {1, 1, 1},
      {-20, 15, 45}
  };

  Renderer<sRGB, SD> renderer;
  renderer.AddMesh(cube);
  renderer.AddMesh(other_cube);
  renderer.SetCamera({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
  renderer.SetVerticalFOV(36.0f);
  renderer.SetLights(lights);
//  renderer.SetDrawOutline(true);
  renderer.Run();

  return 0;
}