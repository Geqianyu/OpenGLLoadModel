
#include "Material.h"

void Material::bind_material(const Shader& _shader)
{
    _shader.set_float("material.Ns", m_Ns);
    m_Ka->bind_texture(_shader, 0);
    m_Kd->bind_texture(_shader, 1);
    m_Ks->bind_texture(_shader, 2);
}