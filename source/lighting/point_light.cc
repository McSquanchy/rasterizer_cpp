//
// Created by kevin on 02/11/2021.
//

#include "point_light.h"

PointLight::PointLight(glm::vec3 origin) : LightSource(1.0f, LightType::POINT), m_world_position(origin) {}
PointLight::PointLight(glm::vec3 origin, float intensity) : LightSource(intensity, LightType::POINT),
                                                            m_world_position(origin) {}

const glm::vec3 &PointLight::P() noexcept {
  return m_world_position;
}
