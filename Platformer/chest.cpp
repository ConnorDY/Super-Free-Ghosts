#include <array>
#include "chest.h"
#include "globals.h"
#include "obelisk.h"
#include "weapons/player_weapon.h"
#include "armourdrop.h"
#include "weapondrop.h"
#include "room.h"

Chest::Chest(Room &room, float x, float y)
	: DamageableObject(
		room, x, y, 31, 19,
		0, 0, false
	),
	  animation(3), slideUpFraction(0),
	  frame(0), leaveTimer(0),
	  slidingUp(false), leaving(false)
{
	rect.setSize(sf::Vector2f(width, height));
	rect.setFillColor(sf::Color(255, 0, 255, 128));
	setHealth(3);
	setDepth(-2);

	// Sprite
	spr.setTexture(room.textureManager.getRef("chest1"));
	setDepth(1);

	// Frames
	for (unsigned int i = 0; i < 8; i++) frames.emplace_back(0, i * 26, 35, 26);

	// Animations
	animations.push_back({ frames[1], frames[2], frames[0] });
	animations.push_back({ frames[2], frames[1], frames[0] });
	animations.push_back({ frames[3], frames[4], frames[5], frames[6], frames[7] });
	animations.push_back({ frames[0] });
}

sf::FloatRect Chest::getRect() const
{
	auto yAdjustment = (1 - slideUpFraction) * height;
	return sf::FloatRect(x, y + yAdjustment, width, height);
}

bool Chest::isLeaving() const
{
	return leaving;
}

void Chest::damage(Object*, int)
{
	int hp = getHealth() - 1;
	if (hp < 0) hp = 0;
	setHealth(hp);
	setAnimation(2 - hp);
}

void Chest::draw(sf::RenderWindow &window)
{
	// Don't bother drawing if it's completely submerged
	if (slideUpFraction == 0) return;

	auto bbox = getRect();

	// Transparency
	double a = 1;

	if (leaving) a = (750. - leaveTimer) / 750.;
	if (a < 0) a = 0;

	spr.setColor(sf::Color(255, 255, 255, 255 * a));

	spr.setPosition(bbox.left, bbox.top - 7.0f);
	window.draw(spr);

	if (DEBUG_MODE)
	{
		rect.setPosition(sf::Vector2f(bbox.left, bbox.top));
		window.draw(rect);
	}
}

void Chest::update(sf::Time deltaTime)
{
	if (slidingUp)
	{
		// Our Y value matters now, so check it:
		pushOutOfHeightmap();
		// Check for collisions and jump to the top of them
		auto mybbox = getRect();
		for (auto obj : allCollisions(x, y - 1))
			if (obj->isSolid())
			{
				auto objbbox = obj->getRect();
				y = std::min<int>(y, objbbox.top - mybbox.height);
			}

		// Continue slide up animation
		slideUpFraction = std::min<float>(1, slideUpFraction + deltaTime.asMilliseconds() / 500.f);
		if (slideUpFraction == 1) slidingUp = false;
	}
	// XXX The usual updates are a waste of time for us atm
	//Object::update(deltaTime);

	if (leaving)
	{
		if (leaveTimer > 750.0)
		{
			leaveTimer = 750.0;
			kill();
		}
		else leaveTimer += deltaTime.asMilliseconds();
	}

	updateAnimation(deltaTime);
}

void Chest::onDoubleJumpedOver()
{
	slidingUp = true;
}

void Chest::setAnimation(int a)
{
	if (animation != a)
	{
		frame = 0.0f;
		animation = a;
	}
}

void Chest::updateAnimation(sf::Time deltaTime)
{
	// Get current animation
	std::vector<sf::IntRect> &anim = animations.at(animation);
	int frames = anim.size();

	// Adjust frame
	if (frames > 1)
	{
		float speed = .019f;
		if (animation == 2) speed = .012f;

		frame += (float)deltaTime.asMilliseconds() * speed;

		if (frame > (float)(frames - 1))
		{
			frame = (float)(frames - 1);

			if (!leaving && getHealth() <= 0)
			{
				PlayerWeapon::Enum options[] = { PlayerWeapon::HAMMER, PlayerWeapon::TRIDENT, PlayerWeapon::SWORD, PlayerWeapon::AXE, PlayerWeapon::WAR_HAMMER };

				room.spawn(new WeaponDrop(room, x + 7, y - 9, options[rand() % (sizeof(options) / sizeof(*options))]));
				leaving = true;
			}
		}
	}

	// Set TextureRect
	spr.setTextureRect(anim[(int)frame]);
}
