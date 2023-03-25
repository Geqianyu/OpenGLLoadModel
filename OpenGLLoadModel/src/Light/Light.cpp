
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Light.h"

Light::Light(const glm::vec3& _position, const glm::vec3& _intensity, const float& _radius) : m_position(_position), m_intensity(_intensity)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    std::vector<float> positions;
    std::vector<unsigned int> indices;
    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = std::asinf(1.0f) * 2.0f;
    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
    {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            float x_segment = static_cast<float>(x) / static_cast<float>(X_SEGMENTS);
            float y_segment = static_cast<float>(y) / static_cast<float>(Y_SEGMENTS);
            float x_position = _radius * std::cos(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
            float y_position = _radius * std::cos(y_segment * PI);
            float z_position = _radius * std::sin(x_segment * 2.0f * PI) * std::sin(y_segment * PI);

            positions.push_back(x_position);
            positions.push_back(y_position);
            positions.push_back(z_position);
        }
    }

    bool odd_row = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!odd_row)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        odd_row = !odd_row;
    }
    m_index_count = static_cast<unsigned int>(indices.size());

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

Light::~Light()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Light::draw(const Shader& _shader)
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLE_STRIP, m_index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}