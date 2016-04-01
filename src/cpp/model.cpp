#include <GL/glew.h>
#include "model.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stdexcept>
#include "stb_image.h"

using namespace std;

GLuint TextureFromFile(const char *file, string directory);

GLuint TextureFromFile(const char *file, string directory) {
  string fileName = string(file);
  fileName = directory + '/' + fileName;

  GLuint textureId;
  glGenTextures(1, &textureId);
  int width, height, numComponent;
  unsigned char *image = stbi_load(fileName.c_str(), &width, &height,
                                   &numComponent, 4);
  if (!image)
    fprintf(stderr, "Texture loading failed for texture: %s\n", fileName.c_str());

  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image);

  return textureId;
}

Model::Model(const string &file) {
  loadModel(file);
}

void Model::draw(Shader &shader) {
  for (auto &it : meshes)
    it.draw(shader);
}

void Model::loadModel(const string &file) {
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(file, aiProcess_Triangulate
                                         | aiProcess_FlipUVs);
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    string error = "ERROR::ASSIMP::";
    error += import.GetErrorString();
    throw new runtime_error(error);
  }

  directory = file.substr(0, file.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (GLuint i = 0; i < node->mNumMeshes; ++i) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (GLuint i = 0; i < node->mNumChildren; ++i)
    processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<GLuint> indices;
  vector<Texture> textures;

  for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
    vertices.push_back(Vertex({mesh->mVertices[i].x, mesh->mVertices[i].y,
            mesh->mVertices[i].z},
        (mesh->mTextureCoords[0])
        ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
        : glm::vec2(0.f, 0.f),
        {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}));
  }

  for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (GLuint j = 0; j < face.mNumIndices; ++j)
      indices.push_back(face.mIndices[j]);
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps =
      loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    vector<Texture> specularMaps =
      loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *material,
                                            aiTextureType type,
                                            const string &typeName) {
  vector<Texture> textures;
  for (GLuint i = 0; i < material->GetTextureCount(type); ++i) {
    aiString str;
    material->GetTexture(type, i, &str);

    GLboolean skip = false;
    for (GLuint j = 0; j < textures_loaded.size(); ++j)
      if (textures_loaded[j].file == str) {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }

    if (!skip) {
      Texture texture(TextureFromFile(str.C_Str(), directory), typeName, str);
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }

  return textures;
}
