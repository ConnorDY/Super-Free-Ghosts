#include "casket.h"
#include "zombie.h"
#include "globals.h"
#include "room.h"
#include "player.h"

namespace {
	int const CASKET_WIDTH = 24;
	int const CASKET_HEIGHT = 40;

	std::vector<sf::IntRect> const ANIM_APPEARING = {
		sf::IntRect(0, 0, 35, 48),
	};

	std::vector<sf::IntRect> const ANIM_CASKET = {
		sf::IntRect(35, 0, 35, 48),
		sf::IntRect(70, 0, 35, 48),
		sf::IntRect(105, 0, 35, 48),
		sf::IntRect(140, 0, 35, 48),
		sf::IntRect(175, 0, 35, 48),
	};
}
Casket::Casket(Room &room, float x, float y)
	: Object(
		room, x, y, CASKET_WIDTH, CASKET_HEIGHT, // x, y, w, h
		0.0f, 0.0f,     // dx, dy
		false           // solid
	),
	rectangle(sf::Vector2f(CASKET_WIDTH, CASKET_HEIGHT)),
	animation(&ANIM_APPEARING),
	frame(0.0f), spawnX(x), spawnY(y), angle(0),
	opening(false), turning(false), spawned(false), under(false), visible(true)
{
	// Sprite
	sprite.setTexture(room.textureManager.getRef("zombie"));
	sprite.setOrigin(11.0f, 40.0f);

	// Depth
	setDepth(-2);
}



/* Actions */
void Casket::draw(sf::RenderWindow &window)
{
	if (!visible) sprite.setColor(sf::Color(255, 255, 255, 125));
	else sprite.setColor(sf::Color(255, 255, 255, 255));

	if (DEBUG_MODE)
	{
		rectangle.setPosition(sf::Vector2f(x, y));
		window.draw(rectangle);
	}

	sprite.setPosition(sf::Vector2f(x + sprite.getOrigin().x - 6, y + sprite.getOrigin().y + 2));
	window.draw(sprite);
}

void Casket::update(sf::Time deltaTime)
{
	if (finished) kill(); // We need to delay this 1 frame because the spawned zombie will not immediately draw
	if (!spawned && (room.heightmapIntersects(sf::FloatRect(x, y, width, height)) || !placeFree(x, y + 10)))
	{
		y -= deltaTime.asSeconds() * 50;
		under = true;
	}
	else if (!spawned)
	{
		spawned = true;
		casketTimer.restart();
	}
	else
	{
		// Rotating
		if (!under)
		{
			setX(spawnX + 6.0f * cos(angle));
			setY(spawnY + 6.0f * sin(angle));

			angle += deltaTime.asMicroseconds() / 220000.0f;
		}

		// Timers
		if (turning && turnTimer.getElapsedTime().asSeconds() >= 0.1) turning = false;

		if (!opening && (casketTimer.getElapsedTime().asSeconds() >= 3 || (under && casketTimer.getElapsedTime().asSeconds() >= .9)))
		{
			openTimer.restart();
			opening = true;
			setAnimation(&ANIM_CASKET);
		}

		if (opening && openTimer.getElapsedTime().asSeconds() >= 0.7)
		{
			finished = true;
			room.spawn(new Zombie(room, x, y));
		}
	}

	// Invincibility
	if (under && !opening && flashTimer.getElapsedTime().asSeconds() >= .1)
	{
		flashTimer.restart();
		visible = !visible;
	}
	else if (!under || opening) visible = true;

	updateAnimation(deltaTime);
}

void Casket::setAnimation(std::vector<sf::IntRect> const *anim)
{
	animation = anim;
	frame = 0.0f;
}

void Casket::updateAnimation(sf::Time deltaTime)
{
	int frames = animation->size();

	// Adjust frame
	float speed = 6.0f;

	frame += deltaTime.asSeconds() * speed;
	frame = fmodf(frame, (float)frames);

	// Set TextureRect
	sprite.setTextureRect((*animation)[(int)frame]);
}
