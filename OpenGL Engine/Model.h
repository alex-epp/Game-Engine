#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <vector>
#include "Mesh.h"
#include "Constants.h"

using glm::mat4;
using glm::vec3;
using glm::translate;
using glm::rotate;
using glm::scale;
using namespace std;

class Model
{
public:
	Model();
	~Model();

	bool loadFromFile(string filepath, string filename);
	void attachUniformBlock(string name, int index) const;

	void render();

	mat4 getModelMatrix();
	void translateTo(vec3 translation);
	void translateBy(vec3 translation);
	void rotateTo(float angle, vec3 axis);
	void rotateBy(float angle, vec3 axis);
	void scaleTo(vec3 factor);
	void scaleBy(vec3 factor);

private:
	mat4 modelMatrix, translationMatrix, rotationMatrix, scaleMatrix;
	mat4 VPlast;
	vector<Mesh> meshes;

	bool modelMatUpdated;
};

