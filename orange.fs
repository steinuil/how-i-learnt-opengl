#version 330 core

in vec3 vertexColor;

out vec4 color;

uniform vec4 changeColor;

void main() {
  color = changeColor;
}
