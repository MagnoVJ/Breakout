#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

enum GameState{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game{

public:
	//Game state
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;
	//Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	//Initialize game state (load all shaders/textures/levels)
	void init();
	//Game loop
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();

};