#version 130

// in_Position was bound to attribute index 0(shaderAttribute)

in  vec3 in_Position;
in  vec3 in_Normal;

uniform vec3 lightDir;
varying float intensity;

void main()
{
  vec4 ld4 = vec4( lightDir, 0.7 );
  vec4 n4  = vec4( in_Normal, 1 );

  intensity = dot(ld4, gl_ModelViewMatrix * n4);
  gl_Position = ftransform();
}
