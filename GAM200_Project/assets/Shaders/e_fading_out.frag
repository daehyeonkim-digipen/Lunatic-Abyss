#version 450 core

uniform float u_time;
uniform float u_lifeTime;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

in vec2 vPosition;
out vec4 FragColor;


void main(){
  vec2 coord = vPosition;

  FragColor = vec4(0, 0, 0, u_lifeTime);
}