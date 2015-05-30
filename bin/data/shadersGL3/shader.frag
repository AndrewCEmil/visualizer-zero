#version 150

out vec4 outputColor;
uniform float myUniform;
in float varyingtexcoord;

void main()
{
    float r = sin(varyingtexcoord * gl_FragCoord.y);
    float g = 0.0;
    float b = 0.0;
    float a = 1.0;
    outputColor = vec4(r, g, b, a);
}