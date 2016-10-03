#pragma once

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "utility/ResourceManager.h"
#include "utility/Utility.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using utility_opengl::ResourceManager;

class GameLevel{

private:
	//Initialize level from tile data
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);

public:
	std::vector<GameObject> bricks;

	GameLevel(){}

	//Load level from file
	void load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);

	//Render level
	void draw(SpriteRenderer& renderer);

	//Check if the level is completed (all non-solid tiles are destroyed)
	GLboolean isCompleted();


};