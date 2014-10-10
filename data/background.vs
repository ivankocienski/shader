#version 130
 
attribute vec4 coord;
varying   vec2 texcoord;

void main(void) { 
  vec2 pos = coord.xy;
  
  // write out
  gl_Position = vec4(pos, 0, 1);
  texcoord    = coord.zw;
}
