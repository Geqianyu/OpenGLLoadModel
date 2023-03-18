#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

out Vs_out
{
    vec3 fragment_position;
    vec3 fragment_normal;
    vec2 fragment_tex_coords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.fragment_position = position;
    vs_out.fragment_normal = normal;
    vs_out.fragment_tex_coords = tex_coords;
    gl_Position = projection * view * model * vec4(position, 1.0);
}