
#ifndef GQY_MESH_H
#define GQY_MESH_H

#include <vector>
#include <string>
#include <memory>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader/Shader.h"
#include "Material/Material.h"
#include "Vertex/Vertex.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const std::shared_ptr<Material> _material) : m_vertices(_vertices), m_indices(_indices), m_material(_material)
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
    std::shared_ptr<Material> m_material;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_VAO, m_VBO, m_EBO;
};

#endif // !GQY_MESH_H
