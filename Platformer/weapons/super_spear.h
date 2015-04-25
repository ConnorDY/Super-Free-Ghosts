#ifndef SUPER_SPEAR_H
#define SUPER_SPEAR_H

#include "spear.h"

class SuperSpear : public Spear
{
	public:
		SuperSpear(Room &room, float x, float y, int dir);
		virtual ~SuperSpear();
		static SuperSpear* spawnAdjusted(Room &room, float x, float y, int dir);
};

#endif
