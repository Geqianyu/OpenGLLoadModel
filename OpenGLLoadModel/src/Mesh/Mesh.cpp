
#include "Mesh.h"

void Mesh::draw(const Shader& _shader)
{
    GLuint diffuse_index = 1;
    GLuint specular_index = 1;
    GLuint normal_index = 1;
    GLuint height_index = 1;
    GLuint texture_index = 0;
    for (Texture& texture : m_textures)
    {
        glActiveTexture(GL_TEXTURE0 + texture_index);
    
        const std::string& name = texture.type;
        if (name == "texture_diffuse")
        {
            glUniform1i(glGetUniformLocation(_shader.get_id(), (name + std::to_string(diffuse_index++)).c_str()), texture_index);
        }
        else if (name == "texture_specular")
        {
            glUniform1i(glGetUniformLocation(_shader.get_id(), (name + std::to_string(specular_index++)).c_str()), texture_index);
        }
        else if (name == "texture_normal")
        {
            glUniform1i(glGetUniformLocation(_shader.get_id(), (name + std::to_string(normal_index++)).c_str()), texture_index);
        }
        else if (name == "texture_height")
        {
            glUniform1i(glGetUniformLocation(_shader.get_id(), (name + std::to_string(height_index++)).c_str()), texture_index);
        }
    
        glBindTexture(GL_TEXTURE_2D, texture.id);
        ++texture_index;
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setup_mesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    glEnableVertexAttribArray(4);
    
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bone_ids));
    glEnableVertexAttribArray(5);
    
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
    glEnableVertexAttribArray(6);

    glBindVertexArray(0);
}