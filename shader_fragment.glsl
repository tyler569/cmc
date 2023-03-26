#version 330 core
out vec3 color;

in vec3 fragmentColor;

void main() {
    color = fragmentColor;
    // color = (1. - gl_FragCoord.zzz) * 20.;
}