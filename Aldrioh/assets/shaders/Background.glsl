#type vertex
#version 330 core

const vec2 quadVertices[4] = vec2[]( vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) );
void main()
{
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}

#type fragment
#version 330 core
uniform float uTime;

out vec4 FragColor;

void main()
{
    float x = (sin(uTime*4+gl_FragCoord.x/200.0)+1.0)/2.0;
    float y = (cos(uTime*4+gl_FragCoord.x/200.0)+1.0)/2.0;
    float r=x;
    float g=y;
    float b=0;
    FragColor = vec4(r, g, b, 1.0);
}