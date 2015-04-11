#include "chest.h"
#include "globals.h"
#include "obelisk.h"

Chest::Chest(TextureManager &textureManager, float x, float y)
	: DamageableObject(
		x, y, 31, 19,
		0, 0, false
	),
	  frame(0), animation(3),
	  slideUpFraction(0),
	  slidingUp(false), leaving(false)
{
	(void)textureManager; // textureManager is unused but will be used in future
	rect.setSize(sf::Vector2f(width, height));
	setHealth(3);

	// Sprite
	spr.setTexture(textureManager.getRef("chest1"));

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

void Chest::damage(int dmg, Room &room, const settings_t &settings)
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

	if (DEBUG_MODE)
	{
		rect.setPosition(sf::Vector2f(bbox.left, bbox.top));
		window.draw(rect);
	}

	// Transparency
	double a = 1;
	if (leaving) a = (750. - leaveTimer.getElapsedTime().asMilliseconds()) / 750.;
	spr.setColor(sf::Color(255, 255, 255, 255 * a));

	spr.setPosition(bbox.left, bbox.top - 7.0f);
	window.draw(spr);
}

void Chest::update(sf::Time deltaTime, Room &room, const settings_t &settings)
{
	if (slidingUp)
	{
		// Our Y value matters now, so check it:
		pushOutOfHeightmap(room);
		// Check for obelisk collisions and jump to the top of them
		auto mybbox = getRect();
		for (auto obj : allCollisions(x, y - 1, room))
			if (dynamic_cast<Obelisk*>(obj) != nullptr)
			{
				auto objbbox = obj->getRect();
				y = std::min<int>(y, objbbox.top - mybbox.height);
			}

		// Continue slide up animation
		slideUpFraction = std::min<float>(1, slideUpFraction + deltaTime.asMilliseconds() / 500.f);
		if (slideUpFraction == 1) slidingUp = false;
	}
	// XXX The usual updates are a waste of time for us atm
	//Object::update(deltaTime, room, settings);

	if (leaving && leaveTimer.getElapsedTime().asMilliseconds() > 750)
	{
		kill(room, settings);
	}

	updateAnimation(deltaTime);
}

void Chest::onDoubleJumpedOver(Room &)
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
				leaving = true;
				leaveTimer.restart();
			}
		}
	}

	// Set TextureRect
	spr.setTextureRect(anim[(int)frame]);
}
