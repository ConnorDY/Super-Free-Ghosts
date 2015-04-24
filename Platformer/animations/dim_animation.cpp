#include "dim_animation.h"
#include "room.h"

DimAnimation::DimAnimation(float time, sf::Color colour)
	: ModalAnimation(),
	  fadeInTime(time), colour(colour),
	  overlayRect(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT))
{
	animationSpeed = 1;
}

DimAnimation::~DimAnimation() {}

void DimAnimation::draw(sf::RenderWindow &window)
{
	window.draw(overlayRect);
}

void DimAnimation::update(sf::Time deltaTime, Room &room, settings_t const &settings)
{
	ModalAnimation::update(deltaTime, room, settings);
	overlayRect.setPosition(room.getViewX(), room.getViewY());
	auto alpha = static_cast<int>(floor(static_cast<float>(colour.a) * std::min<float>(1.0f, currentFrame / fadeInTime)));
	overlayRect.setFillColor(sf::Color(colour.r, colour.g, colour.b, alpha));
	if (currentFrame >= fadeInTime) m_finished = true;
}
