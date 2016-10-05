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

GLuint ParticleGenerator::firstUnusedParticle(){


}