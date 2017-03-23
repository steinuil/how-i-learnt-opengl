#version 330 core

in vec2 textureCoords;

out vec4 color;

uniform sampler2D theTexture;
uniform sampler2D twoTexture;
uniform float blending;

vec4 texture2;

void main() {
  color = mix
    ( texture(theTexture, vec2(textureCoords.x, textureCoords.y - 1.0f))
    , texture(twoTexture, vec2(textureCoords.x, textureCoords.y - 1.0f))
    , blending
    );
}
