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
class Buffer {
 public:
  explicit Buffer(std::shared_ptr<U>);
  [[nodiscard]] glm::vec3 &GetPixelColor(int, int);
  void SetPixelColor(int, int, glm::vec3 &);
  void ClearBuffer() noexcept;

 private:
  std::shared_ptr<U> m_resolution;
  std::vector<glm::vec3> m_pixel_buffer;
};

template<is_resolution U>
Buffer<U>::Buffer(std::shared_ptr<U> resolution)
    : m_resolution(resolution), m_pixel_buffer{} {
  m_pixel_buffer.resize(static_cast<int>(m_resolution->height()*m_resolution->width()));
  for (auto i = 0; i < m_resolution->height()*m_resolution->width(); ++i) {
    m_pixel_buffer.emplace_back();
  }
}

template<is_resolution U>
glm::vec3 &Buffer<U>::GetPixelColor(int x, int y) {
  return m_pixel_buffer[y*m_resolution->width() + x];
}

template<is_resolution U>
void Buffer<U>::SetPixelColor(int x, int y, glm::vec3 &value) {
  m_pixel_buffer[y*m_resolution->width() + x] = value;
}

template<is_resolution U>
void Buffer<U>::ClearBuffer() noexcept {
  for (auto i = 0; i < m_resolution->height()*m_resolution->width(); ++i) {
    m_pixel_buffer[i][0] = 0;
    m_pixel_buffer[i][1] = 0;
    m_pixel_buffer[i][2] = 0;
  }
}

#endif // RASTERIZER_BUFFER_COLORBUFFER_H_
