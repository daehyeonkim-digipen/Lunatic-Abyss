#version 450 core

// Hyunjin Kim
// Exercise - water color
// cs250
// 2022 spring

in vec2 vPosition;
out vec4 FragColor;

uniform float u_time;

float random1d(float val) {
    return fract(fract(sin(dot(vec2(val, val * 10.195783), vec2(12.9898, 78.265433))) * 4758.5453) 
            + fract(cos(val * 10.1934) * 548.92753) * 5.19384);
}

void main() {
    vec2 coord = 6.0 * vPosition; // gl_FragCoord.xy / u_resolution.xy => vPosition;
    coord.x *= 8.0;

    for (int n = 1; n < 58; n++) {
        float i = float(n);
        float x = 0.7 / i * sin(i * coord.x + u_time + 0.2 * random1d(i));
        float y = 2 / i * cos(coord.y + u_time + 0.3 * i * x) + 1.6;
        coord += vec2(y - 0.4, x);
    }
        
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    color *= coord.y / coord.x;

    color *= 20;

    FragColor = color * vec4(0.3804, 0.0, 0.0, 1.0);
}