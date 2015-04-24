#include "torch.h"
#include "flame.h"
#include "room.h"

namespace
{
	sf::IntRect const BBOX(0, 0, 8, 8);
	int const SPRITE_WIDTH = 0, SPRITE_HEIGHT = 0;
}

Torch::Torch(Room &room, float x, float y, int dir)
	: Weapon(
			room, x, y, BBOX.width, BBOX.height, // x, y, w, h
			0.12f * dir, -0.18f,           // dx, dy
			0.0007f, 0.2f,                 // gravity, terminal velocity
			12
	)
{
	// TODO: Set sprite and such
}

Torch::~Torch() {}

void Torch::onDeath()
{
	if (!placeFree(x, y)) {
		// If we hit something (ground, obelisk), try spawning a flame
		// it will put itself out if it's not in a sensible place.
		room.spawn(new Flame(room, x, y, dx > 0 ? 1 : -1, 4));
	}
}

Torch* Torch::spawnAdjusted(Room &room, float x, float y, int dir)
{
	if (dir < 0) x -= BBOX.width;
	return new Torch(room, x, y, dir);
}
