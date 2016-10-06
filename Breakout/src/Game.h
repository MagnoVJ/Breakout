#pragma once

#include "ParticleGenerator.h"
#include "BallObject.h"
#include "GameLevel.h"
#include "SpriteRenderer.h"
#include "utility/ResourceManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <tuple>

using utility_opengl::ResourceManager;

enum GameState{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class Game{

private:
	GLboolean checkCollision(const GameObject& one, const GameObject& two); // AABB - AABB collision
	Collision checkCollision(const BallObject& one, const GameObject& two); // Circle - AABB collision
	Direction vectorDirection(glm::vec2 target);
	void doCollisions();

	void resetLevel();
	void resetPlayer();

public:
	//Game state
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;

	//Levels
	std::vector<GameLevel> levels;
	GLuint                 level;

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