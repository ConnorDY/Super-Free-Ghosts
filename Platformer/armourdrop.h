#ifndef ARMOURDROP_H
#define ARMOURDROP_H

#include "object.h"
#include "player_armour.h"

class ArmourDrop : public Object
{
	private:
		sf::RectangleShape rect;
		PlayerArmour::Enum type;
	public:
		ArmourDrop(Room &room, float x, float y);
		~ArmourDrop();

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif
