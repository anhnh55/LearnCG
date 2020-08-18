#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
}; 
  
uniform Material material;
uniform int isDrawingBackFace;
void main()
{
    //if(isDrawingBackFace == 1){
      //  FragColor = vec4(0.04, 0.04, 0.04, 1.0);
    //}else{
        FragColor = vec4(0.04, 0.04, 0.04, 0.5);
    //}
    
    //FragColor = texture(material.texture_diffuse1, TexCoords);
}