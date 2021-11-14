//
// Created by kevin on 01/11/2021.
//

#ifndef PATH_TRACER_VERTEX_VERTEX_H_
#define PATH_TRACER_VERTEX_VERTEX_H_

#include "glm.hpp"

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
  glm::vec3 &Position() { return m_position; }
  glm::vec3 &DiffColor() { return m_diffuse_color; }
  glm::vec3 &SpecColor() { return m_specular_color; }
  glm::vec3 &ScreenPosition() { return m_screen_position; }
  glm::vec4 &ClipSpacePos() { return m_clip_space; }
  glm::vec4 &HomLocalPos() { return m_hom_local_pos; }
  glm::vec4 &HomWorldPos() { return m_hom_world_pos; }
  glm::vec4 &HomNormal() { return m_hom_normal; }
  glm::vec4 &HomDiffColor() { return m_hom_diff_color; }
  glm::vec4 &HomSpecColor() { return m_hom_spec_color; }
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

 private:
  void ResetTransientProperties() {
//    m_hom_world_pos = {m_position, 1};
    m_hom_normal = {m_normal, 0};
    m_hom_diff_color = {m_diffuse_color, 1};
    m_hom_spec_color = {m_specular_color, 1};
  }
  glm::vec3 m_position, m_diffuse_color, m_specular_color, m_normal, m_world_position{}, m_screen_position{};
  glm::vec4 m_hom_local_pos, m_hom_normal, m_hom_diff_color, m_hom_spec_color, m_hom_world_pos{}, m_clip_space{};
  friend class Triangle;
};

class Triangle {
 public:
  Triangle() = delete;
  Triangle(Vertex &a, Vertex &b, Vertex &c)
      : v0(a),
        v1(b),
        v2(c) {}
  Vertex &V0() { return v0; }
  Vertex &V1() { return v1; }
  Vertex &V2() { return v2; }
  void ResetTransientProperties() {
    v0.ResetTransientProperties();
    v1.ResetTransientProperties();
    v2.ResetTransientProperties();
  }

 private:
  Vertex v0, v1, v2;
};

#endif //PATH_TRACER_VERTEX_VERTEX_H_
