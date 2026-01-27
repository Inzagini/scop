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


uniform Material material;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;


out vec4 FragColor;

void main()
{
   vec3 N = normalize(Normal);
   vec3 L = normalize(-lightDir);

   vec3 ambient = material.ambient * lightColor;

   float diff = max(dot(N, L), 0.0);
   vec3 diffuse = diff * material.diffuse * lightColor;


// Specular
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, N);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = spec * material.specular * lightColor;
   
   vec3 result = ambient + diffuse + specular;

   vec3 color = ambient * diffuse * specular;

   FragColor = vec4(color, material.opacity);
}