//
// Created by kevin on 28/09/2021.
//

#ifndef PATHTRACER_CONSTANTS_COLORS_H_
#define PATHTRACER_CONSTANTS_COLORS_H_

#include <vec3.hpp>

namespace Colors {
namespace {
[[maybe_unused]] inline constexpr glm::vec3 RED() { return {0.95f, 0, 0}; }
[[maybe_unused]] inline constexpr glm::vec3 GREEN() { return {0, 0.95f, 0}; }
[[maybe_unused]] inline constexpr glm::vec3 BLUE() { return {0, 0, 0.95f}; }
[[maybe_unused]] inline constexpr glm::vec3 GRAY() {
  return {0.5f, 0.5f, 0.5f};
}
[[maybe_unused]] inline constexpr glm::vec3 WHITE() {
  return {0.98f, 0.98, 0.98f};
}
[[maybe_unused]] inline constexpr glm::vec3 CYAN() {
  return {0.87f, 0.95, 0.95f};
}
[[maybe_unused]] inline constexpr glm::vec3 YELLOW() {
  return {0.95f, 0.95f, 0};
}
[[maybe_unused]] inline constexpr glm::vec3 BLACK() { return {0, 0, 0}; }
[[maybe_unused]] inline constexpr glm::vec3 BACKGROUND() {
  return {250 / 255.f, 250 / 255.f, 250 / 255.f};
}
[[maybe_unused]] inline constexpr glm::vec3 LIGHTING() {
  return {10.0f, 10.0f, 10.0f};
}
[[maybe_unused]] inline constexpr glm::vec3 MOUNTAIN_MEADOW() {
  return {9 / 255.f, 188 / 255.f, 138 / 255.f};
}
[[maybe_unused]] inline constexpr glm::vec3 HEAT_WAVE() {
  return {241 / 255.f, 115 / 255.f, 0.f};
}
[[maybe_unused]] inline constexpr glm::vec3 BONE() {
  return {224 / 255.f, 221 / 255.f, 207 / 255.f};
}
[[maybe_unused]] inline constexpr glm::vec3 FIERY_ROSE() {
  return {252 / 255.f, 100 / 255.f, 113 / 255.f};
}
[[maybe_unused]] inline constexpr glm::vec<3, int> WIREFRAME_COLOR() {
  return {245, 245, 245};
}
} // namespace
} // namespace Colors

#endif // PATHTRACER_CONSTANTS_COLORS_H_
