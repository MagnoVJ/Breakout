
#include "SpriteRenderer.h"
#include "utility/Texture2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

class PGFlare{

public:
	Texture2D sprite;

	glm::vec2 position, size;
	glm::vec3 color;

	PGFlare(Texture2D& sprite);
	PGFlare(Texture2D& sprite, glm::vec2 position, glm::vec2 size);

	void draw(SpriteRenderer& renderer);

};