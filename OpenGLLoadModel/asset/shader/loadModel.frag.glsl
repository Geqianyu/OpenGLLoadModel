
#version 460 core

out vec4 fragment_color;

in vec2 fragment_tex_coords;

uniform sampler2D texture_diffuse1;

void main()
{    
    fragment_color = texture(texture_diffuse1, fragment_tex_coords);
}
