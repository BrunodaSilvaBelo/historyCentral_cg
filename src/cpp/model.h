#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include "texture.h"
#include "mesh.h"
#include <assimp/scene.h>

class Shader;

class Model {
public:
  Model(const std::string &file);
  void draw(Shader &shader);

private:
  std::string directory;
  std::vector<Mesh> meshes;
  std::vector<Texture> textures_loaded;
  void loadModel(const std::string &file);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       const std::string &typeName);
};

#endif /* MODEL_H */
