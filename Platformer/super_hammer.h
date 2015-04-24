#ifndef SUPER_HAMMER_H
#define SUPER_HAMMER_H

#include "hammer.h"

class SuperHammer : public Hammer
{
	private:
		int remainingBounces;
	public:
		SuperHammer(Room &room, float x, float y, int dir);
		SuperHammer(Room &room, float x, float y, int dir, int remainingBounces);
		virtual ~SuperHammer();
		virtual void move(sf::Time deltaTime);
		virtual void onDeath();
		static SuperHammer* spawnAdjusted(Room &room, float x, float y, int dir);
};

#endif
