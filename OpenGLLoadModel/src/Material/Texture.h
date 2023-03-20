
#ifndef GQY_TEXTURE_H
#define GQY_TEXTURE_H

#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "Shader/Shader.h"

class Texture
{
public:
    Texture() = default;
    Texture(const std::string& _type) : m_type(_type) { }
    ~Texture() = default;

    inline void set_type(const std::string& _type)
    {
        m_type = _type;
    }

    virtual void bind_texture(const Shader& _shader, int _texture_index) = 0;

protected:
    std::string m_type;
};

class Solid_texture : public Texture
{
public:
    Solid_texture() = default;
    Solid_texture(const glm::vec3& _value, const std::string& _type);
    ~Solid_texture() = default;

    void set_texture(const glm::vec3& _value);

    virtual void bind_texture(const Shader& _shader, int _texture_index) override;

private:
    glm::vec3 m_value;
};

class Image_texture : public Texture
{
public:
    Image_texture(const std::string& _path, const std::string& _type);
    ~Image_texture();

    void set_texture(const std::string& _path);

    virtual void bind_texture(const Shader& _shader, int _texture_index) override;

private:
    GLuint m_id;
};

#endif // !GQY_TEXTURE_H
