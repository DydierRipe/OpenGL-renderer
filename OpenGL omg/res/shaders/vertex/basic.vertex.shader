#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in float tex_ID;
layout(location = 4) in float mat_ID;

out vec2 v_tex_coord;
out float v_tex_ID;
out vec3 v_normal;
out vec3 v_fragmentInWorld;

uniform mat4 u_model[30];
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    int mati = int(mat_ID);
    vec4 fragmentPos = u_model[mati] * vec4(position, 1.0);
    
    v_tex_coord = tex_coord;
    v_tex_ID = tex_ID;
    v_normal = normalize(mat3(transpose(inverse(u_model[mati]))) * normal);
    v_fragmentInWorld = vec3(fragmentPos);

    gl_Position = u_proj * u_view * fragmentPos;
}