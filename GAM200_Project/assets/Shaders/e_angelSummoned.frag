#version 450 core

uniform float u_time;
uniform vec2 u_resolution;

in vec2 vPosition;
out vec4 FragColor;

float noise1d(float v){
    return 0.5 * cos(v) * sin(4 * v) * cos(3 * v) * sin(13 * v) + 0.5;
}

const float range = 0.35;

void main(){
    vec2 coord = vPosition - 0.5;
    float color = 1;
    float back = 1;

    back -= smoothstep(range * 0.8, range, coord.x);
    back -= smoothstep(range * -0.8, -range, coord.x);

    color -= smoothstep(range * 0.8, range, coord.x);
    color -= smoothstep(range * -0.8, -range, coord.x);

    color *= sin(coord.x * 3.0 + 0.5 * cos(u_time + sin(coord.x * 30.0 + u_time * 5))) * 2.0;
    color *= cos(coord.x * 4.0 + 0.5 * sin(u_time + cos(coord.x * -10.0 + u_time * 5))) * 2.0;
    color *= sin(coord.x * 8.0 + 0.5 * cos(u_time + sin(coord.x * -20.0 + u_time * 5))) * 2.0;
    color *= cos(coord.x * 7.0 + 0.5 * sin(u_time + cos(coord.x * -50.0 + u_time * 5))) * 2.0;

    FragColor = 2 * vec4(back) * vec4(0.251, 0.1725, 0.3804, 1.0) + 0.2 * vec4(color);
    //FragColor = 2 * vec4(back) * vec4(noise1d(coord.x), noise1d(coord.y), noise1d(color), 1.0) + 0.2 * vec4(color);
}