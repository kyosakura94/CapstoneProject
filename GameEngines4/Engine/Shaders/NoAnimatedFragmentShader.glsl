#version 410 core

in vec2 tex_cord;
in vec3 v_normal;
in vec3 v_pos;
in vec4 bw;


out vec4 fragColor;

struct Light
{
	vec3 lightPos;
	float ambient;
	float diffusevalue;
	vec3 lightColor;
};
struct Material
{
	sampler2D diffuseMap;
	float shininess;
	float transparency;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPosition; 
uniform Light light; 
uniform Material material; 

void main()
{
	//ambient

	vec3 ambient = light.ambient * material.ambient * texture(material.diffuseMap, tex_cord).rgb * light.lightColor;
	
	//diffuse

	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(light.lightPos - v_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * texture(material.diffuseMap, tex_cord).rgb * light.lightColor;
	//specular

	vec3 viewDir = normalize(viewPosition-v_pos);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * material.specular) * light.lightColor;
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, material.transparency);


	//fragColor = texture(inputTexture, tex_cord);
	//fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

}