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
irrklang::ISoundEngine* soundEngine;
TextRenderer* text;

GLfloat shakeTime;

std::list<PGCollideEffect> pgCollideList;

Game::Game(GLuint width, GLuint height)	: state(GAME_ACTIVE), keys(), width(width), height(height), lives(3){

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
	soundEngine->drop();
	delete text;

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

	//Load PowerUps
	ResourceManager::loadTexture("textures/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::loadTexture("textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
	ResourceManager::loadTexture("textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
	ResourceManager::loadTexture("textures/powerup_increase.png", GL_TRUE, "powerup_increase");
	ResourceManager::loadTexture("textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::loadTexture("textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");

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

	FOR(GLuint, i, 1024)
		keysProcessed[i] = GL_FALSE;

	//Sound Engine
	soundEngine = irrklang::createIrrKlangDevice();
	std::cout << std::endl;
	soundEngine->play2D("audio/breakout.mp3", true);

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

	//Text
	text = new TextRenderer(width, height);
	text->load("fonts/ocraext.ttf", 24);

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

	if(state == GAME_MENU){

		if(keys[GLFW_KEY_W] && !keysProcessed[GLFW_KEY_W]){
			level = (level + 1) % 4;
			keysProcessed[GLFW_KEY_W] = GL_TRUE;
		}
		if(keys[GLFW_KEY_S] && !keysProcessed[GLFW_KEY_S]){
			
			if(level > 0)
				--level;
			else
				level = 3;
			keysProcessed[GLFW_KEY_S] = GL_TRUE;
		}
		if(keys[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER]){
			state = GAME_ACTIVE;
			keys[GLFW_KEY_ENTER] = GL_TRUE;
		}

	}

	if(state == GAME_WIN){

		if(keys[GLFW_KEY_ENTER]){
			keysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			postProcessor->chaos = GL_FALSE;
			state = GAME_MENU;
		}

	}

}

void Game::update(GLfloat dt){

	if(state == GAME_ACTIVE){

		//Move ball
		ball->move(dt, width, height);

		//Check for collisions
		doCollisions();

		//Check loss condition
		if(ball->position.y >= height){//Dit ball reach bottom edge?
			
			--lives;

			//Player lose all his lives?
			if(lives == 0){
				resetLevel();
				state = GAME_MENU;
			}
			
			resetPlayer();

		}

		//Update particles
		particlesGen->update(dt, *ball, 2, glm::vec2(ball->radius / 2));

		//Update PowerUps
		updatePowerUps(dt);

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

	if(levels[level].isCompleted()){
		resetLevel();
		resetPlayer();
		postProcessor->chaos = GL_TRUE;

		pgCollideList.clear();

		state = GAME_WIN;
	}

}

void Game::render(){

	if(state == GAME_ACTIVE || state == GAME_MENU || state == GAME_WIN){

		//Begin rendering to prostprocessing quad
		postProcessor->beginRender();
		{
			//Draw background
			renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0, 0), glm::vec2(width, height), 0.0f);

			//Draw level
			levels[level].draw(*renderer);

			//Draw paddle
			player->draw(*renderer);

			//Draw PowerUps
			for(PowerUp& powerUp : powerUps)
				if(!powerUp.destroyed)
					powerUp.draw(*renderer);

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

		std::stringstream ss; ss << lives;
		text->renderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);

	}
	
	if(state == GAME_MENU){
		text->renderText("Press ENTER to start", 250.0f, height / 2, 1.0f);
		text->renderText("Press W or S to select level", 245.0f, height / 2 + 20.0f, 0.75f);
	}

	if(state == GAME_WIN){

		text->renderText("You WON!!!", 320.0, height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0));

		text->renderText("Press ENTER to retry or ESC to quit", 130.0, height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));

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
				if(!box.isSolid){
					box.destroyed = GL_TRUE;
					spawnPowerUps(box);
					soundEngine->play2D("audio/bleep1.mp3", false);
				}
				else{
					shakeTime = 0.05f;
					postProcessor->shake = GL_TRUE;
					soundEngine->play2D("audio/solid.wav", false);
				}

				//Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);

				if(!(ball->passThrough && !box.isSolid)){ // don't do collision resolution on non-solid bricks if pass-through activated 

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

	//Also check collisions on PowerUps and if so, activate them
	for(PowerUp& powerUp : powerUps){

		if(!powerUp.destroyed){
			//First check if powerup passed bottom edge, if so: keep as inactive and destroy
			if(powerUp.position.y >= height)
				powerUp.destroyed = GL_TRUE;

			if(checkCollision(*player, powerUp)){
				//Collided with player, now activate powerup
				activatePowerUp(powerUp);
				powerUp.destroyed = GL_TRUE;
				powerUp.activated = GL_TRUE;
				soundEngine->play2D("audio/solid.wav", false);
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

		//If Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
		ball->stuck = ball->sticky;

		soundEngine->play2D("audio/bleep2.wav", false);

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

	lives = 3;

}

void Game::resetPlayer(){

	//Reset player/ball stats
	player->size = PLAYER_SIZE;
	player->position =  glm::vec2(width / 2 - PLAYER_SIZE.x / 2, height - PLAYER_SIZE.y);

	ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), BALL_INITIAL_VELOCITY);

}

void Game::spawnPowerUps(GameObject& block){

	if(shouldSpawn(75)) //1 in 75 chance
		powerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position, ResourceManager::getTexture("powerup_speed")));
	if(shouldSpawn(75)) 
        powerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.position, ResourceManager::getTexture("powerup_sticky")));
	if(shouldSpawn(75))
		powerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position, ResourceManager::getTexture("powerup_passthrough")));
	if(shouldSpawn(75))
		powerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.position, ResourceManager::getTexture("powerup_increase")));
	if(shouldSpawn(15)) // Negative powerups should spawn more often
		powerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position, ResourceManager::getTexture("powerup_confuse")));
	if(shouldSpawn(15))
		powerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position, ResourceManager::getTexture("powerup_chaos")));

}

void Game::updatePowerUps(GLfloat dt){

	for (PowerUp &powerUp : powerUps){

        powerUp.position += powerUp.velocity * dt;

        if (powerUp.activated){

            powerUp.duration -= dt;

            if (powerUp.duration <= 0.0f){

                // Remove powerup from list (will later be removed)
                powerUp.activated = GL_FALSE;

                // Deactivate effects
                if (powerUp.type == "sticky"){

                    if (!isOtherPowerUpActive(powerUps, "sticky")){	// Only reset if no other PowerUp of type sticky is active
                        ball->sticky = GL_FALSE;
                        player->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.type == "pass-through"){

                    if (!isOtherPowerUpActive(powerUps, "pass-through")){ // Only reset if no other PowerUp of type pass-through is active
                        ball->passThrough = GL_FALSE;
                        ball->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.type == "confuse"){

                    if (!isOtherPowerUpActive(powerUps, "confuse"))	// Only reset if no other PowerUp of type confuse is active
						postProcessor->confuse = GL_FALSE;
                    
                }
                else if (powerUp.type == "chaos"){
                    if (!isOtherPowerUpActive(powerUps, "chaos"))	// Only reset if no other PowerUp of type chaos is active
						postProcessor->chaos = GL_FALSE;
                    
                }                
            }
        }
    }

	//Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
	//Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
	powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(), [](const PowerUp& powerUp) {return powerUp.destroyed && !powerUp.activated;}), powerUps.end());
}

GLboolean Game::shouldSpawn(GLuint chance){
	GLuint random = rand() % chance;
	return random == 0;
}

void Game::activatePowerUp(PowerUp& powerUp){
	//Initiate a powerup based type of powerup
	if(powerUp.type == "speed")
		ball->velocity *= 1.2;
	else if(powerUp.type == "sticky"){
		ball->sticky = GL_TRUE;
		player->color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if(powerUp.type == "pass-through"){
		ball->passThrough = GL_TRUE;
		ball->color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if(powerUp.type == "pad-size-increase")
		player->size.x += 50;
	else if(powerUp.type == "confuse"){
		if(!postProcessor->chaos)
			postProcessor->confuse = GL_TRUE; //Only activate if chaos wasn't already active
	}
	else if(powerUp.type == "chaos")
		if(!postProcessor->confuse)
			postProcessor->chaos = GL_TRUE;

}

GLboolean Game::isOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type){
	//Check if another PowerUp of the same type is still active
	//in which case we don't disable its effect (yet)
	for(const PowerUp& powerUp : powerUps)
		if(powerUp.activated && powerUp.type == type)
			return GL_TRUE;

	return GL_FALSE;

}