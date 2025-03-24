
#version 300 es
precision mediump float;

in vec2 vertexPosition;

void main () {
  gl_Position = vec4(vertexPosition, 0.0, 1.0);
}



#version 300 es
precision mediump float;

out vec4 triangleColor;

void main() {
  triangleColor = vec4(0.294, 0.0, 0.51, 1.0);
}