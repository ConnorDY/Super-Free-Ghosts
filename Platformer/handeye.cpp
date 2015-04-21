#include "handeye.h"
#include "globals.h"
#include "room.h"
#include "player.h"
#include "obelisk.h"

HandEye::HandEye(TextureManager &textureManager, float x, float y)
	: DamageableObject(
		x, y, 53, 40,
		0.0f, 0.0f, false,
		0.00185f / 2.0f, 0.25f
	),
	animation("awake"), frame(0),
	pulling(false), awake(false), waking(false)
{
	for (unsigned int i = 0; i < 7; i++) animations["pull"].emplace_back(0, i * 56, 55, 56);
	for (unsigned int i = 7; i < 17; i++) animations["pulling"].emplace_back(0, i * 56, 55, 56);
	for (unsigned int i = 0; i < 20; i++) animations["awake"].emplace_back(55, i * 40, 53, 40);

	sprite.setTexture(textureManager.getRef("handeye"));
	setDepth(-2);
	setHealth(8);
}


/* Accessors */
bool HandEye::canCollideWith(const Object* obj) const
{
	return dynamic_cast<const Obelisk*>(obj) == nullptr;
}


/* Actions */
void HandEye::draw(sf::RenderWindow &window)
{
	sf::FloatRect boundingRect = getRect();

	if (DEBUG_MODE)
	{
		rectangle.setSize(sf::Vector2f(boundingRect.width, boundingRect.height));
		rectangle.setPosition(boundingRect.left, boundingRect.top);
		window.draw(rectangle);
	}

	float adjx = 0.0f, adjy = 0.0f;

	if (animation != "awake")
	{
		adjx = -2.0f;
		adjy = -10.0f;
	}

	if (sprite.getScale().x < 0.0f) adjx = boundingRect.width - adjx;

	sprite.setPosition(x + adjx, y + adjy);
	window.draw(sprite);
}

void HandEye::update(sf::Time deltaTime, Room &room, const settings_t &settings)
{
	Object::update(deltaTime, room, settings);

	if (!awake && !waking) // TODO: Add check for the distance to the player
	{
		waking = true;
	}

	if (pulling) setAnimation("pulling");
	else if (awake) setAnimation("pull");
	else setAnimation("awake");

	updateAnimation(deltaTime);
}

void HandEye::setAnimation(std::string name)
{
	if (animation != name)
	{
		animation = name;
		frame = 0.0f;
	}
}

void HandEye::updateAnimation(sf::Time deltaTime)
{
	// Get current animation
	std::vector<sf::IntRect> &anim = animations.at(animation);
	int frames = anim.size();

	// Adjust frame
	if (frames > 1)
	{
		double speed = 10.;

		if (awake || waking) frame += deltaTime.asSeconds() * speed;

		if (frame >= frames - 1)
		{
			if (awake && !pulling)
			{
				pulling = true;
			}
			if (waking)
			{
				waking = false;
				awake = true;
				frame = frames - 1;
			}
		}

		frame = fmodf(frame, (float)frames);
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}

void HandEye::onDeath(Room &room, const settings_t &settings)
{
	if (settings.sound_on) room.getSoundManager().playSound("enemy_die");
}
