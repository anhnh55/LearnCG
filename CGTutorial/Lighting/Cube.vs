#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform mat3 normMat;
out vec3 Normal;
out vec3 FragPos;  
out vec2 TexCoord;

void main()
{
    gl_Position = projectionMat*viewMat*modelMat*vec4(aPos, 1.0);
    FragPos = vec3(modelMat * vec4(aPos, 1.0));
    Normal = normMat * aNormal;  
    TexCoord = aTexCoord;
}