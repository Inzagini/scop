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
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool colorEnabled;
uniform bool wireframe;

out vec4 FragColor;

void main()
{
   if (wireframe) {
      FragColor = vec4(0.05, 0.05, 0.05, 1.0);
      return;
   }

   vec3 texColor = hasTexture ? texture(diffuseTex, TexCoord).rgb : vec3(1.0);

   vec3 ambient = material.ambient * lightColor * 0.1;

   vec3 norm = normalize(Normal);
   vec3 L = normalize(lightDir);

   float diff = max(dot(norm, L), 0.0);

   vec3 diffuse = diff * material.diffuse * texColor * lightColor; 

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-L, norm);
   
   float shininess = max(material.shininess, 1.0);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = lightColor * (spec * material.specular); 
   
   vec3 finalColor = colorEnabled ? (ambient + diffuse + specular) : vec3(0.7);

   FragColor = vec4(finalColor, material.opacity);
}
