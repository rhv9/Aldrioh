#type vertex
#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColour;
layout (location = 3) in float aTextureSampler;
layout (location = 4) in float aFlags;

out vec2 fTexCoord;
out vec4 fColour;
out float fFlags;
out float fTextureSampler;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
   gl_Position = u_ViewProjectionMatrix * aPos;
   fTexCoord = aTexCoord;
   fColour = aColour;
   fFlags = aFlags;
   fTextureSampler = aTextureSampler;
}



#type fragment
#version 330 core

in vec2 fTexCoord;
in vec4 fColour;
in float fFlags;
in float fTextureSampler;

out vec4 FragColor;

uniform float uTime;
uniform sampler2D uTextureSamplers[8];

void main()
{
	vec4 colour;
	vec4 texColour = texture(uTextureSamplers[int(fTextureSampler)], fTexCoord);

	if (texColour.a == 0.0)
	{
		discard;
	}

	FragColor = fColour;
}