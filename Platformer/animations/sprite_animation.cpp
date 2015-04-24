#include "sprite_animation.h"

SpriteAnimation::SpriteAnimation(float x, float y, sf::Texture const &texture, std::vector<sf::IntRect> animationFrames, float speed, Object const *replacedObject)
	: ModalAnimation(), sprite(texture), animationFrames(animationFrames),
	  replacedObject(replacedObject)
{
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
