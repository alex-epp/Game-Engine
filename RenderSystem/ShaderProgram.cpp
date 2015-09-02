#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
	linked = false;
}

ShaderProgram::~ShaderProgram()
{
	for (auto shader : shaders)
		if(glIsShader(shader))
			glDeleteShader(shader);

	if(glIsProgram(handle) == GL_TRUE)
		glDeleteProgram(handle);
}

bool ShaderProgram::compileFromFile(const string & filename, ShaderType type)
{
	auto loc = shaderMap.find(filename);
	if (loc != shaderMap.end())
	{
		shaders.push_back(loc->second);
		return true;
	}

	ifstream infile(filename.c_str());
	if (infile.fail())
	{
		LOG_ERR("Could not load ", filename);
		return false;
	}
	string line, source;

	while (true)
	{
		getline(infile, line);
		source += line + "\n";

		if (infile.eof()) break;
	}
	return compileFromString(source, type);
}
bool ShaderProgram::compileFromString(const string & source, ShaderType type)
{
	// Create the shader object
	GLuint shader = 0;
	switch (type)
	{
	case ShaderType::Fragment:       shader = glCreateShader(GL_FRAGMENT_SHADER); break;
	case ShaderType::Geometry:       shader = glCreateShader(GL_GEOMETRY_SHADER); break;
	case ShaderType::TessControl:    shader = glCreateShader(GL_TESS_CONTROL_SHADER); break;
	case ShaderType::TessEvaluation: shader = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
	case ShaderType::Vertex:         shader = glCreateShader(GL_VERTEX_SHADER); break;
	}
	if (shader == 0)
	{
		LOG_ERR("Could not create shader: ", gluErrorString(glGetError()));
		return false;
	}

	// Compile the shader
	const GLchar* codeArray[] = { source.c_str() };
	glShaderSource(shader, 1, codeArray, NULL);
	glCompileShader(shader);

	// Verify compilation
	/*GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		logString = "Vertex shader compilation failed:\n";
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			// TODO - Replace this C code
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);

			logString += "Shader Log: " + string(log);

			free(log);
		}
		return false;
	}*/
	shaders.emplace_back(shader);
	return true;
}

bool ShaderProgram::link()
{
	if (linked)
	{
		LOG_WARN("Program is already linked");
		return false;
	}
	
	// Create the shader program
	handle = glCreateProgram();
	if (handle == 0)
	{
		LOG_ERR("Program object creation failed: ", glGetError());
		return false;
	}

	// Attach each shader
	for (auto shader : shaders)
	{
		glAttachShader(handle, shader);
	}
	
	// Link the program
	glLinkProgram(handle);

	// Verify linkage
	GLint status = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		logString = "Failed to link shader program:\n";

		GLint logLen;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(handle, logLen,
				&written, log);
			logString += "Program log: " +  string(log);
			free(log);
		}
		return false;
	}

	linked = true;

	return true;
}

bool ShaderProgram::use()
{
	glUseProgram(handle);
	return true;
}

void ShaderProgram::bindAttribLocation(GLuint location, string name)
{
	glBindAttribLocation(handle, location, name.c_str());
}
void ShaderProgram::bindFragDataLocation(GLuint location, string name)
{
	glBindFragDataLocation(handle, location, name.c_str() );
}

void ShaderProgram::setUniform(int location, float x, float y, float z)
{
	glUniform3f(location, x, y, z);
}
void ShaderProgram::setUniform(int location, const vec3 & v)
{
	glUniform3fv(location, 1, value_ptr(v));
}
void ShaderProgram::setUniform(int location, const vec4 & v)
{
	glUniform4fv(location, 1, value_ptr(v));
}
void ShaderProgram::setUniform(int location, const mat4 & m)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m));
}
void ShaderProgram::setUniform(int location, const mat3 & m)
{
	glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(m));
}
void ShaderProgram::setUniform(int location, float val)
{
	glUniform1f(location, val);
}
void ShaderProgram::setUniform(int location, int val)
{
	glUniform1i(location, val);
}
void ShaderProgram::setUniform(int location, bool val)
{
	glUniform1i(location, val);
}

void ShaderProgram::attachUniformBlock(string name, int index) const
{
	GLuint blockLocation = glGetUniformBlockIndex(handle, name.c_str());
	glUniformBlockBinding(handle, index, blockLocation);
}

map<string, GLuint> ShaderProgram::shaderMap;