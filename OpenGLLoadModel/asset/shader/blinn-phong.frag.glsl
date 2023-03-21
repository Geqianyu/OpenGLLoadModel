
#version 460 core

struct Light
{
    vec3 position;
    vec3 intensity;
    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    vec3 Ka;
    sampler2D Ka_texture;
    int Ka_map;
    vec3 Kd;
    sampler2D Kd_texture;
    int Kd_map;
    vec3 Ks;
    sampler2D Ks_texture;
    int Ks_map;
    float Ns;
    sampler2D Bump_texture;
    int Bu_map;
};

out vec4 fragment_color;

in Vs_out
{
    vec3 fragment_position;
    vec3 fragment_normal;
    vec2 fragment_tex_coords;
    vec4 fragment_position_light_space;
} fs_in;

uniform Material material;
uniform Light light;
uniform vec3 camera_position;
uniform sampler2D depth_map;

float shadow_calculation(vec3 light_direction)
{
    vec3 projection_coords = fs_in.fragment_position_light_space.xyz / fs_in.fragment_position_light_space.w;
    projection_coords = projection_coords * 0.5 + 0.5;
    float bias = max(0.035 * (1.0 - dot(fs_in.fragment_normal, light_direction)), 0.0);
    float map_depth = texture(depth_map, projection_coords.xy).r;
    float shadow = map_depth < projection_coords.z - bias ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    float gamma = 2.2;
    float distance    = length(light.position - fs_in.fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient_color = material.Ka_map == 1 ? pow(texture(material.Ka_texture, fs_in.fragment_tex_coords).rgb, vec3(gamma)) : material.Ka;
    vec3 ambient = 0.05 * ambient_color * attenuation;

    vec3 light_direction = normalize(light.position - fs_in.fragment_position);
    vec3 normal = normalize(fs_in.fragment_normal);
    float diffuse_coeff = max(dot(light_direction, normal), 0.0);
    vec3 diffuse_color = material.Kd_map == 1 ? pow(texture(material.Kd_texture, fs_in.fragment_tex_coords).rgb, vec3(gamma)) : material.Kd;
    vec3 diffuse = light.intensity * diffuse_coeff * diffuse_color * attenuation;

    vec3 view_direction = normalize(camera_position - fs_in.fragment_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    vec3 halfway_direction = normalize(light_direction + view_direction);  
    float specular_coeff = pow(max(dot(normal, halfway_direction), 0.0), material.Ns);
    vec3 specular_color = material.Ks_map == 1 ? pow(texture(material.Ks_texture, fs_in.fragment_tex_coords).rgb, vec3(gamma)) : material.Ks;
    vec3 specular = light.intensity * specular_color * specular_coeff * attenuation;

    float shadow = shadow_calculation(light_direction);

    vec3 color = pow(ambient + (1.0 - shadow) * (diffuse + specular), vec3(1.0 / gamma));
    fragment_color = vec4(color, 1.0);
}
