#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

class BallObject : public GameObject{

public:
	GLfloat radius;
	GLboolean stuck;
	GLboolean sticky, passThrough;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(GLfloat dt, GLuint windowWidth, GLuint windowHeight);
	void      reset(glm::vec2 position, glm::vec2 velocity);

};