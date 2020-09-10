#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewMat;
uniform mat4 projectionMat;

out vec3 TexCoords;

void main()
{
    vec4 pos = projectionMat*viewMat*vec4(aPos, 1.0); 
    TexCoords = aPos;

    //to get depth = 1
    gl_Position = pos.xyww;
}