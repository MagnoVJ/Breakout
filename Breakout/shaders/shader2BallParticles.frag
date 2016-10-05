#version 330 core

out vec3 color;

in vec2 TexCoords;
in vec2 ParticleColor;

uniform sampler2D sprite;

void main(){
	color = (texture(sprite, TexCoords) * ParticleColor);
}