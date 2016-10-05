#pragma once

#include "utility/Shader.h"
#include "utility/Texture2D.h"
#include "GameObject.h"
#include "utility/Utility.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <vector>

using utility_opengl::Shader;
using utility_opengl::Texture2D;

//Represents a single particle and its state
struct Particle{

	glm::vec2 position, velocity;
	glm::vec4 color;
	GLfloat life;

	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }

};

//ParticleGenerator acts as a container for rendering a large number of
//particles by repeatedly spawning and updating particles and killing
//them after a given amount of time.
class ParticleGenerator{

private_attributes:
	//State
	std::vector<Particle> particles;
	GLuint amount;
	//Render state
	Shader shader;
	Texture2D texture;
	GLuint VAO;

private_methods:
	//Initializes buffer and vertex attributes
	void init();
	//Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	//Respawns particle
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

public_methods:
	//Constructor
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	//Update all particles
	void update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	//Render all particles
	void draw();

};