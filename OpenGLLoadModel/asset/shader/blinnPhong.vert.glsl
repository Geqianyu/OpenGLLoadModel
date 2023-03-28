
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coord;

out Vs_out
{
    vec3 position;
    vec3 normal;
    vec2 texture_coord;
    vec4 position_light_space;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space_matrix;

void main()
{
    vs_out.position = position;
    vs_out.normal = normal;
    vs_out.texture_coord = texture_coord;
    vs_out.position_light_space = light_space_matrix * vec4(position, 1.0);
    gl_Position = projection * view * model * vec4(position, 1.0);
}
