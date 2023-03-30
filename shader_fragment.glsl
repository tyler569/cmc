#version 330 core

in vec2 uv;
in float textureIndex;

out vec4 color;

uniform sampler2DArray textureSampler;

void main() {
    // color = texture(textureSampler, vec3(uv, textureIndex));
    // color = vec4(textureIndex / 256., 0, 0, 1);
    color = texture(textureSampler, vec3(uv, textureIndex));
}