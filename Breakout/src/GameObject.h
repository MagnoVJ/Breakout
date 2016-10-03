#pragma once

#include "utility/Texture2D.h"
#include "SpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

using utility_opengl::Texture2D;

class GameObject{
	
public:
	//Constructor(s)
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));

	//Object state
	glm::vec2  position, size, velocity;
	glm::vec3  color; 
	GLfloat    rotation;
	GLboolean  isSolid;
	GLboolean  destroyed;
	
	//Render state
	Texture2D  sprite;

	//Draw sprite
	virtual void draw(SpriteRenderer& renderer);

};