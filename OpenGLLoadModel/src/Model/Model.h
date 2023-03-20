
#ifndef GQY_MODEL_H
#define GQY_MODEL_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh/Mesh.h"
#include "Shader/Shader.h"
#include "Material/Material.h"

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
        for (std::shared_ptr<Mesh>& mesh : m_meshes)
        {
            mesh->draw(_shader);
        }
    }

private:
    void load_model(const std::string& _path);
    void process_node(aiNode* _node, const aiScene* _scene);
    std::shared_ptr<Mesh> process_mesh(aiMesh* _mesh, const aiScene* _scene);

private:
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::string m_directory;
    bool m_gamma_correction;
};
#endif // !GQY_MODEL_H
