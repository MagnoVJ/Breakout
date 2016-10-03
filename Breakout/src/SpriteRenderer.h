#pragma once

#include "utility/Shader.h"
#include "utility/Texture2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

using utility_opengl::Shader;
using utility_opengl::Texture2D;

class SpriteRenderer{

private:
	Shader shader;
	GLuint quadVAO;

	void initRenderData();

public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void drawSprite(Texture2D& texture, glm::vec2 position, 
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0, glm::vec3 color = glm::vec3(1.0));


};