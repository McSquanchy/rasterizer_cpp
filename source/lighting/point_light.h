//
// Created by kevin on 02/11/2021.
//

#ifndef RASTERIZER_LIGHTING_POINT_LIGHT_H_
#define RASTERIZER_LIGHTING_POINT_LIGHT_H_

#include <vec3.hpp>
#include "light_source.h"

class PointLight : public LightSource {
 public:
  PointLight() = delete;
  explicit PointLight(glm::vec3);
  PointLight(glm::vec3, float);
  [[nodiscard]] const glm::vec3 &P() noexcept override;
 private:
  const glm::vec3 m_world_position{};
};

#endif //RASTERIZER_LIGHTING_POINT_LIGHT_H_
