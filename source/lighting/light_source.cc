//
// Created by kevin on 02/11/2021.
//

#include "light_source.h"

LightSource::LightSource(const float intensity, const LightType type) noexcept: m_type(type), m_intensity(intensity) {}

const float &LightSource::Intensity() const noexcept {
  return m_intensity;
}

const LightType &LightSource::Type() const noexcept {
  return m_type;
}

const glm::vec3 &LightSource::Direction() noexcept {
  return m_dummy_direction;
}

