#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexIndexCoords;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

uniform float uTexCoords[8];

out vec2 vTexCoord;

void main()
{
   gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(aPos, 1.0);
   vTexCoord = vec2(uTexCoords[int(aTexIndexCoords.x)], uTexCoords[int(aTexIndexCoords.y)]);
}

#type fragment
#version 330 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTextureSampler;
uniform vec4 uChangeCol;
uniform vec4 uChangeColWith;

void main()
{
	vec4 texColour = texture(uTextureSampler, vTexCoord);

 	if (texColour == uChangeCol)
 		texColour = uChangeColWith;
	
	FragColor = texColour;
}