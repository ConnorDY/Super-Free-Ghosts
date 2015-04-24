
#ifndef FADE_IN_ANIMATION_H
#define FADE_IN_ANIMATION_H
#include "modal_animation.h"

class FadeInAnimation : public ModalAnimation
{
	private:
		float fadeInTime;
		sf::Color colour;
		sf::RectangleShape overlayRect;
	public:
		FadeInAnimation(float fadeInTime, sf::Color colour);
		virtual ~FadeInAnimation();

		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, settings_t const &settings);
};

#endif
