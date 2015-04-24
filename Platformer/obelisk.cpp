#include "obelisk.h"
#include "room.h"

Obelisk::Obelisk(Room &room, float x, float y, float height)
	: Object(
		room, x, y, 30, height
	)
{
	spr.setTexture(room.textureManager.getRef("obelisk"));
	
	flames.push_back(sf::IntRect(30, 30, 1, 1));
	flames.push_back(sf::IntRect(30, 8, 7, 8));
	flames.push_back(sf::IntRect(30, 0, 7, 8));
	flames.push_back(sf::IntRect(30, 8, 7, 8));
}


/* Actions */
void Obelisk::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	// Flame Animation
	frame += dir * deltaTime.asSeconds() * 5;
	frame = fmod(frame, 4);
}

void Obelisk::draw(sf::RenderWindow &window)
{
	// Draw Obelisk
	spr.setTextureRect(sf::IntRect(0, 0, 30, 72));
	spr.setPosition(sf::Vector2f(x, y - 1)); // "y - 1" as an easy collision fix of sorts
	window.draw(spr);

	spr.setTextureRect(flames.at((unsigned int)frame));
	spr.setPosition(sf::Vector2f(x + 13, y + 17));
	window.draw(spr);
}
