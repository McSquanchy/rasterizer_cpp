//
// Created by kevin on 27/09/2021.
//

#ifndef PATHTRACER_RENDERER_RESOLUTIONS_H_
#define PATHTRACER_RENDERER_RESOLUTIONS_H_

#include <ostream>

class Resolution {
 protected:
  const std::pair<int, int> m_resolution;
  explicit Resolution(std::pair<int, int> t) : m_resolution(std::move(t)) {}

 public:
  virtual ~Resolution() = default;
  virtual constexpr int width() = 0;
  virtual constexpr int height() = 0;
  constexpr float aspectRatio() {
    return static_cast<float>(width()) / static_cast<float>(height());
  }
  friend std::ostream &operator<<(std::ostream &stream, const Resolution &res) {
    stream << "Width: " << res.m_resolution.first << " / Height: " << res.m_resolution.second;
    return stream;
  }
};

class [[maybe_unused]] SD : public Resolution {
 public:
  SD() : Resolution({640, 480}) {}
  constexpr int width() override { return m_resolution.first; }
  constexpr int height() override { return m_resolution.second; }
};

class [[maybe_unused]] HD : public Resolution {
 public:
  HD() : Resolution({1280, 720}) {}
  constexpr int width() override { return m_resolution.first; }
  constexpr int height() override { return m_resolution.second; }
};

class [[maybe_unused]] FullHD : public Resolution {
 public:
  FullHD() : Resolution({1920, 1080}) {}
  constexpr int width() override { return m_resolution.first; }
  constexpr int height() override { return m_resolution.second; }
};

class [[maybe_unused]] QHD : public Resolution {
 public:
  QHD() : Resolution({2560, 1440}) {}
  constexpr int width() override { return m_resolution.first; }
  constexpr int height() override { return m_resolution.second; }
};

class [[maybe_unused]] CustomResolution : public Resolution {
 public:
  explicit CustomResolution(std::pair<int, int> resolution) : Resolution(resolution) {}
  constexpr int width() override { return m_resolution.first; }
  constexpr int height() override { return m_resolution.second; }
};

template<typename T>
concept is_resolution = std::derived_from<T, Resolution>;

#endif //PATHTRACER_RENDERER_RESOLUTIONS_H_