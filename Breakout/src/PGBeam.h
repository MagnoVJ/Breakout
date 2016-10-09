#pragma once 

#include "SpriteRenderer.h"
#include "utility/Texture2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

class PGBeam{

public:
	Texture2D sprite;

	glm::vec2 position, velocity, size;
	glm::vec3 color; 

	GLfloat rotation;

	PGBeam(Texture2D& sprite);
	PGBeam(Texture2D& sprite, glm::vec2 position, glm::vec2 size, glm::vec2 velocity = glm::vec2(0.0f), GLfloat rotation = 0.0f);

	void draw(SpriteRenderer& renderer);

};