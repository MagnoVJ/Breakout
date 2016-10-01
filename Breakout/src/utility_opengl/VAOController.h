#pragma once

#include <GL/glew.h>

#include <iostream>

namespace utility_opengl{

	class VAOController{

	private:
		GLuint VAO, VBO;

	public:
		VAOController(const GLfloat* vertices, GLsizeiptr sizei);
		~VAOController();

		void setAttribute(GLuint index, GLint size, GLsizei stride, int offset);

		void bind();
		void unbind();

		void drawArrays(GLenum mode, GLint first, GLsizei count);


	};

}