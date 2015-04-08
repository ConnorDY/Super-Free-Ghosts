#ifndef FLAME_H
#define FLAME_h
// vim: noet
#include "weapon.h"

class Flame : public Weapon
{
	private:
		int remaining;  // How many more flames are remaining to spawn after this one goes out
		int lifetime;
	public:
		Flame(float x, float y, int remaining, TextureManager &textureManager);
		virtual ~Flame();

		virtual void move(sf::Time deltaTime, Room &room, settings_t const &settings);
		virtual void onDeath(Room &room, settings_t const&);
};

#endif
