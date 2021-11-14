//
// Created by kevin on 02/11/2021.
//

#ifndef RASTERIZER_LIGHTING_LIGHT_H_
#define RASTERIZER_LIGHTING_LIGHT_H_

#include <vec3.hpp>
enum class LightType {
  POINT = 1, DIRECTION = 2, AMBIENT = 3
};

class LightSource {
 public:
  LightSource() = delete;
  [[nodiscard]] const float &Intensity() const noexcept;
  [[nodiscard]] const LightType &Type() const noexcept;
  [[nodiscard]]  virtual const glm::vec3 &P() noexcept = 0;
  [[nodiscard]]  virtual const glm::vec3 &Direction() noexcept;
  [[nodiscard]] glm::vec3 Color() const { return m_color; }
 protected:
  LightSource(float, LightType) noexcept;
 private:
  const LightType m_type;
  const glm::vec3 m_color{0.95f, 0.95f, 0.95f};
  const glm::vec3 m_dummy_direction{};
  const float m_intensity = 1.0f;
};

#endif //RASTERIZER_LIGHTING_LIGHT_H_
