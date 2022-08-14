#version 450 core

uniform vec2 u_resolution;
uniform float u_time;

in vec2 vPosition;
out vec4 FragColor;

mat2 scale(vec2 scale){
    return mat2(1 / scale.x, 0.0, 0.0, 1 / scale.y);
}

const float outerRadius = 0.45;

void main()
{
    vec2 coord = vPosition - 0.5;
    vec2 scaled = scale(vec2(0.9, 0.1)) * coord;
    float color = 1; 

    color -= smoothstep(0.41, outerRadius, length(scaled));
    color -= smoothstep(outerRadius, 0.3, length(scaled));
    color *= 0.7 + abs(0.3 * cos(u_time * 2.1));

    color *= 3;

    FragColor = vec4(color) * vec4(0.3098, 0.0, 0.8863, 1.0);
}