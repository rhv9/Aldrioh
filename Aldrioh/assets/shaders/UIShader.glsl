#type vertex
#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColour;
layout (location = 3) in float aFlags;

out vec2 fTexCoord;
out vec4 fColour;
out float fFlags;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
   gl_Position = u_ViewProjectionMatrix * aPos;
   fTexCoord = aTexCoord;
   fColour = aColour;
   fFlags = aFlags;
}



#type fragment
#version 330 core

in vec2 fTexCoord;
in vec4 fColour;
in float fFlags;

out vec4 FragColor;

uniform sampler2D uTextureSampler;

void main()
{
	vec4 colour;
	vec4 texColour = texture(uTextureSampler, fTexCoord);

	if (fFlags == 1.0)
	{
	}
	else if (texColour.a == 0.0)
		discard;

	FragColor = fColour;
}