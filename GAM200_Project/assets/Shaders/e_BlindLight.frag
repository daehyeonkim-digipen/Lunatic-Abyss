#version 450 core

uniform vec2 u_resolution;
uniform float u_time;

in vec2 vPosition;
out vec4 FragColor;

const float PI = 3.14159265359;

float noise1d(float v){
	return cos(v + cos(v * 90.1415) * 100.1415) * 0.5 + 0.5;
}

mat2 rotate(float angle){
	return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

void main() {
    vec2 coord = vPosition;

	vec4 lightLine = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 2; i++) {
		// float noiseValue = (noise1d(i) - (noise1d(i) * 0.5)) * 0.4;
		// vec2 lineShape = (coord - 0.5);
		// vec2 rotatingPos = rotate(u_time * noiseValue * 10) * lineShape;
		// float lightLength = (cos(u_time * (noiseValue + 0.37) * 13) + 1) * 0.5 + 0.9;

		// vec2 stretchingShape = rotatingPos * vec2(lightLength - 0.4, 30);

		// float line = 1 - smoothstep(0.05, 0.2, length(stretchingShape));

 		float lightLength = (cos(u_time * PI + PI / 3) + 1) * 0.04 + 0.96 - 0.56;
		float line = 1 - smoothstep(0.05, 0.2, length(rotate(i * PI * 0.5) * (coord - 0.5) * vec2(lightLength, 15)));
		line *= 1.5;

		
		lightLine += line;
	}

	vec4 lightSphere = vec4(0.0, 0.0, 0.0, 0.0);
	lightSphere += (1 - smoothstep(0.0, 0.3, length((coord - 0.5) * vec2((cos(u_time * PI) + 1) * 0.1 + 0.9)))) * 2;

	vec4 lightRing2 = vec4(0);
	float ringRad2 = (sin(u_time * PI) + 1) * 0.1 + 0.9;
	ringRad2 *= 0.1;
	const float minRad2 = 0.3;
	lightRing2 += 1 - smoothstep(minRad2 + 0.04, minRad2 + 0.1, length(coord - 0.5) + ringRad2);
	lightRing2 -= 1 - smoothstep(minRad2, minRad2 + 0.04, length(coord - 0.5) + ringRad2);
	lightRing2 *= 0.7;

	FragColor = lightSphere + lightLine + lightRing2; 

	if (FragColor.a == 0) {
		//FragColor += vec4(0, 0, 1, 1);
	}
}