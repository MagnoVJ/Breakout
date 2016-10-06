#include "ParticleGenerator.h"

//Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount) : shader(shader), texture(texture), amount(amount){

	init();
	 
	FOR(GLuint, i, amount)
		particles.push_back(Particle());

}

void ParticleGenerator::init(){

	//Set up mesh and attribute properties

	GLfloat particleVertices[] = {
		0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  0.0f, 0.0f,

        0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 1.0f,  1.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 0.0f
	};

	GLuint VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

}

void ParticleGenerator::update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset){

	//Add new particles
	FOR(GLuint, i, newParticles){
		int unusedParticle = firstUnusedParticle();
		respawnParticle(particles[unusedParticle], object, offset);
	}

	//Update all particles
	FOR(GLuint, i, amount){
		Particle& p = particles[i];
		p.life -= dt; //reduce life
		if(p.life > 0.0f){ //particle is alive then update
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5;
		}

	}

}

void ParticleGenerator::draw(){

	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader.use();
	for(Particle particle : particles){

		if(particle.life > 0.0f){
			shader.setVector2f("offset", particle.position);
			shader.setVector4f("color", particle.color);
			texture.bind();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

	}
	//Reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

GLuint ParticleGenerator::firstUnusedParticle(){

	//First search from last used particle, this will usually return almost instantly
	for(GLuint i = lastUsedParticle; i < amount; i++)
		if(particles[i].life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}

	//Otherwise, do a linear search
	FOR(GLuint, i, lastUsedParticle)
		if(particles[i].life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}

	//All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;

}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset){
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.position = object.position + random + offset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 1.0f;
	particle.velocity = object.velocity * 0.1f;
}