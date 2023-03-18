#version 460 core

out vec4 fragment_color;

in Vs_out
{
    vec3 fragment_position;
    vec3 fragment_normal;
    vec2 fragment_tex_coords;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 light_position;
uniform vec3 camera_position;

void main()
{    
    vec3 ambient_color = texture(texture_diffuse1, fs_in.fragment_tex_coords).rgb;
    vec3 ambient = 0.05 * ambient_color;

    vec3 light_direction = normalize(light_position - fs_in.fragment_position);
    vec3 normal = normalize(fs_in.fragment_normal);
    float diffuse_coeff = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = 2* diffuse_coeff * ambient_color;

    vec3 view_direction = normalize(camera_position - fs_in.fragment_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    vec3 halfway_direction = normalize(light_direction + view_direction);  
    float specular_coeff = pow(max(dot(normal, halfway_direction), 0.0), 32.0);
    vec3 specular = 2* texture(texture_specular1, fs_in.fragment_tex_coords).rgb * specular_coeff;

    fragment_color = vec4(ambient + diffuse + specular, 1.0);
}