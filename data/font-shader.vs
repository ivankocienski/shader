#version 130
 
attribute vec4 coord;
varying   vec2 texcoord;
uniform   vec2 screen_res;

void main(void) { 

  //vec2 pos = (coord.xy / screen_res) * 2.0 - 1;

  vec2 pos = coord.xy / screen_res;
  
  pos.y = 1 - pos.y;

  pos *= 2.0;

  // write out
  gl_Position = vec4(pos - 1, 0, 1);

  texcoord = coord.zw;
}
