
#include "Shader.h"

Shader::Shader(const std::string& _vertex_shader_path, const std::string& _fragment_shader_path)
{
    std::string vertex_shader_code;
    std::string fragment_shader_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;
    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertex_shader_file.open(_vertex_shader_path);
        fragment_shader_file.open(_fragment_shader_path);

        std::stringstream vertex_shader_stream;
        vertex_shader_stream << vertex_shader_file.rdbuf();
        std::stringstream fragment_shader_stream;
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_shader_code = vertex_shader_stream.str();
        fragment_shader_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        // ...
    }

    const char* vertex_shader_code_char = vertex_shader_code.c_str();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code_char, nullptr);
    glCompileShader(vertex_shader);
    check_compile_error(vertex_shader, Check_type::Vertex);

    const char* fragment_shader_code_char = fragment_shader_code.c_str();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code_char, nullptr);
    glCompileShader(fragment_shader);
    check_compile_error(fragment_shader, Check_type::Fragment);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex_shader);
    glAttachShader(m_id, fragment_shader);
    glLinkProgram(m_id);
    check_compile_error(m_id, Check_type::Program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}
