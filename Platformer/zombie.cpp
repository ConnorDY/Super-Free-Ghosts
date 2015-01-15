#include "zombie.h"

#define ZOMBIE_WIDTH  54
#define ZOMBIE_HEIGHT 80
Zombie::Zombie(TextureManager &textureManager, float x, float y)
	: Object(
			Object::Type::Zombie,
			x, y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, // x, y, w, h
			0.0f, 0.0f,     // dx, dy
			false,           // solid
			0.00185f,       // Gravity
			0.5f            // Fall speed
	),
	rectangle(sf::Vector2f(ZOMBIE_WIDTH, ZOMBIE_HEIGHT)),
	animation("casket"),
	moveSpeed(0.22f), frame(0.0f),
	inCasket(true), turning(false)
{
	// Sprite
	sprite.setTexture(textureManager.getRef("zombie"));
	sprite.setOrigin(11.0f, 40.0f);
	sprite.setScale(sf::Vector2f(2.0f, 2.0f));

	// Animations
	animations["casket"].emplace_back(0, 0, 35, 48);
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
		rectangle.setPosition(sf::Vector2f(getX(), getY()));
		window.draw(rectangle);
	}

	sprite.setPosition(sf::Vector2f(roundf(getX() + (sprite.getOrigin().x * 2.0f)) - (12.0f * inCasket), roundf(getY() + (sprite.getOrigin().y * 2.0f) + (2.0f * inCasket))));
	window.draw(sprite);
}

void Zombie::update(sf::Time deltaTime, std::vector<Object*> const objects)
{
	if (inCasket) setDY(0); // Can't fall while in the casket
	if (!placeFree(getX(), getY() + 1, objects) && getDX() == 0) setDX(-0.075f); // Hit the ground

	Object::update(deltaTime, objects);

	if (inCasket) setY(getY() - getDY()); // TODO: not this

	if (!placeFree(getX() + getDX(), getY(), objects) || getX() <= 0.0f)
	{
		// Turn around
		setDX(-getDX());
		sprite.setScale(sf::Vector2f(2.0f * getDir(), 2.0f));
		turning = true;
		turnTimer.restart();
	}

	// Timers
	if (turning && turnTimer.getElapsedTime().asSeconds() >= 0.1) turning = false;
	if (inCasket && casketTimer.getElapsedTime().asSeconds() >= 0.7) inCasket = false;

	// Animations
	if (inCasket) setAnimation("casket");
	else if (placeFree(getX(), getY() + 1, objects) && getDX() == 0.0f) setAnimation("fall");
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
