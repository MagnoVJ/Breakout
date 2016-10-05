#include "BallObject.h"

BallObject::BallObject() : GameObject(), radius(12.5f), stuck(true) { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite) 
	: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), radius(radius), stuck(true){ }

glm::vec2 BallObject::move(GLfloat dt, GLuint windowWidth, GLuint windowHeight){

	//If not stuck to player board
	if(!stuck){

		position += velocity * dt;

		//Check if outside window bounds; if so, reverse velocity and restore at correct position
		if(position.x <= 0.0f){
			velocity.x = -velocity.x;
			position.x = 0.0f;
		}
		else if(position.x + size.x >= windowWidth){
			velocity.x = -velocity.x;
			position.x = windowWidth - size.x;
		}

		if(position.y <= 0.0f){
			velocity.y = -velocity.y;
			position.y = 0.0f;
		}
		//Bola continua "bouoncing" ao alcancar o fundo da tela (para ser comentado depois)
		/*else if(position.y + size.y >= windowHeight){
			velocity.y = -velocity.y;
			position.y = windowHeight - size.y;
		}*/

	}

	return position;

}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::reset(glm::vec2 position, glm::vec2 velocity){
	this->position = position;
	this->velocity = velocity;
	this->stuck = true;
}