#version 330 core
out vec4 FragColor;
in vec3 TexCoords;

struct Material {
    samplerCube texture_cube1;
}; 
  
uniform Material material;

void main()
{
    FragColor = texture(material.texture_cube1, TexCoords);
}