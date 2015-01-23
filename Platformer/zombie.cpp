#include "zombie.h"

#define ZOMBIE_WIDTH  27
#define ZOMBIE_HEIGHT 40
Zombie::Zombie(TextureManager &textureManager, float x, float y)
	: Object(
		x, y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, // x, y, w, h
		0.0f, 0.0f,     // dx, dy
		false,           // solid
		0.00185f / 2.0f,       // Gravity
		0.25f            // Fall speed
	),
	rectangle(sf::Vector2f(ZOMBIE_WIDTH, ZOMBIE_HEIGHT)),
	animation("appear"),
	moveSpeed(0.22f / 2.0f), frame(0.0f), spawnX(x), spawnY(y), angle(0),
	inCasket(true), turning(false), opening(false)
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
}


/* Accessors */
int Zombie::getDir() const
{
	int dir = 1;

	if (sprite.getScale().x > 0.0f) dir = -1;

	return dir;
}


/* Actions */
void Zombie::draw(sf::RenderWindow &window)
{
	if (DEBUG_MODE)
	{
		rectangle.setPosition(sf::Vector2f(x, y));
		window.draw(rectangle);
	}

	sprite.setPosition(sf::Vector2f(x + sprite.getOrigin().x - (6.0f * inCasket), y + sprite.getOrigin().y + inCasket));
	window.draw(sprite);
}

void Zombie::update(sf::Time deltaTime, Room const &room)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;
	auto const objects = room.getObjects();
	if (inCasket) maxFallSpeed = 0; else maxFallSpeed = 0.25;
	if (!placeFree(x, y + 1, objects) && dx == 0) dx = -0.075f / 2.0f; // Hit the ground

	Object::update(deltaTime, room);

	float adj = ZOMBIE_WIDTH;
	if (dx < 0) adj *= -1;

	if (!placeFree(x + dx * mstime, y, objects) || (x <= 0.0f && dx < 0) || (placeFree(x + adj, y + 17, objects) && !placeFree(x, y + 1, objects)))
	{
		// Turn around
		dx = -dx;
		sprite.setScale(sf::Vector2f(1.0f * getDir(), 1.0f));
		turning = true;
		turnTimer.restart();
	}

	// Rotating
	if (inCasket)
	{
		setX(spawnX + (6.0f * cos(angle)));
		setY(spawnY + (6.0f * sin(angle)));

		angle += deltaTime.asMicroseconds() / 220000.0f;
	}

	// Timers
	if (turning && turnTimer.getElapsedTime().asSeconds() >= 0.1) turning = false;

	if (inCasket && !opening && casketTimer.getElapsedTime().asSeconds() >= 3)
	{
		openTimer.restart();
		opening = true;
	}

	if (opening && openTimer.getElapsedTime().asSeconds() >= 0.7)
	{
		inCasket = false;
		opening = false;
	}

	// Animations
	if (opening) setAnimation("casket");
	else if (inCasket) setAnimation("appear");
	else if (placeFree(x, y + 1, objects) && dx == 0.0f) setAnimation("fall");
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
		float speed = 60.0f / 10.0f;

		if (frame < (float)frames) frame += deltaTime.asSeconds() * speed;

		if (frame >= (float)frames) frame = 0.0f; // Reset animation
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}

void Zombie::onDeath(Room const &room)
{
	room.getSoundManager().playSound("enemy_die");
}
