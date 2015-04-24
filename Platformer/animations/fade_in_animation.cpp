#include "fade_in_animation.h"
#include "room.h"

FadeInAnimation::FadeInAnimation(float time, sf::Color colour)
	: ModalAnimation(),
	  fadeInTime(time), colour(colour),
	  overlayRect(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT))
{
	animationSpeed = 1;
}

FadeInAnimation::~FadeInAnimation() {}

void FadeInAnimation::draw(sf::RenderWindow &window)
{
	window.draw(overlayRect);
}

void FadeInAnimation::update(sf::Time deltaTime, Room &room, settings_t const &settings)
{
	ModalAnimation::update(deltaTime, room, settings);
	overlayRect.setPosition(room.getViewX(), room.getViewY());
	auto alpha = static_cast<int>(floor(colour.a * std::max<float>(0, 1 - currentFrame / fadeInTime)));
	overlayRect.setFillColor(sf::Color(colour.r, colour.g, colour.b, alpha));
	if (currentFrame >= fadeInTime) m_finished = true;
}
