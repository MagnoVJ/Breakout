#pragma once

#include "GameObject.h"
#include "utility/Utility.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include <string>

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject{

public_attributes:
	//PowerUp State
	std::string type;
	GLfloat     duration;
	GLboolean   activated;

public_methods:
	//Constructor
	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture);

};