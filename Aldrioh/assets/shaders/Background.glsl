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
uniform vec2 uResolution;

out vec4 FragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / uResolution;

    float d = length(uv);

    FragColor = vec4(uv.y, 0. , 0., 1.0);
}