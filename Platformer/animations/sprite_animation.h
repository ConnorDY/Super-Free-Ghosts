#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H
#include "modal_animation.h"

class SpriteAnimation : public ModalAnimation
{
	private:
		sf::Sprite sprite;
		std::vector<sf::IntRect> animationFrames;
		Object const *replacedObject;
	public:
		SpriteAnimation(Room const &room, float x, float y, int dir, sf::Texture const &texture, std::vector<sf::IntRect> animationFrames, float speed, Object const *replacedObject);
		virtual ~SpriteAnimation();

		virtual bool shouldDraw(Object const* obj) override;

		virtual void update(sf::Time deltaTime) override;
		virtual void draw(sf::RenderWindow &window) override;
};

#endif
