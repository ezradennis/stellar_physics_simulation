#include"EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	// bind the VBO and specify that it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// bring the vertices into the VBO as raw data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}