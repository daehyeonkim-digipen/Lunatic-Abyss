#version 450 core

in vec2 vPosition;

uniform vec2 u_resolution;
uniform float u_time;
uniform vec4 u_color = vec4(0.2667, 0.0, 1.0, 1.0);

out vec4 FragColor;

mat2 scale(vec2 scale){
	return mat2(scale.x, 0.0, 0.0, scale.y);
}

void main() {
    vec2 coord = vPosition;
	coord.y *= 10;
	coord.y -= 0.2;

	vec4 ring = vec4(1);
	ring -= smoothstep(0.4, 0.5, length(coord - 0.5));
	ring -= 1 - smoothstep(0.2, 0.35, length(coord - 0.5));
	ring -= sin(u_time * 2.5) * 0.1 + 0.1;
	ring *= 0.5;

	vec4 ring2 = vec4(1);
	ring2 -= smoothstep(0.3, 0.5, length(coord - 0.5));
	ring2 -= 1 - smoothstep(0.0, 0.4, length(coord - 0.5));
	ring2 -= sin(u_time * 2.5) * 0.1 + 0.1;

	vec4 roundMask = vec4(1);
	roundMask -= smoothstep(0.99, 1.0, length((coord - vec2(0.5, 99.5)) * vec2(0.15, 0.01)));

	vec4 light = vec4(1);
	light -= smoothstep(0.95, 1.0, coord.x);
	light -= smoothstep(0.95, 1.0, 1 - coord.x);
	light *= 0.5 + (1 - coord.y) * 0.05;

    FragColor = roundMask * light + ring;
	FragColor *= u_color;
	FragColor += ring2;
	FragColor *= 0.8;
}