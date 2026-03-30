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
uniform vec2 uScrollingOffset;
 
out vec4 FragColor; 

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
// Also https://thebookofshaders.com/11/
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}
void main() 
{
    vec2 uv = gl_FragCoord.xy / uResolution;
    uv.x = uv.x * (uResolution.x / uResolution.y);
    vec3 colour = vec3(0.0);
    float d = length(uv);
    

    vec2 pos = vec2(uv*100.0) + uScrollingOffset * 2.0;
    colour = vec3( pow(noise(pos)*.5 + 0.73, 50) );
    //colour = vec3( pow(noise(pos), 1) );

    FragColor = vec4(colour, 1.0);
}