#ifndef DIM_ANIMATION_H
#define DIM_ANIMATION_H
#include "modal_animation.h"

class DimAnimation : public ModalAnimation
{
	private:
		float fadeInTime;
		sf::Color colour;
		sf::RectangleShape overlayRect;
	public:
		DimAnimation(Room const &room, float fadeInTime, sf::Color colour);
		virtual ~DimAnimation();

		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime);
};

#endif
