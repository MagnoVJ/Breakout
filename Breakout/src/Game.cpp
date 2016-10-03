#include "Game.h"

/*** Game Globals ***/
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f); 

GameObject* player;
SpriteRenderer* renderer;

Game::Game(GLuint width, GLuint height)	: state(GAME_ACTIVE), keys(), width(width), height(height){


}

Game::~Game(){
	delete player;
	delete renderer;
}

void Game::init(){

	//Load shaders
	ResourceManager::loadShader("shaders/shader1.vert", "shaders/shader1.frag", nullptr, "shader1");

	//Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("shader1").use().setInteger("image", 0);
	ResourceManager::getShader("shader1").use().setMatrix4("projection", projection);
	
	renderer = new SpriteRenderer(ResourceManager::getShader("shader1"));

	//Load Textures
	ResourceManager::loadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::loadTexture("textures/ball.png", GL_TRUE, "ball");
	ResourceManager::loadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::loadTexture("textures/block_solid.png", GL_FALSE, "block_solid");

	ResourceManager::loadTexture("textures/paddle.png", GL_TRUE, "paddle");

	//Load Levels
	GameLevel one; one.load("levels/1_Standard.lvl", width, height * 0.5);
	GameLevel two; two.load("levels/2_A_few_small_gaps.lvl", width, height * 0.5);
	GameLevel three; three.load("levels/3_Space_invader.lvl", width, height * 0.5);
	GameLevel four; four.load("levels/4_Bounce_galore.lvl", width, height * 0.5);

	levels.push_back(one);
	levels.push_back(two);
	levels.push_back(three);
	levels.push_back(four);

	level = 0;

	glm::vec2 playerPos = glm::vec2(width / 2 - PLAYER_SIZE.x / 2, height - PLAYER_SIZE.y);

	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

}

void Game::processInput(GLfloat dt){

	if(state == GAME_ACTIVE){

		GLfloat velocity = PLAYER_VELOCITY * dt;

		if(keys[GLFW_KEY_A]){

		//Quando a tecla A é pressioanada...

			if(player->position.x >= 0)
				player->position.x -= velocity;


		}
		
		if(keys[GLFW_KEY_D]){

		//Quando a tecla D é pressionada...

			if(player->position.x <= width - player->size.x)
				player->position.x += velocity;


		}



	}

}

void Game::update(GLfloat dt){

	

}

void Game::render(){

	if(state == GAME_ACTIVE){

		//Draw background
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0, 0), glm::vec2(width, height), 0.0f);

		//Draw level
		levels[level].draw(*renderer);

		//Draw paddle
		player->draw(*renderer);

	}

}