#version 330 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTextureSampler;

void main()
{
	//FragColor = texture(uTextureSampler, vTexCoord);
	FragColor = texture(uTextureSampler, vTexCoord);
	//FragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);
}