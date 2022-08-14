#version 450 core

in vec2 vPosition;
out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;
uniform float u_dustAlpha;

const float PI = 3.14159265358;

float create_particle(vec2 position) {
        return 1 - smoothstep(0.1, 0.5, length(position));
}

void main() {
    vec2 coord = vPosition - 0.5;
    float blinkingAlpha = 0.3 - 0.2 * abs(sin(u_time));
    blinkingAlpha *= abs(sin(u_dustAlpha / (20 / PI)));
    FragColor = vec4(create_particle(coord)) * vec4(1, 1, 1, blinkingAlpha);
}