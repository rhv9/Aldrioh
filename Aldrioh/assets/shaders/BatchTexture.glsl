#type vertex
#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColour;
layout (location = 3) in float aTextureSampler;
layout (location = 4) in float aFlags;

uniform mat4 u_ViewProjectionMatrix;

out vec2 fTexCoord;
out vec4 fColour;
out float fFlags;
out float fTextureSampler;

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
	vec4 texColour = vec4(0.0);

	switch (int(fTextureSampler))
	{
		case 0: texColour = texture(uTextureSamplers[0], fTexCoord); break;
		case 1: texColour = texture(uTextureSamplers[1], fTexCoord); break;
		case 2: texColour = texture(uTextureSamplers[2], fTexCoord); break;
		case 3: texColour = texture(uTextureSamplers[3], fTexCoord); break;
		case 4: texColour = texture(uTextureSamplers[4], fTexCoord); break;
		case 5: texColour = texture(uTextureSamplers[5], fTexCoord); break;
		case 6: texColour = texture(uTextureSamplers[6], fTexCoord); break;
		case 7: texColour = texture(uTextureSamplers[7], fTexCoord); break;
	}

	if (texColour.a == 0.0)
		discard;

	if (fFlags == 1.0)
	{
		FragColor = fColour;
		return;
	}

	FragColor = texColour * fColour;
}