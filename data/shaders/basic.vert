// VERTEX SHADER
#version 430
#define MAX_LIGHTS 8 // Must match Constants::MAX_LIGHTS

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (location = 0) uniform mat4 model;

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

out vec2 TexCoord;

out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace[MAX_LIGHTS];
out vec3 Normal_cameraspace;
out vec3 Tangent_cameraspace;
out vec3 Bitangent_cameraspace;
out vec3 LightDirection_tangentspace[MAX_LIGHTS];
out vec3 EyeDirection_tangentspace;

void main()
{
	TexCoord = vec2(texcoord.x, 1-texcoord.y);
	
	gl_Position = projection * view * model * vec4(position, 1.0);
	
	//Position_worldspace = (model * vec4(position, 1.0)).xyz; // Vertex position in worldspace
	
	vec3 pos_cameraspace = (view * model * vec4(position, 1)).xyz; // Vertex position in camera space
	EyeDirection_cameraspace = vec3(0, 0, 0) - pos_cameraspace; // Vertex location from camera
	
	// Bump mapping
	mat3 MV3x3 = mat3(view * model);
	Normal_cameraspace = (inverse(transpose(view * model)) * vec4(normal, 0)).xyz;
	Tangent_cameraspace = MV3x3 * normalize(tangent);
	Bitangent_cameraspace = (cross(Tangent_cameraspace, Normal_cameraspace));//MV3x3 * normalize(bitangent); 
	
	mat3 TBN = transpose(mat3(
		Tangent_cameraspace,
		Bitangent_cameraspace,
		Normal_cameraspace));
	
	for(int i=0; i<numLights; i++)
	{
		vec3 lightPos_cameraspace = (view * vec4(lights[0].position, 1)).xyz;
		LightDirection_cameraspace[i] = lightPos_cameraspace + EyeDirection_cameraspace;
		
		LightDirection_tangentspace[i] = TBN * LightDirection_cameraspace[i];
	}
}