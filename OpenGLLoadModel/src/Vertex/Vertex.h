
#ifndef GQY_VERTEX_H
#define GQY_VERTEX_H

#include <glm/glm.hpp>

extern const int MAX_BONE_INFLUENCE;

struct Vertex
{
    Vertex() = default;
    ~Vertex() = default;

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

#endif // !GQY_VERTEX_H
