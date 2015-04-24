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
		Crystal(Room &room, float x, float y);
		~Crystal();

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif
