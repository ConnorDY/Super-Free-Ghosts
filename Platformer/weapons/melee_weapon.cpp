#include "melee_weapon.h"
#include "player.h"
#include "room.h"

namespace
{
	float const SPRITE_OFFSET_X = -25.0f;
	float const SPRITE_OFFSET_Y = -25.0f;
}

MeleeWeapon::MeleeWeapon(Room &room, Player const &player, sf::IntRect boundingBox, int dir, int damage)
	: Weapon(room, 0, 0, boundingBox.width, boundingBox.height, 0, 0, 0, 0, damage),
	  player(player), boundingBox(boundingBox), dir(dir)
{
	setDepth(-5); // Must display on top of player
	if (dir < 0) sprite.setScale(-1, 1);
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
	x = spriteTopLeft.x + SPRITE_OFFSET_X * dir;
	y = spriteTopLeft.y + SPRITE_OFFSET_Y;
}

sf::FloatRect MeleeWeapon::getRect() const
{
	float xAdjust;
	if (dir < 0) xAdjust = -boundingBox.width - boundingBox.left;
	else xAdjust = boundingBox.left;
	return sf::FloatRect(x + xAdjust, y + boundingBox.top, width, height);
}

bool MeleeWeapon::canDamage()
{
	return floor(animationFrame) == 1.0f;
}

void MeleeWeapon::update(sf::Time time)
{
	Weapon::update(time);
	if (player.isKnockedBack()) kill(); // Interrupt if player gets hurt
	if (animationCycle > 0) kill();     // Destroy if animation is complete
}
