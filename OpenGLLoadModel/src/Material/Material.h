
#ifndef GQY_MATERIAL_H
#define GQY_MATERIAL_H

#include <memory>

#include "Texture.h"
#include "Shader/Shader.h"

class Material
{
public:
    Material() = default;
    ~Material() = default;

    inline void set_Ns(const float& _Ns)
    {
        m_Ns = _Ns;
    }

    inline void set_Ni(const float& _Ni)
    {
        m_Ni = _Ni;
    }

    inline void set_Ka(const glm::vec3& _Ka)
    {
        m_Ka = std::make_shared<Solid_texture>(_Ka, "material.Ka");
    }

    inline void set_Ka(const std::string& _path)
    {
        m_Ka = std::make_shared<Image_texture>(_path, "material.Ka_texture");
    }

    inline void set_Kd(const glm::vec3& _Kd)
    {
        m_Kd = std::make_shared<Solid_texture>(_Kd, "material.Kd");
    }

    inline void set_Kd(const std::string& _path)
    {
        m_Kd = std::make_shared<Image_texture>(_path, "material.Kd_texture");
    }

    inline void set_Ks(const glm::vec3& _Ks)
    {
        m_Ks = std::make_shared<Solid_texture>(_Ks, "material.Ks");
    }

    inline void set_Ks(const std::string& _path)
    {
        m_Ks = std::make_shared<Image_texture>(_path, "material.Ks_texture");
    }

    inline void set_Bump(const std::string& _path)
    {
        m_Bump = std::make_shared<Image_texture>(_path, "material.Bump_texture");
    }

    void bind_material(const Shader& _shader);

private:
    float m_Ns{ 0.0f };
    std::shared_ptr<Texture> m_Ka{ nullptr };
    std::shared_ptr<Texture> m_Kd{ nullptr };
    std::shared_ptr<Texture> m_Ks{ nullptr };
    std::shared_ptr<Texture> m_Bump{ nullptr };
    float m_Ni{ 1.0f };
};

#endif // !GQY_MATERIAL_H
