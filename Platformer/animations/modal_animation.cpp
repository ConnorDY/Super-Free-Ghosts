#include "modal_animation.h"

ModalAnimation::ModalAnimation()
	: m_finished(false), currentFrame(0), animationSpeed(2)
{}

ModalAnimation::~ModalAnimation() {}

bool ModalAnimation::finished() { return m_finished; }
bool ModalAnimation::shouldDraw(Object const*) { return true; }

void ModalAnimation::update(sf::Time deltaTime, Room &, settings_t const &)
{
	currentFrame += animationSpeed * deltaTime.asSeconds();
}
