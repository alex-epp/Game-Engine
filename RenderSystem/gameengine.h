#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Constants.h"
#include "../Core/Logger.h"
#include "Model.h"
#include "Input.h"

enum class LightType	
{
	Directional = 0, Positional = 1
};

struct Light
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position; // Or direction
	float radius;
};

struct FrameData
{
	mat4 view;
	mat4 projection;

	float time;

	int numLights; vec2 filler;
	Light lights[Constants::MAX_LIGHTS];
};

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void mainLoop();

	void resizeWindow(int width, int height);
private:
	bool init();
	void quit();

	void attachFrameData(const Model& model);
	void updateFrameData();

	void updateMatrices(mat4 projection, mat4 view);
	void addLight(Light light);

	// For the openGL context
	bool created;
	GLFWwindow* window;

	GLuint frameDataUBO;
	FrameData frameData;
};