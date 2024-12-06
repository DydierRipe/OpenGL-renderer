#version 330 core

layout(location = 0) out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 position;

    float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 position;
    vec3 direction;

    float constant;
	float linear;
	float quadratic;

    float cutOff;
    float innerCutOff;
};

struct phongVal {
    vec3 texDiffAmb;
    vec3 texSpecular;
};

const int NR_POINT_LIGHTS = 1;
const int NR_SPOT_LIGHTS = 2;

in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_fragmentInWorld;

phongVal vals;
uniform Material u_material;
uniform DirLight u_dirLight;
uniform vec3 u_camPos;
uniform PointLight u_pointLights[NR_POINT_LIGHTS];
uniform SpotLight u_spotLights[NR_SPOT_LIGHTS];

bool isDirL = true;

float getSpecular(vec3 lightDir, vec3 normal, vec3 viewDir);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main() {
    vals.texDiffAmb = texture(u_material.diffuse, v_tex_coord).rgb;
    vals.texSpecular = texture(u_material.specular, v_tex_coord).rgb;

    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(u_camPos - v_fragmentInWorld);

    vec3 result = vec3(0.0);

    if (isDirL) result = calcDirLight(u_dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(u_pointLights[i], norm, viewDir);
    }

    for(int i = 0; i < NR_SPOT_LIGHTS; i++) {
        result += calcSpotLight(u_spotLights[i], norm, viewDir);
    }
    color = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    float spec = getSpecular(lightDir, normal, viewDir);

    vec3 ambient = light.ambient * vals.texDiffAmb;
    vec3 diffuse = light.diffuse * diff * vals.texDiffAmb;
    vec3 specular = light.specular * spec * vals.texSpecular;

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - v_fragmentInWorld);
    float diff = max(dot(normal, lightDir), 0.0);

    float spec = getSpecular(lightDir, normal, viewDir);

    float fragmentDistance = length(light.position - v_fragmentInWorld);
    float attenuation = 1.0 / (light.constant + light.linear * fragmentDistance + light.quadratic * (fragmentDistance * fragmentDistance));

    vec3 ambient = light.ambient * vals.texDiffAmb;
    vec3 diffuse = light.diffuse * diff * vals.texDiffAmb;
    vec3 specular = light.specular * spec * vals.texSpecular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - v_fragmentInWorld);
    float diff = max(dot(normal, lightDir), 0.0);;

    float spec = getSpecular(lightDir, normal, viewDir);
    
    float distance = length(light.position - v_fragmentInWorld);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.innerCutOff - light.cutOff;
    float intensity = clamp((theta - light.cutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * vals.texDiffAmb;
    vec3 diffuse = light.diffuse * diff * vals.texDiffAmb;
    vec3 specular = light.specular * spec * vals.texSpecular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float getSpecular(vec3 lightDir, vec3 normal, vec3 viewDir) {
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
}
