//
// Created by kevin on 08/11/2021.
//

#ifndef RASTERIZER_SCENE_SCENE_GRAPH_H_
#define RASTERIZER_SCENE_SCENE_GRAPH_H_

using MeshPtr = std::shared_ptr<Mesh>;

class SceneGraph {
 public:
  SceneGraph() = default;

 private:
  glm::mat4 ModelMatrix;
  std::vector<MeshPtr>
};

#endif //RASTERIZER_SCENE_SCENE_GRAPH_H_
