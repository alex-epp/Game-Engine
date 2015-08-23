#pragma once
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "Vertex.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;

enum class BufferType { Vertex, Index, Colour, Normal, Texcoord };
enum class Usage { Static, Dynamic };

class VAO
{
public:
	VAO();
	~VAO();

	[[deprecated]] void attachBuffer(GLuint location, const vector<float>& data, BufferType type, Usage usage = Usage::Static); // There must be an 'in' variable at 'location'
	[[deprecated]] void attachIndices(const vector<unsigned short>& data);
	
	void attachBuffers(const vector<Vertex>& vertices, const vector<unsigned int>& indices);
	
	void draw();
	GLuint getHandle() { return vao; } // Name is for consistency
	// TODO - Add functions to edit buffers

private:
	GLuint vao; // The vertex array object
	vector<GLuint> vbos;
	GLuint indexBuffer; // The index buffer object. Needed for rendering
	unsigned int numIndices;
};