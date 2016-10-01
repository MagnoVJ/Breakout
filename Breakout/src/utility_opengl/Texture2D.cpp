#include "Texture2D.h"

namespace utility_opengl{

	Texture2D::Texture2D() :
		width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR){

			glGenTextures(1, &id);

	}

	void Texture2D::generate(GLuint width, GLuint height, unsigned char* data){
		this->width = width;
		this->height = height;
		//Create Texture
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
		//Set Texture wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
		//Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::bind() const {
		glBindTexture(GL_TEXTURE_2D, id);
	}

}