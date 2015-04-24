#ifndef SUPER_TRIDENT_H
#define SUPER_TRIDENT_H

#include "trident.h"

class SuperTrident : public Trident
{
	private:
		sf::Sprite tridentOverlay;
	public:
		SuperTrident(Room &room, float x, float y, int dir);
		virtual ~SuperTrident();
		virtual void draw(sf::RenderWindow &window);
		static SuperTrident* spawnAdjusted(Room &room, float x, float y, int dir);
};

#endif
