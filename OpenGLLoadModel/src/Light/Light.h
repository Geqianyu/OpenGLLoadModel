
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

    inline void set_position(const glm::vec3& _position)
    {
        m_position = _position;
    }

    inline glm::vec3 get_intensity() const
    {
        return m_intensity;
    }

    inline void set_intensity(const glm::vec3& _intensity)
    {
        m_intensity = _intensity;
    }

    inline float get_constant() const
    {
        return m_constant;
    }

    inline void set_constant(const float& _constant)
    {
        m_constant = _constant;
    }

    inline float get_linear() const
    {
        return m_linear;
    }

    inline void set_linear(const float& _linear)
    {
        m_linear = _linear;
    }

    inline float get_quadratic() const
    {
        return m_quadratic;
    }

    inline void set_quadratic(const float& _quadratic)
    {
        m_quadratic = _quadratic;
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_intensity;

    float m_constant{ 1.0f };
    float m_linear{ 0.09f };
    float m_quadratic{ 0.032f };

    GLuint m_VAO, m_VBO;
};

#endif // !GQY_LIGHT_H
