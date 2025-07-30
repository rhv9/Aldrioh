#type vertex
#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_ViewProjectionMatrix;

out vec2 vTexCoord;
out float vZ;

void main()
{
   gl_Position = u_ViewProjectionMatrix * aPos;
   vTexCoord = aTexCoord;
}

#type fragment
#version 330 core

in vec2 vTexCoord;
in float vZ;

out vec4 FragColor;

uniform sampler2D uTextureSampler;

void main()
{
	vec4 texColour = texture(uTextureSampler, vTexCoord);

	if (texColour.a == 0.0)
		discard;

	FragColor = texColour;
}