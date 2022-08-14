#version 450 core
layout (location = 0) in vec2 vVertexPosition;
layout (location = 1) in vec2 vVertexTexPosition;
out vec2 vTexCoord;

uniform mat3 u_NDC;

void main() {
  vec2 ndcCoord = vec2(u_NDC * vec3(vVertexPosition, 1.f));
  gl_Position = vec4(ndcCoord, 0.0, 1.0);
  vTexCoord = vVertexTexPosition;
}