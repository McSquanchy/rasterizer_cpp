//
// Created by kevin on 07/11/2021.
//

#ifndef RASTERIZER_UTILITY_FILE_OBJ_READER_H_
#define RASTERIZER_UTILITY_FILE_OBJ_READER_H_

#include <iostream>
#include <fstream>
#include "../../textures/bitmap_texture.h"

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

static bool ReadFile(std::vector<Triangle> &target, const std::string &file, const std::string &texture) {
  std::ifstream stream(file.c_str());

  if (!stream.is_open()) {
    std::cerr << "couldn't find file " << file << "\n";
    return false;
  }
  std::shared_ptr<BitmapTexture> tx = nullptr;
  if (!texture.empty()) {
    tx = std::make_shared<BitmapTexture>(TextureType::DIFFUSE);
    tx->Read(texture.c_str());
  }

  std::string line;
  std::vector<Vertex> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> textureCoordinates;
  while (getline(stream, line)) {
    if (line[0]=='v' && line[1]==' ') {
      line = line.substr(2);
      auto elements = split(line, ' ');
      vertices.push_back(Vertex{glm::vec3{std::stof(elements[0]), std::stof(elements[1]), std::stof(elements[2])}});
    } else if (line[0]=='v' && line[1]=='n') {
      line = line.substr(3);
      auto elements = split(line, ' ');
      normals.emplace_back(std::stof(elements[0]), std::stof(elements[1]), std::stof(elements[2]));
    } else if (line[0]=='v' && line[1]=='t') {
      line = line.substr(3);
      auto elements = split(line, ' ');
      textureCoordinates.emplace_back(std::stof(elements[0]), std::stof(elements[1]));
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
        int textureCoordinate = std::stoi(params[1]);
        Vertex v0 = vertices[vertex - 1];
        v0.SetTexturePosition(textureCoordinates[textureCoordinate - 1]);
        v0.UpdateDiffuseColor({1.0f, 0, 0});
        v0.UpdateSpecularColor({1.0f, 1.0f, 1.0f});
        v0.UpdateNormal(normals[normal - 1]);
        triangleVerts.push_back(v0);
      }
      if (tx!=nullptr)
        target.emplace_back(triangleVerts[0], triangleVerts[1], triangleVerts[2], tx);
      else
        target.emplace_back(triangleVerts[0], triangleVerts[1], triangleVerts[2]);
    }
  }

  std::cout << "Read file " << file << " successfully.\n";
  stream.close();
  return true;
}

}

#endif //RASTERIZER_UTILITY_FILE_OBJ_READER_H_
