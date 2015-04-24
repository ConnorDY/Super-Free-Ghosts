#ifndef TRIDENT_H
#define TRIDENT_H

#include "weapon.h"

class Trident : public Weapon
{
	private:
		float trailX;
	protected:
		// This constructor does not set the animation frames, nor the sprite origin/scale. The only change it makes to the sprite is to set its texture. Hence it is protected, because nothing outside of the inheritance hierarchy could use it.
		Trident(Room &room, float x, float y, int width, int height, int dir);
	public:
		Trident(Room &room, float x, float y, int dir);
		virtual ~Trident();
		virtual void draw(sf::RenderWindow &window) override;
		static Trident* spawnAdjusted(Room &room, float x, float y, int dir);
		static bool canThrow(Room const &room);
};

#endif
