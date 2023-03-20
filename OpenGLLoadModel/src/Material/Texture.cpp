
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.h"

Solid_texture::Solid_texture(const glm::vec3& _value, const std::string& _type) : Texture(_type)
{
    m_value = _value;
}

void Solid_texture::set_texture(const glm::vec3& _value)
{
    m_value = _value;
}

void Solid_texture::bind_texture(const Shader& _shader, int _texture_index)
{
    std::string temp = m_type.substr(0, 11) + "_map";
    _shader.set_int(m_type.substr(0, 11) + "_map", 0);
    _shader.set_vec3(m_type, m_value);
}

Image_texture::Image_texture(const std::string& _path, const std::string& _type) : Texture(_type)
{
    set_texture(_path);
}

Image_texture::~Image_texture()
{
    glDeleteTextures(1, &m_id);
}

void Image_texture::set_texture(const std::string& _path)
{
    glGenTextures(1, &m_id);
    int width, height, channels;
    stbi_uc* image = stbi_load(_path.c_str(), &width, &height, &channels, 0);
    if (image != nullptr)
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        switch (channels)
        {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, image);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            break;
        default:
            break;
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(image);
    }
    else
    {
        GQY_OPENGL_LOAD_MODEL_ERROR("Texture failed to load at path: " + _path);
        stbi_image_free(image);
    }
}

void Image_texture::bind_texture(const Shader& _shader, int _texture_index)
{
    std::string temp = m_type.substr(0, 11) + "_map";
    _shader.set_int(m_type.substr(0, 11) + "_map", 1);
    glActiveTexture(GL_TEXTURE0 + _texture_index);
    _shader.set_int(m_type, _texture_index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}