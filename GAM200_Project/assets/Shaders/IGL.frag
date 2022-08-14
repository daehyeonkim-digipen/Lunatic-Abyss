#version 450 core
in vec2 vTexCoord;
out vec4 fFragColor;

uniform sampler2D u_texture;
uniform mat3 u_model;
uniform vec4 u_color;

void main() {
    vec2 transformedTexCoord = vec2(u_model * vec3(vTexCoord, 1.f));
    fFragColor = texture(u_texture, transformedTexCoord) * u_color;
}