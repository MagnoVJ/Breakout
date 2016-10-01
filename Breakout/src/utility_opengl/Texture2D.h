#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

namespace utility_opengl{

	class Texture2D{

	public:
		GLuint id;
		GLuint width, height;
		GLuint internal_format;
		GLuint image_format;

		GLuint wrap_s;
		GLuint wrap_t;
		GLuint filter_min;
		GLuint filter_max;

		Texture2D();

		void generate(GLuint width, GLuint height, unsigned char* data);

		void bind() const;

	};

}