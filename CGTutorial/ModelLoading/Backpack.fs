#version 330 core
out vec4 FragColor;
in vec3 Normal; 
in vec3 FragPos;  
in vec2 TexCoord;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff*vec3(texture(material.texture_diffuse1, TexCoord)) * light.diffuse;

    vec3 ambient = vec3(texture(material.texture_diffuse1, TexCoord))* light.ambient;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * vec3(texture(material.texture_specular1, TexCoord)) * light.specular;  

    FragColor = vec4((ambient + diffuse + specular), 1.0);
    //FragColor = texture(material.texture_diffuse1, TexCoord);
}