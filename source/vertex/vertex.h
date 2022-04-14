//
// Created by kevin on 01/11/2021.
//

#ifndef PATH_TRACER_VERTEX_VERTEX_H_
#define PATH_TRACER_VERTEX_VERTEX_H_

#include <utility>
#include <iostream>

#include "glm.hpp"
#include "../textures/bitmap_texture.h"

class Vertex {
 public:
  Vertex() = delete;
  explicit Vertex(glm::vec3 pos)
      : m_position(pos),
        m_diffuse_color({}),
        m_specular_color({}),
        m_normal(glm::normalize(m_position)),
        m_hom_local_pos({m_position, 1}),
        m_hom_normal({m_normal, 1}),
        m_hom_diff_color({m_diffuse_color, 1}),
        m_hom_spec_color({m_specular_color, 1}) {}
  Vertex(glm::vec3 pos, glm::vec3 color)
      : m_position(pos),
        m_diffuse_color(color),
        m_specular_color(color),
        m_normal(glm::normalize(m_position)),
        m_hom_local_pos({m_position, 1}),
        m_hom_normal({m_normal, 0}),
        m_hom_diff_color({m_diffuse_color, 1}),
        m_hom_spec_color({m_specular_color, 1}) {}
  Vertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal)
      : m_position(pos),
        m_diffuse_color(color),
        m_specular_color(color),
        m_normal(glm::normalize(normal)),
        m_hom_local_pos({m_position, 1}),
        m_hom_normal({m_normal, 0}),
        m_hom_diff_color({m_diffuse_color, 1}),
        m_hom_spec_color({m_specular_color, 1}) {}
  [[maybe_unused]] glm::vec3 &Position() { return m_position; }
  [[maybe_unused]] glm::vec3 &DiffColor() { return m_diffuse_color; }
  [[maybe_unused]] glm::vec3 &SpecColor() { return m_specular_color; }
  glm::vec3 &ScreenPosition() { return m_screen_position; }
  glm::vec4 &ClipSpacePos() { return m_clip_space; }
  glm::vec4 &HomLocalPos() { return m_hom_local_pos; }
  glm::vec4 &HomWorldPos() { return m_hom_world_pos; }
  glm::vec4 &HomNormal() { return m_hom_normal; }
  glm::vec4 &HomDiffColor() { return m_hom_diff_color; }
  glm::vec2 &TexturePosition() { return m_texture_position; }
  glm::vec4 &HomSpecColor() { return m_hom_spec_color; }
  glm::vec3 &HomTexPos() { return m_hom_texture_position; }
  void UpdateDiffuseColor(glm::vec3 color) {
    m_diffuse_color = color;
    m_hom_diff_color = {m_diffuse_color, 1};
  }

  void UpdateNormal(glm::vec3 normal) {
    m_normal = glm::normalize(normal);
    m_hom_normal = {normal, 0};
  }

  void UpdateSpecularColor(glm::vec3 color) {
    m_specular_color = color;
    m_hom_spec_color = {m_specular_color, 1};
  }

  void UpdateWorldPosition(glm::vec3 pos) {
    m_world_position = pos;
    m_hom_world_pos = {m_world_position, 1};
  }

  void SetTexturePosition(glm::vec2 &pos) {
    m_texture_position = pos;
    m_hom_texture_position = {m_texture_position, 1};
  }

 private:
  void ResetTransientProperties() {
    m_hom_normal = {m_normal, 0};
    m_hom_diff_color = {m_diffuse_color, 1};
    m_hom_spec_color = {m_specular_color, 1};
    m_hom_texture_position = {m_texture_position, 1};
  }
  glm::vec3 m_position, m_diffuse_color, m_specular_color, m_normal, m_world_position{}, m_screen_position{},
      m_hom_texture_position{};
  glm::vec4 m_hom_local_pos, m_hom_normal, m_hom_diff_color, m_hom_spec_color, m_hom_world_pos{}, m_clip_space{};
  glm::vec2 m_texture_position;
  friend class Triangle;
};

class Triangle {
 public:
  Triangle() = delete;
  Triangle(Vertex &a, Vertex &b, Vertex &c)
      : v0(a),
        v1(b),
        v2(c), m_texture(nullptr) {}
  Triangle(Vertex &a, Vertex &b, Vertex &c, std::shared_ptr<BitmapTexture> tx)
      : v0(a),
        v1(b),
        v2(c), m_texture(std::move(tx)) {}
  Vertex &V0() { return v0; }
  Vertex &V1() { return v1; }
  Vertex &V2() { return v2; }
  void ResetTransientProperties() {
    v0.ResetTransientProperties();
    v1.ResetTransientProperties();
    v2.ResetTransientProperties();
  }

  glm::vec3 &GetTextureColor(float x, float y) {
    return m_texture->GetColor(x, y);
  }

  glm::vec3 &GetTextureColorInt(int x, int y) {
    return m_texture->GetColorInt(x, y);
  }

  bool IsTextured() {
    return m_texture!=nullptr;
  }

  std::shared_ptr<BitmapTexture>& Texture() {
    return m_texture;
  }

 private:
  Vertex v0, v1, v2;
  std::shared_ptr<BitmapTexture> m_texture;
};

#endif //PATH_TRACER_VERTEX_VERTEX_H_
