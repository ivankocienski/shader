#version 130 // Specify which version of GLSL we are using.

precision highp float; // Video card drivers require this line to function properly

out vec4 fragColor;
varying float intensity;

void main()
{
  fragColor = vec4(intensity, 0.4, 0.0, 1.0); 
}
