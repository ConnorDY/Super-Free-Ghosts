#include <vector>
#include <SFML/Graphics.hpp>
#include "sword.h"
#include "player.h"

namespace {
	sf::IntRect BBOX(54, 60, 34, 6);
}

Sword::Sword(Room &room, Player const &player, int dir)
	: MeleeWeapon(room, player, BBOX, dir, 10)
{
	auto anim_top = player.isCrouching() ? 75 : 0;
	for (int i = 0; i < 3; i++)
		animationFrames.emplace_back(i * 100, anim_top, 100, 75);
}

Sword::~Sword() {}
