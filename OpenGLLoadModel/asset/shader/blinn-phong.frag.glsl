
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

#define SAMPLE_COUNT 60
#define RIGN_COUNT 10
#define PI 3.141592653589793
#define PI2 6.283185307179586

vec2 poisson_disk[SAMPLE_COUNT];

float random_0to1(vec2 uv)
{ 
    const float a = 12.9898, b = 78.233, c = 43758.5453;
    float dt = dot(uv.xy, vec2(a,b));
    float sn = mod(dt, PI);
    return fract(sin(sn) * c);
}

float bias_calculation(const vec3 light_direction)
{
    return max(0.04 * (1.0 - dot(fs_in.fragment_normal, light_direction)), 0.005);
}

void poisson_disk_samples(const vec2 random_seed)
{
    float angle_step = PI2 * float(RIGN_COUNT) / float(SAMPLE_COUNT);
    float inv_sample_count = 1.0 / float( SAMPLE_COUNT );
    float angle = random_0to1(random_seed) * PI2;
    float radius = inv_sample_count;
    float radius_step = radius;
    for(int i = 0; i < SAMPLE_COUNT; ++i)
    {
        poisson_disk[i] = vec2(cos(angle), sin(angle)) * pow(radius, 0.75);
        radius += radius_step;
        angle += angle_step;
    }
}

float calculate_average_blocker_depth(const vec3 projection_coords, const float bias)
{
    poisson_disk_samples(projection_coords.xy);
    float total_depth=0.0;
    int blocker_count =0;
    vec2 depth_map_size = 1.0 / textureSize(depth_map, 0) * 10.0;
    for (int i=0; i < SAMPLE_COUNT; ++i)
    {
        vec2 sample_texture = projection_coords.xy + poisson_disk[i] * depth_map_size;
        float map_depth = texture(depth_map, sample_texture).r;
        if(projection_coords.z > (map_depth + bias))
        {
            total_depth += map_depth;
            ++blocker_count;
        }
    }

    if(blocker_count == 0)
    {
        return -1.0;
    }
    if(blocker_count == SAMPLE_COUNT)
    {
        return 2.0;
    }
    return total_depth / float(blocker_count);
}

float PCF_of_PCSS(const vec3 projection_coords, const float penumbra_scale, const float bias)
{
    float sum = 0.0;
    vec2 depth_map_size = 1.0 / textureSize(depth_map, 0);
    for(int i=0; i < SAMPLE_COUNT; ++i)
    {
        vec2 sample_texture = projection_coords.xy + poisson_disk[i] * depth_map_size * penumbra_scale;
        float map_depth = texture(depth_map, sample_texture).r;
        sum += (projection_coords.z > (map_depth + bias) ? 1.0 : 0.0);
    }
    return sum / float(SAMPLE_COUNT);
}

float shadow_calculation_use_shadow_map(const vec3 light_direction)
{
    float bias = bias_calculation(light_direction);
    vec3 projection_coords = fs_in.fragment_position_light_space.xyz / fs_in.fragment_position_light_space.w;
    projection_coords = projection_coords * 0.5 + 0.5;
    float map_depth = texture(depth_map, projection_coords.xy).r;
    return map_depth < projection_coords.z - bias ? 1.0 : 0.0;
}

float shadow_calculation_use_PCF(const vec3 light_direction)
{
    float shadow = 0.0;
    vec2 depth_map_size = 1.0 / textureSize(depth_map, 0) * 5.0;
    float bias = bias_calculation(light_direction);
    vec3 projection_coords = fs_in.fragment_position_light_space.xyz / fs_in.fragment_position_light_space.w;
    projection_coords = projection_coords * 0.5 + 0.5;

    poisson_disk_samples(projection_coords.xy);
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        vec2 texture_coord = poisson_disk[i] * depth_map_size + projection_coords.xy;
        float map_depth = texture(depth_map, texture_coord).r;
        shadow += map_depth < projection_coords.z - bias ? 1.0 : 0.0;
    }
    shadow /= float(SAMPLE_COUNT);
    return shadow;
}

float shadow_calculation_use_PCSS(const vec3 light_direction)
{
    float bias = bias_calculation(light_direction);
    vec3 projection_coords = fs_in.fragment_position_light_space.xyz / fs_in.fragment_position_light_space.w;
    projection_coords = projection_coords * 0.5 + 0.5;
    float blocker_depth = calculate_average_blocker_depth(projection_coords, bias);

    if (blocker_depth < 0.0)
    {
        return 0.0;
    }
    if (blocker_depth > 1.5)
    {
        return 1.0;
    }

    float penumbra_scale = (projection_coords.z - blocker_depth) / blocker_depth * 10.0;

    return PCF_of_PCSS(projection_coords, penumbra_scale, bias);
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

    // float shadow = shadow_calculation_use_shadow_map(light_direction);
    // float shadow = shadow_calculation_use_PCF(light_direction);
    float shadow = shadow_calculation_use_PCSS(light_direction);

    vec3 color = pow(ambient + (1.0 - shadow) * (diffuse + specular), vec3(1.0 / gamma));
    fragment_color = vec4(color, 1.0);
}
