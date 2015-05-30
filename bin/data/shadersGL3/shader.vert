#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
uniform float myUniform;
out float varyingtexcoord;



void main(){
    gl_Position = modelViewProjectionMatrix * position;
    varyingtexcoord = myUniform;
}