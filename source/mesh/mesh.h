//
// Created by kevin on 27/10/2021.
//

#ifndef PATH_TRACER_SOURCE_GEOMETRY_GEOMETRY_H_
#define PATH_TRACER_SOURCE_GEOMETRY_GEOMETRY_H_

#include <initializer_list>
#include <fwd.hpp>
#include <mat4x4.hpp>
#include <vector>
#include <memory>
#include "../vertex/vertex.h"

class Mesh {
 public:
  Mesh() = delete;
  Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<Triangle> triangles);
  Mesh(std::initializer_list<Vertex> vertices,
       std::initializer_list<Triangle> triangles,
       glm::vec3 origin);
  Mesh(std::initializer_list<Vertex> vertices,
       std::initializer_list<Triangle> triangles,
       glm::vec3 origin,
       glm::vec3 scale);
  Mesh(std::vector<Triangle> &triangles,
       glm::vec3 origin,
       glm::vec3 scale,
       glm::vec3 rotation);
  Mesh(std::initializer_list<Vertex> vertices,
       std::initializer_list<Triangle> triangles,
       glm::vec3 origin,
       glm::vec3 scale,
       glm::vec3 rotation);
  void PrintModelMatrix() const noexcept;
  void Rotate(glm::vec3 &);
  [[nodiscard]] glm::mat4 &ModelMatrix();
  [[nodiscard]] std::vector<Vertex> &Vertices();
  [[nodiscard]] std::vector<Triangle> &Triangles();
 protected:
  glm::mat4 m_scale{1.0f};
  glm::mat4 m_rotation{1.0f};
  glm::mat4 m_translation{1.0f};
  glm::mat4 m_model{1.0f};
 private:
  std::vector<Vertex> m_vertices{};
  std::vector<Triangle> m_triangles{};
};

#endif //PATH_TRACER_SOURCE_GEOMETRY_GEOMETRY_H_
