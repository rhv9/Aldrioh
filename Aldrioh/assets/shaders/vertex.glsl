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