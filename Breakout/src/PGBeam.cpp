#include "PGBeam.h"

PGBeam::PGBeam(Texture2D& sprite) : sprite(sprite), position(0.0f), velocity(0.0f), rotation(0.0f), color(1.0f) { }

PGBeam::PGBeam(Texture2D& sprite, glm::vec2 position, glm::vec2 size, glm::vec2 velocity, GLfloat rotation) : sprite(sprite), position(position), size(size), velocity(velocity), rotation(rotation), color(1.0f) { }

void PGBeam::draw(SpriteRenderer& renderer){

	renderer.drawSprite(sprite, position, size, rotation, color);

}