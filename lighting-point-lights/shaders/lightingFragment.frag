#version 330 core
out vec4 FragColor;
  
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
struct Material {   //We created this so we should be able to alter shininess on the object type
    //We also remove the ambient material color vector 
    //since the ambient color is equal to the diffuse color anyways now that we control ambient with the light.
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {      //For being able to set the ambient, diffuse and specular lighting individually
    vec3 position; 
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

void main()
{
    //ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    //diffuse 

    //When calculating lighting we usually do not care about the magnitude of a vector or their position; we only care about their direction. 
    //Because we only care about their direction almost all the calculations are done with unit vectors since it simplifies most calculations 
    //(like the dot product). So when doing lighting calculations, 
    //make sure you always normalize the relevant vectors to ensure they're actual unit vectors. 
    //Forgetting to normalize a vector is a popular mistake.
    
    vec3 norm = normalize(Normal);

    //The light's direction vector is the difference vector between the light's position vector and the fragment's position vector.

    vec3 lightDir = normalize(light.position - FragPos); 
    // we have to negate the global light direction vector to switch its direction; it's now a direction vector pointing towards the light source.

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  //The reflect function expects the first vector to point from the light source
                                                 //towards the fragment's position, but the lightDir vector is 
                                                 //currently pointing the other way around: 
                                                 //from the fragment towards the light source 
                                                 //(this depends on the order of subtraction earlier on when we calculated the lightDir vector).
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //We set the power of dot product of view&reflect
                                                                              //direction to configure the shininess
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;
        ambient  *= attenuation; 
diffuse  *= attenuation;
specular *= attenuation; 
    //We first calculate the dot product between the view direction and the reflect direction (and make sure it's not negative) 
    //and then raise it to the power of 32. This 32 value is the shininess value of the highlight. 
    //The higher the shininess value of an object,the more it properly reflects the light
    //instead of scattering it all around and thus the smaller the highlight becomes.
    //We don't want the specular component to be too distracting so we keep the exponent at 32.

    vec3 result = ambient + diffuse + specular ;
    FragColor = vec4(result, 1.0);
}