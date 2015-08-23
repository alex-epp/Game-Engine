// Based partly on https://www.packtpub.com/books/content/opengl-40-building-c-shader-program-class

#pragma once
#include <fstream>
#include <gl/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Logger.h"

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::value_ptr;

enum class ShaderType { Vertex, Fragment, Geometry, TessControl, TessEvaluation };

// TODO - Consider adding a reference to a game engine
class ShaderProgram
{
public:
	
	ShaderProgram();
	~ShaderProgram();
	ShaderProgram(ShaderProgram&& other): linked(other.linked), logString(other.logString), handle(other.handle), shaders(std::move(other.shaders))
	{
		other.handle = 0;
		other.linked = false;
		other.logString = "";
	}

	bool compileFromFile(const string& filename, ShaderType type);
	
	bool link();
	bool use();

	string log() { return logString; }

	GLuint getHandle() const { return handle; }
	bool isLinked() { return linked; }

	// For these next functions, the shader must be 'used'

	void bindAttribLocation(GLuint location, string name);
	void bindFragDataLocation(GLuint location, string name);

	template<typename... Ts>
	void setUniform(const string& name, Ts... args);

	void setUniform(int location, float x, float y, float z);
	void setUniform(int location, const vec3 & v);
	void setUniform(int location, const vec4 & v);
	void setUniform(int location, const mat4 & m);
	void setUniform(int location, const mat3 & m);
	void setUniform(int location, float val);
	void setUniform(int location, int val);
	void setUniform(int location, bool val);

	void attachUniformBlock(string name, int index) const;

protected:
	bool compileFromString(const string& source, ShaderType type);

	bool linked;
	string logString;
	GLuint handle;

	vector<GLuint> shaders;

	static map<string, GLuint> shaderMap;
};

template<typename... Ts>
inline void ShaderProgram::setUniform(const string & name, Ts... args)
{
	GLint location = glGetUniformLocation(handle, name.c_str());
	//if (location < 0)
	//	LOG_WARN("Uniform ", name, " could not be found");
	//else
		setUniform(location, args...);
}