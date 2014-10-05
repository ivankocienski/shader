#version 130

// in_Position was bound to attribute index 0(shaderAttribute)

in  vec3 in_Position;
in  vec3 in_Normal;

uniform vec3 lightDir;
varying float intensity;

void main()
{
  intensity = dot(lightDir, in_Normal);
  gl_Position = ftransform();
}
