#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;

//uniform mat4 u_ViewProjectionMatrix;
//uniform mat4 u_Transform;

void main()
{
   //gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(aPos, 1.0);
   gl_Position = vec4(aPos, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}