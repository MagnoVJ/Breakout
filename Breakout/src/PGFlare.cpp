#include "PGFlare.h"

PGFlare::PGFlare(Texture2D& sprite) : sprite(sprite), position(0.0f), color(1.0f) { }

PGFlare::PGFlare(Texture2D& sprite, glm::vec2 position, glm::vec2 size) : sprite(sprite), position(position), size(size), color(1.0f){ }

void PGFlare::draw(SpriteRenderer& renderer){

	renderer.drawSprite(sprite, position, size, 0.0f, color);

}