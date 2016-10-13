#include "PGCollideEffect.h"

PGCollideEffect::PGCollideEffect(Shader shader, Texture2D textureBeam, Texture2D textureFlare, SpriteRenderer& renderer, glm::vec2 posBeamCenter, glm::vec2 posFlareCenter, GLuint quantBeams, GLfloat radius, GLfloat sizeBeam, GLfloat sizeFlare, glm::vec3 color, GLfloat birth) 
	: shader(shader), textureBeam(textureBeam), textureFlare(textureFlare), renderer(renderer), posBeamCenter(posBeamCenter), posFlareCenter(posFlareCenter), quantBeams(quantBeams), radius(radius), sizeBeam(sizeBeam), sizeFlare(sizeFlare), color(color), birth(birth), 
	  flare(textureFlare){
	
	for(GLuint i = 0; i < this->quantBeams; i++){

		//Position
		GLfloat maxX = this->posBeamCenter.x + this->radius;
		GLfloat minX = this->posBeamCenter.x - this->radius;
		GLfloat maxY = this->posBeamCenter.y + this->radius;
		GLfloat minY = this->posBeamCenter.y - this->radius;
			  
		GLfloat posBeamX = rand() % static_cast<int>(maxX - minX) + minX;
		GLfloat posBeamY = rand() % static_cast<int>(maxY - minY) + minY;

		glm::vec2 centerBeam = glm::vec2(posBeamX, posBeamY);
		glm::vec2 distance = centerBeam - this->posBeamCenter;
		distance = glm::normalize(distance);

		//Faz com que todos os beams tenha a mesma distancia em relacao ao centro do quadrado
		GLfloat posBeamSameDirX = this->posBeamCenter.x + distance.x * this->radius;
		GLfloat posBeamSameDirY = this->posBeamCenter.y + distance.y * this->radius;
			  
		//Rotation
		distance.y *= -1; 
			  
		GLfloat coss = glm::dot(distance, glm::vec2(0.0f, 1.0f));
		GLfloat angle = acos(coss) * 180 / PI;
			  
		//Velocity
		distance.y *= -1;
		GLuint minSpeed = 170, maxSpeed = 171;
		glm::vec2 velocity = distance * static_cast<float>(rand() % (maxSpeed - minSpeed) + minSpeed);
			  
		PGBeam beam(this->textureBeam, glm::vec2(posBeamSameDirX - this->sizeBeam / 2, posBeamSameDirY - this->sizeBeam / 2), glm::vec2(this->sizeBeam), velocity, distance.x>0?angle:-angle);
			  
		beams.push_back(beam);

	}

	flare.position = glm::vec2(this->posFlareCenter.x - this->sizeFlare / 2, this->posFlareCenter.y - this->sizeFlare / 2);
	flare.size = glm::vec2(this->sizeFlare, this->sizeFlare);

}

void PGCollideEffect::update(GLfloat dt){

    for(GLuint i = 0; i < beams.size(); i++)
		beams[i].position += beams[i].velocity * dt;

}

void PGCollideEffect::draw(){

    for(GLuint i = 0; i < beams.size(); i++)
		beams[i].draw(renderer);

	if(glfwGetTime() - birth < 0.05)
		flare.draw(renderer);

}