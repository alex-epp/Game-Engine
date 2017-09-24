// FRAGMENT SHADER
#version 430
#define MAX_LIGHTS 8 // Must match Constants::MAX_LIGHTS

in vec2 TexCoord;

in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[MAX_LIGHTS];
in vec3 Normal_cameraspace;
in vec3 Tangent_cameraspace;
in vec3 Bitangent_cameraspace;
in vec3 LightDirection_tangentspace[MAX_LIGHTS];
in vec3 EyeDirection_tangentspace;

struct Material // Accessed by material.* for simplicity
{
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	sampler2D shininessTexture;
	sampler2D normalsTexture;
	
	bool diffuseTextureFlag, specularTextureFlag, shininessTextureFlag, normalsTextureFlag;
	
	vec3 diffuseColour;
	vec3 ambientColour;
	vec3 specularColour;
	
	float shininess;
};
uniform Material material;

struct Light
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
	float radius;
};
layout (std140) uniform FrameData
{
	mat4 view;
	mat4 projection;
	
	float time;
	
	int numLights; vec2 filler;
	Light lights[MAX_LIGHTS];
};

out vec4 FragColour;

vec3 getAmbient(Light light)
{
	return light.ambient.rgb * light.ambient.a;
}
vec3 getDiffuse(Light light, vec3 normal, vec3 lightDir)
{
	float diff = clamp(dot(normal, lightDir), 0, 1);
	return light.diffuse.rgb * light.diffuse.a * diff;
}
vec3 getSpecular(Light light, vec3 eyeDir, vec3 lightDir, vec3 normal)
{
	vec3 reflectDir = reflect(-lightDir, normal);
	float sh = material.shininessTextureFlag ? texture(material.shininessTexture, TexCoord).x : material.shininess;
	float spec = pow(clamp(dot(eyeDir, reflectDir), 0, 1), sh);
	return light.specular.rgb * light.specular.a * spec;
}
float getAttenuation(Light light, float len)
{
	float denom = max(len - light.radius, 0)/light.radius + 1;
	return 1.0 / (denom*denom);
}

void main()
{
	vec3 ambient = vec3(0), diffuse = vec3(0), specular = vec3(0);
	// Apply lights
	vec3 n, l, E;
	float atten, len;
	if(material.normalsTextureFlag)
	{
		vec3 Normal_tangentspace = normalize(texture(material.normalsTexture, TexCoord).rgb*2.0 - 1.0);
		n = normalize(Normal_tangentspace);
		E = normalize(EyeDirection_tangentspace);
	}
	else
	{
		n = normalize(Normal_cameraspace);
		E = normalize(EyeDirection_cameraspace);
	}
	
	for(int i=0; i<numLights; i++)
	{
		l = material.normalsTextureFlag ? normalize(LightDirection_tangentspace[i]) : normalize(LightDirection_cameraspace[i]);
		len = length(LightDirection_cameraspace[i]);
		atten = getAttenuation(lights[i], len);
		ambient += getAmbient(lights[i]) * atten;
		diffuse += getDiffuse(lights[i], n, l) * atten;
		specular += getSpecular(lights[i], E, l, n) * atten;
	}
	
	// Apply materials
	ambient *= material.ambientColour;
	diffuse *= material.diffuseColour;
	specular *= material.specularColour;
	
	// Apply textures
	if(material.diffuseTextureFlag)
	{
		ambient *= texture(material.diffuseTexture, TexCoord).xyz;
		diffuse *= texture(material.diffuseTexture, TexCoord).xyz;
	}
	if(material.specularTextureFlag)
		specular *= texture(material.specularTexture, TexCoord).xyz;
	

	// Final colour
	FragColour = vec4(ambient + diffuse + specular, 1.0);
	// Colour correction
	//const vec3 gamma = vec3(1.0/2.2);
	//FragColour = vec4(pow(colour, gamma), 1);

	//FragColour = vec4(texture(material.normalsTexture, TexCoord).xyz, 1);
}