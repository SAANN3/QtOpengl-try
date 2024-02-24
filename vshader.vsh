#version 330 core
attribute highp vec4 qt_Vertex;
attribute vec3 aNormal;
attribute vec2 aTexCoords;
//attribute highp vec4 qt_MultiTexCoord0;
uniform highp mat4 qt_ModelViewProjectionMatrix;
//varying highp vec4 qt_TexCoord0;
//uniform mat4 trans;
uniform mat4 position;



out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main(void)
{
    vs_out.FragPos = vec3(position * qt_Vertex);
    vs_out.Normal =(transpose(inverse(mat3(position))))*aNormal;
    vs_out.TexCoords = aTexCoords;
    //Normal = aNormal;



    gl_Position =   qt_ModelViewProjectionMatrix * position * qt_Vertex ;


}
