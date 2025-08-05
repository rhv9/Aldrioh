#type vertex
#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 colour;
layout (location = 3) in uint flags;

out vec2 fTexCoord;
out vec4 fColour;
out uint fFlags;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
   gl_Position = u_ViewProjectionMatrix * aPos;
   fTexCoord = aTexCoord;
   fColour = colour;
}



#type fragment
#version 330 core

in vec2 fTexCoord;
in vec4 fColour;
in uint fFlags;

out vec4 FragColor;

uniform sampler2D uTextureSampler;

void main()
{
	vec4 colour;
	if (fFlags == 0)
		colour = texture(uTextureSampler, vTexCoord);
	else
		colour = fColour;

	if (colour.a == 0.0)
		discard;

	FragColor = colour;
}