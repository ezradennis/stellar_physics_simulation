#include"VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	// bind the VBO and specify that it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// bring the vertices into the VBO as raw data
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}