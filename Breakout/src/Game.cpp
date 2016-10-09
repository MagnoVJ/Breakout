#include "Game.h"

/*** Game Globals ***/
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY = 500.0f; 

const GLfloat BALL_RADIUS = 12.5f;
const glm::vec2 BALL_INITIAL_VELOCITY(200.0f, -450.0f);

GameObject* player;
SpriteRenderer* renderer, *rendererPGCollideEffect;
BallObject* ball;
ParticleGenerator* particlesGen;
PostProcessor* postProcessor;

GLfloat shakeTime;

std::list<PGCollideEffect> pgCollideList;

Game::Game(GLuint width, GLuint height)	: state(GAME_ACTIVE), keys(), width(width), height(height){

	//Inicializações
	shakeTime = 0.0f;

}

Game::~Game(){

	delete player;
	delete renderer;
	delete rendererPGCollideEffect;
	delete ball;
	delete particlesGen;
	delete postProcessor;

}

void Game::init(){

	//Load shaders
	ResourceManager::loadShader("shaders/shader1.vert", "shaders/shader1.frag", nullptr, "shader1");
	ResourceManager::loadShader("shaders/shader2BallParticles.vert", "shaders/shader2BallParticles.frag", nullptr, "particleShader");
	ResourceManager::loadShader("shaders/postProcessing.vert", "shaders/postProcessing.frag", nullptr, "postprocessing");
	ResourceManager::loadShader("shaders/pgCollideEffectShader.vert", "shaders/pgCollideEffectShader.frag", nullptr, "pgCollideEffectShader");

	//Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
	
	ResourceManager::getShader("shader1").use().setInteger("image", 0);
	ResourceManager::getShader("shader1").setMatrix4("projection", projection);
	ResourceManager::getShader("particleShader").use().setInteger("sprite", 0);
	ResourceManager::getShader("particleShader").setMatrix4("projection", projection);
	ResourceManager::getShader("pgCollideEffectShader").use().setInteger("image", 0);
	ResourceManager::getShader("pgCollideEffectShader").setMatrix4("projection", projection);

	renderer                = new SpriteRenderer(ResourceManager::getShader("shader1"));
	rendererPGCollideEffect = new SpriteRenderer(ResourceManager::getShader("pgCollideEffectShader"));

	//Load Textures
	ResourceManager::loadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::loadTexture("textures/ball.png", GL_TRUE, "ball");
	ResourceManager::loadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::loadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::loadTexture("textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::loadTexture("textures/particle.png", GL_TRUE, "particle");

	//PGCollideEffectShader
	ResourceManager::loadTexture("textures/blueBeam.png", GL_TRUE, "beamTexture");
	ResourceManager::loadTexture("textures/blueFleshlight.png", GL_TRUE, "flareTexture");
	ResourceManager::loadTexture("textures/greenBeam.png", GL_TRUE, "greenBeam");
	ResourceManager::loadTexture("textures/greenFleshlight.png", GL_TRUE, "greenFleshlight");
	ResourceManager::loadTexture("textures/yellowBeam.png", GL_TRUE, "yellowBeam");
	ResourceManager::loadTexture("textures/yellowFleshlight.png", GL_TRUE, "yellowFleshlight");
	ResourceManager::loadTexture("textures/orangeBeam.png", GL_TRUE, "orangeBeam");
	ResourceManager::loadTexture("textures/orangeFleshlight.png", GL_TRUE, "orangeFleshlight");

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

	//Player
	glm::vec2 playerPos = glm::vec2(width / 2 - PLAYER_SIZE.x / 2, height - PLAYER_SIZE.y);
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

	//Ball
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	ball = new BallObject(ballPos, BALL_RADIUS, BALL_INITIAL_VELOCITY, ResourceManager::getTexture("ball"));

	//Particle generator
	particlesGen = new ParticleGenerator(ResourceManager::getShader("particleShader"), ResourceManager::getTexture("particle"), 500);

	//Post processing effect
	postProcessor = new PostProcessor(ResourceManager::getShader("postprocessing"), width, height);

}

void Game::update(GLfloat dt){

	if(state == GAME_ACTIVE){

		//Move ball
		ball->move(dt, width, height);

		//Check for collisions
		doCollisions();

		//Check loss condition
		if(ball->position.y >= height){//Dit ball reach bottom edge?
			resetLevel();
			resetPlayer();
		}

		//Update particles
		particlesGen->update(dt, *ball, 2, glm::vec2(ball->radius / 2));

		//Update PGCollideEffect
		for(std::list<PGCollideEffect>::iterator it = pgCollideList.begin(); it != pgCollideList.end();){
			PGCollideEffect& pgRef = *it;
			if(glfwGetTime() - pgRef.birth > 0.3)
				pgCollideList.erase(it++);
			else
				++it;
		}
		
		for(std::list<PGCollideEffect>::iterator& it = pgCollideList.begin(); it != pgCollideList.end(); it++){
			PGCollideEffect& pgRef = *it;
			pgRef.update(dt);
		}
		
		//Reduce shake time
		if(shakeTime > 0.0f){
			shakeTime -= dt;
			if(shakeTime <= 0.0f)
				postProcessor->shake = GL_FALSE;

		}

	}

}

void Game::render(){

	if(state == GAME_ACTIVE){

		//Begin rendering to prostprocessing quad
		postProcessor->beginRender();
		{
			//Draw background
			renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0, 0), glm::vec2(width, height), 0.0f);

			//Draw level
			levels[level].draw(*renderer);

			//Draw paddle
			player->draw(*renderer);

			//Draw particles
			if(!ball->stuck)
				particlesGen->draw();

			//Draw PGCollideEffect
			for(std::list<PGCollideEffect>::iterator& it = pgCollideList.begin(); it != pgCollideList.end(); it++){
				PGCollideEffect& pgRef = *it;
				pgRef.draw();
			}

			//Draw ball
			ball->draw(*renderer);
		}
		postProcessor->endRender();

		//Render postprocessing quad
		postProcessor->render(glfwGetTime());

	}

}

GLboolean Game::checkCollision(const GameObject& one, const GameObject& two){

	//Collision x-axis
	bool collisionX = one.position.x + one.size.x >= two.position.x &&
		              two.position.x + two.size.x >= one.position.x;

	//Collision y-axis
	bool collisionY = one.position.y + one.size.y >= two.position.y &&
		              two.position.y + two.size.y >= one.position.y;

	return collisionX && collisionY;

}

Collision Game::checkCollision(const BallObject& one, const GameObject& two){
	//Get center point circle 
	glm::vec2 center(one.position + one.radius);
	//Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
	glm::vec2 aabb_center(two.position.x + aabb_half_extents.x,
		                  two.position.y + aabb_half_extents.y);
	//Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;

	if(glm::length(difference) <= one.radius)
		return std::make_tuple(GL_TRUE, vectorDirection(difference), difference, closest, aabb_center);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0), closest, aabb_center);

}

Direction Game::vectorDirection(glm::vec2 target){

	glm::vec2 compass[] = {
		glm::vec2( 0.0f, 1.0f), //Up
		glm::vec2( 1.0f, 0.0f), //Right
		glm::vec2( 0.0f,-1.0f), //Down
		glm::vec2(-1.0f, 0.0f)  //Left
	};

	GLfloat max = 0.0f;
	GLuint best_match = -1;

	FOR(GLuint, i, 4){
		
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);

		if(dot_product > max){
			max = dot_product;
			best_match = i;
		}
	}

	return (Direction)best_match;

}

void Game::doCollisions(){

	for(GameObject& box : levels[level].bricks){

		if(!box.destroyed){

			Collision collision = checkCollision(*ball, box);

			if(std::get<0>(collision)){ //If collision is true

				//Destroy block if not solid
				if(!box.isSolid)
					box.destroyed = GL_TRUE;
				else{
					shakeTime = 0.05f;
					postProcessor->shake = GL_TRUE;
				}
				//Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if(dir == LEFT || dir == RIGHT){ //Horizontal collision
					ball->velocity.x = -ball->velocity.x; //Reverse horizontal
					//Relocate
					GLfloat penetration = ball->radius - std::abs(diff_vector.x);
					if(dir == LEFT)
						ball->position.x += penetration; //Move ball to right
					else
						ball->position.x -= penetration; //Move ball to left
				}
				else{ //Vertical collision
					ball->velocity.y = -ball->velocity.y; //Reverse vertical
					//Relocate
					GLfloat penetration = ball->radius - std::abs(diff_vector.y);
					if(dir == UP)
						ball->position.y -= penetration; //Move ball back up
					else
						ball->position.y += penetration; //Move ball back down

				}

				if(!box.isSolid){

					Texture2D* textureBeam;
					Texture2D* textureFleshLight;

					if(box.color.x == 0.2f && box.color.y == 0.6f && box.color.z == 1.0f){ //Blue
						textureBeam = &ResourceManager::getTexture("beamTexture");
						textureFleshLight = &ResourceManager::getTexture("flareTexture");
					}
					else if(box.color.x == 0.0f && box.color.y == 0.7f && box.color.z == 0.0f){ //green
						textureBeam = &ResourceManager::getTexture("greenBeam");
						textureFleshLight = &ResourceManager::getTexture("greenFleshlight");
					}
					else if(box.color.x == 0.8f && box.color.y == 0.8f && box.color.z == 0.4f){ //yellow
						textureBeam = &ResourceManager::getTexture("yellowBeam");
						textureFleshLight = &ResourceManager::getTexture("yellowFleshlight");
					}
					else{
						textureBeam = &ResourceManager::getTexture("orangeBeam");
						textureFleshLight = &ResourceManager::getTexture("orangeFleshlight");
					}

					PGCollideEffect pgCollEffect(ResourceManager::getShader("pgCollideEffectShader"), 
												 *textureBeam,
												 *textureFleshLight,
												 *rendererPGCollideEffect,
												 std::get<4>(collision),
												 std::get<3>(collision), //closest point
												 30,                     //Quantidade de beams
												 20,                     //Raio do spawn       
												 20,                     //Size beam
												 50,                     //Size flare
												 glm::vec3(1.0f),        //Color
												 glfwGetTime());         //Birth   

					pgCollideList.push_front(pgCollEffect);

				}

			}

		}

	}

	//Player collision
	Collision result = checkCollision(*ball, *player);

	if(!ball->stuck && std::get<0>(result)){
		//Chick where it hit the board, and change velocity based on where it hit the board
		GLfloat centerPaddle = player->position.x + player->size.x / 2;
		GLfloat distance = abs((ball->position.x + ball->radius) - centerPaddle);
		GLfloat percentage = distance / (player->size.x / 2);
		//Then move accordingly
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = ball->velocity;
		ball->velocity.x = BALL_INITIAL_VELOCITY.x * percentage * strength;
		ball->velocity.x *= (oldVelocity.x < 0)?-1:1; //matem a direção da bola ao acertar o paddle
		ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
		//With sticky paddle correction
		ball->velocity.y = -1 * abs(ball->velocity.y); 
	}

}

void Game::resetLevel(){
	
	if(level == 0)
		levels[0].load("levels/1_Standard.lvl", width, height * 0.5);
	else if(level == 1)
	    levels[1].load("levels/2_A_few_small_gaps.lvl", width, height * 0.5);
	else if(level == 2)
	    levels[2].load("levels/3_Space_invader.lvl", width, height * 0.5);
	else if(level == 3)
	    levels[3].load("levels/4_Bounce_galore.lvl", width, height * 0.5);

}

void Game::resetPlayer(){
	//Reset player/ball stats
	player->size = PLAYER_SIZE;
	player->position =  glm::vec2(width / 2 - PLAYER_SIZE.x / 2, height - PLAYER_SIZE.y);

	ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), BALL_INITIAL_VELOCITY);

}

void Game::processInput(GLfloat dt){

	if(state == GAME_ACTIVE){

		GLfloat velocity = PLAYER_VELOCITY * dt;

		if(keys[GLFW_KEY_LEFT]){

			//Quando a tecla LEFT é pressioanada...

			if(player->position.x >= 0){

				player->position.x -= velocity;

				if(ball->stuck)
					ball->position.x -= velocity;

			}

		}
		
		if(keys[GLFW_KEY_RIGHT]){

			//Quando a tecla RIGHT é pressionada...

			if(player->position.x <= width - player->size.x){

				player->position.x += velocity;

				if(ball->stuck)
					ball->position.x += velocity;

			}

		}

		if(keys[GLFW_KEY_SPACE]){

			//Quando a tecla SPACE é pressionada...

			ball->stuck = false;


		}

	}

}