#version 450 core
layout (location = 0) in vec2 aPosition;
out vec2 vPosition;

uniform mat3 u_NDC;
uniform mat3 u_model;

void main() {
  vec2 vertPosition = vec2(u_NDC * u_model * vec3(aPosition, 1.f)); // i can't get it :/
  gl_Position = vec4(vertPosition, 0.0, 1.0);
  vPosition = aPosition;
}