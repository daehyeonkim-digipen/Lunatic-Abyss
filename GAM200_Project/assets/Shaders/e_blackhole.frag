#version 450 core

in vec2 vPosition;
out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

const float PI = 3.14159265359;

mat2 rotate(float angle){
    return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

float noise1d(float v){
    return cos(v + cos(v * 31.91415 + v) * 13.1415 + 71.348) * 0.5 + 0.5;
}

vec2 rotating_coord(float speed, vec2 coord, float offset) {
    return rotate(u_time * speed) * (coord - 0.5) + offset;
}

const int MAX_RING = 10;
const float RADIUS = 0.3;

void main() {
    vec2 coord = vPosition;

    float ut = pow(2, 16) + u_time;

    vec4 lightLine2 = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 500; i++) {
		float noiseValue = (noise1d(i) - (noise1d(i) * 0.5)) * 0.01 + 0.2;
		vec2 lineShape = (coord - 0.5);
		vec2 rotatingPos = rotate(ut * PI * noiseValue * 3) * lineShape + vec2(-0.13, 0.11);
		float lightLength = (cos(ut * (noiseValue * noise1d(i * 73)) * 20) + 1) * 0.5 + 0.9 - 0.15 + noiseValue;

		vec2 stretchingShape = rotatingPos * vec2(lightLength, 2);

		float line = 1 - smoothstep(-0.5, 0.2, length(stretchingShape + 0.3));
        
		
		lightLine2 += line;
	}

    float dark = 1;
    float darkRad = 0.3;
    dark -= smoothstep(darkRad, darkRad, length((coord - 0.5)));
    dark *= 10;

    FragColor = lightLine2 * 0.3 - vec4(vec3(dark), 0.0);

    FragColor.a += dark;

    FragColor *= vec4(0.3333, 0.0392, 0.3686, 1.0);

    if (FragColor.a != 0) {
        //FragColor = vec4(0, 0, 1, 1);
    }
}