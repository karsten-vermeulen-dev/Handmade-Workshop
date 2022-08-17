#version 460

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;

	//float attenuationLinear;
	//float attenuationConstant;
	//float attenuationQuadratic;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 vertexOut;
in vec4 colorOut;
in vec2 textureOut;
in vec3 normalOut;

out vec4 pixelColor;

uniform bool isTextured;
uniform sampler2D textureImage;

uniform Light light;
uniform Material material;
uniform vec3 cameraPosition;

//=====================================================================
//float Attenuation(in Light light)
//{
//	float lightDistance = length(light.position - vertexOut);
//	return 1.0 / (light.attenuationConstant + 
//	              light.attenuationLinear * lightDistance + 
//	              light.attenuationQuadratic * lightDistance * lightDistance);
//}
//=====================================================================
void main(void)
{
	//Ambient color
	vec3 ambientColor = light.ambient * material.ambient;

	//Diffuse color	
	vec3 lightDirection = normalize(light.position - vertexOut);
	float lightIntensity = max(dot(lightDirection, normalOut), 0.0);
	vec3 diffuseColor = light.diffuse * material.diffuse * lightIntensity;

	//Specular color
	vec3 viewDirection = normalize(cameraPosition - vertexOut);
	vec3 reflection = reflect(-lightDirection, normalOut);
	float specularTerm = pow(max(dot(viewDirection, reflection), 0.0), material.shininess);
	vec3 specularColor = light.specular * material.specular * specularTerm;

	//Final color
	vec3 finalColor = min((ambientColor + diffuseColor + specularColor), 1.0);

	//add on ATTENUATION before appending to final color value
	//pixelColor += vec4(totalColor) * Attenuation(lights[i]), 1.0);
			
	pixelColor += (colorOut * vec4(finalColor, 1.0));
	
	//if object is also textured then add texel color value
	if(isTextured)
	{
		vec2 uv;
		uv.x = textureOut.x;
		uv.y = 1 - textureOut.y;
		pixelColor *= texture(textureImage, uv);
	}
}