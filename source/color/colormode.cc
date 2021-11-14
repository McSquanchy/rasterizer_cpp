//
// Created by kevin on 21/09/2021.
//

#include "colormode.h"

[[nodiscard]] glm::vec3 ColorMode::lerp(glm::vec3 x, glm::vec3 y, float t) {
  return x * (1.f - t) + y * t;
}

void RGB::Process(glm::vec3 &base_color) const {
  base_color[0] = glm::clamp(base_color[0], 0.0f, 1.0f) * 255;
  base_color[1] = glm::clamp(base_color[1], 0.0f, 1.0f) * 255;
  base_color[2] = glm::clamp(base_color[2], 0.0f, 1.0f) * 255;
}

void sRGB::Process(glm::vec3 &base_color) const {
  base_color[0] = glm::pow(glm::clamp(base_color[0], 0.0f, 1.0f), (1 / m_gamma)) * 255;
  base_color[1] = glm::pow(glm::clamp(base_color[1], 0.0f, 1.0f), (1 / m_gamma)) * 255;
  base_color[2] = glm::pow(glm::clamp(base_color[2], 0.0f, 1.0f), (1 / m_gamma)) * 255;
}