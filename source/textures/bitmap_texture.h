//
// Created by kevin on 21/10/2021.
//

#ifndef PATHTRACER_TEXTURES_BMP_BITMAPTEXTURE_H_
#define PATHTRACER_TEXTURES_BMP_BITMAPTEXTURE_H_

#include <vector>
#include <vec3.hpp>

enum class TextureType {
  NONE [[maybe_unused]] = 0,
  DIFFUSE [[maybe_unused]] = 1,
  EMISSIVE [[maybe_unused]] = 2,
  SPECULAR [[maybe_unused]] = 3,
};

class BitmapTexture {
 public:
  BitmapTexture() : m_type(TextureType::NONE) {}
  explicit BitmapTexture(TextureType type) : m_type(type) {}
  void Read(const char *);
  [[maybe_unused]] void SetType(TextureType t) {
    m_type = t;
  }
  [[nodiscard]] glm::vec3 &GetColor(float x, float y);
  [[nodiscard]] glm::vec3 &GetColorInt(int x, int y);
  [[nodiscard]] bool isDiffuse() const {
    return m_type==TextureType::DIFFUSE;
  }
  [[nodiscard]] bool isEmissive() const {
    return m_type==TextureType::EMISSIVE;
  }
  [[nodiscard]] bool isSpecular() const {
    return m_type==TextureType::SPECULAR;
  }
  [[nodiscard]] int Height() const { return m_height; }
  [[nodiscard]] int Width() const { return m_width; }

 private:
  std::vector<glm::vec3> m_colors;
  int m_width{};
  int m_height{};
  static constexpr float m_gamma = 2.2f;
  TextureType m_type;
};

#endif //PATHTRACER_TEXTURES_BMP_BITMAPTEXTURE_H_
