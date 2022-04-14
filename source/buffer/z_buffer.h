//
// Created by kevin on 08/11/2021.
//

#ifndef RASTERIZER_BUFFER_Z_BUFFER_H_
#define RASTERIZER_BUFFER_Z_BUFFER_H_

#include <iostream>
#include <memory>

template<is_resolution U>
class DepthBuffer {
 public:
  DepthBuffer() = delete;
  explicit DepthBuffer(std::shared_ptr<U>);
  void ResetBuffer() noexcept;
  [[nodiscard]] bool CompareAndSet(int, int, float);
 private:
  int m_size;
  std::shared_ptr<U> m_resolution;
  float m_buff[2073600]{};
};

template<is_resolution U>
DepthBuffer<U>::DepthBuffer(std::shared_ptr<U> resolution)
    : m_size(resolution->width()*resolution->height()), m_resolution(resolution) {
  for (auto i = 0; i < m_size; ++i) {
    m_buff[i] = 150.0f;
  }
}

template<is_resolution U>
void DepthBuffer<U>::ResetBuffer() noexcept {
  for (auto i = 0; i < m_size; ++i) {
    m_buff[i] = 150.0f;
  }
}

template<is_resolution U>
bool DepthBuffer<U>::CompareAndSet(int x, int y, float depth) {
  if (m_buff[y*m_resolution->width() + x] >= depth) {
    m_buff[y*m_resolution->width() + x] = depth;
    return true;
  }
  return false;
}

#endif // RASTERIZER_BUFFER_Z_BUFFER_H_
