#pragma once

#include "PGBeam.h"
#include "PGFlare.h"

#include "utility/Utility.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <time.h>
#include <vector>

class PGCollideEffect{

public:
	glm::vec2 posBeamCenter;
	glm::vec2 posFlareCenter;
	GLfloat radius;
	GLuint quantBeams;

	GLfloat sizeBeam;
	GLfloat sizeFlare;

	glm::vec3 color;

	GLfloat birth;

	std::vector<PGBeam> beams;
	PGFlare flare;

	Shader shader;
	Texture2D textureBeam;
	Texture2D textureFlare;

	SpriteRenderer& renderer;

	PGCollideEffect(Shader			shader, 
					Texture2D		textureBeam, 
					Texture2D		textureFlare,
					SpriteRenderer& renderer,
					glm::vec2       posBeamCenter,
					glm::vec2		posFlareCenter,
					GLuint			quantBeams, 
					GLfloat			radius, 
					GLfloat			sizeBeam,
					GLfloat			sizeFlare,
					glm::vec3		color,
					GLfloat			birth = 0.0f);

	void update(GLfloat dt);
	void draw();

};