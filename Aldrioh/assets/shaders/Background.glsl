#type vertex
#version 330 core

const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
void main()
{
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1, 1, 0, 1);
}