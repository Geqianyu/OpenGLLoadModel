
#ifndef GQY_SHADER_H
#define GQY_SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>

#include "Log/Log.h"

enum class Check_type
{
    Vertex = 0,
    Fragment,
    Program
};

class Shader
{
public:
    Shader(const std::string& _vertex_shader_path, const std::string& _fragment_shader_path);
    ~Shader();

    inline GLuint get_id() const
    {
        return m_id;
    }

    inline void use() const
    {
        glUseProgram(m_id);
    }

    inline void set_bool(const std::string& _name, bool _value) const
    {
        glUniform1i(glGetUniformLocation(m_id, _name.c_str()), static_cast<int>(_value));
    }

    inline void set_int(const std::string& _name, int _value) const
    {
        glUniform1i(glGetUniformLocation(m_id, _name.c_str()), _value);
    }
    inline void set_float(const std::string& _name, float _value) const
    {
        glUniform1f(glGetUniformLocation(m_id, _name.c_str()), _value);
    }

    inline void set_vec2(const std::string& _name, const glm::vec2& _value) const
    {
        glUniform2fv(glGetUniformLocation(m_id, _name.c_str()), 1, &_value[0]);
    }

    inline void set_vec2(const std::string& _name, float _x, float _y) const
    {
        glUniform2f(glGetUniformLocation(m_id, _name.c_str()), _x, _y);
    }

    inline void set_vec3(const std::string& _name, const glm::vec3& _value) const
    {
        glUniform3fv(glGetUniformLocation(m_id, _name.c_str()), 1, &_value[0]);
    }

    inline void set_vec3(const std::string& _name, float _x, float _y, float _z) const
    {
        glUniform3f(glGetUniformLocation(m_id, _name.c_str()), _x, _y, _z);
    }

    inline void set_vec4(const std::string& _name, const glm::vec4& _value) const
    {
        glUniform4fv(glGetUniformLocation(m_id, _name.c_str()), 1, &_value[0]);
    }

    inline void set_vec4(const std::string& _name, float _x, float _y, float _z, float _w) const
    {
        glUniform4f(glGetUniformLocation(m_id, _name.c_str()), _x, _y, _z, _w);
    }

    inline void set_matrix2(const std::string& _name, const glm::mat2& _matrix) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_id, _name.c_str()), 1, GL_FALSE, &_matrix[0][0]);
    }

    inline void set_matrix3(const std::string& _name, const glm::mat3& _matrix) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_id, _name.c_str()), 1, GL_FALSE, &_matrix[0][0]);
    }

    inline void set_matrix4(const std::string& _name, const glm::mat4& _matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_id, _name.c_str()), 1, GL_FALSE, &_matrix[0][0]);
    }


private:
    inline void check_compile_error(GLuint _shader, const Check_type& _type)
    {
        int success = 0;
        char info_log[1024];
        if (_type != Check_type::Program)
        {
            glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(_shader, 1024, nullptr, info_log);
                GQY_OPENGL_LOAD_MODEL_ERROR("error: shader compilation error. " + std::string(info_log));
            }
        }
        else
        {
            glGetProgramiv(_shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(_shader, 1024, nullptr, info_log);
                GQY_OPENGL_LOAD_MODEL_ERROR("error: program linking error. " + std::string(info_log));
            }
        }
    }

private:
    GLuint m_id;
};
#endif // !GQY_SHADER_H
