#include "sprite_animation.h"

SpriteAnimation::SpriteAnimation(Room const &room, float x, float y, int dir, sf::Texture const &texture, std::vector<sf::IntRect> animationFrames, float speed, Object const *replacedObject)
	: ModalAnimation(room), sprite(texture), animationFrames(animationFrames),
	  replacedObject(replacedObject)
{
	sprite.setScale(dir, 1);
	sprite.setPosition(x, y);
	animationSpeed = speed;
}

SpriteAnimation::~SpriteAnimation() {}

bool SpriteAnimation::shouldDraw(Object const *obj)
{
	return obj != replacedObject;
}

void SpriteAnimation::draw(sf::RenderWindow &window)
{
	auto frame = static_cast<size_t>(floor(currentFrame));
	if (frame >= animationFrames.size())
	{
		m_finished = true;
		currentFrame = fmod(currentFrame, animationFrames.size());
		frame = static_cast<size_t>(floor(currentFrame));
	}
	sprite.setTextureRect(animationFrames[frame]);
	window.draw(sprite);
}
