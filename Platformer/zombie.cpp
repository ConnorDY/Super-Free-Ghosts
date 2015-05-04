#include "zombie.h"
#include "globals.h"
#include "room.h"
#include "player.h"
#include "obelisk.h"

namespace
{
	int const ZOMBIE_WIDTH = 24;
	int const ZOMBIE_HEIGHT = 40;

	std::vector<sf::IntRect> const ANIM_FALL {
		sf::IntRect(0, 50, 27, 40),
	};
	std::vector<sf::IntRect> const ANIM_TURN {
		sf::IntRect(28, 50, 27, 40),
	};
	std::vector<sf::IntRect> const ANIM_WALK {
		sf::IntRect(56, 50, 28, 40),
		sf::IntRect(84, 50, 28, 40),
		sf::IntRect(56, 50, 28, 40),
		sf::IntRect(112, 50, 28, 40),
	};
}
#define ZOMBIE_WIDTH  24
#define ZOMBIE_HEIGHT 40
Zombie::Zombie(Room &room, float x, float y)
	: DamageableObject(
		room, x, y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, // x, y, w, h
		0.0f, 0.0f,      // dx, dy
		false,           // solid
		0.00185f / 2.0f, // Gravity
		0.25f            // Fall speed
	),
	rectangle(sf::Vector2f(ZOMBIE_WIDTH, ZOMBIE_HEIGHT)),
	animation(&ANIM_FALL),
	moveSpeed(0.075f / 2.0f), frame(0.0f),
	turning(false)
{
	// Sprite
	sprite.setTexture(room.textureManager.getRef("zombie"));
	sprite.setOrigin(11.0f, 40.0f);

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

bool Zombie::canCollideWith(const Object* obj) const
{
	return dynamic_cast<const Obelisk*>(obj) == nullptr;
}


/* Actions */
void Zombie::draw(sf::RenderWindow &window)
{
	sprite.setColor(sf::Color(255, 255, 255, 255));

	if (DEBUG_MODE)
	{
		rectangle.setPosition(sf::Vector2f(x, y));
		window.draw(rectangle);
	}

	int sign_scalex = -1, adjx = 0;
	if (sprite.getScale().x >= 0.0f) sign_scalex = 1;

	if (sign_scalex == -1) adjx += 2;

	sprite.setPosition(sf::Vector2f(x + sprite.getOrigin().x + adjx, y + sprite.getOrigin().y + 1));
	window.draw(sprite);
}

void Zombie::update(sf::Time deltaTime)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;
	if (dx == 0 && !placeFree(x, y + 1)) dx = -moveSpeed; // Hit the ground

	Object::update(deltaTime);

	if (dx != 0)
	{
		if (std::copysign(dx, getDir()) != dx) throw std::logic_error("Zombie direction is wrong");
		float adj = ZOMBIE_WIDTH;
		if (dx < 0) adj *= -1;

		bool turn = true;

		for (int i = -4; i < 4; i++)
		{
			if (placeFree((float)(x + dx * mstime), y + i)) turn = false;
		}

		if ((turn || (x <= 0.0f && dx < 0) || floor(((double)rand() / RAND_MAX) * (8000. / mstime)) == 1 || (placeFree(x + adj, y + 17) && !placeFree(x, y + 1))) && !placeFree(x, y + 3))
		{
			// Turn around
			dx = -dx;
			sprite.setScale(sf::Vector2f(1.0f * getDir(), 1.0f));
			turning = true;
			turnTimer.restart();
		}
	}

	// Timers
	if (turning && turnTimer.getElapsedTime().asSeconds() >= 0.1) turning = false;

	// Hit Player
	for (auto col : allCollisions(x, y))
	{
		Player* player = dynamic_cast<Player*>(col);
		if (player != nullptr) player->damage(this, 1);
	}

	// Animations
	if (placeFree(x, y + 1) && dx == 0.0f) setAnimation(&ANIM_FALL);
	else if (turning) setAnimation(&ANIM_TURN);
	else setAnimation(&ANIM_WALK);

	updateAnimation(deltaTime);
}

void Zombie::setAnimation(std::vector<sf::IntRect> const *anim)
{
	if (animation != anim)
	{
		animation = anim;
		frame = 0.0f;
	}
}

void Zombie::updateAnimation(sf::Time deltaTime)
{
	// Get current animation
	int frames = animation->size();

	// Adjust frame
	float speed = 6.0f;

	frame += deltaTime.asSeconds() * speed;
	frame = fmodf(frame, (float)frames);

	// Set TextureRect
	sprite.setTextureRect((*animation)[(int)frame]);
}

void Zombie::onDeath()
{
	room.soundManager.playSound("enemy_die");
}
