#ifndef FLASH_ANIMATION_H
#define FLASH_ANIMATION_H
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "modal_animation.h"

class FlashAnimation : public ModalAnimation
{
	private:
		std::vector<int> flashes;
		sf::RectangleShape overlayRect;
	public:
		FlashAnimation(std::vector<int> flashes, float speed, sf::Color colour);
		virtual ~FlashAnimation();

		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, settings_t const &settings);
};

#endif
