#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
uniform float leftxv;
uniform float leftyv;
uniform float rightxv;
uniform float rightyv;

out float leftx;
out float lefty;
out float rightx;
out float righty;


void main(){
    gl_Position = modelViewProjectionMatrix * position;
    leftx = leftxv;
    lefty = leftyv;
    rightx = rightxv;
    righty = rightyv;
}