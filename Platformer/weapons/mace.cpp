#include <vector>
#include <SFML/Graphics.hpp>
#include "mace.h"
#include "player.h"

namespace {
	sf::IntRect BBOX_NORMAL(54, 60, 34, 6);
	sf::IntRect BBOX_CROUCH(56, 64, 34, 6);

	inline sf::IntRect getBB(Player const &player)
	{
		return player.isCrouching() ? BBOX_CROUCH : BBOX_NORMAL;
	}

	inline int getSpriteTop(Player const &player)
	{
		return player.isCrouching() ? 75 : 0;
	}
}

Mace::Mace(Room &room, Player const &player, int dir)
	: MeleeWeapon(room, player, getBB(player), dir, 10)
{
	auto anim_top = getSpriteTop(player);
	for (int i = 0; i < 3; i++)
		animationFrames.emplace_back(i * 100, anim_top + 900, 100, 75);
}

Mace::~Mace() {}
