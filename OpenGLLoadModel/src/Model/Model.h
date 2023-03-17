﻿
#ifndef GQY_MODEL_H
#define GQY_MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh/Mesh.h"
#include "Shader/Shader.h"

GLuint texture_from_file(const std::string& _path, const std::string& _directory, bool gamma = false);

class Model
{
public:
    Model(std::string const& _path, bool _gamma = false) : m_gamma_correction(_gamma)
    {
        load_model(_path);
    }
    ~Model() = default;

    void draw(const Shader& _shader)
    {
        for (Mesh mesh : m_meshes)
        {
            mesh.draw(_shader);
        }
    }

private:
    void load_model(const std::string& _path);
    void process_node(aiNode* _node, const aiScene* _scene);
    Mesh process_mesh(aiMesh* _mesh, const aiScene* _scene);
    std::vector<Texture> load_material_textures(aiMaterial* _mat, aiTextureType _type, const std::string& _type_name);

private:
    // model data 
    std::vector<Texture> m_textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    m_meshes;
    std::string m_directory;
    bool m_gamma_correction;
};
#endif // !GQY_MODEL_H