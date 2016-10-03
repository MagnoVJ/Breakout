#include "GameLevel.h"

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight){

	//Calculate dimensions
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = levelHeight / height;

	//Initialize level tiles based on tileData
	FOR(GLuint, y, height)
		FOR(GLuint, x, width){
			//Check block type from level data (2D level array)
			if(tileData[y][x] == 1){ //Solid

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::getTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.isSolid = GL_TRUE;
				bricks.push_back(obj);

			}
			else if(tileData[y][x] > 1){

				glm::vec3 color = glm::vec3(1.0f); //original: white

				switch(tileData[y][x]){

				case 2:
					color = glm::vec3(0.2f, 0.6f, 1.0f); //blue
					break;
				case 3:
					color = glm::vec3(0.0f, 0.7f, 0.0f); //green
					break;
				case 4:
					color = glm::vec3(0.8f, 0.8f, 0.4f); //yellow
					break;
				case 5:
					color = glm::vec3(1.0f, 0.5f, 0.0f); //red
					break;

				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);

				bricks.push_back(GameObject(pos, size, ResourceManager::getTexture("block"), color));

			}

		}

}

void GameLevel::load(const GLchar* file, GLuint levelWidth, GLuint levelHeight){

	//Clear old data
	bricks.clear();
	//Load from file
	GLuint tileCode;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;

	if(fstream){

		while(std::getline(fstream, line)){

			std::istringstream sstream(line);
			std::vector<GLuint> row;

			while(sstream >> tileCode) //Read each word seperated by space
				row.push_back(tileCode);

			tileData.push_back(row);

		}

		if(tileData.size() > 0)
			init(tileData, levelWidth, levelHeight);

	}

}

void GameLevel::draw(SpriteRenderer& renderer){

	for(GameObject& tile : bricks)
		if(!tile.destroyed)
			tile.draw(renderer);

}

GLboolean GameLevel::isCompleted(){

	for(GameObject& tile : bricks)
		if(!tile.isSolid && !tile.destroyed)
			return GL_FALSE;

	return GL_TRUE;

}