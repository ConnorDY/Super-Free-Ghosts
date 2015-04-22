#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "object.h"
#include "player.h"

class Crystal : public Object
{
	private:
		sf::RectangleShape rect;
		PlayerArmour::Enum type;
	public:
		Crystal(TextureManager &textureManager, float x, float y);
		~Crystal();

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, const settings_t &settings);
};

#endif