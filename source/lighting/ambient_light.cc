//
// Created by kevin on 09/11/2021.
//

#include "ambient_light.h"

AmbientLight::AmbientLight() : LightSource(0.2f, LightType::AMBIENT) {}

AmbientLight::AmbientLight(float intensity) : LightSource(intensity, LightType::AMBIENT) {}

const glm::vec3 &AmbientLight::P() noexcept {
  return m_world_position;
}
