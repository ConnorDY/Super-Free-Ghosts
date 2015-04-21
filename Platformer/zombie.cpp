#include "zombie.h"
#include "globals.h"
#include "room.h"
#include "player.h"
#include "obelisk.h"

#define ZOMBIE_WIDTH  24
#define ZOMBIE_HEIGHT 40
Zombie::Zombie(TextureManager &textureManager, float x, float y)
	: DamageableObject(
		x, y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, // x, y, w, h
		0.0f, 0.0f,     // dx, dy
		false,           // solid
		0.00185f / 2.0f,       // Gravity
		0.25f            // Fall speed
	),
	rectangle(sf::Vector2f(ZOMBIE_WIDTH, ZOMBIE_HEIGHT)),
	animation("appear"),
	moveSpeed(0.075f / 2.0f), frame(0.0f), spawnX(x), spawnY(y), angle(0),
	inCasket(true), opening(false), turning(false), spawned(false), under(false), visible(true)
{
	// Sprite
	sprite.setTexture(textureManager.getRef("zombie"));
	sprite.setOrigin(11.0f, 40.0f);

	// Animations
	animations["appear"].emplace_back(0, 0, 35, 48);

	animations["casket"].emplace_back(35, 0, 35, 48);
	animations["casket"].emplace_back(70, 0, 35, 48);
	animations["casket"].emplace_back(105, 0, 35, 48);
	animations["casket"].emplace_back(140, 0, 35, 48);
	animations["casket"].emplace_back(175, 0, 35, 48);

	animations["fall"].emplace_back(0, 50, 27, 40);
	animations["turn"].emplace_back(28, 50, 27, 40);

	animations["walk"].emplace_back(56, 50, 28, 40);
	animations["walk"].emplace_back(84, 50, 28, 40);
	animations["walk"].push_back(animations.at("walk")[0]);
	animations["walk"].emplace_back(112, 50, 28, 40);

	// Depth
	setDepth(-2);
	setHealth(8);

	// Increase Movement Speed
	if (floor((double)rand() / RAND_MAX * 4.) == 1.0) moveSpeed = .12f / 2.0f;
}


/* Accessors */
int Zombie::getDir() const
{
	int dir = 1;

	if (sprite.getScale().x > 0.0f) dir = -1;

	return dir;
}

bool Zombie::getInCasket() const
{
	return inCasket;
}

bool Zombie::canCollideWith(const Object* obj) const
{
	return dynamic_cast<const Obelisk*>(obj) == nullptr;
}


/* Actions */
void Zombie::draw(sf::RenderWindow &window)
{
	if (!visible) sprite.setColor(sf::Color(255, 255, 255, 125));
	else sprite.setColor(sf::Color(255, 255, 255, 255));

	if (DEBUG_MODE)
	{
		rectangle.setPosition(sf::Vector2f(x, y));
		window.draw(rectangle);
	}

	int sign_scalex = -1, adjx = 0;
	if (sprite.getScale().x >= 0.0f) sign_scalex = 1;

	if (sign_scalex == -1) adjx += 2;

	sprite.setPosition(sf::Vector2f(x + sprite.getOrigin().x - (6.0f * inCasket) + adjx, y + sprite.getOrigin().y + inCasket + 1));
	window.draw(sprite);
}

void Zombie::update(sf::Time deltaTime, Room &room, const settings_t &settings)
{
	if (inCasket && !spawned && (room.heightmapIntersects(sf::FloatRect(x, y, width, height)) || !placeFree(x, y + 10, room)))
	{
		y -= deltaTime.asSeconds() * 50;
		spawnY = y;
		under = true;
	}
	else if (!spawned)
	{
		spawned = true;
		casketTimer.restart();
	}
	else
	{
		double mstime = deltaTime.asMicroseconds() / 1000.0;
		if (inCasket) maxFallSpeed = 0; else maxFallSpeed = 0.25;
		if (!placeFree(x, y + 1, room) && dx == 0) dx = -moveSpeed; // Hit the ground

		Object::update(deltaTime, room, settings);

		float adj = ZOMBIE_WIDTH;
		if (dx < 0) adj *= -1;

		bool turn = true;

		for (int i = -4; i < 4; i++)
		{
			if (placeFree((float)(x + dx * mstime), y + i, room)) turn = false;
		}

		if (!inCasket && !placeFree(x, y + 3, room) && (turn || (x <= 0.0f && dx < 0) || floor(((double)rand() / RAND_MAX) * (8000. / mstime)) == 1 || (placeFree(x + adj, y + 17, room) && !placeFree(x, y + 1, room))))
		{
			// Turn around
			dx = -dx;
			sprite.setScale(sf::Vector2f(1.0f * getDir(), 1.0f));
			turning = true;
			turnTimer.restart();
		}

		// Rotating
		if (inCasket && !under)
		{
			setX(spawnX + (6.0f * cos(angle)));
			setY(spawnY + (6.0f * sin(angle)));

			angle += deltaTime.asMicroseconds() / 220000.0f;
		}

		// Timers
		if (turning && turnTimer.getElapsedTime().asSeconds() >= 0.1) turning = false;

		if (inCasket && !opening && (casketTimer.getElapsedTime().asSeconds() >= 3 || (under && casketTimer.getElapsedTime().asSeconds() >= .9)))
		{
			openTimer.restart();
			opening = true;
		}

		if (opening && openTimer.getElapsedTime().asSeconds() >= 0.7)
		{
			inCasket = false;
			opening = false;
		}

		// Hit Player
		if (!inCasket)
		{
			for (auto col : allCollisions(x, y, room))
			{
				Player* player = dynamic_cast<Player*>(col);
				if (player != nullptr && !player->getInvincible()) player->damage((int)x);
			}
		}
	}

	// Invincibility
	if (under && inCasket && !opening && flashTimer.getElapsedTime().asSeconds() >= .1)
	{
		flashTimer.restart();
		visible = !visible;
	}
	else if (!under || !inCasket || opening) visible = true;

	// Animations
	if (opening) setAnimation("casket");
	else if (inCasket) setAnimation("appear");
	else if (placeFree(x, y + 1, room) && dx == 0.0f) setAnimation("fall");
	else if (turning) setAnimation("turn");
	else setAnimation("walk");

	updateAnimation(deltaTime);
}

void Zombie::setAnimation(std::string name)
{
	if (animation != name)
	{
		animation = name;
		frame = 0.0f;
	}
}

void Zombie::updateAnimation(sf::Time deltaTime)
{
	// Get current animation
	std::vector<sf::IntRect> &anim = animations.at(animation);
	int frames = anim.size();

	// Adjust frame
	if (frames > 1)
	{
		float speed = 6.0f;

		frame += deltaTime.asSeconds() * speed;
		frame = fmodf(frame, (float)frames);
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}

void Zombie::onDeath(Room &room, const settings_t &settings)
{
	if (settings.sound_on) room.getSoundManager().playSound("enemy_die");
}
