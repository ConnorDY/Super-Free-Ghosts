#ifndef TORCH_H
#define TORCH_H
// vim: noet

#include "weapon.h"

class Torch : public Weapon
{
	public:
		Torch(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Torch();

		virtual void onDeath(Room &room, const settings_t &settings);
};

#endif
