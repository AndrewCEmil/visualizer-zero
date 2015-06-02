#version 150

out vec4 outputColor;
in float leftx;
in float lefty;
in float rightx;
in float righty;

void main()
{
    float leftdist = sqrt(((gl_FragCoord.x - leftx) * (gl_FragCoord.x - leftx)) + ((gl_FragCoord.y - lefty) * (gl_FragCoord.y - lefty)));
    float rightdist = sqrt(((gl_FragCoord.x - rightx) * (gl_FragCoord.x - rightx)) + ((gl_FragCoord.y - righty) * (gl_FragCoord.y - righty)));
    float plotwidth = 1024.0;
    float plotheight = 768.0;
    float scl = sqrt((plotheight * plotheight) + (plotwidth * plotwidth));
    float val = ((leftdist + rightdist) / 2.0) / scl;
    float r = val;
    float g = val;
    float b = val;
    float a = 1.0;
    outputColor = vec4(r, g, b, a);
}