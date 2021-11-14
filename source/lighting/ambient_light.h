//
// Created by kevin on 09/11/2021.
//

#ifndef RASTERIZER_SOURCE_LIGHTING_AMBIENT_LIGHT_H_
#define RASTERIZER_SOURCE_LIGHTING_AMBIENT_LIGHT_H_

#include "light_source.h"

class AmbientLight : public LightSource {
 public:
  AmbientLight();
  explicit AmbientLight(float);
  [[nodiscard]] const glm::vec3 &P() noexcept override;
 private:
  const glm::vec3 m_world_position{0, 0, 0};
};

#endif //RASTERIZER_SOURCE_LIGHTING_AMBIENT_LIGHT_H_
