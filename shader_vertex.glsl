#version 330 core

layout(location = 0) in vec3 vertex_position_model_space;
layout(location = 1) in vec3 vertex_color;

out vec3 fragment_color;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertex_position_model_space, 1);
    fragment_color = vertex_position_model_space / 2. + 0.5;
    // fragment_color = vertex_color;
}