#pragma once

#include "PowerUp.h"
#include "PGCollideEffect.h"
#include "PostProcessor.h"
#include "ParticleGenerator.h"
#include "BallObject.h"
#include "GameLevel.h"
#include "SpriteRenderer.h"
#include "utility/ResourceManager.h"
#include "utility/TextRenderer.h"

#include <irrKlang.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <list>
#include <tuple>

using utility_opengl::ResourceManager;
using utility_opengl::TextRenderer;

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

typedef std::tuple<GLboolean, //Ouve colisao
	               Direction, //Direcao 
				   glm::vec2, //Diferenca do closest point com o centro da bola
				   glm::vec2, //Closest point (para o efeito de colisão)
				   glm::vec2  //Centro do aabb
                            > Collision;

class Game{

private:
	GLboolean checkCollision(const GameObject& one, const GameObject& two); // AABB - AABB collision
	Collision checkCollision(const BallObject& one, const GameObject& two); // Circle - AABB collision
	Direction vectorDirection(glm::vec2 target);
	void doCollisions();

	void resetLevel();
	void resetPlayer();

	//PowerUps
	void spawnPowerUps(GameObject& block);
	void updatePowerUps(GLfloat dt);
	GLboolean shouldSpawn(GLuint chance);

	void activatePowerUp(PowerUp& powerUp);
	GLboolean isOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type);

public:
	//Game state
	GameState state;
	GLboolean keys[1024];
	GLboolean keysProcessed[1024];
	GLuint width, height;
	GLuint lives;

	//Levels
	std::vector<GameLevel> levels;
	GLuint                 level;

	//PowerUps
	std::vector<PowerUp> powerUps;

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