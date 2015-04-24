#include "flash_animation.h"
#include "room.h"

FlashAnimation::FlashAnimation(std::vector<int> flashes, float speed, sf::Color colour)
	: ModalAnimation(), flashes(flashes),
	  overlayRect(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT))
{
	overlayRect.setFillColor(colour);
	animationSpeed = speed;
}

FlashAnimation::~FlashAnimation() {}

void FlashAnimation::draw(sf::RenderWindow &window)
{
	if (flashes[0] == floor(currentFrame))
	{
		// We are on a flash frame, draw a flash!
		window.draw(overlayRect);
	}
}

void FlashAnimation::update(sf::Time deltaTime, Room &room, settings_t const &settings)
{
	ModalAnimation::update(deltaTime, room, settings);
	if (flashes.empty())
	{
		m_finished = true;
		return;
	}

	while (!flashes.empty() && flashes[0] < floor(currentFrame))
		flashes.erase(flashes.begin());

	overlayRect.setPosition(room.getViewX(), room.getViewY());
}
