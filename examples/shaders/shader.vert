#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 col;
layout(location = 0) out vec4 outColor;
void main() 
{
    gl_Position = pos;
    outColor = col;
}