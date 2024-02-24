#version 330
attribute highp vec4 qt_Vertex;
uniform mat4 lightSpaceMatrix;
uniform mat4 position;

void main(void)
{
    gl_Position = position * qt_Vertex;
}
