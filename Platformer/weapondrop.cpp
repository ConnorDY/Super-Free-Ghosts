#include <stdexcept>
#include "globals.h"
#include "weapondrop.h"
#include "player.h"
#include "room.h"

namespace
{
	int const WEAPON_DROP_WIDTH = 15;
	int const WEAPON_DROP_HEIGHT = 15;
	int const SPRITE_WIDTH = 75;
	int const SPRITE_HEIGHT = 75;
	int const SPRITE_ORIGIN_X = 30;
	int const SPRITE_ORIGIN_Y = 30;
	int const NUM_FRAMES = 40;
	int const INNER_GLOW_SPRITESTRIP_LEFT = NUM_FRAMES * SPRITE_WIDTH;
	int const OUTER_GLOW_SPRITESTRIP_LEFT = INNER_GLOW_SPRITESTRIP_LEFT + SPRITE_WIDTH;
	float const ANIMATION_SPEED = 15;

	int GCC(__attribute__((const))) getSpriteStripTop(PlayerWeapon::Enum type, bool super)
	{
		using namespace PlayerWeapon;
		auto super_offset = super ? 75 : 0;
		switch(type)
		{
			case SPEAR:       return 0 + super_offset;
			case TRIDENT:     return 150 + super_offset;
			case BOOMERANG:   return 300 + super_offset;
			case HAMMER:      return 450 + super_offset;
			case BOMB:        return 600 + super_offset;
			case SWORD:       return 750 + super_offset;
			case MACE:        return 900 + super_offset;
			case AXE:         return 1050 + super_offset;
			case WAR_HAMMER:  return 1200 + super_offset;
			default: throw std::domain_error("Weapon with no icon given to WeaponDrop");
		}
	}
}

WeaponDrop::WeaponDrop(Room &room, float x, float y, PlayerWeapon::Enum type)
	: Object(room,
		x, y, 15, 15,
		0.0f, 0.0f, false
	),
	  innerGlow(room.textureManager.getRef("weapon_drops")),
	  outerGlow(room.textureManager.getRef("weapon_drops")),
	  type(type)
{
	setDepth(-3);

	DEBUG(
		rect.setSize(sf::Vector2f(width, height));
		rect.setFillColor(sf::Color(0, 255, 70, 200));
	)

	sprite.setTexture(room.textureManager.getRef("weapon_drops"));
	sprite.setOrigin(SPRITE_ORIGIN_X, SPRITE_ORIGIN_Y);
	innerGlow.setOrigin(SPRITE_ORIGIN_X, SPRITE_ORIGIN_Y);
	outerGlow.setOrigin(SPRITE_ORIGIN_X, SPRITE_ORIGIN_Y);
	setWeaponAnimation();

	// This can't move yet, so no need to set these elsewhere... yet.
	DEBUG(rect.setPosition(x, y);)
	sprite.setPosition(x, y);
	outerGlow.setPosition(x, y);
	innerGlow.setPosition(x, y);
}

WeaponDrop::~WeaponDrop()
{
}

void WeaponDrop::draw(sf::RenderWindow &window)
{
	DEBUG(window.draw(rect);)

	window.draw(outerGlow);
	window.draw(innerGlow);
	window.draw(sprite);
}

void WeaponDrop::update(sf::Time deltaTime)
{
	for (auto col : allCollisions(x, y))
	{
		Player* player = dynamic_cast<Player*>(col);
		if (player != nullptr)
		{
			player->setWeapon(type);
			kill();
			return;
		}
	}
	updateAnimation(deltaTime);
}

void WeaponDrop::setWeaponAnimation()
{
	auto spriteStripTop = getSpriteStripTop(type, false); // TODO: When do we drop a super weapon? Does that change if the player is damaged while it's on the ground?
	for (int i = 0; i < NUM_FRAMES; i++)
		animation.emplace_back(i * SPRITE_WIDTH, spriteStripTop, SPRITE_WIDTH, SPRITE_HEIGHT);
	innerGlow.setTextureRect(sf::IntRect(INNER_GLOW_SPRITESTRIP_LEFT, spriteStripTop, SPRITE_WIDTH, SPRITE_HEIGHT));
	outerGlow.setTextureRect(sf::IntRect(OUTER_GLOW_SPRITESTRIP_LEFT, spriteStripTop, SPRITE_WIDTH, SPRITE_HEIGHT));
}

#define MAP(val, oldLow, oldHigh, newLow, newHigh) ((newLow) - (oldLow) + (val) * (((newHigh) - (newLow)) / static_cast<float>((oldHigh) - (oldLow))))

void WeaponDrop::updateAnimation(sf::Time deltaTime)
{
	animationFrame = fmod(animationFrame + ANIMATION_SPEED * deltaTime.asSeconds(), NUM_FRAMES);

	auto shineAmount = fabs(animationFrame - 20);
	printf("%f\n",MAP(shineAmount, 0, 20, 20, 80));
	outerGlow.setColor(sf::Color(255, 255, 255, MAP(shineAmount, 0, 20, 20, 80)));
	innerGlow.setColor(sf::Color(255, 255, 255, MAP(shineAmount, 0, 20, 60, 200)));

	sprite.setTextureRect(animation[static_cast<int>(floor(animationFrame))]);
}
