#include "VAO.h"
#include "../Core/Constants.h"
using core::Constants;


VAO::VAO()
{
	// Create the vertex array object
	//glGenVertexArrays(1, &vao);
}


VAO::~VAO()
{
	if (vbos.size())
	{
		glDeleteBuffers(vbos.size(), &vbos[0]);
	}
	glDeleteBuffers(1, &indexBuffer);
}

void VAO::attachBuffer(GLuint location, const vector<float>& data, BufferType type, Usage usage)
{
	if (location < 0)
	{
		LOG_ERR("Location ", location, " is invalid in attachBuffer()");
		return;
	}

	// Create the buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Populate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], usage == Usage::Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

	// Enable the vertex attribute arrays
	glBindVertexArray(vao);
	glEnableVertexAttribArray(location);

	// Map correct index to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(location, type == BufferType::Texcoord ? 2 : 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	vbos.emplace_back(vbo); // Store for deletion
}
void VAO::attachIndices(const vector<unsigned short>& data)
{
	glBindVertexArray(vao);

	// Create a new buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned short), &data[0], GL_STATIC_DRAW);
	numIndices = data.size();
}

// Based loosely off http://gamedev.stackexchange.com/questions/8042/vertex-array-object-opengl
void VAO::attachBuffers(const vector<Vertex>& vertices, const vector<unsigned int>& indices)
{
	// Create the vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and bind vertex data buffer
	GLuint vbuffer;
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

	// Copy data into the buffer object
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Set up vertex attributes
	glEnableVertexAttribArray(Constants::get().getNum<int>("vertex_location"));
	glVertexAttribPointer(Constants::get().getNum<int>("vertex_location"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(Constants::get().getNum<int>("texcoord_location"));
	glVertexAttribPointer(Constants::get().getNum<int>("texcoord_location"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(Constants::get().getNum<int>("normal_location"));
	glVertexAttribPointer(Constants::get().getNum<int>("normal_location"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(Constants::get().getNum<int>("tangent_location"));
	glVertexAttribPointer(Constants::get().getNum<int>("tangent_location"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// Create and bind index buffer
	GLuint ibuffer;
	glGenBuffers(1, &ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);

	// Copy data into the buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Record the number of indices for rendering
	numIndices = indices.size();

	// Unbind the VAO
	glBindVertexArray(0);
}


void VAO::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
}