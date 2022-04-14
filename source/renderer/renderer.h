//
// Created by kevin on 27/09/2021.
//

#ifndef RASTERIZER_RENDERER_RENDERER_H_
#define RASTERIZER_RENDERER_RENDERER_H_

#include "../buffer/color_buffer.h"
#include "../color/colormode.h"
#include "../constants/colors.h"
#include "../constants/resolutions.h"
#include "../lighting/light_source.h"
#include "../mesh/mesh.h"
#include "SDL2/SDL.h"
#include "../buffer/z_buffer.h"
#include <chrono>
#include <cstdlib>
#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include <functional>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <thread>

template<color_strategy T, is_resolution U>
class Renderer {
 public:
  Renderer();
  [[maybe_unused]] explicit Renderer(std::pair<int, int>);
  ~Renderer() = default;
  [[maybe_unused]] void Run() noexcept;
  void AddMesh(Mesh &);
  void SetCamera(glm::vec3, glm::vec3, glm::vec3);
  void SetVerticalFOV(float);
  void SetLights(std::vector<std::unique_ptr<LightSource>> &);
  void SetDrawOutline(bool);
 private:
  void Rasterize() const noexcept;
  void Render() noexcept;
//  void RenderPrimitive(const std::unique_ptr<Mesh> &, Triangle &) const noexcept;
  void ShadeFragments(const std::unique_ptr<Mesh> &, Triangle &) const noexcept;
  void DrawPixel(int x, int y, glm::vec3 &);
  bool initializeSDL();
  glm::vec4 InterpolateAttribute(glm::vec4 &, glm::vec4 &, glm::vec4 &,
                                 float, float) const;
  void ModelToWorld(Triangle &, const glm::mat4 &) const;
  void TransformNormals(Triangle &, const glm::mat4 &) const;
  void NormalizeVertex(glm::vec4 &) const;
  void NormalizeTriangle(Triangle &) const;
  void ToClipSpace(Triangle &, const glm::mat4 &) const;
  void CameraToScreen(Triangle &) const;
  void DrawOutline(Triangle &) const;
  glm::vec3 NormalizedDeviceCoordinates(glm::vec4 &) const;
  [[maybe_unused]] void PrintMatrix(glm::mat4 &) const noexcept;
  std::vector<std::unique_ptr<Mesh>> m_scene_objects{};
  std::vector<std::unique_ptr<LightSource>> m_scene_lights{};
  std::shared_ptr<T> m_color_mode;
  std::shared_ptr<U> m_resolution;
  std::unique_ptr<ColorBuffer<U>> m_image_buffer;
  glm::vec3 m_camera_origin{};
  glm::vec3 m_camera_lookAt{};
  glm::vec3 m_camera_up{};
  glm::mat4 m_camera_matrix{1.0f};
  glm::mat4 m_projection{1.0f};
  glm::vec3 m_objectRotation{0, 20.0f, 0};
  float m_vertical_fov = 45.0f;
  SDL_Event m_window_event{};
  std::unique_ptr<DepthBuffer<U>> m_zBuff;
  std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>> m_window =
      nullptr;
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>
      m_window_renderer = nullptr;
  bool m_draw_wireframe = false;
  bool m_isRunning;
  bool m_freeze = false;
  bool m_bilinearFiltering = false;
  bool m_render_frame = false;
};

template<color_strategy T, is_resolution U>
[[maybe_unused]] void
Renderer<T, U>::PrintMatrix(glm::mat4 &matrix) const noexcept {
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "["
            << static_cast<float>(matrix[0][0]) << "," << matrix[0][1] << ","
            << matrix[0][2] << "," << matrix[0][3] << "]\n";
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "["
            << static_cast<float>(matrix[1][0]) << "," << matrix[1][1] << ","
            << matrix[1][2] << "," << matrix[1][3] << "]\n";
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "["
            << static_cast<float>(matrix[2][0]) << "," << matrix[2][1] << ","
            << matrix[2][2] << "," << matrix[2][3] << "]\n";
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "["
            << static_cast<float>(matrix[3][0]) << "," << matrix[3][1] << ","
            << matrix[3][2] << "," << matrix[3][3] << "]\n\n";
}

template<color_strategy T, is_resolution U>
Renderer<T, U>::Renderer()
    : m_color_mode(std::make_shared<T>()),
      m_resolution(std::make_shared<U>()),
      m_image_buffer(std::make_unique<ColorBuffer<U>>(m_resolution)),
      m_zBuff(std::make_unique<DepthBuffer<U>>(m_resolution)) {
  if (!initializeSDL()) {
    std::cerr << "Could not initialize SDL. Aborting...\n";
  }
  m_projection = glm::perspective(m_vertical_fov,
                                  (static_cast<float>(m_resolution->width())/
                                      static_cast<float>(m_resolution->height())),
                                  1.f, 150.f);
}

template<color_strategy T, is_resolution U>
[[maybe_unused]] Renderer<T, U>::Renderer(std::pair<int, int> window_size)
    : m_color_mode(std::make_shared<T>()),
      m_resolution(std::make_shared<U>(window_size)),
      m_image_buffer(std::make_unique<ColorBuffer<U>>(m_resolution)),
      m_zBuff(std::make_unique<DepthBuffer<U>>(m_resolution)) {
  if (!initializeSDL()) {
    std::cerr << "Could not initialize SDL. Aborting...\n";
  }
  m_projection = glm::perspective(m_vertical_fov,
                                  (static_cast<float>(m_resolution->width())/
                                      static_cast<float>(m_resolution->height())),
                                  1.f, 150.f);
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::AddMesh(Mesh &g) {
  m_scene_objects.push_back(std::make_unique<Mesh>(std::move(g)));
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::SetCamera(glm::vec3 origin, glm::vec3 lookAt,
                               glm::vec3 up) {
  m_camera_origin = origin;
  m_camera_lookAt = lookAt;
  m_camera_up = up;
  m_camera_matrix = glm::lookAt(m_camera_origin, m_camera_lookAt, m_camera_up);
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::SetVerticalFOV(float fov) {
  m_vertical_fov = fov;
  m_projection = glm::perspective(m_vertical_fov,
                                  (static_cast<float>(m_resolution->width())/
                                      static_cast<float>(m_resolution->height())),
                                  1.f, 150.f);
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::NormalizeVertex(glm::vec4 &vertex) const {
  vertex /= vertex.w;
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::NormalizeTriangle(Triangle &triangle) const {
  NormalizeVertex(triangle.V0().ClipSpacePos());
  NormalizeVertex(triangle.V1().ClipSpacePos());
  NormalizeVertex(triangle.V2().ClipSpacePos());
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::TransformNormals(Triangle &triangle,
                                      const glm::mat4 &m) const {
  triangle.V0().HomNormal() = glm::normalize(m*triangle.V0().HomNormal());
  triangle.V1().HomNormal() = glm::normalize(m*triangle.V1().HomNormal());
  triangle.V2().HomNormal() = glm::normalize(m*triangle.V2().HomNormal());
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::ModelToWorld(Triangle &triangle,
                                  const glm::mat4 &m) const {
  triangle.V0().UpdateWorldPosition(m*triangle.V0().HomLocalPos());
  triangle.V1().UpdateWorldPosition(m*triangle.V1().HomLocalPos());
  triangle.V2().UpdateWorldPosition(m*triangle.V2().HomLocalPos());
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::ToClipSpace(Triangle &triangle,
                                 const glm::mat4 &mvp) const {
  triangle.V0().ClipSpacePos() = mvp*triangle.V0().HomLocalPos();
  triangle.V1().ClipSpacePos() = mvp*triangle.V1().HomLocalPos();
  triangle.V2().ClipSpacePos() = mvp*triangle.V2().HomLocalPos();
  NormalizeTriangle(triangle);
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::CameraToScreen(Triangle &triangle) const {
  triangle.V0().ScreenPosition() =
      NormalizedDeviceCoordinates(triangle.V0().ClipSpacePos());
  triangle.V1().ScreenPosition() =
      NormalizedDeviceCoordinates(triangle.V1().ClipSpacePos());
  triangle.V2().ScreenPosition() =
      NormalizedDeviceCoordinates(triangle.V2().ClipSpacePos());
}

template<color_strategy T, is_resolution U>
glm::vec3 Renderer<T, U>::NormalizedDeviceCoordinates(glm::vec4 &vertex) const {
  return {vertex.x*m_resolution->width() + m_resolution->width()/2,
          vertex.y*m_resolution->height() + m_resolution->height()/2, 0};
}

template<color_strategy T, is_resolution U>
glm::vec4 Renderer<T, U>::InterpolateAttribute(glm::vec4 &a, glm::vec4 &b, glm::vec4 &c,
                                               float u, float v) const {
  auto result = a + u*(b - a) + v*(c - a);
  result /= result.w;
  return result;
}

template<color_strategy T, is_resolution U>
[[maybe_unused]] void Renderer<T, U>::Run() noexcept {
  std::chrono::time_point<std::chrono::system_clock> t1, t2;
  while (m_isRunning) {
    if (!m_freeze || m_render_frame) {
      t1 = std::chrono::high_resolution_clock::now();
      SDL_SetRenderDrawColor(m_window_renderer.get(),
                             0,
                             0,
                             0, 255);
      SDL_RenderClear(m_window_renderer.get());
      m_zBuff->ResetBuffer();
      Rasterize();
      if (!m_draw_wireframe)
        Render();

      SDL_RenderPresent(m_window_renderer.get());
      t2 = std::chrono::high_resolution_clock::now();
      auto ts = ((float) std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()/1000.0f);
      glm::vec3 rotation = ts*m_objectRotation;
      m_scene_objects[0]->Rotate(rotation);
      m_render_frame = false;
    }
    SDL_PollEvent(&m_window_event);
    switch (m_window_event.type) {
      case SDL_KEYDOWN:
        if (m_window_event.key.keysym.sym==SDLK_SPACE)
          m_freeze = !m_freeze;
        if (m_window_event.key.keysym.sym==SDLK_b) {
          m_bilinearFiltering = !m_bilinearFiltering;
          std::cout << (m_bilinearFiltering ? "bilinear filtering on" : "bilinear filtering off") << "\n";
        }
        if (m_window_event.key.keysym.sym==SDLK_w) {
          m_draw_wireframe = !m_draw_wireframe;
//          m_objectRotation.y = m_draw_wireframe ? 2.0f : 20.0f;
        }

        if (m_window_event.key.keysym.sym==SDLK_p && m_freeze)
          m_render_frame = true;
        break;
      case SDL_QUIT:m_isRunning = false;
        break;
      default:break;
    }
  }
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::Rasterize() const noexcept {
  for (auto &object: m_scene_objects) {
    auto mvp = m_projection*m_camera_matrix*object->ModelMatrix();
    for (auto &triangle: object->Triangles()) {
      triangle.ResetTransientProperties();
      ModelToWorld(triangle, object->ModelMatrix());
      TransformNormals(triangle,
                       (glm::determinant(object->ModelMatrix())*(glm::transpose(glm::inverse(object->ModelMatrix())))));
      ToClipSpace(triangle, mvp);
      CameraToScreen(triangle);
      glm::vec3 ab = triangle.V1().HomWorldPos() - triangle.V0().HomWorldPos();
      glm::vec3 ac = triangle.V2().HomWorldPos() - triangle.V0().HomWorldPos();
      auto normal_vector = glm::cross(ab, ac);
      if (glm::dot(normal_vector,
                   glm::normalize(glm::vec3{triangle.V0().HomWorldPos()} -
                       m_camera_origin)) >= 0) // culling
        continue;
      if (m_draw_wireframe) {
        DrawOutline(triangle);
      } else
        ShadeFragments(object, triangle);
    }
  }
}

//template<color_strategy T, is_resolution U>
//void Renderer<T, U>::RenderPrimitive(const std::unique_ptr<Mesh> &obj,
//                                     Triangle &triangle) const noexcept {
//  FillPrimitive(obj, triangle);
//}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::ShadeFragments(const std::unique_ptr<Mesh> &obj,
                                    Triangle &triangle) const noexcept {
  int max_x = std::max((int) triangle.V0().ScreenPosition().x,
                       std::max((int) triangle.V1().ScreenPosition().x,
                                (int) triangle.V2().ScreenPosition().x));
  int min_x = std::min((int) triangle.V0().ScreenPosition().x,
                       std::min((int) triangle.V1().ScreenPosition().x,
                                (int) triangle.V2().ScreenPosition().x));
  int max_y = std::max((int) triangle.V0().ScreenPosition().y,
                       std::max((int) triangle.V1().ScreenPosition().y,
                                (int) triangle.V2().ScreenPosition().y));
  int min_y = std::min((int) triangle.V0().ScreenPosition().y,
                       std::min((int) triangle.V1().ScreenPosition().y,
                                (int) triangle.V2().ScreenPosition().y));

  glm::vec2 vs1{
      triangle.V1().ScreenPosition().x - triangle.V0().ScreenPosition().x,
      triangle.V1().ScreenPosition().y - triangle.V0().ScreenPosition().y};
  glm::vec2 vs2{
      triangle.V2().ScreenPosition().x - triangle.V0().ScreenPosition().x,
      triangle.V2().ScreenPosition().y - triangle.V0().ScreenPosition().y};

  auto a_iPos = m_camera_matrix*obj->ModelMatrix()*triangle.V0().HomLocalPos();
  auto b_iPos = m_camera_matrix*obj->ModelMatrix()*triangle.V1().HomLocalPos();
  auto c_iPos = m_camera_matrix*obj->ModelMatrix()*triangle.V2().HomLocalPos();

  triangle.V0().HomDiffColor() /= a_iPos.z;
  triangle.V0().HomWorldPos() /= a_iPos.z;
  triangle.V0().HomNormal() /= a_iPos.z;
  triangle.V0().HomSpecColor() /= a_iPos.z;
  triangle.V0().HomTexPos() /= a_iPos.z;

  triangle.V1().HomDiffColor() /= b_iPos.z;
  triangle.V1().HomWorldPos() /= b_iPos.z;
  triangle.V1().HomNormal() /= b_iPos.z;
  triangle.V1().HomSpecColor() /= b_iPos.z;
  triangle.V1().HomTexPos() /= b_iPos.z;

  triangle.V2().HomDiffColor() /= c_iPos.z;
  triangle.V2().HomWorldPos() /= c_iPos.z;
  triangle.V2().HomNormal() /= c_iPos.z;
  triangle.V2().HomSpecColor() /= c_iPos.z;
  triangle.V2().HomTexPos() /= c_iPos.z;

  const float Ns = 50.0f;

  for (int x = min_x; x <= max_x; x++) {
    for (int y = min_y; y <= max_y; y++) {
      glm::vec2 q{(float) x - triangle.V0().ScreenPosition().x,
                  (float) y - triangle.V0().ScreenPosition().y};

      float u =
          (float) (q.x*vs2.y - vs2.x*q.y)/(vs1.x*vs2.y - vs2.x*vs1.y);
      float v =
          (float) (vs1.x*q.y - q.x*vs1.y)/(vs1.x*vs2.y - vs2.x*vs1.y);
      if ((u >= 0) && (v >= 0) && (u + v < 1)) { // inside
        auto i_diff_color = InterpolateAttribute(
            triangle.V0().HomDiffColor(), triangle.V1().HomDiffColor(),
            triangle.V2().HomDiffColor(), u, v);

        auto i_spec_color = InterpolateAttribute(
            triangle.V0().HomSpecColor(), triangle.V1().HomSpecColor(),
            triangle.V2().HomSpecColor(), u, v);

        auto i_pos = InterpolateAttribute(
            triangle.V0().HomWorldPos(), triangle.V1().HomWorldPos(),
            triangle.V2().HomWorldPos(), u, v);

        auto i_normal = InterpolateAttribute(triangle.V0().HomNormal(),
                                             triangle.V1().HomNormal(),
                                             triangle.V2().HomNormal(), u, v);
        glm::vec3 normal{i_normal};
        normal = glm::normalize(normal);

        if (triangle.IsTextured()) {
          auto i_tex_pos = triangle.V0().HomTexPos() + u*(triangle.V1().HomTexPos() - triangle.V0().HomTexPos())
              + v*(triangle.V2().HomTexPos() - triangle.V0().HomTexPos());
          i_tex_pos /= i_tex_pos[2];
          if (m_bilinearFiltering) {
            float s = std::clamp(i_tex_pos[0], 0.f, 1.0f)*((float) triangle.Texture()->Width() - 1);
            float t = std::clamp(i_tex_pos[1], 0.f, 1.0f)*((float) triangle.Texture()->Height() - 1);
            int s_floored = std::floor(s);
            int t_floored = std::floor(t);
            auto s_flooredp1 = std::min(s_floored + 1, triangle.Texture()->Width() - 1);
            auto t_flooredp1 = std::min(t_floored + 1, triangle.Texture()->Height() - 1);
            auto c1 = triangle.GetTextureColorInt(s_floored, t_floored);
            auto c2 = triangle.GetTextureColorInt(s_floored, t_flooredp1);
            auto c3 = triangle.GetTextureColorInt(s_flooredp1, t_floored);
            auto c4 = triangle.GetTextureColorInt(s_flooredp1, t_flooredp1);
            float t0 = t - std::floor(t);
            float t1 = s - std::floor(s);
            auto color1 = c1 + t0*(c2 - c1);
            auto color2 = c3 + t0*(c4 - c3);
            auto final = color1 + t1*(color2 - color1);
            i_diff_color = glm::vec4{final, 1};
          } else {
            i_diff_color = glm::vec4{triangle.GetTextureColor(i_tex_pos[0], i_tex_pos[1]), 1};
          }
        }

        glm::vec3 diffuse_color{};
        glm::vec3 specular_color{};
        for (auto &light: m_scene_lights) {
          glm::vec3 l;
          switch (light->Type()) {
            case (LightType::POINT):l = glm::normalize(light->P() - glm::vec3{i_pos});
              break;
            case (LightType::AMBIENT):diffuse_color += light->Color()*glm::vec3{i_diff_color}*light->Intensity();
              continue;
          }
          auto r = glm::normalize(glm::reflect(glm::normalize(glm::vec3{i_pos} - m_camera_origin), normal));
          auto n_dot_l = glm::dot(normal, l);
          if (n_dot_l >= 0) {
            diffuse_color += light->Color()*light->Intensity()*glm::vec3{i_diff_color}*n_dot_l;
            specular_color +=
                light->Color()*light->Intensity()*glm::vec3{i_spec_color}*glm::pow(std::max(0.f, glm::dot(r, l)), Ns);
          }
        }

        auto final_color = diffuse_color + specular_color;
        if (m_zBuff->CompareAndSet(x, y, i_pos.z)) {
          m_color_mode->Process(final_color);
          m_image_buffer->SetPixelColor(x, y, final_color);
        }
      }
    }
  }
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::DrawOutline(Triangle &triangle) const {
  SDL_SetRenderDrawColor(m_window_renderer.get(),
                         Colors::WIREFRAME_COLOR()[0],
                         Colors::WIREFRAME_COLOR()[1],
                         Colors::WIREFRAME_COLOR()[2], 255);
  SDL_RenderDrawLine(m_window_renderer.get(),
                     (int) triangle.V0().ScreenPosition()[0],
                     (int) triangle.V0().ScreenPosition()[1],
                     (int) triangle.V1().ScreenPosition()[0],
                     (int) triangle.V1().ScreenPosition()[1]);
  SDL_RenderDrawLine(m_window_renderer.get(),
                     (int) triangle.V1().ScreenPosition()[0],
                     (int) triangle.V1().ScreenPosition()[1],
                     (int) triangle.V2().ScreenPosition()[0],
                     (int) triangle.V2().ScreenPosition()[1]);
  SDL_RenderDrawLine(m_window_renderer.get(),
                     (int) triangle.V0().ScreenPosition()[0],
                     (int) triangle.V0().ScreenPosition()[1],
                     (int) triangle.V2().ScreenPosition()[0],
                     (int) triangle.V2().ScreenPosition()[1]);
}

template<color_strategy T, is_resolution U>
bool Renderer<T, U>::initializeSDL() {
  std::cout << "\nInitializing SDL\n";
  if (m_window!=nullptr || m_window_renderer!=nullptr) {
    return false;
  }
  m_window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>>(
      SDL_CreateWindow("Computer Graphics", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, m_resolution->width(),
                       m_resolution->height(), 0),
      SDL_DestroyWindow);
  m_window_renderer =
      std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>(
          SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_SOFTWARE),
          SDL_DestroyRenderer);

  if (!m_window) {
    std::cerr << "Failed to create window\n";
    std::cerr << "Error - " << SDL_GetError() << "\n";
    return false;
  }

  if (!m_window_renderer) {
    std::cerr << "Failed to get window's surface\n";
    std::cerr << "Error - " << SDL_GetError() << "\n";
    return false;
  }
  m_isRunning = true;
  return true;
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::DrawPixel(int x, int y, glm::vec3 &color) {
  SDL_SetRenderDrawColor(m_window_renderer.get(),
                         static_cast<glm::uint8>(color[0]),
                         static_cast<glm::uint8>(color[1]),
                         static_cast<glm::uint8>(color[2]), 255);
  SDL_RenderDrawPoint(m_window_renderer.get(), static_cast<int>(x),
                      static_cast<int>(y));
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::Render() noexcept {
  for (auto x = 0; x < m_resolution->width(); ++x) {
    for (auto y = 0; y < m_resolution->height(); ++y) {
      DrawPixel(x, y, m_image_buffer->GetPixelColor(x, y));
    }
  }
  m_image_buffer->ClearBuffer();
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::SetLights(
    std::vector<std::unique_ptr<LightSource>> &lights) {
  m_scene_lights = std::move(lights);
}

template<color_strategy T, is_resolution U>
void Renderer<T, U>::SetDrawOutline(bool value) {
  m_draw_wireframe = value;
  m_objectRotation.y = 20.0f;
}

#endif // RASTERIZER_RENDERER_RENDERER_H_
