#version 330 core

layout(location = 0) in vec3 vertexPositionModelSpace;
layout(location = 1) in vec2 vertexUv;

out vec3 fragmentColor;
out vec2 uv;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPositionModelSpace, 1);
    uv = vertexUv;
}