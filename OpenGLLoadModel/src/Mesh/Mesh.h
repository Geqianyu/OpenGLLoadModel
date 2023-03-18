
#ifndef GQY_MESH_H
#define GQY_MESH_H

#include <vector>
#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader/Shader.h"

constexpr int MAX_BONE_INFLUENCE = 4;

struct Vertex
{
    Vertex() = default;
    ~Vertex() = default;

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    int bone_ids[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    Texture(GLuint _id, const std::string& _type, const std::string& _path) : id(_id), type(_type), path(_path) {}
    ~Texture() = default;

    GLuint id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const std::vector<Texture>& _textures) : m_vertices(_vertices), m_indices(_indices), m_textures(_textures)
    {
        setup_mesh();
    }
    ~Mesh()
    {
        glDeleteBuffers(1, &m_EBO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

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
