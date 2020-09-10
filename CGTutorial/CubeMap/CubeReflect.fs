#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
struct Material {
    samplerCube texture_cube1;
}; 
  
uniform Material material;
void main()
{
    vec3 I = normalize(Position - viewPos); //view direction
    vec3 R = reflect(I, normalize(Normal)); //reflect direction
    FragColor = vec4(texture(material.texture_cube1, R).rgb, 1.0);
}