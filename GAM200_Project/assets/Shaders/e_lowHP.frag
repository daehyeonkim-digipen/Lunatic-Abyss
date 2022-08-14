#version 450 core

in vec2 vPosition;
out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;

float noise1d(float v){
    return cos(v + cos(v * 90.1415) * 100.65) * 0.5 + 0.5;
}

void main() {
    vec2 coord = vPosition - 0.5;
    
    float color;
    color = 1 - 5 * length(coord * coord * coord * 1.1);
    color += 0.1 * noise1d(u_time / 4000);

    vec4 mask = vec4(0.7412, 0.0, 0.0, 1.0);
    
    //color = color;

    FragColor = vec4(1 - color) * mask;
}