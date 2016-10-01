#include "VAOController.h"

namespace utility_opengl{

	VAOController::VAOController(const GLfloat* vertices, GLsizeiptr sizei){

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO); 

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizei, vertices, GL_STATIC_DRAW);

	}

	VAOController::~VAOController(){
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	//index = index do atributo, por exemplo, position = 0
	//size = tamanho do atributo no vertex
	//stride = tamanho do vertex
	//offset  = onde começa o atributo no vertice
	void VAOController::setAttribute(GLuint index, GLint size, GLsizei stride, int offset){
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	}

	void VAOController::bind(){
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
	}

	void VAOController::unbind(){
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void VAOController::drawArrays(GLenum mode, GLint first, GLsizei count){
		glBindVertexArray(VAO);
		glDrawArrays(mode, first, count);
		glBindVertexArray(0);
	}

}