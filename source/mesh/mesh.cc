//
// Created by kevin on 27/10/2021.
//

#include <ext/matrix_transform.hpp>
#include <iostream>
#include <iomanip>
#include "mesh.h"

Mesh::Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<Triangle> triangles) {
  m_vertices = vertices;
  m_triangles = triangles;
//  std::cout << "base constructor called\n";
}

Mesh::Mesh(std::initializer_list<Vertex> vertices,
           std::initializer_list<Triangle> triangles,
           glm::vec3 origin) : Mesh(vertices, triangles) {
//  std::cout << "1 param constructor called\n";
  m_translation = glm::translate(m_translation, origin);
  m_model = m_scale * m_translation * m_rotation;
}

Mesh::Mesh(std::initializer_list<Vertex> vertices,
           std::initializer_list<Triangle> triangles,
           glm::vec3 origin,
           glm::vec3 scale) : Mesh(vertices, triangles) {
//  std::cout << "2 param constructor called\n";
  m_translation = glm::translate(m_translation, origin);
  m_scale = glm::scale(m_scale, scale);
  m_model = m_scale * m_translation;
}

Mesh::Mesh(std::initializer_list<Vertex> vertices,
           std::initializer_list<Triangle> triangles,
           glm::vec3 origin,
           glm::vec3 scale,
           glm::vec3 rotation) : Mesh(vertices, triangles) {
//  std::cout << "3 param constructor called\n";
  m_translation = glm::translate(m_translation, origin);
  m_scale = glm::scale(m_scale, scale);
  m_rotation = glm::rotate(m_rotation, rotation.x, {1, 0, 0}) * glm::rotate(m_rotation, rotation.y, {0, 1, 0})
      * glm::rotate(m_rotation, rotation.z, {0, 0, 1});
  m_model = m_scale * m_translation * m_rotation;
}

Mesh::Mesh(std::vector<Triangle> &triangles, glm::vec3 origin, glm::vec3 scale, glm::vec3 rotation)
    : m_triangles(triangles) {
  m_translation = glm::translate(m_translation, origin);
  m_scale = glm::scale(m_scale, scale);
  m_rotation = glm::mat4{1.0f};
  m_rotation = glm::rotate(m_rotation, rotation.x, {1, 0, 0}) * glm::rotate(m_rotation, rotation.y, {0, 1, 0})
      * glm::rotate(m_rotation, rotation.z, {0, 0, 1});
  m_model = m_scale * m_translation * m_rotation;
}

std::vector<Vertex> &Mesh::Vertices() {
  return {m_vertices};
}

std::vector<Triangle> &Mesh::Triangles() {
  return {m_triangles};
}

glm::mat4 &Mesh::ModelMatrix() {
  return m_model;
}
void Mesh::Rotate(glm::vec3 &rotation) {
  m_rotation =
      glm::rotate(glm::mat4{1.0f}, rotation.x, {1, 0, 0}) * glm::rotate(glm::mat4{1.0f}, rotation.y, {0, 1, 0})
          * glm::rotate(glm::mat4{1.0f}, rotation.z, {0, 0, 1}) * m_rotation;
  m_model = m_scale * m_translation * m_rotation;
}

void Mesh::PrintModelMatrix() const noexcept {
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "[" << static_cast<float>(m_model[0][0]) << ","
            << m_model[0][1]
            << "," << m_model[0][2] << "," << m_model[0][3] << "]\n";
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "[" << static_cast<float>(m_model[1][0]) << ","
            << m_model[1][1]
            << "," << m_model[1][2] << "," << m_model[1][3] << "]\n";
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "[" << static_cast<float>(m_model[2][0]) << ","
            << m_model[2][1]
            << "," << m_model[2][2] << "," << m_model[2][3] << "]\n";
  std::cout << std::setw(2) << std::fixed << std::setprecision(2) << "[" << static_cast<float>(m_model[3][0]) << ","
            << m_model[3][1]
            << "," << m_model[3][2] << "," << m_model[3][3] << "]\n\n";
}