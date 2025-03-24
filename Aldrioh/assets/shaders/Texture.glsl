#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

out vec2 vTexCoord;

void main()
{
   gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(aPos, 1.0);
   vTexCoord = aTexCoord;
}

#type fragment
#version 330 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTextureSampler;

void main()
{
	//FragColor = texture(uTextureSampler, vTexCoord);
	vec4 texColour = texture(uTextureSampler, vTexCoord);

	if (texColour.a < 0.1)
		discard;

	FragColor = texColour;
	//FragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);
}