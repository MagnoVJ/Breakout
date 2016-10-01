#define GLEW_STATIC

#include "src/Game.h"
#include "src/utility_opengl/ResourceManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GAMELOOP(x) while(!glfwWindowShouldClose(x))
#define FOR(T, x, end) for(T x = 0; x < end; x++)

using utility_opengl::ResourceManager;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//Global variables
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

GLfloat deltaTime;
GLfloat lastFrame;

Game* breakout;

int main(int argc, char* argv[]){

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Breakout", nullptr, nullptr);
	
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	//OpenGL configuration
	glewExperimental = GL_TRUE;
	
	glewInit();
	glGetError(); //catch glewInit() bug

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Inicializações
	deltaTime = 0.0f;
	lastFrame  = 0.0f;
	breakout = new Game(WIDTH, HEIGHT);

	breakout->init();

	breakout->state = GAME_ACTIVE;

	/**** GAMELOOP ****/
	GAMELOOP(window){

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		/*** Input ***/
		breakout->processInput(deltaTime);

		/*** Update ***/
		breakout->update(deltaTime);

		/*** Render ***/
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		breakout->render();

		glfwSwapBuffers(window);

	}

	ResourceManager::clear();

	delete breakout;
	glfwTerminate();

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(key >= 0 && key < 1024){
		if(action == GLFW_PRESS)
			breakout->keys[key] = GL_TRUE;
		else if(action == GLFW_RELEASE)
			breakout->keys[key] = GL_FALSE;
	}

}