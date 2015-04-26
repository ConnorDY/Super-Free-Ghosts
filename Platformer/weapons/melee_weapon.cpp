#include "melee_weapon.h"
#include "player.h"
#include "room.h"

MeleeWeapon::MeleeWeapon(Room &room, Player const &player, sf::IntRect boundingBox, int damage)
	: Weapon(room, 0, 0, boundingBox.width, boundingBox.height, 0, 0, 0, 0, damage),
	  player(player), boundingBox(boundingBox)
{
	setDepth(-5); // Must display on top of player
	sprite.setTexture(room.textureManager.getRef("melee"));
	animationSpeed = player.getAnimationSpeed();
	destroyedOnHit = false;
	destroyedOnSolidCollision = false;
	destroyedOnExitView = false;
}

MeleeWeapon::~MeleeWeapon() {}

void MeleeWeapon::move(sf::Time)
{
	auto spriteTopLeft = player.getSpriteTopLeft();
	x = spriteTopLeft.x - 25;
	y = spriteTopLeft.y - 25;
}

sf::FloatRect MeleeWeapon::getRect() const
{
	return sf::FloatRect(x + boundingBox.left, y + boundingBox.top, width, height);
}

void MeleeWeapon::update(sf::Time time)
{
	Weapon::update(time);
	if (animationCycle > 0) kill();
}
