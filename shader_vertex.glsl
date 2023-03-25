#version 330 core

layout(location = 0) in vec3 vertex_position_model_space;

void main() {
    gl_Position.xyz = vertex_position_model_space;
    gl_Position.w = 1.0;
}