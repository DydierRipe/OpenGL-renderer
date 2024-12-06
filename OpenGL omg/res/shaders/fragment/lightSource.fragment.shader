#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_tex_coord;
in float v_tex_ID;

//uniform int u_texSize;
//uniform sampler2D u_texColor[16];

void main() {

    //int index = int(v_tex_ID);
    //index = clamp(index, 0, u_texSize - 1);

    //if (index == 0) {
        color = vec4(1.0, 0.0);
    //}
    //else {
        //color = texture(u_texColor[index], v_tex_coord) * v_color;
    //}
}