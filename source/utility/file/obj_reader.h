//
// Created by kevin on 07/11/2021.
//

#ifndef RASTERIZER_UTILITY_FILE_OBJ_READER_H_
#define RASTERIZER_UTILITY_FILE_OBJ_READER_H_

#include <iostream>
#include <fstream>

namespace utility {

template<typename Out>
static void split(const std::string &s, char delim, Out result) {
  std::istringstream iss(s);
  std::string item;
  while (std::getline(iss, item, delim)) {
    *result++ = item;
  }
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

static bool ReadFile(std::vector<Triangle> &target, const std::string &file) {
  std::ifstream stream(file.c_str());

  if (!stream.is_open()) {
    std::cerr << "couldn't find file " << file << "\n";
    return false;
  }

  std::string line;
  std::vector<Vertex> vertices;
  std::vector<glm::vec3> normals;
  while (getline(stream, line)) {
    if (line[0]=='v' && line[1]==' ') {
      line = line.substr(2);
      auto elements = split(line, ' ');
      vertices.push_back(Vertex{glm::vec3{std::stof(elements[0]), std::stof(elements[1]), std::stof(elements[2])}});
    } else if (line[0]=='v' && line[1]=='n') {
      line = line.substr(3);
      auto elements = split(line, ' ');
      normals.emplace_back(std::stof(elements[0]), std::stof(elements[1]), std::stof(elements[2]));
    } else if (line[0]=='f') {
      line = line.substr(2);
      auto elements = split(line, ' ');
      std::vector<Vertex> triangleVerts;
      for (auto &element: elements) {
        auto params = split(element, '/');
        int vertex = std::stoi(params[0]);
        if (vertex < 0) {
          vertex = (int) vertices.size() + vertex;
        }
        int normal = std::stoi(params[2]);
        Vertex v0 = vertices[vertex - 1];
        v0.UpdateDiffuseColor({1.0f, 0, 0});
        v0.UpdateSpecularColor({1.0f, 1.0f, 1.0f});
        v0.UpdateNormal(normals[normal - 1]);
        triangleVerts.push_back(v0);
      }
      target.emplace_back(triangleVerts[0], triangleVerts[1], triangleVerts[2]);
    }
  }
  std::cout << "Read file " << file << " successfully.\n";
  stream.close();
  return true;
}
}

#endif //RASTERIZER_UTILITY_FILE_OBJ_READER_H_
