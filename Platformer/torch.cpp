#include "torch.h"
#include "flame.h"
#include "room.h"
// vim: noet

Torch::Torch(float x, float y, TextureManager &textureManager)
	: Weapon(
			x, y, 8, 8,    // x, y, w, h
			0.04f, 0.04f,  // dx, dy
			0.0007f, 0.2f, // gravity, terminal velocity
			textureManager
	)
{
}

Torch::~Torch() {}

void Torch::onDeath(Room &room, settings_t const&)
{
	if (!placeFree(x, y, room)) {
		// If we hit something (ground, obelisk), try spawning a flame
		// it will put itself out if it's not in a sensible place.
		room.spawn(new Flame(x, y, 4, room.textureManager));
	}
}
