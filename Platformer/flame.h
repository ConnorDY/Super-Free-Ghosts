#ifndef FLAME_H
#define FLAME_h
// vim: noet
#include "weapon.h"

class Flame : public Weapon
{
	private:
		int remaining;  // How many more flames are remaining to spawn after this one goes out
		int lifetime;
		int dir;
	public:
		Flame(Room &room, float x, float y, int dir, int remaining);
		virtual ~Flame();

		virtual void move(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif
