#version 130
 
varying vec2 texcoord;
uniform sampler2D tex;
 
void main(void) {
  //gl_FragColor = vec4(1, 0, 0, 1); //vec4( texture2D(tex, texcoord).xyz, 1 );
  gl_FragColor = vec4( texture2D(tex, texcoord).xyz, 1 );
}
