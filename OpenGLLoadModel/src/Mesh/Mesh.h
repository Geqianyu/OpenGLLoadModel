
#ifndef GQY_MESH_H
#define GQY_MESH_H

#include <vector>
#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader/Shader.h"

constexpr int MAX_BONE_INFLUENCE = 4;

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 tex_coords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
    //bone indexes which will influence this vertex
    int bone_ids[MAX_BONE_INFLUENCE];
    //weights from each bone
    float weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const std::vector<Texture>& _textures)
    {
        this->m_vertices = _vertices;
        this->m_indices = _indices;
        this->m_textures = _textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setup_mesh();
    }
    ~Mesh() = default;

    void draw(const Shader& _shader);

private:
    void setup_mesh();

private:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture> m_textures;
    GLuint m_VAO, m_VBO, m_EBO;
};


#endif // !GQY_MESH_H
