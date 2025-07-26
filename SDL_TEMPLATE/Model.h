#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <map>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

class Model {
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    glm::mat4 model;
    float scale = 1.0f;
    glm::vec3 translation = glm::vec3(0.0f);
    float radiansRotate = 0.0f;
    glm::vec3 rotateAxis = glm::vec3(0.0f, 1.0f, 0.0f);

    Model(bool gamma = false);
    void init(std::string const& path);
    void Draw(Shader& shader);
    void updateModelMatrix();
    glm::mat3 getNormalMatrix();

private:
    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
};