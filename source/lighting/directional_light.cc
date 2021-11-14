//
// Created by kevin on 09/11/2021.
//

#include "directional_light.h"

DirectionalLight::DirectionalLight(glm::vec3 direction) : LightSource(1.0f, LightType::DIRECTION),
                                                          m_world_position(-direction) {}

DirectionalLight::DirectionalLight(glm::vec3 direction, float intensity)
    : LightSource(intensity, LightType::DIRECTION), m_world_position(-direction) {}

const glm::vec3 &DirectionalLight::P() noexcept {
  return m_world_position;
}
const glm::vec3 &DirectionalLight::Direction() noexcept {
  return m_world_position;
}
