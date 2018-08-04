#version 120


varying vec4 color;
uniform sampler2D texunit0;
uniform vec4 tint;



void main() {
   
vec4 base = texture2D(texunit0, color.xy);    

gl_FragColor=base*tint;     


}             