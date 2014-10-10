#version 130

// in_Position was bound to attribute index 0(shaderAttribute)

in  vec3 in_Position;
in  vec3 in_Normal;


void main() {

  vec4 v = ftransform();
  v.z = 0;

  gl_Position = v; // ftransform();
}
