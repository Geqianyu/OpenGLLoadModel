#version 460 core

struct Light {
    vec3 position;  
    vec3 intensity;
    float constant;
    float linear;
    float quadratic;
};

out vec4 fragment_color;

in Vs_out
{
    vec3 fragment_position;
    vec3 fragment_normal;
    vec2 fragment_tex_coords;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform Light light;

uniform vec3 camera_position;

void main()
{    
    float distance    = length(light.position - fs_in.fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient_color = texture(texture_diffuse1, fs_in.fragment_tex_coords).rgb;
    vec3 ambient = 0.05 * ambient_color * attenuation;

    vec3 light_direction = normalize(light.position - fs_in.fragment_position);
    vec3 normal = normalize(fs_in.fragment_normal);
    float diffuse_coeff = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = light.intensity * diffuse_coeff * ambient_color * attenuation;

    vec3 view_direction = normalize(camera_position - fs_in.fragment_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    vec3 halfway_direction = normalize(light_direction + view_direction);  
    float specular_coeff = pow(max(dot(normal, halfway_direction), 0.0), 32.0);
    vec3 specular = light.intensity * texture(texture_specular1, fs_in.fragment_tex_coords).rgb * specular_coeff * attenuation;

    fragment_color = vec4(ambient + diffuse + specular, 1.0);
}