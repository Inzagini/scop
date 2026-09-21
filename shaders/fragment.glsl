#version 330 core

struct Material
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float opacity;
   float shininess;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform Material material;
uniform sampler2D diffuseTex;
uniform bool hasTexture;
uniform vec3 lightDir;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool colorEnabled;

out vec4 FragColor;

void main()
{
   vec3 texColor = hasTexture ? texture(diffuseTex, TexCoord).rgb : vec3(1.0);

   vec3 ambient = material.ambient * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);

   float diff = max(dot(norm, lightDir), 0.0);

   vec3 diffuse = diff * material.diffuse * texColor * lightColor; 

// Specular
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   
   float shininess = max(material.shininess, 1.0);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = lightColor * (spec * material.specular); 
   
   vec3 finalColor = colorEnabled ? (ambient + diffuse + specular) : texColor * 0.7;

   FragColor = vec4(finalColor, material.opacity);
}
