
#ifndef GQY_LIGHT_H
#define GQY_LIGHT_H

#include <glm/glm.hpp>

#include "Shader/Shader.h"

class Light
{
public:
    Light(const glm::vec3& _position, const glm::vec3& _intensity);
    ~Light();

    void draw(const Shader& _shader);

    inline glm::vec3 get_position() const
    {
        return m_position;
    }

    inline glm::vec3 get_intensity() const
    {
        return m_intensity;
    }

    inline float get_constant() const
    {
        return m_constant;
    }

    inline float get_linear() const
    {
        return m_linear;
    }

    inline float get_quadratic() const
    {
        return m_quadratic;
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_intensity;

    const float m_constant{ 1.0f };
    const float m_linear{ 0.09f };
    const float m_quadratic{ 0.032f };

    GLuint m_VAO, m_VBO;
};

#endif // !GQY_LIGHT_H
