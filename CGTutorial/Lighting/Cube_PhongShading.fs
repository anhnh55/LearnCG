#version 330 core
out vec4 FragColor;
in vec3 Normal; 
in vec3 FragPos;  
uniform vec3 lightPos;  
uniform vec3 viewPos;

  
void main()
{
    vec3 ambientColor = vec3(1,0,0);
    vec3 diffuseColor = vec3(1,0,0);
    vec3 specularColor = vec3(1,1,1);
    float shininess = 32;
    float Ka = 0.4;
    float Kd = 0.7;
    float Ks = 1.0;

    //ambient
    vec3 ambient = Ka * ambientColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float LdotN = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Kd* LdotN * diffuseColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float RdotV = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = Ks * RdotV * specularColor;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}