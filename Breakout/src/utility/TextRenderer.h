#pragma once

#include "ResourceManager.h"

#include "Texture2D.h"
#include "Shader.h"
#include "Utility.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <map>

namespace utility_opengl{

	//Holds all state information relevant to character as loaded using 
	//FreeType
	struct Character{
		GLuint textureID;      //ID that handle glyph texture
		glm::ivec2 size;       //Size of glyph
		glm::ivec2 bearing;    //Offset from baseline to left/top of glyph
		GLuint advance;        //Horizontal offset to advance to next glyph
	};

	//A renderer class for rendering text displayed by a font loaded using the
	//FreeType library. A single font is loaded, processed into a list of Character
	//items for later rendering
	class TextRenderer{

	private:
		GLuint VAO, VBO;

	public:
		//Holds a list of pre-compiled characters
		std::map<GLchar, Character> characters;
		//Shader used for text rendering
		Shader textShader;
		//Constructor
		TextRenderer(GLuint width, GLuint height);
		//Pre-compiles a list of characters from the given font
		void load(std::string font, GLuint fontSize);
		//Renders a string of text using the precompiled list of characters
		void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

	};

}
