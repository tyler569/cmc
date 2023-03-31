#version 330 core

layout(location = 0) in vec3 vertexPositionModelSpace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;
layout(location = 3) in float vertexTextureIndex;

out vec3 normal;
out vec2 uv;
out float textureIndex;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPositionModelSpace, 1);
    uv = vertexUv;
    textureIndex = vertexTextureIndex;
    normal = vertexNormal;
}
