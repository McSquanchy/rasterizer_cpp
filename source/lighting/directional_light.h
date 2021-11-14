//
// Created by kevin on 09/11/2021.
//

#ifndef RASTERIZER_LIGHTING_DIRECTIONAL_LIGHT_H_
#define RASTERIZER_LIGHTING_DIRECTIONAL_LIGHT_H_

#include <vec3.hpp>
#include "light_source.h"

class DirectionalLight : public LightSource {
 public:
  DirectionalLight() = delete;
  explicit DirectionalLight(glm::vec3);
  DirectionalLight(glm::vec3, float);
  [[nodiscard]] const glm::vec3 &P() noexcept override;
  [[nodiscard]] const glm::vec3 &Direction() noexcept override;
 private:
  const glm::vec3 m_world_position{};
};

#endif //RASTERIZER_LIGHTING_DIRECTIONAL_LIGHT_H_
