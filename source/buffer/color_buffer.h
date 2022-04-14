//
// Created by kevin on 08/11/2021.
//

#ifndef RASTERIZER_BUFFER_COLORBUFFER_H_
#define RASTERIZER_BUFFER_COLORBUFFER_H_

#include <memory>
#include <utility>
#include <vector>
#include "../constants/colors.h"

template<is_resolution U>
class ColorBuffer {
 public:
  explicit ColorBuffer(std::shared_ptr<U>);
  [[nodiscard]] glm::vec3 &GetPixelColor(int, int);
  void SetPixelColor(int, int, glm::vec3 &);
  void ClearBuffer() noexcept;

 private:
  std::shared_ptr<U> m_resolution;
  glm::vec3 m_buff[2073600]{};
  unsigned long m_drawCount = 0;
};

template<is_resolution U>
ColorBuffer<U>::ColorBuffer(std::shared_ptr<U> resolution)
    : m_resolution(resolution), m_buff{} {
  for (auto i = 0; i < m_resolution->height()*m_resolution->width(); ++i) {
    m_buff[i] = glm::vec3{};
  }
}

template<is_resolution U>
glm::vec3 &ColorBuffer<U>::GetPixelColor(int x, int y) {
  return m_buff[y*m_resolution->width() + x];
}

template<is_resolution U>
void ColorBuffer<U>::SetPixelColor(int x, int y, glm::vec3 &value) {
  m_buff[y*m_resolution->width() + x] = value;
  ++m_drawCount;
}

template<is_resolution U>
void ColorBuffer<U>::ClearBuffer() noexcept {
  for (auto i = 0; i < m_resolution->height()*m_resolution->width(); ++i) {
    m_buff[i][0] = 0;
    m_buff[i][1] = 0;
    m_buff[i][2] = 0;
  }
  m_drawCount = 0;
}

#endif // RASTERIZER_BUFFER_COLORBUFFER_H_
