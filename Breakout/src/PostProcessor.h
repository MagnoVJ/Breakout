#pragma once

#include "utility/Texture2D.h"
#include "utility/Shader.h"
#include "SpriteRenderer.h"
#include "utility/Utility.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

class PostProcessor{

private_attributes:
	GLuint MSFBO, FBO; //MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
	GLuint RBO; //RBO is used for multisampled color buffer
	GLuint VAO;

private_methods:
	void initRenderData(); //Initialize quad for rendering postprocessing texture

public_attributes:
	Shader posProcShader;
	Texture2D texture;
	GLuint width, height;
	//Options
	GLboolean confuse, chaos, shake;

public_methods:
	PostProcessor(Shader shader, GLuint width, GLuint height);	
	void beginRender(); //Prepares the postprocessor's framebuffer operations before rendering the game
	void endRender(); //Should be called after rendering the game, so it stores all the rendered data into a texture object
	void render(GLfloat time); //Renders the PostProcessor texture quad (as a screen-encompassing large sprite)

};