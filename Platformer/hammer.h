#ifndef HAMMER_H
#define HAMMER_H

#include "weapon.h"

class Hammer : public Weapon
{
	protected:
		// See Spear for what this constructor does
		Hammer(Room &room, float x, float y, int width, int height, int dir);
	public:
		Hammer(Room &room, float x, float y, int dir);
		virtual ~Hammer();
		static Hammer* spawnAdjusted(Room &room, float x, float y, int dir);
};

#endif
