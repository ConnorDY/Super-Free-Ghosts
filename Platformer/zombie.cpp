#include "zombie.h"

#define ZOMBIE_WIDTH  46
#define ZOMBIE_HEIGHT 80
Zombie::Zombie(TextureManager &textureManager, float x, float y)
	: Object(
	Object::Type::Zombie,
	x, y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, // x, y, w, h
	0.0f, 0.0f,     // dx, dy
	true,           // solid
	0.00235f,       // Gravity
	0.5f            // Fall speed
	),
	rectangle(sf::Vector2f((ZOMBIE_WIDTH / 2.0f) - 8.0f, ZOMBIE_HEIGHT / 2.0f)),
	animation("casket"),
	moveSpeed(0.22f), frame(0.0f),
	inCasket(false)
{
	// Box
	rectangle.setOrigin(11.0f, 40.0f);
	rectangle.setScale(sf::Vector2f(2.0f, 2.0f));

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
}


/* Accessors */
int Zombie::getDir() const
{
	int dir = 1;

	if (sprite.getScale().x > 0.0f) dir = -1;

	return dir;
}

sf::FloatRect Zombie::getRect()
{
	return sf::FloatRect(getX() - (sprite.getOrigin().x * fabs(sprite.getScale().x)), getY() - (sprite.getOrigin().y * fabs(sprite.getScale().y)), getWidth(), getHeight());
}

bool Zombie::placeFree(float x, float y, std::vector<Object*> const objects) const
{
	sf::FloatRect temp_rect(x - (sprite.getOrigin().x * fabs(sprite.getScale().x)), y - (sprite.getOrigin().y * fabs(sprite.getScale().y)), getWidth(), getHeight());

	return std::none_of(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return elem->isSolid() && temp_rect.intersects(elem->getRect());
	});
}


/* Actions */
void Zombie::draw(sf::RenderWindow &window)
{
	rectangle.setPosition(sf::Vector2f(getX(), getY()));
	window.draw(rectangle);

	sprite.setPosition(sf::Vector2f(getX(), getY()));
	window.draw(sprite);
}

void Zombie::update(sf::Time deltaTime, std::vector<Object*> const objects)
{
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
		float speed = 60.0f / 5.2f;

		if (frame < (float)frames) frame += deltaTime.asSeconds() * speed;

		if (frame >= (float)frames) frame = 0.0f; // Reset animation
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}
