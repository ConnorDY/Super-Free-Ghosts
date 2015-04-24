#include "modal_animation.h"

ModalAnimation::ModalAnimation(Room const &room)
	: m_finished(false), currentFrame(0), animationSpeed(2), room(room)
{}

ModalAnimation::~ModalAnimation() {}

bool ModalAnimation::finished() { return m_finished; }
bool ModalAnimation::shouldDraw(Object const*) { return true; }

void ModalAnimation::update(sf::Time deltaTime)
{
	currentFrame += animationSpeed * deltaTime.asSeconds();
}
